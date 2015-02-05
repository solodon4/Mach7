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
/// This file contains examples used in our CppCon 2014 talk "Accept No Visitors".
/// \see http://bit.ly/AcceptNoVisitors
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// \see https://parasol.tamu.edu/mach7/
/// \see https://github.com/solodon4/Mach7
/// \see https://github.com/solodon4/SELL
///

#include <string>

struct VarExp;
struct ValExp;
struct NotExp;
struct AndExp;
struct OrExp ;

struct BoolExpVisitor
{
    virtual void visitVarExp(const VarExp&) {}
    virtual void visitValExp(const ValExp&) {}
    virtual void visitNotExp(const NotExp&) {}
    virtual void visitAndExp(const AndExp&) {}
    virtual void visitOrExp (const OrExp &) {}
};

struct MutableBoolExpVisitor : BoolExpVisitor
{
    using BoolExpVisitor::visitVarExp;
    using BoolExpVisitor::visitValExp;
    using BoolExpVisitor::visitNotExp;
    using BoolExpVisitor::visitAndExp;
    using BoolExpVisitor::visitOrExp;

    virtual void visitVarExp(VarExp& x) { BoolExpVisitor::visitVarExp(x); }
    virtual void visitValExp(ValExp& x) { BoolExpVisitor::visitValExp(x); }
    virtual void visitNotExp(NotExp& x) { BoolExpVisitor::visitNotExp(x); }
    virtual void visitAndExp(AndExp& x) { BoolExpVisitor::visitAndExp(x); }
    virtual void visitOrExp (OrExp & x) { BoolExpVisitor::visitOrExp (x); }
};

struct BoolExp
{
    virtual void accept(       BoolExpVisitor&) const = 0; // Read-only introspection
    virtual void accept(MutableBoolExpVisitor&)       = 0; // Mutable visitation
};

struct VarExp : BoolExp { virtual void accept(BoolExpVisitor& v) const { v.visitVarExp(*this); } virtual void accept(MutableBoolExpVisitor& v) { v.visitVarExp(*this); } VarExp(const char* n)            : name(n)        {} std::string name; };
struct ValExp : BoolExp { virtual void accept(BoolExpVisitor& v) const { v.visitValExp(*this); } virtual void accept(MutableBoolExpVisitor& v) { v.visitValExp(*this); } ValExp(bool b)                   : value(b)       {} bool value; };
struct NotExp : BoolExp { virtual void accept(BoolExpVisitor& v) const { v.visitNotExp(*this); } virtual void accept(MutableBoolExpVisitor& v) { v.visitNotExp(*this); } NotExp(BoolExp* e)               : e(e)           {} BoolExp* e; };
struct AndExp : BoolExp { virtual void accept(BoolExpVisitor& v) const { v.visitAndExp(*this); } virtual void accept(MutableBoolExpVisitor& v) { v.visitAndExp(*this); } AndExp(BoolExp* e1, BoolExp* e2) : e1(e1), e2(e2) {} BoolExp* e1; BoolExp* e2; };
struct OrExp  : BoolExp { virtual void accept(BoolExpVisitor& v) const { v.visitOrExp (*this); } virtual void accept(MutableBoolExpVisitor& v) { v.visitOrExp (*this); } OrExp (BoolExp* e1, BoolExp* e2) : e1(e1), e2(e2) {} BoolExp* e1; BoolExp* e2; };

#include <iostream>

void print(const BoolExp* exp)
{
    struct PrintVisitor : BoolExpVisitor
    {
        void visitVarExp(const VarExp& x) { std::cout << x.name;  }
        void visitValExp(const ValExp& x) { std::cout << x.value; }
        void visitNotExp(const NotExp& x) { std::cout << '!'; print(x.e); }
        void visitAndExp(const AndExp& x) { std::cout << '('; print(x.e1); std::cout << " & "; print(x.e2); std::cout << ')'; }
        void visitOrExp (const OrExp & x) { std::cout << '('; print(x.e1); std::cout << " | "; print(x.e2); std::cout << ')'; }
    } printer;

    exp->accept(printer);
}

