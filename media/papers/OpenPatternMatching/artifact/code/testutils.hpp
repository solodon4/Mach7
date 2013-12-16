///
/// \file testutils.hpp
///
/// Various utility functions used in our synthetic tests.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of Mach7 library (http://parasol.tamu.edu/mach7/).
/// Copyright (C) 2011-2013 Texas A&M University.
/// All rights reserved.
///

#pragma once

//------------------------------------------------------------------------------

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string>
#include <typeinfo>
#include <vector>
#include "config.hpp"
#include "timing.hpp"

#define NO_RANDOMIZATION

#if defined(_MSC_VER)
    // Visual C++ 2010 and 2012 don't seem to provide implementation of std::cbrt
    namespace std
    {
        inline float       cbrt(      float arg) { return pow(arg,      float(1.0/3.0)); }
        inline double      cbrt(     double arg) { return pow(arg,     double(1.0/3.0)); }
        inline long double cbrt(long double arg) { return pow(arg,long double(1.0/3.0)); }
    }
#endif

namespace mch ///< Mach7 library namespace
{

//------------------------------------------------------------------------------

#if defined(XTL_TIMING_METHOD_1)
    XTL_MESSAGE("Timing method 1: based on QueryPerformanceCounter()")
#elif defined(XTL_TIMING_METHOD_2)
    XTL_MESSAGE("Timing method 2: based on rdtsc register")
#elif defined(XTL_TIMING_METHOD_3)
    XTL_MESSAGE("Timing method 3: based on clock()")
#endif

//------------------------------------------------------------------------------

#if !defined(XTL_INLINE_TIMED_FUNCS)
#define XTL_INLINE_TIMED_FUNCS 0
#endif

#if XTL_INLINE_TIMED_FUNCS
#define XTL_TIMED_FUNC_BEGIN XTL_FORCE_INLINE_BEGIN
#define XTL_TIMED_FUNC_END   XTL_FORCE_INLINE_END
#else
#define XTL_TIMED_FUNC_BEGIN XTL_DO_NOT_INLINE_BEGIN
#define XTL_TIMED_FUNC_END   XTL_DO_NOT_INLINE_END
#endif

//------------------------------------------------------------------------------

#if defined(XTL_PROFILING) || defined(_DEBUG)
const size_t N = 10000; // Number of times visitor and matching procedure is invoked in one time measuring
const size_t M = 5;     // Number of times time measuring is done
const size_t K = 1;     // Number of experiment repetitions. Each experiment is M*N iterations
#else
const size_t N = 10000; // Number of times visitor and matching procedure is invoked in one time measuring
const size_t M = 101;   // Number of times time measuring is done
const size_t K = 100;   // Number of experiment repetitions. Each experiment is M*N iterations
#endif

//------------------------------------------------------------------------------

struct verdict
{
    verdict(size_t N, long long v = 0, long long m = 0) : iterations(N), vis_time(v), mat_time(m) {}
	size_t    iterations; ///< The number of iterations both timings represent
    long long vis_time;   ///< Time of the existing technique
    long long mat_time;   ///< Time of new (our) technique
};

//------------------------------------------------------------------------------

inline std::ostream& operator<<(std::ostream& os, const verdict& r)
{
	size_t    N = r.iterations;
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

inline long long display(const char* name, std::vector<long long>& timings, size_t N)
{
    long long min, max, avg, med, dev;

    statistics(timings, min, max, avg, med, dev); // Get statistics from timings

    std::fstream file;
   
    file.open((std::string(name)+".csv").c_str(), std::fstream::out | std::fstream::app);

    if (file)
    {
#if !defined(_MSC_VER) || _MSC_VER >= 1600
        // This will convert timings into cycles per iteration
        std::transform(
            timings.begin(), 
            timings.end(), 
            std::ostream_iterator<long long>(file, ", "), 
            [N](long long t) { return cycles(t)/N; }
        );
#endif
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
			  << " Cycles/" << std::setw(7) << N << " iterations ["
              << std::setw(7) << (min) << " --" 
              << std::setw(8) << (avg) << "/" 
              << std::setw(7) << (med) << " --"
              << std::setw(8) << (max) << "]"
#endif
              << std::endl;

    return med;
}

//------------------------------------------------------------------------------

/// Our make functions used for testing map numbers 0..N-1 to N different objects.
/// We iterate until we create different objects and then return only different ones.
template <typename Object>
std::vector<Object*> all_objects(Object* (*make)(int))
{
    std::vector<Object*> result;
    Object* first = make(0);
    result.push_back(first);

    for (size_t i = 1; ; ++i)
    {
        Object* obj = make(i);
        if (typeid(*obj) != typeid(*first))
            result.push_back(obj);
        else
        {
            delete obj;
            break;
        }
    }

    return result;
}

//------------------------------------------------------------------------------

template <typename Object>
inline long long test(Object* (*make)(int), int (*match)(Object*))
{
    std::vector<Object*> unique_objects = all_objects(make);
    const size_t unique_objects_size = unique_objects.size();
    size_t a = 0; // Accumulator to make sure compiler doesn't take some loop invariants out
    size_t j = 0; // Incremental number for the current path/object combination. Ensures all path get tested.
    std::vector<long long> medians(K); // Final verdict of medians for each of the K experiments
    std::vector<Object*>   objects(N);
    std::vector<long long> timings(M);

    for (size_t k = 0; k < K; ++k)
    {
        for (size_t m = 0; m < M; ++m)
        {
            // Since we limit N to account for number of arguments, we rotate
            // available objects in a loop to let each object be matched
            for (size_t n = 0; n < N; ++n)
                objects[n] = unique_objects[j++ % unique_objects_size];

            time_stamp liStart  = get_time_stamp();

            for (size_t i = 0; i < N; ++i)
                a += match(objects[i]);

            time_stamp liFinish = get_time_stamp();
            timings[m] = liFinish-liStart;
        }

        medians[k] = display("test", timings, N); // We are looking for a median per N iterations
    }

    // Destroy all the unique objects
    for (size_t n = 0; n < unique_objects_size; ++n)
        delete unique_objects[n];

    std::sort(medians.begin(), medians.end());
    return cycles(medians[K/2])/N;
}

//------------------------------------------------------------------------------

template <typename Object>
inline long long test(Object* (*make)(int), int (*match)(Object*,Object*))
{
    std::vector<Object*> unique_objects = all_objects(make);
    const size_t unique_objects_size = unique_objects.size();
    size_t N = std::sqrt(::mch::N); // Since we are going to do N*N iterations
    size_t a = 0; // Accumulator to make sure compiler doesn't take some loop invariants out
    size_t j = 0; // Incremental number for the current path/object combination. Ensures all path get tested.
    std::vector<long long> medians(K); // Final verdict of medians for each of the K experiments
    std::vector<Object*>   objects(N);
    std::vector<long long> timings(M);

    for (size_t k = 0; k < K; ++k)
    {
        for (size_t m = 0; m < M; ++m)
        {
            // Since we limit N to account for number of arguments, we rotate
            // available objects in a loop to let each object be matched
            for (size_t n = 0; n < N; ++n)
                objects[n] = unique_objects[j++ % unique_objects_size];

            time_stamp liStart  = get_time_stamp();

            for (size_t i = 0; i < N; ++i)
            for (size_t j = 0; j < N; ++j)
                a += match(objects[i],objects[j]);

            time_stamp liFinish = get_time_stamp();
            timings[m] = liFinish-liStart;
        }

        medians[k] = display("test", timings, N); // We are looking for a median per N iterations
    }

    // Destroy all the unique objects
    for (size_t n = 0; n < unique_objects_size; ++n)
        delete unique_objects[n];

    std::sort(medians.begin(), medians.end());
    return cycles(medians[K/2])/(N*N);
}

//------------------------------------------------------------------------------

template <typename Object>
inline long long test(Object* (*make)(int), int (*match)(Object*,Object*,Object*))
{
    std::vector<Object*> unique_objects = all_objects(make);
    const size_t unique_objects_size = unique_objects.size();
    size_t N = size_t(std::cbrt(double(::mch::N))); // Since we are going to do N*N*N iterations
    size_t a = 0; // Accumulator to make sure compiler doesn't take some loop invariants out
    size_t j = 0; // Incremental number for the current path/object combination. Ensures all path get tested.
    std::vector<long long> medians(K); // Final verdict of medians for each of the K experiments
    std::vector<Object*>   objects(N);
    std::vector<long long> timings(M);

    for (size_t k = 0; k < K; ++k)
    {
        for (size_t m = 0; m < M; ++m)
        {
            // Since we limit N to account for number of arguments, we rotate
            // available objects in a loop to let each object be matched
            for (size_t n = 0; n < N; ++n)
                objects[n] = unique_objects[j++ % unique_objects_size];

            time_stamp liStart  = get_time_stamp();

            for (size_t i = 0; i < N; ++i)
            for (size_t j = 0; j < N; ++j)
            for (size_t l = 0; l < N; ++l)
                a += match(objects[i],objects[j],objects[l]);

            time_stamp liFinish = get_time_stamp();
            timings[m] = liFinish-liStart;
        }

        medians[k] = display("test", timings, N); // We are looking for a median per N iterations
    }

    // Destroy all the unique objects
    for (size_t n = 0; n < unique_objects_size; ++n)
        delete unique_objects[n];

    std::sort(medians.begin(), medians.end());
    return cycles(medians[K/2])/(N*N*N);
}

//------------------------------------------------------------------------------

template <typename Object>
inline long long test(Object* (*make)(int), int (*match)(Object*,Object*,Object*,Object*))
{
    std::vector<Object*> unique_objects = all_objects(make);
    const size_t unique_objects_size = unique_objects.size();
    size_t N = size_t(std::sqrt(std::sqrt(::mch::N))); // Since we are going to do N*N*N*N iterations
    size_t a = 0; // Accumulator to make sure compiler doesn't take some loop invariants out
    size_t j = 0; // Incremental number for the current path/object combination. Ensures all path get tested.
    std::vector<long long> medians(K); // Final verdict of medians for each of the K experiments
    std::vector<Object*>   objects(N);
    std::vector<long long> timings(M);

    for (size_t k = 0; k < K; ++k)
    {
        for (size_t m = 0; m < M; ++m)
        {
            // Since we limit N to account for number of arguments, we rotate
            // available objects in a loop to let each object be matched
            for (size_t n = 0; n < N; ++n)
                objects[n] = unique_objects[j++ % unique_objects_size];

            time_stamp liStart  = get_time_stamp();

            for (size_t i = 0; i < N; ++i)
            for (size_t j = 0; j < N; ++j)
            for (size_t l = 0; l < N; ++l)
            for (size_t h = 0; h < N; ++h)
                a += match(objects[i],objects[j],objects[l],objects[h]);

            time_stamp liFinish = get_time_stamp();
            timings[m] = liFinish-liStart;
        }

        medians[k] = display("test", timings, N); // We are looking for a median per N iterations
    }

    // Destroy all the unique objects
    for (size_t n = 0; n < unique_objects_size; ++n)
        delete unique_objects[n];

    std::sort(medians.begin(), medians.end());
    return cycles(medians[K/2])/(N*N*N);
}

//------------------------------------------------------------------------------

template <typename R, typename A, R (&f1)(A), R (&f2)(A)>
inline size_t get_timings1(
        std::vector<A>&         arguments, 
        std::vector<long long>& timings1, 
        std::vector<long long>& timings2,
        R&                      a1,
        R&                      a2
     )
{
    XTL_ASSERT(timings1.size() == timings2.size());

    size_t N = arguments.size();
    size_t M = timings1.size();

    for (size_t m = 0; m < M; ++m)
    {
        time_stamp liStart1 = get_time_stamp();

        for (size_t i = 0; i < N; ++i)
            a1 += f1(arguments[i]);

        time_stamp liFinish1 = get_time_stamp();

        time_stamp liStart2 = get_time_stamp();

        for (size_t i = 0; i < N; ++i)
            a2 += f2(arguments[i]);

        time_stamp liFinish2 = get_time_stamp();

        XTL_ASSERT(a1==a2);

        timings1[m] = liFinish1-liStart1;
        timings2[m] = liFinish2-liStart2;
    }

	return N; // Number of iterations per measurement
}

//------------------------------------------------------------------------------

template <typename R, typename A, R (&f1)(A), R (&f2)(A)>
inline verdict get_timings1(std::vector<A>& arguments)
{
	size_t N = 0;
    std::vector<long long> medians1(K); // Final verdict of medians for each of the K experiments with visitors
    std::vector<long long> medians2(K); // Final verdict of medians for each of the K experiments with matching
    std::vector<long long> timings1(M); 
    std::vector<long long> timings2(M);
    R a1 = R();
    R a2 = R();

    for (size_t k = 0; k < K; ++k)
    {
        N = get_timings1<R,A,f1,f2>(arguments, timings1, timings2, a1, a2);
        medians1[k] = display("F1", timings1, N);
        medians2[k] = display("F2", timings2, N);
        
        std::ios_base::fmtflags fmt = std::cout.flags(); // use cout flags function to save original format
        std::cout << "\t\t" << verdict(N, medians1[k], medians2[k]) << "\t\t" 
                  << " a1=" << std::setw(8) << std::hex << a1 
                  << " a2=" << std::setw(8) << std::hex << a2 
                  << std::endl;
        std::cout.flags(fmt); // restore format
    }

    if (a1 != a2)
    {
        std::cout << "ERROR: Invariant " << a1 << "==" << a2 << " doesn't hold." << std::endl;
        exit(42);
    }

    std::sort(medians1.begin(), medians1.end());
    std::sort(medians2.begin(), medians2.end());
    return verdict(N,medians1[K/2],medians2[K/2]);
}

//------------------------------------------------------------------------------

template <typename R, typename A, R (&f1)(A,A), R (&f2)(A,A)>
inline size_t get_timings2(
        std::vector<A>&         arguments, 
        std::vector<long long>& timings1, 
        std::vector<long long>& timings2,
        R&                      a1,
        R&                      a2
     )
{
    XTL_ASSERT(timings1.size() == timings2.size());

    size_t N = arguments.size();
    size_t M = timings1.size();

    for (size_t m = 0; m < M; ++m)
    {
        time_stamp liStart1 = get_time_stamp();

        for (size_t i = 0; i < N-1; i += 2)
            a1 += f1(arguments[i],arguments[i+1]);

        time_stamp liFinish1 = get_time_stamp();

        time_stamp liStart2 = get_time_stamp();

        for (size_t i = 0; i < N-1; i += 2)
            a2 += f2(arguments[i],arguments[i+1]);

        time_stamp liFinish2 = get_time_stamp();

        XTL_ASSERT(a1==a2);

        timings1[m] = liFinish1-liStart1;
        timings2[m] = liFinish2-liStart2;
    }

	return N/2; // Number of iterations per measurement
}

//------------------------------------------------------------------------------

template <typename R, typename A, R (&f1)(A,A), R (&f2)(A,A)>
inline verdict get_timings2(std::vector<A>& arguments)
{
	size_t N = 0;
    std::vector<long long> medians1(K); // Final verdict of medians for each of the K experiments with visitors
    std::vector<long long> medians2(K); // Final verdict of medians for each of the K experiments with matching
    std::vector<long long> timings1(M); 
    std::vector<long long> timings2(M);
    R a1 = R();
    R a2 = R();

    for (size_t k = 0; k < K; ++k)
    {
        N = get_timings2<R,A,f1,f2>(arguments, timings1, timings2, a1, a2);
        medians1[k] = display("F1", timings1, N);
        medians2[k] = display("F2", timings2, N);
        
        std::ios_base::fmtflags fmt = std::cout.flags(); // use cout flags function to save original format
        std::cout << "\t\t" << verdict(N, medians1[k], medians2[k]) << "\t\t" 
                  << " a1=" << std::setw(8) << std::hex << a1 
                  << " a2=" << std::setw(8) << std::hex << a2 
                  << std::endl;
        std::cout.flags(fmt); // restore format
    }

    if (a1 != a2)
    {
        std::cout << "ERROR: Invariant " << a1 << "==" << a2 << " doesn't hold." << std::endl;
        exit(42);
    }

    std::sort(medians1.begin(), medians1.end());
    std::sort(medians2.begin(), medians2.end());
    return verdict(N,medians1[K/2],medians2[K/2]);
}

//------------------------------------------------------------------------------

template <typename R, typename A, R (&f1)(A,A,A), R (&f2)(A,A,A)>
inline size_t get_timings3(
        std::vector<A>&         arguments, 
        std::vector<long long>& timings1, 
        std::vector<long long>& timings2,
        R&                      a1,
        R&                      a2
     )
{
    XTL_ASSERT(timings1.size() == timings2.size());

    size_t N = arguments.size();
    size_t M = timings1.size();

    for (size_t m = 0; m < M; ++m)
    {
        time_stamp liStart1 = get_time_stamp();

        for (size_t i = 0; i < N-2; i += 3)
            a1 += f1(arguments[i],arguments[i+1],arguments[i+2]);

        time_stamp liFinish1 = get_time_stamp();

        time_stamp liStart2 = get_time_stamp();

        for (size_t i = 0; i < N-2; i += 3)
            a2 += f2(arguments[i],arguments[i+1],arguments[i+2]);

        time_stamp liFinish2 = get_time_stamp();

        XTL_ASSERT(a1==a2);

        timings1[m] = liFinish1-liStart1;
        timings2[m] = liFinish2-liStart2;
    }

	return N/3; // Number of iterations per measurement
}

//------------------------------------------------------------------------------

template <typename R, typename A, R (&f1)(A,A,A), R (&f2)(A,A,A)>
inline verdict get_timings3(std::vector<A>& arguments)
{
	size_t N = 0;
    std::vector<long long> medians1(K); // Final verdict of medians for each of the K experiments with visitors
    std::vector<long long> medians2(K); // Final verdict of medians for each of the K experiments with matching
    std::vector<long long> timings1(M); 
    std::vector<long long> timings2(M);
    R a1 = R();
    R a2 = R();

    for (size_t k = 0; k < K; ++k)
    {
        N = get_timings3<R,A,f1,f2>(arguments, timings1, timings2, a1, a2);
        medians1[k] = display("F1", timings1, N);
        medians2[k] = display("F2", timings2, N);
        
        std::ios_base::fmtflags fmt = std::cout.flags(); // use cout flags function to save original format
        std::cout << "\t\t" << verdict(N, medians1[k], medians2[k]) << "\t\t" 
                  << " a1=" << std::setw(8) << std::hex << a1 
                  << " a2=" << std::setw(8) << std::hex << a2 
                  << std::endl;
        std::cout.flags(fmt); // restore format
    }

    if (a1 != a2)
    {
        std::cout << "ERROR: Invariant " << a1 << "==" << a2 << " doesn't hold." << std::endl;
        exit(42);
    }

    std::sort(medians1.begin(), medians1.end());
    std::sort(medians2.begin(), medians2.end());
    return verdict(N,medians1[K/2],medians2[K/2]);
}

//------------------------------------------------------------------------------

template <typename R, typename A, R (&f1)(A,A,A,A), R (&f2)(A,A,A,A)>
inline size_t get_timings4(
        std::vector<A>&         arguments, 
        std::vector<long long>& timings1, 
        std::vector<long long>& timings2,
        R&                      a1,
        R&                      a2
     )
{
    XTL_ASSERT(timings1.size() == timings2.size());

    size_t N = arguments.size();
    size_t M = timings1.size();

    for (size_t m = 0; m < M; ++m)
    {
        time_stamp liStart1 = get_time_stamp();

        for (size_t i = 0; i < N-3; i += 4)
            a1 += f1(arguments[i],arguments[i+1],arguments[i+2],arguments[i+3]);

        time_stamp liFinish1 = get_time_stamp();

        time_stamp liStart2 = get_time_stamp();

        for (size_t i = 0; i < N-3; i += 4)
            a2 += f2(arguments[i],arguments[i+1],arguments[i+2],arguments[i+3]);

        time_stamp liFinish2 = get_time_stamp();

        XTL_ASSERT(a1==a2);

        timings1[m] = liFinish1-liStart1;
        timings2[m] = liFinish2-liStart2;
    }

	return N/4; // Number of iterations per measurement
}

//------------------------------------------------------------------------------

template <typename R, typename A, R (&f1)(A,A,A,A), R (&f2)(A,A,A,A)>
inline verdict get_timings4(std::vector<A>& arguments)
{
    size_t N = 0;
    std::vector<long long> medians1(K); // Final verdict of medians for each of the K experiments with visitors
    std::vector<long long> medians2(K); // Final verdict of medians for each of the K experiments with matching
    std::vector<long long> timings1(M); 
    std::vector<long long> timings2(M);
    R a1 = R();
    R a2 = R();

    for (size_t k = 0; k < K; ++k)
    {
        N = get_timings4<R,A,f1,f2>(arguments, timings1, timings2, a1, a2);
        medians1[k] = display("F1", timings1, N);
        medians2[k] = display("F2", timings2, N);
        
        std::ios_base::fmtflags fmt = std::cout.flags(); // use cout flags function to save original format
        std::cout << "\t\t" << verdict(N, medians1[k], medians2[k]) << "\t\t" 
                  << " a1=" << std::setw(8) << std::hex << a1 
                  << " a2=" << std::setw(8) << std::hex << a2 
                  << std::endl;
        std::cout.flags(fmt); // restore format
    }

    if (a1 != a2)
    {
        std::cout << "ERROR: Invariant " << a1 << "==" << a2 << " doesn't hold." << std::endl;
        exit(42);
    }

    std::sort(medians1.begin(), medians1.end());
    std::sort(medians2.begin(), medians2.end());
    return verdict(N,medians1[K/2],medians2[K/2]);
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

} // of namespace mch
