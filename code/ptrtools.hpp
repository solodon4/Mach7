///
/// \file ptrtools.hpp
///
/// This file defines several utility functions for working with pointers,
/// especially pointers to (polymorphic) objects, that we use in XTL
///
/// \autor Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of the XTL framework (http://parasol.tamu.edu/xtl/).
/// Copyright (C) 2011 Texas A&M University.
/// All rights reserved.
///

#pragma once

#include <cstddef>
#include <typeinfo>

//------------------------------------------------------------------------------

template <typename T> inline const T* adjust_ptr(const void* p, ptrdiff_t offset) { return  reinterpret_cast<const T*>(reinterpret_cast<const char*>(p)+offset); }
template <typename T> inline       T* adjust_ptr(      void* p, ptrdiff_t offset) { return  reinterpret_cast<      T*>(reinterpret_cast<      char*>(p)+offset); }

//------------------------------------------------------------------------------

template <typename T> inline const T* addr(const T* t) { return  t; }
template <typename T> inline       T* addr(      T* t) { return  t; }
template <typename T> inline const T* addr(const T& t) { return &t; }
template <typename T> inline       T* addr(      T& t) { return &t; }

//------------------------------------------------------------------------------

template <typename T, typename U> inline const T* stat_cast(const U* p) { return static_cast<const T*>(p); }
template <typename T, typename U> inline       T* stat_cast(      U* p) { return static_cast<      T*>(p); }

//------------------------------------------------------------------------------

template <typename T> inline const std::type_info& vtbl_typeid(intptr_t vtbl) { return typeid(*reinterpret_cast<const T*>(&vtbl)); }
template <typename T> inline const std::type_info& vtbl_typeid(const void* p) { return vtbl_typeid<T>(*reinterpret_cast<const intptr_t*>(p)); }

struct polymorphic_dummy { virtual ~polymorphic_dummy(){} };

inline const std::type_info& vtbl_typeid(intptr_t vtbl) { return vtbl_typeid<polymorphic_dummy>(vtbl); }
inline const std::type_info& vtbl_typeid(const void* p) { return vtbl_typeid<polymorphic_dummy>(p); }

//------------------------------------------------------------------------------

template <int N> struct requires_bits    { enum { value = requires_bits<(N+1)/2>::value+1 }; };
template <>      struct requires_bits<1> { enum { value = 0 }; };

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
