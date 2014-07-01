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
#include "type_switch3.hpp"

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

void do_match(const Shape* s0, const Shape* s1, const Shape* s2)
{
    const char* text = "unknown";

    Match3(s0,s1,s2)
    {
    Case3(Circle,   Circle  , Circle  ) text = "C,C,C"; break;
    Case3(Circle,   Circle  , Square  ) text = "C,C,S"; break;
    Case3(Circle,   Square  , Circle  ) text = "C,S,C"; break;
    Case3(Circle,   Square  , Square  ) text = "C,S,S"; break;
    Case3(Circle,   Square  , Triangle) text = "C,S,T"; break;
    Case3(Circle,   Triangle, Square  ) text = "C,T,S"; break;
    Case3(Circle,   Triangle, Triangle) text = "C,T,T"; break;
    Case3(Circle,   Triangle, Circle  ) text = "C,T,C"; break;
    Case3(Circle,   Circle  , Triangle) text = "C,C,T"; break;
                                                 
    //Case3(Square,   Circle  , Circle  ) text = "S,C,C"; break;
    //Case3(Square,   Circle  , Square  ) text = "S,C,S"; break;
    Case3(Square,   Square  , Circle  ) text = "S,S,C"; break;
    Case3(Square,   Square  , Square  ) text = "S,S,S"; break;
    Case3(Square,   Square  , Triangle) text = "S,S,T"; break;
    Case3(Square,   Triangle, Square  ) text = "S,T,S"; break;
    Case3(Square,   Triangle, Triangle) text = "S,T,T"; break;
    Case3(Square,   Triangle, Circle  ) text = "S,T,C"; break;
    Case3(Square,   Circle  , Triangle) text = "S,C,T"; break;

    Case3(Triangle, Circle  , Circle  ) text = "T,C,C"; break;
    Case3(Triangle, Circle  , Square  ) text = "T,C,S"; break;
    Case3(Triangle, Square  , Circle  ) text = "T,S,C"; break;
    Case3(Triangle, Square  , Square  ) text = "T,S,S"; break;
    Case3(Triangle, Square  , Triangle) text = "T,S,T"; break;
    Case3(Triangle, Triangle, Square  ) text = "T,T,S"; break;
    Case3(Triangle, Triangle, Triangle) text = "T,T,T"; break;
    Case3(Triangle, Triangle, Circle  ) text = "T,T,C"; break;
    Case3(Triangle, Circle  , Triangle) text = "T,C,T"; break;

    Otherwise3()                        text = "other"; break;
    }
    EndMatch3

    std::cout << text << std::endl;
}

//------------------------------------------------------------------------------

int main()
{
    Shape* c = new Circle(loc(1,1),7);
    Shape* s = new Square(loc(1,1),2);
    Shape* t = new Triangle(loc(1,1),loc(1,0),loc(0,0));

    Shape* shapes[] = {c,s,t};

    for (size_t i = 0; i < 3; ++i)
        for (size_t j = 0; j < 2; ++j)
            for (size_t k = 0; k < 2; ++k)
                do_match(shapes[i], shapes[j], shapes[k]);

    for (size_t i = 0; i < 3; ++i)
        for (size_t j = 0; j < 2; ++j)
            for (size_t k = 0; k < 2; ++k)
                do_match(shapes[i], shapes[j], shapes[k]);
}

//------------------------------------------------------------------------------
