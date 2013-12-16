///
/// \file testutils.hpp
///
/// Various utility functions used in our synthetic tests.
///
/// \autor Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of the XTL framework (http://parasol.tamu.edu/xtl/).
/// Copyright (C) 2005-2012 Texas A&M University.
/// All rights reserved.
///

#pragma once

//------------------------------------------------------------------------------

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string>
#include <vector>
#include "config.hpp"
#include "timing.hpp"

//------------------------------------------------------------------------------

#if defined(XTL_TIMING_METHOD_1)
    XTL_MESSAGE("Timing method 1: based on QueryPerformanceCounter()")
#elif defined(XTL_TIMING_METHOD_2)
    XTL_MESSAGE("Timing method 2: based on rdtsc register")
#elif defined(XTL_TIMING_METHOD_3)
    XTL_MESSAGE("Timing method 3: based on clock()")
#endif

//------------------------------------------------------------------------------

#if defined(XTL_PROFILING)
const size_t N = 1000;  // Number of times visitor and matching procedure is invoked in one time measuring
const size_t M = 11;    // Number of times time measuring is done
const size_t K = 3;     // Number of experiment repetitions. Each experiment is M*N iterations
#else
const size_t N = 10000; // Number of times visitor and matching procedure is invoked in one time measuring
const size_t M = 101;   // Number of times time measuring is done
const size_t K = 100;   // Number of experiment repetitions. Each experiment is M*N iterations
#endif

//------------------------------------------------------------------------------

struct verdict
{
    verdict(long long v = 0, long long m = 0) : vis_time(v), mat_time(m) {}
    long long vis_time;
    long long mat_time;
};

//------------------------------------------------------------------------------

inline std::ostream& operator<<(std::ostream& os, const verdict& r)
{
    long long v = r.vis_time;
    long long m = r.mat_time;

    if (XTL_UNLIKELY(v <= 0 || m <= 0))
        return os << "ERROR: Insufficient timer resolution. Increase number of iterations N";
    else
    if (XTL_UNLIKELY(v <= m))
        return os << std::setw(3) << int(m*100/v-100) << "% slower" 
                  << " V=" << std::setw(3) << cycles(v)/N 
                  << " M=" << std::setw(3) << cycles(m)/N;
    else
        return os << std::setw(3) << int(v*100/m-100) << "% faster"
                  << " V=" << std::setw(3) << cycles(v)/N 
                  << " M=" << std::setw(3) << cycles(m)/N;
}

//------------------------------------------------------------------------------

template <typename Container>
inline typename Container::value_type mean(const Container& c)
{
    return std::accumulate(c.begin(),c.end(),typename Container::value_type())/c.size();
}

//------------------------------------------------------------------------------

template <typename Container>
inline typename Container::value_type deviation(const Container& c)
{
    typedef typename Container::value_type value_type;
    value_type m = mean(c);
    value_type d = 0;

    for (typename Container::const_iterator p = c.begin(); p != c.end(); ++p)
        d += (*p-m)*(*p-m);

    return value_type(std::sqrt(double(d)/c.size()));
}

//------------------------------------------------------------------------------

template <typename T>
inline void statistics(std::vector<T>& measurements, T& min, T& max, T& avg, T& med, T& dev)
{
    std::sort(measurements.begin(), measurements.end());
    min = measurements.front();
    max = measurements.back();
    avg = mean(measurements);
    med = measurements[measurements.size()/2];
    dev = deviation(measurements);
}

//------------------------------------------------------------------------------

inline long long display(const char* name, std::vector<long long>& timings)
{
    long long min, max, avg, med, dev;

    statistics(timings, min, max, avg, med, dev); // Get statistics from timings

    std::fstream file;
   
    file.open(std::string(name)+".csv", std::fstream::out | std::fstream::app);

    if (file)
    {
        // This will convert timings into cycles per iteration
        std::transform(
            timings.begin(), 
            timings.end(), 
            std::ostream_iterator<long long>(file, ", "), 
            [](long long t) { return cycles(t)/N; }
        );
        file << "End" << std::endl;
    }

    file.close();

    std::cout << name << " Time: ["
              << std::setw(4) << microseconds(min) << " --" 
              << std::setw(5) << microseconds(avg) << "/" 
              << std::setw(4) << microseconds(med) << " --"
              << std::setw(5) << microseconds(max) << "] Dev = " 
              << std::setw(4) << microseconds(dev)
#if   defined(XTL_TIMING_METHOD_1) || defined(XTL_TIMING_METHOD_2)
              << " Cycles/iteration: ["
              << std::setw(4) << cycles(min)/N << " --" 
              << std::setw(5) << cycles(avg)/N << "/" 
              << std::setw(4) << cycles(med)/N << " --"
              << std::setw(5) << cycles(max)/N << "]"
#endif
              << std::endl;

    return med;
}

//------------------------------------------------------------------------------

