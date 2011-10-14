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
#include "vtblmap.hpp"

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

typedef const void* dyn_cast_offset_function(const void* p, std::ptrdiff_t& offset);

template <typename T>
inline T memoized_cast_ex(const void* p, dyn_cast_offset_function* dyn_off_getter)
{
    XTL_ASSERT(p); // All checks for null should have been done before
    static vtblmap<std::ptrdiff_t> ofsmap; // One per target type T
    std::ptrdiff_t& offset = ofsmap.get(p,just_allocated);

    switch (offset)
    {
    case just_allocated:
        return reinterpret_cast<T>(dyn_off_getter(p,offset));
    case no_cast_exists:
        return 0;
    default:
        return reinterpret_cast<T>(reinterpret_cast<std::intptr_t>(p)+offset);
    }
}

//------------------------------------------------------------------------------

/// A generic wrapper to obtain an offset of dynamic_cast from type U* to type T*.
/// We pass this function by address as it is the slowest part anyways, so one 
/// extra function call is not going make it slower, but will save on generated code.
template <typename T, typename U>
inline const void* dyn_cast_offset(const void* p, std::ptrdiff_t& offset)
{
    if (T k = dynamic_cast<T>(static_cast<const U*>(p)))
    {
        offset = reinterpret_cast<const char*>(k)-reinterpret_cast<const char*>(p);
        return k;
    }
    else
    {
        offset = no_cast_exists;
        return 0;
    }
}

//------------------------------------------------------------------------------

template <typename T, typename U>
inline T memoized_cast(const U* u)
{
    return u ? memoized_cast_ex<T>(u,&dyn_cast_offset<T,U>) : 0;
}

//------------------------------------------------------------------------------

template <typename T, typename U>
inline T memoized_cast(U* u)
{
    return const_cast<T>(memoized_cast<typename cast_target<T>::type const*>(static_cast<typename cast_target<U*>::type const*>(u)));
}

//------------------------------------------------------------------------------
