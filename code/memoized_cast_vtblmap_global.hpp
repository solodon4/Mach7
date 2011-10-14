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
#include "vtblmap2.hpp"

//------------------------------------------------------------------------------

// A dedicated constant marking that table entry was just allocated
#define just_allocated 0x0FF1C1A0
// A dedicated constant marking impossible offset
#define no_cast_exists 0x0FF1C1A1

//------------------------------------------------------------------------------

template <typename T> struct cast_target;
template <typename T> struct cast_target<      T*> { typedef T type; };
template <typename T> struct cast_target<const T*> { typedef T type; };

//------------------------------------------------------------------------------

/*extern */vtbl2offsets<> global_offset_map(just_allocated);

//------------------------------------------------------------------------------

template <typename T, typename U>
inline T memoized_cast_non_null(const U* p)
{
    XTL_ASSERT(p);

    // TODO: 
    // 1. vtbl with pointers directly to table instead of indecies
    // 2. store type index inside match_members
    // 3. try smaller type int instead of size_t or ptrdiff_t
    std::ptrdiff_t& offset = global_offset_map.get(p,type_index<T>());

    if (offset == just_allocated)
    {
        T k = dynamic_cast<T>(p);
        offset = k ? reinterpret_cast<const char*>(k)-reinterpret_cast<const char*>(p) : no_cast_exists;
        return k;
    }

    return 
        offset == no_cast_exists 
            ? 0 
            : reinterpret_cast<T>(reinterpret_cast<const char*>(p)+offset);
}

//------------------------------------------------------------------------------

template <typename T, typename U>
inline T memoized_cast_non_null(U* u)
{
    return 
        const_cast<T>(
            memoized_cast_non_null<typename cast_target<T >::type const*>(
                       static_cast<typename cast_target<U*>::type const*>(u)
            )
        );
}

//------------------------------------------------------------------------------

//template <typename T, typename U>
//inline T memoized_cast(const U* u)
//{
//    return  u
//            ? memoized_cast_non_null<T>(u) 
//            : 0;
//}

//------------------------------------------------------------------------------

template <typename T, typename U>
inline T memoized_cast(U* u)
{
    return  u
            ? memoized_cast_non_null<T>(u) 
            : 0;
}

//------------------------------------------------------------------------------
