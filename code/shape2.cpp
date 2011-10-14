//#define POD_ONLY
#include <windows.h>
#include <iostream>
#include "match_shape.hpp"

std::ostream& operator<<(std::ostream& os, const loc& l)
{
    return os << '(' << l.first << ',' << l.second << ')';
}

std::ostream& operator<<(std::ostream& os, const cloc& l)
{
    return os << '(' << l.first << ',' << l.second << ')';
}

real heron(const loc& a, const loc& b, const loc& c) { return 1.0/2; }

real area(const Shape& shape)
{
    wildcard _; // Meta variable
    loc  x,y,z;
    real r,s;

    if (match<Circle>(_,r)(shape))
        return 3.14 * r * r;

    if (match<Square>(_,s)(shape))
        return s * s;

    if (match<Triangle>(x,y,z)(shape))
        return heron(x,y,z);

    assert(!"Inexhaustive search");
}

//variable<loc> a,b,c;
//
//if (match<Triangle>(a,b,c)(shape))
//    return loc(
//        (a.m_value.first +b.m_value.first +c.m_value.first) /3,
//        (a.m_value.second+b.m_value.second+c.m_value.second)/3);
//
//if (const Triangle* p = match<Triangle>()(shape))
//    return loc(
//        (p->first.first +p->second.first +p->third.first) /3,
//        (p->first.second+p->second.second+p->third.second)/3);


loc center(/*const*/ Shape& shape)
{
    loc c;

    if (match<Circle>(c/*,_*/)(shape))
        return c;

    real x,y,s;

    if (match<Square>(
            match<loc>(x,y),s)(shape))
        return loc(x+s/2,y+s/2);

    real x1,y1,x2,y2,x3,y3;

    if (match<Triangle>(
            match<loc>(x1,y1),
            match<loc>(x2,y2),
            match<loc>(x3,y3)
                       )(shape))
        return loc((x1+x2+x3)/3,(y1+y2+y3)/3);

    assert(!"Inexhaustive search");
}

int dummy(Shape* s)
{
    if (match<Circle>()(s))   return 1;
    if (match<Square>()(s))   return 2;
    if (match<Triangle>()(s)) return 3;
    assert(!"Inexhaustive search");
}

int dummy_dyn(Shape* s)
{
    if (dynamic_cast<Circle*>(s))   return 1;
    if (dynamic_cast<Square*>(s))   return 2;
    if (dynamic_cast<Triangle*>(s)) return 3;
    assert(!"Inexhaustive search");
}

void foo(Shape* s)
{
    loc  x,y,z;
    real a;

    //if (match<Circle>(x |= x == val(loc(1,1)), a)(s))
    //    std::cout << "Matched against guard" << a << std::endl;

    if (match<Circle>(x,4.0)(s))
        std::cout << "Circle with center " << x << " and FIXED radius " << std::endl;

    if (match<Circle>(x,a)(s))
        std::cout << "Circle with center " << x << " and radius " << a << std::endl;
    else
    if (match<Square>(x,a)(s))
        std::cout << "Square with top left " << x << " and side " << a << std::endl;
    else
    if (match<Triangle>(x,y,z)(s))
        std::cout << "Triangle with corners " << x << ',' << y << ',' << z << std::endl;
}

void bar(ADTShape& s)
{
    variable<cloc>  x,y,z;
    variable<real> a;

#ifndef POD_ONLY
    if (matchex<ADTShapeEx,ADTShape::circle>(x,a)(s))
        std::cout << "ADTCircleEx with center " << x << " and radius " << a << std::endl;
    else
    if (matchex<ADTShapeEx,ADTShape::square>(x,a)(s))
        std::cout << "ADTSquareEx with top left " << x << " and side " << a << std::endl;
    else
    if (matchex<ADTShapeEx,ADTShape::triangle>(x,y,z)(s))
        std::cout << "ADTTriangleEx with corners " << x << ',' << y << ',' << z << std::endl;
#endif
    if (matchex<ADTShape,ADTShape::circle>(x,a)(s))
        std::cout << "ADTCircle with center " << x << " and radius " << a << std::endl;
    else
    if (matchex<ADTShape,ADTShape::square>(x,a)(s))
        std::cout << "ADTSquare with top left " << x << " and side " << a << std::endl;
    else
    if (matchex<ADTShape,ADTShape::triangle>(x,y,z)(s))
        std::cout << "ADTTriangle with corners " << x << ',' << y << ',' << z << std::endl;
}

