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
/// Various utility functions used in our synthetic tests.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// \see https://parasol.tamu.edu/mach7/
/// \see https://github.com/solodon4/Mach7
/// \see https://github.com/solodon4/SELL
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

    size_t NN = shapes.size();
    size_t MM = timingsV.size();

    for (size_t m = 0; m < MM; ++m)
    {
        unsigned char j = 0;
        time_stamp liStart1 = get_time_stamp();

        for (size_t i = 0; i < NN-1; i += 2)
            aV += do_visit(*shapes[i],*shapes[i+1],some_numbers[j++]);

        time_stamp liFinish1 = get_time_stamp();

        j = 0;

        time_stamp liStart2 = get_time_stamp();

        for (size_t i = 0; i < NN-1; i += 2)
            aM += do_match(*shapes[i],*shapes[i+1],some_numbers[j++]);

        time_stamp liFinish2 = get_time_stamp();

        XTL_ASSERT(aV==aM);

        timingsV[m] = liFinish1-liStart1;
        timingsM[m] = liFinish2-liStart2;
    }

    return NN/2; // Number of iterations per measurement
}

//------------------------------------------------------------------------------

} // of namespace mch
