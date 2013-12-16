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

#include "type_switchN-patterns.hpp"
#include "patterns/all.hpp"
#include <iostream>

bool is_odd(int a)  { return a%2==1; } 
bool is_even(int a) { return a%2==0; } 

using namespace mch;

int fac(int n)
{
    var<unsigned short> m;

    Match (n)
    {
      Case(0) return 1;
      Case(1) return 1;
      Case(m) return m*fac(m-1);
      Case(_) return 0;
    }
    EndMatch
}

unsigned int gcd(const unsigned int a, const unsigned int b)
{
    var<unsigned int> x, y;

    Match (a,b)
    {
    Case(_,a)   return a;
    Case(_,a+y) return gcd(a,y);
    Case(b+y,_) return gcd(b,y);
    //Case(x,+x)   return x;
    //Case(x,+x+y) return gcd(x,y);
    //Case(x,+x-y) return gcd(x-y,y);
    }
    EndMatch
}

int main()
{
    std::cout << gcd(24,16) << std::endl;
    std::cout << gcd( 1, 5) << std::endl;
    std::cout << gcd(13,16) << std::endl;
    std::cout << gcd(51,34) << std::endl;
//    std::cout << fac(-1);
    std::cout << fac(0) << fac(1) << fac(2) << fac(3) << fac(4) << std::endl;

    const int values[] = {10, 13, -23, 7};

    for (size_t i = 0, n = XTL_ARR_SIZE(values); i < n; ++i)
    {
        Match(values[i])
        {
        Case(is_odd)  std::cout << "odd"     << std::endl; break;
        Case(is_even) std::cout << "even"    << std::endl; break;
        Otherwise()   std::cout << "neither" << std::endl; break;
        }
        EndMatch

        std::cout << std::endl;
    }
}