BoolExp* copy(const BoolExp* exp)
{
    struct CopyVisitor : BoolExpVisitor
    {
        BoolExp* result;

        void visitVarExp(const VarExp& x) { result = new VarExp(x.name.c_str()); }
        void visitValExp(const ValExp& x) { result = new ValExp(x.value); }
        void visitNotExp(const NotExp& x) { result = new NotExp(copy(x.e)); }
        void visitAndExp(const AndExp& x) { result = new AndExp(copy(x.e1), copy(x.e2)); }
        void visitOrExp (const OrExp & x) { result = new  OrExp(copy(x.e1), copy(x.e2)); }
    } copier;

    exp->accept(copier);
    return copier.result;
}

#include <map>

template <class K, class T, class C,  class A>
std::ostream& operator<<(std::ostream& os, const std::map<K,T,C,A>& m)
{
	for (typename std::map<K,T,C,A>::const_iterator p = m.begin(); p != m.end(); ++p)
	{
		os << p->first << '=' << p->second << std::endl; 
	}

	return os;
}

typedef std::map<std::string,bool> Context;

bool eval(Context& ctx, const BoolExp* exp)
{
    struct EvalVisitor : BoolExpVisitor
    {
        EvalVisitor(Context& c) : result(false), m_ctx(c) {}

        bool     result;
        Context& m_ctx;

        void visitVarExp(const VarExp& x) { result = m_ctx[x.name]; }
        void visitValExp(const ValExp& x) { result = x.value; }
        void visitNotExp(const NotExp& x) { result =!eval(m_ctx, x.e); }
        void visitAndExp(const AndExp& x) { result = eval(m_ctx, x.e1) && eval(m_ctx, x.e2); }
        void visitOrExp (const OrExp & x) { result = eval(m_ctx, x.e1) || eval(m_ctx, x.e2); }
    } evaluator(ctx);

    exp->accept(evaluator);
    return evaluator.result;
}

BoolExp* replace(const BoolExp* where, const char* what, const BoolExp* with)
{
    struct ReplaceVisitor : BoolExpVisitor
    {
        ReplaceVisitor(const char* name, const BoolExp* with) : result(nullptr), m_name(name), m_with(with) {}

        BoolExp*       result;
        const char*    m_name;
        const BoolExp* m_with;

        void visitVarExp(const VarExp& x) { result = x.name == m_name ? copy(m_with) : copy(&x); }
        void visitValExp(const ValExp& x) { result = copy(&x); }
        void visitNotExp(const NotExp& x) { result = new NotExp(replace(x.e,  m_name, m_with)); }
        void visitAndExp(const AndExp& x) { result = new AndExp(replace(x.e1, m_name, m_with), replace(x.e2, m_name, m_with)); }
        void visitOrExp (const OrExp & x) { result = new  OrExp(replace(x.e1, m_name, m_with), replace(x.e2, m_name, m_with)); }
    } replacer(what, with);

    where->accept(replacer);
    return replacer.result;
}

BoolExp* inplace(BoolExp* where, const char* name, const BoolExp* with)
{
    struct InplaceVisitor : MutableBoolExpVisitor
    {
        InplaceVisitor(const char* name, const BoolExp* with) : result(nullptr), name(name), with(with) {}

        BoolExp*       result;
        const char*    name;
        const BoolExp* with;

        void visitVarExp(VarExp& x) { result = x.name == name ? copy(with) : &x; }
        void visitValExp(ValExp& x) { result = &x; }
        void visitNotExp(NotExp& x) { result = &x; x.e  = inplace(x.e,  name, with); }
        void visitAndExp(AndExp& x) { result = &x; x.e1 = inplace(x.e1, name, with);
                                                   x.e2 = inplace(x.e2, name, with); }
        void visitOrExp (OrExp & x) { result = &x; x.e1 = inplace(x.e1, name, with);
                                                   x.e2 = inplace(x.e2, name, with); }
    } inplacer(name, with);

    where->accept(inplacer);
    return inplacer.result;
}

