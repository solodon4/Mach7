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

XTL_TIMED_FUNC_BEGIN
int fac1(int n)
{
    if (n >= 0)
    {
        if (n == 0)
            return 1;
        else
            return n*fac1(n-1);
    }
    else
        return 0;
}
XTL_TIMED_FUNC_END

//------------------------------------------------------------------------------

value_of<int> val0 = value_of<int>(0);
wildcard  wc;

XTL_TIMED_FUNC_BEGIN
int fac(const object& n)
{
    var_of<int> var0;

    if (val0.matches(n))
        return 1;
    if (var0.matches(n))
        return var0 * fac(object_of<int>(var0-1));
    if (  wc.matches(n))
        return 0;
}
XTL_TIMED_FUNC_END

//------------------------------------------------------------------------------

static const object* args[20] = {
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
};

//------------------------------------------------------------------------------

inline int fac2(int n) { return fac(*args[n]); }

//------------------------------------------------------------------------------

int main()
{
    using namespace mch;

    std::vector<int> arguments(N);

    for (size_t i = 0; i < N; ++i)
        arguments[i] = rand() % 20;

    verdict v = get_timings1<int,int,fac1,fac2>(arguments);
    std::cout << "Verdict: \t" << v << std::endl;
}

//------------------------------------------------------------------------------
