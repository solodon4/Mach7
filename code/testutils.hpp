///
/// \file testutils.hpp
///
/// Various utility functions used in our synthetic tests.
///
/// \autor Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of the XTL framework (http://parasol.tamu.edu/xtl/).
/// Copyright (C) 2005-2011 Texas A&M University.
/// All rights reserved.
///

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

#pragma once

//------------------------------------------------------------------------------

#if !defined(NUMBER_OF_VFUNCS)
#define NUMBER_OF_VFUNCS  1
#endif

#if !defined(NUMBER_OF_DERIVED)
#define NUMBER_OF_DERIVED 100
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

struct ShapeVisitor;

//------------------------------------------------------------------------------

struct OtherBase
{
    OtherBase() : m_foo(0xAAAAAAAA) {}
    virtual int foo() { return m_foo; };

    int m_foo;
};

//------------------------------------------------------------------------------

struct Shape
{
    size_t        m_kind;
    const size_t* m_all_kinds;
    size_t        m_fdc_id; // Fast dynamic cast ID

    Shape(size_t n = -1, size_t fdc_id = -1) : m_kind(n), m_all_kinds(0), m_fdc_id(fdc_id),
        m_member0(n+0),
        m_member1(n+1),
        m_member2(n+2),
        m_member3(n+3),
        m_member4(n+4),
        m_member5(n+5),
        m_member6(n+6),
        m_member7(n+7),
        m_member8(n+8),
        m_member9(n+9)
    {}

    virtual ~Shape() {}
    virtual void accept(ShapeVisitor&) const = 0;
    virtual void raise() const {} // Polymorphic exception idiom: http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Polymorphic_Exception
    #define FOR_EACH_MAX NUMBER_OF_VFUNCS-2
    #define FOR_EACH_N(N) virtual void foo ## N() {}
    #include "loop_over_numbers.hpp"
    #undef  FOR_EACH_N
    #undef  FOR_EACH_MAX

    int m_member0;
    int m_member1;
    int m_member2;
    int m_member3;
    int m_member4;
    int m_member5;
    int m_member6;
    int m_member7;
    int m_member8;
    int m_member9;
};

//------------------------------------------------------------------------------

extern Shape* make_shape(size_t);
extern size_t do_visit(const Shape&, size_t);
extern size_t do_match(const Shape&, size_t);

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

int relative_performance(long long v, long long m)
{
    if (XTL_UNLIKELY(v <= 0 || m <= 0))
    {
        std::cout << "ERROR: Insufficient timer resolution. Increase number of iterations N" << std::endl;
        exit(42);
    }
    else
    if (XTL_UNLIKELY(v <= m))
    {
        int percent = int(m*100/v-100);
        std::cout << "\t\t" << percent << "% slower" << std::endl;
        return +percent; // Positive number indicates how many percents slower we are 
    }
    else
    {
        int percent = int(v*100/m-100);
        std::cout << "\t\t" << percent << "% faster" << std::endl;
        return -percent; // Negative number indicates how many percents faster we are 
    }
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
        std::copy(timings.begin(), timings.end(), std::ostream_iterator<long long>(file, ", "));
        file << "End" << std::endl;
    }

    file.close();

    std::cout << name << " Time: ["
              << std::setw(4) << microseconds(min) << " -- " 
              << std::setw(4) << microseconds(avg) << "/" 
              << std::setw(4) << microseconds(med) << " -- "
              << std::setw(4) << microseconds(max) << "] Dev = " 
              << std::setw(4) << microseconds(dev)
#if   defined(XTL_TIMING_METHOD_1)
              // FIX: 1000 is heuristic here for my laptop. QueryPerformanceCounter doesn't specify how it is related to cycles!
              << " Cycles/iteration: ["
              << std::setw(4) << min*1000/N << " -- " 
              << std::setw(4) << avg*1000/N << "/" 
              << std::setw(4) << med*1000/N << " -- "
              << std::setw(4) << max*1000/N << "]"
