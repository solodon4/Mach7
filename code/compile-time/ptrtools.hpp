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

template <typename T, typename S> inline auto adjust_ptr_if_polymorphic(const S* p, std::ptrdiff_t offset) -> typename std::enable_if< std::is_polymorphic<S>::value,const T*>::type { return  reinterpret_cast<const T*>(reinterpret_cast<const char*>(p)+offset); }
template <typename T, typename S> inline auto adjust_ptr_if_polymorphic(const S* p, std::ptrdiff_t       ) -> typename std::enable_if<!std::is_polymorphic<S>::value,const T*>::type { return  reinterpret_cast<const T*>(reinterpret_cast<const char*>(p)); }
template <typename T, typename S> inline auto adjust_ptr_if_polymorphic(      S* p, std::ptrdiff_t offset) -> typename std::enable_if< std::is_polymorphic<S>::value,      T*>::type { return  reinterpret_cast<      T*>(reinterpret_cast<      char*>(p)+offset); }
template <typename T, typename S> inline auto adjust_ptr_if_polymorphic(      S* p, std::ptrdiff_t       ) -> typename std::enable_if<!std::is_polymorphic<S>::value,      T*>::type { return  reinterpret_cast<      T*>(reinterpret_cast<      char*>(p)); }

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

/// A dummy template class to let the linker unify all the instances of the same 
/// table without us having to bother to keep a dedicated .cpp file around.
/// All the tables are spreadNxM where N is number of input bits and M is 
/// the number of bits to spread by e.g. 2 means make it every 2nd bit
template <typename dummy = void>
struct morton
{
    static const uint32_t spread8x4[256];
    static const uint16_t spread8x2[256];
    static const uint16_t spread4x4[16];
    static const uint16_t spread4x2[16];
};

//------------------------------------------------------------------------------

template <typename _>
const uint32_t morton<_>::spread8x4[256] =
{
    0x00000000, 0x00000001, 0x00000010, 0x00000011, 0x00000100, 0x00000101, 0x00000110, 0x00000111,
    0x00001000, 0x00001001, 0x00001010, 0x00001011, 0x00001100, 0x00001101, 0x00001110, 0x00001111,
    0x00010000, 0x00010001, 0x00010010, 0x00010011, 0x00010100, 0x00010101, 0x00010110, 0x00010111,
    0x00011000, 0x00011001, 0x00011010, 0x00011011, 0x00011100, 0x00011101, 0x00011110, 0x00011111,
    0x00100000, 0x00100001, 0x00100010, 0x00100011, 0x00100100, 0x00100101, 0x00100110, 0x00100111,
    0x00101000, 0x00101001, 0x00101010, 0x00101011, 0x00101100, 0x00101101, 0x00101110, 0x00101111,
    0x00110000, 0x00110001, 0x00110010, 0x00110011, 0x00110100, 0x00110101, 0x00110110, 0x00110111,
    0x00111000, 0x00111001, 0x00111010, 0x00111011, 0x00111100, 0x00111101, 0x00111110, 0x00111111,
    0x01000000, 0x01000001, 0x01000010, 0x01000011, 0x01000100, 0x01000101, 0x01000110, 0x01000111,
    0x01001000, 0x01001001, 0x01001010, 0x01001011, 0x01001100, 0x01001101, 0x01001110, 0x01001111,
    0x01010000, 0x01010001, 0x01010010, 0x01010011, 0x01010100, 0x01010101, 0x01010110, 0x01010111,
    0x01011000, 0x01011001, 0x01011010, 0x01011011, 0x01011100, 0x01011101, 0x01011110, 0x01011111,
    0x01100000, 0x01100001, 0x01100010, 0x01100011, 0x01100100, 0x01100101, 0x01100110, 0x01100111,
    0x01101000, 0x01101001, 0x01101010, 0x01101011, 0x01101100, 0x01101101, 0x01101110, 0x01101111,
    0x01110000, 0x01110001, 0x01110010, 0x01110011, 0x01110100, 0x01110101, 0x01110110, 0x01110111,
    0x01111000, 0x01111001, 0x01111010, 0x01111011, 0x01111100, 0x01111101, 0x01111110, 0x01111111,
    0x10000000, 0x10000001, 0x10000010, 0x10000011, 0x10000100, 0x10000101, 0x10000110, 0x10000111,
    0x10001000, 0x10001001, 0x10001010, 0x10001011, 0x10001100, 0x10001101, 0x10001110, 0x10001111,
    0x10010000, 0x10010001, 0x10010010, 0x10010011, 0x10010100, 0x10010101, 0x10010110, 0x10010111,
    0x10011000, 0x10011001, 0x10011010, 0x10011011, 0x10011100, 0x10011101, 0x10011110, 0x10011111,
    0x10100000, 0x10100001, 0x10100010, 0x10100011, 0x10100100, 0x10100101, 0x10100110, 0x10100111,
    0x10101000, 0x10101001, 0x10101010, 0x10101011, 0x10101100, 0x10101101, 0x10101110, 0x10101111,
    0x10110000, 0x10110001, 0x10110010, 0x10110011, 0x10110100, 0x10110101, 0x10110110, 0x10110111,
    0x10111000, 0x10111001, 0x10111010, 0x10111011, 0x10111100, 0x10111101, 0x10111110, 0x10111111,
    0x11000000, 0x11000001, 0x11000010, 0x11000011, 0x11000100, 0x11000101, 0x11000110, 0x11000111,
    0x11001000, 0x11001001, 0x11001010, 0x11001011, 0x11001100, 0x11001101, 0x11001110, 0x11001111,
    0x11010000, 0x11010001, 0x11010010, 0x11010011, 0x11010100, 0x11010101, 0x11010110, 0x11010111,
    0x11011000, 0x11011001, 0x11011010, 0x11011011, 0x11011100, 0x11011101, 0x11011110, 0x11011111,
    0x11100000, 0x11100001, 0x11100010, 0x11100011, 0x11100100, 0x11100101, 0x11100110, 0x11100111,
    0x11101000, 0x11101001, 0x11101010, 0x11101011, 0x11101100, 0x11101101, 0x11101110, 0x11101111,
    0x11110000, 0x11110001, 0x11110010, 0x11110011, 0x11110100, 0x11110101, 0x11110110, 0x11110111,
    0x11111000, 0x11111001, 0x11111010, 0x11111011, 0x11111100, 0x11111101, 0x11111110, 0x11111111,
};

