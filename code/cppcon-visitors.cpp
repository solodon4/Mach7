struct VariableExp;
struct Constant   ;
struct NotExp     ;
struct AndExp     ;
struct OrExp      ;

struct BooleanExpVisitor
{
    void visitVariableExp(const VariableExp&) {}
    void visitConstant   (const Constant   &) {}
    void visitNotExp     (const NotExp     &) {}
    void visitAndExp     (const AndExp     &) {}
    void visitOrExp      (const OrExp      &) {}
};

struct BooleanExp               { virtual void accept(BooleanExpVisitor&) = 0;                             };
struct VariableExp : BooleanExp { virtual void accept(BooleanExpVisitor& v) { v.visitVariableExp(*this); } std::string name; };
struct Constant    : BooleanExp { virtual void accept(BooleanExpVisitor& v) { v.visitConstant   (*this); } bool value; };
struct NotExp      : BooleanExp { virtual void accept(BooleanExpVisitor& v) { v.visitNotExp     (*this); } BooleanExp* operand; };
struct AndExp      : BooleanExp { virtual void accept(BooleanExpVisitor& v) { v.visitAndExp     (*this); } BooleanExp* operand1; BooleanExp* operand2; };
struct OrExp       : BooleanExp { virtual void accept(BooleanExpVisitor& v) { v.visitOrExp      (*this); } BooleanExp* operand1; BooleanExp* operand2; };

#include <map>

typedef std::map<std::string,bool> Context;

bool evaluate(Context& ctx, BooleanExp& exp)
{
    struct EvaluateVisitor : BooleanExpVisitor
    {
        EvaluateVisitor(Context& c) : m_ctx(c) {}

        bool     result;
        Context& m_ctx;

        void visitVariableExp(const VariableExp& x) { result = m_ctx[x.name]; }
        void visitConstant   (const Constant   & x) { result = x.value; }
        void visitNotExp     (const NotExp     & x) { result =!evaluate(m_ctx, x.operand); }
        void visitAndExp     (const AndExp     & x) { result = evaluate(m_ctx, x.operand1) && evaluate(m_ctx, x.operand2); }
        void visitOrExp      (const OrExp      & x) { result = evaluate(m_ctx, x.operand1) || evaluate(m_ctx, x.operand2); }
    } evaluator(ctx);

    exp.accept(evaluator);
    return evaluator.result;
}
