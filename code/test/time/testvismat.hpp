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
/// Various utility functions used in our synthetic tests.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// \see https://parasol.tamu.edu/mach7/
/// \see https://github.com/solodon4/Mach7
/// \see https://github.com/solodon4/SELL
///

#pragma once

//------------------------------------------------------------------------------

#include "testutils.hpp"
#include "testshape.hpp"

//------------------------------------------------------------------------------

/// Application including this header must provide implementation of this.
extern Shape* make_shape(size_t);

//------------------------------------------------------------------------------

namespace mch ///< Mach7 library namespace
{

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

extern size_t run_timings(
        std::vector<Shape*>&    shapes, 
        std::vector<long long>& timingsV, 
        std::vector<long long>& timingsM,
        size_t& aV,
        size_t& aM
     );

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

size_t run_timings_parallel(
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

    return N; // Number of iterations per measurement
}

#endif // XTL_MULTI_THREADING

//------------------------------------------------------------------------------

verdict test_sequential()
{
    std::cout << "=================== Sequential Test ===================" << std::endl;

    size_t n = 0, a1 = 0, a2 = 0;
    std::vector<long long> mediansV(K); // Final verdict of medians for each of the K experiments with visitors
    std::vector<long long> mediansM(K); // Final verdict of medians for each of the K experiments with matching
    std::vector<long long> timingsV(M);
    std::vector<long long> timingsM(M);
    std::vector<Shape*>    shapes(N);

    for (size_t i = 0; i < N; ++i)
        shapes[i] = make_shape(i);

    for (size_t k = 0; k < K; ++k)
    {
        n = run_timings(shapes, timingsV, timingsM, a1, a2);
        mediansV[k] = display("AreaVisSeq", timingsV, n);
        mediansM[k] = display("AreaMatSeq", timingsM, n);

        std::ios_base::fmtflags fmt = std::cout.flags(); // use cout flags function to save original format
        std::cout << "\t\t" << verdict(n, mediansV[k], mediansM[k]) << "\t\t" 
                  << " a1=" << std::setw(8) << std::hex << a1 
                  << " a2=" << std::setw(8) << std::hex << a2 
                  << std::endl;
        std::cout.flags(fmt); // restore format
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
    return verdict(n,mediansV[K/2],mediansM[K/2]);
}

//------------------------------------------------------------------------------

verdict test_randomized()
{
#if !defined(NO_RANDOMIZATION)
    srand (unsigned(get_time_stamp()/get_frequency())); // Randomize pseudo random number generator
#endif
    std::cout << "=================== Randomized Test ===================" << std::endl;

    size_t n = 0, a1 = 0, a2 = 0;
    std::vector<long long> mediansV(K); // Final verdict of medians for each of the K experiments with visitors
    std::vector<long long> mediansM(K); // Final verdict of medians for each of the K experiments with matching
    std::vector<long long> timingsV(M);
    std::vector<long long> timingsM(M);
    std::vector<Shape*>    shapes(N);

    for (size_t i = 0; i < N; ++i)
        shapes[i] = make_shape(rand());

    for (size_t k = 0; k < K; ++k)
    {
        n = run_timings(shapes, timingsV, timingsM, a1, a2);
        mediansV[k] = display("AreaVisRnd", timingsV, n);
        mediansM[k] = display("AreaMatRnd", timingsM, n);
        
        std::ios_base::fmtflags fmt = std::cout.flags(); // use cout flags function to save original format
        std::cout << "\t\t" << verdict(n, mediansV[k], mediansM[k]) << "\t\t" 
                  << " a1=" << std::setw(8) << std::hex << a1 
                  << " a2=" << std::setw(8) << std::hex << a2 
                  << std::endl;
        std::cout.flags(fmt); // restore format
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
    return verdict(n,mediansV[K/2],mediansM[K/2]);
}

//------------------------------------------------------------------------------

verdict test_repetitive()
{
    std::cout << "=================== Repetitive Test ===================" << std::endl;

    size_t n = 0, a1 = 0, a2 = 0;
    std::vector<long long> mediansV(K); // Final verdict of medians for each of the K experiments with visitors
    std::vector<long long> mediansM(K); // Final verdict of medians for each of the K experiments with matching
    std::vector<long long> timingsV(M);
    std::vector<long long> timingsM(M);
    std::vector<Shape*>    shapes(N);

    for (size_t k = 0; k < K; ++k)
    {
        for (size_t i = 0; i < N; ++i)
            shapes[i] = make_shape((k+i)*2-k-2*i);

        n = run_timings(shapes, timingsV, timingsM, a1, a2);
        mediansV[k] = display("AreaVisRep", timingsV, n);
        mediansM[k] = display("AreaMatRep", timingsM, n);
        
        std::ios_base::fmtflags fmt = std::cout.flags(); // use cout flags function to save original format
        std::cout << "\t\t" << verdict(n, mediansV[k], mediansM[k]) << "\t\t" 
                  << " a1=" << std::setw(8) << std::hex << a1 
                  << " a2=" << std::setw(8) << std::hex << a2 
                  << std::endl;
        std::cout.flags(fmt); // restore format

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
    return verdict(n,mediansV[K/2],mediansM[K/2]);
}

//------------------------------------------------------------------------------

} // of namespace mch
