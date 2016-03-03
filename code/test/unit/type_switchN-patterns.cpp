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

#include <iostream>
#include <utility>
#include <mach7/type_switchN-patterns.hpp> // Support for N-ary Match statement on patterns
#include <mach7/patterns/primitive.hpp>    // Wildcard, variable and value patterns

//------------------------------------------------------------------------------

typedef std::pair<double,double> loc;

//------------------------------------------------------------------------------

// An Algebraic Data Type implemented through inheritance
struct Shape
{
    virtual ~Shape() {}
};

//------------------------------------------------------------------------------

struct Circle : Shape
{
    Circle(const loc& c, const double& r) : center(c), radius(r) {}

    const loc& get_center() const { return center; }

    loc    center;
    double radius;
};

//------------------------------------------------------------------------------

struct Square : Shape
{
    Square(const loc& c, const double& s) : upper_left(c), side(s) {}

    loc    upper_left;
    double side;
};

//------------------------------------------------------------------------------

struct Triangle : Shape
{
    Triangle(const loc& a, const loc& b, const loc& c) : first(a), second(b), third(c) {}

    loc first;
    loc second;
    loc third;
};

//------------------------------------------------------------------------------

void do_match(const Shape* s0)
{
    const char* text = "unknown";

    mch::var<const Circle&>   c;
    mch::var<const Square&>   s;
    mch::var<const Triangle&> t;

    Match(s0)
    {
    Case(c) text = "C"; break;
    Case(s) text = "S"; break;
    Case(t) text = "T"; break;
    Otherwise() text = "other"; break;
    }
    EndMatch

    std::cout << text << std::endl;
}

//------------------------------------------------------------------------------

void do_match(const Shape* s0, const Shape* s1)
{
    const char* text = "unknown";

    mch::var<const Circle&>   c;
    mch::var<const Square&>   s;
    mch::var<const Triangle&> t;

    Match(s0,s1)
    {
  //Case(c, c) text = "C,C"; break;
    Case(c, s) text = "C,S"; break;
    Case(s, c) text = "S,C"; break;
  //Case(s, s) text = "S,S"; break;
    Case(s, t) text = "S,T"; break;
    Case(t, s) text = "T,S"; break;
    Case(t, t) text = "T,T"; break;
    Case(t, c) text = "T,C"; break;
    Case(c, t) text = "C,T"; break;
    Otherwise()text = "other"; break;
    }
    EndMatch

    std::cout << text << std::endl;
}

//------------------------------------------------------------------------------

void do_match(Shape* s0, Shape* s1, Shape* s2)
{
    const char* text = "unknown";

    mch::var<const Circle&>   c;
    mch::var<const Square&>   s;
    mch::var<const Triangle&> t;

    Match(s0,s1,s2)
    {
    Case(c, c, c) text = "C,C,C"; break;
    Case(c, c, s) text = "C,C,S"; break;
    Case(c, s, c) text = "C,S,C"; break;
    Case(c, s, s) text = "C,S,S"; break;
    Case(c, s, t) text = "C,S,T"; break;
    Case(c, t, s) text = "C,T,S"; break;
    Case(c, t, t) text = "C,T,T"; break;
    Case(c, t, c) text = "C,T,C"; break;
    Case(c, c, t) text = "C,C,T"; break;
             
  //Case(s, c, c) text = "S,C,C"; break;
  //Case(s, c, s) text = "S,C,S"; break;
    Case(s, s, c) text = "S,S,C"; break;
    Case(s, s, s) text = "S,S,S"; break;
    Case(s, s, t) text = "S,S,T"; break;
    Case(s, t, s) text = "S,T,S"; break;
    Case(s, t, t) text = "S,T,T"; break;
    Case(s, t, c) text = "S,T,C"; break;
  //Case(s, c, t) text = "S,C,T"; break;

    Case(t, c, c) text = "T,C,C"; break;
    Case(t, c, s) text = "T,C,S"; break;
    Case(t, s, c) text = "T,S,C"; break;
    Case(t, s, s) text = "T,S,S"; break;
    Case(t, s, t) text = "T,S,T"; break;
    Case(t, t, s) text = "T,T,S"; break;
    Case(t, t, t) text = "T,T,T"; break;
    Case(t, t, c) text = "T,T,C"; break;
    Case(t, c, t) text = "T,C,T"; break;

    Otherwise()   text = "other"; break;
    }
    EndMatch

    std::cout << text << std::endl;
}

