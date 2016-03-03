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

#include <utility>

// Enable declarations in case clauses, which are disabled by default
#define XTL_CLAUSE_DECL 1

#include <mach7/match.hpp>                 // Support for Match statement

//------------------------------------------------------------------------------

typedef std::pair<double,double> loc;

// An Algebraic Data Type implemented through inheritance
struct Shape
{
    virtual ~Shape() {}
};

struct Circle : Shape
{
    Circle(const loc& c, const double& r) : center(c), radius(r) {}
    loc    center;
    double radius;
};

struct Square : Shape
{
    Square(const loc& c, const double& s) : upper_left(c), side(s) {}
    loc    upper_left;
    double side;
};

struct Triangle : Shape
{
    Triangle(const loc& a, const loc& b, const loc& c) : first(a), second(b), third(c) {}
    loc first;
    loc second;
    loc third;
};

//------------------------------------------------------------------------------

loc point_within(const Shape* shape)
{
    Match(shape)
    {
    Case(const Circle&   x) return x.center;
    Case(const Square&   x) return x.upper_left;
    Case(const Triangle& x) return x.first;
    Otherwise()             return loc(0,0);
    }
    EndMatch

    XTL_UNREACHABLE; // To avoid warning that control may reach end of a non-void function
}

//------------------------------------------------------------------------------

int main()
{
    point_within(new Triangle(loc(0,0),loc(1,0),loc(0,1)));
    point_within(new Square(loc(1,0),1));
    point_within(new Circle(loc(0,0),1));
}

//------------------------------------------------------------------------------
