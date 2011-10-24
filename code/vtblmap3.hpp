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
#include <string>
#include <vector>
#endif

//------------------------------------------------------------------------------

/// Type capable of representing bit offsets and bit counts in intptr_t
typedef unsigned char  bit_offset_t;
/// The smallest integral type capable of representing the amount N of different 
/// vtbl pointers in the program. Roughly N should be equal to some constant c
/// multiplied by the amount of different classes polymorphic classes in the 
/// program. Constant c accounts for potential multiple inheritance.
typedef unsigned short vtbl_count_t;

//------------------------------------------------------------------------------

const bit_offset_t min_log_size = 3;  // FIX: Make this configurable
const bit_offset_t max_log_size = 16; // FIX: Make this configurable
static const double lg2 = std::log(2.0);

//------------------------------------------------------------------------------

/// Finds the number of trailing zeros in v.
/// The following code to count trailing zeros was taken from:
/// http://graphics.stanford.edu/~seander/bithacks.html#ZerosOnRightFloatCast
inline unsigned int trailing_zeros(unsigned int v)
{
#ifdef _MSC_VER
  #pragma warning( push )
  #pragma warning( disable : 4146 ) // warning C4146: unary minus operator applied to unsigned type, result still unsigned
#endif
    float  f = (float)(v & -v); // cast the least significant bit in v to a float
    return (*(uint32_t *)&f >> 23) - 0x7f; // the result goes here
#ifdef _MSC_VER
  #pragma warning( pop )
#endif
}

//------------------------------------------------------------------------------

/// Counts the number of bits set in v (the Brian Kernighan's way)
/// The following code to count set bits was taken from:
/// http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetKernighan
inline unsigned int bits_set(intptr_t v)
{
    unsigned int c = 0; // c accumulates the total bits set in v

    for (; v; c++)
        v &= v - 1; // clear the least significant bit set

    return c;
}

//------------------------------------------------------------------------------
// FIX: Optimize this draft function
inline size_t req_bits(size_t v)
{
    size_t r = 1;   // r-1 will be lg(v)

    while (v >>= 1) // unroll for more speed...
        r++;

    return r;
};

//------------------------------------------------------------------------------

