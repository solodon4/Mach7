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
#include <mach7/patterns/n+k.hpp>          // Generalized n+k patterns
#include "testutils.hpp"

//------------------------------------------------------------------------------

using namespace mch;

//------------------------------------------------------------------------------

template <typename T>
inline T sqr(const T& x) { return x*x; }

//------------------------------------------------------------------------------

int fib_opt(int n)
{
	if (n == 1 || n == 2) 
        return 1;

    return 
        n%2 == 0 
            ? sqr(fib_opt(n/2+1)) - sqr(fib_opt(n/2-1))
            : sqr(fib_opt(n/2+1)) + sqr(fib_opt(n/2));
}

//------------------------------------------------------------------------------

XTL_TIMED_FUNC_BEGIN
int fib2(int n)
{
    var<int> m;

    Match(n)
    {
      Case(1)     return 1;
      Case(2)     return 1;
      Case(2*m)   return sqr(fib2(m+1)) - sqr(fib2(m-1));
      Case(2*m+1) return sqr(fib2(m+1)) + sqr(fib2(m));
    }
    EndMatch
}
XTL_TIMED_FUNC_END

//------------------------------------------------------------------------------


int main()
{
    std::vector<int> arguments(N);

    for (size_t i = 0; i < N; ++i)
        arguments[i] = rand() % 20 + 1;

    verdict v = get_timings1<int,int,fib2,fib2>(arguments);
    std::cout << "Verdict: \t" << v << std::endl;
}

//------------------------------------------------------------------------------
