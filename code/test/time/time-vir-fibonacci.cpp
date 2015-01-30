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

template <typename T>
inline T sqr(const T& x) { return x*x; }

//------------------------------------------------------------------------------

XTL_TIMED_FUNC_BEGIN
int fib_opt(int n)
{
	if (n == 1 || n == 2) 
        return 1;

    return 
        n%2 == 0 
            ? sqr(fib_opt(n/2+1)) - sqr(fib_opt(n/2-1))
            : sqr(fib_opt(n/2+1)) + sqr(fib_opt(n/2));
}
XTL_TIMED_FUNC_END

//------------------------------------------------------------------------------

value_of<int> val1 = value_of<int>(1);
value_of<int> val2 = value_of<int>(2);

XTL_TIMED_FUNC_BEGIN
int fib(const object& n)
{
    if (val1.matches(n) || val2.matches(n))
        return 1;

    var_of<int>    v;
    p_times_c<int> x2(v,2);

    
    if (x2.matches(n))
        return sqr(fib(object_of<int>(v+1))) - sqr(fib(object_of<int>(v-1)));

    p_plus_c<int>  p1(x2,1);
    
    if (p1.matches(n))
        return sqr(fib(object_of<int>(v+1))) + sqr(fib(object_of<int>(v)));

    XTL_UNREACHABLE; // To avoid warning that control may reach end of a non-void function
}
XTL_TIMED_FUNC_END

//------------------------------------------------------------------------------

static const object* args[21] = {
    make_obj(0),
    make_obj(1),
    make_obj(2),
    make_obj(3),
    make_obj(4),
    make_obj(5),
    make_obj(6),
    make_obj(7),
    make_obj(8),
    make_obj(9),
    make_obj(10),
    make_obj(11),
    make_obj(12),
    make_obj(13),
    make_obj(14),
    make_obj(15),
    make_obj(16),
    make_obj(17),
    make_obj(18),
    make_obj(19),
    make_obj(20),
};

//------------------------------------------------------------------------------

//inline int fib2(int n) { return fib(*args[n]); }
inline int fib2(int n) { return fib(object_of<int>(n)); }

//------------------------------------------------------------------------------

int main()
{
    using namespace mch;

    std::vector<int> arguments(N);

    for (size_t i = 0; i < N; ++i)
        arguments[i] = rand() % 20 + 1;

    verdict v = get_timings1<int,int,fib_opt,fib2>(arguments);
    std::cout << "Verdict: \t" << v << std::endl;
}

//------------------------------------------------------------------------------
