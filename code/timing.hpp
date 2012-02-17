///
/// \file timing.hpp
///
/// This file defines types and routines for platform-independent time measuring.
///
/// \autor Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of the XTL framework (http://parasol.tamu.edu/xtl/).
/// Copyright (C) 2005-2011 Texas A&M University.
/// All rights reserved.
///

#pragma once

// This one is for pure convenience to let us choose which method to measure
// with when several are available.
#if (!defined(XTL_TIMING_METHOD_1) && !defined(XTL_TIMING_METHOD_2) && !defined(XTL_TIMING_METHOD_3))
    #if   defined(_WIN32)
        /// This timing method would work on Windows systems starting from Windows 2000
        #define XTL_TIMING_METHOD_1
    #elif defined(__GNUC__)
        /// This timing method would work on any Pentium processor when compiled with GCC
        #define XTL_TIMING_METHOD_2
    #else
        /// This timing method is the least precise, relying only on standard functions from time.h
        #define XTL_TIMING_METHOD_3
    #endif
#endif

#if defined(XTL_TIMING_METHOD_1)
    #if !defined(NOMINMAX)
        #define  NOMINMAX
    #endif
    #include <windows.h>

    /// The type used to record a time stamp. The type must have operator- defined.
    typedef LONGLONG  time_stamp;
    /// The type capable of holding a difference of two time stamps.
    typedef LONGLONG  time_stamp_diff;
    /// Returns a current time stamp.
    inline time_stamp get_time_stamp() { LARGE_INTEGER li; QueryPerformanceCounter(&li);   return li.QuadPart; }
    /// Returns how many time stamps are there in 1 second (frequency).
    inline time_stamp get_frequency()  { LARGE_INTEGER li; QueryPerformanceFrequency(&li); return li.QuadPart; }

#elif defined(XTL_TIMING_METHOD_2)

    #include <stdint.h>

    // The following code is taken from: http://en.wikipedia.org/wiki/Time_Stamp_Counter
    extern "C" {
      __inline__ uint64_t rdtsc() {
        uint32_t lo, hi;
        __asm__ __volatile__ (      // serialize
        "xorl %%eax,%%eax \n        cpuid"
        ::: "%rax", "%rbx", "%rcx", "%rdx");
        /* We cannot use "=A", since this would use %rax on x86_64 and return only the lower 32bits of the TSC */
        __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
        return (uint64_t)hi << 32 | lo;
      }
    }
    /// The type used to record a time stamp. The type must have operator- defined.
    typedef uint64_t  time_stamp;
    /// The type capable of holding a difference of two time stamps.
    typedef  int64_t  time_stamp_diff;
    /// Returns a current time stamp.
    inline time_stamp get_time_stamp() { return rdtsc(); }
    /// Returns how many time stamps are there in 1 second (frequency).
    inline time_stamp get_frequency()  { return 2530000000; } // FIX: This ideally has to be MGz of computer we are running on

#elif defined(XTL_TIMING_METHOD_3)

    #include <time.h>

    /// The type used to record a time stamp. The type must have operator- defined.
    typedef clock_t   time_stamp;
    /// The type capable of holding a difference of two time stamps.
    typedef clock_t   time_stamp_diff;
    /// Returns a current time stamp.
    inline time_stamp get_time_stamp() { return clock(); }
    /// Returns how many time stamps are there in 1 second (frequency).
    inline time_stamp get_frequency()  { return CLOCKS_PER_SEC; }

#else

    #error Timing method has not been chosen

#endif

static const time_stamp time_stamp_frequency = get_frequency();

inline long long       seconds(const long long& l) { return l/time_stamp_frequency; }
inline long long  milliseconds(const long long& l) { return l*1000/time_stamp_frequency; }
inline long long  microseconds(const long long& l) { return l*1000000/time_stamp_frequency; }
inline long long   nanoseconds(const long long& l) { return l*1000000000/time_stamp_frequency; }

namespace dbl
{
    inline double      seconds(const long long& l) { return double(l)/time_stamp_frequency; }
    inline double milliseconds(const long long& l) { return double(l)*1000/time_stamp_frequency; }
    inline double microseconds(const long long& l) { return double(l)*1000000/time_stamp_frequency; }
    inline double  nanoseconds(const long long& l) { return double(l)*1000000000/time_stamp_frequency; }
}
