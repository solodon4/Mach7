//
//  Mach7: Pattern Matching Library for C++
//
//  Copyright 2011-2013, Texas A&M University.
//  Copyright 2014 Yuriy Solodkyy.
//  All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//
//      * Redistributions of source code must retain the above copyright
//        notice, this list of conditions and the following disclaimer.
//
//      * Redistributions in binary form must reproduce the above copyright
//        notice, this list of conditions and the following disclaimer in the
//        documentation and/or other materials provided with the distribution.
//
//      * Neither the names of Mach7 project nor the names of its contributors
//        may be used to endorse or promote products derived from this software
//        without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
//  IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY
//  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
//  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

///
/// \file
///
/// This file is a part of Mach7 library test suite.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// \see https://parasol.tamu.edu/xtl/
/// \see https://parasol.tamu.edu/mach7/
/// \see https://github.com/solodon4/Mach7
/// \see https://github.com/solodon4/SELL
///

#ifndef HAS_BOOST

#include <iostream>

int main() {
    std::cout << "This test requires Boost!\n";
    return 1;
}

#else

#include <iostream>
#include <boost/variant.hpp>

#include <mach7/type_switchN-patterns-xtl.hpp> // Support for N-ary Match statement on patterns with arbitrary subtyping relation
#include <mach7/adapters/boost/adapt_boost_variant.hpp> // Mach7 adapters for boost::variant
#include <mach7/patterns/constructor.hpp>  // Support for constructor patterns
#include <mach7/patterns/primitive.hpp>    // Wildcard, variable and value patterns

//----------------------------------------------------------------------------------------------------------------------

static_assert(xtl::is_subtype<int,int>::value,"No reflexivity");

