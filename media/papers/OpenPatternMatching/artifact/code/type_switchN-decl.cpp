#include <iostream>
#include <utility>
#include "type_switchN-decl.hpp"

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

void do_match(const Shape* s0)
{
    const char* text = "unknown";

    Match(s0)
    {
    Case(const Circle&   a) text = "C"; XTL_UNUSED(a); break;
    Case(const Square&   a) text = "S"; XTL_UNUSED(a); break;
    Case(const Triangle& a) text = "T"; XTL_UNUSED(a); break;
    Otherwise()             text = "other";            break;
    }
    EndMatch

    std::cout << text << std::endl;
}

//------------------------------------------------------------------------------

void do_match(const Shape* s0, const Shape* s1)
{
    const char* text = "unknown";

    Match(s0,s1)
    {
  //Case(const Circle&   a, const Circle&   b) text = "C,C"; XTL_UNUSED(a); XTL_UNUSED(b); break;
    Case(const Circle&   a, const Square&   b) text = "C,S"; XTL_UNUSED(a); XTL_UNUSED(b); break;
    Case(const Square&   a, const Circle&   b) text = "S,C"; XTL_UNUSED(a); XTL_UNUSED(b); break;
  //Case(const Square&   a, const Square&   b) text = "S,S"; XTL_UNUSED(a); XTL_UNUSED(b); break;
    Case(const Square&   a, const Triangle& b) text = "S,T"; XTL_UNUSED(a); XTL_UNUSED(b); break;
    Case(const Triangle& a, const Square&   b) text = "T,S"; XTL_UNUSED(a); XTL_UNUSED(b); break;
    Case(const Triangle& a, const Triangle& b) text = "T,T"; XTL_UNUSED(a); XTL_UNUSED(b); break;
    Case(const Triangle& a, const Circle&   b) text = "T,C"; XTL_UNUSED(a); XTL_UNUSED(b); break;
    Case(const Circle&   a, const Triangle& b) text = "C,T"; XTL_UNUSED(a); XTL_UNUSED(b); break;
    Otherwise()                                text = "other";                             break;
    }
    EndMatch

    std::cout << text << std::endl;
}

//------------------------------------------------------------------------------

void do_match(Shape* s0, const Shape* s1, const Shape* s2)
{
    const char* text = "unknown";

    Match(s0,s1,s2)
    {
    Case(Circle&   a, const Circle&   b, const Circle&   c) text = "C,C,C"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); break;
    Case(Circle&   a, const Circle&   b, const Square&   c) text = "C,C,S"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); break;
    Case(Circle&   a, const Square&   b, const Circle&   c) text = "C,S,C"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); break;
    Case(Circle&   a, const Square&   b, const Square&   c) text = "C,S,S"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); break;
    Case(Circle&   a, const Square&   b, const Triangle& c) text = "C,S,T"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); break;
    Case(Circle&   a, const Triangle& b, const Square&   c) text = "C,T,S"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); break;
    Case(Circle&   a, const Triangle& b, const Triangle& c) text = "C,T,T"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); break;
    Case(Circle&   a, const Triangle& b, const Circle&   c) text = "C,T,C"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); break;
    Case(Circle&   a, const Circle&   b, const Triangle& c) text = "C,C,T"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); break;
                   
  //Case(Square&   a, const Circle&   b, const Circle&   c) text = "S,C,C"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); break;
  //Case(Square&   a, const Circle&   b, const Square&   c) text = "S,C,S"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); break;
    Case(Square&   a, const Square&   b, const Circle&   c) text = "S,S,C"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); break;
    Case(Square&   a, const Square&   b, const Square&   c) text = "S,S,S"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); break;
    Case(Square&   a, const Square&   b, const Triangle& c) text = "S,S,T"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); break;
    Case(Square&   a, const Triangle& b, const Square&   c) text = "S,T,S"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); break;
    Case(Square&   a, const Triangle& b, const Triangle& c) text = "S,T,T"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); break;
    Case(Square&   a, const Triangle& b, const Circle&   c) text = "S,T,C"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); break;
  //Case(Square&   a, const Circle&   b, const Triangle& c) text = "S,C,T"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); break;

    Case(Triangle& a, const Circle&   b, const Circle&   c) text = "T,C,C"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); break;
    Case(Triangle& a, const Circle&   b, const Square&   c) text = "T,C,S"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); break;
    Case(Triangle& a, const Square&   b, const Circle&   c) text = "T,S,C"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); break;
    Case(Triangle& a, const Square&   b, const Square&   c) text = "T,S,S"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); break;
    Case(Triangle& a, const Square&   b, const Triangle& c) text = "T,S,T"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); break;
    Case(Triangle& a, const Triangle& b, const Square&   c) text = "T,T,S"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); break;
    Case(Triangle& a, const Triangle& b, const Triangle& c) text = "T,T,T"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); break;
    Case(Triangle& a, const Triangle& b, const Circle&   c) text = "T,T,C"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); break;
    Case(Triangle& a, const Circle&   b, const Triangle& c) text = "T,C,T"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); break;

    Otherwise()                                             text = "other";                                              break;
    }
    EndMatch

    std::cout << text << std::endl;
}

