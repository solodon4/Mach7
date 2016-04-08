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

#ifndef HAS_BOOST

#include <iostream>

int main() {
    std::cout << "This test requires Boost!\n";
    return 1;
}

#else

#include <iostream>
#include "testutils.hpp"
#include <mach7/type_switchN-patterns-xtl.hpp> // Support for N-ary Match statement on patterns with arbitrary subtyping relation
#include <mach7/patterns/constructor.hpp>      // Support for constructor patterns
#include <mach7/adapters/boost/adapt_boost_variant.hpp> // Mach7 adapters for boost::variant

struct P { int m_p; P(int i = 0) : m_p(i) {} };
struct Q { int m_q; Q(int i = 0) : m_q(i) {} };
struct R { int m_r; R(int i = 0) : m_r(i) {} };

typedef boost::variant<P, Q, R> VP;

//------------------------------------------------------------------------------

XTL_TIMED_FUNC_BEGIN
int do_mach7_1(const VP& vp)
{
    using mch::C;

    Match(vp)
    {
        Case(C<P>()) return match0.m_p;
        Case(C<Q>()) return match0.m_q;
        Case(C<R>()) return match0.m_r;
    }
    EndMatch

	return -1;
}
XTL_TIMED_FUNC_END

//------------------------------------------------------------------------------

// An experiment with how much overhead pattern temporaries bring. This should
// give closer results to XTL-specialized type switch.
mch::var<const P&> vP;
mch::var<const Q&> vQ;
mch::var<const R&> vR;

//------------------------------------------------------------------------------

XTL_TIMED_FUNC_BEGIN
int do_mach7_1v(const VP& vp)
{
    Match(vp)
    {
        Case(vP) return match0.m_p;
        Case(vQ) return match0.m_q;
        Case(vR) return match0.m_r;
    }
    EndMatch

    return -1;
}
XTL_TIMED_FUNC_END

//------------------------------------------------------------------------------

XTL_TIMED_FUNC_BEGIN
int do_visit_1(const VP& vp)
{
    struct increment : boost::static_visitor<int>
    {
        int operator()(const P& p) const { return p.m_p; }
        int operator()(const Q& q) const { return q.m_q; }
        int operator()(const R& r) const { return r.m_r; }
    };

    return apply_visitor(increment(), vp);
}
XTL_TIMED_FUNC_END

//------------------------------------------------------------------------------

XTL_TIMED_FUNC_BEGIN
int do_mach7_2(const VP& vp1, const VP& vp2)
{
    using mch::C;

    Match(vp1, vp2)
    {
        Case(C<P>(), C<P>()) return 20*match0.m_p + match1.m_p;
        Case(C<P>(), C<Q>()) return 20*match0.m_p + match1.m_q;
        Case(C<P>(), C<R>()) return 20*match0.m_p + match1.m_r;
        Case(C<Q>(), C<P>()) return 40*match0.m_q + match1.m_p;
        Case(C<Q>(), C<Q>()) return 40*match0.m_q + match1.m_q;
        Case(C<Q>(), C<R>()) return 40*match0.m_q + match1.m_r;
        Case(C<R>(), C<P>()) return 60*match0.m_r + match1.m_p;
        Case(C<R>(), C<Q>()) return 60*match0.m_r + match1.m_q;
        Case(C<R>(), C<R>()) return 60*match0.m_r + match1.m_r;
    }
    EndMatch

    return -1;
}
XTL_TIMED_FUNC_END

//------------------------------------------------------------------------------

XTL_TIMED_FUNC_BEGIN
int do_mach7_2v(const VP& vp1, const VP& vp2)
{
    Match(vp1, vp2)
    {
        Case(vP, vP) return 20*match0.m_p + match1.m_p;
        Case(vP, vQ) return 20*match0.m_p + match1.m_q;
        Case(vP, vR) return 20*match0.m_p + match1.m_r;
        Case(vQ, vP) return 40*match0.m_q + match1.m_p;
        Case(vQ, vQ) return 40*match0.m_q + match1.m_q;
        Case(vQ, vR) return 40*match0.m_q + match1.m_r;
        Case(vR, vP) return 60*match0.m_r + match1.m_p;
        Case(vR, vQ) return 60*match0.m_r + match1.m_q;
        Case(vR, vR) return 60*match0.m_r + match1.m_r;
    }
    EndMatch

    return -1;
}
XTL_TIMED_FUNC_END

//------------------------------------------------------------------------------

XTL_TIMED_FUNC_BEGIN
int do_visit_2(const VP& vp1, const VP& vp2)
{
    struct increment : boost::static_visitor<int>
    {
        int operator()(const P& match0, const P& match1) const { return 20*match0.m_p + match1.m_p; }
        int operator()(const P& match0, const Q& match1) const { return 20*match0.m_p + match1.m_q; }
        int operator()(const P& match0, const R& match1) const { return 20*match0.m_p + match1.m_r; }
        int operator()(const Q& match0, const P& match1) const { return 40*match0.m_q + match1.m_p; }
        int operator()(const Q& match0, const Q& match1) const { return 40*match0.m_q + match1.m_q; }
        int operator()(const Q& match0, const R& match1) const { return 40*match0.m_q + match1.m_r; }
        int operator()(const R& match0, const P& match1) const { return 60*match0.m_r + match1.m_p; }
        int operator()(const R& match0, const Q& match1) const { return 60*match0.m_r + match1.m_q; }
        int operator()(const R& match0, const R& match1) const { return 60*match0.m_r + match1.m_r; }

    };

    return apply_visitor(increment(), vp1, vp2);
}
XTL_TIMED_FUNC_END

//------------------------------------------------------------------------------

static const VP args[20] = {
    P(0),
    Q(1),
    R(2),
    P(3),
    Q(4),
    R(5),
    P(6),
    Q(7),
    R(8),
    P(9),
    Q(10),
    R(11),
    P(12),
    Q(13),
    R(14),
    P(15),
    Q(16),
    R(17),
    P(18),
    Q(19),
};

//------------------------------------------------------------------------------

int main()
{
    using namespace mch;

    std::vector<VP> arguments(N);

    for (size_t i = 0; i < N; ++i)
        arguments[i] = args[rand() % 20];

    verdict v1 = get_timings1<int,const VP&,do_visit_1,do_mach7_1>(arguments);
    verdict v1v= get_timings1<int,const VP&,do_visit_1,do_mach7_1v>(arguments);
    verdict v1w= get_timings1<int,const VP&,do_mach7_1v,do_mach7_1>(arguments);
    verdict v2 = get_timings2<int,const VP&,do_visit_2,do_mach7_2>(arguments);
    verdict v2v= get_timings2<int,const VP&,do_visit_2,do_mach7_2v>(arguments);
    verdict v2w= get_timings2<int,const VP&,do_mach7_2v,do_mach7_2>(arguments);

    std::cout << std::endl;
    std::cout << "Verdict 1: \t" << v1 << std::endl;
    std::cout << "Verdict'1: \t" << v1v<< std::endl;
    std::cout << "Verdict\"1:\t" << v1w<< std::endl;
    std::cout << "Verdict 2: \t" << v2 << std::endl;
    std::cout << "Verdict'2: \t" << v2v<< std::endl;
    std::cout << "Verdict\"2:\t" << v2w<< std::endl;

}

//------------------------------------------------------------------------------

#endif
