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

using namespace mch;

//------------------------------------------------------------------------------

typedef std::pair<double,int> arg_type;

//------------------------------------------------------------------------------

template <typename T>
inline T sqr(const T& x) { return x*x; }

//------------------------------------------------------------------------------

extern double power1(const double, const int);
extern double power_opt(const double, const int);

inline double power1(const arg_type a) { return power_opt(a.first,a.second); }

//------------------------------------------------------------------------------

XTL_TIMED_FUNC_BEGIN
double power1(const double x, const int n)
{
    if (n == 0) return 1.0;
	if (n == 1) return x;
	if (n % 2 == 0) return sqr(power1(x,n/2));
	if (n % 2 == 1) return x*power1(x,n-1);
}
XTL_TIMED_FUNC_END

//------------------------------------------------------------------------------

XTL_TIMED_FUNC_BEGIN
double power_opt(double x, int n)
{
	if (n == 0) return 1.0;
	if (n == 1) return x;
	return 
        n%2 == 0
            ?   sqr(power_opt(x,n/2)) 
            : x*sqr(power_opt(x,n/2));
}
XTL_TIMED_FUNC_END

//------------------------------------------------------------------------------


int main()
{
    std::vector<arg_type> arguments(N);

    for (size_t i = 0; i < N; ++i)
        arguments[i] = arg_type(1.0+1.0/double(1+rand()-RAND_MAX/2), rand() % 100);

    verdict v = get_timings1<double,arg_type,power1,power1>(arguments);
    std::cout << "Verdict: \t" << v << std::endl;
}

//------------------------------------------------------------------------------
