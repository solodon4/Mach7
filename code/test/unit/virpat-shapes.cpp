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

//------------------------------------------------------------------------------

template <typename A, typename B>
std::ostream& operator<<(std::ostream& os, const std::pair<A,B>& p)
{
    return os << '(' << p.first << ',' << p.second << ')';
}

//------------------------------------------------------------------------------

typedef std::pair<double,double> loc;

//------------------------------------------------------------------------------

#include "../time/virpat.hpp"

//------------------------------------------------------------------------------

struct Shape : object
{
    typedef Shape this_type;

    virtual bool operator==(const object& obj) const
    {
        if (typeid(*this) == typeid(obj))
            return true;
        else
            return false;
    }
    virtual std::ostream& operator>>(std::ostream& os) const
    {
        return os << "Shape()" << std::endl;
    }

};

//------------------------------------------------------------------------------

struct Circle : Shape
{
    typedef Circle this_type;

    Circle(const loc& c = loc(), const double& r = 0.0) : center(c), radius(r) {}

    virtual bool operator==(const object& obj) const
    {
        if (typeid(*this) == typeid(obj))
        {
            const Circle* p = static_cast<const Circle*>(&obj);
            return radius == p->radius && center == p->center;
        }

        return false;
    }
    virtual std::ostream& operator>>(std::ostream& os) const
    {
        return os << "Circle()" << std::endl;
    }

    object_of<loc>    center;
    object_of<double> radius;
};

//------------------------------------------------------------------------------

struct Square : Shape
{
    typedef Square this_type;

    Square(const loc& c = loc(), const double& s = 0.0) : upper_left(c), side(s) {}

    virtual bool operator==(const object& obj) const
    {
        if (typeid(*this) == typeid(obj))
        {
            const Square* p = static_cast<const Square*>(&obj);
            return side == p->side && upper_left == p->upper_left;
        }

        return false;
    }
    virtual std::ostream& operator>>(std::ostream& os) const
    {
        return os << "Square()" << std::endl;
    }

    object_of<loc>    upper_left;
    object_of<double> side;
};

//------------------------------------------------------------------------------

struct Triangle : Shape
{
    typedef Triangle this_type;

    Triangle(const loc& a = loc(), const loc& b = loc(), const loc& c = loc()) : first(a), second(b), third(c) {}

    virtual bool operator==(const object& obj) const
    {
        if (typeid(*this) == typeid(obj))
        {
            const Triangle* p = static_cast<const Triangle*>(&obj);
            return first == p->first && second == p->second && third == p->third;
        }

        return false;
    }
    virtual std::ostream& operator>>(std::ostream& os) const
    {
        return os << "Triangle()" << std::endl;
    }

    object_of<loc> first;
    object_of<loc> second;
    object_of<loc> third;
};

//------------------------------------------------------------------------------

namespace mch ///< Mach7 library namespace
{
template <> struct bindings<Shape>    {};

template <> struct bindings<Circle>   { Members(Circle::center    , Circle::radius); };
template <> struct bindings<Square>   { Members(Square::upper_left, Square::side);   };
template <> struct bindings<Triangle> { Members(Triangle::first   , Triangle::second, Triangle::third); };

// Members binding for std::pair and cloc C struct we use for unions case

template <typename X, typename Y>
struct bindings<std::pair<X,Y>> { typedef std::pair<X,Y> arg_type; Members(arg_type::first, arg_type::second); };
} // of namespace mch

//------------------------------------------------------------------------------

int foo(const object& obj)
{
    ref_of<Circle>   c;

    if (c.matches(obj)) return 1;

    ref_of<Square>   s;

    if (s.matches(obj)) return 2;

    ref_of<Triangle> t;

    if (t.matches(obj)) return 3;

    ref_of<Shape>    p;

    if (p.matches(obj)) return 4;

    return 0;
}

//------------------------------------------------------------------------------

int main()
{
    object* objects[5] = {new Circle(loc(4,4),44), new Square(loc(2,2),22), new Triangle(loc(1,1),loc(2,1),loc(2,2)), new Shape(), new object_of<int>(42)};

    std::cout << foo(*objects[0]) << std::endl;
    std::cout << foo(*objects[1]) << std::endl;
    std::cout << foo(*objects[2]) << std::endl;
    std::cout << foo(*objects[3]) << std::endl;
    std::cout << foo(*objects[4]) << std::endl;

    for (size_t i = 0; i < 5; ++i)
    {
        var_of<loc>    v;
        var_of<double> d;
        std::cout << (cls_of0<Circle>()  .matches(*objects[i]) ? 'C' : '.')
                  << (cls_of0<Square>()  .matches(*objects[i]) ? 'S' : '.')
                  << (cls_of0<Triangle>().matches(*objects[i]) ? 'T' : '.');
        if (cls_of2<Circle>(v,d).matches(*objects[i])) std::cout << 'c' << v.m_var << ':' << d;
        if (cls_of1<Square>(v)  .matches(*objects[i])) std::cout << 's' << v.m_var;
        if (cls_of1<Triangle>(v).matches(*objects[i])) std::cout << 't' << v.m_var;
        std::cout << std::endl;
    }

    
}

//------------------------------------------------------------------------------
