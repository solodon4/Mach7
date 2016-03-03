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

#include <iostream>

// Enable declarations in case clauses, which are disabled by default
#define XTL_CLAUSE_DECL 1

#include <mach7/match.hpp>                 // Support for Match statement

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
    Plus(const Expr& e1, const Expr& e2) : e1(e1), e2(e2) {}
    const Expr& e1;
    const Expr& e2;
};

struct Minus : Expr
{
    Minus(const Expr& e1, const Expr& e2) : e1(e1), e2(e2) {}
    const Expr& e1;
    const Expr& e2;
};

struct Times : Expr
{
    Times(const Expr& e1, const Expr& e2) : e1(e1), e2(e2) {}
    const Expr& e1;
    const Expr& e2;
};

struct Divide : Expr
{
    Divide(const Expr& e1, const Expr& e2) : e1(e1), e2(e2) {}
    const Expr& e1;
    const Expr& e2;
};

//------------------------------------------------------------------------------

int eval(const Expr& e)
{
    Match(e)
    {
        Case(const Value&  x) return x.value;
        Case(const Plus&   x) return eval(x.e1) + eval(x.e2);
        Case(const Minus&  x) return eval(x.e1) - eval(x.e2);
        Case(const Times&  y) return eval(y.e1) * eval(y.e2);
        Case(const Divide& z) return eval(z.e1) / eval(z.e2);
    }
    EndMatch

    XTL_UNREACHABLE; // To avoid warning that control may reach end of a non-void function
}

//------------------------------------------------------------------------------

int main()
{
    Value a = 17;
    Value b = 25;
    Plus  c = Plus(a,b);

    std::cout << eval(a) << std::endl;
    std::cout << eval(b) << std::endl;
    std::cout << eval(c) << std::endl;
}

//------------------------------------------------------------------------------
