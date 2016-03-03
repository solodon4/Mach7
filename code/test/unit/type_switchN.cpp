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
#include <mach7/type_switchN.hpp>          // Support for N-ary type switch statement

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

    Match(s0)
    {
    Case(Circle)   text = "C"; break;
    Case(Square)   text = "S"; break;
    Case(Triangle) text = "T"; break;
    Otherwise()    text = "other"; break;
    }
    EndMatch

    std::cout << text << std::endl;
}

//------------------------------------------------------------------------------

void do_match(const Shape* s0, const Shape* s1)
{
    const char* text = "unknown";

    Match(s0,s1)
    {
    Case(Circle  , Circle  ) text = "C,C"; break;
    Case(Circle  , Square  ) text = "C,S"; break;
    Case(Square  , Circle  ) text = "S,C"; break;
    Case(Square  , Square  ) text = "S,S"; break;
    Case(Square  , Triangle) text = "S,T"; break;
    Case(Triangle, Square  ) text = "T,S"; break;
    Case(Triangle, Triangle) text = "T,T"; break;
    Case(Triangle, Circle  ) text = "T,C"; break;
    Case(Circle  , Triangle) text = "C,T"; break;
    Otherwise()              text = "other"; break;
    }
    EndMatch

    std::cout << text << std::endl;
}

//------------------------------------------------------------------------------

void do_match(const Shape* s0, const Shape* s1, const Shape* s2)
{
    const char* text = "unknown";

    Match(s0,s1,s2)
    {
    Case(Circle,   Circle  , Circle  ) text = "C,C,C"; break;
    Case(Circle,   Circle  , Square  ) text = "C,C,S"; break;
    Case(Circle,   Square  , Circle  ) text = "C,S,C"; break;
    Case(Circle,   Square  , Square  ) text = "C,S,S"; break;
    Case(Circle,   Square  , Triangle) text = "C,S,T"; break;
    Case(Circle,   Triangle, Square  ) text = "C,T,S"; break;
    Case(Circle,   Triangle, Triangle) text = "C,T,T"; break;
    Case(Circle,   Triangle, Circle  ) text = "C,T,C"; break;
    Case(Circle,   Circle  , Triangle) text = "C,C,T"; break;

  //Case(Square,   Circle  , Circle  ) text = "S,C,C"; break;
  //Case(Square,   Circle  , Square  ) text = "S,C,S"; break;
    Case(Square,   Square  , Circle  ) text = "S,S,C"; break;
    Case(Square,   Square  , Square  ) text = "S,S,S"; break;
    Case(Square,   Square  , Triangle) text = "S,S,T"; break;
    Case(Square,   Triangle, Square  ) text = "S,T,S"; break;
    Case(Square,   Triangle, Triangle) text = "S,T,T"; break;
    Case(Square,   Triangle, Circle  ) text = "S,T,C"; break;
  //Case(Square,   Circle  , Triangle) text = "S,C,T"; break;

    Case(Triangle, Circle  , Circle  ) text = "T,C,C"; break;
    Case(Triangle, Circle  , Square  ) text = "T,C,S"; break;
    Case(Triangle, Square  , Circle  ) text = "T,S,C"; break;
    Case(Triangle, Square  , Square  ) text = "T,S,S"; break;
    Case(Triangle, Square  , Triangle) text = "T,S,T"; break;
    Case(Triangle, Triangle, Square  ) text = "T,T,S"; break;
    Case(Triangle, Triangle, Triangle) text = "T,T,T"; break;
    Case(Triangle, Triangle, Circle  ) text = "T,T,C"; break;
    Case(Triangle, Circle  , Triangle) text = "T,C,T"; break;

    Otherwise()                        text = "other"; break;
    }
    EndMatch

    std::cout << text << std::endl;
}

//------------------------------------------------------------------------------

