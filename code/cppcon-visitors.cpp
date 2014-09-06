#include <string>

struct VariableExp;
struct Constant   ;
struct NotExp     ;
struct AndExp     ;
struct OrExp      ;

struct BooleanExpVisitor
{
    virtual void visitVariableExp(const VariableExp&) {}
    virtual void visitConstant   (const Constant   &) {}
    virtual void visitNotExp     (const NotExp     &) {}
    virtual void visitAndExp     (const AndExp     &) {}
    virtual void visitOrExp      (const OrExp      &) {}
};

struct MutableBooleanExpVisitor
{
    virtual void visitVariableExp(VariableExp&) {}
    virtual void visitConstant   (Constant   &) {}
    virtual void visitNotExp     (NotExp     &) {}
    virtual void visitAndExp     (AndExp     &) {}
    virtual void visitOrExp      (OrExp      &) {}
};

struct BooleanExp               { virtual void accept(BooleanExpVisitor&  ) const = 0;                           virtual void accept(MutableBooleanExpVisitor&  ) = 0; };
struct VariableExp : BooleanExp { virtual void accept(BooleanExpVisitor& v) const { v.visitVariableExp(*this); } virtual void accept(MutableBooleanExpVisitor& v) { v.visitVariableExp(*this); } VariableExp(const char* n) : name(n) {} std::string name; };
struct Constant    : BooleanExp { virtual void accept(BooleanExpVisitor& v) const { v.visitConstant   (*this); } virtual void accept(MutableBooleanExpVisitor& v) { v.visitConstant   (*this); } Constant(bool b) : value(b) {}          bool value; };
struct NotExp      : BooleanExp { virtual void accept(BooleanExpVisitor& v) const { v.visitNotExp     (*this); } virtual void accept(MutableBooleanExpVisitor& v) { v.visitNotExp     (*this); } NotExp(BooleanExp* e) : e(e) {}   BooleanExp* e; };
struct AndExp      : BooleanExp { virtual void accept(BooleanExpVisitor& v) const { v.visitAndExp     (*this); } virtual void accept(MutableBooleanExpVisitor& v) { v.visitAndExp     (*this); } AndExp(BooleanExp* e1, BooleanExp* e2) : e1(e1), e2(e2) {} BooleanExp* e1; BooleanExp* e2; };
struct OrExp       : BooleanExp { virtual void accept(BooleanExpVisitor& v) const { v.visitOrExp      (*this); } virtual void accept(MutableBooleanExpVisitor& v) { v.visitOrExp      (*this); } OrExp (BooleanExp* e1, BooleanExp* e2) : e1(e1), e2(e2) {} BooleanExp* e1; BooleanExp* e2; };

#include <iostream>

void print(const BooleanExp* exp)
{
    struct PrintVisitor : BooleanExpVisitor
    {
        void visitVariableExp(const VariableExp& x) { std::cout << x.name; }
        void visitConstant   (const Constant   & x) { std::cout << x.value; }
        void visitNotExp     (const NotExp     & x) { std::cout << '!'; print(x.e); }
        void visitAndExp     (const AndExp     & x) { std::cout << '('; print(x.e1); std::cout << " & "; print(x.e2); std::cout << ')'; }
        void visitOrExp      (const OrExp      & x) { std::cout << '('; print(x.e1); std::cout << " | "; print(x.e2); std::cout << ')'; }
    } printer;

    exp->accept(printer);
}

BooleanExp* copy(const BooleanExp* exp)
{
    struct CopyVisitor : BooleanExpVisitor
    {
        BooleanExp* result;

        void visitVariableExp(const VariableExp& x) { result = new VariableExp(x.name.c_str()); }
        void visitConstant   (const Constant   & x) { result = new Constant(x.value); }
        void visitNotExp     (const NotExp     & x) { result = new NotExp(copy(x.e)); }
        void visitAndExp     (const AndExp     & x) { result = new AndExp(copy(x.e1), copy(x.e2)); }
        void visitOrExp      (const OrExp      & x) { result = new  OrExp(copy(x.e1), copy(x.e2)); }
    } copier;

    exp->accept(copier);
    return copier.result;
}

#include <map>

