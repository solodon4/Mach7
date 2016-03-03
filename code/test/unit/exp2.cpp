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
/// \see https://parasol.tamu.edu/mach7/
/// \see https://github.com/solodon4/Mach7
/// \see https://github.com/solodon4/SELL
///

#include <mach7/type_switchN-patterns.hpp> // Support for N-ary Match statement on patterns
#include <mach7/patterns/address.hpp>      // Address and dereference combinators
#include <mach7/patterns/any.hpp>          // Support for one-of pattern
#include <mach7/patterns/bindings.hpp>     // Mach7 support for bindings on arbitrary UDT
#include <mach7/patterns/combinators.hpp>  // Support for pattern combinators &&, || and !
#include <mach7/patterns/constructor.hpp>  // Support for constructor patterns
#include <mach7/patterns/equivalence.hpp>  // Equivalence combinator +
#include <mach7/patterns/guard.hpp>        // Support for guard patterns
#include <mach7/patterns/n+k.hpp>          // Generalized n+k patterns
#include <mach7/patterns/primitive.hpp>    // Wildcard, variable and value patterns
#include <mach7/patterns/regex.hpp>        // Regular expression patterns

#include <iostream>
#include <string>

struct Expr          { virtual ~Expr() {} };
struct Value  : Expr { int value; Value (int v) : value(v) {} };
struct Plus   : Expr { const Expr* exp1; const Expr* exp2; Plus  (const Expr* e1, const Expr* e2) : exp1(e1), exp2(e2) {} };
struct Minus  : Expr { const Expr* exp1; const Expr* exp2; Minus (const Expr* e1, const Expr* e2) : exp1(e1), exp2(e2) {} };
struct Times  : Expr { const Expr* exp1; const Expr* exp2; Times (const Expr* e1, const Expr* e2) : exp1(e1), exp2(e2) {} };
struct Divide : Expr { const Expr* exp1; const Expr* exp2; Divide(const Expr* e1, const Expr* e2) : exp1(e1), exp2(e2) {} };
/*
bool operator==(const Expr&, const Expr&) { return false; }
bool operator==(const Value&  a, const Value&  b) { return a.value == b.value; }
bool operator==(const Plus&   a, const Plus&   b) { return a.exp1==b.exp1 && a.exp2==b.exp2; }
bool operator==(const Minus&  a, const Minus&  b) { return a.exp1==b.exp1 && a.exp2==b.exp2; }
bool operator==(const Times&  a, const Times&  b) { return a.exp1==b.exp1 && a.exp2==b.exp2; }
bool operator==(const Divide& a, const Divide& b) { return a.exp1==b.exp1 && a.exp2==b.exp2; }
*/
struct MyExpr
{
    enum Tag {Value,Plus,Minus,Times,Divide} tag;
    union { 
        struct { int value; };
        struct { MyExpr* exp1; MyExpr* exp2; };
    };
};

int myeval(const MyExpr* e)
{
    switch (e->tag)
    {
    case MyExpr::Value:  return e->value;
    case MyExpr::Plus:   return myeval(e->exp1) + myeval(e->exp2);
    case MyExpr::Minus:  return myeval(e->exp1) - myeval(e->exp2);
    case MyExpr::Times:  return myeval(e->exp1) * myeval(e->exp2);
    case MyExpr::Divide: return myeval(e->exp1) / myeval(e->exp2);
    }

    XTL_UNREACHABLE; // To avoid warning that control may reach end of a non-void function
}

struct IExpr           { int m_tag; IExpr(int t) : m_tag(t) {} };
struct IValue  : IExpr { enum { tag = 1 }; int value;                            IValue (int v) : IExpr(tag), value(v) {} };
struct IPlus   : IExpr { enum { tag = 2 }; const IExpr* exp1; const IExpr* exp2; IPlus  (const IExpr* e1, const IExpr* e2) : IExpr(tag), exp1(e1), exp2(e2) {} };
struct IMinus  : IExpr { enum { tag = 3 }; const IExpr* exp1; const IExpr* exp2; IMinus (const IExpr* e1, const IExpr* e2) : IExpr(tag), exp1(e1), exp2(e2) {} };
struct ITimes  : IExpr { enum { tag = 4 }; const IExpr* exp1; const IExpr* exp2; ITimes (const IExpr* e1, const IExpr* e2) : IExpr(tag), exp1(e1), exp2(e2) {} };
struct IDivide : IExpr { enum { tag = 5 }; const IExpr* exp1; const IExpr* exp2; IDivide(const IExpr* e1, const IExpr* e2) : IExpr(tag), exp1(e1), exp2(e2) {} };

