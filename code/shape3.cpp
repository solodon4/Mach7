//#define POD_ONLY

#include <cmath>
#include <iostream>
#include <windows.h>
//#define SELECT
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
    loc  x,y,z;
    double r,s;

    SWITCH(shape)
    {
    //default:
    //matching(Circle,_,r)(shape)     return 3.14 * r * r;
    //matching(Square,_,s)(shape)     return s * s;
    //matching(Triangle,x,y,z)(shape) return heron(x,y,z);
    CASES_BEGIN
    CASE(Circle,_,r)     return 3.14 * r * r;
    CASE(Square,_,s)     return s * s;
    CASE(Triangle,x,y,z) return heron(x,y,z);
    CASES_END

    //    __vtbl2lines_map.update(111, __selector_var, dynamic_cast<Circle*>(__selector_var));   
    //case 2*111: 
    //    if (!match<Circle>(_,r)(__selector_var))     
    //        { case 2*111+1:; } 
    //    else 
    //        return 3.14 * r * r;
    //    __vtbl2lines_map.update(112, __selector_var, dynamic_cast<Square*>(__selector_var));   
    //case 2*112: 
    //    if (!match<Square>(_,s)(__selector_var))     
    //        { case 2*112+1:; } 
    //    else 
    //        return s * s;       
    //    __vtbl2lines_map.update(113, __selector_var, dynamic_cast<Triangle*>(__selector_var)); 
    //case 2*113: 
    //    if (!match<Triangle>(x,y,z)(__selector_var)) 
    //        { case 2*113+1:; } 
    //    else 
    //        return heron(x,y,z);

    //case __LINE__: if (match<Circle>(__LINE__,_,r)(shape))     return 3.14 * r * r;
    //case 2: if (match<Square>(2,_,s)(shape))     return s * s;
    //case 3: if (match<Triangle>(3,x,y,z)(shape)) return heron(x,y,z);
    
    }
    assert(!"Inexhaustive search"); 
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
