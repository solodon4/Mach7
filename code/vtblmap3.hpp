///
/// \file vtblmap3.hpp
///
/// This file defines class vtblmap<T> used for fast mapping of vtbl pointers 
/// to type T.
///
/// \autor Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of the XTL framework (http://parasol.tamu.edu/xtl/).
/// Copyright (C) 2005-2011 Texas A&M University.
/// All rights reserved.
///

#pragma once

#include <cmath>
#include <cstdint>
#include <cstring>
#include <unordered_map>
#include "ptrtools.hpp"  // Helper functions to work with pointers
#include "config.hpp"    // Various compiler/platform dependent macros

#if defined(XTL_DUMP_PERFORMANCE)
// For print out purposes only
#include <algorithm>
#include <bitset>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#endif

const int initial_collisions_before_update = 1;
const int renewed_collisions_before_update = 16;

template <typename T> size_t get_frequency(intptr_t);

// --------------------[ Collisions Management ]--------------------
// Definition: Collision is condition in which cache cell for a
//             given vtbl is occupied by a different vtbl.
// Rules:
// - We only try to rearrange cache when actual collision happens
// - There is no point in rearranging if table.size() hasn't changed
// - Counting conflicts without changes in table size may decrease 
//   the counter to overflow in repetitive scenario.
// - There is always M such that growing cache to size 2^M will have
//   zero collisions. Such M is often too large to justify growth.
// - We only try log sizes N and N+1, where N = log2(|table|)
// - We do not know apriory if the set of all vtbls that will come 
//   through this vtblmap can have 0 collisions for log size N, N+1
// - Cache arrangement with no conflicts can be a good indicator that
//   such arrangement will still be possible after adding one more vtbl.
// - Cache arrangement with conflicts will have them with more vtbls 
//   added unless the cache size got increased.
// - In most cases we'd want at least one rearrangement to happen as
//   default value for irrelevant bits is usually irrelevant for a
//   given vtblmap and has to be recomputed.
// - We also have to avoid rearranging after every vtbl added as
//   for some especially large tables conflicts with log sizes N and
//   N+1 are unavoidable.
// - Once we found offset/size combination that renders no conflicts
//   there is no point in computing entropy further.
// - Addition of extra vtbl can make probability of conflict smaller
//   since total number of vtbls increased and we divide by it.
// - Addition of extra vtbl can make probability of conflict larger
//   when that vtbl conflicts with some that was not in conflict before.
//------------------------------------------------------------------------------

/// Natural logarithm of 2 needed for conversion into log base 2.
const double ln2 = 0.69314718055994528622676398299518041312694549560546875;

//------------------------------------------------------------------------------

template <typename T>
class vtblmap;

//------------------------------------------------------------------------------

template <typename T>
class vtblmap<T&>
{
private:

    enum { local_cache_bits = VTBL_DEFAULT_CACHE_BITS, local_cache_size = 1 << local_cache_bits };

#ifdef XTL_USE_VTBL_FREQUENCY
    struct stored_type
    {
        stored_type() : value(), hits(0) {}
        operator const T&() const { return value; }
        operator       T&()       { return value; }
        T      value;
        size_t hits;
    };
#else
    typedef T stored_type;
#endif

#ifdef _MSC_VER
   	/// Hash functor for vtbl.
    /// MSVC uses by default a complicated hash function on all integral types,
    /// but for our application to v-tables identity works best.
    struct vtbl_hasher
    {
	    typedef intptr_t argument_type;
	    typedef size_t   result_type;

        /// hash key to size_t value by pseudorandomizing transform
        size_t operator()(const intptr_t key) const { return key; }
    };

    /// A map from vtbl to T
    typedef std::unordered_map<intptr_t, stored_type, vtbl_hasher> vtbl_to_t_map;
#else
    /// A map from vtbl to T
    typedef std::unordered_map<intptr_t, stored_type>              vtbl_to_t_map;
#endif

    typedef typename vtbl_to_t_map::iterator    iterator;
    typedef typename vtbl_to_t_map::value_type  value_type;

public:
    
#if defined(XTL_DUMP_PERFORMANCE)
    vtblmap(const char* fl, size_t ln, const vtbl_count_t expected_size = min_expected_size) : file(fl), line(ln), updates(0),
        cache_mask((1<<std::min(max_log_size,bit_offset_t(req_bits(expected_size-1))))-1),
        cache(cache_mask < local_cache_size ? local_cache : new cache_entry[cache_mask+1]),
        optimal_shift(VTBL_IRRELEVANT_BITS),
        table(expected_size),
        last_table_size(0),
        collisions_before_update(initial_collisions_before_update)
    {
        std::memset(cache,0,(cache_mask+1)*sizeof(cache_entry)); // Reset cache
    }
#endif
    vtblmap(const vtbl_count_t expected_size = min_expected_size) : XTL_DUMP_PERFORMANCE_ONLY(file("unspecified"), line(0), updates(0),)
        cache_mask((1<<std::min(max_log_size,bit_offset_t(req_bits(expected_size-1))))-1),
        cache(cache_mask < local_cache_size ? local_cache : new cache_entry[cache_mask+1]),
        optimal_shift(VTBL_IRRELEVANT_BITS),
        table(expected_size),
        last_table_size(0),
        collisions_before_update(initial_collisions_before_update)
    {
        //std::clog << "expected_size=" << expected_size << std::endl;
        std::memset(cache,0,(cache_mask+1)*sizeof(cache_entry)); // Reset cache
    }

   ~vtblmap()
    {
        if (cache_mask > local_cache_size) delete[] cache; 
        XTL_DUMP_PERFORMANCE_ONLY(std::clog << *this << std::endl); 
    }

