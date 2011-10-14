#include <iostream>
#include "match_generic.hpp"

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
    Plus(const Expr* e1, const Expr* e2) : exp1(e1), exp2(e2) {}
    const Expr* exp1;
    const Expr* exp2;
};

struct Minus : Expr
{
    Minus(const Expr* e1, const Expr* e2) : exp1(e1), exp2(e2) {}
    const Expr* exp1;
    const Expr* exp2;
};

struct Mult : Expr
{
    Mult(const Expr* e1, const Expr* e2) : exp1(e1), exp2(e2) {}
    const Expr* exp1;
    const Expr* exp2;
};

struct Div : Expr
{
    Div(const Expr* e1, const Expr* e2) : exp1(e1), exp2(e2) {}
    const Expr* exp1;
    const Expr* exp2;
};

template <> struct match_members<Value>  { CM(0,Value::value); };
template <> struct match_members<Plus>   { CM(0,Plus::exp1);  CM(1,Plus::exp2);  };
template <> struct match_members<Minus>  { CM(0,Minus::exp1); CM(1,Minus::exp2); };
template <> struct match_members<Mult>   { CM(0,Mult::exp1);  CM(1,Mult::exp2);  };
template <> struct match_members<Div>    { CM(0,Div::exp1);   CM(1,Div::exp2);   };

int eval(const Expr* e)
{
    Match(e)
    {
    Case(Value,n)   return n;
    Case(Plus, a,b) return eval(a) + eval(b);
    Case(Minus,a,b) return eval(a) - eval(b);
    Case(Mult, a,b) return eval(a) * eval(b);
    Case(Div,  a,b) return eval(a) / eval(b);
    }
    EndMatch
}

int main()
{
    Expr* a = new Value(17);
    Expr* b = new Value(25);
    Expr* c = new Plus(a,b);
    std::cout << eval(c) << std::endl;
}