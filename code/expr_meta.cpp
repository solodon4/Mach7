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

#include <cstddef> 

template <size_t N>                 struct value  {};
template <typename E1, typename E2> struct plus   {};
template <typename E1, typename E2> struct minus  {};
template <typename E1, typename E2> struct times  {};
template <typename E1, typename E2> struct divide {};

template <typename E>               struct eval;
template <size_t N>                 struct eval<value<N>>      { static const int res = N; };
template <typename E1, typename E2> struct eval<plus<E1,E2>>   { static const int res = eval<E1>::res+eval<E2>::res; };
template <typename E1, typename E2> struct eval<minus<E1,E2>>  { static const int res = eval<E1>::res-eval<E2>::res; };
template <typename E1, typename E2> struct eval<times<E1,E2>>  { static const int res = eval<E1>::res*eval<E2>::res; };
template <typename E1, typename E2> struct eval<divide<E1,E2>> { static const int res = eval<E1>::res/eval<E2>::res; };

int main()
{
    typedef plus<times<value<3>,value<4>>, value<2>> expression;
    static_assert(eval<expression>::res == 14, "Error");
}
