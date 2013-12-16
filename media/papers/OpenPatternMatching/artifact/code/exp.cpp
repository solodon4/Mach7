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
#include "patterns/constructor.hpp" // Support for constructor patterns

#include <iostream>
#include <string>

struct Expr;
struct Value;
struct Plus;
struct Minus;
struct Times;
struct Divide;

// Original visitation interface with default
struct ExprVisitor
{
    virtual void visit(const Expr&) { throw 5; } // Default handling of future classes
    virtual void visit(const Value&)  = 0;
    virtual void visit(const Plus&)   = 0;
    virtual void visit(const Minus&)  = 0;
    virtual void visit(const Times&)  = 0;
    virtual void visit(const Divide&) = 0;
};

// Root of the expression hierarchy
struct Expr 
{ 
    virtual void accept(ExprVisitor&) const = 0;
};

struct Value : Expr
{
    Value(int v) : value(v) {}
    void accept(ExprVisitor& v) const { v.visit(*this); }
    int value;
};

struct Plus : Expr
{
    Plus(const Expr* e1, const Expr* e2) : exp1(e1), exp2(e2) {}
    void accept(ExprVisitor& v) const { v.visit(*this); }
    const Expr* exp1;
    const Expr* exp2;
};

struct Minus : Expr
{
    Minus(const Expr* e1, const Expr* e2) : exp1(e1), exp2(e2) {}
    void accept(ExprVisitor& v) const { v.visit(*this); }
    const Expr* exp1;
    const Expr* exp2;
};

struct Times : Expr
{
    Times(const Expr* e1, const Expr* e2) : exp1(e1), exp2(e2) {}
    void accept(ExprVisitor& v) const { v.visit(*this); }
    const Expr* exp1;
    const Expr* exp2;
};

struct Divide : Expr
{
    Divide(const Expr* e1, const Expr* e2) : exp1(e1), exp2(e2) {}
    void accept(ExprVisitor& v) const { v.visit(*this); }
    const Expr* exp1;
    const Expr* exp2;
};

//------------------------------------------------------------------------------
#if 1
std::string to_str(const Expr* e);

struct ToStrVisitor : virtual ExprVisitor
{
    // This function is needed to allow derived visitors to override handling of subvisitors
    virtual std::string evaluate(const Expr* e) const { return to_str(e); }

    using ExprVisitor::visit; // Bring in those we do not override for overload resolution

    void visit(const Value&   ) { result = "X"/*std::to_string(e.value)*/; }
    void visit(const Plus&   e) { result = '(' + evaluate(e.exp1) + '+' + evaluate(e.exp2) + ')'; }
    void visit(const Minus&  e) { result = '(' + evaluate(e.exp1) + '-' + evaluate(e.exp2) + ')'; }
    void visit(const Times&  e) { result = '(' + evaluate(e.exp1) + '*' + evaluate(e.exp2) + ')'; }
    void visit(const Divide& e) { result = '(' + evaluate(e.exp1) + '/' + evaluate(e.exp2) + ')'; }

    std::string result;
};

std::string to_str(const Expr* e)
{
    ToStrVisitor v;
    e->accept(v);
    return v.result;
}

#else

std::string to_str(const Expr* e)
{
    struct ToStrVisitor : virtual ExprVisitor
    {
        // This function is needed to allow derived visitors to override handling of subvisitors
        virtual std::string evaluate(const Expr* e) const { return to_str(e); }
        
        using ExprVisitor::visit; // Bring in those we do not override for overload resolution
        
        void visit(const Value&  e) { result = "X"/*std::to_string(e.value)*/; }
        void visit(const Plus&   e) { result = '(' + evaluate(e.exp1) + '+' + evaluate(e.exp2) + ')'; }
        void visit(const Minus&  e) { result = '(' + evaluate(e.exp1) + '-' + evaluate(e.exp2) + ')'; }
        void visit(const Times&  e) { result = '(' + evaluate(e.exp1) + '*' + evaluate(e.exp2) + ')'; }
        void visit(const Divide& e) { result = '(' + evaluate(e.exp1) + '/' + evaluate(e.exp2) + ')'; }
        
        std::string result;
    } v;
    e->accept(v);
    return v.result;
}
#endif

//------------------------------------------------------------------------------

