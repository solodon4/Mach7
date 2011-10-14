#include <iostream>
#include "match_generic.hpp"

struct Expr 
{ 
    virtual ~Expr() {} 
};

struct Con : Expr
{
    Con(int v) : value(v) {}
    int value;
};

struct Plus : Expr
{
    Plus(const Expr* e1, const Expr* e2) : exp1(e1), exp2(e2) {}
    const Expr* exp1;
    const Expr* exp2;
};

template <> struct match_members<Con>  { CM(0,Con::value); };
template <> struct match_members<Plus> { CM(0,Plus::exp1); CM(1,Plus::exp2); };

int eval(const Expr* e)
{
    Match(e)
    {
    Case(Con,n)    return n;
    Case(Plus,a,b) return eval(a)+eval(b);
    }
    EndMatch
}

int main()
{
    Expr* a = new Con(17);
    Expr* b = new Con(25);
    Expr* c = new Plus(a,b);
    std::cout << eval(c) << std::endl;
}