    typedef typename vtbl_to_t_map::mapped_type mapped_type;

    /// Structure describing entry in the cache
    struct cache_entry
    {
        intptr_t     vtbl;   ///< vtbl for which value has been computed
        stored_type* ptr;    ///< pointer to actual value in the table!
    };

    /// This is the main function to get the value of type T associated with
    /// the vtbl of a given pointer.
    ///
    /// \note The function returns the value "by reference" to indicate that you 
    ///       may take address or change the value of the cell!
    /// \note The important bit on saving pointers to elements of the hash table
    ///       comes from ISO C++0x standard draft from 2010-03-26 
    ///       section [unord.req], paragraphs 13:
    /// 
    /// 13. The insert members shall not affect the validity of references to 
    /// container elements, but may invalidate all iterators to the container. 
    /// The erase members shall invalidate only iterators and references to 
    /// the erased elements. 
    inline T& get(const void* p) throw()
    {
        const intptr_t vtbl = *reinterpret_cast<const intptr_t*>(p);
        cache_entry&   ce   = cache[(vtbl>>optimal_shift) & cache_mask];

        XTL_ASSERT(vtbl); // Since this represents VTBL pointer it cannot be null

        if (XTL_UNLIKELY(ce.vtbl != vtbl))
        {
            const iterator q = table.find(vtbl);

            if (q != table.end())
                ce.ptr = &q->second;
            else
            {
                // If this is an actual collision, we rearrange cache
                if (ce.vtbl && table.size() != last_table_size && !--collisions_before_update)
                    return update(vtbl); // try to rearrange cache
                else
                    ce.ptr = &table.insert(value_type(vtbl,stored_type())).first->second;
            }

            //if (XTL_LIKELY(ce.vtbl))
            //{
            //    const iterator q = table.find(vtbl);

            //    if (q != table.end())
            //        ce.ptr = &q->second;
            //    else
            //    {
            //        // If this is an actual collision, we rearrange cache
            //        if (table.size() != last_table_size && !--collisions_before_update)
            //            return update(vtbl); // try to rearrange cache
            //        else
            //            ce.ptr = &table.insert(value_type(vtbl,stored_type())).first->second;
            //    }
            //}
            //else
            //    ce.ptr = &table.insert(value_type(vtbl,stored_type())).first->second;

            //-----------------------------------------------
            //if (ce.vtbl && table.size() != last_table_size && !--collisions_before_update)
            //    return update(vtbl); // try to rearrange cache

            //ce.ptr = &table[vtbl];
            //-----------------------------------------------
            //if (XTL_LIKELY(ce.vtbl))
            //{
            //    const iterator q = table.find(vtbl);

            //    if (XTL_LIKELY(q != table.end()))
            //    {
            //        size_t sz = table.size();

            //        if (++collisions > sz && sz != last_table_size)
            //            return update(vtbl);   // try to rearrange cache
            //        else
            //            ce.ptr = &q->second;
            //    }
            //    else
            //        ce.ptr = &table[vtbl];
            //}
            //else
            //    ce.ptr = &table[vtbl];
            //-----------------------------------------------
            //size_t sz = table.size();

            //if (++collisions > sz && sz != last_table_size)
            //    return update(vtbl);   // try to rearrange cache
            //else
            //    ce.ptr = &table[vtbl];
            //-----------------------------------------------
            //if (XTL_LIKELY(ce.vtbl))
            //{
            //    const iterator q = table.find(vtbl);

            //    if (XTL_LIKELY(q != table.end()))
            //        ce.ptr = &q->second;
            //    else
            //        return update(vtbl);   // try to rearrange cache
            //}
            //else
            //    ce.ptr = &table[vtbl];

            ce.vtbl = vtbl;
        }

        XTL_USE_VTBL_FREQUENCY_ONLY(++ce.ptr->hits);
        return *ce.ptr;
    }

    T& update(intptr_t vtbl)
    {
        XTL_ASSERT(last_table_size  < table.size()); // We will only call this if size changed
        //XTL_ASSERT(table.find(vtbl) == table.end()); // This is guaranteed by the caller

        intptr_t diff = 0;
        intptr_t prev = vtbl;

        for (typename vtbl_to_t_map::const_iterator p = table.begin(); p != table.end(); ++p)
        {
            intptr_t vtbl = p->first;
            diff |= prev ^ vtbl;
            prev = vtbl;
        }

        stored_type& result = table[vtbl];

        XTL_DUMP_PERFORMANCE_ONLY(++updates); // Record update
        last_table_size = table.size();       // Update memoized value
        collisions_before_update = renewed_collisions_before_update;         // Reset collisions counter

        if (true/*diff != m_different*/)
        {
            bit_offset_t k  = req_bits(cache_mask);     // current log_size
            bit_offset_t n  = req_bits(table.size()-1); // needed  log_size
            bit_offset_t m  = req_bits(diff);           // highest bit in which vtbls differ
            bit_offset_t z  = trailing_zeros(static_cast<unsigned int>(diff)); // amount of lowest bits in which vtbls do not differ
            bit_offset_t l1 = std::min(max_log_size,std::max(k,n));
            bit_offset_t l2 = std::min(max_log_size,std::max(k,bit_offset_t(n+1)));
            bit_offset_t no = l1;
            bit_offset_t zo = z;
            double e_max = 0.0;

            for (bit_offset_t i = l1; i <= l2; ++i)
            {
                for (bit_offset_t j = z; j <= m-i; ++j)
                {
                    double e,c;
                    size_t t = get_stats_for(i,j,e,c);

                    XTL_ASSERT(t == last_table_size ? e >= e_max : true); // t == table.size() means there is no conflict, thus entropy should be largest
                    XTL_ASSERT(c > 0.0 ? t < last_table_size : true);     // When probability of conflict is not 0, there should be collisions

                    if (e > e_max)
                    {
                        e_max = e;
                        no = i;
                        zo = j;
                    }

                    if (t == last_table_size)
                    {
                        // We found size and offset without conflicts, exit both loops
                        i = l2+1; // to exit both for loops
                        collisions_before_update = initial_collisions_before_update; // since it is likely that adding another vtbl may still render no conflicts
                        break;
                    }
                }
            }

            if (no > k)
            {
                if (cache_mask > local_cache_size) delete[] cache; 
                cache_mask = (1<<no)-1;
                cache = cache_mask < local_cache_size ? local_cache : new cache_entry[1<<no];
            }
            else
                no = k;

            std::memset(cache,0,(1<<no)*sizeof(cache_entry)); // Reset cache
            optimal_shift = zo;

            for (typename vtbl_to_t_map::iterator p = table.begin(); p != table.end(); ++p)
            {
                intptr_t vtbl = p->first;
                cache_entry& ce = cache[(vtbl>>optimal_shift) & cache_mask];
                ce.vtbl = vtbl;
                ce.ptr  = &p->second;
            }
        }
//#if defined(XTL_DUMP_PERFORMANCE)
//        std::clog << "Vtbl:New" << std::bitset<8*sizeof(intptr_t)>((unsigned long long)vtbl) << " -> " << ((vtbl >> optimal_shift) & cache_mask) << '\t' << vtbl_typeid(vtbl).name() << std::endl;
//        *this >> std::clog;       
//#endif
        XTL_USE_VTBL_FREQUENCY_ONLY(++result.hits);
        return result;
    }