int myeval(const IExpr* e)
{
    switch (e->m_tag)
    {
    case IValue::tag:  { const IValue*  p = static_cast<const IValue*> (e); return p->value;                          }
    case IPlus::tag:   { const IPlus*   p = static_cast<const IPlus*>  (e); return myeval(p->exp1) + myeval(p->exp2); }
    case IMinus::tag:  { const IMinus*  p = static_cast<const IMinus*> (e); return myeval(p->exp1) - myeval(p->exp2); }
    case ITimes::tag:  { const ITimes*  p = static_cast<const ITimes*> (e); return myeval(p->exp1) * myeval(p->exp2); }
    case IDivide::tag: { const IDivide* p = static_cast<const IDivide*>(e); return myeval(p->exp1) / myeval(p->exp2); }
    }

    XTL_UNREACHABLE; // To avoid warning that control may reach end of a non-void function
}

struct EExpr           { virtual void raise() const = 0; };
struct EValue  : EExpr { virtual void raise() const { throw(*this); } int value; EValue (int v) : value(v) {} };
struct EPlus   : EExpr { virtual void raise() const { throw(*this); } const EExpr* exp1; const EExpr* exp2; EPlus  (const EExpr* e1, const EExpr* e2) : exp1(e1), exp2(e2) {} };
struct EMinus  : EExpr { virtual void raise() const { throw(*this); } const EExpr* exp1; const EExpr* exp2; EMinus (const EExpr* e1, const EExpr* e2) : exp1(e1), exp2(e2) {} };
struct ETimes  : EExpr { virtual void raise() const { throw(*this); } const EExpr* exp1; const EExpr* exp2; ETimes (const EExpr* e1, const EExpr* e2) : exp1(e1), exp2(e2) {} };
struct EDivide : EExpr { virtual void raise() const { throw(*this); } const EExpr* exp1; const EExpr* exp2; EDivide(const EExpr* e1, const EExpr* e2) : exp1(e1), exp2(e2) {} };

//struct EExpr           { virtual void raise() const = 0; };
//struct EValue  : EExpr { virtual void raise() const { throw(*this); } int value; };
//struct EPlus   : EExpr { virtual void raise() const { throw(*this); } const EExpr* exp1, *exp2; };
//struct EMinus  : EExpr { virtual void raise() const { throw(*this); } const EExpr* exp1, *exp2; };
//struct ETimes  : EExpr { virtual void raise() const { throw(*this); } const EExpr* exp1, *exp2; };
//struct EDivide : EExpr { virtual void raise() const { throw(*this); } const EExpr* exp1, *exp2; };

int myeval(const EExpr* e)
{
    try { e->raise(); }
    catch(const EValue & e) { return e.value; }
    catch(const EPlus  & e) { return myeval(e.exp1) + myeval(e.exp2); }
    catch(const EMinus & e) { return myeval(e.exp1) - myeval(e.exp2); }
    catch(const ETimes & e) { return myeval(e.exp1) * myeval(e.exp2); }
    catch(const EDivide& e) { return myeval(e.exp1) / myeval(e.exp2); }

    XTL_UNREACHABLE; // To avoid warning that control may reach end of a non-void function
}

//class Expr          { virtual ~Expr() {} };
//class Value  : Expr { int value; };
//class Plus   : Expr { Expr& exp1, &exp2; };
//class Minus  : Expr { Expr& exp1, &exp2; };
//class Times  : Expr { Expr& exp1, &exp2; };
//class Divide : Expr { Expr& exp1, &exp2; };