/// Experimenting with extending XTL subtyping mechanism to also have an equivalent of checked downcast.
void test_variant_subtyping()
{
    // Since any value that int can hold can be held inside that variant (semantic subtyping)
    static_assert(xtl::is_subtype<int,boost::variant<double,float,int,unsigned int*>>::value, "Not a subtype");

    typedef boost::variant<double,float,int,unsigned int*> my_variant_1;

    std::cout << "===========" << std::endl;
    {
        my_variant_1 v1 = xtl::subtype_cast<my_variant_1>(3.1415);
        std::cout << '(' << v1.which() << ',' << v1 << ')' << std::endl;

        my_variant_1 v2 = xtl::subtype_cast<my_variant_1>(42);
        std::cout << '(' << v2.which() << ',' << v2 << ')' << std::endl;

        std::cout << "-----------" << std::endl;

        double* p1 = xtl::subtype_dynamic_cast<double*>(&v1); std::cout << '(' << p1 << ',' << (p1 ? *p1 : 99999) << ')' << std::endl;
        float*  p2 = xtl::subtype_dynamic_cast<float* >(&v1); std::cout << '(' << p2 << ',' << (p2 ? *p2 : 88888) << ')' << std::endl;
        int*    p3 = xtl::subtype_dynamic_cast<int*   >(&v1); std::cout << '(' << p3 << ',' << (p3 ? *p3 : 77777) << ')' << std::endl;

        double* q1 = xtl::subtype_dynamic_cast<double*>(&v2); std::cout << '(' << q1 << ',' << (q1 ? *q1 : 66666) << ')' << std::endl;
        float*  q2 = xtl::subtype_dynamic_cast<float* >(&v2); std::cout << '(' << q2 << ',' << (q2 ? *q2 : 55555) << ')' << std::endl;
        int*    q3 = xtl::subtype_dynamic_cast<int*   >(&v2); std::cout << '(' << q3 << ',' << (q3 ? *q3 : 44444) << ')' << std::endl;

        std::cout << "sizeof(intptr_t)"  << sizeof(std::intptr_t) << std::endl;
        std::cout << "sizeof(v1.which_)" << sizeof(v1.which())    << std::endl;

    }

    std::cout << "===========" << std::endl;
    {
        const my_variant_1 v1 = xtl::subtype_cast<my_variant_1>(3.1415);
        std::cout << '(' << v1.which() << ',' << v1 << ')' << std::endl;

        const my_variant_1 v2 = xtl::subtype_cast<my_variant_1>(42);
        std::cout << '(' << v2.which() << ',' << v2 << ')' << std::endl;

        std::cout << "-----------" << std::endl;

        const double* p1 = xtl::subtype_dynamic_cast<const double*>(&v1); std::cout << '(' << p1 << ',' << (p1 ? *p1 : 99999) << ')' << std::endl;
        const float*  p2 = xtl::subtype_dynamic_cast<const float* >(&v1); std::cout << '(' << p2 << ',' << (p2 ? *p2 : 88888) << ')' << std::endl;
        const int*    p3 = xtl::subtype_dynamic_cast<const int*   >(&v1); std::cout << '(' << p3 << ',' << (p3 ? *p3 : 77777) << ')' << std::endl;

        const double* q1 = xtl::subtype_dynamic_cast<const double*>(&v2); std::cout << '(' << q1 << ',' << (q1 ? *q1 : 66666) << ')' << std::endl;
        const float*  q2 = xtl::subtype_dynamic_cast<const float* >(&v2); std::cout << '(' << q2 << ',' << (q2 ? *q2 : 55555) << ')' << std::endl;
        const int*    q3 = xtl::subtype_dynamic_cast<const int*   >(&v2); std::cout << '(' << q3 << ',' << (q3 ? *q3 : 44444) << ')' << std::endl;
    }

    using namespace mch;

    var<double> d;
    var<float>  f;
    var<int>    n;
    var<unsigned int*> p;
    std::cout << "===========" << std::endl;

    my_variant_1 vars[3] = {42, 3.14f, 9.876};

    for (int k = 0; k < 2; ++k)
        for (int i = 0; i < 3; ++i)
        {
            Match(vars[i])
            {
                Case(C<double>(d)) std::cout << "double " << d << std::endl; match0 += 1.0; break;
                Case(C<float> (f)) std::cout << "float  " << f << std::endl; match0 -= 1.0; break;
                Case(C<int>   (n)) std::cout << "int    " << n << std::endl; match0++;      break;
            }
            EndMatch
        }

    const my_variant_1 vars2[6] = {42, 3.14f, 9.876, 7, 2.72f, 1.2345};

    for (int i = 0; i < 6; ++i)
    {
        Match(vars2[i])
        {
            Case(C<double>(d)) std::cout << "double " << d << std::endl; break;
            Case(C<float> (f)) std::cout << "float  " << f << std::endl; break;
            Case(C<int>   (n)) std::cout << "int    " << n << std::endl; break;
//          Case(C<unsigned int*>(p)) std::cout << "unsigned int* " << p << "->" << *p << std::endl; break; // FIX: fails to compile
        }
        EndMatch
    }

    // Match on 2 arguments

    var<double> q;
    var<float>  l;
    var<int>    m;

    for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 6; ++j)
    {
        Match(vars[i], vars2[j])
        {
            Case(C<double>(d), C<double>(q)) std::cout << "double " << d << "\t double " << q << std::endl; match0 += 1.0; break;
            Case(C<float> (f), C<double>(q)) std::cout << "float  " << f << "\t double " << q << std::endl; match0 -= 1.0; break;
            Case(C<int>   (n), C<double>(q)) std::cout << "int    " << n << "\t double " << q << std::endl; match0++;      break;

            Case(C<double>(d), C<float>(l) ) std::cout << "double " << d << "\t float  " << l << std::endl; match0 += 1.0; break;
            Case(C<float> (f), C<float>(l) ) std::cout << "float  " << f << "\t float  " << l << std::endl; match0 -= 1.0; break;
            Case(C<int>   (n), C<float>(l) ) std::cout << "int    " << n << "\t float  " << l << std::endl; match0++;      break;

            Case(C<double>(d), C<int>(m)   ) std::cout << "double " << d << "\t int    " << m << std::endl; match0 += 1.0; break;
            Case(C<float> (f), C<int>(m)   ) std::cout << "float  " << f << "\t int    " << m << std::endl; match0 -= 1.0; break;
            Case(C<int>   (n), C<int>(m)   ) std::cout << "int    " << n << "\t int    " << m << std::endl; match0++;      break;
        }
        EndMatch
    }

}

