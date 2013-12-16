///
/// \file ptrtools.hpp
///
/// This file defines several utility functions for working with pointers,
/// especially pointers to (polymorphic) objects, that we use in XTL
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of Mach7 library (http://parasol.tamu.edu/mach7/).
/// Copyright (C) 2011-2013 Texas A&M University.
/// All rights reserved.
///

#pragma once

#include "config.hpp"
#include <cstddef>
#include <memory>
#include <typeinfo>
#include <type_traits>

#if defined(_MSC_VER)
    #include <excpt.h>
#endif
#if !defined(_MSC_VER) || _MSC_VER >= 1600 
    #include <cstdint>
#else
    // FIX: These are all workarounds to support earlier versions of compilers
    //      for the type switching functionality only, which does not require
    //      C++11, only variadic macros.
    #include <crtdefs.h>
    namespace std { typedef ::intptr_t intptr_t; }
    typedef uintptr_t  uint32_t; // FIX: workaround for now
#endif

#if defined(__GNUC__) && XTL_GCC_VERSION < 40600
namespace std
{
    /// GCC of some version prior to 4.6.1 (not sure which exactly) does not 
    /// provide std::addressof. The following workaround is borrowed from:
    /// http://en.cppreference.com/w/cpp/memory/addressof
    template <typename T>
    T* addressof(T& arg) 
    {
        return reinterpret_cast<T*>(&const_cast<char&>(reinterpret_cast<const volatile char&>(arg)));
    }
} // of namespace std
#endif

//------------------------------------------------------------------------------

template <typename T> inline const T* adjust_ptr(const void* p, std::ptrdiff_t offset) { return  reinterpret_cast<const T*>(reinterpret_cast<const char*>(p)+offset); }
template <typename T> inline       T* adjust_ptr(      void* p, std::ptrdiff_t offset) { return  reinterpret_cast<      T*>(reinterpret_cast<      char*>(p)+offset); }

//------------------------------------------------------------------------------

template <typename T> inline const T* addr(const T* t) { return t; }
template <typename T> inline       T* addr(      T* t) { return t; }
template <typename T> inline const T* addr(const T& t) { return std::addressof(t); }
template <typename T> inline       T* addr(      T& t) { return std::addressof(t); }

//------------------------------------------------------------------------------

template <typename T, typename U> inline const T* stat_cast(const U* p) { return static_cast<const T*>(p); }
template <typename T, typename U> inline       T* stat_cast(      U* p) { return static_cast<      T*>(p); }

//------------------------------------------------------------------------------

template <typename T> const std::type_info& vtbl_typeid(std::intptr_t vtbl) 
{
#ifdef _MSC_VER
    // FIX: The SEH handler below works in debug but not in release builds
    //__try { return typeid(*reinterpret_cast<const T*>(&vtbl)); }
    //__except(EXCEPTION_EXECUTE_HANDLER) { return typeid(void); }
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

template <size_t N> struct requires_bits_    { static const size_t value = requires_bits_<(N+1)/2>::value+1; };
template <>         struct requires_bits_<1> { static const size_t value = 0; };
template <>         struct requires_bits_<0> { static const size_t value = 0; };

/// Returns the amount of bits required to represent a given number.
/// Compile-time version of #req_bits
template <size_t N> struct requires_bits     { static const size_t value = requires_bits_<(N+1)>::value; };
template <>         struct requires_bits<0>  { static const size_t value = 1; };

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
}

//------------------------------------------------------------------------------
