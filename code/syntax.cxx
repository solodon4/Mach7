///
/// \file
///
/// This file is a part of pattern matching testing suite.
///
/// \autor Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of the XTL framework (http://parasol.tamu.edu/xtl/).
/// Copyright (C) 2005-2011 Texas A&M University.
/// All rights reserved.
///

#include <ostream>
#include <utility>
#include "match_generic.hpp"

typedef std::pair<double,double> loc;

std::ostream& operator<<(std::ostream& os, const loc& l) { return os << '{' << l.first << ',' << l.second << '}'; }

//------------------------------------------------------------------------------

#if XTL_VISITOR_FORWARDING
    #define forwarding "Yes"
#else
    #define forwarding "No "
#endif

//------------------------------------------------------------------------------

#if XTL_DEFAULT_SYNTAX == 'S' || XTL_DEFAULT_SYNTAX == 'P' || XTL_DEFAULT_SYNTAX == 'K' || XTL_DEFAULT_SYNTAX == 'F' || XTL_DEFAULT_SYNTAX == 'U' || XTL_DEFAULT_SYNTAX == 'E'
    #define syntax "Special"
#else
    #define syntax "Generic"
#endif

//------------------------------------------------------------------------------

#if XTL_DEFAULT_SYNTAX == 'S' || XTL_DEFAULT_SYNTAX == 's'
    #define encoding "Sequential "
    #define XTL_S_ONLY(...) __VA_ARGS__
#else
    #define XTL_S_ONLY(...)
#endif

#if XTL_DEFAULT_SYNTAX == 'P' || XTL_DEFAULT_SYNTAX == 'p'
    #define encoding "Polymorphic"
    #define XTL_P_ONLY(...) __VA_ARGS__
#else
    #define XTL_P_ONLY(...)
#endif

#if XTL_DEFAULT_SYNTAX == 'K' || XTL_DEFAULT_SYNTAX == 'k'
    #define encoding "Kind       "
    #define XTL_K_ONLY(...) __VA_ARGS__
#else
    #define XTL_K_ONLY(...)
#endif

#if XTL_DEFAULT_SYNTAX == 'F' || XTL_DEFAULT_SYNTAX == 'f'
    #define encoding "Forwarding "
    #define XTL_F_ONLY(...) __VA_ARGS__
#else
    #define XTL_F_ONLY(...)
#endif

#if XTL_DEFAULT_SYNTAX == 'U' || XTL_DEFAULT_SYNTAX == 'u'
    #define encoding "Union      "
    #define XTL_U_ONLY(...) __VA_ARGS__
#else
    #define XTL_U_ONLY(...)
#endif

#if XTL_DEFAULT_SYNTAX == 'E' || XTL_DEFAULT_SYNTAX == 'e'
    #define encoding "Exception  "
    #define XTL_E_ONLY(...) __VA_ARGS__
#else
    #define XTL_E_ONLY(...)
#endif

#if XTL_DEFAULT_SYNTAX == 'K' || XTL_DEFAULT_SYNTAX == 'k' || XTL_DEFAULT_SYNTAX == 'F' || XTL_DEFAULT_SYNTAX == 'f'
    #define XTL_TAG_ONLY(...) __VA_ARGS__
#else
    #define XTL_TAG_ONLY(...)
#endif

#if XTL_DEFAULT_SYNTAX == 'P' || XTL_DEFAULT_SYNTAX == 'p' || XTL_DEFAULT_SYNTAX == 'S' || XTL_DEFAULT_SYNTAX == 's'
    #define XTL_OPEN_ONLY(...) __VA_ARGS__
#else
    #define XTL_OPEN_ONLY(...)
#endif

//------------------------------------------------------------------------------

#if XTL_DEFAULT_SYNTAX == 'U' || XTL_DEFAULT_SYNTAX == 'u'

struct cloc { double first; double second; };

std::ostream& operator<<(std::ostream& os, const cloc& l) { return os << '{' << l.first << ',' << l.second << '}'; }

enum shape_kind {UnknownShape, Circle, Square, Triangle} kind;

struct Shape
{
    shape_kind kind;

#ifndef POD_ONLY
	Shape(const cloc& c, double r) : kind(Circle), center(c), radius(r) {}
	Shape(double s, const cloc& c) : kind(Square), upper_left(c), size(s) {}
	Shape(const cloc& x, const cloc& y, const cloc& z) : kind(Triangle), first(x), second(y), third(z) {}
#endif

#ifndef _MSC_VER
	union
	{
		struct { cloc center;     double radius; }; // circle;
		struct { cloc upper_left; double size; };   // square;
		struct { cloc first, second, third; };      // triangle;
	};
#else
    // MSVC doesn't seem to allow anonymous structs inside union so we just dump
    // here same members name directly to make the rest just compile at least.
	cloc center;     double radius; // circle;
	cloc upper_left; double size;   // square;
	cloc first, second, third;      // triangle;
#endif

};

template <> struct match_members<Shape>          { KV(UnknownShape); KS(Shape::kind); };

