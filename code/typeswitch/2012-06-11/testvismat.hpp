///
/// \file testvismat.hpp
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

#include "testutils.hpp"
#include "testshape.hpp"

//------------------------------------------------------------------------------

extern Shape* make_shape(size_t);

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

#if XTL_MULTI_THREADING

#include <atomic>
#include <thread>

#define run_timings run_timings_parallel

//------------------------------------------------------------------------------

void partial_do_visit(std::vector<Shape*>& shapes, size_t L, size_t R, std::atomic<size_t>& result)
{
    size_t a = 0;
    unsigned char j = (unsigned char)L;

    for (size_t i = L; i < R; ++i)
        a += do_visit(*shapes[i],some_numbers[j++]);

    result += a;
}

//------------------------------------------------------------------------------

void partial_do_match(std::vector<Shape*>& shapes, size_t L, size_t R, std::atomic<size_t>& result)
{
    size_t a = 0;
    unsigned char j = (unsigned char)L;

    for (size_t i = L; i < R; ++i)
        a += do_match(*shapes[i],some_numbers[j++]);

    result += a;
}

//------------------------------------------------------------------------------

static const size_t num_extra_threads = 1;

//------------------------------------------------------------------------------

void run_timings_parallel(
        std::vector<Shape*>&    shapes, 
        std::vector<long long>& timingsV, 
        std::vector<long long>& timingsM,
        size_t& aV,
        size_t& aM
     )
{
    XTL_ASSERT(timingsM.size() == timingsV.size());

    const size_t N = shapes.size();
    const size_t M = timingsV.size();
    const size_t C = N/(num_extra_threads+1);
    std::atomic<size_t> aa;

    for (size_t m = 0; m < M; ++m)
    {
        //unsigned char j = 0;
        size_t l = 0;

        time_stamp liStart1 = get_time_stamp();   // <- Begin timing

        std::thread threadsV[num_extra_threads];

        aa = 0; // Reset atomic accumulator

        // Launch num_extra_threads additional threads given each of them chunk from l to l+c:
        for (size_t i = 0; i < num_extra_threads; ++i, l += C) 
            threadsV[i] = std::thread(partial_do_visit, std::ref(shapes), l, l+C, std::ref(aa));

        // Do the last chunk in the main thread
        partial_do_visit(shapes, l, N, aa);

        // Join the threads with the main thread
        for (size_t i = 0; i < num_extra_threads; ++i)
            threadsV[i].join(); 

        aV += aa;

        time_stamp liFinish1 = get_time_stamp();  // <- End timing

        //j = 0;
        l  = 0;
        aa = 0; // Reset atomic accumulator

        time_stamp liStart2 = get_time_stamp();   // <- Begin timing

        std::thread threadsM[num_extra_threads];

        aa = 0; // Reset atomic accumulator

        // Launch num_extra_threads additional threads given each of them chunk from l to l+c:
        for (size_t i = 0; i < num_extra_threads; ++i, l += C) 
            threadsM[i] = std::thread(partial_do_match, std::ref(shapes), l, l+C, std::ref(aa));

        // Do the last chunk in the main thread
        partial_do_match(shapes, l, N, aa);

        // Join the threads with the main thread
        for (size_t i = 0; i < num_extra_threads; ++i)
            threadsM[i].join(); 

        aM += aa;

        time_stamp liFinish2 = get_time_stamp();  // <- End timing

        XTL_ASSERT(aV==aM);

        timingsV[m] = liFinish1-liStart1;
        timingsM[m] = liFinish2-liStart2;
    }
}

#endif // XTL_MULTI_THREADING

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
        shapes[i] = make_shape(i);

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
            shapes[i] = make_shape(rand());

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
