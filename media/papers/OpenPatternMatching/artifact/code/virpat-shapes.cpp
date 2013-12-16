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

#include <iostream>
#include <utility>
#include "virpat.hpp"

//------------------------------------------------------------------------------

template <typename A, typename B>
std::ostream& operator<<(std::ostream& os, const std::pair<A,B>& p)
{
    return os << '(' << p.first << ',' << p.second << ')';
}

//------------------------------------------------------------------------------

typedef std::pair<double,double> loc;

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

template <> struct bindings<Circle>   { CM(0,Circle::center);     CM(1,Circle::radius); };
template <> struct bindings<Square>   { CM(0,Square::upper_left); CM(1,Square::side);   };
template <> struct bindings<Triangle> { CM(0,Triangle::first);    CM(1,Triangle::second); CM(2,Triangle::third); };

// Members binding for std::pair and cloc C struct we use for unions case

template <typename X, typename Y> 
struct bindings<std::pair<X,Y>> { CM(0,std::pair<X,Y>::first);  CM(1,std::pair<X,Y>::second); };
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
