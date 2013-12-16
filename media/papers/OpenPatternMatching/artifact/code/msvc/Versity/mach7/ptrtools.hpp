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
#include <typeinfo>
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

namespace mch ///< Mach7 library namespace
{

//------------------------------------------------------------------------------

/// Generic function that obtains a value of a primary vtbl-pointer from a 
/// polymorphic object, pointed to by p.
/// \note This default implementation assumes the vtbl-pointer is located at 
///       offset 0 from the address, pointed to by p and the user is responsible
///       to overload this function for class hierarchies, where this is not the case.
inline std::intptr_t vtbl_of(const void* p)
{
    return *reinterpret_cast<const std::intptr_t*>(p);
}

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

#if 0

template <typename dummy>
struct MortonTable256
{
    static const unsigned short array[256];
};

//------------------------------------------------------------------------------

template <typename dummy>
const unsigned short MortonTable256<dummy>::array[256]    =
{
    0x0000, 0x0001, 0x0004, 0x0005, 0x0010, 0x0011, 0x0014, 0x0015, 
    0x0040, 0x0041, 0x0044, 0x0045, 0x0050, 0x0051, 0x0054, 0x0055, 
    0x0100, 0x0101, 0x0104, 0x0105, 0x0110, 0x0111, 0x0114, 0x0115, 
    0x0140, 0x0141, 0x0144, 0x0145, 0x0150, 0x0151, 0x0154, 0x0155, 
    0x0400, 0x0401, 0x0404, 0x0405, 0x0410, 0x0411, 0x0414, 0x0415, 
    0x0440, 0x0441, 0x0444, 0x0445, 0x0450, 0x0451, 0x0454, 0x0455, 
    0x0500, 0x0501, 0x0504, 0x0505, 0x0510, 0x0511, 0x0514, 0x0515, 
    0x0540, 0x0541, 0x0544, 0x0545, 0x0550, 0x0551, 0x0554, 0x0555, 
    0x1000, 0x1001, 0x1004, 0x1005, 0x1010, 0x1011, 0x1014, 0x1015, 
    0x1040, 0x1041, 0x1044, 0x1045, 0x1050, 0x1051, 0x1054, 0x1055, 
    0x1100, 0x1101, 0x1104, 0x1105, 0x1110, 0x1111, 0x1114, 0x1115, 
    0x1140, 0x1141, 0x1144, 0x1145, 0x1150, 0x1151, 0x1154, 0x1155, 
    0x1400, 0x1401, 0x1404, 0x1405, 0x1410, 0x1411, 0x1414, 0x1415, 
    0x1440, 0x1441, 0x1444, 0x1445, 0x1450, 0x1451, 0x1454, 0x1455, 
    0x1500, 0x1501, 0x1504, 0x1505, 0x1510, 0x1511, 0x1514, 0x1515, 
    0x1540, 0x1541, 0x1544, 0x1545, 0x1550, 0x1551, 0x1554, 0x1555, 
    0x4000, 0x4001, 0x4004, 0x4005, 0x4010, 0x4011, 0x4014, 0x4015, 
    0x4040, 0x4041, 0x4044, 0x4045, 0x4050, 0x4051, 0x4054, 0x4055, 
    0x4100, 0x4101, 0x4104, 0x4105, 0x4110, 0x4111, 0x4114, 0x4115, 
    0x4140, 0x4141, 0x4144, 0x4145, 0x4150, 0x4151, 0x4154, 0x4155, 
    0x4400, 0x4401, 0x4404, 0x4405, 0x4410, 0x4411, 0x4414, 0x4415, 
    0x4440, 0x4441, 0x4444, 0x4445, 0x4450, 0x4451, 0x4454, 0x4455, 
    0x4500, 0x4501, 0x4504, 0x4505, 0x4510, 0x4511, 0x4514, 0x4515, 
    0x4540, 0x4541, 0x4544, 0x4545, 0x4550, 0x4551, 0x4554, 0x4555, 
    0x5000, 0x5001, 0x5004, 0x5005, 0x5010, 0x5011, 0x5014, 0x5015, 
    0x5040, 0x5041, 0x5044, 0x5045, 0x5050, 0x5051, 0x5054, 0x5055, 
    0x5100, 0x5101, 0x5104, 0x5105, 0x5110, 0x5111, 0x5114, 0x5115, 
    0x5140, 0x5141, 0x5144, 0x5145, 0x5150, 0x5151, 0x5154, 0x5155, 
    0x5400, 0x5401, 0x5404, 0x5405, 0x5410, 0x5411, 0x5414, 0x5415, 
    0x5440, 0x5441, 0x5444, 0x5445, 0x5450, 0x5451, 0x5454, 0x5455, 
    0x5500, 0x5501, 0x5504, 0x5505, 0x5510, 0x5511, 0x5514, 0x5515, 
    0x5540, 0x5541, 0x5544, 0x5545, 0x5550, 0x5551, 0x5554, 0x5555
};

//------------------------------------------------------------------------------

/// Interleaves bits of two numbers (aka Morton numbers).
/// The following code to interleave bits was taken from:
/// http://graphics.stanford.edu/~seander/bithacks.html#InterleaveTableLookup
inline uint32_t interleave(
    uint16_t x, ///< Interleave lower 16 bits of x and y, so the bits of x
    uint16_t y  ///< are in the even positions and bits from y in the odd;
)
{
    return  MortonTable256<void>::array[y >> 8]   << 17 | 
            MortonTable256<void>::array[x >> 8]   << 16 |
            MortonTable256<void>::array[y & 0xFF] <<  1 | 
            MortonTable256<void>::array[x & 0xFF];
}

#else

//------------------------------------------------------------------------------

/// Interleaves bits of two numbers (aka Morton numbers).
/// The following code to interleave bits was taken from:
/// http://graphics.stanford.edu/~seander/bithacks.html#InterleaveBMN
inline uint32_t interleave(
    uint16_t x, ///< Interleave lower 16 bits of x and y, so the bits of x
    uint16_t y  ///< are in the even positions and bits from y in the odd;
)
{
    x = (x | (x << 8)) & 0x00FF00FF;
    x = (x | (x << 4)) & 0x0F0F0F0F;
    x = (x | (x << 2)) & 0x33333333;
    x = (x | (x << 1)) & 0x55555555;

    y = (y | (y << 8)) & 0x00FF00FF;
    y = (y | (y << 4)) & 0x0F0F0F0F;
    y = (y | (y << 2)) & 0x33333333;
    y = (y | (y << 1)) & 0x55555555;

    return x | (y << 1); // the resulting 32-bit Morton Number.  
}

#endif

//------------------------------------------------------------------------------

/// Interleaves bits of three numbers (aka Morton numbers).
/// The following code to interleave bits was taken from:
/// http://stackoverflow.com/questions/1024754/how-to-compute-a-3d-morton-number-interleave-the-bits-of-3-ints
inline uint32_t interleave(
    uint16_t x, ///< Interleave lower 10 bits of x, y and z, so the bits
    uint16_t y, ///< of x are in the mod 0 positions, bits from y in mod 1
    uint16_t z  ///< and bits from z in mod 2 positions;
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
/// FIX: Replace condition on j to better handle odd numbers.
template <typename T, size_t N>
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

inline intptr_t interleave(const intptr_t (&vtbl)[1]) { return vtbl[0]; }
inline intptr_t interleave(const intptr_t (&vtbl)[2]) { return interleave(vtbl[0],vtbl[1]); }
inline intptr_t interleave(const intptr_t (&vtbl)[3]) { return interleave(vtbl[0],vtbl[1],vtbl[2]); }
inline intptr_t interleave(const intptr_t (&vtbl)[4]) { return interleave(interleave(vtbl[0],vtbl[1]),interleave(vtbl[2],vtbl[3])); }

//------------------------------------------------------------------------------

} // of namespace mch