//------------------------------------------------------------------------------

void do_match(Shape* s0, Shape* s1, Shape* s2, Shape* s3)
{
    const char* text = "unknown";

    Match(s0,s1,s2,s3)
    {
    Case(Circle&   a, const Circle&   b, const Circle&   c, const Circle&   t) text = "C,C,C,C"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Circle&   a, const Circle&   b, const Circle&   c, const Square&   t) text = "C,C,C,S"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Circle&   a, const Circle&   b, const Square&   c, const Circle&   t) text = "C,C,S,C"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Circle&   a, const Circle&   b, const Square&   c, const Square&   t) text = "C,C,S,S"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Circle&   a, const Circle&   b, const Square&   c, const Triangle& t) text = "C,C,S,T"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Circle&   a, const Circle&   b, const Triangle& c, const Square&   t) text = "C,C,T,S"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Circle&   a, const Circle&   b, const Triangle& c, const Triangle& t) text = "C,C,T,T"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Circle&   a, const Circle&   b, const Triangle& c, const Circle&   t) text = "C,C,T,C"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Circle&   a, const Circle&   b, const Circle&   c, const Triangle& t) text = "C,C,C,T"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
                                                                   
    Case(Circle&   a, const Square&   b, const Circle&   c, const Circle&   t) text = "C,S,C,C"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Circle&   a, const Square&   b, const Circle&   c, const Square&   t) text = "C,S,C,S"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Circle&   a, const Square&   b, const Square&   c, const Circle&   t) text = "C,S,S,C"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Circle&   a, const Square&   b, const Square&   c, const Square&   t) text = "C,S,S,S"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Circle&   a, const Square&   b, const Square&   c, const Triangle& t) text = "C,S,S,T"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Circle&   a, const Square&   b, const Triangle& c, const Square&   t) text = "C,S,T,S"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Circle&   a, const Square&   b, const Triangle& c, const Triangle& t) text = "C,S,T,T"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Circle&   a, const Square&   b, const Triangle& c, const Circle&   t) text = "C,S,T,C"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Circle&   a, const Square&   b, const Circle&   c, const Triangle& t) text = "C,S,C,T"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
                                                                   
    Case(Circle&   a, const Triangle& b, const Circle&   c, const Circle&   t) text = "C,T,C,C"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Circle&   a, const Triangle& b, const Circle&   c, const Square&   t) text = "C,T,C,S"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Circle&   a, const Triangle& b, const Square&   c, const Circle&   t) text = "C,T,S,C"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Circle&   a, const Triangle& b, const Square&   c, const Square&   t) text = "C,T,S,S"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Circle&   a, const Triangle& b, const Square&   c, const Triangle& t) text = "C,T,S,T"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Circle&   a, const Triangle& b, const Triangle& c, const Square&   t) text = "C,T,T,S"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Circle&   a, const Triangle& b, const Triangle& c, const Triangle& t) text = "C,T,T,T"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Circle&   a, const Triangle& b, const Triangle& c, const Circle&   t) text = "C,T,T,C"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Circle&   a, const Triangle& b, const Circle&   c, const Triangle& t) text = "C,T,C,T"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
                                                             
                                                             
    Case(Square&   a, const Circle&   b, const Circle&   c, const Circle&   t) text = "S,C,C,C"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Square&   a, const Circle&   b, const Circle&   c, const Square&   t) text = "S,C,C,S"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Square&   a, const Circle&   b, const Square&   c, const Circle&   t) text = "S,C,S,C"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Square&   a, const Circle&   b, const Square&   c, const Square&   t) text = "S,C,S,S"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Square&   a, const Circle&   b, const Square&   c, const Triangle& t) text = "S,C,S,T"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Square&   a, const Circle&   b, const Triangle& c, const Square&   t) text = "S,C,T,S"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Square&   a, const Circle&   b, const Triangle& c, const Triangle& t) text = "S,C,T,T"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Square&   a, const Circle&   b, const Triangle& c, const Circle&   t) text = "S,C,T,C"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Square&   a, const Circle&   b, const Circle&   c, const Triangle& t) text = "S,C,C,T"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
                                                                   
    Case(Square&   a, const Square&   b, const Circle&   c, const Circle&   t) text = "S,S,C,C"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Square&   a, const Square&   b, const Circle&   c, const Square&   t) text = "S,S,C,S"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Square&   a, const Square&   b, const Square&   c, const Circle&   t) text = "S,S,S,C"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Square&   a, const Square&   b, const Square&   c, const Square&   t) text = "S,S,S,S"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Square&   a, const Square&   b, const Square&   c, const Triangle& t) text = "S,S,S,T"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Square&   a, const Square&   b, const Triangle& c, const Square&   t) text = "S,S,T,S"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Square&   a, const Square&   b, const Triangle& c, const Triangle& t) text = "S,S,T,T"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Square&   a, const Square&   b, const Triangle& c, const Circle&   t) text = "S,S,T,C"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Square&   a, const Square&   b, const Circle&   c, const Triangle& t) text = "S,S,C,T"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
                                                                   
    Case(Square&   a, const Triangle& b, const Circle&   c, const Circle&   t) text = "S,T,C,C"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Square&   a, const Triangle& b, const Circle&   c, const Square&   t) text = "S,T,C,S"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Square&   a, const Triangle& b, const Square&   c, const Circle&   t) text = "S,T,S,C"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Square&   a, const Triangle& b, const Square&   c, const Square&   t) text = "S,T,S,S"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Square&   a, const Triangle& b, const Square&   c, const Triangle& t) text = "S,T,S,T"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Square&   a, const Triangle& b, const Triangle& c, const Square&   t) text = "S,T,T,S"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Square&   a, const Triangle& b, const Triangle& c, const Triangle& t) text = "S,T,T,T"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Square&   a, const Triangle& b, const Triangle& c, const Circle&   t) text = "S,T,T,C"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Square&   a, const Triangle& b, const Circle&   c, const Triangle& t) text = "S,T,C,T"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
                                                             
                                                             
    Case(Triangle& a, const Circle&   b, const Circle&   c, const Circle&   t) text = "T,C,C,C"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Triangle& a, const Circle&   b, const Circle&   c, const Square&   t) text = "T,C,C,S"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Triangle& a, const Circle&   b, const Square&   c, const Circle&   t) text = "T,C,S,C"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Triangle& a, const Circle&   b, const Square&   c, const Square&   t) text = "T,C,S,S"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Triangle& a, const Circle&   b, const Square&   c, const Triangle& t) text = "T,C,S,T"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Triangle& a, const Circle&   b, const Triangle& c, const Square&   t) text = "T,C,T,S"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Triangle& a, const Circle&   b, const Triangle& c, const Triangle& t) text = "T,C,T,T"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Triangle& a, const Circle&   b, const Triangle& c, const Circle&   t) text = "T,C,T,C"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Triangle& a, const Circle&   b, const Circle&   c, const Triangle& t) text = "T,C,C,T"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
                                                                   
    Case(Triangle& a, const Square&   b, const Circle&   c, const Circle&   t) text = "T,S,C,C"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Triangle& a, const Square&   b, const Circle&   c, const Square&   t) text = "T,S,C,S"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Triangle& a, const Square&   b, const Square&   c, const Circle&   t) text = "T,S,S,C"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Triangle& a, const Square&   b, const Square&   c, const Square&   t) text = "T,S,S,S"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Triangle& a, const Square&   b, const Square&   c, const Triangle& t) text = "T,S,S,T"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Triangle& a, const Square&   b, const Triangle& c, const Square&   t) text = "T,S,T,S"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Triangle& a, const Square&   b, const Triangle& c, const Triangle& t) text = "T,S,T,T"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Triangle& a, const Square&   b, const Triangle& c, const Circle&   t) text = "T,S,T,C"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Triangle& a, const Square&   b, const Circle&   c, const Triangle& t) text = "T,S,C,T"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
                                                                   
    Case(Triangle& a, const Triangle& b, const Circle&   c, const Circle&   t) text = "T,T,C,C"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Triangle& a, const Triangle& b, const Circle&   c, const Square&   t) text = "T,T,C,S"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Triangle& a, const Triangle& b, const Square&   c, const Circle&   t) text = "T,T,S,C"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Triangle& a, const Triangle& b, const Square&   c, const Square&   t) text = "T,T,S,S"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Triangle& a, const Triangle& b, const Square&   c, const Triangle& t) text = "T,T,S,T"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Triangle& a, const Triangle& b, const Triangle& c, const Square&   t) text = "T,T,T,S"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Triangle& a, const Triangle& b, const Triangle& c, const Triangle& t) text = "T,T,T,T"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Triangle& a, const Triangle& b, const Triangle& c, const Circle&   t) text = "T,T,T,C"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    Case(Triangle& a, const Triangle& b, const Circle&   c, const Triangle& t) text = "T,T,C,T"; XTL_UNUSED(a); XTL_UNUSED(b); XTL_UNUSED(c); XTL_UNUSED(t); break;
    
    Otherwise()                                                                text = "other";                                                               break;
    }
    EndMatch

    std::cout << text << std::endl;
}

//------------------------------------------------------------------------------

int main()
{
    Shape* c = new Circle(loc(1,1),7);
    Shape* s = new Square(loc(1,1),2);
    Shape* t = new Triangle(loc(1,1),loc(1,0),loc(0,0));

    Shape* shapes[] = {c,s,t};

    for (size_t n = 0; n < 3; n++)
    {
        // 1 argument
        for (size_t i = 0; i < 3; ++i)
            do_match(shapes[i]);
        
        // 2 arguments
        for (size_t i = 0; i < 3; ++i)
        for (size_t j = 0; j < 3; ++j)
            do_match(shapes[i], shapes[j]);
        
        // 3 arguments
        for (size_t i = 0; i < 3; ++i)
        for (size_t j = 0; j < 3; ++j)
        for (size_t k = 0; k < 3; ++k)
            do_match(shapes[i], shapes[j], shapes[k]);
        
        // 4 arguments
        for (size_t i = 0; i < 3; ++i)
        for (size_t j = 0; j < 3; ++j)
        for (size_t k = 0; k < 3; ++k)
        for (size_t l = 0; l < 3; ++l)
            do_match(shapes[i], shapes[j], shapes[k], shapes[l]);
    }
}

//------------------------------------------------------------------------------
