///
/// \file testvismat.hpp
///
/// Various utility functions used in our synthetic tests.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of Mach7 library (http://parasol.tamu.edu/mach7/).
/// Copyright (C) 2011-2012 Texas A&M University.
/// All rights reserved.
///

#pragma once

//------------------------------------------------------------------------------

#include "testvismat.hpp"

//------------------------------------------------------------------------------

namespace mch ///< Mach7 library namespace
{

//------------------------------------------------------------------------------

size_t run_timings(
        std::vector<Shape*>&    shapes, 
        std::vector<long long>& timingsV, 
        std::vector<long long>& timingsM,
        size_t& aV,
        size_t& aM
     )
{
    XTL_ASSERT(timingsM.size() == timingsV.size());

    size_t N = shapes.size();
    size_t M = timingsV.size();

    for (size_t m = 0; m < M; ++m)
    {
        unsigned char j = 0;
        time_stamp liStart1 = get_time_stamp();

        for (size_t i = 0; i < N-1; i += 2)
            aV += do_visit(*shapes[i],*shapes[i+1],some_numbers[j++]);

        time_stamp liFinish1 = get_time_stamp();

        j = 0;

        time_stamp liStart2 = get_time_stamp();

        for (size_t i = 0; i < N-1; i += 2)
            aM += do_match(*shapes[i],*shapes[i+1],some_numbers[j++]);

        time_stamp liFinish2 = get_time_stamp();

        XTL_ASSERT(aV==aM);

        timingsV[m] = liFinish1-liStart1;
        timingsM[m] = liFinish2-liStart2;
    }

    return N/2; // Number of iterations per measurement
}

//------------------------------------------------------------------------------

} // of namespace mch