typedef std::map<std::string,bool> Context;

bool evaluate(Context& ctx, const BooleanExp* exp)
{
    struct EvaluateVisitor : BooleanExpVisitor
    {
        EvaluateVisitor(Context& c) : m_ctx(c), result(false) {}

        bool     result;
        Context& m_ctx;

        void visitVariableExp(const VariableExp& x) { result = m_ctx[x.name]; }
        void visitConstant   (const Constant   & x) { result = x.value; }
        void visitNotExp     (const NotExp     & x) { result =!evaluate(m_ctx, x.e); }
        void visitAndExp     (const AndExp     & x) { result = evaluate(m_ctx, x.e1) && evaluate(m_ctx, x.e2); }
        void visitOrExp      (const OrExp      & x) { result = evaluate(m_ctx, x.e1) || evaluate(m_ctx, x.e2); }
    } evaluator(ctx);

    exp->accept(evaluator);
    return evaluator.result;
}

BooleanExp* replace(const BooleanExp* where, const char* what, const BooleanExp* with)
{
    struct ReplaceVisitor : BooleanExpVisitor
    {
        ReplaceVisitor(const char* name, const BooleanExp* with) : m_name(name), m_with(with), result(nullptr) {}

        BooleanExp*       result;
        const char*       m_name;
        const BooleanExp* m_with;

        void visitVariableExp(const VariableExp& x) { result = x.name == m_name ? copy(m_with) : copy(&x); }
        void visitConstant   (const Constant   & x) { result = copy(&x); }
        void visitNotExp     (const NotExp     & x) { result = new NotExp(replace(x.e,  m_name, m_with)); }
        void visitAndExp     (const AndExp     & x) { result = new AndExp(replace(x.e1, m_name, m_with), replace(x.e2, m_name, m_with)); }
        void visitOrExp      (const OrExp      & x) { result = new  OrExp(replace(x.e1, m_name, m_with), replace(x.e2, m_name, m_with)); }
    } replacer(what, with);

    where->accept(replacer);
    return replacer.result;
}

BooleanExp* inplace(BooleanExp* where, const char* what, const BooleanExp* with)
{
    struct InplaceVisitor : MutableBooleanExpVisitor
    {
        InplaceVisitor(const char* name, const BooleanExp* with) : m_name(name), m_with(with), result(nullptr) {}

        BooleanExp*       result;
        const char*       m_name;
        const BooleanExp* m_with;

        void visitVariableExp(VariableExp& x) { result = x.name == m_name ? copy(m_with) : &x; }
        void visitConstant   (Constant   & x) { result = &x; }
        void visitNotExp     (NotExp     & x) { result = &x; x.e  = inplace(x.e,  m_name, m_with); }
        void visitAndExp     (AndExp     & x) { result = &x; x.e1 = inplace(x.e1, m_name, m_with);
                                                             x.e2 = inplace(x.e2, m_name, m_with); }
        void visitOrExp      (OrExp      & x) { result = &x; x.e1 = inplace(x.e1, m_name, m_with);
                                                             x.e2 = inplace(x.e2, m_name, m_with); }
    } inplacer(what, with);

    where->accept(inplacer);
    return inplacer.result;
}

bool equal(const BooleanExp*, const BooleanExp*);

bool eq(const BooleanExp&   , const BooleanExp&   ) { return false; }
bool eq(const VariableExp& a, const VariableExp& b) { return a.name  == b.name;  }
bool eq(const Constant&    a, const Constant&    b) { return a.value == b.value; }
bool eq(const NotExp&      a, const NotExp&      b) { return equal(a.e, b.e); }
bool eq(const AndExp&      a, const AndExp&      b) { return equal(a.e1, b.e1) && equal(a.e2,b.e2); }
bool eq(const  OrExp&      a, const  OrExp&      b) { return equal(a.e1, b.e1) && equal(a.e2,b.e2); }

template <typename Exp>
struct EqualityToVisitor : BooleanExpVisitor
{
    EqualityToVisitor(const Exp* x1) : m_x1(x1), result(false) {}

    bool       result;
    const Exp* m_x1;