    /// Compute entropy and probability of collision for all vtbls currently 
    /// present in the table when they are mapped into cache of size 2^log_size 
    /// with given offset used to ignore irrelevant bits.
    size_t get_stats_for(bit_offset_t log_size, bit_offset_t offset, double& entropy, double& conflict) const
    {
        const size_t   cache_size = 1<<log_size;
        const intptr_t cache_mask = cache_size-1;

        XTL_ASSERT(log_size <= max_log_size); 
        XTL_ASSERT(req_bits(table.size()) < sizeof(vtbl_count_t)*8); // Make sure vtbl_count_t is sufficiently large

        // We do this to not resort to vectors and heap and keep counting on stack
        XTL_VLAZ(histogram, vtbl_count_t, cache_size, 1<<max_log_size); // vtbl_count_t histogram[cache_size];

        for (typename vtbl_to_t_map::const_iterator p = table.begin(); p != table.end(); ++p)
            histogram[(p->first >> offset) & cache_mask]++;

        double total = double(table.size());
        size_t entries = 0;

        entropy  = 0.0;
        conflict = 0.0;

        for (size_t j = 0; j < cache_size; ++j)
        {
            if (histogram[j])
            {
                double pi = histogram[j]/total;
                entropy -= pi*std::log(pi)/ln2;
                ++entries;
            }

            if (histogram[j] > 1)
                conflict += (histogram[j]-1)/total;
        }
#if defined(XTL_DUMP_PERFORMANCE)
        //std::clog <<    "ENTROPY: "  << entropy 
        //          << "\t CONFLICT: " << conflict 
        //          << "\t entries="   << entries 
        //          << "\t log_size="  << (int)log_size 
        //          << "\t offset="    << (int)offset 
        //          << '[' << line << ']' << std::endl;
#endif
        return entries;
    }

#if defined(XTL_DUMP_PERFORMANCE)
    std::ostream& operator>>(std::ostream& os) const
    {
        os << file << '[' << line << ']' << std::endl;

        intptr_t diff = 0;
        intptr_t prev = 0;
        size_t   log_size  = req_bits(cache_mask);
        size_t   cache_size = (1<<log_size);

        // We do this to not resort to vectors and heap and keep counting on stack
        XTL_VLAZ(histogram, vtbl_count_t, cache_size, 1<<max_log_size); // vtbl_count_t histogram[cache_size];
        std::vector<intptr_t> vtbls(table.size());
        std::vector<intptr_t>::iterator q = vtbls.begin();

        for (typename vtbl_to_t_map::const_iterator p = table.begin(); p != table.end(); ++p)
        {
            intptr_t vtbl = *q++ = p->first;

            if (prev)
                diff |= prev ^ vtbl; // Update diff with information about which bits in all vtbls differ

            prev = vtbl;
            histogram[(vtbl >> optimal_shift) & cache_mask]++;
        }

        // Sort vtables to output them is address order
        std::sort(vtbls.begin(),vtbls.end());

        for (q = vtbls.begin(); q != vtbls.end(); ++q)
        {
            intptr_t vtbl = *q;
            os << "Vtbl:   " << std::bitset<8*sizeof(intptr_t)>((unsigned long long)vtbl) 
               << " -> " << std::setw(3) << ((vtbl >> optimal_shift) & cache_mask) 
               << '\t';
            XTL_USE_VTBL_FREQUENCY_ONLY(os << ' ' << table.find(vtbl)->second.hits << " \t");
            if (histogram[(vtbl >> optimal_shift) & cache_mask] > 1)
                os << '[' << histogram[(vtbl >> optimal_shift) & cache_mask] << ']';
            else
                os << "   ";

            os << '\t' << vtbl_typeid(vtbl).name() << std::endl;        
        }

        // FIX: G++ crashes when we use std::stringstream here, so we have to workaround it manually
        std::string str(8*sizeof(prev),'0');

        for(size_t j = 1, i = 8*sizeof(prev); i; --i, j<<=1)
            if (diff & j)
                str[i-1] = 'X';
            else
            if (prev & j)
                str[i-1] = '1';

        double entropy;
        double conflict;
        size_t entries = get_stats_for(log_size, optimal_shift, entropy, conflict);

        os << "VTBLS:  "     << str
           << " total="      << table.size()
           << " log_size="   << log_size
           << " shift="      << optimal_shift
           << " width="      << str.find_last_of("X")-str.find_first_of("X")+1 
           XTL_DUMP_PERFORMANCE_ONLY(<< " updates=" << updates)
           << " entries: "   << entries
           << " Entropy: "   << entropy
           << " Conflict: "  << conflict << "\t ";

        bool show = false;

        for (size_t i = table.size(); i != ~0; --i)
        {
            size_t n = std::count(histogram,histogram+cache_size,i);

            if (show = show || n > 0)
                os << i << "->" << n << "; ";
        }

        os << std::count(histogram,histogram+cache_size,0)*100/cache_size << "% unused " << '[' << line << ']' << std::endl;

        bit_offset_t k  = req_bits(cache_mask);     // current log_size
        bit_offset_t n  = req_bits(table.size()-1); // needed  log_size
        bit_offset_t m  = req_bits(diff);           // highest bit in which vtbls differ
        bit_offset_t z  = trailing_zeros(static_cast<unsigned int>(diff)); // amount of lowest bits in which vtbls do not differ
        bit_offset_t l1 = std::min(max_log_size,std::min(k,n));
        bit_offset_t l2 = std::min(max_log_size,std::max(k,bit_offset_t(n+1)));

        for (bit_offset_t i = l1; i <= l2; ++i)
        {
            for (bit_offset_t j = z; j <= m-i; ++j)
            {
                double e, p;
                size_t t = get_stats_for(i,j,e,p);
                os << "\tlog_size=" << int(i) << " shift=" << int(j) << " Entropy=" << e << " Conflict=" << p << (t == table.size() ? " \t*" : "") << std::endl;
            }
        }

        return os/* << std::endl*/;
    }
    friend std::ostream& operator<<(std::ostream& os, const vtblmap& m) { return m >> os; }
#endif
private:

    /// Cache mask to access entries. Always cache_size-1 since cache_size is a power of 2
    /// \note We currently rely in constructors on this member be first in 
    ///       declaration order so that it is initialized first!
    size_t cache_mask;

    /// Cached mappings of vtbl to some indecies
    cache_entry* cache;

    /// Optimal shift computed based on the vtbl pointers already in the map.
    /// Most of the time this value would be equal to @irrelevant_bits, but not
    /// necessarily always. In case of collisions, optimal_shift will have
    /// a value of a shift that maximizes entropy of caching vtbl pointers (which 
    /// effectively also minimizes probability of not finding something in cache)
    size_t optimal_shift;

    /// Actual mapping of vtbl pointers (in reality keys obtained from them) 
    /// to values of type T. Values in this table are cached in @cache.
    vtbl_to_t_map table;

    /// Memoized table.size() during last cache rearranging
    size_t last_table_size;

    /// Number of colisions that we will still tolerate before next update
    int collisions_before_update;

#if defined(XTL_DUMP_PERFORMANCE)
    const char* file;    ///< File in which this vtblmap is instantiated
    size_t      line;    ///< Line in the file where it is instantiated
    size_t      updates; ///< Amount of reconfigurations performed at run time
#endif

    /// An on board memory for cache for log sizes smaller than default.
    cache_entry local_cache[local_cache_size];

};

//------------------------------------------------------------------------------

/// Same as @vtblmap<T> but takes selector type S as an argument
template <typename S, typename T>
class vtblmap_of;

//------------------------------------------------------------------------------

/// Same as @vtblmap<T&> but takes selector type S as an argument
template <typename S, typename T>
class vtblmap_of<S,T&>
{
private:

    static_assert(std::is_polymorphic<S>::value, "Type parameter S has to be polymorphic");

    enum { local_cache_bits = VTBL_DEFAULT_CACHE_BITS, local_cache_size = 1 << local_cache_bits };

#ifdef XTL_USE_VTBL_FREQUENCY
    struct stored_type
    {
        stored_type() : value(), hits(0) {}
        operator const T&() const { return value; }
        operator       T&()       { return value; }
        T      value;
        size_t hits;
    };
#else
    typedef T stored_type;
#endif

#ifdef _MSC_VER
   	/// Hash functor for vtbl.
    /// MSVC uses by default a complicated hash function on all integral types,
    /// but for our application to v-tables identity works best.
    struct vtbl_hasher
    {
	    typedef intptr_t argument_type;
	    typedef size_t   result_type;

        /// hash key to size_t value by pseudorandomizing transform
        size_t operator()(const intptr_t key) const { return key; }
    };

    /// A map from vtbl to T
    typedef std::unordered_map<intptr_t, stored_type, vtbl_hasher> vtbl_to_t_map;
#else
    /// A map from vtbl to T
    typedef std::unordered_map<intptr_t, stored_type>              vtbl_to_t_map;
#endif