#elif defined(XTL_TIMING_METHOD_2)
              << " Cycles/iteration: ["
              << std::setw(4) << min/N << " -- " 
              << std::setw(4) << avg/N << "/" 
              << std::setw(4) << med/N << " -- "
              << std::setw(4) << max/N << "]"
#endif
              << std::endl;

    return med;
}

//------------------------------------------------------------------------------

inline void run_timings(
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
        time_stamp liStart1 = get_time_stamp();

        for (size_t i = 0; i < N; ++i)
            aV += do_visit(*shapes[i],i);

        time_stamp liFinish1 = get_time_stamp();

        time_stamp liStart2 = get_time_stamp();

        for (size_t i = 0; i < N; ++i)
            aM += do_match(*shapes[i],i);

        time_stamp liFinish2 = get_time_stamp();

        XTL_ASSERT(aV==aM);

        timingsV[m] = liFinish1-liStart1;
        timingsM[m] = liFinish2-liStart2;
    }
}

//------------------------------------------------------------------------------

int test_sequential()
{
    std::cout << "=================== Sequential Test ===================" << std::endl;

    size_t a1 = 0, a2 = 0;
    std::vector<int>       percentages(K); // Final verdict of medians for each of the K experiments
    std::vector<long long> timingsV(M);
    std::vector<long long> timingsM(M);
    std::vector<Shape*>    shapes(N);

    for (size_t i = 0; i < N; ++i)
        shapes[i] = make_shape(i%K);

    for (size_t n = 0; n < K; ++n)
    {
        run_timings(shapes, timingsV, timingsM, a1, a2);
        long long avgV = display("AreaVisSeq", timingsV);
        long long avgM = display("AreaMatSeq", timingsM);
        percentages[n] = relative_performance(avgV, avgM);
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

    std::sort(percentages.begin(), percentages.end());
    return percentages[percentages.size()/2];
}

//------------------------------------------------------------------------------

int test_randomized()
{
#if !defined(NO_RANDOMIZATION)
    srand (unsigned(get_time_stamp()/get_frequency())); // Randomize pseudo random number generator
#endif
    std::cout << "=================== Randomized Test ===================" << std::endl;

    size_t a1 = 0, a2 = 0;
    std::vector<int>       percentages(K); // Final verdict of medians for each of the K experiments
    std::vector<long long> timingsV(M);
    std::vector<long long> timingsM(M);
    std::vector<Shape*>    shapes(N);

    for (size_t n = 0; n < K; ++n)
    {
        for (size_t i = 0; i < N; ++i)
            shapes[i] = make_shape(rand()%K);

        run_timings(shapes, timingsV, timingsM, a1, a2);

        long long avgV = display("AreaVisRnd", timingsV);
        long long avgM = display("AreaMatRnd", timingsM);
        percentages[n] = relative_performance(avgV, avgM);

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

    std::sort(percentages.begin(), percentages.end());
    return percentages[percentages.size()/2];
}

//------------------------------------------------------------------------------

int test_repetitive()
{
    std::cout << "=================== Repetitive Test ===================" << std::endl;

    size_t a1 = 0, a2 = 0;
    std::vector<int>       percentages(K); // Final verdict of medians for each of the K experiments
    std::vector<long long> timingsV(M);
    std::vector<long long> timingsM(M);
    std::vector<Shape*>    shapes(N);

    for (size_t n = 0; n < K; ++n)
    {
        for (size_t i = 0; i < N; ++i)
            shapes[i] = make_shape((n+i)*2-n-2*i);

        run_timings(shapes, timingsV, timingsM, a1, a2);
        long long avgV = display("AreaVisRep", timingsV);
        long long avgM = display("AreaMatRep", timingsM);
        percentages[n] = relative_performance(avgV, avgM);

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

    std::sort(percentages.begin(), percentages.end());
    return percentages[percentages.size()/2];
}

//------------------------------------------------------------------------------
