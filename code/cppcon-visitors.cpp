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

struct BooleanExp               { virtual void accept(BooleanExpVisitor&) = 0;                             };
struct VariableExp : BooleanExp { virtual void accept(BooleanExpVisitor& v) { v.visitVariableExp(*this); } VariableExp(const char* n) : name(n) {} std::string name; };
struct Constant    : BooleanExp { virtual void accept(BooleanExpVisitor& v) { v.visitConstant   (*this); } Constant(bool b) : value(b) {}          bool value; };
struct NotExp      : BooleanExp { virtual void accept(BooleanExpVisitor& v) { v.visitNotExp     (*this); } NotExp(BooleanExp* e) : operand(e) {}   BooleanExp* operand; };
struct AndExp      : BooleanExp { virtual void accept(BooleanExpVisitor& v) { v.visitAndExp     (*this); } AndExp(BooleanExp* e1, BooleanExp* e2) : operand1(e1), operand2(e2) {} BooleanExp* operand1; BooleanExp* operand2; };
struct OrExp       : BooleanExp { virtual void accept(BooleanExpVisitor& v) { v.visitOrExp      (*this); } OrExp (BooleanExp* e1, BooleanExp* e2) : operand1(e1), operand2(e2) {} BooleanExp* operand1; BooleanExp* operand2; };

#include <iostream>

void print(BooleanExp* exp)
{
    struct PrintVisitor : BooleanExpVisitor
    {
        void visitVariableExp(const VariableExp& x) { std::cout << x.name; }
        void visitConstant   (const Constant   & x) { std::cout << x.value; }
        void visitNotExp     (const NotExp     & x) { std::cout << '!'; print(x.operand); }
        void visitAndExp     (const AndExp     & x) { print(x.operand1); std::cout << " & "; print(x.operand2); }
        void visitOrExp      (const OrExp      & x) { print(x.operand1); std::cout << " | "; print(x.operand2); }
    } printer;

    exp->accept(printer);
}

#include <map>

typedef std::map<std::string,bool> Context;

bool evaluate(Context& ctx, BooleanExp* exp)
{
    struct EvaluateVisitor : BooleanExpVisitor
    {
        EvaluateVisitor(Context& c) : m_ctx(c), result(false) {}

        bool     result;
        Context& m_ctx;

        void visitVariableExp(const VariableExp& x) { result = m_ctx[x.name]; }
        void visitConstant   (const Constant   & x) { result = x.value; }
        void visitNotExp     (const NotExp     & x) { result =!evaluate(m_ctx, x.operand); }
        void visitAndExp     (const AndExp     & x) { result = evaluate(m_ctx, x.operand1) && evaluate(m_ctx, x.operand2); }
        void visitOrExp      (const OrExp      & x) { result = evaluate(m_ctx, x.operand1) || evaluate(m_ctx, x.operand2); }
    } evaluator(ctx);

    exp->accept(evaluator);
    return evaluator.result;
}

int main()
{
    BooleanExp* exp = new OrExp(new AndExp(new VariableExp("X"), new VariableExp("Y")), new NotExp(new VariableExp("Z")));

    print(exp); std::cout << std::endl;

    Context ctx;
    ctx["X"] = !ctx["S"];

    std::cout << evaluate(ctx, exp) << std::endl;
    for (auto x : ctx) { std::cout << x.first << '=' << x.second << std::endl; }
}
