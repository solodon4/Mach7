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
#include "patterns/constructor.hpp" // Support for constructor patterns
#include "timing.hpp"
#include "shape_bindings.hpp"
#include <cmath>
#include <iostream>

//#define POD_ONLY

mch::wildcard _; // Meta variable

std::ostream& operator<<(std::ostream& os, const loc& l)
{
    return os << '(' << l.first << ',' << l.second << ')';
}

std::ostream& operator<<(std::ostream& os, const cloc& l)
{
    return os << '(' << l.first << ',' << l.second << ')';
}

double heron(const loc& l1, const loc& l2, const loc& l3) 
{
    double a2 = (l1.first-l2.first)*(l1.first-l2.first) + (l1.second-l2.second)*(l1.second-l2.second);
    double b2 = (l2.first-l3.first)*(l2.first-l3.first) + (l2.second-l3.second)*(l2.second-l3.second);
    double c2 = (l3.first-l1.first)*(l3.first-l1.first) + (l3.second-l1.second)*(l3.second-l1.second);
    return std::sqrt((a2+b2+c2)*(a2+b2+c2) - 2.0*(a2*a2+b2*b2+c2*c2))/4.0;
}

double area(const Shape& shape)
{
    loc  x,y,z;
    double r,s;

    if (mch::C<Circle>(_,r)(shape))
        return 3.14 * r * r;

    if (mch::C<Square>(_,s)(shape))
        return s * s;

    if (mch::C<Triangle>(x,y,z)(shape))
        return heron(x,y,z);

    XTL_ASSERT(!"Inexhaustive search");
    return 0.0;
}
/*
double area_ptr(const Shape& shape)
{
    loc    *x,*y,*z;
    double *r,*s;

    if (mch::C<Circle>(_,r)(shape))
        return 3.14 * *r * *r;

    if (mch::C<Square>(_,s)(shape))
        return *s * *s;

    if (mch::C<Triangle>(x,y,z)(shape))
        return heron(*x,*y,*z);

    XTL_ASSERT(!"Inexhaustive search");
    return 0.0;
}
*/
loc center(/*const*/ Shape& shape)
{
    loc c;

    if (mch::C<Circle>(c/*,_*/)(shape))
        return c;

    double x,y,s;

    if (mch::C<Square>(
            mch::C<loc>(x,y),s)(shape))
        return loc(x+s/2,y+s/2);

    double x1,y1,x2,y2,x3,y3;

    if (mch::C<Triangle>(
            mch::C<loc>(x1,y1),
            mch::C<loc>(x2,y2),
            mch::C<loc>(x3,y3)
                       )(shape))
        return loc((x1+x2+x3)/3,(y1+y2+y3)/3);

    XTL_ASSERT(!"Inexhaustive search");
    return loc();
}

int dummy(Shape* s)
{
    if (mch::C<Circle>()(s))   return 1;
    if (mch::C<Square>()(s))   return 2;
    if (mch::C<Triangle>()(s)) return 3;
    XTL_ASSERT(!"Inexhaustive search");
    return 0;
}

int dummy_dyn(Shape* s)
{
    if (dynamic_cast<Circle*>(s))   return 1;
    if (dynamic_cast<Square*>(s))   return 2;
    if (dynamic_cast<Triangle*>(s)) return 3;
    XTL_ASSERT(!"Inexhaustive search");
    return 0;
}

void foo(Shape* s)
{
    loc  x,y,z;
    double a;

    //if (mch::C<Circle>(x |= x == val(loc(1,1)), a)(s))
    //    std::cout << "Matched against guard" << a << std::endl;

    if (mch::C<Circle>(x,4.0)(s))
        std::cout << "Circle with center " << x << " and FIXED radius " << std::endl;

    if (mch::C<Circle>(x,a)(s))
        std::cout << "Circle with center " << x << " and radius " << a << std::endl;
    else
    if (mch::C<Square>(x,a)(s))
        std::cout << "Square with top left " << x << " and side " << a << std::endl;
    else
    if (mch::C<Triangle>(x,y,z)(s))
        std::cout << "Triangle with corners " << x << ',' << y << ',' << z << std::endl;
}