struct M { char   im; M(char   m = 0)   : im(m) {} };
struct N { double in; N(double n = 0.0) : in(n) {} };

std::ostream& operator<<(std::ostream& os, const M& m) { return os << m.im; } // Helper to allow printing of M
std::ostream& operator<<(std::ostream& os, const N& n) { return os << n.in; } // Helper to allow printing of N

typedef boost::variant<M, N> VM;

/// Tests mutability of the subject and binding by reference
void test_variant_mutation()
{
    using namespace mch;

    VM vm[2] = { M('A'), N(3.1415926) };

    for (size_t i = 0; i < 2; ++i)
    {
        Match(vm[i])
        {
            Case(C<M>()) match0.im = 'm';         break;
            Case(C<N>()) match0.in = 2*match0.in; break;
        }
        EndMatch
    }

    for (size_t i = 0; i < 2; ++i)
    {
        var<M&> m; var<N&> n; // Demonstrates assignment to the actual subject via variable patterns of reference type

        Match(vm[i])
        {
            Case(C<M>(m))
                std::cout << "M -> " << match0.im << '=' << m << std::endl;
                m = M('X');
                std::cout << "M -> " << match0.im << '=' << m << std::endl;
                break;
            Case(C<N>(n))
                std::cout << "N -> " << match0.in << '=' << n << std::endl;
                n = N(42);
                std::cout << "N -> " << match0.in << '=' << n << std::endl;
                break;
        }
        EndMatch
    }
}

//----------------------------------------------------------------------------------------------------------------------

struct A             { int    a; virtual void foo() { std::cout << "A" << std::endl; } virtual void foo() const { std::cout << "const A" << std::endl; } A() {} };
struct B : A         { int    b; virtual void foo() { std::cout << "B" << std::endl; } virtual void foo() const { std::cout << "const B" << std::endl; } B() {} };
struct C : A         { double c; virtual void foo() { std::cout << "C" << std::endl; } virtual void foo() const { std::cout << "const C" << std::endl; } };
struct D : C, B      { float  d; virtual void foo() { std::cout << "D" << std::endl; } virtual void foo() const { std::cout << "const D" << std::endl; } };
struct X : virtual A { char   x; virtual void foo() { std::cout << "X" << std::endl; } virtual void foo() const { std::cout << "const X" << std::endl; } };
struct Y : virtual A { short  y; virtual void foo() { std::cout << "Y" << std::endl; } virtual void foo() const { std::cout << "const Y" << std::endl; } };
struct Z : X, Y      { char   z; virtual void foo() { std::cout << "Z" << std::endl; } virtual void foo() const { std::cout << "const Z" << std::endl; } };

static_assert( xtl::is_subtype<      B ,       A >::value, "      B  <:       A ");
static_assert( xtl::is_subtype<      B*,       A*>::value, "      B* <:       A*");
static_assert( xtl::is_subtype<const B*, const A*>::value, "const B* <: const A*");
static_assert( xtl::is_subtype<      B*, const A*>::value, "      B* <: const A*");
static_assert(!xtl::is_subtype<const B*,       A*>::value, "const B* <:       A*");

static_assert(!xtl::is_subtype<      A ,       B >::value, "      A  <:       B ");
static_assert(!xtl::is_subtype<      A*,       B*>::value, "      A* <:       B*");
static_assert(!xtl::is_subtype<const A*, const B*>::value, "const A* <: const B*");
static_assert(!xtl::is_subtype<      A*, const B*>::value, "      A* <: const B*");
static_assert(!xtl::is_subtype<const A*,       B*>::value, "const A* <:       B*");

