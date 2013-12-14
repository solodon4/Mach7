///
/// \file ptrtools.hpp
///
/// This file defines several utility functions for working with pointers,
/// especially pointers to (polymorphic) objects, that we use in XTL
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of Mach7 library (http://parasol.tamu.edu/mach7/).
/// Copyright (C) 2011-2012 Texas A&M University.
/// All rights reserved.
///

#pragma once

#include "config.hpp"
#include <cstddef>
#include <cstdint>
#include <typeinfo>
#ifdef _MSC_VER
#include <excpt.h>
#endif

//------------------------------------------------------------------------------

template <typename T> inline const T* adjust_ptr(const void* p, std::ptrdiff_t offset) { return  reinterpret_cast<const T*>(reinterpret_cast<const char*>(p)+offset); }
template <typename T> inline       T* adjust_ptr(      void* p, std::ptrdiff_t offset) { return  reinterpret_cast<      T*>(reinterpret_cast<      char*>(p)+offset); }

//------------------------------------------------------------------------------

template <typename T> inline const T* addr(const T* t) { return  t; }
template <typename T> inline       T* addr(      T* t) { return  t; }
template <typename T> inline const T* addr(const T& t) { return &t; }
template <typename T> inline       T* addr(      T& t) { return &t; }

//------------------------------------------------------------------------------

template <typename T, typename U> inline const T* stat_cast(const U* p) { return static_cast<const T*>(p); }
template <typename T, typename U> inline       T* stat_cast(      U* p) { return static_cast<      T*>(p); }

//------------------------------------------------------------------------------

template <typename T> const std::type_info& vtbl_typeid(std::intptr_t vtbl) 
{
#ifdef _MSC_VER
    // FIX: The SEH handler below works in debug but not in release builds
    __try { return typeid(*reinterpret_cast<const T*>(&vtbl)); }
    __except(EXCEPTION_EXECUTE_HANDLER) { return typeid(void); }
    XTL_UNUSED(vtbl);
    return typeid(void);
#else
    try 
    { 
        XTL_WARNING_PUSH;
        XTL_WARNING_IGNORE_STRICT_ALIASING;
        return typeid(*reinterpret_cast<const T*>(&vtbl)); 
        XTL_WARNING_POP
    }
    catch (...)
    {
        return typeid(void); 
    }
#endif
}

template <typename T> 
inline const std::type_info& vtbl_typeid(const void* p) 
{
    return vtbl_typeid<T>(*reinterpret_cast<const std::intptr_t*>(p)); 
}

struct polymorphic_dummy { virtual ~polymorphic_dummy(){} };

inline const std::type_info& vtbl_typeid(std::intptr_t vtbl) { return vtbl_typeid<polymorphic_dummy>(vtbl); }
inline const std::type_info& vtbl_typeid(const void* p)      { return vtbl_typeid<polymorphic_dummy>(p); }

//------------------------------------------------------------------------------

template <size_t N> struct requires_bits_    { enum { value = requires_bits_<(N+1)/2>::value+1 }; };
template <>         struct requires_bits_<1> { enum { value = 0 }; };
template <>         struct requires_bits_<0> { enum { value = 0 }; };

/// Returns the amount of bits required to represent a given number.
/// Compile-time version of #req_bits
template <size_t N> struct requires_bits     { enum { value = requires_bits_<(N+1)>::value }; };
template <>         struct requires_bits<0>  { enum { value = 1 }; };

//------------------------------------------------------------------------------

/// Finds the number of trailing zeros in v.
/// The following code to count trailing zeros was taken from:
/// http://graphics.stanford.edu/~seander/bithacks.html#ZerosOnRightFloatCast
inline uint32_t trailing_zeros(uint32_t v)
{
    static_assert(sizeof(v) == sizeof(float), "trailing_zeros function assumes float to be of same size as uint32_t");
#ifdef _MSC_VER
  #pragma warning( push )
  #pragma warning( disable : 4146 ) // warning C4146: unary minus operator applied to unsigned type, result still unsigned
#endif
    float  f = (float)(v & -v); // cast the least significant bit in v to a float
    XTL_WARNING_PUSH;
    XTL_WARNING_IGNORE_STRICT_ALIASING;
    return (*(uint32_t *)&f >> 23) - 0x7f; // the result goes here
    XTL_WARNING_POP
#ifdef _MSC_VER
  #pragma warning( pop )
#endif
}

//------------------------------------------------------------------------------

/// Counts the number of bits set in v (the Brian Kernighan's way)
/// The following code to count set bits was taken from:
/// http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetKernighan
inline unsigned int bits_set(std::intptr_t v)
{
    unsigned int c = 0; // c accumulates the total bits set in v

    for (; v; c++)
        v &= v - 1; // clear the least significant bit set

    return c;
}

//------------------------------------------------------------------------------

/// Returns the amount of bits required to represent a given number.
/// FIX: Optimize this draft function
inline size_t req_bits(size_t v)
{
    size_t r = 1;   // r-1 will be lg(v)

    while (v >>= 1) // unroll for more speed...
        r++;

    return r;
};

//------------------------------------------------------------------------------

/// Interleaves bits of two numbers (aka Morton numbers).
/// The following code to interleave bits was taken from:
/// http://graphics.stanford.edu/~seander/bithacks.html#InterleaveTableObvious
inline uint32_t interleave(
    uint32_t x, ///< Interleave lower 16 bits of x and y, so the bits of x
    uint32_t y  ///< are in the even positions and bits from y in the odd;
)
{
    // x and y must initially be less than 65536.
    x &= 0xFFFF;
    y &= 0xFFFF;

    x = (x | (x << 8)) & 0x00FF00FF;
    x = (x | (x << 4)) & 0x0F0F0F0F;
    x = (x | (x << 2)) & 0x33333333;
    x = (x | (x << 1)) & 0x55555555;

    y = (y | (y << 8)) & 0x00FF00FF;
    y = (y | (y << 4)) & 0x0F0F0F0F;
    y = (y | (y << 2)) & 0x33333333;
    y = (y | (y << 1)) & 0x55555555;

    return x | (y << 1); // the resulting 32-bit Morton Number.  
};

//------------------------------------------------------------------------------

/// Interleaves bits of three numbers (aka Morton numbers).
/// The following code to interleave bits was taken from:
/// http://stackoverflow.com/questions/1024754/how-to-compute-a-3d-morton-number-interleave-the-bits-of-3-ints
inline uint32_t interleave(
    uint32_t x, ///< Interleave lower 10 bits of x, y and z, so the bits
    uint32_t y, ///< of x are in the mod 0 positions, bits from y in mod 1
    uint32_t z  ///< and bits from z in mod 2 positions;
)
{
    x &= 0x03FF;
    y &= 0x03FF;
    z &= 0x03FF;

    x = (x | (x << 16)) & 0x030000FF;
    x = (x | (x <<  8)) & 0x0300F00F;
    x = (x | (x <<  4)) & 0x030C30C3;
    x = (x | (x <<  2)) & 0x09249249;

    y = (y | (y << 16)) & 0x030000FF;
    y = (y | (y <<  8)) & 0x0300F00F;
    y = (y | (y <<  4)) & 0x030C30C3;
    y = (y | (y <<  2)) & 0x09249249;

    z = (z | (z << 16)) & 0x030000FF;
    z = (z | (z <<  8)) & 0x0300F00F;
    z = (z | (z <<  4)) & 0x030C30C3;
    z = (z | (z <<  2)) & 0x09249249;

    return x | (y << 1) | (z << 2);
}

//------------------------------------------------------------------------------

/// Interleaves bits of N numbers (aka Morton numbers).
/// FIX: Specialize for the 3 faster interleaves you have
/// FIX: Replace condition on j to better handle odd numbers.
template <typename T, unsigned int N>
inline T interleave(const T (&vtbl)[N])
{
    T result = 0;
    T bit    = 1;

    for (size_t i = 0, j = 0; i < sizeof(T)*8/N; ++i, bit <<= 1)
        for (size_t n = 0; n < N; ++n)
            result |= ((vtbl[n] & bit) >> i) << j++;

    return result;
}

//------------------------------------------------------------------------------
