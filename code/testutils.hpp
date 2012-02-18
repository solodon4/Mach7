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

#include "testshape.hpp"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string>
#include <vector>
#include "config.hpp"
#include "timing.hpp"

#if defined(XTL_TIMING_METHOD_1)
    XTL_MESSAGE("Timing method 1: based on QueryPerformanceCounter()")
#elif defined(XTL_TIMING_METHOD_2)
    XTL_MESSAGE("Timing method 2: based on rdtsc register")
#elif defined(XTL_TIMING_METHOD_3)
    XTL_MESSAGE("Timing method 3: based on clock()")
#endif


//------------------------------------------------------------------------------

#if defined(XTL_PROFILING)
const size_t N = 1000; // The amount of times visitor and matching procedure is invoked in one time measuring
const size_t M = 11;   // The amount of times time measuring is done
#else
const size_t N = 10000; // The amount of times visitor and matching procedure is invoked in one time measuring
const size_t M = 101;   // The amount of times time measuring is done
#endif

const size_t K = NUMBER_OF_DERIVED; // The amount of cases we have in hierarchy

//------------------------------------------------------------------------------

extern Shape* make_shape(size_t);

//------------------------------------------------------------------------------

template <typename Container>
typename Container::value_type mean(const Container& c)
{
    return std::accumulate(c.begin(),c.end(),typename Container::value_type())/c.size();
}

//------------------------------------------------------------------------------

template <typename Container>
typename Container::value_type deviation(const Container& c)
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
void statistics(std::vector<T>& measurements, T& min, T& max, T& avg, T& med, T& dev)
{
    std::sort(measurements.begin(), measurements.end());
    min = measurements.front();
    max = measurements.back();
    avg = mean(measurements);
    med = measurements[measurements.size()/2];
    dev = deviation(measurements);
}

//------------------------------------------------------------------------------

