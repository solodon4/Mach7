#include <iostream>
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

template <> struct match_members<Shape>    { /*KS(Shape::kind);*/ };

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

    const Shape* shapes[] = {c,s,t};

    wildcard _;
    double   x;
    variable<double> v;
    loc      l;
    cloc     cl;

    double m = 0.0;

    for (size_t i = 0; i < 3; ++i)
    {
        TypeMatch(shapes[i])
        {
        TypeCase(Circle)   std::cout << "Circle"   << std::endl; m += matched->radius;      break;
        TypeCase(Square)   std::cout << "Square"   << std::endl; m += matched->side;        break;
        TypeCase(Triangle) std::cout << "Triangle" << std::endl; m += matched->first.first; break;
        TypeCase(Triangle) std::cout << "Triangle" << std::endl; m += matched->first.first; break; // NOTE: Possible to have another type case match
        }
        EndTypeMatch
    }

    std::cout << m << std::endl;

    std::cout << "CASES" << std::endl;

    m = 0.0;

    for (size_t i = 0; i < 3; ++i)
    {
        MatchP(shapes[i])
        {
        CaseP(Circle,_,r)    std::cout << "Circle"   << std::endl; m += r;       break;
        CaseP(Square,_,r)    std::cout << "Square"   << std::endl; m += r;       break;
        CaseP(Triangle,p)    std::cout << "Triangle" << std::endl; m += p.first; break;
        CaseP(Triangle,p)    std::cout << "Triangle" << std::endl; m += p.first; break; // NOTE: Possible to have another regular match
        }
        EndMatchP
     }

    std::cout << m << std::endl;

    // Open/Closed case with bound variables explicitly declared

    m = 0.0;

    for (size_t i = 0; i < 3; ++i)
    {
        Match(shapes[i])
        {
            Que(Circle,_,x)          std::cout << "Circle"   << std::endl; m += x;       break;
            Que(Square,_,v |= v > 5) std::cout << "Square>5" << std::endl; m += v;       break;
             Alt(      _,v |= v > 3) std::cout << "Square>3" << std::endl; m += v;       break;
             Alt(      _,v |= v > 1) std::cout << "Square>1" << std::endl; m += v;       break;
             Alt(      _,v |= v > 0) std::cout << "Square>0" << std::endl; m += v;       break;
             Alt(      _,x)          std::cout << "Square"   << std::endl; m += x;       break;
            Que(Triangle,l)          std::cout << "Triangle" << std::endl; m += l.first; break;
            Otherwise()              std::cout << "Other"    << std::endl; m += 2;       break;
        }
        EndMatch
    }

    std::cout << m << std::endl;

    // Open/Closed case with bound variables automatically declared

    m = 0.0;

    for (size_t i = 0; i < 3; ++i)
    {
        Match(shapes[i])
        {
            Case(Circle,c,r)  std::cout << "Circle"   << std::endl; m += r;       break;
            Case(Square,c,s)  std::cout << "Square"   << std::endl; m += s;       break;
            Case(Triangle,p)  std::cout << "Triangle" << std::endl; m += p.first; break;
            Otherwise()       std::cout << "Other"    << std::endl; m += 2;       break;
        }
        EndMatch
    }

    std::cout << m << std::endl;

    std::cout << "PREPROCESSSED" << std::endl;

    m = 0.0;
    for (size_t i = 0; i < 3; ++i)
    {
        //std::cout << "Selector: " << adtshapes[i]->kind << std::endl;
        {
            auto const __selector_ptr = addr(adtshapes[i]);
            ;
            enum 
            {
                __base_line = 171 
            }
            ;
            typedef generic_switch<decltype(*__selector_ptr)> switch_traits;
            static switch_traits::static_data_type static_data;
            switch_traits::local_data_type local_data;
            //int choice = switch_traits::choose(__selector_ptr,static_data,local_data);
            //std::cout << "Choice: " << choice << std::endl;
            switch (switch_traits::choose(__selector_ptr,static_data,local_data)) 
            {
            case switch_traits::CaseLabel<171-__base_line>::entry:
                {
                    {
                        {
                            {
                            }
                        }
                    }
                    {
                        typedef switch_traits::disambiguate<sizeof(ADTShape::circle)<sizeof(switch_traits::selector_type)>::parameter<ADTShape::circle> target_specific;
                        if (target_specific::main_condition(__selector_ptr, local_data)) 
                        {
                            switch_traits::on_first_pass(__selector_ptr, local_data, 174-__base_line);
            case target_specific::CaseLabel<174-__base_line>::value:
                            //std::cout << "Jumped to : " << target_specific::CaseLabel<174-__base_line>::value << std::endl;
                            auto matched = target_specific::get_matched(__selector_ptr,local_data);
                            if ((match<target_specific::target_type,target_specific::layout>(_,x)(matched))) 
                            {
                                std::cout << "ADTCircle"   << std::endl;
                                m += x;
                                break;
                            }
                        }
                    }
                    {
                        typedef switch_traits::disambiguate<sizeof(ADTShape::square)<sizeof(switch_traits::selector_type)>::parameter<ADTShape::square> target_specific;
                        if (target_specific::main_condition(__selector_ptr, local_data)) 
                        {
                            switch_traits::on_first_pass(__selector_ptr, local_data, 175-__base_line);
            case target_specific::CaseLabel<175-__base_line>::value:
                            //std::cout << "Jumped to : " << target_specific::CaseLabel<175-__base_line>::value << std::endl;
                            auto matched = target_specific::get_matched(__selector_ptr,local_data);
                            if ((match<target_specific::target_type,target_specific::layout>(_,v |= v > 5)(matched))) 
                            {
                                std::cout << "ADTSquare>5" << std::endl;
                                m += v;
                                break;
                            }
                            else if (match<target_specific::target_type,target_specific::layout>(_,v |= v > 3)(matched)) 
                            {
                                std::cout << "ADTSquare>3" << std::endl;
                                m += v;
                                break;
                            }
                            else if (match<target_specific::target_type,target_specific::layout>(_,v |= v > 1)(matched)) 
                            {
                                std::cout << "ADTSquare>1" << std::endl;
                                m += v;
                                break;
                            }
                            else if (match<target_specific::target_type,target_specific::layout>(_,v |= v > 0)(matched)) 
                            {
                                std::cout << "ADTSquare>0" << std::endl;
                                m += v;
                                break;
                            }
                            else if (match<target_specific::target_type,target_specific::layout>(_,x)(matched)) 
                            {
                                std::cout << "ADTSquare"   << std::endl;
                                m += x;
                                break;
                            }
                        }
                    }
                    {
                        typedef switch_traits::disambiguate<sizeof(ADTShape::triangle)<sizeof(switch_traits::selector_type)>::parameter<ADTShape::triangle> target_specific;
                        if (target_specific::main_condition(__selector_ptr, local_data)) 
                        {
                            switch_traits::on_first_pass(__selector_ptr, local_data, 180-__base_line);
            case target_specific::CaseLabel<180-__base_line>::value:
                            //std::cout << "Jumped to : " << target_specific::CaseLabel<180-__base_line>::value << std::endl;
                            auto matched = target_specific::get_matched(__selector_ptr,local_data);
                            if ((match<target_specific::target_type,target_specific::layout>(cl)(matched))) 
                            {
                                std::cout << "ADTTriangle" << std::endl;
                                m += cl.first;
                                break;
                            }
                        }
                    }
                }
                switch_traits::on_end(__selector_ptr, local_data, 182-__base_line);
            case switch_traits::CaseLabel<182-__base_line>::exit:
                //std::cout << "Jumped to : " << switch_traits::CaseLabel<182-__base_line>::exit << std::endl;
                ;
            }
        }
    }

    std::cout << m << std::endl;

