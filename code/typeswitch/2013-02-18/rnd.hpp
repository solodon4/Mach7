///
/// \file rnd.hpp
///
/// Defines a compile-time equivalent of function rand()
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of Mach7 library (http://parasol.tamu.edu/mach7/).
/// Copyright (C) 2011-2013 Texas A&M University.
/// All rights reserved.
///

#pragma once

#include <cstddef>

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

/// A run-time equivalent of the above
inline size_t get_rnd(size_t n)
{
    return (get_seed(n+1) >> 16) & 0x7fff;
}
