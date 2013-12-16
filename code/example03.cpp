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

// Enable declarations in case clauses, which are disabled by default
#define XTL_CLAUSE_DECL 1

#include "match.hpp"                // Support for Match statement

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
