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
#include "patterns/combinators.hpp" // Support for pattern combinators
#include "patterns/constructor.hpp" // Support for constructor patterns
#include "patterns/guard.hpp"       // Support for guard patterns
#include "patterns/n+k.hpp"         // Support for n+k patterns
#include <iostream>
#include <utility>

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
    enum ShapeKind {SK_Circle,SK_Square,SK_Triangle};
    ShapeKind kind;
    Shape(ShapeKind k) : kind(k) {}
    virtual void accept(ShapeVisitor& v) = 0;
};

struct Circle : Shape
{
    Circle(const loc& c, const double& r) : Shape(SK_Circle), center(c), radius(r) {}

    void accept(ShapeVisitor& v) { v.visit(*this); }

    const loc& get_center() const { return center; }

    loc  center;
    double radius;
};

struct Square : Shape
{
    Square(const loc& c, const double& s) : Shape(SK_Square), upper_left(c), side(s) {}

    void accept(ShapeVisitor& v) { v.visit(*this); }

    loc  upper_left;
    double side;
};

struct Triangle : Shape
{
    Triangle(const loc& a, const loc& b, const loc& c) : Shape(SK_Triangle), first(a), second(b), third(c) {}

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

namespace mch ///< Mach7 library namespace
{
template <> struct bindings<Shape>    { /*KS(Shape::kind);*/ };

template <> struct bindings<Circle>   { KV(Shape,Shape::SK_Circle);  CM(0,Circle::get_center); CM(1,Circle::radius); };
template <> struct bindings<Square>   { KV(Shape,Shape::SK_Square);  CM(0,Square::upper_left); CM(1,Square::side);   };
template <> struct bindings<Triangle> { KV(Shape,Shape::SK_Triangle);CM(0,Triangle::first);    CM(1,Triangle::second); CM(2,Triangle::third); };

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

    ADTShape* adtshapes[] = {&ac,&as,&at};

    Shape* c = new Circle(loc(1,1),7);
    Shape* s = new Square(loc(1,1),2);
    Shape* t = new Triangle(loc(1,1),loc(1,0),loc(0,0));

    const Shape* shapes[] = {c,s,t};

    mch::wildcard _;
    double   x;
    mch::var<double> v;
    loc      l;
    cloc     cl;

    double m = 0.0;

    for (size_t i = 0; i < 3; ++i)
    {
        MatchP(shapes[i])
        {
        CaseP(Circle)   std::cout << "Circle"   << std::endl; m += matched->radius;      break;
        CaseP(Square)   std::cout << "Square"   << std::endl; m += matched->side;        break;
        CaseP(Triangle) std::cout << "Triangle" << std::endl; m += matched->first.first; break;
        CaseP(Triangle) std::cout << "Triangle" << std::endl; m += matched->first.first; break; // NOTE: Possible to have another type case match
        }
        EndMatchP
    }

    std::cout << m << std::endl;

    std::cout << "CASES" << std::endl;

    m = 0.0;

    for (size_t i = 0; i < 3; ++i)
    {
        MatchP(shapes[i])
        {
        CaseP(Circle,_,r)    std::cout << "Circle"   << std::endl; m += r;       XTL_UNUSED(_); break;
        CaseP(Square,_,r)    std::cout << "Square"   << std::endl; m += r;       XTL_UNUSED(_); break;
        CaseP(Triangle,p)    std::cout << "Triangle" << std::endl; m += p.first; break;
        CaseP(Triangle,p)    std::cout << "Triangle" << std::endl; m += p.first; break; // NOTE: Possible to have another regular match
        }
        EndMatchP
     }

    std::cout << m << std::endl;

    // Open/Closed case with bound variables explicitly declared

    m = 0.0;

    for (size_t i = 0; i < 3; ++i)
    {
        Match(shapes[i])
        {
            Qua(Circle,_,x)          std::cout << "Circle"   << std::endl; m += x;       break;
            Qua(Square,_,v |= v > 5) std::cout << "Square>5" << std::endl; m += v;       break;
             When(     _,v |= v > 3) std::cout << "Square>3" << std::endl; m += v;       break;
             When(     _,v |= v > 1) std::cout << "Square>1" << std::endl; m += v;       break;
             When(     _,v |= v > 0) std::cout << "Square>0" << std::endl; m += v;       break;
             When(     _,x)          std::cout << "Square"   << std::endl; m += x;       break;
            Qua(Triangle,l)          std::cout << "Triangle" << std::endl; m += l.first; break;
            Otherwise()              std::cout << "Other"    << std::endl; m += 2;       break;
        }
        EndMatch
    }

