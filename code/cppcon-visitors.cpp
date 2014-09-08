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

struct MutableBoolExpVisitor
{
    virtual void visitVarExp(VarExp&) {}
    virtual void visitValExp(ValExp&) {}
    virtual void visitNotExp(NotExp&) {}
    virtual void visitAndExp(AndExp&) {}
    virtual void visitOrExp (OrExp &) {}
};

struct BoolExp          { virtual void accept(BoolExpVisitor&  ) const = 0;                      virtual void accept(MutableBoolExpVisitor&  ) = 0; };
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
        void visitVarExp(const VarExp& x) { std::cout << x.name; }
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

bool evaluate(Context& ctx, const BoolExp* exp)
{
    struct EvaluateVisitor : BoolExpVisitor
    {
        EvaluateVisitor(Context& c) : m_ctx(c), result(false) {}

        bool     result;
        Context& m_ctx;

        void visitVarExp(const VarExp& x) { result = m_ctx[x.name]; }
        void visitValExp(const ValExp& x) { result = x.value; }
        void visitNotExp(const NotExp& x) { result =!evaluate(m_ctx, x.e); }
        void visitAndExp(const AndExp& x) { result = evaluate(m_ctx, x.e1) && evaluate(m_ctx, x.e2); }
        void visitOrExp (const OrExp & x) { result = evaluate(m_ctx, x.e1) || evaluate(m_ctx, x.e2); }
    } evaluator(ctx);

    exp->accept(evaluator);
    return evaluator.result;
}

