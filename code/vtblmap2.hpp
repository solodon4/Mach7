///
/// \file vtblmap2.hpp
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

#include <cstdint>
#include <cstring>
#include <unordered_map>
#include "config.hpp"    // Various compiler/platform dependent macros

#if defined(DUMP_PERFORMANCE)
#include <algorithm>
#include <bitset> // For print out purposes only
#include <iostream>
#include <string>
#endif
/// Finds the number of trailing zeros in v.
/// The following code to count trailing zeros was taken from:
/// http://graphics.stanford.edu/~seander/bithacks.html#ZerosOnRightFloatCast
inline unsigned int trailing_zeros(unsigned int v)
{
    float  f = (float)(v & -v); // cast the least significant bit in v to a float
    return (*(uint32_t *)&f >> 23) - 0x7f; // the result goes here
}

//------------------------------------------------------------------------------

#if defined(USE_PEARSON_HASH)
static const unsigned char H[256] = 
{
      1,  87,  49,  12, 176, 178, 102, 166, 121, 193,   6,  84, 249, 230,  44, 163,
     14, 197, 213, 181, 161,  85, 218,  80,  64, 239,  24, 226, 236, 142,  38, 200,
    110, 177, 104, 103, 141, 253, 255,  50,  77, 101,  81,  18,  45,  96,  31, 222,
     25, 107, 190,  70,  86, 237, 240,  34,  72, 242,  20, 214, 244, 227, 149, 235,
     97, 234,  57,  22,  60, 250,  82, 175, 208,   5, 127, 199, 111,  62, 135, 248,
    174, 169, 211,  58,  66, 154, 106, 195, 245, 171,  17, 187, 182, 179,   0, 243,
    132,  56, 148,  75, 128, 133, 158, 100, 130, 126,  91,  13, 153, 246, 216, 219,
    119,  68, 223,  78,  83,  88, 201,  99, 122,  11,  92,  32, 136, 114,  52, 10 ,
    138,  30,  48, 183, 156,  35,  61,  26, 143,  74, 251,  94, 129, 162,  63, 152,
    170,   7, 115, 167, 241, 206,   3, 150,  55,  59, 151, 220,  90,  53,  23, 131,
    125, 173,  15, 238,  79,  95,  89,  16, 105, 137, 225, 224, 217, 160,  37, 123,
    118,  73,   2, 157,  46, 116,   9, 145, 134, 228, 207, 212, 202, 215,  69, 229,
     27, 188,  67, 124, 168, 252,  42,   4,  29, 108,  21, 247,  19, 205,  39, 203,
    233,  40, 186, 147, 198, 192, 155,  33, 164, 191,  98, 204, 165, 180, 117, 76 ,
    140,  36, 210, 172,  41,  54, 159,   8, 185, 232, 113, 196, 231,  47, 146, 120,
     51,  65,  28, 144, 254, 221,  93, 189, 194, 139, 112,  43,  71, 109, 184, 209
};

inline unsigned char pearson_hash(intptr_t key)
{
    unsigned char h = H[(unsigned char)key];
    h = H[h ^ (unsigned char)(key>> 8)];
    h = H[h ^ (unsigned char)(key>>16)];
    //h = H[h ^ (unsigned char)(key>>24)];
    return h;
}
#endif

//------------------------------------------------------------------------------

#ifdef TRACE_PERFORMANCE
#include <algorithm>
#include <bitset> // For print out purposes only
#include <set>

template <size_t N>
class vtblmap_performance
{
public:
    vtblmap_performance() : cache_hits(0), cache_misses(0), common(~0), differ(0), prev(0) {}
   ~vtblmap_performance() { std:: cout << *this << std::endl; }

    /// A few useful constants
    enum
    {
        cache_bits = N,
        cache_mask = (1<<cache_bits)-1,
        /// Irrelevant lowest bits in vtbl pointers that are always the same for given 
        /// compiler/platform configuration.
        irrelevant_bits = VTBL_IRRELEVANT_BITS
    };