template <typename T>
class vtblmap
{
/*
private:

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
    typedef std::unordered_map<intptr_t, T, vtbl_hasher> vtbl_to_t_map;
    #else
    /// A map from vtbl to T
    typedef std::unordered_map<intptr_t, T>              vtbl_to_t_map;
    #endif

    typedef typename vtbl_to_t_map::iterator    iterator;
    typedef typename vtbl_to_t_map::value_type  value_type;

public:

    typedef typename vtbl_to_t_map::mapped_type mapped_type;

    /// Structure describing entry in the cache
    struct cache_entry
    {
        intptr_t vtbl;   ///< vtbl for which value has been computed
        T        value;  ///< a copy of the value from the hash table!
    };

    /// This is the main function to get the value of type T associated with
    /// the vtbl of a given pointer.
    /// \note The function returns the value "by value" to indicate that you 
    ///       may not try to take address of it as it may be from the cache!
    inline T get(const void* p, const T dflt = T()) throw()
    {
        const intptr_t vtbl = *reinterpret_cast<const intptr_t*>(p);
        cache_entry&   ce   = cache[(vtbl>>optimal_shift) & cache_mask];

        XTL_ASSERT(vtbl); // Since this represents VTBL pointer it cannot be null

        if (XTL_UNLIKELY(ce.vtbl != vtbl))
        {
            // if this is an actual collision and the current occupier is not in the hash table
            if (ce.vtbl && table.find(ce.vtbl) == table.end())  
                return sync(vtbl,dflt); // sync everything from cache back into the hash table

            // This is unused cache entry or the current occupier of the cache is already in the hash table
            ce.vtbl  = vtbl;
            ce.value = dflt;
        }

        return ce.value;
    }

    T sync(intptr_t vtbl, T value)
    {
        size_t new_ones = 1;

        for (size_t i = 0; i < cache_size; ++i)
        {
            const cache_entry& ce = cache[i];
            const iterator     q  = table.find(ce.vtbl);

            if (q == table.end())
            {
                table[ce.vtbl] = ce.value;
                ++new_ones;
            }
        }

        intptr_t diff = 0;
        intptr_t prev = vtbl;

        for (typename vtbl_to_t_map::const_iterator p = table.begin(); p != table.end(); ++p)
        {
            intptr_t vtbl = p->first;
            diff |= prev ^ vtbl;
            prev = vtbl;
        }

        size_t z = trailing_zeros(static_cast<unsigned int>(diff));
        size_t s = table.size();

        if (XTL_UNLIKELY(irrelevant_bits != z || table_size != s))
        {
            optimal_shift   = get_optimal_shift();
            irrelevant_bits = z;
            table_size      = s;
            std::memset(cache,0,sizeof(cache)); // Reset cache
        }

        table[vtbl] = value;
    }

    size_t get_optimal_shift() const
    {
        size_t opt_shift   = 0;
        double opt_entropy = 0.0;
        double total       = double(table.size());
        size_t r = trailing_zeros(static_cast<unsigned int>(m_differ));

        for (size_t i = r; i < sizeof(intptr_t)*8; ++i)
        {
            size_t histogram[1<<N] = {};

            for (typename vtbl_to_t_map::const_iterator p =  table.begin(); p !=  table.end(); ++p)
            {
                intptr_t vtbl = p->first;
                os << "Vtbl:   " << std::bitset<8*sizeof(intptr_t)>((unsigned long long)vtbl) << " -> " << ((vtbl >> i) & cache_mask) << '\t' << vtbl_typeid(vtbl).name() << std::endl;
                histogram[(vtbl >> i) & cache_mask]++;
            }

            double entropy = 0.0;

            for (size_t j = 0; j < (1<<N); ++j)
            {
                if (histogram[j])
                {
                    double pi = histogram[j]/total;
                    entropy -= pi*std::log(pi)/std::log(2.0);
                }
            }

            // We have >= here instead of just > to favor large shifts when multiple optimal exist (happens often)
            if (entropy >= opt_entropy)
            {
                opt_entropy = entropy;
                opt_shift   = i;
            }

            //std::cout << "Shift: " << i << " -> " << entropy << std::endl; 
        }

        //std::cout << "Optimal Shift: " << opt_shift << " -> " << opt_entropy << " after " << int(total) << " vtbls" << std::endl;
        //std::cout << *this << std::endl;
        return opt_shift;
    }

#if defined(XTL_DUMP_PERFORMANCE)
    std::ostream& operator>>(std::ostream& os) const
    {
        // FIX: G++ crashes when we use std::stringstream here, so we have to workaround it manually
        std::string str(8*sizeof(m_prev),'0');

        for(size_t j = 1, i = 8*sizeof(m_prev); i; --i, j<<=1)
            if (m_differ & j)
                str[i-1] = 'X';
            else
            if (m_prev & j)
                str[i-1] = '1';

        size_t histogram[1<<N] = {};

        for (typename vtbl_to_t_map::const_iterator p =  table.begin(); p !=  table.end(); ++p)
        {
            intptr_t vtbl = p->first;
        #if defined(XTL_USE_PEARSON_HASH)
            unsigned char h = pearson_hash(vtbl >> optimal_shift);
            //os << "Vtbl:   " << std::bitset<8*sizeof(intptr_t)>((unsigned long long)vtbl) << " -> " << (size_t(h) & cache_mask) << std::endl;
            histogram[size_t(h) & cache_mask]++;
        #else
            os << "Vtbl:   " << std::bitset<8*sizeof(intptr_t)>((unsigned long long)vtbl) << " -> " << ((vtbl >> optimal_shift) & cache_mask) << '\t' << vtbl_typeid(vtbl).name() << std::endl;
            histogram[(vtbl >> optimal_shift) & cache_mask]++;
        #endif
        }

        double entropy = 0.0;
        double total   = double(table.size());

        for (size_t j = 0; j < (1<<N); ++j)
        {
            if (histogram[j])
            {
                double pi = histogram[j]/total;
                entropy -= pi*std::log(pi)/std::log(2.0);
            }
        }

        os << "VTBLS: " << str 
           << " irrelevant="    << irrelevant_bits
           << " shift="         << optimal_shift
           << " width="         << str.find_last_of("X")-str.find_first_of("X")+1 
           << " Entropy: "      << entropy << "\t ";

        bool show = false;

        for (size_t i = table.size(); i != ~0; --i)
        {
            size_t n = std::count(histogram,histogram+XTL_ARR_SIZE(histogram),i);

            if (show = show || n > 0)
                os << i << "->" << n << "; ";
        }

        os << std::count(histogram,histogram+XTL_ARR_SIZE(histogram),0)*100/(1<<N) << "% unused" << std::endl;

        //for (size_t i = 0; i < 1<<cache_bits; ++i)
        //{
        //    if (cache[i].vtbl == 0)
        //        os << i << ',';
        //}

        return os; // << std::endl;
    }
    friend std::ostream& operator<<(std::ostream& os, const vtblmap& m) { return m >> os; }
#endif
//private:

    /// Cached mappings of vtbl to some indecies
    cache_entry* cache;

    /// Variable that tracks bits that are different in all vtbl pointers 
    /// passed through this intance.
    intptr_t m_differ;

    /// A variable that holds the value of previous vtbl pointer, that passed 
    /// through the map. It is needed for computing @m_differ.
    intptr_t m_prev;

    /// Irrelevant lowest bits in vtbl pointers that came through this map
    size_t irrelevant_bits;

    /// The amount of bits in which all vtbl pointers differ
    size_t different_bits;

    /// Optimal shift computed based on the vtbl pointers already in the map.
    /// Most of the time this value would be equal to @irrelevant_bits, but not
    /// necessarily always. In case of collisions, optimal_shift will have
    /// a value of a shift that maximizes entropy of caching vtbl pointers (which 
    /// effectively also minimizes probability of not finding something in cache)
    size_t optimal_shift;

    /// Actual mapping of vtbl pointers (in reality keys obtained from them) 
    /// to values of type T. Values in this table are cached in @cache.
    vtbl_to_t_map table;
*/
};

