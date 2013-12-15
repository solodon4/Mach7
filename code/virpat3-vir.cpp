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

int fac(const object& n)
{
    value    val0 = val(0);
    variable var0;
    wildcard  wc;

    if (val0.matches(n)) return 1;
    if (var0.matches(n))
    {
        //if (typeid(n) == typeid(object_of<int>))
        {
            const object_of<int>* p = static_cast<const object_of<int>*>(var0.m_obj_ref);
            return p->m_value * fac(object_of<int>(p->m_value-1));
        }
        //else
        //    return 0;
    }
    if (  wc.matches(n)) return 0;
}


//------------------------------------------------------------------------------

const size_t T = 1000000;

//------------------------------------------------------------------------------

int main()
{
    int tests[]   = {0,1,2,3,4,5,6,7,8,9,10};
    const size_t N = XTL_ARR_SIZE(tests);
    object* objects[N] = {};

    for (size_t i = 0; i < N; ++i)
        objects[i] = make_obj(tests[i]);

    int r = 0;

    for (size_t i = 0; i < N; ++i)
    {
        mch::time_stamp liStart1 = mch::get_time_stamp();

        for (size_t j = 0; j < T; ++j)
        {
            r += fac(*objects[i]);
            //std::cout << "r=" << r << " u=" << u << std::endl;
        }

        mch::time_stamp liFinish1 = mch::get_time_stamp();
        std::cout << liStart1 << '-' << liFinish1 << ':' << (liFinish1-liStart1) << std::endl;
        std::cout << "fac(" << i << ")=" << fac(*objects[i]) << " r=" << r << " timing=" << mch::cycles(liFinish1-liStart1)/T << " cycles/iteration" << std::endl;
    }
}

//------------------------------------------------------------------------------