loc center_vis(/*const*/ Shape& shape)
{
    struct CenterVisitor : ShapeVisitor
    {
	    virtual void visit(const Circle& s)   { c = s.center; }
	    virtual void visit(const Square& s)   { c = loc(s.upper_left.first+s.side/2,s.upper_left.second+s.side/2); }
	    virtual void visit(const Triangle& s) { c = loc((s.first.first+s.second.first+s.third.first)/3,(s.first.second+s.second.second+s.third.second)/3); }
        loc c;
    };

    CenterVisitor v;
    shape.accept(v);
    return v.c;
}

int dummy_vis(Shape* s)
{
    struct Visitor : ShapeVisitor
    {
	    virtual void visit(const Circle& s)   { result = 1; }
	    virtual void visit(const Square& s)   { result = 2; }
	    virtual void visit(const Triangle& s) { result = 3; }
        int result;
    };

    Visitor v;
    s->accept(v);
    return v.result;
}

void time_center(Shape& s)
{
    LARGE_INTEGER Freq;

    QueryPerformanceFrequency(&Freq);

    const int N = 10000;
    LARGE_INTEGER liStart1, liFinish1, liStart2, liFinish2;
    loc c1,c2;
    QueryPerformanceCounter(&liStart1);
    for (int i = 0; i < N; ++i)
        c1 = center_vis(s);
    QueryPerformanceCounter(&liFinish1);

    QueryPerformanceCounter(&liStart2);
    for (int i = 0; i < N; ++i)
        c2 = center(s);
    QueryPerformanceCounter(&liFinish2);

    assert(c1==c2);

    std::cout << "CenterV Time:" << (liFinish1.QuadPart-liStart1.QuadPart)*1000000/Freq.QuadPart << std::endl;
    std::cout << "CenterM Time:" << (liFinish2.QuadPart-liStart2.QuadPart)*1000000/Freq.QuadPart << std::endl;
    std::cout << (liFinish2.QuadPart-liStart2.QuadPart)*100/(liFinish1.QuadPart-liStart1.QuadPart)-100 << "% slower" << std::endl;
}

void time_dummy(Shape& s)
{
    LARGE_INTEGER Freq;

    QueryPerformanceFrequency(&Freq);

    const int N = 10000;
    LARGE_INTEGER liStart1, liFinish1, liStart2, liFinish2, liStart3, liFinish3;
    int c1,c2;
    QueryPerformanceCounter(&liStart1);
    for (int i = 0; i < N; ++i)
        c1 = dummy_vis(&s);
    QueryPerformanceCounter(&liFinish1);

    QueryPerformanceCounter(&liStart2);
    for (int i = 0; i < N; ++i)
        c2 = dummy(&s);
    QueryPerformanceCounter(&liFinish2);

    QueryPerformanceCounter(&liStart3);
    for (int i = 0; i < N; ++i)
        c2 = dummy_dyn(&s);
    QueryPerformanceCounter(&liFinish3);

    assert(c1==c2);

    std::cout << "DummyV Time:" << (liFinish1.QuadPart-liStart1.QuadPart)*1000000/Freq.QuadPart << std::endl;
    std::cout << "DummyM Time:" << (liFinish2.QuadPart-liStart2.QuadPart)*1000000/Freq.QuadPart << std::endl;
    std::cout << "DummyD Time:" << (liFinish3.QuadPart-liStart3.QuadPart)*1000000/Freq.QuadPart << std::endl;
    std::cout << (liFinish2.QuadPart-liStart2.QuadPart)*100/(liFinish1.QuadPart-liStart1.QuadPart)-100 << "% slower" << std::endl;
    std::cout << (liFinish3.QuadPart-liStart3.QuadPart)*100/(liFinish1.QuadPart-liStart1.QuadPart)-100 << "% slower" << std::endl;
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

    time_center(*c);
    time_center(*s);
    time_center(*t);
    time_dummy(*c);
    time_dummy(*s);
    time_dummy(*t);
}