//------------------------------------------------------------------------------

template <typename T>
class vtblmap<T&>
{
private:

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
    typedef std::unordered_map<intptr_t, T, vtbl_hasher> vtbl_to_t_map;
#else
    /// A map from vtbl to T
    typedef std::unordered_map<intptr_t, T>              vtbl_to_t_map;
#endif

    typedef typename vtbl_to_t_map::iterator    iterator;
    typedef typename vtbl_to_t_map::value_type  value_type;

public:

    
#if defined(XTL_DUMP_PERFORMANCE)
    vtblmap(const char* fl, size_t ln, const bit_offset_t log_size = min_log_size) : file(fl), line(ln), updates(0),
        cache(new cache_entry[1<<log_size]),
        cache_mask((1<<log_size)-1),
        optimal_shift(VTBL_IRRELEVANT_BITS),
        last_table_size(0),
        collisions(0)
    {
        std::memset(cache,0,(1<<log_size)*sizeof(cache_entry)); // Reset cache
    }
#endif
    vtblmap(const bit_offset_t log_size = min_log_size) : XTL_DUMP_PERFORMANCE_ONLY(file("unspecified"), line(0), updates(0),)
        cache(new cache_entry[1<<log_size]),
        cache_mask((1<<log_size)-1),
        optimal_shift(VTBL_IRRELEVANT_BITS),
        last_table_size(0),
        collisions(0)
    {
        std::memset(cache,0,(1<<log_size)*sizeof(cache_entry)); // Reset cache
    }