    void visitVariableExp(const VariableExp& x) { result = eq(*m_x1,x); }
    void visitConstant   (const Constant   & x) { result = eq(*m_x1,x); }
    void visitNotExp     (const NotExp     & x) { result = eq(*m_x1,x); }
    void visitAndExp     (const AndExp     & x) { result = eq(*m_x1,x); }
    void visitOrExp      (const OrExp      & x) { result = eq(*m_x1,x); }
};

bool equal(const BooleanExp* x1, const BooleanExp* x2)
{
    struct EqualityVisitor : BooleanExpVisitor
    {
        EqualityVisitor(const BooleanExp* x2) : m_x2(x2), result(false) {}

        bool              result;
        const BooleanExp* m_x2;

        void visitVariableExp(const VariableExp& x) { EqualityToVisitor<VariableExp> v(&x); m_x2->accept(v); result = v.result; }
        void visitConstant   (const Constant   & x) { EqualityToVisitor<Constant>    v(&x); m_x2->accept(v); result = v.result; }
        void visitNotExp     (const NotExp     & x) { EqualityToVisitor<NotExp>      v(&x); m_x2->accept(v); result = v.result; }
        void visitAndExp     (const AndExp     & x) { EqualityToVisitor<AndExp>      v(&x); m_x2->accept(v); result = v.result; }
        void visitOrExp      (const OrExp      & x) { EqualityToVisitor<OrExp>       v(&x); m_x2->accept(v); result = v.result; }
    } equator(x2);

    x1->accept(equator);
    return equator.result;
}

template <typename R>
struct BooleanExpVisitorReturning : BooleanExpVisitor
{
    R result;
    R apply_to(BooleanExp& x) { x.accept(*this); return result; }
};

// Other example: unify
bool match(const BooleanExp* x1, const BooleanExp* x2)
{
    struct MatchVisitor : BooleanExpVisitor
    {
        MatchVisitor(const BooleanExp* x2) : m_x2(x2), result(false) {}

        bool              result;
        const BooleanExp* m_x2;

        void visitVariableExp(const VariableExp& x) { EqualityToVisitor<VariableExp> v(&x); m_x2->accept(v); result = v.result; }
        void visitConstant   (const Constant   & x) { EqualityToVisitor<Constant>    v(&x); m_x2->accept(v); result = v.result; }
        void visitNotExp     (const NotExp     & x) { EqualityToVisitor<NotExp>      v(&x); m_x2->accept(v); result = v.result; }
        void visitAndExp     (const AndExp     & x) { EqualityToVisitor<AndExp>      v(&x); m_x2->accept(v); result = v.result; }
        void visitOrExp      (const OrExp      & x) { EqualityToVisitor<OrExp>       v(&x); m_x2->accept(v); result = v.result; }
    } matcher(x2);

    x1->accept(matcher);
    return matcher.result;
}

int main()
{
    BooleanExp* exp1 = new AndExp(new OrExp(new VariableExp("X"), new VariableExp("Y")), new NotExp(new VariableExp("Z")));

    std::cout << "exp1 = "; print(exp1); std::cout << std::endl;

    BooleanExp* exp2 = copy(exp1);

    std::cout << "exp2 = "; print(exp2); std::cout << std::endl;

    BooleanExp* exp3 = replace(exp1, "Z", exp2);

    std::cout << "exp3 = "; print(exp3); std::cout << std::endl;

    BooleanExp* exp4 = inplace(exp1, "Z", exp2);

    std::cout << "exp4 = "; print(exp4); std::cout << std::endl;
    std::cout << "exp1 = "; print(exp1); std::cout << " updated! " << std::endl;

    std::cout << (equal(exp1,exp4) ? "exp1 == exp4" : "exp1 <> exp4") << std::endl;
    std::cout << (equal(exp1,exp2) ? "exp1 == exp2" : "exp1 <> exp2") << std::endl;

    Context ctx;
    ctx["Y"] = true;
    std::cout << evaluate(ctx, exp1) << std::endl;
    std::cout << evaluate(ctx, exp2) << std::endl;
    std::cout << evaluate(ctx, exp3) << std::endl;
    for (auto x : ctx) { std::cout << x.first << '=' << x.second << std::endl; }
}
