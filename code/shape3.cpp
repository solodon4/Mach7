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

//#define POD_ONLY

#include <cmath>
#include <iostream>
#include "match_shape.hpp"

wildcard _; // Meta variable

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

double area_select(Shape* shape)
{
    MatchP(shape)
    {
    CaseP(Circle,_,r)     
        std::cout << "Circle" << std::endl; 
        return 3.14 * r * r;
    CaseP(Square,_,s)     
        std::cout << "Square" << std::endl; 
        return s * s;
    CaseP(Triangle,x,y,z) 
        std::cout << "Triangle" << std::endl; 
        return heron(x,y,z);
    }
    EndMatchP

    XTL_ASSERT(!"Inexhaustive search"); 
    return 0.0;
}

int main()
{
    Shape* c = new Circle(loc(1,1),7);
    Shape* s = new Square(loc(2,2),2);
    Shape* t = new Triangle(loc(0,0),loc(0,1),loc(1,0));

    std::cout << "Area: " << area_select(t) << std::endl;
    std::cout << "Area: " << area_select(t) << std::endl;
    std::cout << "Area: " << area_select(s) << std::endl;
    std::cout << "Area: " << area_select(s) << std::endl;
    std::cout << "Area: " << area_select(c) << std::endl;
    std::cout << "Area: " << area_select(c) << std::endl;
}
