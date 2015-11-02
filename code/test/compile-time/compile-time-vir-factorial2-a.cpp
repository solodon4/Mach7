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

XTL_TIMED_FUNC_BEGIN
unsigned int fac1(unsigned int n)
{
    return n ? n*fac1(n-1) : 1;
}
XTL_TIMED_FUNC_END

//------------------------------------------------------------------------------

int main()
{
    using namespace mch;

    std::vector<unsigned int> arguments(N);

    for (size_t i = 0; i < N; ++i)
        arguments[i] = rand() % 20;

    verdict v = get_timings1<unsigned int,unsigned int,fac1,fac1>(arguments);
    std::cout << "Verdict: \t" << v << std::endl;
}

//------------------------------------------------------------------------------
