///
/// \file
///
/// This file is a part of pattern matching testing suite.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of Mach7 library (http://parasol.tamu.edu/mach7/).
/// Copyright (C) 2011-2013 Texas A&M University.
/// All rights reserved.
///

#include <iostream>
#include <mach7/type_switchN-patterns.hpp> // Support for N-ary Match statement on patterns
#include <mach7/patterns/address.hpp>      // Address and dereference combinators
#include <mach7/patterns/bindings.hpp>     // Mach7 support for bindings on arbitrary UDT
#include <mach7/patterns/constructor.hpp>  // Support for constructor patterns

//------------------------------------------------------------------------------

struct Expr 
{ 
    virtual ~Expr() {}
};

struct Value : Expr
{
    Value(int v) : value(v) {}
    int value;
};

struct Plus : Expr
{
    Plus(const Expr* e1, const Expr* e2) : e1(e1), e2(e2) {}
    const Expr* e1;
    const Expr* e2;
};

struct Minus : Expr
{
    Minus(const Expr* e1, const Expr* e2) : e1(e1), e2(e2) {}
    const Expr* e1;
    const Expr* e2;
};

struct Times : Expr
{
    Times(const Expr* e1, const Expr* e2) : e1(e1), e2(e2) {}
    const Expr* e1;
    const Expr* e2;
};

struct Divide : Expr
{
    Divide(const Expr* e1, const Expr* e2) : e1(e1), e2(e2) {}
    const Expr* e1;
    const Expr* e2;
};

//------------------------------------------------------------------------------

namespace mch ///< Mach7 library namespace
{
template <> struct bindings<Value> { Members(Value::value); };
template <> struct bindings<Plus>  { Members(Plus::e1   , Plus::e2);   };
template <> struct bindings<Minus> { Members(Minus::e1  , Minus::e2);  };
template <> struct bindings<Times> { Members(Times::e1  , Times::e2);  };
template <> struct bindings<Divide>{ Members(Divide::e1 , Divide::e2); };
} // of namespace mch

//------------------------------------------------------------------------------

#if 1
int evl(const Expr& e)
{
    mch::var<const Expr&> e1, e2;
    mch::var<int> n;

    Match(e)
    {
        Case(mch::C<Value> (n)      ) return n;
        Case(mch::C<Plus>  (&e1,&e2)) return evl(e1) + evl(e2);
        Case(mch::C<Minus> (&e1,&e2)) return evl(e1) - evl(e2);
        Case(mch::C<Times> (&e1,&e2)) return evl(e1) * evl(e2);
        Case(mch::C<Divide>(&e1,&e2)) return evl(e1) / evl(e2);
    }
    EndMatch
}
#else
int evl(const Expr& e)
{
    mch::var<const Expr*> e1, e2;
    mch::var<int> n;

    Match(e)
    {
        Case(mch::C<Value> (n)    ) return n;
        Case(mch::C<Plus>  (e1,e2)) return evl(*e1) + evl(*e2);
        Case(mch::C<Minus> (e1,e2)) return evl(*e1) - evl(*e2);
        Case(mch::C<Times> (e1,e2)) return evl(*e1) * evl(*e2);
        Case(mch::C<Divide>(e1,e2)) return evl(*e1) / evl(*e2);
    }
    EndMatch
}
#endif

//------------------------------------------------------------------------------

int main()
{
    Value a = 17;
    Value b = 25;
    Plus  c = Plus(&a,&b);

    std::cout << evl(a) << std::endl;
    std::cout << evl(b) << std::endl;
    std::cout << evl(c) << std::endl;
}

//------------------------------------------------------------------------------