/// Tests new XTL functionaliy on subtyping induced by subclassing
void test_subtype_dynamic_cast()
{
    A a; const A ca;
    B b; const B cb;
    a = xtl::subtype_cast<A>(b);
    a = xtl::subtype_cast<A>(cb);
    //b = xtl::subtype_cast<B>(a); // error

    A* qa = xtl::subtype_cast<A*>(&b);
    const A* qaa= xtl::subtype_cast<const A*>(&cb); XTL_UNUSED(qaa);
    //B* qb = xtl::subtype_cast<B*>(&a); // error
    B* qb = xtl::subtype_dynamic_cast<B*>(qa);
    qa->foo();
    if (qb) qb->foo();

    std::cout << "===========" << std::endl;
    {
        A* pA = new A;
        B* pB = new B;
        C* pC = new C;
        D* pD = new D;
        X* pX = new X;
        Y* pY = new Y;
        Z* pZ = new Z;

        A* pa = xtl::subtype_cast<A*>(pA);
        A* pb = xtl::subtype_cast<A*>(pB);
        A* pc = xtl::subtype_cast<A*>(pC);
        A* pdb= xtl::subtype_cast<A*>(static_cast<B*>(pD));
        A* pdc= xtl::subtype_cast<A*>(static_cast<C*>(pD));
        A* px = xtl::subtype_cast<A*>(pX);
        A* py = xtl::subtype_cast<A*>(pY);
        A* pz = xtl::subtype_cast<A*>(pZ);

        if (A* p = xtl::subtype_dynamic_cast<A*>(pa)) p->foo();
        if (B* p = xtl::subtype_dynamic_cast<B*>(pb)) p->foo();
        if (C* p = xtl::subtype_dynamic_cast<C*>(pc)) p->foo();
        if (D* p = xtl::subtype_dynamic_cast<D*>(pdb))p->foo();
        if (D* p = xtl::subtype_dynamic_cast<D*>(pdc))p->foo();
        if (X* p = xtl::subtype_dynamic_cast<X*>(px)) p->foo();
        if (Y* p = xtl::subtype_dynamic_cast<Y*>(py)) p->foo();
        if (Z* p = xtl::subtype_dynamic_cast<Z*>(pz)) p->foo();
        if (B* p = xtl::subtype_dynamic_cast<B*>(pdc))p->foo();
        if (C* p = xtl::subtype_dynamic_cast<C*>(pdb))p->foo();

        if (const A* p = xtl::subtype_dynamic_cast<const A*>(pa)) p->foo();
        if (const B* p = xtl::subtype_dynamic_cast<const B*>(pb)) p->foo();
        if (const C* p = xtl::subtype_dynamic_cast<const C*>(pc)) p->foo();
        if (const D* p = xtl::subtype_dynamic_cast<const D*>(pdb))p->foo();
        if (const D* p = xtl::subtype_dynamic_cast<const D*>(pdc))p->foo();
        if (const X* p = xtl::subtype_dynamic_cast<const X*>(px)) p->foo();
        if (const Y* p = xtl::subtype_dynamic_cast<const Y*>(py)) p->foo();
        if (const Z* p = xtl::subtype_dynamic_cast<const Z*>(pz)) p->foo();
        if (const B* p = xtl::subtype_dynamic_cast<const B*>(pdc))p->foo();
        if (const C* p = xtl::subtype_dynamic_cast<const C*>(pdb))p->foo();
    }
    {
        const A* pA = new A;
        const B* pB = new B;
        const C* pC = new C;
        const D* pD = new D;
        const X* pX = new X;
        const Y* pY = new Y;
        const Z* pZ = new Z;

        const A* pa = xtl::subtype_cast<const A*>(pA);
        const A* pb = xtl::subtype_cast<const A*>(pB);
        const A* pc = xtl::subtype_cast<const A*>(pC);
        const A* pdb= xtl::subtype_cast<const A*>(static_cast<const B*>(pD));
        const A* pdc= xtl::subtype_cast<const A*>(static_cast<const C*>(pD));
        const A* px = xtl::subtype_cast<const A*>(pX);
        const A* py = xtl::subtype_cast<const A*>(pY);
        const A* pz = xtl::subtype_cast<const A*>(pZ);

        if (const A* p = xtl::subtype_dynamic_cast<const A*>(pa)) p->foo();
        if (const B* p = xtl::subtype_dynamic_cast<const B*>(pb)) p->foo();
        if (const C* p = xtl::subtype_dynamic_cast<const C*>(pc)) p->foo();
        if (const D* p = xtl::subtype_dynamic_cast<const D*>(pdb))p->foo();
        if (const D* p = xtl::subtype_dynamic_cast<const D*>(pdc))p->foo();
        if (const X* p = xtl::subtype_dynamic_cast<const X*>(px)) p->foo();
        if (const Y* p = xtl::subtype_dynamic_cast<const Y*>(py)) p->foo();
        if (const Z* p = xtl::subtype_dynamic_cast<const Z*>(pz)) p->foo();
        if (const B* p = xtl::subtype_dynamic_cast<const B*>(pdc))p->foo();
        if (const C* p = xtl::subtype_dynamic_cast<const C*>(pdb))p->foo();

        if (const A* p = xtl::subtype_dynamic_cast<const A*>(pa)) p->foo();
        if (const B* p = xtl::subtype_dynamic_cast<const B*>(pb)) p->foo();
        if (const C* p = xtl::subtype_dynamic_cast<const C*>(pc)) p->foo();
        if (const D* p = xtl::subtype_dynamic_cast<const D*>(pdb))p->foo();
        if (const D* p = xtl::subtype_dynamic_cast<const D*>(pdc))p->foo();
        if (const X* p = xtl::subtype_dynamic_cast<const X*>(px)) p->foo();
        if (const Y* p = xtl::subtype_dynamic_cast<const Y*>(py)) p->foo();
        if (const Z* p = xtl::subtype_dynamic_cast<const Z*>(pz)) p->foo();
        if (const B* p = xtl::subtype_dynamic_cast<const B*>(pdc))p->foo();
        if (const C* p = xtl::subtype_dynamic_cast<const C*>(pdb))p->foo();
    }

    static_assert(xtl::is_subtype<B,A>::value,"Subclassing");
}

