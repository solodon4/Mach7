///
/// \file lambda.cpp
///
/// An example of implementing Lambda Calculus interpreter in Mach7.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of Mach7 library (http://parasol.tamu.edu/mach7/).
/// Copyright (C) 2011-2012 Texas A&M University.
/// All rights reserved.
///

//------------------------------------------------------------------------------

#include <mach7/type_switchN-patterns.hpp> // Support for N-ary Match statement on patterns
#include <mach7/patterns/address.hpp>      // Address and dereference combinators
#include <mach7/patterns/bindings.hpp>     // Mach7 support for bindings on arbitrary UDT
#include <mach7/patterns/constructor.hpp>  // Support for constructor patterns
#include <mach7/patterns/equivalence.hpp>  // Equivalence combinator +
#include <mach7/patterns/primitive.hpp>    // Wildcard, variable and value patterns
#include "testutils.hpp"

//------------------------------------------------------------------------------

struct Term { virtual ~Term() {} };
struct Var : Term { std::string name;       Var(const char* n) : name(n) {} };
struct Abs : Term { Var*  var;  Term* body; Abs(Var*  v, Term* t) : var(v), body(t) {} };
struct App : Term { Term* func; Term* arg;  App(Term* f, Term* a) : func(f), arg(a) {} };

//------------------------------------------------------------------------------

namespace mch ///< Mach7 library namespace
{
template <> struct bindings<Var> { CM(0,Var::name); };
template <> struct bindings<Abs> { CM(0,Abs::var);  CM(1,Abs::body); };
template <> struct bindings<App> { CM(0,App::func); CM(1,App::arg);  };
} // of namespace mch

//------------------------------------------------------------------------------

using namespace mch; // Enable use of pattern-matching constructs without namespace qualification

//------------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& os, const Term& t)
{
    std::string      s;
    var<const Var&>  v;
    var<const Term&> t1,t2;

    Match(t)
    {
    Case(C<Var>(s))       return os << s;
    Case(C<Abs>(&v,&t1))  return os << '\\' << v << '.' << t1;
    Case(C<App>(&t1,&t2)) return os << '(' << t1 << ')' << '(' << t2 << ')';
    }
    EndMatch

    return os; // To prevent all control path warning
}

//------------------------------------------------------------------------------

/// Substitutes every occurence of variable #v in #s with #t.
Term* substitute(const Term& s, const Var& v, const Term& t) { return nullptr; } 

//------------------------------------------------------------------------------

bool operator==(const Term&, const Term&);
inline bool operator!=(const Term& left, const Term& right) { return !(left == right); }

//------------------------------------------------------------------------------

bool operator==(const Term& left, const Term& right)
{
    //std::clog << "(" << left << ',' << right << ')' << std::endl;
    var<std::string> s;
    //var<const Var&>  v;
    var<const Term&> v,t,f;

    Match(left,right)
    {
    Case(C<Var>(s),     C<Var>(+s)     ) return true;
    Case(C<Abs>(&v,&t), C<Abs>(&+v,&+t)) return true;
    Case(C<App>(&f,&t), C<App>(&+f,&+t)) return true;
    Otherwise()                          return false;
    }
    EndMatch

    return false; // To prevent all control path warning
}

//------------------------------------------------------------------------------

Term* random_term(int n)
{
    static Var* variables[] = {new Var("a"), new Var("b"), new Var("c"), new Var("d"), new Var("e"), new Var("f")};
    const int N = XTL_ARR_SIZE(variables);
    Var* v = variables[rand()%N];

    if (n < 3)
    {
        switch (n)
        {
        case 0: return v;
        case 1: return new Abs(v,v);
        case 2: return new App(new Abs(v,v),variables[rand()%N]);
        }
    }
    else
    {
        switch (n % 3)
        {
        case 0: return v;
        case 1: return new Abs(v,random_term(n/3));
        case 2: return new App(random_term(n/3),random_term(n/3));
        }
    }
}

//------------------------------------------------------------------------------

inline size_t compare_terms2(Term* left, Term* right) { return *left == *right; }

//------------------------------------------------------------------------------
 
int main()
{
    std::vector<Term*> arguments(N);

    for (size_t i = 0; i < N; ++i)
    {
        arguments[i] = random_term(rand()%1000);
        //std::cout << *arguments[i] << std::endl;
    }
    /*
    for (size_t i = 0; i < N; ++i)
    {
        for (size_t j = 0; j < N; ++j)
            std::cout << compare_terms1(arguments[i],arguments[j]);

        std::cout << "\t" << *arguments[i] << std::endl;
    }
    return 0;
    */
    verdict v = get_timings2<size_t,Term*,compare_terms2,compare_terms2>(arguments);
    std::cout << "Verdict: \t" << v << std::endl;
}

//------------------------------------------------------------------------------
