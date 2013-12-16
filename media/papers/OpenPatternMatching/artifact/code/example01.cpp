///
/// \file
///
/// This file is a part of pattern matching testing suite.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of Mach7 library (http://parasol.tamu.edu/mach7/).
/// Copyright (C) 2011-2013 Texas A&M University.
/// All rights reserved.
///

#include <utility>
#include "match.hpp"                // Support for Match statement

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
    Case(Circle)   return matched->center;
    Case(Square)   return matched->upper_left;
    Case(Triangle) return matched->first;
    Otherwise()    return loc(0,0);
    }
    EndMatch
}

//------------------------------------------------------------------------------

int main()
{
    point_within(new Triangle(loc(0,0),loc(1,0),loc(0,1)));
    point_within(new Square(loc(1,0),1));
    point_within(new Circle(loc(0,0),1));
}

//------------------------------------------------------------------------------
