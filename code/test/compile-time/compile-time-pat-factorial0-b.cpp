///
/// \file
///
/// This file is a part of pattern matching testing suite.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of Mach7 library (http://parasol.tamu.edu/mach7/).
/// Copyright (C) 2011-2012 Texas A&M University.
/// All rights reserved.
///

#include <iostream>
#include <mach7/type_switchN-patterns.hpp> // Support for N-ary Match statement on patterns
#include <mach7/patterns/primitive.hpp>    // Wildcard, variable and value patterns
#include "testutils.hpp"

//------------------------------------------------------------------------------

using namespace mch;

//------------------------------------------------------------------------------

XTL_TIMED_FUNC_BEGIN
int fac2(int n)
{
    int m;

    Match (n)
    {
      Case(0) return 1;
      Case(m) return m*fac2(m-1);
      Case(_) return 0;
    }
    EndMatch
}
XTL_TIMED_FUNC_END

//------------------------------------------------------------------------------


int main()
{
    std::vector<int> arguments(N);

    for (size_t i = 0; i < N; ++i)
        arguments[i] = rand() % 20;

    verdict v = get_timings1<int,int,fac2,fac2>(arguments);
    std::cout << "Verdict: \t" << v << std::endl;
}

//------------------------------------------------------------------------------
