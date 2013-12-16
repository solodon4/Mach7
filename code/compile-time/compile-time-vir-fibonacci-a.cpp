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

int main()
{
    using namespace mch;

    std::vector<int> arguments(N);

    for (size_t i = 0; i < N; ++i)
        arguments[i] = rand() % 20 + 1;

    verdict v = get_timings1<int,int,fib_opt,fib_opt>(arguments);
    std::cout << "Verdict: \t" << v << std::endl;
}

//------------------------------------------------------------------------------