namespace mch ///< Mach7 library namespace
{
template <> struct bindings<Value>  { CM(0,Value::value); };
template <> struct bindings<Plus>   { CM(0,Plus::exp1);   CM(1,Plus::exp2);   };
template <> struct bindings<Minus>  { CM(0,Minus::exp1);  CM(1,Minus::exp2);  };
template <> struct bindings<Times>  { CM(0,Times::exp1);  CM(1,Times::exp2);  };
template <> struct bindings<Divide> { CM(0,Divide::exp1); CM(1,Divide::exp2); };
} // of namespace mch

int myeval(const Expr* e)
{
    Match(e)
    {
    Case(Value, n)   return n;
    Case(Plus,  a,b) return myeval(a) + myeval(b);
    Case(Minus, a,b) return myeval(a) - myeval(b);
    Case(Times, a,b) return myeval(a) * myeval(b);
    Case(Divide,a,b) return myeval(a) / myeval(b);
    }
    EndMatch
}

const Expr* factorize(const Expr* e)
{
    const Expr *e1, *e2, *e3, *e4;

    if (mch::C<Plus>(
            mch::C<Times>(e1,e2),
            mch::C<Times>(e3,e4)
        )(e))
    {
        if (e1 == e3)
            return new Times(e1, new Plus(e2,e4));
        if (e2 == e4)
            return new Times(new Plus(e1,e3), e4);
    }

    return e;
}
/*
const Expr* factorize1(const Expr* e)
{
    var<const Expr*> e1, e2, e3, e4;
    if (mch::C<Plus>(
            mch::C<Times>(e1,e2), 
            mch::C<Times>(e3 |= e1 == e3,e4)
        )(e)) return new Times(e1, new Plus(e2,e4));
    else
    if (mch::C<Plus>(
            mch::C<Times>(e1,e2), 
            mch::C<Times>(e3,e4 |= e2 == e4)
        )(e)) return new Times(new Plus(e1,e3), e4);
    else
    return e;
}
*/
//------------------------------------------------------------------------------
// We now extend the class hierarchy with 2 additional classes Mod and Pow, not
// known to the original visitation interface.

struct ExtendedExpr1;
struct Mod;
struct Pow;

struct ExtendedExpr1Visitor : virtual ExprVisitor
{
    virtual void visit(const Expr& e);
    virtual void visit(const ExtendedExpr1&) { throw 6; } 
    virtual void visit(const Mod&)   = 0;
    virtual void visit(const Pow&)   = 0;
};

struct ExtendedExpr1 : Expr
{
            void accept(        ExprVisitor&  v) const { v.visit(*this); }
    virtual void accept(ExtendedExpr1Visitor& v) const { v.visit(*this); }
};

void ExtendedExpr1Visitor::visit(const Expr& e)
{
    // We really want to throw exception when it is not of that type as we 
    // cannot forward to base as it will arrive here again. When expression 
    // is not of that type, it means it is from a module that extended the 
    // hierarchy orthogonally to us, so we don't know anything about that class.
    dynamic_cast<const ExtendedExpr1&>(e).accept(*this);
}

struct Mod : ExtendedExpr1
{
    Mod(const Expr* e1, const Expr* e2) : exp1(e1), exp2(e2) {}
    void accept(ExtendedExpr1Visitor& v) const { v.visit(*this); }
    const Expr* exp1;
    const Expr* exp2;
};

struct Pow : ExtendedExpr1
{
    Pow(const Expr* e1, const Expr* e2) : exp1(e1), exp2(e2) {}
    void accept(ExtendedExpr1Visitor& v) const { v.visit(*this); }
    const Expr* exp1;
    const Expr* exp2;
};

std::string to_str_ex1(const Expr* e);

struct Extended1ToStrVisitor : ExtendedExpr1Visitor, ToStrVisitor
{
    // This function is needed to allow derived visitors to override handling of subvisitors
    virtual std::string evaluate(const Expr* e) const { return to_str_ex1(e); }

    using ToStrVisitor::visit; // Bring in those we do not override for overload resolution

    void visit(const ExtendedExpr1& e) { visit(static_cast<const Expr&>(e)); }
    void visit(const Mod& e)           { result = '(' + evaluate(e.exp1) + '%' + evaluate(e.exp2) + ')'; }
    void visit(const Pow& e)           { result = '(' + evaluate(e.exp1) + '^' + evaluate(e.exp2) + ')'; }
};