   ~vtblmap()
    {
        delete[] cache; 
        XTL_DUMP_PERFORMANCE_ONLY(std::cout << *this << std::endl); 
    }

    typedef typename vtbl_to_t_map::mapped_type mapped_type;

    /// Structure describing entry in the cache
    struct cache_entry
    {
        intptr_t vtbl;   ///< vtbl for which value has been computed
        T*       ptr;    ///< pointer to actual value in the table!
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
            if (ce.vtbl && ++collisions > 4 && table.size() != last_table_size)
                return update(vtbl); // try to rearrange cache

            ce.ptr = &table[vtbl];

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

            //size_t sz = table.size();

            //if (/*++collisions > sz &&*/ sz != last_table_size)
            //    return update(vtbl);   // try to rearrange cache
            //else
            //    ce.ptr = &table[vtbl];

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

        return *ce.ptr;
    }

    T& update(intptr_t vtbl)
    {
        XTL_ASSERT(last_table_size  < table.size()); // We will only call this if size changed
        XTL_ASSERT(table.find(vtbl) == table.end()); // This is guaranteed by the caller

        XTL_DUMP_PERFORMANCE_ONLY(++updates); // Record update
        last_table_size = table.size();       // Update memoized value
        collisions = 0;                       // Reset collitions counter

        intptr_t diff = 0;
        intptr_t prev = vtbl;

        for (typename vtbl_to_t_map::const_iterator p = table.begin(); p != table.end(); ++p)
        {
            intptr_t vtbl = p->first;
            diff |= prev ^ vtbl;
            prev = vtbl;
        }

        T& result = table[vtbl];

        if (true/*diff != m_different*/)
        {
            bit_offset_t k = req_bits(cache_mask);     // current log_size
            bit_offset_t n = req_bits(table.size()-1); // needed log_size
            bit_offset_t l = std::min(bit_offset_t(n+1),max_log_size);
            bit_offset_t z = trailing_zeros(static_cast<unsigned int>(diff));
            bit_offset_t m = req_bits(diff);
            bit_offset_t no = n;
            bit_offset_t zo = z;
            double e_max = 0.0;

            for (bit_offset_t i = n; i <= l; ++i)
            {
                for (bit_offset_t j = z; j <= m-i; ++j)
                {
                    double e = entropy(i,j);

                    if (e > e_max)
                    {
                        e_max = e;
                        no = i;
                        zo = j;
                    }
                }
            }

            if (no > k)
            {
                delete[] cache;
                cache = new cache_entry[1<<no];
                cache_mask = (1<<no)-1;
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
#if defined(XTL_DUMP_PERFORMANCE)
        //std::cout << "Vtbl:New" << std::bitset<8*sizeof(intptr_t)>((unsigned long long)vtbl) << " -> " << ((vtbl >> optimal_shift) & cache_mask) << '\t' << vtbl_typeid(vtbl).name() << std::endl;
        //*this >> std::cout;       
#endif
        return result;
    }

    /// Compute entropy of collision probabilities when all vtbls currently 
    /// present in the table will be mapped into cache of size 2^log_size 
    /// with given offset used to ignore irrelevant bits.
    double entropy(bit_offset_t log_size, bit_offset_t offset) const
    {
        const size_t   cache_size = 1<<log_size;
        const intptr_t cache_mask = cache_size-1;

        XTL_ASSERT(log_size <= max_log_size); 
        XTL_ASSERT(table.size() < 1 << sizeof(vtbl_count_t)*8);

        // We do this to not resort to vectors and heap and keep counting on stack
        XTL_VLAZ(histogram, vtbl_count_t, cache_size, 1<<max_log_size); // vtbl_count_t histogram[cache_size];

        for (typename vtbl_to_t_map::const_iterator p = table.begin(); p != table.end(); ++p)
            histogram[(p->first >> offset) & cache_mask]++;

        double result = 0.0;
        double total  = double(table.size());

        for (size_t j = 0; j < cache_size; ++j)
        {
            if (histogram[j])
            {
                double pi = histogram[j]/total;
                result -= pi*std::log(pi)/lg2;
            }
        }
#if defined(XTL_DUMP_PERFORMANCE)
        //std::cout << "ENTROPY: " << result << "\t log_size=" << (int)log_size << " offset=" << (int)offset << '[' << line << ']' << std::endl;
#endif
        return result;
    }

    /// Computes probability of conflict assuming uniform distribution of vtbls
    double conflict(bit_offset_t log_size, bit_offset_t offset) const
    {
        const size_t   cache_size = 1<<log_size;
        const intptr_t cache_mask = cache_size-1;

        XTL_ASSERT(log_size <= max_log_size); 
        XTL_ASSERT(table.size() < 1 << sizeof(vtbl_count_t)*8);

        // We do this to not resort to vectors and heap and keep counting on stack
        XTL_VLAZ(histogram, vtbl_count_t, cache_size, 1<<max_log_size); // vtbl_count_t histogram[cache_size];

        for (typename vtbl_to_t_map::const_iterator p = table.begin(); p != table.end(); ++p)
            histogram[(p->first >> offset) & cache_mask]++;

        double result = 0.0;
        double total  = double(table.size());

        for (size_t j = 0; j < cache_size; ++j)
            if (histogram[j] > 1)
                result += (histogram[j]-1)/total;

        return result;
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
               << " -> " << ((vtbl >> optimal_shift) & cache_mask) 
               << '\t';
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

        os << "VTBLS:  "     << str
           << " total="      << table.size()
           << " log_size="   << log_size
           << " shift="      << optimal_shift
           << " width="      << str.find_last_of("X")-str.find_first_of("X")+1 
           XTL_DUMP_PERFORMANCE_ONLY(<< " updates=" << updates)
           << " Entropy: "   << entropy(log_size, optimal_shift)
           << " Conflict: "  << conflict(log_size, optimal_shift) << "\t ";

        bool show = false;

        for (size_t i = table.size(); i != ~0; --i)
        {
            size_t n = std::count(histogram,histogram+cache_size,i);

            if (show = show || n > 0)
                os << i << "->" << n << "; ";
        }

        os << std::count(histogram,histogram+cache_size,0)*100/cache_size << "% unused " << '[' << line << ']' << std::endl;

        //bit_offset_t k = req_bits(cache_mask);     // current log_size
        bit_offset_t n = req_bits(table.size()-1); // needed log_size
        bit_offset_t l = std::min(bit_offset_t(n+1),max_log_size);
        bit_offset_t z = trailing_zeros(static_cast<unsigned int>(diff));
        bit_offset_t m = req_bits(diff);

        for (bit_offset_t i = n; i <= l; ++i)
        {
            for (bit_offset_t j = z; j <= m-i; ++j)
            {
                double e = entropy(i,j);
                double p = conflict(i,j);
                os << "\tEntropy for log_size=" << int(i) << " and shift=" << int(j) << " is " << e << " or " << e/i << " per cell witch conflict " << p << std::endl;
            }
        }

        return os/* << std::endl*/;
    }
    friend std::ostream& operator<<(std::ostream& os, const vtblmap& m) { return m >> os; }
#endif
private:

    /// Cached mappings of vtbl to some indecies
    cache_entry* cache;

    /// Cache mask to access entries. Always cache_size-1 since cache_size is a power of 2
    size_t cache_mask;

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

    /// Number of colisions since last cache rearranging
    size_t collisions;

#if defined(XTL_DUMP_PERFORMANCE)
    const char* file;    ///< File in which this vtblmap is instantiated
    size_t      line;    ///< Line in the file where it is instantiated
    size_t      updates; ///< Amount of reconfigurations performed at run time
#endif
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
