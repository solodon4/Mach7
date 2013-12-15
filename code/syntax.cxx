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

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <utility>

//------------------------------------------------------------------------------

#if XTL_DEFAULT_SYNTAX == 'u' && XTL_REDUNDANCY_CHECKING
// We do this as we will be using tags as first argument for unified code of 
// Match in this case, which will render wrong catch handlers. So we workaround
// it with just some different types in catch handlers to let it compile.
#undef  XTL_REDUNDANCY_CATCH
#define XTL_REDUNDANCY_CATCH(L) catch(int (*)[XTL_COUNTER])
#endif

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

struct loc 
{
    friend std::ostream& operator<<(std::ostream& os, const loc& l) { return os << '{' << l.first << ',' << l.second << '}'; }
    double first; 
    double second; 
};

enum shape_kind {UnknownShape, Circle, Square, Triangle} kind;

struct Shape
{
    shape_kind kind;

#ifndef POD_ONLY
	Shape(const loc& c, double r) : kind(Circle), center(c), radius(r) {}
	Shape(double s, const loc& c) : kind(Square), upper_left(c), size(s) {}
	Shape(const loc& x, const loc& y, const loc& z) : kind(Triangle), first(x), second(y), third(z) {}
#endif

#ifndef _MSC_VER
	union
	{
		struct { loc center;     double radius; }; // circle;
		struct { loc upper_left; double size; };   // square;
		struct { loc first, second, third; };      // triangle;
	};
#else
    // MSVC doesn't seem to allow anonymous structs inside union so we just dump
    // here same members name directly to make the rest just compile at least.
	loc center;     double radius; // circle;
	loc upper_left; double size;   // square;
	loc first, second, third;      // triangle;
#endif

};

namespace mch ///< Mach7 library namespace
{
template <> struct bindings<Shape>          { KV(Shape,UnknownShape); KS(Shape::kind); };

template <> struct bindings<Shape,Circle>   { KV(Shape,Circle);  CM(0,Shape::center);     CM(1,Shape::radius); };
template <> struct bindings<Shape,Square>   { KV(Shape,Square);  CM(0,Shape::upper_left); CM(1,Shape::size); };
template <> struct bindings<Shape,Triangle> { KV(Shape,Triangle);CM(0,Shape::first);      CM(1,Shape::second); CM(2,Shape::third); };
} // of namespace mch

#else

typedef std::pair<double,double> loc;

std::ostream& operator<<(std::ostream& os, const loc& l) { return os << '{' << l.first << ',' << l.second << '}'; }

