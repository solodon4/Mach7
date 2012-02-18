///
/// \file
///
/// This file is a part of pattern matching testing suite.
///
/// \autor Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of the XTL framework (http://parasol.tamu.edu/xtl/).
/// Copyright (C) 2005-2012 Texas A&M University.
/// All rights reserved.
///

/*
x^0       = 1
x^(2*n)   = xn*xn where xn = x^n
x^(2*n+1) = x * x^(2*n)

and a very fast implementation of Fibonacci numbers would be

fib 1       = 1
fib 2       = 1
fib (2*n)   = (fib(n+1))^2 - (fib(n-1))^2
fib (2*n+1) = (fib(n+1))^2 + (fib n   )^2
*/
#include "match.hpp"
#include <math.h>
#include <iostream>
#include "testshape.hpp"

//------------------------------------------------------------------------------

int factorial(int n)
{
    variable<int> m;

	if (match<int>(0)(n))   return 1;
	if (match<int>(m+1)(n)) return (m+1)*factorial(m);
	XTL_ASSERT(!"Should never happen");	
    return 0;
}

//------------------------------------------------------------------------------

template <typename T>
inline T sqr(const T& x) { return x*x; }

//------------------------------------------------------------------------------

double power(double x, int n)
{
	if (n == 0) return 1.0;
	if (n == 1) return x;
	if (n % 2 == 0) return sqr(power(x,n/2));
	if (n % 2 == 1) return x*power(x,n-1);
}

//------------------------------------------------------------------------------

double power_opt(double x, int n)
{
	if (n == 0) return 1.0;
	if (n == 1) return x;
	return 
        n%2 == 0
            ?   sqr(power_opt(x,n/2)) 
            : x*sqr(power_opt(x,n/2));
}

//------------------------------------------------------------------------------

double power1(double x, int n)
{
	variable<int> m;

	if (match<int>(0)(n))     return 1.0;
    if (match<int>(1)(n))     return x;
	if (match<int>(m*2)(n))   return sqr(power1(x,m));
	if (match<int>(m*2+1)(n)) return x*power1(x,2*m);
}

//------------------------------------------------------------------------------

double power2(double x, int n)
{
    variable<int> m;

    Match(n)
    {
      When(0)     return 1.0;
      When(1)     return x;
      When(2*m)   return   sqr(power2(x,m));
      When(2*m+1) return x*sqr(power2(x,m));
    }
    EndMatch
}

//------------------------------------------------------------------------------

int fib(int n)
{
	if (n == 1 || n == 2) return 1;
	if (n % 2 == 0) return sqr(fib(n/2+1)) - sqr(fib(n/2-1));
	if (n % 2 == 1) return sqr(fib(n/2+1)) + sqr(fib(n/2));
}

//------------------------------------------------------------------------------

int fib_opt(int n)
{
	if (n == 1 || n == 2) 
        return 1;

    return 
        n%2 == 0 
            ? sqr(fib_opt(n/2+1)) - sqr(fib_opt(n/2-1))
            : sqr(fib_opt(n/2+1)) + sqr(fib_opt(n/2));
}

//------------------------------------------------------------------------------

int fib1(int n)
{
    variable<int> m;

    if (match<int>(1)(n))     return 1;
    if (match<int>(2)(n))     return 1;
    if (match<int>(m*2)(n))   return sqr(fib1(m+1)) - sqr(fib1(m-1));
    if (match<int>(m*2+1)(n)) return sqr(fib1(m+1)) + sqr(fib1(m));
}

//------------------------------------------------------------------------------

int fib2(int n)
{
    variable<int> m;

    Match(n)
    {
      When(1)     return 1;
      When(2)     return 1;
      When(2*m)   return sqr(fib2(m+1)) - sqr(fib2(m-1));
      When(2*m+1) return sqr(fib2(m+1)) + sqr(fib2(m));
    }
    EndMatch
}

//------------------------------------------------------------------------------

struct DummyShape : Shape
{
    void accept(ShapeVisitor&) const {}
};

//------------------------------------------------------------------------------

struct ShapeVisitor {};

//------------------------------------------------------------------------------

XTL_TIMED_FUNC_BEGIN
size_t do_match(const Shape& s, size_t n)
{
    //return power2(1.01,n);
    return fib2(n%100+1);
}
XTL_TIMED_FUNC_END

//------------------------------------------------------------------------------

XTL_TIMED_FUNC_BEGIN
size_t do_visit(const Shape& s, size_t n)
{
    //return power_opt(1.01,n);
    return fib_opt(n%100+1);
}
XTL_TIMED_FUNC_END

//------------------------------------------------------------------------------

Shape* make_shape(size_t)
{
    return new DummyShape;
}

//------------------------------------------------------------------------------

#include "testutils.hpp"    // Utilities for timing tests

//------------------------------------------------------------------------------

int main()
{
	double x = 2.0;

	for (int i = 0; i < 10; ++i)
		std::cout << x << '^' << i << '=' << power_opt(x,i) << (power_opt(x,i) == power2(x,i) ? "" : "WRONG") << std::endl;

	for (int i = 1; i < 10; ++i)
		std::cout << "fib(" << i << ")=" << fib_opt(i) << (fib_opt(i) == fib2(i) ? "" : "WRONG") << std::endl;

	for (int i = 1; i < 10; ++i)
		std::cout << "factorial(" << i << ")=" << factorial(i) << std::endl;

    verdict pp = test_repetitive();
    std::cout << "OVERALL: "
              << "Repetitive: " << pp << "; "
              << std::endl; 
}

//------------------------------------------------------------------------------