// Disambiguation between value and type in first argument of Match macro is only possible when Match resolves to MatchG
#if XTL_DEFAULT_SYNTAX == 'G'

    // Union case with bound variables explicitly declared

    m = 0.0;

    for (size_t i = 0; i < 3; ++i)
    {
        //std::cout << "Selector: " << adtshapes[i]->kind << std::endl;
        Match(adtshapes[i])
        {
            Otherwise()                        std::cout << "Other"       << std::endl; m += 2;       break;
            Que(ADTShape::circle,_,x)          std::cout << "ADTCircle"   << std::endl; m += x;       break;
            Que(ADTShape::square,_,v |= v > 5) std::cout << "ADTSquare>5" << std::endl; m += v;       break;
             Alt(                _,v |= v > 3) std::cout << "ADTSquare>3" << std::endl; m += v;       break;
             Alt(                _,v |= v > 1) std::cout << "ADTSquare>1" << std::endl; m += v;       break;
             Alt(                _,v |= v > 0) std::cout << "ADTSquare>0" << std::endl; m += v;       break;
             Alt(                _,x)          std::cout << "ADTSquare"   << std::endl; m += x;       break;
            Que(ADTShape::triangle,cl)         std::cout << "ADTTriangle" << std::endl; m += cl.first;break;
        }
        EndMatch
    }

    std::cout << m << std::endl;

    // Union case with bound variables automatically declared

    m = 0.0;

    for (size_t i = 0; i < 3; ++i)
    {
        Match(adtshapes[i])
        {
            Case(ADTShape::circle,c,r)  std::cout << "ADTCircle"   << std::endl; m += r;       break;
            Case(ADTShape::square,c,s)  std::cout << "ADTSquare"   << std::endl; m += s;       break;
            Case(ADTShape::triangle,p)  std::cout << "ADTTriangle" << std::endl; m += p.first; break;
            Otherwise()                 std::cout << "Other"       << std::endl; m += 2;       break;
        }
        EndMatch
    }

    std::cout << m << std::endl;