template <typename Object>
inline long long test(Object* (*make)(int), int (*match)(Object*))
{
    size_t a = 0; // Accumulator to make sure compiler doesn't take some loop invariants out
    size_t j = 0; // Incremental number for the current path/object combination. Ensures all path get tested.
    std::vector<long long> medians(K); // Final verdict of medians for each of the K experiments
    std::vector<Object*>   objects(N);
    std::vector<long long> timings(M);

    for (size_t k = 0; k < K; ++k)
    {
        for (size_t n = 0; n < N; ++n)
            objects[n] = make(j++);

        for (size_t m = 0; m < M; ++m)
        {
            time_stamp liStart  = get_time_stamp();

            for (size_t i = 0; i < N; ++i)
                a += match(objects[i]);

            time_stamp liFinish = get_time_stamp();
            timings[m] = liFinish-liStart;
        }

        for (size_t n = 0; n < N; ++n)
            delete objects[n];

        medians[k] = display("test", timings); // We are looking for a median per N iterations
    }

    std::sort(medians.begin(), medians.end());
    return cycles(medians[K/2])/N;
}

//------------------------------------------------------------------------------

inline void print_xtl_macros(std::ostream& os = std::clog)
{
#if defined(XTL_DEFAULT_SYNTAX)
    os << "XTL_DEFAULT_SYNTAX=" << XTL_STRING_LITERAL(XTL_DEFAULT_SYNTAX) << std::endl;
#else
    os << "XTL_DEFAULT_SYNTAX=" << std::endl;
#endif

#if defined(XTL_USE_VTBL_FREQUENCY)
    os << "XTL_USE_VTBL_FREQUENCY=" << XTL_STRING_LITERAL(XTL_USE_VTBL_FREQUENCY) << std::endl;
#else
    os << "XTL_USE_VTBL_FREQUENCY=" << std::endl;
#endif

#if defined(XTL_USE_MEMOIZED_CAST)
    os << "XTL_USE_MEMOIZED_CAST=" << XTL_STRING_LITERAL(XTL_USE_MEMOIZED_CAST) << std::endl;
#else
    os << "XTL_USE_MEMOIZED_CAST=" << std::endl;
#endif

#if defined(XTL_EXTRACTORS_MIGHT_THROW)
    os << "XTL_EXTRACTORS_MIGHT_THROW=" << XTL_STRING_LITERAL(XTL_EXTRACTORS_MIGHT_THROW) << std::endl;
#else
    os << "XTL_EXTRACTORS_MIGHT_THROW=" << std::endl;
#endif

#if defined(XTL_PRELOAD_LOCAL_STATIC_VARIABLES)
    os << "XTL_PRELOAD_LOCAL_STATIC_VARIABLES=" << XTL_STRING_LITERAL(XTL_PRELOAD_LOCAL_STATIC_VARIABLES) << std::endl;
#else
    os << "XTL_PRELOAD_LOCAL_STATIC_VARIABLES=" << std::endl;
#endif

#if defined(XTL_RANDOMIZE_TAGS)
    os << "XTL_RANDOMIZE_TAGS=" << XTL_STRING_LITERAL(XTL_RANDOMIZE_TAGS) << std::endl;
#else
    os << "XTL_RANDOMIZE_TAGS=" << std::endl;
#endif

#if defined(XTL_CLAUSES_NUM_ESTIMATES_TYPES_NUM)
    os << "XTL_CLAUSES_NUM_ESTIMATES_TYPES_NUM=" << XTL_STRING_LITERAL(XTL_CLAUSES_NUM_ESTIMATES_TYPES_NUM) << std::endl;
#else
    os << "XTL_CLAUSES_NUM_ESTIMATES_TYPES_NUM=" << std::endl;
#endif

#if defined(XTL_IRRELEVANT_VTBL_BITS)
    os << "XTL_IRRELEVANT_VTBL_BITS=" << XTL_STRING_LITERAL(XTL_IRRELEVANT_VTBL_BITS) << std::endl;
#else
    os << "XTL_IRRELEVANT_VTBL_BITS=" << std::endl;
#endif

#if defined(XTL_LOCAL_CACHE_LOG_SIZE)
    os << "XTL_LOCAL_CACHE_LOG_SIZE=" << XTL_STRING_LITERAL(XTL_LOCAL_CACHE_LOG_SIZE) << std::endl;
#else
    os << "XTL_LOCAL_CACHE_LOG_SIZE=" << std::endl;
#endif

#if defined(XTL_MAX_LOG_INC)
    os << "XTL_MAX_LOG_INC=" << XTL_STRING_LITERAL(XTL_MAX_LOG_INC) << std::endl;
#else
    os << "XTL_MAX_LOG_INC=" << std::endl;
#endif

#if defined(XTL_MAX_LOG_SIZE)
    os << "XTL_MAX_LOG_SIZE=" << XTL_STRING_LITERAL(XTL_MAX_LOG_SIZE) << std::endl;
#else
    os << "XTL_MAX_LOG_SIZE=" << std::endl;
#endif

#if defined(XTL_MIN_LOG_SIZE)
    os << "XTL_MIN_LOG_SIZE=" << XTL_STRING_LITERAL(XTL_MIN_LOG_SIZE) << std::endl;
#else
    os << "XTL_MIN_LOG_SIZE=" << std::endl;
#endif

#if defined(XTL_REDUNDANCY_CHECKING)
    os << "XTL_REDUNDANCY_CHECKING=" << XTL_STRING_LITERAL(XTL_REDUNDANCY_CHECKING) << std::endl;
#else
    os << "XTL_REDUNDANCY_CHECKING=" << std::endl;
#endif

#if defined(XTL_FALL_THROUGH)
    os << "XTL_FALL_THROUGH=" << XTL_STRING_LITERAL(XTL_FALL_THROUGH) << std::endl;
#else
    os << "XTL_FALL_THROUGH=" << std::endl;
#endif

#if defined(XTL_USE_BRACES)
    os << "XTL_USE_BRACES=" << XTL_STRING_LITERAL(XTL_USE_BRACES) << std::endl;
#else
    os << "XTL_USE_BRACES=" << std::endl;
#endif

#if defined(XTL_DUMP_PERFORMANCE)
    os << "XTL_DUMP_PERFORMANCE=" << XTL_STRING_LITERAL(XTL_DUMP_PERFORMANCE) << std::endl;
#else
    os << "XTL_DUMP_PERFORMANCE=" << std::endl;
#endif

#if defined(XTL_INLINE_TIMED_FUNCS)
    os << "XTL_INLINE_TIMED_FUNCS=" << XTL_STRING_LITERAL(XTL_INLINE_TIMED_FUNCS) << std::endl;
#else
    os << "XTL_INLINE_TIMED_FUNCS=" << std::endl;
#endif

#if defined(XTL_TEST_VIRTUAL_INHERITANCE)
    os << "XTL_TEST_VIRTUAL_INHERITANCE=" << XTL_STRING_LITERAL(XTL_TEST_VIRTUAL_INHERITANCE) << std::endl;
#else
    os << "XTL_TEST_VIRTUAL_INHERITANCE=" << std::endl;
#endif

#if defined(XTL_VISITOR_FORWARDING)
    os << "XTL_VISITOR_FORWARDING=" << XTL_STRING_LITERAL(XTL_VISITOR_FORWARDING) << std::endl;
#else
    os << "XTL_VISITOR_FORWARDING=" << std::endl;
#endif

#if defined(XTL_RND_SEED)
    os << "XTL_RND_SEED=" << XTL_STRING_LITERAL(XTL_RND_SEED) << std::endl;
#else
    os << "XTL_RND_SEED=" << std::endl;
#endif

//#if defined(XTL_SUPPORTS_ALLOCA)
//    os << "XTL_SUPPORTS_ALLOCA=" << XTL_STRING_LITERAL(XTL_SUPPORTS_ALLOCA) << std::endl;
//#else
//    os << "XTL_SUPPORTS_ALLOCA=" << std::endl;
//#endif

#if defined(XTL_SUPPORTS_VLA)
    os << "XTL_SUPPORTS_VLA=" << XTL_STRING_LITERAL(XTL_SUPPORTS_VLA) << std::endl;
#else
    os << "XTL_SUPPORTS_VLA=" << std::endl;
#endif

//#if defined(XTL_TIMING_METHOD_1)
//    os << "XTL_TIMING_METHOD_1=" << XTL_STRING_LITERAL(XTL_TIMING_METHOD_1) << std::endl;
//#else
//    os << "XTL_TIMING_METHOD_1=" << std::endl;
//#endif
//
//#if defined(XTL_TIMING_METHOD_2)
//    os << "XTL_TIMING_METHOD_2=" << XTL_STRING_LITERAL(XTL_TIMING_METHOD_2) << std::endl;
//#else
//    os << "XTL_TIMING_METHOD_2=" << std::endl;
//#endif
//
//#if defined(XTL_TIMING_METHOD_3)
//    os << "XTL_TIMING_METHOD_3=" << XTL_STRING_LITERAL(XTL_TIMING_METHOD_3) << std::endl;
//#else
//    os << "XTL_TIMING_METHOD_3=" << std::endl;
//#endif

//#if defined(XTL_CPP0X_TEMPLATE)
//    os << "XTL_CPP0X_TEMPLATE=" << XTL_STRING_LITERAL(XTL_CPP0X_TEMPLATE) << std::endl;
//#else
//    os << "XTL_CPP0X_TEMPLATE=" << std::endl;
//#endif
//
//#if defined(XTL_CPP0X_TYPENAME)
//    os << "XTL_CPP0X_TYPENAME=" << XTL_STRING_LITERAL(XTL_CPP0X_TYPENAME) << std::endl;
//#else
//    os << "XTL_CPP0X_TYPENAME=" << std::endl;
//#endif
} 

//------------------------------------------------------------------------------
