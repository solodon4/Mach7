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

#include "shape_bindings.hpp"

#include <mach7/match.hpp>                 // Support for Match statement
#include <mach7/patterns/bindings.hpp>     // Mach7 support for bindings on arbitrary UDT
#include <mach7/patterns/combinators.hpp>  // Support for pattern combinators &&, || and !
#include <mach7/patterns/constructor.hpp>  // Support for constructor patterns
#include <mach7/patterns/guard.hpp>        // Support for guard patterns
#include <mach7/patterns/n+k.hpp>          // Generalized n+k patterns
#include <mach7/patterns/primitive.hpp>    // Wildcard, variable and value patterns

//#define POD_ONLY

double heron(const loc&, const loc&, const loc&) { return 1.0/2; }

double area(const Shape& shape)
{
    mch::wildcard      _; // Meta variable
    mch::var<loc>  x,y,z;
    mch::var<double> r,s;

    if (mch::C<Circle>(_,r)(shape))
        return 3.14 * r * r;

    if (mch::C<Square>(_,s)(shape))
        return s * s;

    if (mch::C<Triangle>(x,y,z)(shape))
        return heron(x,y,z);

    XTL_ASSERT(!"Inexhaustive search");
    XTL_UNREACHABLE; // To avoid warning that control may reach end of a non-void function
}

// FIX: Normally this test is supposed to test non-const subject, however this
//      renders error in VS2012 and VS2013, so we just workaround it here to
//      use const as I don't have fix for those compilers yet.
#if defined(_MSC_VER) && (1700 <= _MSC_VER && _MSC_VER < 1900)
    #define WORKAROUND_CONST const
#else
    #define WORKAROUND_CONST
#endif

loc center(WORKAROUND_CONST Shape& shape)
{
    mch::var<loc> c;

    if (mch::C<Circle>(c/*,_*/)(shape))
        return c;

    mch::var<double> x,y,s;

    if (mch::C<Square>(
            mch::C<loc>(x,y),s)(shape))
        return loc(x+s/2,y+s/2);

    mch::var<double> x1,y1,x2,y2,x3,y3;

    if (mch::C<Triangle>(
            mch::C<loc>(x1,y1),
            mch::C<loc>(x2,y2),
            mch::C<loc>(x3,y3)
                       )(shape))
        return loc((x1+x2+x3)/3,(y1+y2+y3)/3);

    XTL_ASSERT(!"Inexhaustive search");
    XTL_UNREACHABLE; // To avoid warning that control may reach end of a non-void function
}

void foo(Shape* s)
{
    mch::var<loc>  x,y,z;
    mch::var<double> a,b,c;
    //mch::wildcard       _;

    auto pattern = mch::C<Circle>(x, -a*2+1);

    if (pattern(s))
        std::cout << "Matched against pattern " << a << std::endl;

    if (mch::C<Circle>(x, -a*2+1)(s))
        std::cout << "Matched against subexpression " << a << std::endl;

    if (mch::C<Circle>(x |= x == loc(1,1), a)(s))
        std::cout << "Matched against guard " << a << std::endl;

    if (mch::C<Circle>(x, a |= a > 3 && a < 5)(s))
        std::cout << "Matched radius against COMPLEX guard " << a << std::endl;

    if (mch::C<Circle>(mch::C<loc>(b,1), a)(s))
        std::cout << "Matched against subexpression " << b << std::endl;

    if (mch::C<Circle>(x,4.0)(s))
        std::cout << "Circle with center " << x << " and FIXED radius " << std::endl;

    if (mch::C<Circle>(x,a)(s))
        std::cout << "Circle with center " << x << " and radius " << a << std::endl;
    else
    if (mch::C<Square>(x,a)(s))
        std::cout << "Square with top left " << x << " and side " << a << std::endl;
    else
    if (mch::C<Triangle>(x,y,z)(s))
        std::cout << "Triangle with corners " << x << ',' << y << ',' << z << std::endl;
}

void bar(WORKAROUND_CONST ADTShape& s)
{
    mch::var<cloc>  x,y,z;
    mch::var<double> a;

#ifndef POD_ONLY
    if (mch::C<ADTShapeEx,ADTShape::circle>(x,a)(s))
        std::cout << "ADTCircleEx with center " << x << " and radius " << a << std::endl;
    else
    if (mch::C<ADTShapeEx,ADTShape::square>(x,a)(s))
        std::cout << "ADTSquareEx with top left " << x << " and side " << a << std::endl;
    else
    if (mch::C<ADTShapeEx,ADTShape::triangle>(x,y,z)(s))
        std::cout << "ADTTriangleEx with corners " << x << ',' << y << ',' << z << std::endl;
#endif
    if (mch::C<ADTShape,ADTShape::circle>(x,a)(s))
        std::cout << "ADTCircle with center " << x << " and radius " << a << std::endl;
    else
    if (mch::C<ADTShape,ADTShape::square>(x,a)(s))
        std::cout << "ADTSquare with top left " << x << " and side " << a << std::endl;
    else
    if (mch::C<ADTShape,ADTShape::triangle>(x,y,z)(s))
        std::cout << "ADTTriangle with corners " << x << ',' << y << ',' << z << std::endl;
}

int main()
{
    Shape* c = new Circle(loc(1,1),4);
    Shape* s = new Square(loc(2,2),2);
    Shape* t = new Triangle(loc(0,0),loc(0,1),loc(1,0));

    loc lll;
    std::cout << lll;

    foo(c); std::cout << "Area: " << area(*c) << " Center: " << center(*c) << std::endl;
    foo(s); std::cout << "Area: " << area(*s) << " Center: " << center(*s) << std::endl;
    foo(t); std::cout << "Area: " << area(*t) << " Center: " << center(*t) << std::endl;

#ifdef POD_ONLY
    ADTShape ac = {ADTShape::circle,   {{{1,1}, 4}}};
    ADTShape as = {ADTShape::square,   {{{2,2}, 2}}};
    ADTShape at = {ADTShape::triangle, {{{1,1}, 1}}};
#else
    cloc l00 = {0,0};
    cloc l11 = {1,1};
    cloc l10 = {1,0};
    ADTShape ac(l11, 4);
    ADTShape as(2, l00);
    ADTShape at(l11, l10, l00);
#endif
    bar(ac);
    bar(as);
    bar(at);

    ADTShapeEx xc = ac;
    ADTShapeEx xs = as;
    ADTShapeEx xt = at;

    bar(xc);
    bar(xs);
    bar(xt);
}