    typedef typename vtbl_to_t_map::iterator    iterator;
    typedef typename vtbl_to_t_map::value_type  value_type;

public:
    
#if defined(XTL_DUMP_PERFORMANCE)
    vtblmap_of(const char* fl, size_t ln, const vtbl_count_t expected_size = min_expected_size) : 
        file(fl), 
        line(ln), 
        updates(0), 
        clauses(expected_size),
        conflicts(0),
        cache_mask((1<<std::min(max_log_size,bit_offset_t(req_bits(expected_size-1))))-1),
        cache(cache_mask < local_cache_size ? local_cache : new cache_entry[cache_mask+1]),
        optimal_shift(VTBL_IRRELEVANT_BITS),
        table(expected_size),
        last_table_size(0),
        collisions_before_update(initial_collisions_before_update)
    {
        std::memset(cache,0,(cache_mask+1)*sizeof(cache_entry)); // Reset cache
    }
#endif
    vtblmap_of(const vtbl_count_t expected_size = min_expected_size) : XTL_DUMP_PERFORMANCE_ONLY(file("unspecified"), line(0), updates(0), clauses(expected_size), conflicts(0),)
        cache_mask((1<<std::min(max_log_size,bit_offset_t(req_bits(expected_size-1))))-1),
        cache(cache_mask < local_cache_size ? local_cache : new cache_entry[cache_mask+1]),
        optimal_shift(VTBL_IRRELEVANT_BITS),
        table(expected_size),
        last_table_size(0),
        collisions_before_update(initial_collisions_before_update)
    {
        std::memset(cache,0,(cache_mask+1)*sizeof(cache_entry)); // Reset cache
    }

   ~vtblmap_of()
    {
        if (cache_mask > local_cache_size) delete[] cache; 
        XTL_DUMP_PERFORMANCE_ONLY(std::clog << *this << std::endl); 
    }

    typedef typename vtbl_to_t_map::mapped_type mapped_type;

    /// Structure describing entry in the cache
    struct cache_entry
    {
        intptr_t     vtbl;   ///< vtbl for which value has been computed
        stored_type* ptr;    ///< pointer to actual value in the table!
    };

    /// This is the main function to get the value of type T associated with
    /// the vtbl of a given pointer.
    ///
    /// \note The function returns the value "by reference" to indicate that you 
    ///       may take address or change the value of the cell!
    /// \note The important bit on saving pointers to elements of the hash table
    ///       comes from ISO C++0x standard draft from 2010-03-26 
    ///       section [unord.req], paragraphs 13:
    /// 
    /// 13. The insert members shall not affect the validity of references to 
    /// container elements, but may invalidate all iterators to the container. 
    /// The erase members shall invalidate only iterators and references to 
    /// the erased elements. 
    inline T& get(const S* p) throw()
    {
        const intptr_t vtbl = *reinterpret_cast<const intptr_t*>(p);
        cache_entry&   ce   = cache[(vtbl>>optimal_shift) & cache_mask];

        XTL_ASSERT(vtbl); // Since this represents VTBL pointer it cannot be null

        if (XTL_UNLIKELY(ce.vtbl != vtbl))
        {
            //XTL_DUMP_PERFORMANCE_ONLY(if (ce.vtbl) ++conflicts);
            //const iterator q = table.find(vtbl);

            //if (q != table.end())
            //    ce.ptr = &q->second;
            //else
            //{
            //    // If this is an actual collision, we rearrange cache
            //    if (ce.vtbl && !--collisions_before_update && table.size() != last_table_size)
            //        return update(vtbl); // try to rearrange cache
            //    else
            //        ce.ptr = &table.insert(value_type(vtbl,stored_type())).first->second;
            //}

            //-----------------------------------------------
            //if (XTL_LIKELY(ce.vtbl))
            //{
            //    XTL_DUMP_PERFORMANCE_ONLY(++conflicts);
            //    const iterator q = table.find(vtbl);

            //    if (q != table.end())
            //        ce.ptr = &q->second;
            //    else
            //    {
            //        // If this is an actual collision, we rearrange cache
            //        if (table.size() != last_table_size && !--collisions_before_update)
            //            return update(vtbl); // try to rearrange cache
            //        else
            //            ce.ptr = &table.insert(value_type(vtbl,stored_type())).first->second;
            //    }
            //}
            //else
            //    ce.ptr = &table.insert(value_type(vtbl,stored_type())).first->second;

            //-----------------------------------------------
            XTL_DUMP_PERFORMANCE_ONLY(if (ce.vtbl) ++conflicts);
            if (ce.vtbl && --collisions_before_update <= 0 && table.size() != last_table_size)
                return update(vtbl); // try to rearrange cache

            ce.ptr = &table[vtbl];
            //-----------------------------------------------
            //if (XTL_LIKELY(ce.vtbl))
            //{
            //    const iterator q = table.find(vtbl);

            //    if (XTL_LIKELY(q != table.end()))
            //    {
            //        size_t sz = table.size();

            //        if (++collisions > sz && sz != last_table_size)
            //            return update(vtbl);   // try to rearrange cache
            //        else
            //            ce.ptr = &q->second;
            //    }
            //    else
            //        ce.ptr = &table[vtbl];
            //}
            //else
            //    ce.ptr = &table[vtbl];
            //-----------------------------------------------
            //size_t sz = table.size();

            //if (++collisions > sz && sz != last_table_size)
            //    return update(vtbl);   // try to rearrange cache
            //else
            //    ce.ptr = &table[vtbl];
            //-----------------------------------------------
            //if (XTL_LIKELY(ce.vtbl))
            //{
            //    const iterator q = table.find(vtbl);

            //    if (XTL_LIKELY(q != table.end()))
            //        ce.ptr = &q->second;
            //    else
            //        return update(vtbl);   // try to rearrange cache
            //}
            //else
            //    ce.ptr = &table[vtbl];

            ce.vtbl = vtbl;
        }

        XTL_USE_VTBL_FREQUENCY_ONLY(++ce.ptr->hits);
        return *ce.ptr;
    }