//------------------------------------------------------------------------------

template <typename _>
const uint16_t morton<_>::spread8x2[256] =
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

template <typename _>
const uint16_t morton<_>::spread4x4[16] =
{
    0x0000, 0x0001, 0x0010, 0x0011, 0x0100, 0x0101, 0x0110, 0x0111,
    0x1000, 0x1001, 0x1010, 0x1011, 0x1100, 0x1101, 0x1110, 0x1111,
};

//------------------------------------------------------------------------------

template <typename _>
const uint16_t morton<_>::spread4x2[16] =
{
    0x0000, 0x0001, 0x0004, 0x0005, 0x0010, 0x0011, 0x0014, 0x0015, 
    0x0040, 0x0041, 0x0044, 0x0045, 0x0050, 0x0051, 0x0054, 0x0055, 
};

//------------------------------------------------------------------------------

/// Interleaves bits of two numbers (aka Morton numbers).
/// The following code to interleave bits was taken from:
/// http://graphics.stanford.edu/~seander/bithacks.html#InterleaveBMN
inline uint32_t interleave(
    register uint32_t x, ///< Interleave lower 16 bits of x and y, so the bits of x
    register uint32_t y  ///< are in the even positions and bits from y in the odd;
)
{
    x &= 0xFFFF;
    y &= 0xFFFF;

    x = (x | (x << 8)) & 0x00FF00FF; // 00000000111111110000000011111111
    x = (x | (x << 4)) & 0x0F0F0F0F; // 00001111000011110000111100001111
    x = (x | (x << 2)) & 0x33333333; // 00110011001100110011001100110011
    x = (x | (x << 1)) & 0x55555555; // 01010101010101010101010101010101

    y = (y | (y << 8)) & 0x00FF00FF; // 00000000111111110000000011111111
    y = (y | (y << 4)) & 0x0F0F0F0F; // 00001111000011110000111100001111
    y = (y | (y << 2)) & 0x33333333; // 00110011001100110011001100110011
    y = (y | (y << 1)) & 0x55555555; // 01010101010101010101010101010101

    return x | (y << 1); // the resulting 32-bit Morton Number.  
}

