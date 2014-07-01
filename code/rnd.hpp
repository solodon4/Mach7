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
/// Defines a compile-time equivalent of function rand()
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// \see https://parasol.tamu.edu/mach7/
/// \see https://github.com/solodon4/Mach7
/// \see https://github.com/solodon4/SELL
///

#pragma once

#include <cstddef>

namespace mch ///< Mach7 library namespace
{

#if !defined(XTL_RND_SEED)
/// Initial seed for the pseudo-random sequence. Will change the sequence in 
/// the same way a call to seed() changes rand() sequence.
#define XTL_RND_SEED 1
#endif

template <size_t N>
struct seed
{
    static const size_t value = seed<N-1>::value * 214013L + 2531011L;
};

template <> struct seed<0> { static const size_t value = XTL_RND_SEED; };

/// This generates values that match sequence of rand() calls.
/// \note Matches GCC's and Visual C++ implementation.
template <size_t N>
struct rnd
{
    static const size_t value = (seed<N+1>::value >> 16) & 0x7fff;
};

/// A run-time equivalent of the above
inline size_t get_seed(size_t n)
{
    return n == 0 ? XTL_RND_SEED : get_seed(n-1) * 214013L + 2531011L;
}

inline size_t get_seed_fast(size_t n)
{
    size_t result = XTL_RND_SEED;
    for (size_t i = 0; i < n; ++i) result = result * 214013L + 2531011L;
    return result;
}

/// A run-time equivalent of the above
inline size_t get_rnd(size_t n)
{
    return (get_seed_fast(n+1) >> 16) & 0x7fff;
}

} // of namespace mch
