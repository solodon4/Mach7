///
/// \file
///
/// This file is a part of pattern matching testing suite.
///
/// \autor Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of the XTL framework (http://parasol.tamu.edu/xtl/).
/// Copyright (C) 2005-2011 Texas A&M University.
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
#include <math.h>
#include <iostream>
#include "match_generic.hpp"
#include "timing.hpp"

template <typename T> T& identity(T& t) { return t; }

int my(int* n) { return *n; }

template <> struct match_members<int>    { CM(0,my); };

int factorial(int n)
{
    variable<int> m;

	if (match<int>(0)(n))   return 1;
	if (match<int>(m+1)(n)) return (m+1)*factorial(m);
	XTL_ASSERT(!"Should never happen");	
    return 0;
}

template <typename T>
inline T sqr(const T& x) { return x*x; }

double power(double x, int n)
{
	variable<int> m;

	if (match<int>(0)(n))     return 1.0;
    if (match<int>(1)(n))     return x;
	if (match<int>(m*2)(n))   return sqr(power(x,m));
	if (match<int>(m*2+1)(n)) return x*power(x,2*m);
	//XTL_ASSERT(!"Should never happen");	
    //return 0.0;
}

int fib(int n)
{
    variable<int> m;

	if (match<int>(1)(n))     return 1;
    if (match<int>(2)(n))     return 1;
	if (match<int>(m*2)(n))   return sqr(fib(m+1)) - sqr(fib(m-1));
	if (match<int>(m*2+1)(n)) return sqr(fib(m+1)) + sqr(fib(m));
	//XTL_ASSERT(!"Should never happen");	
 //   return 0;
}

double power1(double x, int n)
{
	if (n == 0) return 1.0;
	if (n == 1) return x;
	if (n % 2 == 0) return sqr(power1(x,n/2));
	if (n % 2 == 1) return x*power1(x,n-1);
	XTL_ASSERT(!"Should never happen");	
    return 0.0;
}

int fib1(int n)
{
	if (n == 1 || n == 2) return 1;
	if (n % 2 == 0) return sqr(fib1(n/2+1)) - sqr(fib1(n/2-1));
	if (n % 2 == 1) return sqr(fib1(n/2+1)) + sqr(fib1(n/2));
	XTL_ASSERT(!"Should never happen");	
    return 0;
}

int main()
{
	double x = 2.0;

	for (int i = 0; i < 10; ++i)
		std::cout << x << '^' << i << '=' << power1(x,i) << std::endl;

	for (int i = 1; i < 10; ++i)
		std::cout << "fib(" << i << ")=" << fib1(i) << std::endl;

	for (int i = 1; i < 10; ++i)
		std::cout << "factorial(" << i << ")=" << factorial(i) << std::endl;

    {
        const int N = 10000;
        double sum1 = 0;

        time_stamp liStart1 = get_time_stamp();
        for (int i = 0; i < N; ++i)
            sum1 += power(x,i);
        time_stamp liFinish1 = get_time_stamp();

        double sum2 = 0;
        time_stamp liStart2 = get_time_stamp();
        for (int i = 0; i < N; ++i)
            sum2 += power1(x,i);
        time_stamp liFinish2 = get_time_stamp();

        std::cout << "PowerM Time:" << microseconds(liFinish1-liStart1) << "mks\t Sum:" << sum1 << std::endl;
        std::cout << "PowerN Time:" << microseconds(liFinish2-liStart2) << "mks\t Sum:" << sum2 << std::endl;
        std::cout << (liFinish1-liStart1)*100/(liFinish2-liStart2)-100 << "% slower" << std::endl;
    }
    {
        const int N = 1000;
        int sum1 = 0;

        time_stamp liStart1 = get_time_stamp();
        for (int i = 1; i < N; ++i)
            sum1 += fib(i);
        time_stamp liFinish1 = get_time_stamp();

        int sum2 = 0;
        time_stamp liStart2 = get_time_stamp();
        for (int i = 1; i < N; ++i)
            sum2 += fib1(i);
        time_stamp liFinish2 = get_time_stamp();

        std::cout << "FibM Time:" << microseconds(liFinish1-liStart1) << "mks\t Sum:" << sum1 << std::endl;
        std::cout << "FibN Time:" << microseconds(liFinish2-liStart2) << "mks\t Sum:" << sum2 << std::endl;
        std::cout << (liFinish1-liStart1)*100/(liFinish2-liStart2)-100 << "% slower" << std::endl;
    }
}