template <> struct match_members<Shape,Circle>   { KV(Circle);  CM(0,Shape::center);     CM(1,Shape::radius); };
template <> struct match_members<Shape,Square>   { KV(Square);  CM(0,Shape::upper_left); CM(1,Shape::size); };
template <> struct match_members<Shape,Triangle> { KV(Triangle);CM(0,Shape::first);      CM(1,Shape::second); CM(2,Shape::third); };

#else

// An Algebraic Data Type implemented through inheritance
struct Shape
{
    XTL_TAG_ONLY(  enum ShapeKind {SK_Shape=1,SK_Circle,SK_Square,SK_Triangle}; )
    XTL_TAG_ONLY(  ShapeKind kind; )
    XTL_TAG_ONLY(  Shape(ShapeKind k) : kind(k) {} )
    XTL_OPEN_ONLY( virtual ~Shape() {} )
    XTL_E_ONLY(    virtual void raise() const = 0; ) // Polymorphic exception idiom: http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Polymorphic_Exception
};

struct Circle : Shape
{
    Circle(const loc& c, const double& r) : XTL_TAG_ONLY(Shape(SK_Circle),) center(c), radius(r) {}
    void raise() const { throw *this; }

    const loc& get_center() const { return center; }

    loc    center;
    double radius;
};

struct Square : Shape
{
    Square(const loc& c, const double& s) : XTL_TAG_ONLY(Shape(SK_Square),) upper_left(c), side(s) {}
    void raise() const { throw *this; }

    loc    upper_left;
    double side;
};

struct Triangle : Shape
{
    Triangle(const loc& a, const loc& b, const loc& c) : XTL_TAG_ONLY(Shape(SK_Triangle),) first(a), second(b), third(c) {}
    void raise() const { throw *this; }

    loc first;
    loc second;
    loc third;
};

XTL_TAG_ONLY(SKV(Shape,Shape::SK_Shape));

template <> struct match_members<Shape>    { XTL_TAG_ONLY(KS(Shape::kind)); XTL_TAG_ONLY(KV(Shape::SK_Shape)); XTL_E_ONLY(RS(Shape::raise)); };

template <> struct match_members<Circle>   { XTL_TAG_ONLY(KV(Shape::SK_Circle));   XTL_TAG_ONLY(BCS(Circle,  Shape)); CM(0,Circle::get_center); CM(1,Circle::radius); };
template <> struct match_members<Square>   { XTL_TAG_ONLY(KV(Shape::SK_Square));   XTL_TAG_ONLY(BCS(Square,  Shape)); CM(0,Square::upper_left); CM(1,Square::side);   };
template <> struct match_members<Triangle> { XTL_TAG_ONLY(KV(Shape::SK_Triangle)); XTL_TAG_ONLY(BCS(Triangle,Shape)); CM(0,Triangle::first);    CM(1,Triangle::second); CM(2,Triangle::third); };

#endif

//------------------------------------------------------------------------------

size_t do_match_1(const Shape& s, size_t)
{
    Match(s)
      XTL_USE_BRACES_ONLY({)
        Case(Circle,c,r)     std::cout << "Circle("   << c << ',' << r << ')'             << std::endl;
        //Case(Square,c,s)     std::cout << "Square("   << c << ',' << s << ')'             << std::endl;
        Case(Triangle,x,y,z) std::cout << "Triangle(" << x << ',' << y << ',' << z << ')' << std::endl;
        Otherwise()          std::cout << "Other()"                                       << std::endl;
      XTL_USE_BRACES_ONLY(})
    EndMatch
    return 0;
}

//------------------------------------------------------------------------------

#if XTL_DEFAULT_SYNTAX == 'U' || XTL_DEFAULT_SYNTAX == 'u'
Shape* make_shape(size_t i)
{
#ifdef POD_ONLY
    Shape ac = {Circle,   {{{1,1}, 7}}};
    Shape as = {Square,   {{{1,1}, 2}}};
    Shape at = {Triangle, {{{1,1}, 1}}};
#else
    cloc l00 = {0,0};
    cloc l11 = {1,1};
    cloc l10 = {1,0};
    Shape ac(l11, 7);
    Shape as(2, l11);
    Shape at(l11, l10, l00);
#endif

    switch (i%3)
    {
    case 0: return new Shape(ac);
    case 1: return new Shape(as);
    case 2: return new Shape(at);
    }
    return 0;
}
#else
Shape* make_shape(size_t i)
{
    switch (i%3)
    {
    case 0: return new Circle(loc(1,1),7);                  
    case 1: return new Square(loc(1,1),2);                  
    case 2: return new Triangle(loc(1,1),loc(1,0),loc(0,0));
    }
    return 0;
}
#endif

//------------------------------------------------------------------------------

#define msg_prefix "SYNTAX: " syntax " ENCODING: " encoding " FORWARDING: " forwarding " TEST: "

//------------------------------------------------------------------------------

int main()
{
    do_match_1(*make_shape(0), 0);
    do_match_1(*make_shape(1), 0);
    do_match_1(*make_shape(2), 0);
}

//------------------------------------------------------------------------------
