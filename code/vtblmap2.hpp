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
#include <unordered_map>
#include "config.hpp"    // Various compiler/platform dependent macros

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
class vtblmap
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
        cache_entry&   ce   = cache[key & cache_mask];

        XTL_ASSERT(vtbl);                                // Since this represents VTBL pointer it cannot be null
        XTL_ASSERT(!(vtbl & (1<<irrelevant_bits)-1));    // Assertion here means your irrelevant_bits is not correct as there are 1 bits in what we discard

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
        cache_entry&   ce   = cache[key & cache_mask];

        XTL_ASSERT(vtbl);                                // Since this represents VTBL pointer it cannot be null
        XTL_ASSERT(!(vtbl & (1<<irrelevant_bits)-1));    // Assertion here means your irrelevant_bits is not correct as there are 1 bits in what we discard

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
class vtblmap<T&,N>
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
        const intptr_t key  = vtbl>>irrelevant_bits;     // We do this as we rely that hash function is identity
        cache_entry&   ce   = cache[key & cache_mask];

        XTL_ASSERT(vtbl);                                // Since this represents VTBL pointer it cannot be null
        XTL_ASSERT(!(vtbl & (1<<irrelevant_bits)-1));    // Assertion here means your irrelevant_bits is not correct as there are 1 bits in what we discard

        if (ce.vtbl != vtbl)
        {
            const iterator q = table.find(key);

            ce.ptr = 
                q != table.end() 
                    ? &q->second
                    : &table.insert(value_type(key,dflt)).first->second;

            ce.vtbl  = vtbl;
        }

        return *ce.ptr;
    }

    inline bool get_ex(const void* p, T& val) throw()
    {
        const intptr_t vtbl = *reinterpret_cast<const intptr_t*>(p);
        const intptr_t key  = vtbl>>irrelevant_bits;     // We do this as we rely that hash function is identity
        cache_entry&   ce   = cache[key & cache_mask];

        XTL_ASSERT(vtbl);                                // Since this represents VTBL pointer it cannot be null
        XTL_ASSERT(!(vtbl & (1<<irrelevant_bits)-1));    // Assertion here means your irrelevant_bits is not correct as there are 1 bits in what we discard

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

struct line_offset
{
    int line;   // We can choose smaller type for line to give more space to offset
    int offset; // FIX: We assume here offsets within object can only be ints
};

template <int N = VTBL_DEFAULT_CACHE_BITS>
class vtbl2lines : public vtblmap<line_offset&,N>
{
private:
    typedef vtblmap<line_offset,N> base_type;
public:
    inline void update(int ln, const void* base, const void* derived) throw()
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

template <int N = VTBL_DEFAULT_CACHE_BITS>
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

template <int N = VTBL_DEFAULT_CACHE_BITS>
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

    inline void update(int ln, const void* t, size_t j) throw()
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
