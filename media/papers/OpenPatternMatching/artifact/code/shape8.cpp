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

#include "match.hpp"                // Support for Match statement
#include "patterns/primitive.hpp"   // Support for primitive patterns
#include <iostream>
#include <utility>
#include <vector>

typedef std::pair<double,double> loc;
struct cloc { double first; double second; };

struct Shape;
struct Circle;
struct Square;
struct Triangle;

struct ShapeVisitor
{
	virtual void visit(const Circle&)   {}
	virtual void visit(const Square&)   {}
	virtual void visit(const Triangle&) {}
};

// An Algebraic Data Type implemented through inheritance
struct Shape
{
    enum ShapeKind {SK_Shape=1,SK_Circle,SK_Square,SK_Triangle};
    ShapeKind kind;
    const size_t* m_all_kinds;
    Shape(ShapeKind k) : kind(k), m_all_kinds(0) {}
    virtual void accept(ShapeVisitor& v) = 0;
};

struct Circle : Shape
{
    Circle(const loc& c, const double& r) : Shape(SK_Circle), center(c), radius(r) { m_all_kinds = (const size_t*)mch::get_kinds<Shape>(mch::lbl_type(SK_Circle)); }

    void accept(ShapeVisitor& v) { v.visit(*this); }

    const loc& get_center() const { return center; }

    loc  center;
    double radius;
};

struct Square : Shape
{
    Square(const loc& c, const double& s) : Shape(SK_Square), upper_left(c), side(s) { m_all_kinds = (const size_t*)mch::get_kinds<Shape>(mch::lbl_type(SK_Square)); }

    void accept(ShapeVisitor& v) { v.visit(*this); }

    loc  upper_left;
    double side;
};

struct Triangle : Shape
{
    Triangle(const loc& a, const loc& b, const loc& c) : Shape(SK_Triangle), first(a), second(b), third(c) { m_all_kinds = (const size_t*)mch::get_kinds<Shape>(mch::lbl_type(SK_Triangle)); }

    void accept(ShapeVisitor& v) { v.visit(*this); }

    loc first;
    loc second;
    loc third;
};

struct ADTShape
{
	enum shape_kind {circle, square, triangle} kind;

#ifndef POD_ONLY
	ADTShape(const cloc& c, double r) : kind(circle), center(c), radius(r) {}
	ADTShape(double s, const cloc& c) : kind(square), upper_left(c), size(s) {}
	ADTShape(const cloc& x, const cloc& y, const cloc& z) : kind(triangle), first(x), second(y), third(z) {}
	virtual ~ADTShape() {}
#endif

#ifndef _MSC_VER
	union
	{
		struct { cloc center;     double radius; }; // as_circle;
		struct { cloc upper_left; double size; }; //   as_square;
		struct { cloc first, second, third; }; //    as_triangle;
	};
#else
    // MSVC doesn't seem to allow anonymous structs inside union so we just dump
    // here same members name directly to make the rest just compile at least.
	cloc center;     double radius; // as_circle;
	cloc upper_left; double size;   // as_square;
	cloc first, second, third;    // as_triangle;
#endif
};

SKV(Shape,Shape::SK_Shape);

namespace mch ///< Mach7 library namespace
{
template <> struct bindings<Shape>    { KS(Shape::kind); KV(Shape,Shape::SK_Shape); };

template <> struct bindings<Circle>   { KV(Shape,Shape::SK_Circle);   BCS(Circle,  Shape); CM(0,Circle::get_center); CM(1,Circle::radius); };
template <> struct bindings<Square>   { KV(Shape,Shape::SK_Square);   BCS(Square,  Shape); CM(0,Square::upper_left); CM(1,Square::side);   };
template <> struct bindings<Triangle> { KV(Shape,Shape::SK_Triangle); BCS(Triangle,Shape); CM(0,Triangle::first);    CM(1,Triangle::second); CM(2,Triangle::third); };

template <> struct bindings<ADTShape> { KS(ADTShape::kind); };

template <> struct bindings<ADTShape,ADTShape::circle>   { KV(ADTShape,ADTShape::circle);  CM(0,ADTShape::center);     CM(1,ADTShape::radius); };
template <> struct bindings<ADTShape,ADTShape::square>   { KV(ADTShape,ADTShape::square);  CM(0,ADTShape::upper_left); CM(1,ADTShape::size); };
template <> struct bindings<ADTShape,ADTShape::triangle> { KV(ADTShape,ADTShape::triangle);CM(0,ADTShape::first);      CM(1,ADTShape::second); CM(2,ADTShape::third); };
} // of namespace mch