//------------------------------------------------------------------------------

/// Interleaves bits of two numbers (aka Morton numbers).
/// The following code to interleave bits was taken from:
/// http://graphics.stanford.edu/~seander/bithacks.html#InterleaveTableLookup
inline uint32_t interleave8x2(
    register uint32_t x, ///< Interleave lower 16 bits of x and y, so the bits of x
    register uint32_t y  ///< are in the even positions and bits from y in the odd;
)
{
    return morton<>::spread8x2[x      & 0xFF]
         | morton<>::spread8x2[x >> 8 & 0xFF] << 16
         | morton<>::spread8x2[y      & 0xFF] <<  1
         | morton<>::spread8x2[y >> 8 & 0xFF] << 17 
         ;
}

//------------------------------------------------------------------------------

inline uint32_t interleave4x2(
    register uint32_t x, ///< Interleave lower 16 bits of x and y, so the bits of x
    register uint32_t y  ///< are in the even positions and bits from y in the odd;
)
{
    return morton<>::spread4x2[x      & 0x0F] 
         | morton<>::spread4x2[x >> 4 & 0x0F] << 8 
         | morton<>::spread4x2[x >> 8 & 0x0F] << 16 
         | morton<>::spread4x2[x >>12 & 0x0F] << 24
         | morton<>::spread4x2[y      & 0x0F] << 1
         | morton<>::spread4x2[y >> 4 & 0x0F] << 9 
         | morton<>::spread4x2[y >> 8 & 0x0F] << 17 
         | morton<>::spread4x2[y >>12 & 0x0F] << 25
         ;
}

//------------------------------------------------------------------------------

