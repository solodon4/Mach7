#include <utility>
#include "match.hpp"

template <typename T> struct remove_ref     { typedef T type; };
template <typename T> struct remove_ref<T&> { typedef T type; };


#define KS(...)  static inline decltype(&__VA_ARGS__) kind_selector() { return &__VA_ARGS__; }
#define KV(kind) static const size_t kind_value = kind;
/// Macro that starts the switch on dynamic type of a variable s that can be 
/// either pointer or reference to a polymorphic type.
#define KIND_SWITCH(s)\
        auto const   __selector_ptr = addr(s);\
        const void*  __casted_ptr;\
        switch (apply_member(__selector_ptr, match_members<remove_ref<decltype(*__selector_ptr)>::type>::kind_selector()))

/// Macro that defines the case statement for the above switch
/// NOTE: If Visual C++ gives you error C2051: case expression not constant
///       on this CASE label, just change the Debug Format in project setting 
///       Project -> Properties -> C/C++ -> General -> Debug Information Format 
///       from "Program Database for Edit And Continue (/ZI)" 
///       to   "Program Database (/Zi)", which is the default in Release builds,
///       but not in Debug. This is a known bug of Visual C++ described here:
///       http://connect.microsoft.com/VisualStudio/feedback/details/375836/-line-not-seen-as-compile-time-constant
#define KIND_CASE(C,...) } case match_members<C>::kind_value: { auto result_of_dynamic_cast = static_cast<C*>(__selector_ptr);
#define KIND_CASES_BEGIN {
#define KIND_CASES_END   } default: ;

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

template <> struct match_members<Shape>    { KS(Shape::kind); };

template <> struct match_members<Circle>   { KV(Shape::SK_Circle);  CM(0,Circle::get_center); CM(1,Circle::radius); };
template <> struct match_members<Square>   { KV(Shape::SK_Square);  CM(0,Square::upper_left); CM(1,Square::side);   };
template <> struct match_members<Triangle> { KV(Shape::SK_Triangle);CM(0,Triangle::first);    CM(1,Triangle::second); CM(2,Triangle::third); };

int main()
{
    Shape* c = new Circle(loc(1,1),7);
    Shape* s = new Square(loc(2,2),2);
    Shape* t = new Triangle(loc(0,0),loc(0,1),loc(1,0));

    Shape* shapes[] = {c,s,t};
    size_t m = 0;

    for (size_t i = 0; i < 3; ++i)
    {
        KIND_SWITCH(shapes[i])
        {
        KIND_CASES_BEGIN 
        KIND_CASE(Circle)   m += result_of_dynamic_cast->radius;      break;
        KIND_CASE(Square)   m += result_of_dynamic_cast->side;        break;
        KIND_CASE(Triangle) m += result_of_dynamic_cast->first.first; break;
        KIND_CASES_END
        }
    }

    std::cout << m << std::endl;
}
