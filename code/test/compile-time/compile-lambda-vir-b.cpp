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

#include "virpat.hpp"
#include "testutils.hpp"

//------------------------------------------------------------------------------

struct Term : object 
{
    virtual ~Term() {} 
    virtual bool operator==(const object& obj) const 
    {
        return typeid(*this) == typeid(obj); 
    }
    virtual std::ostream& operator>>(std::ostream& os) const 
    {
        return os << "Term()"; 
    }
};

//------------------------------------------------------------------------------

struct Var : Term
{
    object_of<std::string> name;
    Var(const char* n) : name(n) {} 
    virtual bool operator==(const object& obj) const 
    {
        const Var* p = unbox<Var>(obj); 
        return p && name == p->name; 
    }
    virtual std::ostream& operator>>(std::ostream& os) const 
    {
        return os << "Var(" << name << ')'; 
    }
};

//------------------------------------------------------------------------------

struct Abs : Term
{
    Var*  var;
    Term* body;
    Abs(Var*  v, Term* t) : var(v), body(t) {}
    virtual bool operator==(const object& obj) const 
    {
        const Abs* p = unbox<Abs>(obj); 
        return p && *var == *p->var && *body == *p->body; 
    }
    virtual std::ostream& operator>>(std::ostream& os) const 
    {
        return os << "Abs(" << *var << ',' << *body << ')'; 
    }
};

//------------------------------------------------------------------------------

struct App : Term
{
    Term* func;
    Term* arg;
    App(Term* f, Term* a) : func(f), arg(a) {}
    virtual bool operator==(const object& obj) const 
    {
        const App* p = unbox<App>(obj); 
        return p && *func == *p->func && *arg == *p->arg; 
    }
    virtual std::ostream& operator>>(std::ostream& os) const 
    {
        return os << "App(" << *func << ',' << *arg << ')'; 
    }
};

//------------------------------------------------------------------------------

template <typename F, F f>
auto deref(typename mch::member_traits<F>::class_type const* p) -> decltype(*mch::apply_member(p,f))
{
    return *mch::apply_member(p,f);
}

//------------------------------------------------------------------------------

namespace mch ///< Mach7 library namespace
{
template <> struct bindings<Var> { Members(Var::name); };
template <> struct bindings<Abs> { Members((deref<decltype(&Abs::var), &Abs::var>),  (deref<decltype(&Abs::body),&Abs::body>)); };
template <> struct bindings<App> { Members((deref<decltype(&App::func),&App::func>), (deref<decltype(&App::arg), &App::arg>)); };
} // of namespace mch

//------------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& os, const Term& t)
{
    var_of<std::string> s;
    ref_of<Var>  v;
    ref_of<Term> t1,t2;

    if (cls_of1<Var>(s)    .matches(t)) return os << s.m_var;
    if (cls_of2<Abs>(v,t1) .matches(t)) return os << '\\' << *v.m_var << '.' << *t1.m_var;
    if (cls_of2<App>(t1,t2).matches(t)) return os << '(' << *t1.m_var << ')' << '(' << *t2.m_var << ')';

    return os; // To prevent all control path warning
}

//------------------------------------------------------------------------------

bool operator==(const Term&, const Term&);
inline bool operator!=(const Term& left, const Term& right) { return !(left == right); }

//------------------------------------------------------------------------------

bool operator==(const Term& left, const Term& right)
{
    //std::clog << "(" << left << ',' << right << ')' << std::endl;
    var_of<std::string> s;
    equivalence es(s);
    ref_of<Term> v,t,f;
    equivalence ev(v),et(t),ef(f);

    if (cls_of1<Var>(s)  .matches(left) && cls_of1<Var>(es)   .matches(right)) return true;
    if (cls_of2<Abs>(v,t).matches(left) && cls_of2<Abs>(ev,et).matches(right)) return true;
    if (cls_of2<App>(f,t).matches(left) && cls_of2<App>(ef,et).matches(right)) return true;

    return false; // To prevent all control path warning
}

//------------------------------------------------------------------------------

bool equal_terms(const Term& left, const Term& right)
{
    if (typeid(left) != typeid(right))
        return false;

    if (typeid(left) == typeid(Var))
    {
        return static_cast<const Var&>(left).name == static_cast<const Var&>(right).name;
    }
    else
    if (typeid(left) == typeid(Abs))
    {
        const Abs& l = static_cast<const Abs&>(left);
        const Abs& r = static_cast<const Abs&>(right);

        return equal_terms(*l.var, *r.var) 
            && equal_terms(*l.body,*r.body);
    }
    else
    if (typeid(left) == typeid(App))
    {
        const App& l = static_cast<const App&>(left);
        const App& r = static_cast<const App&>(right);

        return equal_terms(*l.func,*r.func) 
            && equal_terms(*l.arg, *r.arg);
    }
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

inline size_t compare_terms1(Term* left, Term* right) { return equal_terms(*left, *right); }
inline size_t compare_terms2(Term* left, Term* right) { return *left == *right; }

//------------------------------------------------------------------------------
 
int main()
{
    using namespace mch;
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
    verdict v = get_timings2<size_t,Term*,compare_terms1,compare_terms2>(arguments);
    std::cout << "Verdict: \t" << v << std::endl;
}

//------------------------------------------------------------------------------
