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

typedef std::pair<unsigned int,unsigned int> arg_type;

//------------------------------------------------------------------------------

extern unsigned int gcd1(const unsigned int, const unsigned int);

inline unsigned int gcd1(const arg_type a) { return gcd1(a.first,a.second); }

//------------------------------------------------------------------------------

XTL_TIMED_FUNC_BEGIN
unsigned int gcd1(const unsigned int a, const unsigned int b)
{
    if (a > b)
        return gcd1(a-b,b);
    else
    if (a < b)
        return gcd1(a,b-a);
    else
        return a;
}
XTL_TIMED_FUNC_END

//------------------------------------------------------------------------------

extern unsigned int gcd2(const object_of<unsigned int>& a, const object_of<unsigned int>& b);

inline unsigned int gcd2(const arg_type a) { return gcd2(object_of<unsigned int>(a.first),object_of<unsigned int>(a.second)); }

//------------------------------------------------------------------------------

wildcard wc;

XTL_TIMED_FUNC_BEGIN
unsigned int gcd2(const object_of<unsigned int>& a, const object_of<unsigned int>& b)
{
    value_of<unsigned int> valA(a.m_value);

    if (wc.matches(a) && valA.matches(b)) return a.m_value;

    var_of<unsigned int> y;
    p_plus_c<unsigned int> pa(y,a.m_value);

    if (wc.matches(a) && pa.matches(b)) return gcd2(a,object_of<unsigned int>(y));

    p_plus_c<unsigned int> pb(y,b.m_value);

    if (pb.matches(a) && wc.matches(b)) return gcd2(b,object_of<unsigned int>(y));
}
XTL_TIMED_FUNC_END

//------------------------------------------------------------------------------

int main()
{
    const size_t M = 10000; // Largest argument to GCD
    std::vector<arg_type> arguments(N);

    for (size_t i = 0; i < N; ++i)
        arguments[i] = arg_type(rand() % M + 1, rand() % M + 1);

    verdict v = get_timings1<unsigned int,arg_type,gcd1,gcd2>(arguments);
    std::cout << "Verdict: \t" << v << std::endl;
}

//------------------------------------------------------------------------------