BoolExp* replace(const BoolExp* where, const char* what, const BoolExp* with)
{
    struct ReplaceVisitor : BoolExpVisitor
    {
        ReplaceVisitor(const char* name, const BoolExp* with) : m_name(name), m_with(with), result(nullptr) {}

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

BoolExp* inplace(BoolExp* where, const char* what, const BoolExp* with)
{
    struct InplaceVisitor : MutableBoolExpVisitor
    {
        InplaceVisitor(const char* name, const BoolExp* with) : m_name(name), m_with(with), result(nullptr) {}

        BoolExp*       result;
        const char*       m_name;
        const BoolExp* m_with;

        void visitVarExp(VarExp& x) { result = x.name == m_name ? copy(m_with) : &x; }
        void visitValExp(ValExp& x) { result = &x; }
        void visitNotExp(NotExp& x) { result = &x; x.e  = inplace(x.e,  m_name, m_with); }
        void visitAndExp(AndExp& x) { result = &x; x.e1 = inplace(x.e1, m_name, m_with);
                                                   x.e2 = inplace(x.e2, m_name, m_with); }
        void visitOrExp (OrExp & x) { result = &x; x.e1 = inplace(x.e1, m_name, m_with);
                                                   x.e2 = inplace(x.e2, m_name, m_with); }
    } inplacer(what, with);

    where->accept(inplacer);
    return inplacer.result;
}

bool equal(const BoolExp*, const BoolExp*);

bool eq(const BoolExp& , const BoolExp& ) { return false; }
bool eq(const VarExp& a, const VarExp& b) { return a.name  == b.name;  }
bool eq(const ValExp& a, const ValExp& b) { return a.value == b.value; }
bool eq(const NotExp& a, const NotExp& b) { return equal(a.e, b.e); }
bool eq(const AndExp& a, const AndExp& b) { return equal(a.e1, b.e1) && equal(a.e2,b.e2); }
bool eq(const  OrExp& a, const  OrExp& b) { return equal(a.e1, b.e1) && equal(a.e2,b.e2); }

template <typename Exp>
struct EqualityToVisitor : BoolExpVisitor
{
    EqualityToVisitor(const Exp* x1) : m_x1(x1), result(false) {}

    bool       result;
    const Exp* m_x1;

    void visitVarExp(const VarExp& x) { result = eq(*m_x1,x); }
    void visitValExp(const ValExp& x) { result = eq(*m_x1,x); }
    void visitNotExp(const NotExp& x) { result = eq(*m_x1,x); }
    void visitAndExp(const AndExp& x) { result = eq(*m_x1,x); }
    void visitOrExp (const OrExp & x) { result = eq(*m_x1,x); }
};

bool equal(const BoolExp* x1, const BoolExp* x2)
{
    struct EqualityVisitor : BoolExpVisitor
    {
        EqualityVisitor(const BoolExp* x2) : m_x2(x2), result(false) {}

        bool           result;
        const BoolExp* m_x2;

        void visitVarExp(const VarExp& x) { EqualityToVisitor<VarExp> v(&x); m_x2->accept(v); result = v.result; }
        void visitValExp(const ValExp& x) { EqualityToVisitor<ValExp> v(&x); m_x2->accept(v); result = v.result; }
        void visitNotExp(const NotExp& x) { EqualityToVisitor<NotExp> v(&x); m_x2->accept(v); result = v.result; }
        void visitAndExp(const AndExp& x) { EqualityToVisitor<AndExp> v(&x); m_x2->accept(v); result = v.result; }
        void visitOrExp (const OrExp & x) { EqualityToVisitor<OrExp>  v(&x); m_x2->accept(v); result = v.result; }
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

bool mc(const BoolExp& , const BoolExp&  , Assignments& ctx) { return false; }
bool mc(const VarExp& a, const BoolExp& b, Assignments& ctx) { if (ctx[a.name] == nullptr) { ctx[a.name] = copy(&b); return true; } else { return equal(ctx[a.name],&b); } }
bool mc(const ValExp& a, const ValExp&  b, Assignments& ctx) { return a.value == b.value; }
bool mc(const NotExp& a, const NotExp&  b, Assignments& ctx) { return match(a.e,  b.e,  ctx); }
bool mc(const AndExp& a, const AndExp&  b, Assignments& ctx) { return match(a.e1, b.e1, ctx) && match(a.e2,b.e2, ctx); }
bool mc(const  OrExp& a, const  OrExp&  b, Assignments& ctx) { return match(a.e1, b.e1, ctx) && match(a.e2,b.e2, ctx); }

template <typename Exp>
struct MatchToVisitor : BoolExpVisitor
{
    MatchToVisitor(const Exp* p, Assignments& ctx) : m_p(p), m_ctx(ctx), result(false) {}

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
        MatchVisitor(const BoolExp* x, Assignments& ctx) : m_x(x), m_ctx(ctx), result(false) {}

        bool           result;
        const BoolExp* m_x;
        Assignments&   m_ctx;

        void visitVarExp(const VarExp& x) { MatchToVisitor<VarExp> v(&x,m_ctx); m_x->accept(v); result = v.result; }
        void visitValExp(const ValExp& x) { MatchToVisitor<ValExp> v(&x,m_ctx); m_x->accept(v); result = v.result; }
        void visitNotExp(const NotExp& x) { MatchToVisitor<NotExp> v(&x,m_ctx); m_x->accept(v); result = v.result; }
        void visitAndExp(const AndExp& x) { MatchToVisitor<AndExp> v(&x,m_ctx); m_x->accept(v); result = v.result; }
        void visitOrExp (const OrExp & x) { MatchToVisitor<OrExp>  v(&x,m_ctx); m_x->accept(v); result = v.result; }
    } matcher(x,ctx);

    p->accept(matcher);
    return matcher.result;
}

//template <typename SelfType>
//struct BinaryBoolExpVisitor : BoolExpVisitor


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
    std::cout << evaluate(ctx, exp1) << std::endl;
    std::cout << evaluate(ctx, exp2) << std::endl;
    std::cout << evaluate(ctx, exp3) << std::endl;

	std::cout << ctx << std::endl;
    //for (auto x : ctx) { std::cout << x.first << '=' << x.second << std::endl; }

    Assignments ctx2;

    if (match(exp2,exp3,ctx2))
    {
        std::cout << "exp2 matches exp3 with assignments: " << std::endl;
		for (Assignments::const_iterator p = ctx2.begin(); p != ctx2.end(); ++p)
		{
			std::cout << p->first << '='; print(p->second); std::cout << std::endl; 
		}
		//std::cout << ctx2 << std::endl;
        //for (auto x : ctx2) { std::cout << x.first << '='; print(x.second); std::cout << std::endl; }
    }
}