    void update(intptr_t vtbl, intptr_t cached_vtbl)
    {
        if (vtbl != cached_vtbl)
        {
            vtbls.insert(vtbl);

            //std::cout << "Differ: " << std::bitset<8*sizeof(intptr_t)>((unsigned long long)differ) << std::endl;
            //std::cout << "Common: " << std::bitset<8*sizeof(intptr_t)>((unsigned long long)common) << std::endl;
            //std::cout << "Prev  : " << std::bitset<8*sizeof(intptr_t)>((unsigned long long)prev)   << std::endl;
            //std::cout << "Vtbl  : " << std::bitset<8*sizeof(intptr_t)>((unsigned long long)vtbl)   << std::endl;
            //std::cout << " Pr^Vt: " << std::bitset<8*sizeof(intptr_t)>((unsigned long long)  prev ^ vtbl)  << std::endl;
            //std::cout << "~Pr^Vt: " << std::bitset<8*sizeof(intptr_t)>((unsigned long long)~(prev ^ vtbl)) << std::endl;

            ++cache_misses;

            if (prev)
            {
                intptr_t prvt = prev ^ vtbl;
                common &= ~prvt;
                differ |=  prvt;
            }

            prev = vtbl;
        }
        else
            ++cache_hits;
    }

    std::ostream& operator>>(std::ostream& os) const
    {
        std::cout << "Vtbl cache hits: "   << cache_hits << '\t'
                  << "Vtbl cache misses: " << cache_misses 
                  << std::endl;

        size_t uses[1<<N] = {};

        for (std::set<intptr_t>::const_iterator p = vtbls.begin(); p != vtbls.end(); ++p)
        {
            intptr_t vtbl = *p;
            const intptr_t key  = vtbl >> irrelevant_bits; // We do this as we rely that hash function is identity
        #if defined(USE_PEARSON_HASH)
            unsigned char h = pearson_hash(key);
            os << "Vtbl:   " << std::bitset<8*sizeof(intptr_t)>((unsigned long long)vtbl) << " -> " << (size_t(h) & cache_mask) << " -> " << (key & cache_mask) << std::endl;
            uses[h & cache_mask]++;
        #else
            os << "Vtbl:   " << std::bitset<8*sizeof(intptr_t)>((unsigned long long)vtbl) << " -> " << (key & cache_mask) << std::endl;
            uses[key & cache_mask]++;
        #endif
        }

        bool show = false;

        for (size_t i = vtbls.size(); i != ~0; --i)
        {
            size_t n = std::count(uses,uses+ARR_SIZE(uses),i);

            if (show = show || n > 0)
                std::cout << i << " -> " << n << std::endl;
        }

        size_t i = trailing_zeros(static_cast<unsigned int>(differ));

        if (i != VTBL_IRRELEVANT_BITS)
        {
            os << "WARNING: Empirically computed irrelevant_bits " << i 
               << " differs from the predefined one " STRING_LITERAL(VTBL_IRRELEVANT_BITS) 
               << ". See vtbl patterns below: " << std::endl;
        }

        os << "Common: " << std::bitset<8*sizeof(intptr_t)>((unsigned long long)prev|differ) << std::endl
           << "Differ: " << std::bitset<8*sizeof(intptr_t)>((unsigned long long)differ) << std::endl;

        return os;
    }

    friend std::ostream& operator<<(std::ostream& os, const vtblmap_performance& p) { return p >> os; }

    intptr_t cache_hits;
    intptr_t cache_misses;
    intptr_t common;
    intptr_t differ;
    intptr_t prev;
    std::set<intptr_t> vtbls;
};

#define UPDATE_VTBL_PERFORMANCE(vtbl, cached_vtbl) this->update(vtbl, cached_vtbl)
#define TRACE_PERFORMANCE_ONLY(x) x
#else
#define UPDATE_VTBL_PERFORMANCE(vtbl, cached_vtbl)
#define TRACE_PERFORMANCE_ONLY(x)
#endif

