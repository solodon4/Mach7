///
/// \file
///
/// This file is a part of pattern matching testing suite.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of Mach7 library (http://parasol.tamu.edu/mach7/).
/// Copyright (C) 2011-2013 Texas A&M University.
/// All rights reserved.
///

#define XTL_TIMING_METHOD_3

#include "testshape.hpp"
#include "match.hpp"
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
template <size_t N> struct bindings<shape_kind<N>> { KV(Shape,tag<N>::value); CM(0,shape_kind<N>::m_member0); CM(1,shape_kind<N>::m_member1); };
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
#define FOR_EACH_N(N) template <> struct bindings<shape_kind<N>> { KV(Shape,tag<N>::value);  BCS(shape_kind<N>,Shape); CM(0,shape_kind<N>::m_member0); CM(1,shape_kind<N>::m_member1); };
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
