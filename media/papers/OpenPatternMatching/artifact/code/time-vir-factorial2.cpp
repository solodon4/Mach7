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
unsigned int fac1(unsigned int n)
{
    return n ? n*fac1(n-1) : 1;
}
XTL_TIMED_FUNC_END

//------------------------------------------------------------------------------

value_of<unsigned int> val0 = value_of<unsigned int>(0);

XTL_TIMED_FUNC_BEGIN
unsigned int fac(const object_of<unsigned int>& n)
{
    var_of<unsigned int>   v;
    p_plus_c<unsigned int> p(v,1);

    if (val0.matches(n))
        return 1;
    if (p.matches(n))
        return (v+1) * fac(object_of<unsigned int>(v));
}
XTL_TIMED_FUNC_END

//------------------------------------------------------------------------------

static const object_of<unsigned int>* args[20] = {
    make_obj(0U),
    make_obj(1U),
    make_obj(2U),
    make_obj(3U),
    make_obj(4U),
    make_obj(5U),
    make_obj(6U),
    make_obj(7U),
    make_obj(8U),
    make_obj(9U),
    make_obj(10U),
    make_obj(11U),
    make_obj(12U),
    make_obj(13U),
    make_obj(14U),
    make_obj(15U),
    make_obj(16U),
    make_obj(17U),
    make_obj(18U),
    make_obj(19U),
};

//------------------------------------------------------------------------------

inline unsigned int fac2(unsigned int n) { return fac(*args[n]); }

//------------------------------------------------------------------------------

int main()
{
    using namespace mch;

    std::vector<unsigned int> arguments(N);

    for (size_t i = 0; i < N; ++i)
        arguments[i] = rand() % 20;

    verdict v = get_timings1<unsigned int,unsigned int,fac1,fac2>(arguments);
    std::cout << "Verdict: \t" << v << std::endl;
}

//------------------------------------------------------------------------------
