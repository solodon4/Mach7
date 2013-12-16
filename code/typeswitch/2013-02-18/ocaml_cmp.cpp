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

#include <iostream>
#include <iomanip>
#include <vector>
#include "match.hpp"
#include "timing.hpp"

#if !defined(NUMBER_OF_VFUNCS)
#define NUMBER_OF_VFUNCS  1
#endif
#if !defined(NUMBER_OF_DERIVED)
#define NUMBER_OF_DERIVED 100
#endif

struct Shape
{
    virtual ~Shape() {}
};

template <size_t N>
struct shape_kind : Shape
{
};

size_t do_match(const Shape& s)
{
    MatchP(s)
    {
        #define FOR_EACH_MAX  NUMBER_OF_DERIVED-1
        #define FOR_EACH_N(N) CaseP(shape_kind<N>) return N;
        #include "loop_over_numbers.hpp"
        #undef  FOR_EACH_N
        #undef  FOR_EACH_MAX
    }
    EndMatchP
    return size_t(-1);
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

int main()
{
    std::vector<Shape*> array(N);

    for (size_t j = 0; j < N; ++j)
        array[j] = make_shape(j%K);

    //Shape* s = make_shape(42);

    time_stamp total_time = 0;
    size_t z = 0;

    for (size_t i = 0; i < M; ++i)
    {
        time_stamp start = get_time_stamp();

        for (size_t j = 0; j < N; ++j)
            z = z + do_match(*array[j]);
            //z = z + do_match(*s);

        time_stamp finish = get_time_stamp();
        total_time += finish-start;
    }

    for (size_t j = 0; j < N; ++j)
        delete array[j];

    std::cout << "\nAverage time for " << N << " runs takes " << std::setprecision(5) << dbl::seconds(total_time)/M << " seconds: " << z << std::endl;
}