    std::cout << m << std::endl;

    // Open/Closed case with bound variables automatically declared

    m = 0.0;

    for (size_t i = 0; i < 3; ++i)
    {
        Match(shapes[i])
        {
            Case(Circle,c,r)  std::cout << "Circle"   << std::endl; m += r;       XTL_UNUSED(c); break;
            Case(Square,c,s)  std::cout << "Square"   << std::endl; m += s;       XTL_UNUSED(c); break;
            Case(Triangle,p)  std::cout << "Triangle" << std::endl; m += p.first; break;
            Otherwise()       std::cout << "Other"    << std::endl; m += 2;       break;
        }
        EndMatch
    }

    std::cout << m << std::endl;

// Disambiguation between value and type in first argument of Match macro is only possible when Match resolves to generic one
#if XTL_DEFAULT_SYNTAX == 'G'

    // Union case with bound variables explicitly declared

    m = 0.0;

    for (size_t i = 0; i < 3; ++i)
    {
        //std::cout << "Selector: " << adtshapes[i]->kind << std::endl;
        Match(adtshapes[i])
        {
            // FIX: Otherwise is not supported at the moment on discriminated 
            //      unions as we use default to jump back for rematching for 
            //      closed case and enter the fall-through behavior for open case
            //Otherwise()                        std::cout << "Other"       << std::endl; m += 2;       break;
            Qua(ADTShape::circle,_,x)          std::cout << "ADTCircle"   << std::endl; m += x;       break;
            Qua(ADTShape::square,_,v |= v > 5) std::cout << "ADTSquare>5" << std::endl; m += v;       break;
             When(               _,v |= v > 3) std::cout << "ADTSquare>3" << std::endl; m += v;       break;
             When(               _,v |= v > 1) std::cout << "ADTSquare>1" << std::endl; m += v;       break;
             When(               _,v |= v > 0) std::cout << "ADTSquare>0" << std::endl; m += v;       break;
             When(               _,x)          std::cout << "ADTSquare"   << std::endl; m += x;       break;
            Qua(ADTShape::triangle,cl)         std::cout << "ADTTriangle" << std::endl; m += cl.first;break;
        }
        EndMatch
    }

    std::cout << m << std::endl;

    // Union case with bound variables automatically declared

    m = 0.0;

    for (size_t i = 0; i < 3; ++i)
    {
        Match(adtshapes[i])
        {
            Case(ADTShape::circle,c,r)  std::cout << "ADTCircle"   << std::endl; m += r;       XTL_UNUSED(c); break;
            Case(ADTShape::square,c,s)  std::cout << "ADTSquare"   << std::endl; m += s;       XTL_UNUSED(c); break;
            Case(ADTShape::triangle,p)  std::cout << "ADTTriangle" << std::endl; m += p.first; break;
#if XTL_DEFAULT_SYNTAX != 'G'
            // FIX: Otherwise is not supported at the moment on discriminated 
            //      unions as we use default to jump back for rematching for 
            //      closed case and enter the fall-through behavior for open case
            Otherwise()                 std::cout << "Other"       << std::endl; m += 2;       break;
#endif
        }
        EndMatch
    }

    std::cout << m << std::endl;

#endif

    m = 0.0;

    for (size_t i = 0; i < 3; ++i)
    {
        Match(shapes[i])
        {
            Qua(Circle,_,x)
                std::cout << "Circle"   << std::endl;
                m += x;
                break;
            Qua(Square,_,v |= v > 5)
                std::cout << "Square>5" << std::endl;
                m += v;
                break;
              When(     _,v |= v > 3)
                std::cout << "Square>3" << std::endl;
                m += v;
                break;
              When(     _,v |= v > 1)
                std::cout << "Square>1" << std::endl;
                m += v;
                break;
              When(     _,v |= v > 0)
                std::cout << "Square>0" << std::endl;
                m += v;
                break;
              When(     _,x)
                std::cout << "Square"   << std::endl;
                m += x;
                break;
            Qua(Triangle,l)
                std::cout << "Triangle" << std::endl;
                m += l.first;
                break;
            Otherwise()
                std::cout << "Other"    << std::endl;
                m += 2;
                break;
        }
        EndMatch
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
