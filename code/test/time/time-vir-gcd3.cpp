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
/// This file is a part of Mach7 library test suite.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// \see https://parasol.tamu.edu/mach7/
/// \see https://github.com/solodon4/Mach7
/// \see https://github.com/solodon4/SELL
///

#include <iostream>
#include "virpat.hpp"
#include "testutils.hpp"

//------------------------------------------------------------------------------

using namespace mch;

//------------------------------------------------------------------------------

typedef std::pair<unsigned int,unsigned int> arg_type;

//------------------------------------------------------------------------------

extern unsigned int gcd1(const unsigned int, const unsigned int);

inline unsigned int gcd1(const arg_type a) { return gcd1(a.first,a.second); }

//------------------------------------------------------------------------------

XTL_TIMED_FUNC_BEGIN
unsigned int gcd1(const unsigned int a, const unsigned int b)
{
    return b == 0 ? a : gcd1(b, a % b);
}
XTL_TIMED_FUNC_END

//------------------------------------------------------------------------------

extern unsigned int gcd2(const object_of<unsigned int>& a, const object_of<unsigned int>& b);

inline unsigned int gcd2(const arg_type a) { return gcd2(object_of<unsigned int>(a.first),object_of<unsigned int>(a.second)); }

//------------------------------------------------------------------------------

value_of<unsigned int> val0(0);

XTL_TIMED_FUNC_BEGIN
unsigned int gcd2(const object_of<unsigned int>& a, const object_of<unsigned int>& b)
{
    var_of<unsigned int> x;
    if (x.matches(a) && val0.matches(b)) return x;

    var_of<unsigned int> y;
    if (x.matches(a) && y.matches(b)) return gcd2(b, object_of<unsigned int>(x % y));

    XTL_UNREACHABLE; // To avoid warning that control may reach end of a non-void function
}
XTL_TIMED_FUNC_END

//------------------------------------------------------------------------------

int main()
{
    const size_t MM = 10000; // Largest argument to GCD
    std::vector<arg_type> arguments(N);

    for (size_t i = 0; i < N; ++i)
        arguments[i] = arg_type(rand() % MM + 1, rand() % MM + 1);

    verdict v = get_timings1<unsigned int,arg_type,gcd1,gcd2>(arguments);
    std::cout << "Verdict: \t" << v << std::endl;
}

//------------------------------------------------------------------------------
