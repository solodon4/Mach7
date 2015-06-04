//
//  Mach7: Pattern Matching Library for C++
//
//  Copyright 2011-2013, Texas A&M University.
//  Copyright 2014 Yuriy Solodkyy.
//  All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//
//      * Redistributions of source code must retain the above copyright
//        notice, this list of conditions and the following disclaimer.
//
//      * Redistributions in binary form must reproduce the above copyright
//        notice, this list of conditions and the following disclaimer in the
//        documentation and/or other materials provided with the distribution.
//
//      * Neither the names of Mach7 project nor the names of its contributors
//        may be used to endorse or promote products derived from this software
//        without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
//  IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY
//  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
//  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

///
/// \file
///
/// This file defines types and routines for platform-independent time measuring.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// \see https://parasol.tamu.edu/mach7/
/// \see https://github.com/solodon4/Mach7
/// \see https://github.com/solodon4/SELL
///

#pragma once

// This one is for pure convenience to let us choose which method to measure
// with when several are available.
#if (!defined(XTL_TIMING_METHOD_1) && !defined(XTL_TIMING_METHOD_2) && !defined(XTL_TIMING_METHOD_3))
    #if   defined(_MSC_VER)
      #if defined(_MSC_EXTENSIONS)
        /// This timing method would work on Windows systems starting from Windows 2000
        #define XTL_TIMING_METHOD_1
      #else
        /// TODO: Get MSVC equivalent of RDTSC asembly to avoid inclusion of windows headers incompatible with /Za
        #define XTL_TIMING_METHOD_3
      #endif
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

namespace mch ///< Mach7 library namespace
{
    /// The type used to record a time stamp. The type must have operator- defined.
    typedef LONGLONG  time_stamp;
    /// The type capable of holding a difference of two time stamps.
    typedef LONGLONG  time_stamp_diff;
    /// Returns a current time stamp.
    inline time_stamp get_time_stamp() { LARGE_INTEGER li; QueryPerformanceCounter(&li);   return li.QuadPart; }
    /// Returns how many time stamps are there in 1 second (frequency).
    inline time_stamp get_frequency()  { LARGE_INTEGER li; QueryPerformanceFrequency(&li); return li.QuadPart; }
    /// Estimates the number of cycles in a given time_stamp_diff value
    /// FIX: 1000 is heuristic here for my laptop. QueryPerformanceCounter doesn't specify how it is related to cycles!
    inline time_stamp_diff cycles(time_stamp_diff tsd)  { return tsd*1000; }

} // of namespace mch

#elif defined(XTL_TIMING_METHOD_2)

    #include <stdint.h>
    #include <unistd.h>

namespace mch ///< Mach7 library namespace
{

#if 0
    /// The type used to record a time stamp. The type must have operator- defined.
    typedef unsigned long long int time_stamp;
    /// The type capable of holding a difference of two time stamps.
    typedef   signed long long int time_stamp_diff;
    // The following code is taken from: http://en.wikipedia.org/wiki/Time_Stamp_Counter
    extern "C" {
        inline unsigned long long int rdtsc(void)
        {
            unsigned long long int x;
            __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
            return x;
        }
    }
#else
    /// The type used to record a time stamp. The type must have operator- defined.
    typedef uint64_t  time_stamp;
    /// The type capable of holding a difference of two time stamps.
    typedef  int64_t  time_stamp_diff;
    // The following code is taken from: http://en.wikipedia.org/wiki/Time_Stamp_Counter
    extern "C" {
        inline uint64_t rdtsc() {
        uint32_t lo, hi;
        __asm__ __volatile__ (      // serialize
        "xorl %%eax,%%eax \n        cpuid"
        ::: "%rax", "%rbx", "%rcx", "%rdx");
        /* We cannot use "=A", since this would use %rax on x86_64 and return only the lower 32bits of the TSC */
        __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
        return (uint64_t)hi << 32 | lo;
      }
    }
#endif
    /// Returns a current time stamp.
    inline time_stamp get_time_stamp() { return rdtsc(); }
    /// Returns how many time stamps are there in 1 second (frequency).
    inline time_stamp get_frequency()
    {
        time_stamp cycles[2];
        cycles[0] = rdtsc();
        usleep(100000); // Sleep for 1/10th of a second
        cycles[1] = rdtsc();
        return 10*(cycles[1]-cycles[0]);
    }
    /// Estimates the number of cycles in a given time_stamp_diff value
    inline time_stamp_diff cycles(time_stamp_diff tsd)  { return tsd; }

} // of namespace mch

#elif defined(XTL_TIMING_METHOD_3)

    #include <time.h>

namespace mch ///< Mach7 library namespace
{
    /// The type used to record a time stamp. The type must have operator- defined.
    typedef clock_t   time_stamp;
    /// The type capable of holding a difference of two time stamps.
    typedef clock_t   time_stamp_diff;
    /// Returns a current time stamp.
    inline time_stamp get_time_stamp() { return clock(); }
    /// Returns how many time stamps are there in 1 second (frequency).
    inline time_stamp get_frequency()  { return CLOCKS_PER_SEC; }
    /// Estimates the number of cycles in a given time_stamp_diff value
    /// FIX: Not implemented!
    inline time_stamp_diff cycles(time_stamp_diff tsd)  { return tsd; }
} // of namespace mch

#else

    #error Timing method has not been chosen

#endif

namespace mch ///< Mach7 library namespace
{

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

} // of namespace mch
