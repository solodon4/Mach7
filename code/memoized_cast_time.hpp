///
/// \file memoized_cast.hpp
///
/// This file defines function memoized_cast<T>(U) that behaves as dynamic_cast
/// but is much faster when multiple invokations are involved because of caching
///
/// \autor Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of the XTL framework (http://parasol.tamu.edu/xtl/).
/// Copyright (C) 2005-2011 Texas A&M University.
/// All rights reserved.
///

#include <cstdint>
#include <iostream>
#include <unordered_map>
#include "config.hpp"    // Various compiler/platform dependent macros

//------------------------------------------------------------------------------

typedef std::unordered_map<std::intptr_t, std::ptrdiff_t> vtbl2offset;
static const std::ptrdiff_t no_cast_exists = 0x0FF1C1A1; // A dedicated constant marking impossible offset

//------------------------------------------------------------------------------

template <typename T> struct cast_target;
template <typename T> struct cast_target<      T*> { typedef T type; };
template <typename T> struct cast_target<const T*> { typedef T type; };

//------------------------------------------------------------------------------

template <typename T>
inline vtbl2offset& offset_map()
{
    static vtbl2offset ofsmap;
    return ofsmap;
}

//------------------------------------------------------------------------------

template <typename T, typename U>
inline std::ptrdiff_t get_offset(const U* p)
{
    const std::intptr_t vtbl = *reinterpret_cast<const std::intptr_t*>(p);
    vtbl2offset& ofsmap = offset_map<T>();
    const vtbl2offset::iterator q = ofsmap.find(vtbl);

    if (q != ofsmap.end())
        return q->second;
    else
    {
        T k = dynamic_cast<T>(p);
        const std::ptrdiff_t offset = k ? reinterpret_cast<const char*>(k)-reinterpret_cast<const char*>(p) : no_cast_exists;
        ofsmap.insert(vtbl2offset::value_type(vtbl,offset));
        return offset;
    }
}

//------------------------------------------------------------------------------

// Number of bits based on which the caching will be made. The cache will have
// 2^cache_bits entries.
const int cache_bits = 7;
const int cache_mask = (1<<cache_bits)-1;

//------------------------------------------------------------------------------

/// Irrelevant lowest bits in vtbl pointers that are always the same for given 
/// compiler/platform configuration.
const int irrelevant_bits = VTBL_IRRELEVANT_BITS;

//------------------------------------------------------------------------------

/// Structure describing entry in the cache
struct cache_entry
{
    std::intptr_t  vtbl;   ///< vtbl for which offset has been computed
    std::ptrdiff_t offset; ///< offset that has to be added for ptr with given vtbl
};

//------------------------------------------------------------------------------

template <typename T, typename U>
inline T memoized_cast(const U* p)
{
    if (p)
    {
#ifndef DYN_CAST_CACHING
        static cache_entry cache[1<<cache_bits] = {};
        const std::intptr_t vtbl = *reinterpret_cast<const std::intptr_t*>(p);
        //XTL_ASSERT(vtbl);                                // Since this represents VTBL pointer it cannot be null
        //XTL_ASSERT(!(vtbl & (1<<irrelevant_bits)-1));    // Assertion here means your irrelevant_bits is not correct as there are 1 bits in what we discard
        cache_entry& ce = cache[(vtbl>>irrelevant_bits) & cache_mask];
        std::ptrdiff_t offset;

        if (ce.vtbl == vtbl)
            offset = ce.offset;
        else
        {
            ce.vtbl = vtbl;
            ce.offset = offset = get_offset<T>(p);
        }
#else
        std::ptrdiff_t offset = get_offset<T>(p);
#endif
        return 
            offset == no_cast_exists 
                ? 0 
                : reinterpret_cast<T>(reinterpret_cast<const char*>(p)+offset);
    }

    return 0;
}

//------------------------------------------------------------------------------

template <typename T, typename U>
inline T memoized_cast(U* u)
{
    return const_cast<T>(memoized_cast<typename cast_target<T>::type const*>(static_cast<typename cast_target<U*>::type const*>(u)));
}

//------------------------------------------------------------------------------