//------------------------------------------------------------------------------

/// Global counter of allocated type indecies
size_t global_index = 1;

/// Returns the index that will be used for a given type T
template <typename T> 
inline size_t type_index() 
{ 
    static size_t idx = global_index++; 
    return idx; 
}

//------------------------------------------------------------------------------

template <typename T, size_t N = VTBL_DEFAULT_CACHE_BITS>
class vtblmap TRACE_PERFORMANCE_ONLY(: vtblmap_performance<N>)
{
private:

#if defined(USE_PEARSON_HASH_XXX)
    struct vtbl_hasher
    {
	    typedef intptr_t argument_type;
	    typedef size_t   result_type;

        /// hash key to size_t value by pseudorandomizing transform
        size_t operator()(const intptr_t key) const 
        {
            return pearson_hash(key);
        }
    };

    /// A map from vtbl to T
    typedef std::unordered_map<intptr_t, T, vtbl_hasher> vtbl_to_t_map;

#else
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
#endif
    typedef typename vtbl_to_t_map::iterator    iterator;
    typedef typename vtbl_to_t_map::value_type  value_type;

public:

    typedef typename vtbl_to_t_map::mapped_type mapped_type;

    /// A few useful constants
    enum
    {
        cache_bits = N,
        cache_mask = (1<<cache_bits)-1,
        /// Irrelevant lowest bits in vtbl pointers that are always the same for given 
        /// compiler/platform configuration.
        irrelevant_bits = VTBL_IRRELEVANT_BITS
    };

    /// Structure describing entry in the cache
    struct cache_entry
    {
        intptr_t vtbl;   ///< vtbl for which value has been computed
        T        value;  ///< a copy of a value from the hash table!
    };

    /// This is the main function to get the value of type T associated with
    /// the vtbl of a given pointer.
    /// \note The function returns the value "by value" to indicate that you 
    ///       may not try to take address of it as it may be from the cache!
    inline T get(const void* p, const T dflt = T()) throw()
    {
        const intptr_t vtbl = *reinterpret_cast<const intptr_t*>(p);
        const intptr_t key  = vtbl>>irrelevant_bits;     // We do this as we rely that hash function is identity
    #if defined(USE_PEARSON_HASH)
        cache_entry&   ce   = cache[pearson_hash(key) & cache_mask];
    #else
        cache_entry&   ce   = cache[key & cache_mask];
    #endif

        XTL_ASSERT(vtbl);                                // Since this represents VTBL pointer it cannot be null
        XTL_ASSERT(!(vtbl & (1<<irrelevant_bits)-1));    // Assertion here means your irrelevant_bits is not correct as there are 1 bits in what we discard
        UPDATE_VTBL_PERFORMANCE(vtbl, ce.vtbl);          // When TRACE_PERFORMANCE is enabled, this will update our performance counters

        if (ce.vtbl != vtbl)
        {
            const iterator q = table.find(key);

            ce.value = 
                q != table.end() 
                    ? q->second
                    : table.insert(value_type(key,dflt)).first->second;

            ce.vtbl  = vtbl;
        }

        return ce.value;
    }

    inline bool get_ex(const void* p, T& val) throw()
    {
        const intptr_t vtbl = *reinterpret_cast<const intptr_t*>(p);
        const intptr_t key  = vtbl>>irrelevant_bits;     // We do this as we rely that hash function is identity
    #if defined(USE_PEARSON_HASH)
        cache_entry&   ce   = cache[pearson_hash(key) & cache_mask];
    #else
        cache_entry&   ce   = cache[key & cache_mask];
    #endif

        XTL_ASSERT(vtbl);                                // Since this represents VTBL pointer it cannot be null
        XTL_ASSERT(!(vtbl & (1<<irrelevant_bits)-1));    // Assertion here means your irrelevant_bits is not correct as there are 1 bits in what we discard
        UPDATE_VTBL_PERFORMANCE(vtbl, ce.vtbl);          // When TRACE_PERFORMANCE is enabled, this will update our performance counters

        bool result = false;

        if (ce.vtbl != vtbl)
        {
            const iterator q = table.find(key);

            ce.value = 
                !(result = q == table.end())
                    ? q->second
                    : table.insert(value_type(key,val)).first->second;

            ce.vtbl  = vtbl;
        }

        val = ce.value;
        return result;
    }

private:

    /// Cached mappings of vtbl to some indecies
    cache_entry   cache[1<<cache_bits];
    /// Actual mapping of vtbl pointers (in reality keys obtained from them) 
    /// to values of type T. Values in this table are cached in @cache.
    vtbl_to_t_map table;

};

//------------------------------------------------------------------------------

template <typename T, size_t N>
class vtblmap<T&,N> TRACE_PERFORMANCE_ONLY(: vtblmap_performance<N>)
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

    vtblmap() : m_differ(0), m_prev(0), irrelevant_bits(VTBL_IRRELEVANT_BITS) {}
#if defined(DUMP_PERFORMANCE)
   ~vtblmap() { std::cout << *this << std::endl; }
#endif
    typedef typename vtbl_to_t_map::mapped_type mapped_type;

    /// A few useful constants
    enum
    {
        cache_bits = N,
        cache_mask = (1<<cache_bits)-1,
        /// Irrelevant lowest bits in vtbl pointers that are always the same for given 
        /// compiler/platform configuration.
        ///irrelevant_bits = VTBL_IRRELEVANT_BITS
    };

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
    inline T& get(const void* p, const T& dflt = T()) throw()
    {
        const intptr_t vtbl = *reinterpret_cast<const intptr_t*>(p);
    #if defined(USE_PEARSON_HASH)
        cache_entry&   ce   = cache[pearson_hash(vtbl>>irrelevant_bits) & cache_mask];
    #else
        cache_entry&   ce   = cache[(vtbl>>irrelevant_bits) & cache_mask];
    #endif

        XTL_ASSERT(vtbl);                                // Since this represents VTBL pointer it cannot be null
        //XTL_ASSERT(!(vtbl & (1<<irrelevant_bits)-1));    // Assertion here means your irrelevant_bits is not correct as there are 1 bits in what we discard
        UPDATE_VTBL_PERFORMANCE(vtbl, ce.vtbl);          // When TRACE_PERFORMANCE is enabled, this will update our performance counters

        if (ce.vtbl != vtbl)
        {
            const iterator q = table.find(vtbl);

            if (q != table.end())
                ce.ptr = &q->second;
            else
            {
                ce.ptr = &table.insert(value_type(vtbl,dflt)).first->second;

                // Update m_differ with information about which bits in all vtbls differ
                if (m_prev)
                    m_differ |= m_prev ^ vtbl;

                m_prev = vtbl;

                // If this is an actual collision, we recompute irrelevant_bits
                if (ce.vtbl)
                {
                    size_t r = trailing_zeros(static_cast<unsigned int>(m_differ));

                    if (irrelevant_bits != r)
                    {
                        irrelevant_bits = r;
                        auto saved_ptr = ce.ptr; // Since we've alread written it. Putting the whole insertion later degrades performance
                        std::memset(cache,0,sizeof(cache)); // Reset cache
                        ce.ptr = saved_ptr;
                    }
                }
            }

            ce.vtbl  = vtbl;
        }

        return *ce.ptr;
    }

    inline bool get_ex(const void* p, T& val) throw()
    {
        const intptr_t vtbl = *reinterpret_cast<const intptr_t*>(p);
        const intptr_t key  = vtbl>>irrelevant_bits;     // We do this as we rely that hash function is identity
    #if defined(USE_PEARSON_HASH)
        cache_entry&   ce   = cache[pearson_hash(key) & cache_mask];
    #else
        cache_entry&   ce   = cache[key & cache_mask];
    #endif

        XTL_ASSERT(vtbl);                                // Since this represents VTBL pointer it cannot be null
        XTL_ASSERT(!(vtbl & (1<<irrelevant_bits)-1));    // Assertion here means your irrelevant_bits is not correct as there are 1 bits in what we discard
        UPDATE_VTBL_PERFORMANCE(vtbl, ce.vtbl);          // When TRACE_PERFORMANCE is enabled, this will update our performance counters

        bool result = false;

        if (ce.vtbl != vtbl)
        {
            const iterator q = table.find(key);

            ce.value = 
                !(result = q == table.end())
                    ? q->second
                    : table.insert(value_type(key,val)).first->second;

            ce.vtbl  = vtbl;
        }

        val = ce.value;
        return result;
    }
