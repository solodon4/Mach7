//
//  Mach7: Pattern Matching Library for C++
//
//  Copyright 2011-2013, Texas A&M University.
//  Copyright 2014-2015, Yuriy Solodkyy.
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

#include <iostream>
#include <mach7/type_switchN-patterns.hpp> // Support for N-ary Match statement on patterns
#include <mach7/patterns/address.hpp>      // Address and dereference combinators
#include <mach7/patterns/bindings.hpp>     // Mach7 support for bindings on arbitrary UDT
#include <mach7/patterns/constructor.hpp>  // Support for constructor patterns
#include <mach7/patterns/primitive.hpp>    // Wildcard, variable and value patterns
#include <xtl/adapters/std/memory.hpp>     // XTL subtyping adapters for standard smart pointers

using namespace mch;

//------------------------------------------------------------------------------

struct Expr 
{ 
    virtual ~Expr() {}
};

typedef std::unique_ptr<Expr> ExprPtr;

struct Value : Expr
{
    Value(int v) : value(v) {}
    int value;
};

struct Plus : Expr
{
    Plus(ExprPtr e1, ExprPtr e2) : e1(std::move(e1)), e2(std::move(e2)) {}
    ExprPtr e1;
    ExprPtr e2;
};

struct Minus : Expr
{
    Minus(ExprPtr e1, ExprPtr e2) : e1(std::move(e1)), e2(std::move(e2)) {}
    ExprPtr e1;
    ExprPtr e2;
};

struct Times : Expr
{
    Times(ExprPtr e1, ExprPtr e2) : e1(std::move(e1)), e2(std::move(e2)) {}
    ExprPtr e1;
    ExprPtr e2;
};

struct Divide : Expr
{
    Divide(ExprPtr e1, ExprPtr e2) : e1(std::move(e1)), e2(std::move(e2)) {}
    ExprPtr e1;
    ExprPtr e2;
};

//------------------------------------------------------------------------------

namespace mch ///< Mach7 library namespace
{
template <> struct bindings<Value> { Members(Value::value); };
template <> struct bindings<Plus>  { Members(Plus::e1  , Plus::e2);   };
template <> struct bindings<Minus> { Members(Minus::e1 , Minus::e2);  };
template <> struct bindings<Times> { Members(Times::e1 , Times::e2);  };
template <> struct bindings<Divide>{ Members(Divide::e1, Divide::e2); };
} // of namespace mch

//------------------------------------------------------------------------------

#if 1
int evl(const Expr& e)
{
    var<const Expr&> e1, e2;
    var<int> n;

    Match(e)
    {
        Case(C<Value> (n)      ) return n;
        Case(C<Plus>  (&e1,&e2)) return evl(e1) + evl(e2);
        Case(C<Minus> (&e1,&e2)) return evl(e1) - evl(e2);
        Case(C<Times> (&e1,&e2)) return evl(e1) * evl(e2);
        Case(C<Divide>(&e1,&e2)) return evl(e1) / evl(e2);
    }
    EndMatch

    XTL_UNREACHABLE; // To avoid warning that control may reach end of a non-void function
}
#else
int evl(const Expr& e)
{
    var<ExprPtr> e1, e2;
    var<int> n;

    Match(e)
    {
        Case(C<Value> (n)    ) return n;
        Case(C<Plus>  (e1,e2)) return evl(*e1) + evl(*e2);
        Case(C<Minus> (e1,e2)) return evl(*e1) - evl(*e2);
        Case(C<Times> (e1,e2)) return evl(*e1) * evl(*e2);
        Case(C<Divide>(e1,e2)) return evl(*e1) / evl(*e2);
    }
    EndMatch

    XTL_UNREACHABLE; // To avoid warning that control may reach end of a non-void function
}
#endif

//------------------------------------------------------------------------------

int main()
{
    ExprPtr a(new Value(17));
    ExprPtr b(new Value(25));

    std::cout << evl(*a) << std::endl;
    std::cout << evl(*b) << std::endl;

    ExprPtr c(new Plus(std::move(a),std::move(b)));

    std::cout << evl(*c) << std::endl;
}

//------------------------------------------------------------------------------
