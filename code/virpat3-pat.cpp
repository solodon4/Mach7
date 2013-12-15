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
#include "type_switchN-patterns.hpp"
#include "patterns/all.hpp"
#include "config.hpp"
#include "timing.hpp"

//------------------------------------------------------------------------------

#if defined(XTL_TIMING_METHOD_1)
    XTL_MESSAGE("Timing method 1: based on QueryPerformanceCounter()")
#elif defined(XTL_TIMING_METHOD_2)
    XTL_MESSAGE("Timing method 2: based on rdtsc register")
#elif defined(XTL_TIMING_METHOD_3)
    XTL_MESSAGE("Timing method 3: based on clock()")
#endif

using namespace mch;

//------------------------------------------------------------------------------

int fac(int n)
{
    var<unsigned short> m;

    Match (n)
    {
      Case(0) return 1;
      Case(1) return 1;
      Case(m) return m*fac(m-1);
      Case(_) return 0;
    }
    EndMatch
}

//------------------------------------------------------------------------------

const size_t T = 1000000;

//------------------------------------------------------------------------------

int main()
{
    int tests[]   = {0,1,2,3,4,5,6,7,8,9,10};
    const size_t N = XTL_ARR_SIZE(tests);

    int r = 0;

    for (size_t i = 0; i < N; ++i)
    {
        mch::time_stamp liStart1 = mch::get_time_stamp();

        for (size_t j = 0; j < T; ++j)
        {
            r += fac(tests[i]);
        }

        mch::time_stamp liFinish1 = mch::get_time_stamp();
        std::cout << liStart1 << '-' << liFinish1 << ':' << (liFinish1-liStart1) << std::endl;
        std::cout << "fac(" << i << ")=" << fac(tests[i]) << " r=" << r << " timing=" << mch::cycles(liFinish1-liStart1)/T << " cycles/iteration" << std::endl;
    }
}

//------------------------------------------------------------------------------