    T& update(intptr_t vtbl)
    {
        XTL_ASSERT(last_table_size  < table.size()); // We will only call this if size changed
        //XTL_ASSERT(table.find(vtbl) == table.end()); // This is guaranteed by the caller

        intptr_t diff = 0;
        intptr_t prev = vtbl;

        for (typename vtbl_to_t_map::const_iterator p = table.begin(); p != table.end(); ++p)
        {
            intptr_t vtbl = p->first;
            diff |= prev ^ vtbl;
            prev = vtbl;
        }

        stored_type& result = table[vtbl];
        XTL_USE_VTBL_FREQUENCY_ONLY(++result.hits);

//#if defined(XTL_DUMP_PERFORMANCE)
//        std::clog << "\nVtbl:New" << std::bitset<8*sizeof(intptr_t)>((unsigned long long)vtbl) << " -> " << ((vtbl >> optimal_shift) & cache_mask) << '\t' << vtbl_typeid(vtbl).name() << std::endl;
//        *this >> std::clog;       
//#endif

        XTL_DUMP_PERFORMANCE_ONLY(++updates); // Record update
        last_table_size = table.size();       // Update memoized value
        collisions_before_update = renewed_collisions_before_update;         // Reset collisions counter

        if (true/*diff != m_different*/)
        {
            bit_offset_t k  = req_bits(cache_mask);     // current log_size
            bit_offset_t n  = req_bits(table.size()-1); // needed  log_size
            bit_offset_t m  = req_bits(diff);           // highest bit in which vtbls differ
            bit_offset_t z  = trailing_zeros(static_cast<unsigned int>(diff)); // amount of lowest bits in which vtbls do not differ
            bit_offset_t l1 = std::min(max_log_size,std::max(k,n));
            bit_offset_t l2 = std::min(max_log_size,std::max(k,bit_offset_t(n+1)));
            bit_offset_t no = l1;
            bit_offset_t zo = z;
            double e_max = 0.0;

            for (bit_offset_t i = l1; i <= l2; ++i)
            {
                for (bit_offset_t j = z; j <= m-i; ++j)
                {
                    double e,c;
                    size_t t = get_stats_for(i,j,e,c);

                    XTL_ASSERT(t == last_table_size ? e >= e_max : true); // t == table.size() means there is no conflict, thus entropy should be largest
                    XTL_ASSERT(c > 0.0 ? t < last_table_size : true);     // When probability of conflict is not 0, there should be collisions

                    if (e > e_max)
                    {
                        e_max = e;
                        no = i;
                        zo = j;
                    }

                    if (t == last_table_size)
                    {
                        // We found size and offset without conflicts, exit both loops
                        i = l2+1; // to exit both for loops
                        collisions_before_update = initial_collisions_before_update; // since it is likely that adding another vtbl may still render no conflicts
                        break;
                    }
                }
            }

            if (optimal_shift != zo || no > k)
            {
                if (no > k)
                {
                    if (cache_mask > local_cache_size) delete[] cache; 
                    cache_mask = (1<<no)-1;
                    cache = cache_mask < local_cache_size ? local_cache : new cache_entry[1<<no];
                }
                else
                    no = k;

                std::memset(cache,0,(1<<no)*sizeof(cache_entry)); // Reset cache
                optimal_shift = zo;

                for (typename vtbl_to_t_map::iterator p = table.begin(); p != table.end(); ++p)
                {
                    intptr_t vtbl = p->first;
                    cache_entry& ce = cache[(vtbl>>optimal_shift) & cache_mask];
                    ce.vtbl = vtbl;
                    ce.ptr  = &p->second;
                }
            }
        }

//#if defined(XTL_DUMP_PERFORMANCE)
//        std::clog << "After" << std::endl;
//        *this >> std::clog;       
//#endif
        return result;
    }

#if defined(XTL_USE_VTBL_FREQUENCY) || defined(XTL_DUMP_PERFORMANCE)
    /// Struct holding sum and a square sum of vtbl frequencies falling into a given bin.
    struct SQS 
    {
        size_t       sum;
        size_t       sqsum;
        vtbl_count_t count;
    };
#endif

#if defined(XTL_USE_VTBL_FREQUENCY)
    /// Compute entropy and probability of collision for all vtbls currently 
    /// present in the table when they are mapped into cache of size 2^log_size 
    /// with given offset used to ignore irrelevant bits.
    size_t get_stats_for(bit_offset_t log_size, bit_offset_t offset, double& entropy, double& conflict) const
    {
        const size_t   cache_size = 1<<log_size;
        const intptr_t cache_mask = cache_size-1;

        XTL_ASSERT(log_size <= max_log_size); 
        XTL_ASSERT(req_bits(table.size()) < sizeof(vtbl_count_t)*8); // Make sure vtbl_count_t is sufficiently large

        // We do this to not resort to vectors and heap and keep counting on stack
        XTL_VLAZ(histogram, SQS, cache_size, 1<<max_log_size); // SQS histogram[cache_size];
        size_t sum = 0;

        for (typename vtbl_to_t_map::const_iterator p = table.begin(); p != table.end(); ++p)
        {
            SQS&   sqs  = histogram[(p->first >> offset) & cache_mask];
            size_t hits = p->second.hits;
            sum       += hits;
            sqs.sum   += hits;
            sqs.sqsum += hits*hits;
            ++sqs.count;
        }

        size_t entries = 0;

        entropy  = 0.0;
        conflict = 0.0;

        for (size_t j = 0; j < cache_size; ++j)
        {
            SQS& sqs = histogram[j];

            if (sqs.sum)
            {
                double pi = double(sqs.sum)/sum;
                entropy -= pi*std::log(pi)/ln2;
                ++entries;

                if (sqs.count > 1)
                    conflict += pi*(1.0-double(sqs.sqsum)/(sqs.sum*sqs.sum));
            }
        }
//#if defined(XTL_DUMP_PERFORMANCE)
//        std::clog <<    "ENTROPY: "  << entropy 
//                  << "\t CONFLICT: " << conflict 
//                  << "\t entries="   << entries 
//                  << "\t log_size="  << (int)log_size 
//                  << "\t offset="    << (int)offset 
//                  << '[' << line << ']' << std::endl;
//#endif
        return entries;
    }
#else
    /// Compute entropy and probability of collision for all vtbls currently 
    /// present in the table when they are mapped into cache of size 2^log_size 
    /// with given offset used to ignore irrelevant bits.
    size_t get_stats_for(bit_offset_t log_size, bit_offset_t offset, double& entropy, double& conflict) const
    {
        const size_t   cache_size = 1<<log_size;
        const intptr_t cache_mask = cache_size-1;

        XTL_ASSERT(log_size <= max_log_size); 
        XTL_ASSERT(req_bits(table.size()) < sizeof(vtbl_count_t)*8); // Make sure vtbl_count_t is sufficiently large

        // We do this to not resort to vectors and heap and keep counting on stack
        XTL_VLAZ(histogram, vtbl_count_t, cache_size, 1<<max_log_size); // vtbl_count_t histogram[cache_size];

        for (typename vtbl_to_t_map::const_iterator p = table.begin(); p != table.end(); ++p)
            histogram[(p->first >> offset) & cache_mask]++;

        double total = double(table.size());
        size_t entries = 0;

        entropy  = 0.0;
        conflict = 0.0;

        for (size_t j = 0; j < cache_size; ++j)
        {
            if (histogram[j])
            {
                double pi = histogram[j]/total;
                entropy -= pi*std::log(pi)/ln2;
                ++entries;
            }

            if (histogram[j] > 1)
                conflict += (histogram[j]-1)/total;
        }
#if defined(XTL_DUMP_PERFORMANCE)
        //std::clog <<    "ENTROPY: "  << entropy 
        //          << "\t CONFLICT: " << conflict 
        //          << "\t entries="   << entries 
        //          << "\t log_size="  << (int)log_size 
        //          << "\t offset="    << (int)offset 
        //          << '[' << line << ']' << std::endl;
#endif
        return entries;
    }
#endif

#if defined(XTL_DUMP_PERFORMANCE)
    std::ostream& operator>>(std::ostream& os) const
    {
        os << file << '[' << line << ']' << std::endl;

        intptr_t diff = 0;
        intptr_t prev = 0;
        size_t   log_size  = req_bits(cache_mask);
        size_t   cache_size = (1<<log_size);

#if defined(XTL_USE_VTBL_FREQUENCY)
        // We do this to not resort to vectors and heap and keep counting on stack
        XTL_VLAZ(histogram, SQS, cache_size, 1<<max_log_size); // SQS histogram[cache_size];
#else
        // We do this to not resort to vectors and heap and keep counting on stack
        XTL_VLAZ(histogram, vtbl_count_t, cache_size, 1<<max_log_size); // vtbl_count_t histogram[cache_size];
#endif
        std::vector<intptr_t> vtbls(table.size());
        std::vector<intptr_t>::iterator q = vtbls.begin();

        size_t sum = 0;

        for (typename vtbl_to_t_map::const_iterator p = table.begin(); p != table.end(); ++p)
        {
            intptr_t vtbl = *q++ = p->first;

            if (prev)
                diff |= prev ^ vtbl; // Update diff with information about which bits in all vtbls differ

            prev = vtbl;

#if defined(XTL_USE_VTBL_FREQUENCY)
            SQS&   sqs  = histogram[(vtbl >> optimal_shift) & cache_mask];
            size_t hits = p->second.hits;
            sum       += hits;
            sqs.sum   += hits;
            sqs.sqsum += hits*hits;
            ++sqs.count;
#else
            histogram[(vtbl >> optimal_shift) & cache_mask]++;
#endif

        }

        // Sort vtables to output them is address order
        std::sort(vtbls.begin(),vtbls.end());

        for (q = vtbls.begin(); q != vtbls.end(); ++q)
        {
            intptr_t vtbl = *q;
            os << "Vtbl:   " << std::bitset<8*sizeof(intptr_t)>((unsigned long long)vtbl) 
               << " -> "     << std::setw(3) << ((vtbl >> optimal_shift) & cache_mask) 
               << ' ';

#if defined(XTL_USE_VTBL_FREQUENCY)
            SQS& sqs  = histogram[(vtbl >> optimal_shift) & cache_mask];

            if (sqs.count > 1)
                os << '['  << sqs.count << ']';
            else
                os << "   ";

            size_t hits = table.find(vtbl)->second.hits;

            os << " f=" << std::setw(5) << hits << " "
               << " p=" << std::setw(9) << std::fixed << std::setprecision(7) << double(hits)/sum
               << " s=" << std::setw(9) << std::fixed << std::setprecision(7) << double(hits*hits)/(sqs.sum*sqs.sum);
#else
            if (histogram[(vtbl >> optimal_shift) & cache_mask] > 1)
                os << '[' << histogram[(vtbl >> optimal_shift) & cache_mask] << ']';
            else
                os << "   ";
#endif

            os << '\t' << vtbl_typeid(vtbl).name() << std::endl;        
        }

        // FIX: G++ crashes when we use std::stringstream here, so we have to workaround it manually
        std::string str(8*sizeof(prev),'0');

        for(size_t j = 1, i = 8*sizeof(prev); i; --i, j<<=1)
            if (diff & j)
                str[i-1] = 'X';
            else
            if (prev & j)
                str[i-1] = '1';

        double entropy;
        double conflict;
        size_t entries = get_stats_for(log_size, optimal_shift, entropy, conflict);

        os << "VTBLS:  "     << str
           << " clauses="    << std::setw(3) << clauses
           << " total="      << std::setw(3) << table.size()
           << " log_size="   << log_size
           << " shift="      << std::setw(2) << optimal_shift
           << " width="      << std::setw(2) << str.find_last_of("X")-str.find_first_of("X")+1 
           << " updates="    << std::setw(2) << updates
           << " conflicts="  << std::setw(4) << conflicts
           << " entries: "   << std::setw(3) << entries
           XTL_USE_VTBL_FREQUENCY_ONLY(<< " sum=" << std::setw(5) << sum)
           << " Entropy: "   << std::setw(9) << std::fixed << std::setprecision(7) << entropy
           << " Conflict: "  << std::setw(9) << std::fixed << std::setprecision(7) << conflict
           XTL_USE_VTBL_FREQUENCY_ONLY(<< " confl/sum: " << std::setw(9) << std::fixed << std::setprecision(7) << double(conflicts)/sum) << "\t ";

#if defined(XTL_USE_VTBL_FREQUENCY)
        bool show = false;
        size_t  d = 0;

        for (size_t i = table.size(); i != ~0; --i)
        {
            d = 0;

            for (size_t j = 0; j < cache_size; ++j)
                if (histogram[j].count == i)
                    ++d;

            if (show = show || d > 0)
                os << i << "->" << d << "; ";
        }

        os << d*100/cache_size << "% unused " << '[' << line << ']' << std::endl;
#else
        bool show = false;

        for (size_t i = table.size(); i != ~0; --i)
        {
            size_t n = std::count(histogram,histogram+cache_size,i);

            if (show = show || n > 0)
                os << i << "->" << n << "; ";
        }

        os << std::count(histogram,histogram+cache_size,0)*100/cache_size << "% unused " << '[' << line << ']' << std::endl;
#endif
        bit_offset_t k  = req_bits(cache_mask);     // current log_size
        bit_offset_t n  = req_bits(table.size()-1); // needed  log_size
        bit_offset_t m  = req_bits(diff);           // highest bit in which vtbls differ
        bit_offset_t z  = trailing_zeros(static_cast<unsigned int>(diff)); // amount of lowest bits in which vtbls do not differ
        bit_offset_t l1 = std::min(max_log_size,std::min(k,n));
        bit_offset_t l2 = std::min(max_log_size,std::max(k,bit_offset_t(n+1)));

        for (bit_offset_t i = l1; i <= l2; ++i)
        {
            for (bit_offset_t j = z; j <= m-i; ++j)
            {
                double e, p;
                size_t t = get_stats_for(i,j,e,p);
                os << "\tlog_size=" << int(i) << " shift=" << std::setw(2) << int(j) << " Entropy=" << std::setw(9) << std::fixed << std::setprecision(7) << e << " Conflict=" << std::setw(9) << std::fixed << std::setprecision(7) << p << (t == table.size() ? " \t*" : "") << std::endl;
            }
        }

        return os/* << std::endl*/;
    }
    friend std::ostream& operator<<(std::ostream& os, const vtblmap_of& m) { return m >> os; }
#endif
private:

    /// Cache mask to access entries. Always cache_size-1 since cache_size is a power of 2
    /// \note We currently rely in constructors on this member be first in 
    ///       declaration order so that it is initialized first!
    size_t cache_mask;

    /// Cached mappings of vtbl to some indecies
    cache_entry* cache;

    /// Optimal shift computed based on the vtbl pointers already in the map.
    /// Most of the time this value would be equal to @irrelevant_bits, but not
    /// necessarily always. In case of collisions, optimal_shift will have
    /// a value of a shift that maximizes entropy of caching vtbl pointers (which 
    /// effectively also minimizes probability of not finding something in cache)
    size_t optimal_shift;

    /// Actual mapping of vtbl pointers (in reality keys obtained from them) 
    /// to values of type T. Values in this table are cached in @cache.
    vtbl_to_t_map table;

    /// Memoized table.size() during last cache rearranging
    size_t last_table_size;

    /// Number of colisions that we will still tolerate before next update
    int collisions_before_update;

#if defined(XTL_DUMP_PERFORMANCE)
    const char* file;      ///< File in which this vtblmap_of is instantiated
    size_t      line;      ///< Line in the file where it is instantiated
    size_t      updates;   ///< Amount of reconfigurations performed at run time
    size_t      clauses;   ///< Size of the table expected from the number of clauses
    size_t      conflicts; ///< The amount of conflicts we've encountered
#endif

    /// An on board memory for cache for log sizes smaller than default.
    cache_entry local_cache[local_cache_size];

};

//------------------------------------------------------------------------------

struct type_switch_info
{
    std::ptrdiff_t offset; ///< FIX: We assume here offsets within object can only be ints
    std::size_t    line;   ///< We can choose smaller type for line to give more space to offset
    //int offset; ///< FIX: We assume here offsets within object can only be ints
    //int    line;   ///< We can choose smaller type for line to give more space to offset
};

//------------------------------------------------------------------------------

/// This class is an alternative to static variables inside functions, allowing 
/// it to be preallocated and thus avoid if in the function body. Parameter N is
/// differentiating parameter that can be set to e.g. __LINE__ to make it "unique".
/// The disadvantage of using this class might be worse locality as the static 
/// variable inside this class, even though preallocated will most likely be 
/// elsewhere.
template <typename T, size_t N>
struct preallocated
{
    static T value;
};

template <typename T, size_t N>
T preallocated<T,N>::value;

//------------------------------------------------------------------------------