int main()
{
#ifdef POD_ONLY
    ADTShape ac = {ADTShape::circle,   {{{1,1}, 7}}};
    ADTShape as = {ADTShape::square,   {{{1,1}, 2}}};
    ADTShape at = {ADTShape::triangle, {{{1,1}, 1}}};
#else
    cloc l00 = {0,0};
    cloc l11 = {1,1};
    cloc l10 = {1,0};
    ADTShape ac(l11, 7);
    ADTShape as(2, l11);
    ADTShape at(l11, l10, l00);
#endif

    //ADTShape* adtshapes[] = {&ac,&as,&at};

    Shape* c = new Circle(loc(1,1),7);
    Shape* s = new Square(loc(1,1),2);
    Shape* t = new Triangle(loc(1,1),loc(1,0),loc(0,0));

    const Shape* shapes[] = {c,s,t};

    //wildcard _;
    //double   x;
    mch::var<double> v;
    loc      l;
    //cloc     cl;

    double m = 0.0;

    for (size_t i = 0; i < 3; ++i)
    {
        MatchQ(shapes[i])
        {
        //CaseQ(Shape)         std::cout << "Shape"    << std::endl; m += 42;      break;
        CaseQ(Circle,_,r)    std::cout << "Circle"   << std::endl; m += r;       XTL_UNUSED(_); break;
        CaseQ(Square,_,r)    std::cout << "Square"   << std::endl; m += r;       XTL_UNUSED(_); break;
        CaseQ(Triangle,p)    std::cout << "Triangle" << std::endl; m += p.first; break;
        OtherwiseQ()         std::cout << "Shape"    << std::endl; m += 42;      break;
        }
        EndMatchQ
     }

    std::cout << m << std::endl;
}

void test_read(const Shape* shape)
{
    Match(shape)
    {
        Qua(Circle)   const Circle*   s = matched; XTL_UNUSED(s); break;
        Qua(Square)   const Square*   s = matched; XTL_UNUSED(s); break;
        Qua(Triangle) const Triangle* s = matched; XTL_UNUSED(s); break;
        Otherwise()   const Shape*    s = matched; XTL_UNUSED(s); break;
    }
    EndMatch
}

void test_write(Shape* shape)
{
    Match(shape)
    {
        Qua(Circle)         Circle*   s = matched; XTL_UNUSED(s); break;
        Qua(Square)         Square*   s = matched; XTL_UNUSED(s); break;
        Qua(Triangle)       Triangle* s = matched; XTL_UNUSED(s); break;
        Otherwise()         Shape*    s = matched; XTL_UNUSED(s); break;
    }
    EndMatch
}

void test_autodecl(const Shape* shape)
{
    double m = 0.0;

    Match(shape)
    {
        Case(Circle,c,r)  std::cout << "Circle"   << std::endl; m += r;       XTL_UNUSED(c); break;
        Case(Square,c,s)  std::cout << "Square"   << std::endl; m += s;       XTL_UNUSED(c); break;
        Case(Triangle,p)  std::cout << "Triangle" << std::endl; m += p.first; break;
        Otherwise()       std::cout << "Other"    << std::endl; m += 2;       break;
    }
    EndMatch
}