bool equal(const BoolExp*, const BoolExp*);

bool eq(const BoolExp& , const BoolExp& ) { return false; }
bool eq(const VarExp& a, const VarExp& b) { return a.name  == b.name;  }
bool eq(const ValExp& a, const ValExp& b) { return a.value == b.value; }
bool eq(const NotExp& a, const NotExp& b) { return equal(a.e,  b.e);   }
bool eq(const AndExp& a, const AndExp& b) { return equal(a.e1, b.e1) && equal(a.e2,b.e2); }
bool eq(const  OrExp& a, const  OrExp& b) { return equal(a.e1, b.e1) && equal(a.e2,b.e2); }

template <typename Exp>
struct EqualToVisitor : BoolExpVisitor
{
    EqualToVisitor(const Exp* x1) : result(false), m_x1(x1) {}

    bool       result;
    const Exp* m_x1;

    void visitVarExp(const VarExp& x) { result = eq(*m_x1,x); }
    void visitValExp(const ValExp& x) { result = eq(*m_x1,x); }
    void visitNotExp(const NotExp& x) { result = eq(*m_x1,x); }
    void visitAndExp(const AndExp& x) { result = eq(*m_x1,x); }
    void visitOrExp (const OrExp & x) { result = eq(*m_x1,x); }
};

template <typename Visitor, typename T, typename A1>
auto execute(T&& t, const A1&& a1) -> decltype(Visitor(a1).result)
{
    Visitor v(std::forward<A1>(a1));
    std::forward(t).accept(v);
    return v.result;
}

bool equal(const BoolExp* x1, const BoolExp* x2)
{
    struct EqualityVisitor : BoolExpVisitor
    {
        EqualityVisitor(const BoolExp* x2) : result(false), x2(x2) {}

        bool           result;
        const BoolExp* x2;

        void visitVarExp(const VarExp& x1) { EqualToVisitor<VarExp> v(&x1); x2->accept(v); result = v.result; } // result = execute<EqualToVisitor<VarExp>>(x2,x1);
        void visitValExp(const ValExp& x1) { EqualToVisitor<ValExp> v(&x1); x2->accept(v); result = v.result; }
        void visitNotExp(const NotExp& x1) { EqualToVisitor<NotExp> v(&x1); x2->accept(v); result = v.result; }
        void visitAndExp(const AndExp& x1) { EqualToVisitor<AndExp> v(&x1); x2->accept(v); result = v.result; }
        void visitOrExp (const OrExp & x1) { EqualToVisitor<OrExp>  v(&x1); x2->accept(v); result = v.result; }
    } equator(x2);

    x1->accept(equator);
    return equator.result;
}

template <typename R>
struct BoolExpVisitorReturning : BoolExpVisitor
{
    R result;
    R apply_to(BoolExp& x) { x.accept(*this); return result; }
};

typedef std::map<std::string,const BoolExp*> Assignments;

bool match(const BoolExp*, const BoolExp*, Assignments&);

bool mc(const BoolExp& , const BoolExp&  , Assignments&    ) { return false; }
bool mc(const VarExp& a, const BoolExp& b, Assignments& ctx) { if (ctx[a.name] == nullptr) { ctx[a.name] = copy(&b); return true; } else { return equal(ctx[a.name],&b); } }
bool mc(const ValExp& a, const ValExp&  b, Assignments&    ) { return a.value == b.value; }
bool mc(const NotExp& a, const NotExp&  b, Assignments& ctx) { return match(a.e,  b.e,  ctx); }
bool mc(const AndExp& a, const AndExp&  b, Assignments& ctx) { return match(a.e1, b.e1, ctx) && match(a.e2,b.e2, ctx); }
bool mc(const  OrExp& a, const  OrExp&  b, Assignments& ctx) { return match(a.e1, b.e1, ctx) && match(a.e2,b.e2, ctx); }

