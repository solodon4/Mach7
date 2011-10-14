///
/// \file vtblmap.hpp
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

#include <functional>
#include <unordered_map>

template <typename T, size_t cache_bits = 3>
class vtblmap
{
private:

    typedef std::unordered_map<intptr_t, T> base_type;
    typedef typename base_type::iterator    iterator;
    typedef typename base_type::value_type  value_type;

public:

    /// A few useful constants
    enum
    {
        cache_mask = (1<<cache_bits)-1,

        // FIX: Make this less empirical
    #ifdef _MSC_VER
        irrelevant_bits = 3  // vtbl in MSVC seem to be alligned by 4 bytes
    #else
        irrelevant_bits = 4  // vtbl in G++ seem to be alligned by 8 bytes
    #endif
    };

    /// Structure describing entry in the cache
    struct cache_entry
    {
        intptr_t  vtbl; ///< vtbl for which value has been computed
        T*        ptr;  ///< pointer to value associated with given vtbl
    };

   	/// Hash functor for vtbl
    struct vtbl_hasher
    {
	    typedef intptr_t argument_type;
	    typedef size_t   result_type;

        /// hash key to size_t value by pseudorandomizing transform
        size_t operator()(const intptr_t key) const { return key; }
    };

#ifdef _MSC_VER
    typedef std::unordered_map<intptr_t, T, vtbl_hasher> vtbl_to_t_map;
#else
    typedef std::unordered_map<intptr_t, T> vtbl_to_t_map;
#endif

    /// This is the main function to get hold of a value associated with the vtbl of a given pointer
    inline T& get(const void* p, const T& dflt = T()) throw()
    {
        const intptr_t vtbl = *reinterpret_cast<const intptr_t*>(p);
        const intptr_t key  = vtbl>>irrelevant_bits; // We do this as we rely that hash function is identity
        cache_entry& ce = cache[key & cache_mask];

        if (ce.vtbl != vtbl)
        {
            const iterator q = table.find(key);
            ce.ptr  = q != table.end() ? &q->second : &table.insert(value_type(key,dflt)).first->second;
            ce.vtbl = vtbl;
        }

        return *ce.ptr;
    }

    vtbl_to_t_map table;
    cache_entry   cache[1<<cache_bits];
};