//----------------------------------------------------------------------------------------------------------------------

// The following is also an attempt to handle tagged classes via XTL
struct Shape
{
    enum Kind { KUnknown, KRectangle, KEllipse, KTriangle } kind;
    static const Kind class_kind = KUnknown;
    Shape(Kind k = KUnknown) : kind(k) {}
    virtual void foo()       { std::cout <<       "Shape" << std::endl; }
    virtual void foo() const { std::cout << "const Shape" << std::endl; }
};

struct Rectange : Shape { static const Kind class_kind = KRectangle; Rectange() : Shape(KRectangle) {} virtual void foo() { std::cout << "Rectangle" << std::endl; } virtual void foo() const { std::cout << "const Rectangle" << std::endl; } };
struct Ellipse  : Shape { static const Kind class_kind = KEllipse;   Ellipse()  : Shape(KEllipse)   {} virtual void foo() { std::cout << "Ellipse"   << std::endl; } virtual void foo() const { std::cout << "const Ellipse"   << std::endl; } };
struct Triangle : Shape { static const Kind class_kind = KTriangle;  Triangle() : Shape(KTriangle)  {} virtual void foo() { std::cout << "Triangle"  << std::endl; } virtual void foo() const { std::cout << "const Triangle"  << std::endl; } };

namespace xtl
{
    // Tell XTL that Shape hierarchy is XTL-polymorphic
    template<>
    struct is_poly_morphic<Shape>
    {
        static const bool value = true;
    };

    // Explains to XTL how to perform a subtype dynamic cast on a shape
    template <class T>
    typename std::enable_if<xtl::is_subtype<T, Shape>::value && !std::is_same<T, Shape>::value, T*>::type
    subtype_dynamic_cast_impl(target<T*>, Shape* pv)
    {
        return T::class_kind == pv->kind ? static_cast<T*>(pv) : nullptr;
    }
} // of namespace xtl