// Forward declare cases
struct VValue;
struct VPlus;
struct VMinus; 
struct VTimes;
struct VDivide;

// Cases interface
struct Visitor
{
    virtual void visitValue (const VValue& ) {}
    virtual void visitPlus  (const VPlus&  ) {}
    virtual void visitMinus (const VMinus& ) {}
    virtual void visitTimes (const VTimes& ) {}
    virtual void visitDivide(const VDivide&) {}
};

struct VExpr           { virtual void accept(Visitor& v) const  = 0; };
struct VValue  : VExpr { virtual void accept(Visitor& v) const { v.visitValue (*this); } int value;                            VValue (int v) : value(v) {} };
struct VPlus   : VExpr { virtual void accept(Visitor& v) const { v.visitPlus  (*this); } const VExpr* exp1; const VExpr* exp2; VPlus  (const VExpr* e1, const VExpr* e2) : exp1(e1), exp2(e2) {} };
struct VMinus  : VExpr { virtual void accept(Visitor& v) const { v.visitMinus (*this); } const VExpr* exp1; const VExpr* exp2; VMinus (const VExpr* e1, const VExpr* e2) : exp1(e1), exp2(e2) {} };
struct VTimes  : VExpr { virtual void accept(Visitor& v) const { v.visitTimes (*this); } const VExpr* exp1; const VExpr* exp2; VTimes (const VExpr* e1, const VExpr* e2) : exp1(e1), exp2(e2) {} };
struct VDivide : VExpr { virtual void accept(Visitor& v) const { v.visitDivide(*this); } const VExpr* exp1; const VExpr* exp2; VDivide(const VExpr* e1, const VExpr* e2) : exp1(e1), exp2(e2) {} };

int myeval(const VExpr& e)
{
    struct EvalVisitor : Visitor
    {
    int result;
    void visitValue (const VValue&  e) { result = e.value; }
    void visitPlus  (const VPlus&   e) { result = myeval(*e.exp1) + myeval(*e.exp2); }
    void visitMinus (const VMinus&  e) { result = myeval(*e.exp1) - myeval(*e.exp2); }
    void visitTimes (const VTimes&  e) { result = myeval(*e.exp1) * myeval(*e.exp2); }
    void visitDivide(const VDivide& e) { result = myeval(*e.exp1) / myeval(*e.exp2); }
    };

    EvalVisitor v;
    e.accept(v);
    return v.result;
}

// Root of the expression hierarchy
//struct Expr 
//{
//    virtual ~Expr() {} 
//};
//
//struct Value : Expr
//{
//    Value(int v) : value(v) {}
//    int value;
//};
//
//struct Plus : Expr
//{
//    Plus(const Expr* e1, const Expr* e2) : exp1(e1), exp2(e2) {}
//    const Expr* exp1;
//    const Expr* exp2;
//};
//
//struct Minus : Expr
//{
//    Minus(const Expr* e1, const Expr* e2) : exp1(e1), exp2(e2) {}
//    const Expr* exp1;
//    const Expr* exp2;
//};
//
//struct Times : Expr
//{
//    Times(const Expr* e1, const Expr* e2) : exp1(e1), exp2(e2) {}
//    const Expr* exp1;
//    const Expr* exp2;
//};
//
//struct Divide : Expr
//{
//    Divide(const Expr* e1, const Expr* e2) : exp1(e1), exp2(e2) {}
//    const Expr* exp1;
//    const Expr* exp2;
//};

//------------------------------------------------------------------------------

namespace mch ///< Mach7 library namespace
{
template <> struct bindings<Value>  { Members(Value::value); };
template <> struct bindings<Plus>   { Members(Plus::exp1   , Plus::exp2);   };
template <> struct bindings<Minus>  { Members(Minus::exp1  , Minus::exp2);  };
template <> struct bindings<Times>  { Members(Times::exp1  , Times::exp2);  };
template <> struct bindings<Divide> { Members(Divide::exp1 , Divide::exp2); };
} // of namespace mch

//------------------------------------------------------------------------------

using namespace mch;