void bar(ADTShape& s)
{
    mch::var<cloc>  x,y,z;
    mch::var<double> a;

#ifndef POD_ONLY
    if (mch::C<ADTShapeEx,ADTShape::circle>(x,a)(s))
        std::cout << "ADTCircleEx with center " << x << " and radius " << a << std::endl;
    else
    if (mch::C<ADTShapeEx,ADTShape::square>(x,a)(s))
        std::cout << "ADTSquareEx with top left " << x << " and side " << a << std::endl;
    else
    if (mch::C<ADTShapeEx,ADTShape::triangle>(x,y,z)(s))
        std::cout << "ADTTriangleEx with corners " << x << ',' << y << ',' << z << std::endl;
#endif
    if (mch::C<ADTShape,ADTShape::circle>(x,a)(s))
        std::cout << "ADTCircle with center " << x << " and radius " << a << std::endl;
    else
    if (mch::C<ADTShape,ADTShape::square>(x,a)(s))
        std::cout << "ADTSquare with top left " << x << " and side " << a << std::endl;
    else
    if (mch::C<ADTShape,ADTShape::triangle>(x,y,z)(s))
        std::cout << "ADTTriangle with corners " << x << ',' << y << ',' << z << std::endl;
}

double area_vis(Shape& shape)
{
    struct Visitor : ShapeVisitor
    {
	    virtual void visit(const Circle& s)   { result = 3.14 * s.radius * s.radius; }
	    virtual void visit(const Square& s)   { result = s.side * s.side; }
	    virtual void visit(const Triangle& s) { result = heron(s.first,s.second,s.third); }
        double result;
    };

    Visitor v;
    shape.accept(v);
    return v.result;
}

loc center_vis(/*const*/ Shape& shape)
{
    struct Visitor : ShapeVisitor
    {
	    virtual void visit(const Circle& s)   { result = s.center; }
	    virtual void visit(const Square& s)   { result = loc(s.upper_left.first+s.side/2,s.upper_left.second+s.side/2); }
	    virtual void visit(const Triangle& s) { result = loc((s.first.first+s.second.first+s.third.first)/3,(s.first.second+s.second.second+s.third.second)/3); }
        loc result;
    };

    Visitor v;
    shape.accept(v);
    return v.result;
}

int dummy_vis(Shape* s)
{
    struct Visitor : ShapeVisitor
    {
	    virtual void visit(const Circle&)   { result = 1; }
	    virtual void visit(const Square&)   { result = 2; }
	    virtual void visit(const Triangle&) { result = 3; }
        int result;
    };

    Visitor v;
    s->accept(v);
    return v.result;
}

void time_area(Shape& s)
{
    const int N = 10000;
    double a1,a2;
    mch::time_stamp liStart1 = mch::get_time_stamp();
    for (int i = 0; i < N; ++i)
        a1 = area_vis(s);
    mch::time_stamp liFinish1 = mch::get_time_stamp();

    mch::time_stamp liStart2 = mch::get_time_stamp();
    for (int i = 0; i < N; ++i)
        a2 = area(s);
    mch::time_stamp liFinish2 = mch::get_time_stamp();

    XTL_ASSERT(a1==a2); XTL_UNUSED(a1); XTL_UNUSED(a2); 

    std::cout << "AreaV Time:" << mch::microseconds(liFinish1-liStart1) << std::endl;
    std::cout << "AreaM Time:" << mch::microseconds(liFinish2-liStart2) << std::endl;
    std::cout << (liFinish2-liStart2)*100/(liFinish1-liStart1)-100 << "% slower" << std::endl;
}