//------------------------------------------------------------------------------

void do_match(Shape* s0, Shape* s1, Shape* s2, Shape* s3)
{
    const char* text = "unknown";

    mch::var<const Circle&>   c;
    mch::var<const Square&>   s;
    mch::var<const Triangle&> t;

    Match(s0,s1,s2,s3)
    {
    Case(c, c, c, c) text = "C,C,C,C"; break;
    Case(c, c, c, s) text = "C,C,C,S"; break;
    Case(c, c, s, c) text = "C,C,S,C"; break;
    Case(c, c, s, s) text = "C,C,S,S"; break;
    Case(c, c, s, t) text = "C,C,S,T"; break;
    Case(c, c, t, s) text = "C,C,T,S"; break;
    Case(c, c, t, t) text = "C,C,T,T"; break;
    Case(c, c, t, c) text = "C,C,T,C"; break;
    Case(c, c, c, t) text = "C,C,C,T"; break;
                                                       
    Case(c, s, c, c) text = "C,S,C,C"; break;
    Case(c, s, c, s) text = "C,S,C,S"; break;
    Case(c, s, s, c) text = "C,S,S,C"; break;
    Case(c, s, s, s) text = "C,S,S,S"; break;
    Case(c, s, s, t) text = "C,S,S,T"; break;
    Case(c, s, t, s) text = "C,S,T,S"; break;
    Case(c, s, t, t) text = "C,S,T,T"; break;
    Case(c, s, t, c) text = "C,S,T,C"; break;
    Case(c, s, c, t) text = "C,S,C,T"; break;
                                                           
    Case(c, t, c, c) text = "C,T,C,C"; break;
    Case(c, t, c, s) text = "C,T,C,S"; break;
    Case(c, t, s, c) text = "C,T,S,C"; break;
    Case(c, t, s, s) text = "C,T,S,S"; break;
    Case(c, t, s, t) text = "C,T,S,T"; break;
    Case(c, t, t, s) text = "C,T,T,S"; break;
    Case(c, t, t, t) text = "C,T,T,T"; break;
    Case(c, t, t, c) text = "C,T,T,C"; break;
    Case(c, t, c, t) text = "C,T,C,T"; break;
                                                     
                                                     
    Case(s, c, c, c) text = "S,C,C,C"; break;
    Case(s, c, c, s) text = "S,C,C,S"; break;
    Case(s, c, s, c) text = "S,C,S,C"; break;
    Case(s, c, s, s) text = "S,C,S,S"; break;
    Case(s, c, s, t) text = "S,C,S,T"; break;
    Case(s, c, t, s) text = "S,C,T,S"; break;
    Case(s, c, t, t) text = "S,C,T,T"; break;
    Case(s, c, t, c) text = "S,C,T,C"; break;
    Case(s, c, c, t) text = "S,C,C,T"; break;
                                                       
    Case(s, s, c, c) text = "S,S,C,C"; break;
    Case(s, s, c, s) text = "S,S,C,S"; break;
    Case(s, s, s, c) text = "S,S,S,C"; break;
    Case(s, s, s, s) text = "S,S,S,S"; break;
    Case(s, s, s, t) text = "S,S,S,T"; break;
    Case(s, s, t, s) text = "S,S,T,S"; break;
    Case(s, s, t, t) text = "S,S,T,T"; break;
    Case(s, s, t, c) text = "S,S,T,C"; break;
    Case(s, s, c, t) text = "S,S,C,T"; break;
                                                           
    Case(s, t, c, c) text = "S,T,C,C"; break;
    Case(s, t, c, s) text = "S,T,C,S"; break;
    Case(s, t, s, c) text = "S,T,S,C"; break;
    Case(s, t, s, s) text = "S,T,S,S"; break;
    Case(s, t, s, t) text = "S,T,S,T"; break;
    Case(s, t, t, s) text = "S,T,T,S"; break;
    Case(s, t, t, t) text = "S,T,T,T"; break;
    Case(s, t, t, c) text = "S,T,T,C"; break;
    Case(s, t, c, t) text = "S,T,C,T"; break;
                                                         
                                                         
    Case(t, c, c, c) text = "T,C,C,C"; break;
    Case(t, c, c, s) text = "T,C,C,S"; break;
    Case(t, c, s, c) text = "T,C,S,C"; break;
    Case(t, c, s, s) text = "T,C,S,S"; break;
    Case(t, c, s, t) text = "T,C,S,T"; break;
    Case(t, c, t, s) text = "T,C,T,S"; break;
    Case(t, c, t, t) text = "T,C,T,T"; break;
    Case(t, c, t, c) text = "T,C,T,C"; break;
    Case(t, c, c, t) text = "T,C,C,T"; break;
                                                           
    Case(t, s, c, c) text = "T,S,C,C"; break;
    Case(t, s, c, s) text = "T,S,C,S"; break;
    Case(t, s, s, c) text = "T,S,S,C"; break;
    Case(t, s, s, s) text = "T,S,S,S"; break;
    Case(t, s, s, t) text = "T,S,S,T"; break;
    Case(t, s, t, s) text = "T,S,T,S"; break;
    Case(t, s, t, t) text = "T,S,T,T"; break;
    Case(t, s, t, c) text = "T,S,T,C"; break;
    Case(t, s, c, t) text = "T,S,C,T"; break;
                                                           
    Case(t, t, c, c) text = "T,T,C,C"; break;
    Case(t, t, c, s) text = "T,T,C,S"; break;
    Case(t, t, s, c) text = "T,T,S,C"; break;
    Case(t, t, s, s) text = "T,T,S,S"; break;
    Case(t, t, s, t) text = "T,T,S,T"; break;
    Case(t, t, t, s) text = "T,T,T,S"; break;
    Case(t, t, t, t) text = "T,T,T,T"; break;
    Case(t, t, t, c) text = "T,T,T,C"; break;
    Case(t, t, c, t) text = "T,T,C,T"; break;
    
    Otherwise()      text = "other"; break;
    }
    EndMatch

    std::cout << text << std::endl;
}

//------------------------------------------------------------------------------

int main()
{
    Shape* c = new Circle(loc(1,1),7);
    Shape* s = new Square(loc(1,1),2);
    Shape* t = new Triangle(loc(1,1),loc(1,0),loc(0,0));

    Shape* shapes[] = {c,s,t};

    for (size_t n = 0; n < 3; n++)
    {
        // 1 argument
        for (size_t i = 0; i < 3; ++i)
            do_match(shapes[i]);
        
        // 2 arguments
        for (size_t i = 0; i < 3; ++i)
        for (size_t j = 0; j < 3; ++j)
            do_match(shapes[i], shapes[j]);
        
        // 3 arguments
        for (size_t i = 0; i < 3; ++i)
        for (size_t j = 0; j < 3; ++j)
        for (size_t k = 0; k < 3; ++k)
            do_match(shapes[i], shapes[j], shapes[k]);
        
        // 4 arguments
        for (size_t i = 0; i < 3; ++i)
        for (size_t j = 0; j < 3; ++j)
        for (size_t k = 0; k < 3; ++k)
        for (size_t l = 0; l < 3; ++l)
            do_match(shapes[i], shapes[j], shapes[k], shapes[l]);
    }
}

//------------------------------------------------------------------------------