//------------------------------------------------------------------------------
void str(const char* s)
{
    var<int> n,m,y,d;
    auto month = m |= m > 0 && m < 13; // Save pattern to variable
    auto day   = d |= d > 0 && d < 31; // Day pattern
    Match(s)
    {
        Case(rex("([0-9]+)-([0-9]+)-([0-9]+)", 979)) break;  // Local phone
#if XTL_SUPPORT(initializer_list)
        Case(rex("([0-9]+)-([0-9]+)-([0-9]+)", any({800,888,877,866,855}), n, m)) break; // Toll-free phone
#endif
        Case(rex("([0-9]{4})-([0-9]{2})-([0-9]{2})", y, month, day)) break;
        Otherwise() break; // Something else
    }
    EndMatch
}

int myeval(const Expr* e)
{
    var<int> n; var<const Expr*> a,b;

    Match(e)
    {
    Case(C<Value> (n))   return n;
    Case(C<Plus>  (a,b)) return myeval(a) + myeval(b);
    Case(C<Minus> (a,b)) return myeval(a) - myeval(b);
    Case(C<Times> (a,b)) return myeval(a) * myeval(b);
    Case(C<Divide>(a,b)) return myeval(a) / myeval(b);
    }
    EndMatch

    XTL_UNREACHABLE; // To avoid warning that control may reach end of a non-void function
}

//------------------------------------------------------------------------------

int dyn_eval(const Expr* e)
{
    if (const Value*  p = dynamic_cast<const Value* >(e)) return p->value;
    if (const Plus*   p = dynamic_cast<const Plus*  >(e)) return dyn_eval(p->exp1) + dyn_eval(p->exp2);
    if (const Minus*  p = dynamic_cast<const Minus* >(e)) return dyn_eval(p->exp1) - dyn_eval(p->exp2);
    if (const Times*  p = dynamic_cast<const Times* >(e)) return dyn_eval(p->exp1) * dyn_eval(p->exp2);
    if (const Divide* p = dynamic_cast<const Divide*>(e)) return dyn_eval(p->exp1) / dyn_eval(p->exp2);

    XTL_UNREACHABLE; // To avoid warning that control may reach end of a non-void function
}

//------------------------------------------------------------------------------

const Expr* factorize(const Expr* e)
{
    var<const Expr*> x, a, b;

    Match(e)
    Case(C<Plus>(C<Times>(x,a), C<Times>(+x,b)))
        return new Times(x, new Plus(a,b));
    Case(C<Plus>(C<Times>(a,x), C<Times>(b,+x)))
        return new Times(new Plus(a,b), x);
    EndMatch

    return e;
}

//------------------------------------------------------------------------------

bool operator==(const Expr& e1, const Expr& e2)
{
    var<int> n; var<const Expr&> a, b;

    Match(e1               , e2                 )
     Case(C<Plus>  (&a, &b), C<Plus>  (&+a, &+b)) return true;
     Case(C<Minus> (&a, &b), C<Minus> (&+a, &+b)) return true;
     Case(C<Times> (&a, &b), C<Times> (&+a, &+b)) return true;
     Case(C<Divide>(&a, &b), C<Divide>(&+a, &+b)) return true;
     Case(C<Value> (n),      C<Value> (+n)      ) return true;
    EndMatch

    return false;
}

//------------------------------------------------------------------------------

int main()
{
    Expr* a = new Value(17);
    Expr* b = new Value(25);
    Expr* c = new Plus(a,b);
    std::cout << myeval(c)   << std::endl;
    //std::cout << to_str(c) << std::endl;

    Expr* f = new Times(a,b);
    Expr* g = new Plus(f,f);
    const Expr* h = factorize(g);
    std::cout << myeval(g) << std::endl;
    std::cout << myeval(h) << std::endl;
    std::cout << (*f == *f) << std::endl;
    std::cout << (*f == *g) << std::endl;
    EExpr* sum = new EPlus(new EValue(10), new EValue(12));
    std::cout << myeval(sum) << std::endl;
    VExpr* vsum = new VPlus(new VValue(10), new VValue(12));
    std::cout << myeval(*vsum) << std::endl;
}
