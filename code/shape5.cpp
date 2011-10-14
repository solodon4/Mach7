#include <utility>
#include "match_generic.hpp"

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

template <> struct match_members<Shape>    { KS(Shape::kind); };

template <> struct match_members<Circle>   { KV(Shape::SK_Circle);  CM(0,Circle::get_center); CM(1,Circle::radius); };
template <> struct match_members<Square>   { KV(Shape::SK_Square);  CM(0,Square::upper_left); CM(1,Square::side);   };
template <> struct match_members<Triangle> { KV(Shape::SK_Triangle);CM(0,Triangle::first);    CM(1,Triangle::second); CM(2,Triangle::third); };

template <> struct match_members<ADTShape> { KS(ADTShape::kind); };

template <> struct match_members<ADTShape,ADTShape::circle>   { KV(ADTShape::circle);  CM(0,ADTShape::center);     CM(1,ADTShape::radius); };
template <> struct match_members<ADTShape,ADTShape::square>   { KV(ADTShape::square);  CM(0,ADTShape::upper_left); CM(1,ADTShape::size); };
template <> struct match_members<ADTShape,ADTShape::triangle> { KV(ADTShape::triangle);CM(0,ADTShape::first);      CM(1,ADTShape::second); CM(2,ADTShape::third); };

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

    Shape* shapes[] = {c,s,t};

    wildcard _;
    double   x;
    loc      l;
    cloc     cl;

    double m = 0.0;

    for (size_t i = 0; i < 3; ++i)
    {
        KIND_SWITCH(shapes[i])
        {
        KIND_CASES_BEGIN 
        KIND_CASE(Circle,_,x) std::cout << "Circle"   << std::endl; m += x;       break;
        KIND_CASE(Square,_,x) std::cout << "Square"   << std::endl; m += x;       break;
        KIND_CASE(Triangle,l) std::cout << "Triangle" << std::endl; m += l.first; break;
      //KIND_CASE(Triangle,l) std::cout << "Triangle" << std::endl; m += l.first; break; // NOTE: Not possible to have another kind match
        KIND_CASES_END
        }
    }

    std::cout << m << std::endl;

    m = 0.0;

    for (size_t i = 0; i < 3; ++i)
    {
        TYPE_SWITCH(shapes[i])
        {
        TYPE_CASES_BEGIN 
        TYPE_CASE(Circle)   std::cout << "Circle"   << std::endl; m += matched->radius;      break;
        TYPE_CASE(Square)   std::cout << "Square"   << std::endl; m += matched->side;        break;
        TYPE_CASE(Triangle) std::cout << "Triangle" << std::endl; m += matched->first.first; break;
        TYPE_CASE(Triangle) std::cout << "Triangle" << std::endl; m += matched->first.first; break; // NOTE: Possible to have another type case match
        TYPE_CASES_END
        }
    }

    std::cout << m << std::endl;

    m = 0.0;

    for (size_t i = 0; i < 3; ++i)
    {
        SWITCH(shapes[i])
        {
        CASES_BEGIN 
        CASE(Circle,_,x) std::cout << "Circle"   << std::endl; m += x;       break;
        CASE(Square,_,x) std::cout << "Square"   << std::endl; m += x;       break;
        CASE(Triangle,l) std::cout << "Triangle" << std::endl; m += l.first; break;
        CASE(Triangle,l) std::cout << "Triangle" << std::endl; m += l.first; break; // NOTE: Possible to have another regular match
        CASES_END
        }
    }

    std::cout << m << std::endl;

    m = 0.0;

    for (size_t i = 0; i < 3; ++i)
    {
        UNION_SWITCH(adtshapes[i])
        {
        UNION_CASES_BEGIN 
        UNION_CASE(ADTShape,ADTShape::circle,_,x)  std::cout << "ADTCircle"   << std::endl; m += x;       break;
        UNION_CASE(ADTShape,ADTShape::square,_,x)  std::cout << "ADTSquare"   << std::endl; m += x;       break;
        UNION_CASE(ADTShape,ADTShape::triangle,cl) std::cout << "ADTTriangle" << std::endl; m += cl.first;break;
      //UNION_CASE(ADTShape,ADTShape::triangle,cl) std::cout << "ADTTriangle" << std::endl; m += cl.first;break; // NOTE: Not possible to have another union match
        UNION_CASES_END
        }
    }

    std::cout << m << std::endl;

    m = 0.0;

    for (size_t i = 0; i < 3; ++i)
    {
        GENERIC_SWITCH(shapes[i])
        {
        GENERIC_CASES_BEGIN 
        GENERIC_CASE(Circle,_,x) std::cout << "Circle"   << std::endl; m += x;       break;
        GENERIC_CASE(Square,_,x) std::cout << "Square"   << std::endl; m += x;       break;
        GENERIC_CASE(Triangle,l) std::cout << "Triangle" << std::endl; m += l.first; break;
        GENERIC_CASES_END
        }
    }

    std::cout << m << std::endl;

    m = 0.0;

    for (size_t i = 0; i < 3; ++i)
    {
        GENERIC_SWITCH(adtshapes[i])
        {
        GENERIC_CASES_BEGIN 
        GENERIC_CASE(ADTShape::circle,_,x)  std::cout << "ADTCircle"   << std::endl; m += x;       break;
        GENERIC_CASE(ADTShape::square,_,x)  std::cout << "ADTSquare"   << std::endl; m += x;       break;
        GENERIC_CASE(ADTShape::triangle,cl) std::cout << "ADTTriangle" << std::endl; m += cl.first;break;
        GENERIC_CASES_END
        }
    }

    std::cout << m << std::endl;

    generic_switch<Shape>::foo();
    generic_switch<Circle>::foo();
}