long long display(const char* name, std::vector<long long>& timings)
{
    long long min, max, avg, med, dev;

    statistics(timings, min, max, avg, med, dev); // Get statistics from timings

    std::fstream file;
   
    file.open(std::string(name)+".csv", std::fstream::out | std::fstream::app);

    if (file)
    {
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
              << std::setw(4) << microseconds(min) << " -- " 
              << std::setw(4) << microseconds(avg) << "/" 
              << std::setw(4) << microseconds(med) << " -- "
              << std::setw(4) << microseconds(max) << "] Dev = " 
              << std::setw(4) << microseconds(dev)
#if   defined(XTL_TIMING_METHOD_1) || defined(XTL_TIMING_METHOD_2)
              << " Cycles/iteration: ["
              << std::setw(4) << cycles(min)/N << " -- " 
              << std::setw(4) << cycles(avg)/N << "/" 
              << std::setw(4) << cycles(med)/N << " -- "
              << std::setw(4) << cycles(max)/N << "]"
#endif
              << std::endl;

    return med;
}

//------------------------------------------------------------------------------

/// Just a table of numbers that would prevent compiler from optimizing too much
static int some_numbers[256] = {
   2,   3,   5,   7,  11,  13,  17,  19,  23,  29, 
  31,  37,  41,  43,  47,  53,  59,  61,  67,  71, 
  73,  79,  83,  89,  97, 101, 103, 107, 109, 113, 
 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 
 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 
 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 
 283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 
 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 
 419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 
 467, 479, 487, 491, 499, 503, 509, 521, 523, 541, 
 547, 557, 563, 569, 571, 577, 587, 593, 599, 601, 
 607, 613, 617, 619, 631, 641, 643, 647, 653, 659, 
   2,   3,   5,   7,  11,  13,  17,  19,  23,  29, 
  31,  37,  41,  43,  47,  53,  59,  61,  67,  71, 
  73,  79,  83,  89,  97, 101, 103, 107, 109, 113, 
 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 
 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 
 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 
 283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 
 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 
 419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 
 467, 479, 487, 491, 499, 503, 509, 521, 523, 541, 
 547, 557, 563, 569, 571, 577, 587, 593, 599, 601, 
 607, 613, 617, 619, 631, 641, 643, 647, 653, 659, 
   2,   3,   5,   7,  11,  13,  17,  19,  23,  29, 
  31,  37,  41,  43,  47,  53 
};

//------------------------------------------------------------------------------

void run_timings(
        std::vector<Shape*>&    shapes, 
        std::vector<long long>& timingsV, 
        std::vector<long long>& timingsM,
        size_t& aV,
        size_t& aM
     )
{
    XTL_ASSERT(timingsM.size() == timingsV.size());

    size_t N = shapes.size();
    size_t M = timingsV.size();

    for (size_t m = 0; m < M; ++m)
    {
        unsigned char j = 0;
        time_stamp liStart1 = get_time_stamp();

        for (size_t i = 0; i < N; ++i)
            aV += do_visit(*shapes[i],some_numbers[j++]);

        time_stamp liFinish1 = get_time_stamp();

        j = 0;

        time_stamp liStart2 = get_time_stamp();

        for (size_t i = 0; i < N; ++i)
            aM += do_match(*shapes[i],some_numbers[j++]);

        time_stamp liFinish2 = get_time_stamp();

        XTL_ASSERT(aV==aM);

        timingsV[m] = liFinish1-liStart1;
        timingsM[m] = liFinish2-liStart2;
    }
}

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

verdict test_sequential()
{
    std::cout << "=================== Sequential Test ===================" << std::endl;

    size_t a1 = 0, a2 = 0;
    std::vector<long long> mediansV(K); // Final verdict of medians for each of the K experiments with visitors
    std::vector<long long> mediansM(K); // Final verdict of medians for each of the K experiments with matching
    std::vector<long long> timingsV(M);
    std::vector<long long> timingsM(M);
    std::vector<Shape*>    shapes(N);

    for (size_t i = 0; i < N; ++i)
        shapes[i] = make_shape(i%K);

    for (size_t k = 0; k < K; ++k)
    {
        run_timings(shapes, timingsV, timingsM, a1, a2);
        mediansV[k] = display("AreaVisSeq", timingsV);
        mediansM[k] = display("AreaMatSeq", timingsM);
        std::cout << "\t\t" << verdict(mediansV[k], mediansM[k]) << std::endl;
    }

    for (size_t i = 0; i < N; ++i)
    {
        delete shapes[i];
        shapes[i] = 0;
    }

    if (a1 != a2)
    {
        std::cout << "ERROR: Invariant " << a1 << "==" << a2 << " doesn't hold." << std::endl;
        exit(42);
    }

    std::sort(mediansV.begin(), mediansV.end());
    std::sort(mediansM.begin(), mediansM.end());
    return verdict(mediansV[K/2],mediansM[K/2]);
}

//------------------------------------------------------------------------------

verdict test_randomized()
{
#if !defined(NO_RANDOMIZATION)
    srand (unsigned(get_time_stamp()/get_frequency())); // Randomize pseudo random number generator
#endif
    std::cout << "=================== Randomized Test ===================" << std::endl;

    size_t a1 = 0, a2 = 0;
    std::vector<long long> mediansV(K); // Final verdict of medians for each of the K experiments with visitors
    std::vector<long long> mediansM(K); // Final verdict of medians for each of the K experiments with matching
    std::vector<long long> timingsV(M);
    std::vector<long long> timingsM(M);
    std::vector<Shape*>    shapes(N);

    for (size_t k = 0; k < K; ++k)
    {
        for (size_t i = 0; i < N; ++i)
            shapes[i] = make_shape(rand()%K);

        run_timings(shapes, timingsV, timingsM, a1, a2);
        mediansV[k] = display("AreaVisRnd", timingsV);
        mediansM[k] = display("AreaMatRnd", timingsM);
        std::cout << "\t\t" << verdict(mediansV[k], mediansM[k]) << std::endl;

        for (size_t i = 0; i < N; ++i)
        {
            delete shapes[i];
            shapes[i] = 0;
        }
    }

    if (a1 != a2)
    {
        std::cout << "ERROR: Invariant " << a1 << "==" << a2 << " doesn't hold." << std::endl;
        exit(42);
    }

    std::sort(mediansV.begin(), mediansV.end());
    std::sort(mediansM.begin(), mediansM.end());
    return verdict(mediansV[K/2],mediansM[K/2]);
}

//------------------------------------------------------------------------------

verdict test_repetitive()
{
    std::cout << "=================== Repetitive Test ===================" << std::endl;

    size_t a1 = 0, a2 = 0;
    std::vector<long long> mediansV(K); // Final verdict of medians for each of the K experiments with visitors
    std::vector<long long> mediansM(K); // Final verdict of medians for each of the K experiments with matching
    std::vector<long long> timingsV(M);
    std::vector<long long> timingsM(M);
    std::vector<Shape*>    shapes(N);

    for (size_t k = 0; k < K; ++k)
    {
        for (size_t i = 0; i < N; ++i)
            shapes[i] = make_shape((k+i)*2-k-2*i);

        run_timings(shapes, timingsV, timingsM, a1, a2);
        mediansV[k] = display("AreaVisRep", timingsV);
        mediansM[k] = display("AreaMatRep", timingsM);
        std::cout << "\t\t" << verdict(mediansV[k], mediansM[k]) << std::endl;

        for (size_t i = 0; i < N; ++i)
        {
            delete shapes[i];
            shapes[i] = 0;
        }
    }

    if (a1 != a2)
    {
        std::cout << "ERROR: Invariant " << a1 << "==" << a2 << " doesn't hold." << std::endl;
        exit(42);
    }

    std::sort(mediansV.begin(), mediansV.end());
    std::sort(mediansM.begin(), mediansM.end());
    return verdict(mediansV[K/2],mediansM[K/2]);
}

//------------------------------------------------------------------------------
