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

#include <mach7/type_switchN-patterns.hpp> // Support for N-ary Match statement on patterns
#include <mach7/patterns/n+k.hpp>          // Generalized n+k patterns
#include <mach7/patterns/predicate.hpp>    // Support for predicate patterns
#include <mach7/patterns/primitive.hpp>    // Wildcard, variable and value patterns

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

    XTL_UNREACHABLE; // To avoid warning that control may reach end of a non-void function
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

    XTL_UNREACHABLE; // To avoid warning that control may reach end of a non-void function
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