void time_center(Shape& s)
{
    const int N = 10000;
    loc c1,c2;
    mch::time_stamp liStart1 = mch::get_time_stamp();
    for (int i = 0; i < N; ++i)
        c1 = center_vis(s);
    mch::time_stamp liFinish1 = mch::get_time_stamp();

    mch::time_stamp liStart2 = mch::get_time_stamp();
    for (int i = 0; i < N; ++i)
        c2 = center(s);
    mch::time_stamp liFinish2 = mch::get_time_stamp();

    XTL_ASSERT(c1==c2); XTL_UNUSED(c1); XTL_UNUSED(c2); 

    std::cout << "CenterV Time:" << mch::microseconds(liFinish1-liStart1) << std::endl;
    std::cout << "CenterM Time:" << mch::microseconds(liFinish2-liStart2) << std::endl;
    std::cout << (liFinish2-liStart2)*100/(liFinish1-liStart1)-100 << "% slower" << std::endl;
}

void time_dummy(Shape& s)
{
    const int N = 10000;
    int c1,c2;
    mch::time_stamp liStart1 = mch::get_time_stamp();
    for (int i = 0; i < N; ++i)
        c1 = dummy_vis(&s);
    mch::time_stamp liFinish1 = mch::get_time_stamp();

    mch::time_stamp liStart2 = mch::get_time_stamp();
    for (int i = 0; i < N; ++i)
        c2 = dummy(&s);
    mch::time_stamp liFinish2 = mch::get_time_stamp();

    mch::time_stamp liStart3 = mch::get_time_stamp();
    for (int i = 0; i < N; ++i)
        c2 = dummy_dyn(&s);
    mch::time_stamp liFinish3 = mch::get_time_stamp();

    XTL_ASSERT(c1==c2); XTL_UNUSED(c1); XTL_UNUSED(c2);

    std::cout << "DummyV Time:" << mch::microseconds(liFinish1-liStart1) << std::endl;
    std::cout << "DummyD Time:" << mch::microseconds(liFinish3-liStart3) << std::endl;
    std::cout << "DummyM Time:" << mch::microseconds(liFinish2-liStart2) << std::endl;
    
    std::cout << (liFinish3-liStart3)*100/(liFinish1-liStart1)-100 << "% slower" << std::endl;
    std::cout << (liFinish2-liStart2)*100/(liFinish1-liStart1)-100 << "% slower" << std::endl;
}


int main()
{
    Shape* c = new Circle(loc(1,1),7);
    Shape* s = new Square(loc(2,2),2);
    Shape* t = new Triangle(loc(0,0),loc(0,1),loc(1,0));

    foo(c); std::cout << "Area: " << area(*c) << " Center: " << center(*c) << std::endl;
    foo(s); std::cout << "Area: " << area(*s) << " Center: " << center(*s) << std::endl;
    foo(t); std::cout << "Area: " << area(*t) << " Center: " << center(*t) << std::endl;

#ifdef POD_ONLY
    ADTShape ac = {ADTShape::circle,   {{{1,1}, 4}}};
    ADTShape as = {ADTShape::square,   {{{2,2}, 2}}};
    ADTShape at = {ADTShape::triangle, {{{1,1}, 1}}};
#else
    cloc l00 = {0,0};
    cloc l11 = {1,1};
    cloc l10 = {1,0};
    ADTShape ac(l11, 4);
    ADTShape as(2, l00);
    ADTShape at(l11, l10, l00);
#endif
    bar(ac);
    bar(as);
    bar(at);

    ADTShapeEx xc = ac;
    ADTShapeEx xs = as;
    ADTShapeEx xt = at;

    bar(xc);
    bar(xs);
    bar(xt);

    std::cout << std::endl;
    std::cout << "Timing Circle Only" << std::endl;
    time_area(*c);
    std::cout << "Timing Square Only" << std::endl;
    time_area(*s);
    std::cout << "Timing Triangle Only" << std::endl;
    time_area(*t);

    std::cout << std::endl;
    std::cout << "Timing Circle Only" << std::endl;
    time_center(*c);
    std::cout << "Timing Square Only" << std::endl;
    time_center(*s);
    std::cout << "Timing Triangle Only" << std::endl;
    time_center(*t);

    std::cout << std::endl;
    std::cout << "Timing Circle Only" << std::endl;
    time_dummy(*c);
    std::cout << "Timing Square Only" << std::endl;
    time_dummy(*s);
    std::cout << "Timing Triangle Only" << std::endl;
    time_dummy(*t);
}
