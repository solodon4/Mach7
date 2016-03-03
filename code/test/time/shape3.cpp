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

#include <mach7/match.hpp>                 // Support for Match statement
#include <mach7/patterns/constructor.hpp>  // Support for constructor patterns
#include "timing.hpp"
#include "../unit/shape_bindings.hpp"
#include <cmath>
#include <iostream>

//#define POD_ONLY

mch::wildcard _; // Meta variable

double heron(const loc& l1, const loc& l2, const loc& l3) 
{
    double a2 = (l1.first-l2.first)*(l1.first-l2.first) + (l1.second-l2.second)*(l1.second-l2.second);
    double b2 = (l2.first-l3.first)*(l2.first-l3.first) + (l2.second-l3.second)*(l2.second-l3.second);
    double c2 = (l3.first-l1.first)*(l3.first-l1.first) + (l3.second-l1.second)*(l3.second-l1.second);
    return std::sqrt((a2+b2+c2)*(a2+b2+c2) - 2.0*(a2*a2+b2*b2+c2*c2))/4.0;
}

double area_select(Shape* shape)
{
    MatchP(shape)
    {
    CaseP(Circle,_,r)
        XTL_UNUSED(_); 
        std::cout << "Circle" << std::endl; 
        return 3.14 * r * r;
    CaseP(Square,_,s)
        XTL_UNUSED(_); 
        std::cout << "Square" << std::endl; 
        return s * s;
    CaseP(Triangle,x,y,z) 
        std::cout << "Triangle" << std::endl; 
        return heron(x,y,z);
    }
    EndMatchP

    XTL_ASSERT(!"Inexhaustive search"); 
    return 0.0;
}

int main()
{
    Shape* c = new Circle(loc(1,1),7);
    Shape* s = new Square(loc(2,2),2);
    Shape* t = new Triangle(loc(0,0),loc(0,1),loc(1,0));

    std::cout << "Area: " << area_select(t) << std::endl;
    std::cout << "Area: " << area_select(t) << std::endl;
    std::cout << "Area: " << area_select(s) << std::endl;
    std::cout << "Area: " << area_select(s) << std::endl;
    std::cout << "Area: " << area_select(c) << std::endl;
    std::cout << "Area: " << area_select(c) << std::endl;
}