void do_match(const Shape* s0, const Shape* s1, const Shape* s2, const Shape* s3)
{
    const char* text = "unknown";

    Match(s0,s1,s2,s3)
    {
    Case(Circle,   Circle,   Circle  , Circle  ) text = "C,C,C,C"; break;
    Case(Circle,   Circle,   Circle  , Square  ) text = "C,C,C,S"; break;
    Case(Circle,   Circle,   Square  , Circle  ) text = "C,C,S,C"; break;
    Case(Circle,   Circle,   Square  , Square  ) text = "C,C,S,S"; break;
    Case(Circle,   Circle,   Square  , Triangle) text = "C,C,S,T"; break;
    Case(Circle,   Circle,   Triangle, Square  ) text = "C,C,T,S"; break;
    Case(Circle,   Circle,   Triangle, Triangle) text = "C,C,T,T"; break;
    Case(Circle,   Circle,   Triangle, Circle  ) text = "C,C,T,C"; break;
    Case(Circle,   Circle,   Circle  , Triangle) text = "C,C,C,T"; break;
                                                           
    Case(Circle,   Square,   Circle  , Circle  ) text = "C,S,C,C"; break;
    Case(Circle,   Square,   Circle  , Square  ) text = "C,S,C,S"; break;
    Case(Circle,   Square,   Square  , Circle  ) text = "C,S,S,C"; break;
    Case(Circle,   Square,   Square  , Square  ) text = "C,S,S,S"; break;
    Case(Circle,   Square,   Square  , Triangle) text = "C,S,S,T"; break;
    Case(Circle,   Square,   Triangle, Square  ) text = "C,S,T,S"; break;
    Case(Circle,   Square,   Triangle, Triangle) text = "C,S,T,T"; break;
    Case(Circle,   Square,   Triangle, Circle  ) text = "C,S,T,C"; break;
    Case(Circle,   Square,   Circle  , Triangle) text = "C,S,C,T"; break;
                                                           
    Case(Circle,   Triangle, Circle  , Circle  ) text = "C,T,C,C"; break;
    Case(Circle,   Triangle, Circle  , Square  ) text = "C,T,C,S"; break;
    Case(Circle,   Triangle, Square  , Circle  ) text = "C,T,S,C"; break;
    Case(Circle,   Triangle, Square  , Square  ) text = "C,T,S,S"; break;
    Case(Circle,   Triangle, Square  , Triangle) text = "C,T,S,T"; break;
    Case(Circle,   Triangle, Triangle, Square  ) text = "C,T,T,S"; break;
    Case(Circle,   Triangle, Triangle, Triangle) text = "C,T,T,T"; break;
    Case(Circle,   Triangle, Triangle, Circle  ) text = "C,T,T,C"; break;
    Case(Circle,   Triangle, Circle  , Triangle) text = "C,T,C,T"; break;
                                                           
                                                           
    Case(Square,   Circle,   Circle  , Circle  ) text = "S,C,C,C"; break;
    Case(Square,   Circle,   Circle  , Square  ) text = "S,C,C,S"; break;
    Case(Square,   Circle,   Square  , Circle  ) text = "S,C,S,C"; break;
    Case(Square,   Circle,   Square  , Square  ) text = "S,C,S,S"; break;
    Case(Square,   Circle,   Square  , Triangle) text = "S,C,S,T"; break;
    Case(Square,   Circle,   Triangle, Square  ) text = "S,C,T,S"; break;
    Case(Square,   Circle,   Triangle, Triangle) text = "S,C,T,T"; break;
    Case(Square,   Circle,   Triangle, Circle  ) text = "S,C,T,C"; break;
    Case(Square,   Circle,   Circle  , Triangle) text = "S,C,C,T"; break;
                                                           
    Case(Square,   Square,   Circle  , Circle  ) text = "S,S,C,C"; break;
    Case(Square,   Square,   Circle  , Square  ) text = "S,S,C,S"; break;
    Case(Square,   Square,   Square  , Circle  ) text = "S,S,S,C"; break;
    Case(Square,   Square,   Square  , Square  ) text = "S,S,S,S"; break;
    Case(Square,   Square,   Square  , Triangle) text = "S,S,S,T"; break;
    Case(Square,   Square,   Triangle, Square  ) text = "S,S,T,S"; break;
    Case(Square,   Square,   Triangle, Triangle) text = "S,S,T,T"; break;
    Case(Square,   Square,   Triangle, Circle  ) text = "S,S,T,C"; break;
    Case(Square,   Square,   Circle  , Triangle) text = "S,S,C,T"; break;
                                                           
    Case(Square,   Triangle, Circle  , Circle  ) text = "S,T,C,C"; break;
    Case(Square,   Triangle, Circle  , Square  ) text = "S,T,C,S"; break;
    Case(Square,   Triangle, Square  , Circle  ) text = "S,T,S,C"; break;
    Case(Square,   Triangle, Square  , Square  ) text = "S,T,S,S"; break;
    Case(Square,   Triangle, Square  , Triangle) text = "S,T,S,T"; break;
    Case(Square,   Triangle, Triangle, Square  ) text = "S,T,T,S"; break;
    Case(Square,   Triangle, Triangle, Triangle) text = "S,T,T,T"; break;
    Case(Square,   Triangle, Triangle, Circle  ) text = "S,T,T,C"; break;
    Case(Square,   Triangle, Circle  , Triangle) text = "S,T,C,T"; break;
                                                           
                                                           
    Case(Triangle, Circle,   Circle  , Circle  ) text = "T,C,C,C"; break;
    Case(Triangle, Circle,   Circle  , Square  ) text = "T,C,C,S"; break;
    Case(Triangle, Circle,   Square  , Circle  ) text = "T,C,S,C"; break;
    Case(Triangle, Circle,   Square  , Square  ) text = "T,C,S,S"; break;
    Case(Triangle, Circle,   Square  , Triangle) text = "T,C,S,T"; break;
    Case(Triangle, Circle,   Triangle, Square  ) text = "T,C,T,S"; break;
    Case(Triangle, Circle,   Triangle, Triangle) text = "T,C,T,T"; break;
    Case(Triangle, Circle,   Triangle, Circle  ) text = "T,C,T,C"; break;
    Case(Triangle, Circle,   Circle  , Triangle) text = "T,C,C,T"; break;
                                                           
    Case(Triangle, Square,   Circle  , Circle  ) text = "T,S,C,C"; break;
    Case(Triangle, Square,   Circle  , Square  ) text = "T,S,C,S"; break;
    Case(Triangle, Square,   Square  , Circle  ) text = "T,S,S,C"; break;
    Case(Triangle, Square,   Square  , Square  ) text = "T,S,S,S"; break;
    Case(Triangle, Square,   Square  , Triangle) text = "T,S,S,T"; break;
    Case(Triangle, Square,   Triangle, Square  ) text = "T,S,T,S"; break;
    Case(Triangle, Square,   Triangle, Triangle) text = "T,S,T,T"; break;
    Case(Triangle, Square,   Triangle, Circle  ) text = "T,S,T,C"; break;
    Case(Triangle, Square,   Circle  , Triangle) text = "T,S,C,T"; break;
                                                           
    Case(Triangle, Triangle, Circle  , Circle  ) text = "T,T,C,C"; break;
    Case(Triangle, Triangle, Circle  , Square  ) text = "T,T,C,S"; break;
    Case(Triangle, Triangle, Square  , Circle  ) text = "T,T,S,C"; break;
    Case(Triangle, Triangle, Square  , Square  ) text = "T,T,S,S"; break;
    Case(Triangle, Triangle, Square  , Triangle) text = "T,T,S,T"; break;
    Case(Triangle, Triangle, Triangle, Square  ) text = "T,T,T,S"; break;
    Case(Triangle, Triangle, Triangle, Triangle) text = "T,T,T,T"; break;
    Case(Triangle, Triangle, Triangle, Circle  ) text = "T,T,T,C"; break;
    Case(Triangle, Triangle, Circle  , Triangle) text = "T,T,C,T"; break;
    
    Otherwise()                                  text = "other";   break;
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
