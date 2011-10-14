//#define POD_ONLY
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
    variable<void> _; // Meta variable
    variable<loc>  x,y,z;
    variable<real> r,s;

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
    variable<loc> c;

    if (match<Circle>(c/*,_*/)(shape))
        return c;

    variable<real> x,y,s;

    if (match<Square>(
            match<loc>(x,y),s)(shape))
        return loc(x+s/2,y+s/2);

    variable<real> x1,y1,x2,y2,x3,y3;

    if (match<Triangle>(
            match<loc>(x1,y1),
            match<loc>(x2,y2),
            match<loc>(x3,y3)
                       )(shape))
        return loc((x1+x2+x3)/3,(y1+y2+y3)/3);

    assert(!"Inexhaustive search");
}

void foo(Shape* s) // FIX: doesn't work without const
{
    variable<loc>  x,y,z;
    variable<real> a;
    variable<void> _;

    //if (match<Circle>(x | x == val(loc(1,1)), a)(s))
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

int main()
{
    Shape* c = new Circle(loc(1,1),4);
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
}