template <typename Exp>
struct MatchToVisitor : BoolExpVisitor
{
    MatchToVisitor(const Exp* p, Assignments& ctx) : result(false), m_p(p), m_ctx(ctx) {}

    bool         result;
    const Exp*   m_p;
    Assignments& m_ctx;

    void visitVarExp(const VarExp& x) { result = mc(*m_p,x,m_ctx); }
    void visitValExp(const ValExp& x) { result = mc(*m_p,x,m_ctx); }
    void visitNotExp(const NotExp& x) { result = mc(*m_p,x,m_ctx); }
    void visitAndExp(const AndExp& x) { result = mc(*m_p,x,m_ctx); }
    void visitOrExp (const OrExp & x) { result = mc(*m_p,x,m_ctx); }
};

// Other example: unify
bool match(const BoolExp* p, const BoolExp* x, Assignments& ctx)
{
    struct MatchVisitor : BoolExpVisitor
    {
        MatchVisitor(const BoolExp* x, Assignments& ctx) : result(false), x(x), ctx(ctx) {}

        bool           result;
        const BoolExp* x;
        Assignments&   ctx;

        void visitVarExp(const VarExp& p) { MatchToVisitor<VarExp> v(&p,ctx); x->accept(v); result = v.result; }
        void visitValExp(const ValExp& p) { MatchToVisitor<ValExp> v(&p,ctx); x->accept(v); result = v.result; }
        void visitNotExp(const NotExp& p) { MatchToVisitor<NotExp> v(&p,ctx); x->accept(v); result = v.result; }
        void visitAndExp(const AndExp& p) { MatchToVisitor<AndExp> v(&p,ctx); x->accept(v); result = v.result; }
        void visitOrExp (const OrExp & p) { MatchToVisitor<OrExp>  v(&p,ctx); x->accept(v); result = v.result; }
    } matcher(x,ctx);

    p->accept(matcher);
    return matcher.result;
}

int main()
{
    BoolExp* exp1 = new AndExp(new OrExp(new VarExp("X"), new VarExp("Y")), new NotExp(new VarExp("Z")));

    std::cout << "exp1 = "; print(exp1); std::cout << std::endl;

    BoolExp* exp2 = copy(exp1);

    std::cout << "exp2 = "; print(exp2); std::cout << std::endl;

    BoolExp* exp3 = replace(exp1, "Z", exp2);

    std::cout << "exp3 = "; print(exp3); std::cout << std::endl;

    BoolExp* exp4 = inplace(exp1, "Z", exp2);

    std::cout << "exp4 = "; print(exp4); std::cout << std::endl;
    std::cout << "exp1 = "; print(exp1); std::cout << " updated! " << std::endl;

    std::cout << (equal(exp1,exp4) ? "exp1 == exp4" : "exp1 <> exp4") << std::endl;
    std::cout << (equal(exp1,exp2) ? "exp1 == exp2" : "exp1 <> exp2") << std::endl;

    Context ctx;
    ctx["Y"] = true;
    std::cout << eval(ctx, exp1) << std::endl;
    std::cout << eval(ctx, exp2) << std::endl;
    std::cout << eval(ctx, exp3) << std::endl;

	std::cout << ctx << std::endl;

    Assignments ctx2;

    if (match(exp2,exp3,ctx2))
    {
        std::cout << "exp2 matches exp3 with assignments: " << std::endl;

		for (Assignments::const_iterator p = ctx2.begin(); p != ctx2.end(); ++p)
		{
			std::cout << p->first << '='; print(p->second); std::cout << std::endl; 
		}
    }
}