std::string to_str_ex1(const Expr* e)
{
    Extended1ToStrVisitor v;
    e->accept(v);
    return v.result;
}

//------------------------------------------------------------------------------
// We now independently from previous extension extend the class hierarchy with 
// another 2 classes Min and Max, not known to neither the original nor first 
// extended visitation interface.

struct ExtendedExpr2;
struct Min;
struct Max;

struct ExtendedExpr2Visitor : virtual ExprVisitor
{
    virtual void visit(const Expr& e);
    virtual void visit(const ExtendedExpr2&) { throw 7; } 
    virtual void visit(const Min&)   = 0;
    virtual void visit(const Max&)   = 0;
};

struct ExtendedExpr2 : Expr
{
            void accept(        ExprVisitor&  v) const { v.visit(*this); }
    virtual void accept(ExtendedExpr2Visitor& v) const { v.visit(*this); }
};

void ExtendedExpr2Visitor::visit(const Expr& e)
{
    // We really want to throw exception when it is not of that type as we 
    // cannot forward to base as it will arrive here again. When expression 
    // is not of that type, it means it is from a module that extended the 
    // hierarchy orthogonally to us, so we don't know anything about that class.
    dynamic_cast<const ExtendedExpr2&>(e).accept(*this);
}

struct Min : ExtendedExpr2
{
    Min(const Expr* e1, const Expr* e2) : exp1(e1), exp2(e2) {}
    void accept(ExtendedExpr2Visitor& v) const { v.visit(*this); }
    const Expr* exp1;
    const Expr* exp2;
};

struct Max : ExtendedExpr2
{
    Max(const Expr* e1, const Expr* e2) : exp1(e1), exp2(e2) {}
    void accept(ExtendedExpr2Visitor& v) const { v.visit(*this); }
    const Expr* exp1;
    const Expr* exp2;
};

std::string to_str_ex2(const Expr* e);

struct Extended2ToStrVisitor : ExtendedExpr2Visitor, ToStrVisitor
{
    // This function is needed to allow derived visitors to override handling of subvisitors
    virtual std::string evaluate(const Expr* e) const { return to_str_ex2(e); }

    using ToStrVisitor::visit; // Bring in those we do not override for overload resolution

    void visit(const ExtendedExpr2& e) { visit(static_cast<const Expr&>(e)); }
    void visit(const Min& e)           { result = "min(" + evaluate(e.exp1) + ',' + evaluate(e.exp2) + ')'; }
    void visit(const Max& e)           { result = "max(" + evaluate(e.exp1) + ',' + evaluate(e.exp2) + ')'; }
};

std::string to_str_ex2(const Expr* e)
{
    Extended2ToStrVisitor v;
    e->accept(v);
    return v.result;
}

//------------------------------------------------------------------------------

int main()
{
    Expr* a = new Value(17);
    Expr* b = new Value(25);
    Expr* c = new Plus(a,b);
    std::cout << myeval(c)   << std::endl;
    std::cout << to_str(c) << std::endl;

    // Mix of original operations with first extension
    Expr* d1 = new Mod(b,c);
    std::cout << myeval(d1)       << std::endl;
    std::cout << to_str_ex1(d1) << std::endl;
    Expr* e1 = new Minus(c,d1);
    std::cout << myeval(e1)       << std::endl;
    std::cout << to_str_ex1(e1) << std::endl;

    // Mix of original operations with second extension
    Expr* d2 = new Min(b,c);
    std::cout << myeval(d2)       << std::endl;
    std::cout << to_str_ex2(d2) << std::endl;
    Expr* e2 = new Minus(c,d2);
    std::cout << myeval(e2)       << std::endl;
    std::cout << to_str_ex2(e2) << std::endl;

    // Mix of operations from first and second extension
    Expr* e = new Plus(d1,d2);
    std::cout << myeval(e)       << std::endl;
    //std::cout << to_str_ex2(e) << std::endl; // WRONG!

    Expr* f = new Times(a,b);
    Expr* g = new Plus(f,f);
    const Expr* h = factorize(g);
    std::cout << to_str_ex2(g) << std::endl;
    std::cout << to_str_ex2(h) << std::endl;
}