#if defined(DUMP_PERFORMANCE)
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

        os << "VTBLS: " << str << " irrelevant=" << irrelevant_bits << " width=" << str.find_last_of("X")-str.find_first_of("X")+1 << " \t";
        size_t uses[1<<N] = {};

        for (typename vtbl_to_t_map::const_iterator p =  table.begin(); p !=  table.end(); ++p)
        {
            intptr_t vtbl = p->first;
        #if defined(USE_PEARSON_HASH)
            unsigned char h = pearson_hash(vtbl >> irrelevant_bits);
            //os << "Vtbl:   " << std::bitset<8*sizeof(intptr_t)>((unsigned long long)vtbl) << " -> " << (size_t(h) & cache_mask) << std::endl;
            uses[size_t(h) & cache_mask]++;
        #else
            //os << "Vtbl:   " << std::bitset<8*sizeof(intptr_t)>((unsigned long long)vtbl) << " -> " << ((vtbl >> irrelevant_bits) & cache_mask) << std::endl;
            uses[(vtbl >> irrelevant_bits) & cache_mask]++;
        #endif
        }

        bool show = false;

        for (size_t i = table.size(); i != ~0; --i)
        {
            size_t n = std::count(uses,uses+ARR_SIZE(uses),i);

            if (show = show || n > 0)
                os << i << "->" << n << "; ";
        }

        os << std::count(uses,uses+ARR_SIZE(uses),0)*100/(1<<N) << "% unused" << std::endl;

        for (size_t i = 0; i < 1<<cache_bits; ++i)
        {
            if (cache[i].vtbl == 0)
                os << i << ',';
        }

        return os << std::endl;
    }
    friend std::ostream& operator<<(std::ostream& os, const vtblmap& m) { return m >> os; }
#endif
//private:

    /// Cached mappings of vtbl to some indecies
    cache_entry   cache[1<<cache_bits];

    /// Variable that tracks bits that are different in all vtbl pointers 
    /// passed through this intance.
    intptr_t m_differ;

    /// A variable that holds the value of previous vtbl pointer, that passed 
    /// through the map. It is needed for computing @m_differ.
    intptr_t m_prev;

    /// Irrelevant lowest bits in vtbl pointers that are always the same for given 
    /// compiler/platform configuration.
    size_t irrelevant_bits;

    /// Actual mapping of vtbl pointers (in reality keys obtained from them) 
    /// to values of type T. Values in this table are cached in @cache.
    vtbl_to_t_map table;

};

//------------------------------------------------------------------------------

struct type_switch_info
{
    std::ptrdiff_t offset; ///< FIX: We assume here offsets within object can only be ints
    std::size_t    line;   ///< We can choose smaller type for line to give more space to offset
    //int offset; ///< FIX: We assume here offsets within object can only be ints
    //int    line;   ///< We can choose smaller type for line to give more space to offset
};

