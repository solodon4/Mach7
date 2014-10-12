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

XTL_TIMED_FUNC_BEGIN
int fac1(int n)
{
    if (n >= 0)
    {
        if (n == 0)
            return 1;
        else
            return n*fac1(n-1);
    }
    else
        return 0;
}
XTL_TIMED_FUNC_END

//------------------------------------------------------------------------------

value_of<int> val0 = value_of<int>(0);
wildcard  wc;

XTL_TIMED_FUNC_BEGIN
int fac(const object& n)
{
    var_of<int> var0;

    if (val0.matches(n))
        return 1;
    if (var0.matches(n))
        return var0 * fac(object_of<int>(var0-1));
    if (  wc.matches(n))
        return 0;

    XTL_UNREACHABLE; // To avoid warning that control may reach end of a non-void function
}
XTL_TIMED_FUNC_END

//------------------------------------------------------------------------------

static const object* args[20] = {
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
};

//------------------------------------------------------------------------------

inline int fac2(int n) { return fac(*args[n]); }

//------------------------------------------------------------------------------

int main()
{
    using namespace mch;

    std::vector<int> arguments(N);

    for (size_t i = 0; i < N; ++i)
        arguments[i] = rand() % 20;

    verdict v = get_timings1<int,int,fac1,fac2>(arguments);
    std::cout << "Verdict: \t" << v << std::endl;
}

//------------------------------------------------------------------------------
