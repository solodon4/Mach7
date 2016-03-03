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

#define XTL_TIMING_METHOD_3

#include "testshape.hpp"
#include <mach7/match.hpp>
#include "timing.hpp"

//------------------------------------------------------------------------------

template <size_t N>
struct shape_kind : OtherBase, Shape
{
#if XTL_DEFAULT_SYNTAX == 'K' || XTL_DEFAULT_SYNTAX == 'k' || XTL_DEFAULT_SYNTAX == 'F' || XTL_DEFAULT_SYNTAX == 'f'
    shape_kind() : Shape(tag<N>::value) {}
#endif
    void accept(ShapeVisitor&) const {}
};

//------------------------------------------------------------------------------

#if XTL_DEFAULT_SYNTAX == 'K' || XTL_DEFAULT_SYNTAX == 'k'

namespace mch ///< Mach7 library namespace
{
template <>         struct bindings<Shape>         { KS(Shape::m_kind); };
template <size_t N> struct bindings<shape_kind<N>> { KV(Shape,tag<N>::value); Members(shape_kind<N>::m_member0,shape_kind<N>::m_member1); };
} // of namespace mch

#elif XTL_DEFAULT_SYNTAX == 'F' || XTL_DEFAULT_SYNTAX == 'f'

SKV(Shape,0); // Declare the smallest kind value for Shape hierarchy

namespace mch ///< Mach7 library namespace
{
template <>         struct bindings<Shape>         { KS(Shape::m_kind); KV(Shape,tag<NUMBER_OF_DERIVED+1>::value); };

// NOTE: We need to explicitly instantiate all bindings as otherwise our kinds 
//       associations don't get instantiated and recorded. This happens because
//       now that bindings with specific N > base cases is never instantiated
// FIX:  Wonder though why it didn't instantiate them for base cases at least since
//       we instantiate them explicitly in Case statements
#define FOR_EACH_MAX NUMBER_OF_DERIVED-1
#define FOR_EACH_N(N) template <> struct bindings<shape_kind<N>> { KV(Shape,tag<N>::value);  BCS(shape_kind<N>,Shape); Members(shape_kind<N>::m_member0,shape_kind<N>::m_member1); };
#include "loop_over_numbers.hpp"
#undef  FOR_EACH_N
#undef  FOR_EACH_MAX


} // of namespace mch

#endif

//------------------------------------------------------------------------------

size_t do_match(const Shape& s)
{
    Match(s)
    {
    #define FOR_EACH_MAX  NUMBER_OF_DERIVED-1
    #define FOR_EACH_N(N) Case(shape_kind<N>) return N;
    #include "loop_over_numbers.hpp"
    #undef  FOR_EACH_N
    #undef  FOR_EACH_MAX
    }
    EndMatch
    return invalid;
}

Shape* make_shape(int i)
{
    switch (i % NUMBER_OF_DERIVED)
    {
    #define FOR_EACH_MAX  NUMBER_OF_DERIVED-1
    #define FOR_EACH_N(N) case N: return new shape_kind<N>;
    #include "loop_over_numbers.hpp"
    #undef  FOR_EACH_N
    #undef  FOR_EACH_MAX
    }
    return new shape_kind<0>;
}

const size_t N = 1000000;// The amount of times visitor and matching procedure is invoked in one time measuring
const size_t M = 101;    // The amount of times time measuring is done
const size_t K = NUMBER_OF_DERIVED; // The amount of cases we have in hierarchy

#include <iostream>
#include <iomanip>
#include <vector>

int main()
{
    std::vector<Shape*> array(N);

    for (size_t j = 0; j < N; ++j)
        array[j] = make_shape(j%K);

    //Shape* s = make_shape(42);

    mch::time_stamp total_time = 0;
    size_t z = 0;

    for (size_t i = 0; i < M; ++i)
    {
        mch::time_stamp start = mch::get_time_stamp();

        for (size_t j = 0; j < N; ++j)
            z = z + do_match(*array[j]);
            //z = z + do_match(*s);

        mch::time_stamp finish = mch::get_time_stamp();
        total_time += finish-start;
    }

    for (size_t j = 0; j < N; ++j)
        delete array[j];

    std::cout << "\nAverage time for " << N << " runs takes " << std::setprecision(5) << mch::dbl::seconds(total_time)/M << " seconds: " << z << std::endl;
}