// An Algebraic Data Type implemented through inheritance
struct Shape
{
    XTL_TAG_ONLY(  enum ShapeKind {SK_Shape=1,SK_Circle,SK_Square,SK_Triangle}; )
    XTL_TAG_ONLY(  ShapeKind kind; )
    XTL_TAG_ONLY(  Shape(ShapeKind k) : kind(k) {} )
    XTL_OPEN_ONLY( virtual ~Shape() {} )
    XTL_E_ONLY(    virtual void raise() const /*throw(Shape)*/ = 0; ) // Polymorphic exception idiom: http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Polymorphic_Exception
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

namespace mch ///< Mach7 library namespace
{
template <> struct bindings<Shape>    { XTL_TAG_ONLY(KS(Shape::kind)); XTL_TAG_ONLY(KV(Shape,Shape::SK_Shape)); XTL_E_ONLY(RS(Shape::raise)); };

template <> struct bindings<Circle>   { XTL_TAG_ONLY(KV(Shape,Shape::SK_Circle));   XTL_TAG_ONLY(BCS(Circle,  Shape)); CM(0,Circle::get_center); CM(1,Circle::radius); };
template <> struct bindings<Square>   { XTL_TAG_ONLY(KV(Shape,Shape::SK_Square));   XTL_TAG_ONLY(BCS(Square,  Shape)); CM(0,Square::upper_left); CM(1,Square::side);   };
template <> struct bindings<Triangle> { XTL_TAG_ONLY(KV(Shape,Shape::SK_Triangle)); XTL_TAG_ONLY(BCS(Triangle,Shape)); CM(0,Triangle::first);    CM(1,Triangle::second); CM(2,Triangle::third); };
} // of namespace mch

#endif

//------------------------------------------------------------------------------

#if XTL_DEFAULT_SYNTAX == 'U' || XTL_DEFAULT_SYNTAX == 'u'
Shape* make_shape(size_t i)
{
#ifdef POD_ONLY
    Shape ac = {Circle,   {{{1,1}, 7}}};
    Shape as = {Square,   {{{1,1}, 2}}};
    Shape at = {Triangle, {{{1,1}, 1}}};
#else
    loc l00 = {0,0};
    loc l11 = {1,1};
    loc l10 = {1,0};
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


namespace mch ///< Mach7 library namespace
{
template <> struct bindings<loc>    { CM(0, loc::first); CM(1, loc::second); };
} // of namespace mch

//------------------------------------------------------------------------------

/// Tests Case-clauses without Otherwise-clause
size_t do_match_1_case(const Shape& s, size_t)
{
    Match(s)
      XTL_USE_BRACES_ONLY({)
        Case(Circle,c,r)     std::cout << "Circle("   << c << ',' << r << ')'             << std::endl;
        Case(Square,c,s)     std::cout << "Square("   << c << ',' << s << ')'             << std::endl;
        Case(Triangle,x,y,z) std::cout << "Triangle(" << x << ',' << y << ',' << z << ')' << std::endl;
      XTL_USE_BRACES_ONLY(})
    EndMatch

    return 0;
}

//------------------------------------------------------------------------------

/// Tests Case-clauses with Otherwise-clause
size_t do_match_2_case(const Shape& s, size_t)
{
    Match(s)
      XTL_USE_BRACES_ONLY({)
        Case(Circle,c,r)     std::cout << "Circle("   << c << ',' << r << ')'             << std::endl;
        Case(Square,c,s)     std::cout << "Square("   << c << ',' << s << ')'             << std::endl;
        Case(Triangle,x,y,z) std::cout << "Triangle(" << x << ',' << y << ',' << z << ')' << std::endl;
        Otherwise()          std::cout << "Other()"                                       << std::endl;
      XTL_USE_BRACES_ONLY(})
    EndMatch

    return 0;
}

//------------------------------------------------------------------------------

/// Tests Qua-clauses without Otherwise-clause
/// \note Results should always match those of do_match_1_case
size_t do_match_1_que(const Shape& s, size_t)
{
    mch::var<loc> c,x,y,z;
    mch::var<double> r,w;

    Match(s)
      XTL_USE_BRACES_ONLY({)
        Qua(Circle,c,r)     std::cout << "Circle("   << c << ',' << r << ')'             << std::endl;
        Qua(Square,c,w)     std::cout << "Square("   << c << ',' << w << ')'             << std::endl;
        Qua(Triangle,x,y,z) std::cout << "Triangle(" << x << ',' << y << ',' << z << ')' << std::endl;
      XTL_USE_BRACES_ONLY(})
    EndMatch

    return 0;
}

//------------------------------------------------------------------------------

/// Tests Qua-clauses with Otherwise-clause
/// \note Results should always match those of do_match_2_case
size_t do_match_2_que(const Shape& s, size_t)
{
    mch::var<loc> c,x,y,z;
    mch::var<double> r,w;

    Match(s)
      XTL_USE_BRACES_ONLY({)
        Qua(Circle,c,r)     std::cout << "Circle("   << c << ',' << r << ')'             << std::endl;
        Qua(Square,c,w)     std::cout << "Square("   << c << ',' << w << ')'             << std::endl;
        Qua(Triangle,x,y,z) std::cout << "Triangle(" << x << ',' << y << ',' << z << ')' << std::endl;
        Otherwise()         std::cout << "Other()"                                       << std::endl;
      XTL_USE_BRACES_ONLY(})
    EndMatch

    return 0;
}

//------------------------------------------------------------------------------

/// Tests When-subclauses inside Qua-clauses without Otherwise-clause
size_t do_match_3(const Shape& s, size_t)
{
    mch::var<loc> c,x,y,z;
    mch::var<double> r,w;

    Match(s)
      XTL_USE_BRACES_ONLY({)
        Qua(Circle,c,r |= r > 5) std::cout << "Circle(" << c << ',' << r << ">5" << ')' << std::endl;
            When(  c,r |= r > 3) std::cout << "Circle(" << c << ',' << r << ">3" << ')' << std::endl;
            When(  c,r |= r > 1) std::cout << "Circle(" << c << ',' << r << ">1" << ')' << std::endl;
            When(  c,r |= r > 0) std::cout << "Circle(" << c << ',' << r << ">0" << ')' << std::endl;
            When(  c,r)          std::cout << "Circle(" << c << ',' << r << "$$" << ')' << std::endl;
        Qua(Square,c,w |= w > 5) std::cout << "Square(" << c << ',' << w << ">5" << ')' << std::endl;
            When(  c,w |= w > 3) std::cout << "Square(" << c << ',' << w << ">3" << ')' << std::endl;
            When(  c,w |= w > 1) std::cout << "Square(" << c << ',' << w << ">1" << ')' << std::endl;
            When(  c,w |= w > 0) std::cout << "Square(" << c << ',' << w << ">0" << ')' << std::endl;
            When(  c,w)          std::cout << "Square(" << c << ',' << w << "$$" << ')' << std::endl;
        Qua(Triangle,x,y,mch::C<loc>(r,w |= r != w)) std::cout << "Triangle(" << x << ',' << y << ',' << r << "!=" << w << ')' << std::endl;
            When(    x,y,mch::C<loc>(r,w |= r >  w)) std::cout << "Triangle(" << x << ',' << y << ',' << r << "> " << w << ')' << std::endl;
            When(    x,y,mch::C<loc>(r,w |= r >= w)) std::cout << "Triangle(" << x << ',' << y << ',' << r << ">=" << w << ')' << std::endl;
            When(    x,y,mch::C<loc>(r,w |= r == w)) std::cout << "Triangle(" << x << ',' << y << ',' << r << "==" << w << ')' << std::endl;
            When(    x,y,mch::C<loc>(r,w |= r <= w)) std::cout << "Triangle(" << x << ',' << y << ',' << r << "<=" << w << ')' << std::endl;
            When(    x,y,mch::C<loc>(r,w |= r <  w)) std::cout << "Triangle(" << x << ',' << y << ',' << r << "< " << w << ')' << std::endl;
            When(    x,y,mch::C<loc>(r,w))           std::cout << "Triangle(" << x << ',' << y << ',' << r << "$$" << w << ')' << std::endl;
      XTL_USE_BRACES_ONLY(})
    EndMatch

    return 0;
}

//------------------------------------------------------------------------------

/// Tests When-subclauses inside Qua-clauses with Otherwise-clause
size_t do_match_4(const Shape& s, size_t)
{
    mch::var<loc> c,x,y,z;
    mch::var<double> r,w;

    Match(s)
      XTL_USE_BRACES_ONLY({)
        Qua(Circle,c,r |= r > 5) std::cout << "Circle(" << c << ',' << r << ">5" << ')' << std::endl;
            When(  c,r |= r > 3) std::cout << "Circle(" << c << ',' << r << ">3" << ')' << std::endl;
            When(  c,r |= r > 1) std::cout << "Circle(" << c << ',' << r << ">1" << ')' << std::endl;
            When(  c,r |= r > 0) std::cout << "Circle(" << c << ',' << r << ">0" << ')' << std::endl;
            When(  c,r)          std::cout << "Circle(" << c << ',' << r << "$$" << ')' << std::endl;
        Qua(Square,c,w |= w > 5) std::cout << "Square(" << c << ',' << w << ">5" << ')' << std::endl;
            When(  c,w |= w > 3) std::cout << "Square(" << c << ',' << w << ">3" << ')' << std::endl;
            When(  c,w |= w > 1) std::cout << "Square(" << c << ',' << w << ">1" << ')' << std::endl;
            When(  c,w |= w > 0) std::cout << "Square(" << c << ',' << w << ">0" << ')' << std::endl;
            When(  c,w)          std::cout << "Square(" << c << ',' << w << "$$" << ')' << std::endl;
        Qua(Triangle,x,y,mch::C<loc>(r,w |= r != w)) std::cout << "Triangle(" << x << ',' << y << ',' << r << "!=" << w << ')' << std::endl;
            When(    x,y,mch::C<loc>(r,w |= r >  w)) std::cout << "Triangle(" << x << ',' << y << ',' << r << "> " << w << ')' << std::endl;
            When(    x,y,mch::C<loc>(r,w |= r >= w)) std::cout << "Triangle(" << x << ',' << y << ',' << r << ">=" << w << ')' << std::endl;
            When(    x,y,mch::C<loc>(r,w |= r == w)) std::cout << "Triangle(" << x << ',' << y << ',' << r << "==" << w << ')' << std::endl;
            When(    x,y,mch::C<loc>(r,w |= r <= w)) std::cout << "Triangle(" << x << ',' << y << ',' << r << "<=" << w << ')' << std::endl;
            When(    x,y,mch::C<loc>(r,w |= r <  w)) std::cout << "Triangle(" << x << ',' << y << ',' << r << "< " << w << ')' << std::endl;
            When(    x,y,mch::C<loc>(r,w))           std::cout << "Triangle(" << x << ',' << y << ',' << r << "$$" << w << ')' << std::endl;
        Otherwise()              std::cout << "Other()"                                 << std::endl;
      XTL_USE_BRACES_ONLY(})
    EndMatch

    return 0;
}

//------------------------------------------------------------------------------

/// Tests When-subclauses used witihout Qua-clauses.
/// \note The use of Otherwise-clauses is not permitted in this case
size_t do_match_5(const Shape& s, size_t)
{
    mch::var<loc> c,x,y,z;
    mch::var<double> r,w;

    Match(s)
      XTL_USE_BRACES_ONLY({)
        When()      std::cout << "1" << std::endl;
        When()      std::cout << "2" << std::endl;
        When()      std::cout << "3" << std::endl;
      XTL_USE_BRACES_ONLY(})
    EndMatch

    return 0;
}

//------------------------------------------------------------------------------

template <typename T> inline T sqr(const T& x) { return x*x; }

//------------------------------------------------------------------------------

int fib(int n)
{
    mch::var<int> m;

#if XTL_DEFAULT_SYNTAX != 'P' && XTL_DEFAULT_SYNTAX != 'E' && XTL_DEFAULT_SYNTAX != 'F' && XTL_DEFAULT_SYNTAX != 'U' && XTL_DEFAULT_SYNTAX != 'K'
    Match(n)
    XTL_USE_BRACES_ONLY({)
      When(1)     return 1;
      When(2)     return 1;
      When(m*2)   return sqr(fib(m+1)) - sqr(fib(m-1));
      When(m*2+1) return sqr(fib(m+1)) + sqr(fib(m));
    XTL_USE_BRACES_ONLY(})
    EndMatch
#else
    if (mch::C<int>(1)(n))     return 1;
    if (mch::C<int>(2)(n))     return 1;
    if (mch::C<int>(m*2)(n))   return sqr(fib(m+1)) - sqr(fib(m-1));
    if (mch::C<int>(m*2+1)(n)) return sqr(fib(m+1)) + sqr(fib(m));
#endif

    return -1;
}

//------------------------------------------------------------------------------

typedef size_t test_func(const Shape&, size_t);

//------------------------------------------------------------------------------

void test(test_func f, const Shape& s)
{
    std::streambuf* buf = std::cout.rdbuf(); // Save output buffer of std::cout
    std::stringstream ss1;                   // Create temporary string stream
    std::cout.rdbuf(ss1.rdbuf());            // Redirect output to string stream
    size_t r1 = f(s,0);                      // 1st call
    std::stringstream ss2;                   // Create temporary string stream
    std::cout.rdbuf(ss2.rdbuf());            // Redirect output to string stream
    size_t r2 = f(s,0);                      // 2nd call
    std::cout.rdbuf(buf);                    // Restore old output buffer of the passed stream

    if (r1 != r2 || ss1.str() != ss2.str())
    {
        std::cerr << "ERROR: Different outputs on the same arguments! " << r1 << '~' << r2 << std::endl;
        std::cerr << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
        std::cerr << ss1.str();
        std::cerr << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
        std::cerr << ss2.str();
        std::cerr << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    }
    else
        std::cout << ss1.str();
}

//------------------------------------------------------------------------------

template <test_func f1, test_func f2>
size_t must_be_same(const Shape& s, size_t)
{
    std::streambuf* buf = std::cout.rdbuf(); // Save output buffer of std::cout
    std::stringstream ss1;                   // Create temporary string stream
    std::cout.rdbuf(ss1.rdbuf());            // Redirect output to string stream
    size_t r1 = f1(s,0);                     // 1st call
    std::stringstream ss2;                   // Create temporary string stream
    std::cout.rdbuf(ss2.rdbuf());            // Redirect output to string stream
    size_t r2 = f2(s,0);                     // 2nd call
    std::cout.rdbuf(buf);                    // Restore old output buffer of the passed stream

    if (r1 != r2 || ss1.str() != ss2.str())
    {
        std::cerr << "ERROR: Outputs that must be the same are different! " << r1 << '~' << r2 << std::endl;
        std::cerr << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
        std::cerr << ss1.str();
        std::cerr << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
        std::cerr << ss2.str();
        std::cerr << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    }
    else
        std::cout << ss1.str();

    return r1;
}

//------------------------------------------------------------------------------

size_t do_match_all(const Shape&, size_t);
struct { test_func* func; const char* name; } funcs[] = {
    { &do_match_all,                                 "do_match_all"}, 
    { &must_be_same<do_match_1_case,do_match_1_que>, "Case/Qua No  Otherwise"}, 
    { &must_be_same<do_match_2_case,do_match_2_que>, "Case/Qua Yes Otherwise"}, 
    { &do_match_3,                                   "When     No  Otherwise"}, 
    { &do_match_4,                                   "When     Yes Otherwise"}, 
    { &do_match_5,                                   "When     Empty"}
};
const int N = XTL_ARR_SIZE(funcs);

//------------------------------------------------------------------------------

size_t do_match_all(const Shape& s, size_t)
{
    for (int i = 1; i < N; ++i)
    {
        std::cout << "------------------- " << funcs[i].name << " -------------------" << std::endl;
        test(funcs[i].func,s);
    }

    return 0;
}

//------------------------------------------------------------------------------

int main(int argc, const char* argv[])
{
    if (fib(7) != 13) std::cout << "Fibonacci failed";
    if (fib(7) != 13) std::cout << "Fibonacci failed";
    if (fib(9) != 34) std::cout << "Fibonacci failed";

    Shape* shapes[] = {make_shape(0),make_shape(1),make_shape(2)};
    int i = argc > 1 ? std::atoi(argv[1]) : 0;

    if (i > 0 && i < N)
    {
        test(funcs[i].func,*shapes[0]);
        test(funcs[i].func,*shapes[1]);
        test(funcs[i].func,*shapes[2]);
    }
    else
    for (int i = 1; i < N; ++i)
    {
        std::cout << "------------------- " << funcs[i].name << " -------------------" << std::endl;
        test(funcs[i].func,*shapes[0]);
        test(funcs[i].func,*shapes[1]);
        test(funcs[i].func,*shapes[2]);
    }
}

//------------------------------------------------------------------------------
