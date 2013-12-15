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

#include "match.hpp"                // Support for Match statement
#include "patterns/combinators.hpp" // Support for pattern combinators
#include "patterns/constructor.hpp" // Support for constructor patterns
#include "patterns/guard.hpp"       // Support for guard patterns
#include "patterns/n+k.hpp"         // Support for n+k patterns

#include <cmath>
#include <complex>
#include <iostream>

namespace mch ///< Mach7 library namespace
{

size_t gcd(size_t a, size_t b)
{
    if (a > b)
        return gcd(size_t(a-b),b);
    else
    if (a < b)
        return gcd(a,size_t(b-a));
    else
        return a;
}

int gcd(int a, int b)
{
    if (a > b)
        return gcd(a-b,b);
    else
    if (a < b)
        return gcd(a,b-a);
    else
        return a;
}

struct gcd_functor
{
    template <class A, class B> 
    auto operator()(A&& a, B&& b) const -> XTL_RETURN
    (
        gcd(std::forward<A>(a),std::forward<B>(b))
    )
};

template <typename E1, typename E2> auto gcd(E1&& e1, E2&& e2) -> XTL_RETURN_ENABLE_IF
(
    is_expression<E1>::value || is_expression<E2>::value, 
    make_expr<gcd_functor>(filter(std::forward<E1>(e1)),filter(std::forward<E2>(e2)))
)

struct sqrt_functor
{
    template <class A> 
    A operator()(const A& a) const { return std::sqrt(a); }
};

template <typename E> auto sqrt(E&& e) -> XTL_RETURN_ENABLE_IF
(
    is_expression<E>::value, 
    make_expr<sqrt_functor>(filter(std::forward<E>(e)))
)

template <typename E1, typename S>
inline bool solve(const expr<sqrt_functor,E1>& e, const S& r)
{
    // sqrt(e) = r  => e = r^2;
    return solve(e.m_e1,r*r);
}

template <>
inline bool solve(const expr<division,ref2<var<double>>,ref2<var<double>>>& e, const double& r)
{
    // n/m = 3.1415926 => n=22,m=7
    long m[2][2] = {
                        {1,0},
                        {0,1}
                   };
    double x      = r;
    //double startx = r;
    long maxden   = 100;
    long ai;

    /* loop finding terms until denom gets too big */
    while (m[1][0] *  ( ai = (long)x ) + m[1][1] <= maxden) {
        long t;
        t = m[0][0] * ai + m[0][1];
        m[0][1] = m[0][0];
        m[0][0] = t;
        t = m[1][0] * ai + m[1][1];
        m[1][1] = m[1][0];
        m[1][0] = t;
        if(x==(double)ai) break;     // AF: division by zero
        x = 1/(x - (double) ai);
        if(x>(double)0x7FFFFFFF) break;  // AF: representation failure
    } 

    ///* now remaining x is between 0 and 1/ai */
    ///* approx as either 0 or 1/m where m is max that will fit in maxden */
    ///* first try zero */
    //printf("%ld/%ld, error = %e\n", m[0][0], m[1][0], startx - ((double) m[0][0] / (double) m[1][0]));

    ///* now try other possibility */
    //ai = (maxden - m[1][1]) / m[1][0];
    //m[0][0] = m[0][0] * ai + m[0][1];
    //m[1][0] = m[1][0] * ai + m[1][1];
    //printf("%ld/%ld, error = %e\n", m[0][0], m[1][0], startx - ((double) m[0][0] / (double) m[1][0]));
    e.m_e1(m[0][0]);
    e.m_e2(m[1][0]);
    return true;
}

} // of namespace mch

int main()
{
    using namespace mch; // Mach7's library namespace

    const std::complex<double>  i(0.0,1.0);
          std::complex<double>  c(1,1);
      var<std::complex<double>> d;
      var<double> a,b,r,f;

    std::cout << a << "+"    << b << "*i" << std::endl;

  //if (C<std::complex<double>>(b*i)(i))   std::cout << b << "*i" << std::endl;
    if (C<std::complex<double>>(b*i+a)(i)) std::cout << a << "+"    << b << "*i" << std::endl;
    if (C<double>(sqrt(a))(4.0))   std::cout << "sqrt("  << a << ")=4" << std::endl;
    if (C<double>(a/b)(3.1415926)) std::cout << "3.1415926=" << a << "/" << b << std::endl;
    if (C<double>(-a)(3.1415926))  std::cout << "-3.1415926=" << a << std::endl;

  //if (C<std::complex<double>>(i*a)(c)) std::cout << "(2,2) + " << a << " * i" << std::endl;
    if (C<std::complex<double>>(d*i)(c)) std::cout << d << " * i" << std::endl;
	if (C<std::complex<double>>(i)(c))   std::cout << "i" << std::endl;
    if (C<std::complex<double>>(d)(c))   std::cout << d << std::endl;

    int n = 3;
    var<size_t> m;

    Match(n)
    {
        When(m)   std::cout << m << std::endl;
        When(m+0) std::cout << m << "+0" << std::endl;
        When(m+1) std::cout << m << "+1" << std::endl;
        When(m+2) std::cout << m << "+2" << std::endl;
        When(m+3) std::cout << m << "+3" << std::endl;
        When(m+4) std::cout << m << "+4" << std::endl;
        When(m+5) std::cout << m << "+5" << std::endl;
    }
    EndMatch

  //var<size_t> m1(12);
    var<size_t> m2( 3);

    Match(n)
    {
        When(m |= gcd(m,m2) == 3)   std::cout << "gcd-3"   << std::endl;
        When(m |= gcd(m,3u) == 3)   std::cout << "gcd-3a"  << std::endl;
        When(m |= gcd(3U,m) == 3)   std::cout << "gcd-3b"  << std::endl;
        // When(m |= true/*gcd(3u,3u)== 3*/)   std::cout << "gcd-3c"  << std::endl; // FIX: doesn't work with true there
        When(m |= gcd(m,m2) == 2)   std::cout << "gcd-2"   << std::endl;
        When(m |= gcd(m,m2) == 1)   std::cout << "gcd-1"   << std::endl;
    }
    EndMatch

}
