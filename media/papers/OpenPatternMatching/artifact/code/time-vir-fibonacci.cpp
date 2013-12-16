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

template <typename T>
inline T sqr(const T& x) { return x*x; }

//------------------------------------------------------------------------------

XTL_TIMED_FUNC_BEGIN
int fib_opt(int n)
{
	if (n == 1 || n == 2) 
        return 1;

    return 
        n%2 == 0 
            ? sqr(fib_opt(n/2+1)) - sqr(fib_opt(n/2-1))
            : sqr(fib_opt(n/2+1)) + sqr(fib_opt(n/2));
}
XTL_TIMED_FUNC_END

//------------------------------------------------------------------------------

value_of<int> val1 = value_of<int>(1);
value_of<int> val2 = value_of<int>(2);

XTL_TIMED_FUNC_BEGIN
int fib(const object& n)
{
    if (val1.matches(n) || val2.matches(n))
        return 1;

    var_of<int>    v;
    p_times_c<int> x2(v,2);

    
    if (x2.matches(n))
        return sqr(fib(object_of<int>(v+1))) - sqr(fib(object_of<int>(v-1)));

    p_plus_c<int>  p1(x2,1);
    
    if (p1.matches(n))
        return sqr(fib(object_of<int>(v+1))) + sqr(fib(object_of<int>(v)));
}
XTL_TIMED_FUNC_END

//------------------------------------------------------------------------------

static const object* args[21] = {
    make_obj(0),
    make_obj(1),
    make_obj(2),
    make_obj(3),
    make_obj(4),
    make_obj(5),
    make_obj(6),
    make_obj(7),
    make_obj(8),
    make_obj(9),
    make_obj(10),
    make_obj(11),
    make_obj(12),
    make_obj(13),
    make_obj(14),
    make_obj(15),
    make_obj(16),
    make_obj(17),
    make_obj(18),
    make_obj(19),
    make_obj(20),
};

//------------------------------------------------------------------------------

//inline int fib2(int n) { return fib(*args[n]); }
inline int fib2(int n) { return fib(object_of<int>(n)); }

//------------------------------------------------------------------------------

int main()
{
    using namespace mch;

    std::vector<int> arguments(N);

    for (size_t i = 0; i < N; ++i)
        arguments[i] = rand() % 20 + 1;

    verdict v = get_timings1<int,int,fib_opt,fib2>(arguments);
    std::cout << "Verdict: \t" << v << std::endl;
}

//------------------------------------------------------------------------------
