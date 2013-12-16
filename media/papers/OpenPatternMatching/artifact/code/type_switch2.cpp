#include <iostream>
#include <utility>
#include "type_switch2.hpp"

//------------------------------------------------------------------------------

typedef std::pair<double,double> loc;

//------------------------------------------------------------------------------

// An Algebraic Data Type implemented through inheritance
struct Shape
{
    virtual ~Shape() {}
};

//------------------------------------------------------------------------------

struct Circle : Shape
{
    Circle(const loc& c, const double& r) : center(c), radius(r) {}

    const loc& get_center() const { return center; }

    loc    center;
    double radius;
};

//------------------------------------------------------------------------------

struct Square : Shape
{
    Square(const loc& c, const double& s) : upper_left(c), side(s) {}

    loc    upper_left;
    double side;
};

//------------------------------------------------------------------------------

struct Triangle : Shape
{
    Triangle(const loc& a, const loc& b, const loc& c) : first(a), second(b), third(c) {}

    loc first;
    loc second;
    loc third;
};

//------------------------------------------------------------------------------

void do_match(const Shape* s0, const Shape* s1)
{
    const char* text = "unknown";

    Match2(s0,s1)
    {
    //Case2(Circle  , Circle  ) text = "C,C"; break;
    Case2(Circle  , Square  ) text = "C,S"; break;
    Case2(Square  , Circle  ) text = "S,C"; break;
    //Case2(Square  , Square  ) text = "S,S"; break;
    Case2(Square  , Triangle) text = "S,T"; break;
    Case2(Triangle, Square  ) text = "T,S"; break;
    Case2(Triangle, Triangle) text = "T,T"; break;
    Case2(Triangle, Circle  ) text = "T,C"; break;
    Case2(Circle  , Triangle) text = "C,T"; break;
    Otherwise2()              text = "other"; break;
    }
    EndMatch2

    std::cout << text << std::endl;
}

//------------------------------------------------------------------------------

int main()
{
    Shape* c = new Circle(loc(1,1),7);
    Shape* s = new Square(loc(1,1),2);
    Shape* t = new Triangle(loc(1,1),loc(1,0),loc(0,0));

    Shape* shapes[] = {c,s,t};

    for (size_t i = 0; i < 3; ++i)
        for (size_t j = 0; j < 2; ++j)
            do_match(shapes[i], shapes[j]);

    for (size_t i = 0; i < 2; ++i)
        for (size_t j = 0; j < 3; ++j)
            do_match(shapes[i], shapes[j]);
}

//------------------------------------------------------------------------------