#endif

    m = 0.0;

    for (size_t i = 0; i < 3; ++i)
    {
        Match(shapes[i])
        {
            Que(Circle,_,x)
                std::cout << "Circle"   << std::endl;
                m += x;
                break;
            Que(Square,_,v |= v > 5)
                std::cout << "Square>5" << std::endl;
                m += v;
                break;
             Alt(      _,v |= v > 3)
                std::cout << "Square>3" << std::endl;
                m += v;
                break;
             Alt(      _,v |= v > 1)
                std::cout << "Square>1" << std::endl;
                m += v;
                break;
             Alt(      _,v |= v > 0)
                std::cout << "Square>0" << std::endl;
                m += v;
                break;
             Alt(      _,x)
                std::cout << "Square"   << std::endl;
                m += x;
                break;
            Que(Triangle,l)
                std::cout << "Triangle" << std::endl;
                m += l.first;
                break;
            Otherwise()
                std::cout << "Other"    << std::endl;
                m += 2;
                break;
        }
        EndMatch
    }

    std::cout << m << std::endl;
}

void test_read(const Shape* shape)
{
    Match(shape)
    {
        Que(Circle)   const Circle*   s = matched; break;
        Que(Square)   const Square*   s = matched; break;
        Que(Triangle) const Triangle* s = matched; break;
        Otherwise()   const Shape*    s = matched; break;
    }
    EndMatch
}

void test_write(Shape* shape)
{
    Match(shape)
    {
        Que(Circle)         Circle*   s = matched; break;
        Que(Square)         Square*   s = matched; break;
        Que(Triangle)       Triangle* s = matched; break;
        Otherwise()         Shape*    s = matched; break;
    }
    EndMatch
}

void test_autodecl(const Shape* shape)
{
    double m = 0.0;

    Match(shape)
    {
        Case(Circle,c,r)  std::cout << "Circle"   << std::endl; m += r;       break;
        Case(Square,c,s)  std::cout << "Square"   << std::endl; m += s;       break;
        Case(Triangle,p)  std::cout << "Triangle" << std::endl; m += p.first; break;
        Otherwise()       std::cout << "Other"    << std::endl; m += 2;       break;
    }
    EndMatch
}