template <size_t N = VTBL_DEFAULT_CACHE_BITS>
class vtbl2lines : public vtblmap<type_switch_info&,N>
{
private:
    typedef vtblmap<type_switch_info,N> base_type;
public:
    inline void update(size_t ln, const void* base, const void* derived) throw()
    {
        typename base_type::mapped_type& x = this->get(base);

        if (x.line == 0)
        {
            x.line   = ln;
            x.offset = intptr_t(derived)-intptr_t(base);
        }

        XTL_ASSERT(x.offset == intptr_t(derived)-intptr_t(base));
    }
};

//------------------------------------------------------------------------------

template <size_t N = VTBL_DEFAULT_CACHE_BITS>
class vtbl2indecies : private vtblmap<size_t,N>
{
public:

    vtbl2indecies() : indecies(0) {}

    inline size_t get(const void* p) throw()
    {
        size_t c = indecies;

        if (base_type::get_ex(p, c))
            ++indecies;

        return c;
    }

private:

    typedef vtblmap<size_t,N> base_type;

    /// The amount of allocated indecies
    size_t indecies;

};

//------------------------------------------------------------------------------

inline size_t req_bits(size_t v)
{
    // FIX: Optimize this draft function
    if (v == 0)
        return 0;

    size_t bit = 1;
    size_t i   = 0;

    for (size_t j = 0, n = 8*sizeof(size_t); j < n; ++j, bit <<= 1)
        if (v & bit)
            i = j;

    return i+1; // FIX: Not exact for powers of 2.
};

//------------------------------------------------------------------------------

template <typename T>
class table
{
public:

    table(size_t r = 0, size_t c = 0, const T& dflt = T()) :
        m_rows(r), m_cols(c), m_rows_capacity(1<<req_bits(r)), m_cols_capacity(1<<req_bits(c)), 
        m_default_value(dflt), m_data(new T[m_rows_capacity*m_cols_capacity]) 
    {
       std::fill(m_data, m_data+m_rows_capacity*m_cols_capacity,m_default_value);
    }

    table(size_t r, size_t c, size_t rc, size_t cc, const T& dflt = T()) :
        m_rows(r), m_cols(c), m_rows_capacity(rc), m_cols_capacity(cc), 
        m_default_value(dflt), m_data(new T[m_rows_capacity*m_cols_capacity]) 
    {
       std::fill(m_data, m_data+m_rows_capacity*m_cols_capacity,m_default_value);
    }

   ~table()
    {
        delete [] m_data;
    }

    T& operator()(size_t i, size_t j)
    {
        XTL_ASSERT(i < m_rows && j < m_cols);
        XTL_ASSERT(m_data);
        return m_data[m_cols_capacity*i+j];
    }

    void resize(size_t r, size_t c)
    {
        if (r < m_rows_capacity && c < m_cols_capacity)
        {
            m_rows = r;
            m_cols = c;
            // FIX: Fill new data
        }
    }

    void grow_to_include(size_t i, size_t j)
    {
        size_t r = i >= m_rows ? i+1 : m_rows;
        size_t c = j >= m_cols ? j+1 : m_cols;

        if (r != m_rows || c != m_cols)
            resize(r,c);
    }

public:

    size_t m_rows;
    size_t m_cols;
    size_t m_rows_capacity;
    size_t m_cols_capacity;
    T      m_default_value;
    T*     m_data;

};

//------------------------------------------------------------------------------

template <size_t N = VTBL_DEFAULT_CACHE_BITS>
class vtbl2offsets : private vtbl2indecies<N>
{
    typedef vtbl2indecies<N> base;

public:

    vtbl2offsets(std::ptrdiff_t dflt) : offsets(104,104,dflt) {} // FIX: Hardcoded for now

    inline std::ptrdiff_t& get(const void* p, size_t j) throw()
    {
        size_t i = base::get(p);
        return offsets(i,j);
    }

    inline void update(size_t ln, const void* t, size_t j) throw()
    {
        std::ptrdiff_t& line = get(t,j);

        if (line == offsets.m_default_value)
            line = ln;
    }

private:

    /// 2D table of offsets with vtbl index as row and type index as column
    ::table<std::ptrdiff_t> offsets;

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