/// Interleaves bits of three numbers (aka Morton numbers).
/// The following code to interleave bits was taken from:
/// http://stackoverflow.com/questions/1024754/how-to-compute-a-3d-morton-number-interleave-the-bits-of-3-ints
inline uint32_t interleave(
    register uint32_t x, ///< Interleave lower 10 bits of x, y and z, so the bits
    register uint32_t y, ///< of x are in the mod 0 positions, bits from y in mod 1
    register uint32_t z  ///< and bits from z in mod 2 positions;
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

inline uint32_t interleave8x4(
    register uint32_t x,
    register uint32_t y,
    register uint32_t z,
    register uint32_t w
)
{
    return morton<>::spread8x4[x      & 0xFF] 
         | morton<>::spread8x4[y      & 0xFF] << 1
         | morton<>::spread8x4[z      & 0xFF] << 2 
         | morton<>::spread8x4[w      & 0xFF] << 3
         ;
}

//------------------------------------------------------------------------------

inline uint32_t interleave4x4(
    register uint32_t x,
    register uint32_t y,
    register uint32_t z,
    register uint32_t w
)
{
    return morton<>::spread4x4[x      & 0x0F] 
         | morton<>::spread4x4[x >> 4 & 0x0F] << 16     
         | morton<>::spread4x4[y      & 0x0F] << 1
         | morton<>::spread4x4[y >> 4 & 0x0F] << 17
         | morton<>::spread4x4[z      & 0x0F] << 2
         | morton<>::spread4x4[z >> 4 & 0x0F] << 18 
         | morton<>::spread4x4[w      & 0x0F] << 3
         | morton<>::spread4x4[w >> 4 & 0x0F] << 19
         ;
}

//------------------------------------------------------------------------------

/// Interleaves bits of two numbers (aka Morton numbers).
/// The following code to interleave bits was taken from:
/// http://graphics.stanford.edu/~seander/bithacks.html#InterleaveBMN
inline uint32_t interleave(
    register uint32_t x,
    register uint32_t y,
    register uint32_t z,
    register uint32_t w
)
{
    x &= 0xFF;                        // x = 00000000 00000000 00000000 ABCDEFGH
    y &= 0xFF;                        // y = 00000000 00000000 00000000 ABCDEFGH
    z &= 0xFF;                        // z = 00000000 00000000 00000000 ABCDEFGH
    w &= 0xFF;                        // w = 00000000 00000000 00000000 ABCDEFGH

    x = (x | (x << 12)) & 0x000F000F; // x = 00000000 0000ABCD 00000000 0000EFGH
    y = (y | (y << 12)) & 0x000F000F; // y = 00000000 0000ABCD 00000000 0000EFGH
    z = (z | (z << 12)) & 0x000F000F; // z = 00000000 0000ABCD 00000000 0000EFGH
    w = (w | (w << 12)) & 0x000F000F; // w = 00000000 0000ABCD 00000000 0000EFGH

    x = (x | (x <<  6)) & 0x03030303; // x = 000000AB 000000CD 000000EF 000000GH 
    y = (y | (y <<  6)) & 0x03030303; // y = 000000AB 000000CD 000000EF 000000GH 
    z = (z | (z <<  6)) & 0x03030303; // z = 000000AB 000000CD 000000EF 000000GH 
    w = (w | (w <<  6)) & 0x03030303; // w = 000000AB 000000CD 000000EF 000000GH 

    x = (x | (x <<  3)) & 0x11111111; // x = 000A000B 000C000D 000E000F 000G000H
    y = (y | (y <<  3)) & 0x11111111; // y = 000A000B 000C000D 000E000F 000G000H
    z = (z | (z <<  3)) & 0x11111111; // z = 000A000B 000C000D 000E000F 000G000H
    w = (w | (w <<  3)) & 0x11111111; // w = 000A000B 000C000D 000E000F 000G000H

    return x | (y << 1) | (z << 2) | (w << 3); // the resulting 32-bit Morton Number.  
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

// Current tests for the choice of interleave implementation for compilers:
//       GCC 4.5.2:                 GCC 4.6.1:                 Visual C++ 10:             Visual C++ 11:                     
// _x2 - 53% faster V= 65 M= 42   - 45% faster V= 55 M= 38   -  9% faster V= 51 M= 47   - 13% faster V= 55 M= 48  
// 8x2 - 58% faster V= 61 M= 39 * - 64% faster V= 56 M= 34 * - 13% faster V= 50 M= 44 * - 17% faster V= 52 M= 44 * // Fastest for all 4
// 4x2 - 50% faster V= 62 M= 41   - 59% faster V= 59 M= 37   -  9% faster V= 50 M= 46   -  3% faster V= 52 M= 50   

// _x4 - 54% faster V= 64 M= 41   - 89% slower V= 58 M=111   -256% slower V= 54 M=194 * -102% slower V= 54 M=110  
// 8x4 - 56% faster V= 61 M= 39 * - 68% slower V= 60 M=101   -306% slower V= 54 M=222 + - 70% slower V= 53 M= 92 *
// 4x4 - 48% faster V= 65 M= 44   - 59% faster V= 60 M= 37 * -866% slower V= 52 M=508   -107% slower V= 55 M=114  

inline intptr_t interleave(const intptr_t (&vtbl)[1]) { return vtbl[0]; }
inline intptr_t interleave(const intptr_t (&vtbl)[2]) { return interleave8x2(vtbl[0],vtbl[1]); }
inline intptr_t interleave(const intptr_t (&vtbl)[3]) { return interleave(vtbl[0],vtbl[1],vtbl[2]); }
#if defined(__GNUC__)
inline intptr_t interleave(const intptr_t (&vtbl)[4]) { return interleave4x4(vtbl[0],vtbl[1],vtbl[2],vtbl[3]); }
#elif defined(_MSC_VER)
inline intptr_t interleave(const intptr_t (&vtbl)[4]) { return interleave8x4(vtbl[0],vtbl[1],vtbl[2],vtbl[3]); }
#else
inline intptr_t interleave(const intptr_t (&vtbl)[4]) { return interleave(vtbl[0],vtbl[1],vtbl[2],vtbl[3]); }
//inline intptr_t interleave(const intptr_t (&vtbl)[4]) { return interleave(interleave(vtbl[0],vtbl[1]),interleave(vtbl[2],vtbl[3])); }
#endif

//------------------------------------------------------------------------------

} // of namespace mch