/// Checks tagged classes can be expressed withing the XTL approach as well
void test_shape_subtyping()
{
    typedef Shape    A;
    typedef Rectange B;
    typedef Ellipse  C;
    typedef Triangle D;
    A a; const A ca;
    B b; const B cb;
    a = xtl::subtype_cast<A>(b);
    a = xtl::subtype_cast<A>(cb);
    //b = xtl::subtype_cast<B>(a); // error

    A* qa = xtl::subtype_cast<A*>(&b);
    const A* qaa= xtl::subtype_cast<const A*>(&cb); XTL_UNUSED(qaa);
    //B* qb = xtl::subtype_cast<B*>(&a); // error
    B* qb = xtl::subtype_dynamic_cast<B*>(qa);
    qa->foo();
    if (qb) qb->foo();

    std::cout << "===========" << std::endl;
    {
        A* pA = new A;
        B* pB = new B;
        C* pC = new C;
        D* pD = new D;

        A* pa = xtl::subtype_cast<A*>(pA);
        A* pb = xtl::subtype_cast<A*>(pB);
        A* pc = xtl::subtype_cast<A*>(pC);
        A* pd = xtl::subtype_cast<A*>(pD);

        if (A* p = xtl::subtype_dynamic_cast<A*>(pa)) p->foo();
        if (B* p = xtl::subtype_dynamic_cast<B*>(pb)) p->foo();
        if (C* p = xtl::subtype_dynamic_cast<C*>(pc)) p->foo();
        if (D* p = xtl::subtype_dynamic_cast<D*>(pd)) p->foo();

        if (const A* p = xtl::subtype_dynamic_cast<const A*>(pa)) p->foo();
        if (const B* p = xtl::subtype_dynamic_cast<const B*>(pb)) p->foo();
        if (const C* p = xtl::subtype_dynamic_cast<const C*>(pc)) p->foo();
        if (const D* p = xtl::subtype_dynamic_cast<const D*>(pd)) p->foo();
    }
    {
        const A* pA = new A;
        const B* pB = new B;
        const C* pC = new C;
        const D* pD = new D;

        const A* pa = xtl::subtype_cast<const A*>(pA);
        const A* pb = xtl::subtype_cast<const A*>(pB);
        const A* pc = xtl::subtype_cast<const A*>(pC);
        const A* pd = xtl::subtype_cast<const A*>(pD);

        if (const A* p = xtl::subtype_dynamic_cast<const A*>(pa)) p->foo();
        if (const B* p = xtl::subtype_dynamic_cast<const B*>(pb)) p->foo();
        if (const C* p = xtl::subtype_dynamic_cast<const C*>(pc)) p->foo();
        if (const D* p = xtl::subtype_dynamic_cast<const D*>(pd)) p->foo();

        if (const A* p = xtl::subtype_dynamic_cast<const A*>(pa)) p->foo();
        if (const B* p = xtl::subtype_dynamic_cast<const B*>(pb)) p->foo();
        if (const C* p = xtl::subtype_dynamic_cast<const C*>(pc)) p->foo();
        if (const D* p = xtl::subtype_dynamic_cast<const D*>(pd)) p->foo();
    }

    static_assert(xtl::is_subtype<B,A>::value,"Subclassing");

    const Shape* shapes[8] = {new Shape, new Rectange, new Ellipse, new Triangle,
                              new Shape, new Rectange, new Ellipse, new Triangle};

    for (int i = 0; i < 8; ++i)
    {
        Match(shapes[i])
        {
            Case(mch::C<Triangle>()) std::cout << "Triangle: " << std::endl; break;
            Case(mch::C<Ellipse> ()) std::cout << "Ellipse:  " << std::endl; break;
            Case(mch::C<Rectange>()) std::cout << "Rectange: " << std::endl; break;
            Case(mch::C<Shape>()   ) std::cout << "Shape:    " << std::endl; break;
        }
        EndMatch
    }

}

int main()
{
    test_subtype_dynamic_cast();
    test_variant_subtyping();
    test_variant_mutation();
    test_shape_subtyping();
}

#endif
