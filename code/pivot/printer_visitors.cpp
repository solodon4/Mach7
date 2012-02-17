#include <algorithm>
#include <deque>
#include <iomanip>
#include <vector>
#include <assert.h>
#include <ipr/utility>           // Some Pivot utilities
#include "printer.hpp"
#include "visitor_base.hpp"      // various IPR visitor base classes
#include "iprutils.hpp"          // Helper functions to work with IPR
#include "predicates.hpp"        // Helper predicates on IPR nodes
#include "strutils.hpp"          // String utilities
#include "precedence.hpp"

/// Visitors version of C++ printer
namespace cxxv
{

/// Set of template parameters with corresponding nesting of the currently
/// processed template to be resolved for Rname nodes.
std::deque<const ipr::Parameter_list*> template_parameters_stack; // FIX: Make this thread local somehow

//==============================================================================
// Forward Declarations
//==============================================================================

std::string eval_name(const ipr::Name&);
std::string eval_type(const ipr::Type&, const std::string& = "", size_t = 0);
std::string eval_classic(const ipr::Classic&);
std::string eval_decl(const ipr::Decl&);
std::string eval_expr(const ipr::Expr&);
std::string eval_params(const ipr::Parameter_list& parameters);

//------------------------------------------------------------------------------

inline std::string eval(const ipr::Name& n)    { return eval_name(n); }
inline std::string eval(const ipr::Type& n)    { return eval_type(n); }
inline std::string eval(const ipr::Classic& n) { return eval_classic(n); }
inline std::string eval(const ipr::Decl& n)    { return eval_decl(n); }
inline std::string eval(const ipr::Expr& n)    { return eval_expr(n); }

//------------------------------------------------------------------------------

template <typename T>
std::string eval_sqnc(const ipr::Sequence<T>& s, int from = 0, const char* separator = ",")
{
    std::string result;

    for (int i = from, n = s.size(); i < n; i++)
    {
        if (i != from)
            result += separator;

        result += eval(s[i]);
    }

    return result;
}

//------------------------------------------------------------------------------

class IndentationBasedVisitor;

//------------------------------------------------------------------------------

void evaluate_expr(const ipr::Expr& n, IndentationBasedVisitor& ii);
void evaluate_expr(const ipr::Expr& n, std::ostream& os = std::cout, int indent_level = 0);
void evaluate_expr(const ipr::Expr& n, std::ostream& os, comments_interface& c, int indent_level = 0);
void evaluate_stmt(const ipr::Stmt& n, IndentationBasedVisitor& ii);
void evaluate_stmt(const ipr::Stmt& n, std::ostream& os = std::cout, int indent_level = 0);
void evaluate_stmt(const ipr::Stmt& n, std::ostream& os, comments_interface& c, int indent_level = 0);
void evaluate_decl(const ipr::Decl& n, IndentationBasedVisitor& ii);
void evaluate_decl(const ipr::Decl& n, std::ostream& os, bool part_of_expression);
void evaluate_decl(const ipr::Decl& n, std::ostream& os = std::cout, int indent_level = 0);
void evaluate_decl(const ipr::Decl& n, std::ostream& os, comments_interface& c, int indent_level = 0);
void evaluate_udt (const ipr::Udt&  n, IndentationBasedVisitor& ii);
void evaluate_udt (const ipr::Udt&  n, std::ostream& os = std::cout, int indent_level = 0);
void evaluate_udt (const ipr::Udt&  n, std::ostream& os, comments_interface& c, int indent_level = 0);
bool evaluate_map (const ipr::Mapping& n, const ipr::Named_map& d, std::ostream& os, int indent_level);

#define eval_classic_ex eval_expr

//==============================================================================
// Helper functions
//==============================================================================

std::vector<std::string> tokenize_with(const std::string& str, const std::string& delimiters)
{
    std::vector<std::string> res;
    tokenize(str, back_inserter(res), delimiters);
    return res;
}

//------------------------------------------------------------------------------

std::ostream& indent(std::ostream& os, int indent_level = 0, bool empty_line = false)
{
    os << std::endl;

    if (empty_line)
        os << std::endl;

    if (indent_level)
        os << std::setw(indent_level*4) << ' ';

    return os;
}

//------------------------------------------------------------------------------

std::string prefix_specifiers(const ipr::Decl& s, ipr::Decl::Specifier mask = ipr::Decl::Specifier(-1))
{
    ipr::Decl::Specifier spec = mask & s.specifiers() & ipr::applicable_specifiers(s);
    std::string result;

    if (ipr::is_in_class_declaration(s))
    {
        if (spec & ipr::Decl::Public)    result += "public: ";
        if (spec & ipr::Decl::Protected) result += "protected: ";
        if (spec & ipr::Decl::Private)   result += "private: ";
    }

    if (spec & ipr::Decl::Auto)      result += "auto ";
    if (spec & ipr::Decl::Register)  result += "register ";
    if (spec & ipr::Decl::Static)    result += "static ";
    if (spec & ipr::Decl::Extern)    result += "extern ";
    if (spec & ipr::Decl::Mutable)   result += "mutable ";
    if (spec & ipr::Decl::Inline)    result += "inline ";
    if (spec & ipr::Decl::Virtual)   result += "virtual ";
    if (spec & ipr::Decl::Explicit)  result += "explicit ";
  //if (spec & ipr::Decl::Pure)      result += "pure ";
    if (spec & ipr::Decl::Friend)    result += "friend ";
    if (spec & ipr::Decl::Typedef)   result += "typedef ";
    if (spec & ipr::Decl::Export)    result += "export ";
    if (spec & ipr::Decl::Constexpr) result += "constexpr ";

    return result;
}

//------------------------------------------------------------------------------

std::string get_declarator(const ipr::Decl& d, bool explicit_global_scope = false)
{
    std::string declarator = eval_name(d.name());
    const ipr::Region* hr = &d.home_region();
    const ipr::Region* lr = &d.lexical_region();

    assert(lr);

    while (hr != lr)
    {
        assert(hr);

        if (ipr::util::node_has_member(*hr, &ipr::Region::enclosing))
        {
            if (const ipr::Udt* udt = ipr::util::view<ipr::Udt>(hr->owner()))
			{
				declarator = eval_name(udt->name()) + "::" + declarator;
			}
			else
			if (const ipr::Mapping* map = ipr::util::view<ipr::Mapping>(hr->owner()))
			{
				// Do nothing, template id with parameters have already been prepended
			}
			else
				assert(!"Unhandled case in get_declarator");

            hr = &hr->enclosing();
        }
        else  // We've reached global scope whose enclosing regions is null
		{
			if (explicit_global_scope)
				declarator = "::" + declarator;

            break; // The following may happen on friend functions for example
		}
    }

    return declarator;
}

//==============================================================================
// NamePrintVisitor
//==============================================================================

// Visitor that prints a name.
class NamePrintVisitor : public ipr::assertive_visitor_base<std::string>
{
public:

    // Names

    virtual void visit(const ipr::Identifier  & e)    { result = to_str(e.string()); }
    virtual void visit(const ipr::Id_expr     & e)    { result = eval_name(e.name()); }
    virtual void visit(const ipr::Operator    & e)    { result = "operator"  + to_str(e.opname()); }
    virtual void visit(const ipr::Conversion  & e)    { result = "operator " + eval_type(e.target()); }
    virtual void visit(const ipr::Scope_ref   & e)    
    {
        result = eval_expr(e.scope()) + "::";

        if (const ipr::Template_id* p = ipr::util::view<ipr::Template_id>(e.member()))
            if (ipr::is_template_dependent(e.scope()))
                result += "template ";

        result += eval_expr(e.member());
    }
    virtual void visit(const ipr::Template_id & e)    
    {
        std::string args = eval_sqnc(e.args().elements());

        if (args.length() > 0 && args[args.length()-1] == '>')
            args += ' ';

        result = eval_name(e.template_name()) + '<' + args + '>';
    }
    virtual void visit(const ipr::Ctor_name   & e)    { result = eval_type(e.object_type()); }
    virtual void visit(const ipr::Dtor_name   & e)    { result = "~" + eval_type(e.object_type()); }
    virtual void visit(const ipr::Rname       & e)    
    {
        //std::cout << template_parameters_stack.size() << ": Accessing: R[" << e.level() << ',' << e.position() << ']' << std::endl;
        //assert(e.level()    < template_parameters_stack.size()); // We should have processed corresponding Named_map and put parameters on the stack
        //assert(e.position() < template_parameters_stack[e.level()]->size()); // Parameter list of that level should be long enough to have such position
        // FIX: The above assertion crashes on use_stl_vector.cpp for probably an IPR bug
        //      Temporarily removed with the below condition:
        if (e.level()    < template_parameters_stack.size() &&
            e.position() < template_parameters_stack[e.level()]->size())
            result = eval_name(template_parameters_stack[e.level()]->operator[](e.position()).name());
        else
            result = "BUGGY_IPR_RNAME_" + to_str(e.level()) + '_' + to_str(e.position());
    }
    virtual void visit(const ipr::Type_id     & e)    { result = eval_type(e.type_expr()); }
};

//==============================================================================
// TypePrintVisitor
//==============================================================================

// Visitor that prints a type expression.
class TypePrintVisitor : public ipr::assertive_visitor_base<std::string>
{
private:

    typedef ipr::assertive_visitor_base<std::string> base_type;

public:

    TypePrintVisitor(const std::string& declarator = "", size_t precedence = 0) : base_type(declarator), m_precedence(precedence) {}

    std::string declarator(const ipr::Type& result_type) const
    {
        return 
            ::precedence(result_type) < m_precedence
                ? '(' + result + ')'
                :       result;
    }

    std::string postfix(const ipr::Type& result_type) const 
    {
        std::string res = declarator(result_type);
        return !res.empty() && isid(res[0]) ? ' '+res : res;
    }

    std::string  prefix(const ipr::Type& result_type) const
    {
        std::string res = declarator(result_type);
        return !res.empty() && isid(res[res.length()-1]) ? res+' ' : res;
    }

    // Type expressions

    virtual void visit(const ipr::Array         & t)  { result = eval_type(t.element_type(), declarator(t) + '[' + eval_expr(t.bound())+ ']', ipr::precedence<ipr::Array>::value); }
    virtual void visit(const ipr::Void          & t)  { result = "void"               + postfix(t); }
    virtual void visit(const ipr::Bool          & t)  { result = "bool"               + postfix(t); }
    virtual void visit(const ipr::Char          & t)  { result = "char"               + postfix(t); }
    virtual void visit(const ipr::sChar         & t)  { result = "signed char"        + postfix(t); }
    virtual void visit(const ipr::uChar         & t)  { result = "unsigned char"      + postfix(t); }
    virtual void visit(const ipr::Wchar_t       & t)  { result = "wchar_t"            + postfix(t); }
    virtual void visit(const ipr::Short         & t)  { result = "short"              + postfix(t); }
    virtual void visit(const ipr::uShort        & t)  { result = "unsigned short"     + postfix(t); }
    virtual void visit(const ipr::Int           & t)  { result = "int"                + postfix(t); }
    virtual void visit(const ipr::uInt          & t)  { result = "unsigned int"       + postfix(t); }
    virtual void visit(const ipr::Long          & t)  { result = "long"               + postfix(t); }
    virtual void visit(const ipr::uLong         & t)  { result = "unsigned long"      + postfix(t); }
    virtual void visit(const ipr::Long_long     & t)  { result = "long long"          + postfix(t); }
    virtual void visit(const ipr::uLong_long    & t)  { result = "unsigned long long" + postfix(t); }
    virtual void visit(const ipr::Float         & t)  { result = "float"              + postfix(t); }
    virtual void visit(const ipr::Double        & t)  { result = "double"             + postfix(t); }
    virtual void visit(const ipr::Long_double   & t)  { result = "long double"        + postfix(t); }
    virtual void visit(const ipr::Ellipsis      & t)  { result = "..."; }
    virtual void visit(const ipr::Decltype      & t)  { result = "decltype(" + eval_expr(t.expr()) + ')'; }
    virtual void visit(const ipr::Function      & t)  
    { 
        result = eval_type(t.target(), declarator(t) + '(' + eval_expr(t.source()) + ')', ipr::precedence<ipr::Function>::value);

        if (!can_throw_everything(t)) // we skip throw specification when any exception can be thrown
            result += " throw(" + eval_expr(t.throws()) + ')';
    }
    virtual void visit(const ipr::Pointer       & t)  { result = eval_type(t.points_to(), '*' + postfix(t), ipr::precedence<ipr::Pointer>::value); }
    virtual void visit(const ipr::Reference     & t)  { result = eval_type(t.refers_to(), '&' + postfix(t), ipr::precedence<ipr::Reference>::value); }
    virtual void visit(const ipr::Ptr_to_member & t)  { result = eval_type(t.member_type(), eval_type(t.containing_type()) + "::*" + postfix(t), ipr::precedence<ipr::Ptr_to_member>::value); }
    virtual void visit(const ipr::Qualified     & t)
    {
        std::string qualifiers;

        if (t.qualifiers() & ipr::Type::Const)
            qualifiers += "const ";

        if (t.qualifiers() & ipr::Type::Volatile)
            qualifiers += "volatile ";

        if (t.qualifiers() & ipr::Type::Restrict)
#ifdef _MSC_VER
            qualifiers += "__restrict ";
#else
            qualifiers += "__restrict__ ";
#endif

        result = eval_type(t.main_variant(), qualifiers + postfix(t.main_variant()), ::precedence(t.main_variant()));
    }
    
    virtual void visit(const ipr::Product       & t)  { result = eval_sqnc(t.elements()); }
    virtual void visit(const ipr::Sum           & t)  { result = eval_sqnc(t.elements()); }
    virtual void visit(const ipr::Template      & t)
    {
        result = "template <" + eval_type(t.source()) + "> " + eval_type(t.target(), postfix(t));
    }
    virtual void visit(const ipr::As_type       & t)  { result = eval_expr(t.expr()) + postfix(t); } // This may represent user-defined class
    virtual void visit(const ipr::Udt           & t)  { result = eval_name(t.name()) + postfix(t); } // This is analog of Decl handling in expression context - we only refer the name

private:

    /// Declarator's precedence
    size_t m_precedence;

};

//==============================================================================
// ClassicExprPrintVisitor
//==============================================================================

// Visitor that prints an expression in the classic C++ meaning of it.
class ClassicExprPrintVisitor : public ipr::assertive_visitor_base<std::string>
{
public:

    virtual void visit(const ipr::Literal& e)         { result = to_str(e.string()); }

    // Unary Operations

    virtual void visit(const ipr::Address& e)         { result = "&" + eval_classic_ex(e.operand()); }
    virtual void visit(const ipr::Array_delete& e)    { result = "delete[] " + eval_classic_ex(e.operand()); }
    virtual void visit(const ipr::Complement& e)      { result = "~" + eval_classic_ex(e.operand()); }
    virtual void visit(const ipr::Delete& e)          { result = "delete " + eval_classic_ex(e.operand()); }
    virtual void visit(const ipr::Deref& e)           { result = "*" + eval_classic_ex(e.operand()); }
    virtual void visit(const ipr::Expr_sizeof& e)     { result = "sizeof(" + eval_classic_ex(e.operand()) + ")"; }
    virtual void visit(const ipr::Expr_typeid& e)     { result = "typeid(" + eval_classic_ex(e.operand()) + ")"; }
    virtual void visit(const ipr::Not& e)             { result = "!" + eval_classic_ex(e.operand()); }
    virtual void visit(const ipr::Paren_expr& e)      { result = "(" + eval_classic_ex(e.operand()) + ")"; }
    virtual void visit(const ipr::Post_decrement& e)  { result = eval_classic_ex(e.operand()) + "--"; }
    virtual void visit(const ipr::Post_increment& e)  { result = eval_classic_ex(e.operand()) + "++"; }
    virtual void visit(const ipr::Pre_decrement& e)   { result = "--" + eval_classic_ex(e.operand()); }
    virtual void visit(const ipr::Pre_increment& e)   { result = "++" + eval_classic_ex(e.operand()); }
    virtual void visit(const ipr::Throw& e)           { result = "throw " + eval_classic_ex(e.operand()); }
    virtual void visit(const ipr::Type_sizeof& e)     { result = "sizeof(" + eval_classic_ex(e.operand()) + ")"; }
    virtual void visit(const ipr::Type_typeid& e)     { result = "typeid(" + eval_classic_ex(e.operand()) + ")"; }
    virtual void visit(const ipr::Unary_minus& e)     { result = "-" + eval_classic_ex(e.operand()); }
    virtual void visit(const ipr::Unary_plus& e)      { result = "+" + eval_classic_ex(e.operand()); }

    // Binary Operations

    virtual void visit(const ipr::Plus& e)            { result = eval_classic_ex(e.first()) + " + "  + eval_classic_ex(e.second()); }
    virtual void visit(const ipr::Minus& e)           { result = eval_classic_ex(e.first()) + " - "  + eval_classic_ex(e.second()); }
    virtual void visit(const ipr::Mul& e)             { result = eval_classic_ex(e.first()) + " * "  + eval_classic_ex(e.second()); }
    virtual void visit(const ipr::Div& e)             { result = eval_classic_ex(e.first()) + " / "  + eval_classic_ex(e.second()); }
    virtual void visit(const ipr::Modulo& e)          { result = eval_classic_ex(e.first()) + " % "  + eval_classic_ex(e.second()); }
    virtual void visit(const ipr::Bitand& e)          { result = eval_classic_ex(e.first()) + " & "  + eval_classic_ex(e.second()); }
    virtual void visit(const ipr::Bitor& e)           { result = eval_classic_ex(e.first()) + " | "  + eval_classic_ex(e.second()); }
    virtual void visit(const ipr::Bitxor& e)          { result = eval_classic_ex(e.first()) + " ^ "  + eval_classic_ex(e.second()); }
    virtual void visit(const ipr::Lshift& e)          { result = eval_classic_ex(e.first()) + " << " + eval_classic_ex(e.second()); }
    virtual void visit(const ipr::Rshift& e)          { result = eval_classic_ex(e.first()) + " >> " + eval_classic_ex(e.second()); }

    virtual void visit(const ipr::Assign& e)          { result = eval_classic_ex(e.first()) + " = "   + eval_classic_ex(e.second()); }
    virtual void visit(const ipr::Plus_assign& e)     { result = eval_classic_ex(e.first()) + " += "  + eval_classic_ex(e.second()); }
    virtual void visit(const ipr::Minus_assign& e)    { result = eval_classic_ex(e.first()) + " -= "  + eval_classic_ex(e.second()); }
    virtual void visit(const ipr::Mul_assign& e)      { result = eval_classic_ex(e.first()) + " *= "  + eval_classic_ex(e.second()); }
    virtual void visit(const ipr::Div_assign& e)      { result = eval_classic_ex(e.first()) + " /= "  + eval_classic_ex(e.second()); }
    virtual void visit(const ipr::Modulo_assign& e)   { result = eval_classic_ex(e.first()) + " %= "  + eval_classic_ex(e.second()); }
    virtual void visit(const ipr::Bitand_assign& e)   { result = eval_classic_ex(e.first()) + " &= "  + eval_classic_ex(e.second()); }
    virtual void visit(const ipr::Bitor_assign& e)    { result = eval_classic_ex(e.first()) + " |= "  + eval_classic_ex(e.second()); }
    virtual void visit(const ipr::Bitxor_assign& e)   { result = eval_classic_ex(e.first()) + " ^= "  + eval_classic_ex(e.second()); }
    virtual void visit(const ipr::Lshift_assign& e)   { result = eval_classic_ex(e.first()) + " <<= " + eval_classic_ex(e.second()); }
    virtual void visit(const ipr::Rshift_assign& e)   { result = eval_classic_ex(e.first()) + " >>= " + eval_classic_ex(e.second()); }

    virtual void visit(const ipr::And& e)             { result = eval_classic_ex(e.first()) + " && "  + eval_classic_ex(e.second()); }
    virtual void visit(const ipr::Or& e)              { result = eval_classic_ex(e.first()) + " || "  + eval_classic_ex(e.second()); }
    virtual void visit(const ipr::Equal& e)           { result = eval_classic_ex(e.first()) + " == "  + eval_classic_ex(e.second()); }
    virtual void visit(const ipr::Not_equal& e)       { result = eval_classic_ex(e.first()) + " != "  + eval_classic_ex(e.second()); }
    virtual void visit(const ipr::Greater& e)         { result = eval_classic_ex(e.first()) + " > "   + eval_classic_ex(e.second()); }
    virtual void visit(const ipr::Greater_equal& e)   { result = eval_classic_ex(e.first()) + " >= "  + eval_classic_ex(e.second()); }
    virtual void visit(const ipr::Less& e)            { result = eval_classic_ex(e.first()) + " < "   + eval_classic_ex(e.second()); }
    virtual void visit(const ipr::Less_equal& e)      { result = eval_classic_ex(e.first()) + " <= "  + eval_classic_ex(e.second()); }

    virtual void visit(const ipr::Comma& e)           { result = eval_classic_ex(e.first()) + " , " + eval_classic_ex(e.second()); }
    virtual void visit(const ipr::Datum& e)           { result = eval_classic_ex(e.first()) + '('   + eval_sqnc(e.args().elements()) + ')'; }

    // Binary Operations: Casts

    virtual void visit(const ipr::Cast& e)            { result =                 "(" + eval_type(e.type()) + ")"  + eval_classic_ex(e.expr()); }
    virtual void visit(const ipr::Const_cast& e)      { result =       "const_cast<" + eval_type(e.type()) + ">(" + eval_classic_ex(e.expr()) + ")"; }
    virtual void visit(const ipr::Dynamic_cast& e)    { result =     "dynamic_cast<" + eval_type(e.type()) + ">(" + eval_classic_ex(e.expr()) + ")"; }
    virtual void visit(const ipr::Reinterpret_cast& e){ result = "reinterpret_cast<" + eval_type(e.type()) + ">(" + eval_classic_ex(e.expr()) + ")"; }
    virtual void visit(const ipr::Static_cast& e)     { result =      "static_cast<" + eval_type(e.type()) + ">(" + eval_classic_ex(e.expr()) + ")"; }

    // Binary Operations: Member selection

    virtual void visit(const ipr::Array_ref& e)       { result = eval_classic_ex(e.first()) + "["  + eval_classic_ex(e.second()) + "]"; }    
    virtual void visit(const ipr::Arrow& e)           
    {
        if (e.has_impl_decl())
        {
            result = eval_classic_ex(e.base()) + '.' + eval_name(e.impl_decl().name()) + "()";

            if (!is_phantom(e.member()))
                result += "->" + eval_classic_ex(e.member());
        }
        else
            result = eval_classic_ex(e.first()) + "->" + eval_classic_ex(e.second()); 
    }
    virtual void visit(const ipr::Dot& e)             { result = eval_classic_ex(e.first()) + "."   + eval_classic_ex(e.second()); }
    virtual void visit(const ipr::Arrow_star& e)      { result = eval_classic_ex(e.first()) + "->*" + eval_classic_ex(e.second()); }
    virtual void visit(const ipr::Dot_star& e)        { result = eval_classic_ex(e.first()) + ".*"  + eval_classic_ex(e.second()); }

    // Ternary operations

    virtual void visit(const ipr::New& e)             { result = "new " + eval_type(e.allocated_type()); } // FIX: Take placement into account
    virtual void visit(const ipr::Conditional& e)     { result = eval_classic_ex(e.condition()) + " ? "  + eval_classic_ex(e.then_expr()) + " : " + eval_classic_ex(e.else_expr()); }

    // Other operations

    virtual void visit(const ipr::Call& e)
    {
        std::string func_name = eval_classic_ex(e.function());

        if (const ipr::Fundecl* target = call_target(e)) // Call target is known
        {
            const ipr::Parameter_list& parameters = target->parameters();

            if (is_member_function(*target))
            {
                // Call to a member function
                assert(e.args().size() > 0); // There must be argument that represents this pointer

                // Because of function call unification, calls to member functions
                // are still represented as regular calls with this pointer passed
                // explicitly as the first argument. The original call might have
                // been on reference to object, in which case an ipr::Address node
                // is artificially inserted. Here we are trying to undo that 
                // unnecessary address to print the dot when possible.
                if (const ipr::Address* addr = ipr::util::view<ipr::Address>(e.args()[0]))
                {
                    // The address is artifical and the actual call is through dot
                    result = eval_classic_ex(addr->operand()) + '.';
                }
                else
                {
                    // The actual call is through this pointer
                    result = eval_classic_ex(e.args()[0]) + "->";
                }

                // Note that using target->name() instead of func_name might omit scope etc.
                result += func_name + '(' + eval_sqnc(e.args().elements(), 1) + ')';
            }
            else // not a member function
            {
                // Call to a regular function
                result  = func_name + '(' + eval_sqnc(e.args().elements()) + ')';
            }
        }
        else // Call target is not known
        {
            if (const ipr::Function* ft = ipr::util::view<ipr::Function>(e.function().type()))
                if (is_member_function_type(*ft))
                {
                    // Call to a member function
                    assert(e.args().size() > 0); // There must be argument that represents this pointer

                    // Because of function call unification, calls to member functions
                    // are still represented as regular calls with this pointer passed
                    // explicitly as the first argument. The original call might have
                    // been on reference to object, in which case an ipr::Address node
                    // is artificially inserted. Here we are trying to undo that 
                    // unnecessary address to print the dot when possible.
                    if (const ipr::Address* addr = ipr::util::view<ipr::Address>(e.args()[0]))
                    {
                        // The address is artifical and the actual call is through dot
                        result = eval_classic_ex(addr->operand()) + '.';
                    }
                    else
                    {
                        // The actual call is through this pointer
                        result = eval_classic_ex(e.args()[0]) + "->";
                    }

                    // Note that using target->name() instead of func_name might omit scope etc.
                    result += func_name + '(' + eval_sqnc(e.args().elements(), 1) + ')';
                    return;
                }

            result = func_name + '(' + eval_sqnc(e.args().elements()) + ')';
        }
    }
};

//==============================================================================
// IndentationBasedVisitor
//==============================================================================

// Common implementation base class for visitors that require indentation support
class IndentationBasedVisitor : public ipr::assertive_visitor_base<std::ostream&>
{
private:

    typedef ipr::assertive_visitor_base<std::ostream&> base_type;

public:

    IndentationBasedVisitor(
        std::ostream&       os, 
        bool                part_of_expression
    ) :
        base_type(os),
        m_part_of_expression(part_of_expression),
        m_indent_level(0),
        m_commenter(0)
    {}

    IndentationBasedVisitor(
        std::ostream&       os,
        int                 indent_level = 0
    ) :
        base_type(os),
        m_part_of_expression(false),
        m_indent_level(indent_level),
        m_commenter(0)
    {}
    
    IndentationBasedVisitor(
        std::ostream&       os,
        comments_interface& c,
        int                 indent_level = 0
    ) :
        base_type(os),
        m_part_of_expression(false),
        m_indent_level(indent_level),
        m_commenter(&c)
    {}

    friend void evaluate_stmt(const ipr::Stmt&, IndentationBasedVisitor&);
    friend void evaluate_decl(const ipr::Decl&, IndentationBasedVisitor&);

protected:

    // A syntactic sugar to show the intent that we would like to forward
    // evaluation to a subcomponent.
    void forward_to(const ipr::Node& n, bool indent = true)
    {
        if (indent)
            ++m_indent_level;

        n.accept(*this);

        if (indent)
            --m_indent_level;
    }

    template <class T>
    void sequence_forward_to(const ipr::Sequence<T>& s, bool indent = true)
    {
        for (typename ipr::Sequence<T>::iterator p = s.begin(); p != s.end(); ++p)
            forward_to(*p, indent);
    }

    std::ostream& indent(bool empty_line = false) const
    {
        if (!m_part_of_expression)
        {
            result << std::endl;

            if (empty_line)
                result << std::endl;

            if (m_indent_level)
                result << std::setw(m_indent_level*4) << ' ';
        }

        return result;
    }

    std::ostream& before(const ipr::Stmt& s, bool empty_line = false) const
    {
        // FIX: Add support for printing a line that shows which file the source
        //      is coming from till that file becomes different.
        if (m_part_of_expression)
            return result;

        // Print comments that user generated
        if (m_commenter)
        {
            // Separate into strings line-by-line
            std::vector<std::string> tokens = tokenize_with(m_commenter->before(s), "\n");
            // Get rid of duplicates
            std::vector<std::string>::iterator new_end = std::unique(tokens.begin(), tokens.end());
            tokens.resize(new_end-tokens.begin());
            // Show each line with markers as to which statement it refers to
            for (std::vector<std::string>::const_iterator p = tokens.begin(); p != tokens.end(); ++p)
            {
                indent(empty_line) << "//-> " << *p;
                empty_line = false;
            }
        }

        // Print comments/annotations that were in the original source file
        const ipr::Sequence<ipr::Annotation>& annotations = s.annotation();

        for (ipr::Sequence<ipr::Annotation>::iterator p = annotations.begin(); p != annotations.end(); ++p)
        {
            indent(empty_line) << to_str(p->name());
            empty_line = false;
        }

        return indent(empty_line);
    }

    void after(const ipr::Stmt& s, bool new_line = false) const
    {
        if (m_part_of_expression)
            return;

        if (m_commenter)
        {
            // Separate into strings line-by-line
            std::vector<std::string> tokens = tokenize_with(m_commenter->after(s), "\n");
            // Get rid of duplicates
            std::vector<std::string>::iterator new_end = std::unique(tokens.begin(), tokens.end());
            tokens.resize(new_end-tokens.begin());
            // Show each line with markers as to which statement it refers to
            for (std::vector<std::string>::const_iterator p = tokens.begin(); p != tokens.end(); ++p)
            {
                if (new_line)
                    indent();

                result << (new_line ? "//<- " : " //<- ") << *p;
                new_line = true;
            }
        }
    }

    /// Indicates that corresponding statement is printed as part of expression
    /// and thus trailing ; as well as new-lines and comments should be avoided.
    const bool m_part_of_expression;

    /// A level of indentation currently used.
    int m_indent_level;

    /// A user-supplied object used to generate comments to the source
    comments_interface* m_commenter;

};

//==============================================================================
// StatementPrintVisitor
//==============================================================================

// Visitor that prints executable code.
class StatementPrintVisitor : public IndentationBasedVisitor
{
public:

    StatementPrintVisitor(IndentationBasedVisitor& ii) : IndentationBasedVisitor(ii) {}
    StatementPrintVisitor(std::ostream& os, bool part_of_expression) : IndentationBasedVisitor(os,part_of_expression) {}
    StatementPrintVisitor(std::ostream& os, int indent_level = 0) : IndentationBasedVisitor(os,indent_level) {}
    StatementPrintVisitor(std::ostream& os, comments_interface& c, int indent_level = 0) : IndentationBasedVisitor(os,c,indent_level) {}

    // Declarations

    virtual void visit(const ipr::Labeled_stmt & s)         { before(s) << (is_case_label(s.label()) ? "case " : "") << eval_expr(s.label()) << ": "; forward_to(s.stmt(), false/*, !ipr::util::view<ipr::Labeled_stmt>(s.stmt())*/); after(s); }
    virtual void visit(const ipr::Block        & s)
    {
        if (s.handlers().size())
        {
            before(s) << "try";
            indent()  << '{';
        }
        else
            before(s) << '{';

        // Print statements in the body
        sequence_forward_to(s.body());

        indent() << '}';

        // Print catch handlers
        sequence_forward_to(s.handlers(), false);

        after(s,true);
    }
    virtual void visit(const ipr::Ctor_body    & s)         { if (s.inits().size()) result << " : " << eval_sqnc(s.inits().elements()); forward_to(s.block(),false); }
    virtual void visit(const ipr::If_then      & s)         { before(s,true) << "if (" << eval_classic_ex(s.condition()) << ")"; forward_to(s.then_stmt()); after(s,true); }
    virtual void visit(const ipr::If_then_else & s)         { before(s,true) << "if (" << eval_classic_ex(s.condition()) << ")"; forward_to(s.then_stmt()); indent() << "else"; forward_to(s.else_stmt()); after(s,true); }
    virtual void visit(const ipr::Switch       & s)         { before(s,true) << "switch (" << eval_classic_ex(s.condition()) << ')'; forward_to(s.body(),false); after(s,true); }
    virtual void visit(const ipr::While        & s)         { before(s,true) << "while (" << eval_classic_ex(s.condition()) << ")"; forward_to(s.body()); after(s,true); }
    virtual void visit(const ipr::Do           & s)         { before(s,true) << "do "; forward_to(s.body()); indent() << "while (" << eval_classic_ex(s.condition()) << ");"; after(s,true); }
    virtual void visit(const ipr::For          & s)         { before(s,true) << "for (" << eval_classic_ex(s.initializer()) << ";" << eval_classic_ex(s.condition()) << ";" << eval_classic_ex(s.increment()) << ")"; forward_to(s.body()); after(s,true); }
    virtual void visit(const ipr::Break        & s)         { before(s) << "break;"; after(s); }
    virtual void visit(const ipr::Continue     & s)         { before(s) << "continue;"; after(s); }
    virtual void visit(const ipr::Goto         & s)
    {
        const ipr::Identifier* p = ipr::util::view<ipr::Identifier>(s.target());
        assert(p); // When this asserts, ipr::Goto stopped pointing to just Identifier
        before(s) << "goto " << (p ? eval_name(*p) : "???") << ";";
        after(s);
    }
    virtual void visit(const ipr::Return       & s)         { before(s) << "return " << eval_classic_ex(s.value()) << ";"; after(s); }
    virtual void visit(const ipr::Handler      & s)         { before(s) << "catch(" << eval_decl(s.exception()) << ')'; forward_to(s.body(),false); after(s); }
    virtual void visit(const ipr::Expr_stmt    & s)         { before(s) << eval_classic_ex(s.expr()) << ";"; after(s); }
    virtual void visit(const ipr::Empty_stmt   & s)         { before(s) << ";"; after(s); }

    // Declarations
    virtual void visit(const ipr::Decl         & s)         { evaluate_decl(s,*this); }
};

//==============================================================================
// DeclarationPrintVisitor
//==============================================================================

// Visitor that prints definitions in the program.
class DeclarationPrintVisitor : public IndentationBasedVisitor
{
public:

    DeclarationPrintVisitor(IndentationBasedVisitor& ii) : IndentationBasedVisitor(ii) {}
    DeclarationPrintVisitor(std::ostream& os, bool part_of_expression) : IndentationBasedVisitor(os,part_of_expression) {}
    DeclarationPrintVisitor(std::ostream& os, int indent_level = 0) : IndentationBasedVisitor(os,indent_level) {}
    DeclarationPrintVisitor(std::ostream& os, comments_interface& c, int indent_level = 0) : IndentationBasedVisitor(os,c,indent_level) {}

    virtual void visit(const ipr::Expr& e) { result << eval_expr(e); }
    virtual void visit(const ipr::Decl&);
    virtual void visit(const ipr::Typedecl&);
    virtual void visit(const ipr::Fundecl&);
    virtual void visit(const ipr::Alias&);
    virtual void visit(const ipr::Bitfield&);
    virtual void visit(const ipr::Enumerator&);
    virtual void visit(const ipr::Named_map&);
    virtual void visit(const ipr::Global_scope&);
    virtual void visit(const ipr::Namespace&);
    virtual void visit(const ipr::Class&);
    virtual void visit(const ipr::Union&);
    virtual void visit(const ipr::Enum&);
};

//------------------------------------------------------------------------------

void DeclarationPrintVisitor::visit(const ipr::Decl & s)
{
    std::string declarator = get_declarator(s); //eval_name(s.name());
/*
    if (is_member(s) && !is_in_class_declaration(s))
    {
        const ipr::Udt* udt = ipr::util::view<ipr::Udt>(s.home_region().owner());
        assert(udt); // Because is_member implies this
        declarator = eval_name(udt->name()) + "::" + declarator; // FIX: This won't work for general case of namespaces, specializations etc., just a quick hack
    }
*/
    before(s) << prefix_specifiers(s) << eval_type(s.type(), declarator);

    if (s.has_initializer())
        result << " = " << eval_expr(s.initializer());

    if (!m_part_of_expression)
        result << ';';

    after(s);
}

//------------------------------------------------------------------------------

void DeclarationPrintVisitor::visit(const ipr::Typedecl& n)
{
    before(n,true) << prefix_specifiers(n) << eval_type(n.type(), eval_name(n.name()));

    if (is_namespace(n))
    {
        assert(n.has_initializer()); // Namespaces cannot be forward declared
        // initializer() would be a Namespace node, to which we forward traversal
        forward_to(n.initializer(), false);
    }
    else
    {
        if (n.has_initializer())
            forward_to(n.initializer(), false);
        else
        if (!m_part_of_expression)
            result << ';';
    }

    after(n,true); // Print comment after the type declaration/namespace
}

//------------------------------------------------------------------------------

void DeclarationPrintVisitor::visit(const ipr::Fundecl& f)
{
    before(f,true) << prefix_specifiers(f);

    // NOTE: For some reason currently virtual member functions are not marked with
    //       virtual specifier on the Fundecl but instead with the virtual specifier
    //       on the first parameter to account for later support of multi-methods.
    if (is_virtual_member_function(f) && is_in_class_declaration(f))
        result << "virtual ";

    const ipr::Parameter_list& parameters = f.parameters();

    // Evaluate declarator that will be used to output the result type that can
    // be complicated as function can return arrays or function pointers
    //std::string declarator = eval_name(f.name()) + '(' + eval_params(parameters) + ')';

    std::string declarator = get_declarator(f) + '(' + eval_params(parameters) + ')';

    // Prepend class name to the name when declaration/definition is made outside
    // of class and refers to a member function
    //if (is_member(f) && !is_in_class_declaration(f))
    //    declarator = eval_name(f.membership().name()) + "::" + declarator; // FIX: This won't work for general case of namespaces, specializations etc., just a quick hack
    //if (is_member_function(f) && !is_in_class_declaration(f))
    //    declarator = eval_name(f.membership().name()) + "::" + declarator; // FIX: This won't work for general case of namespaces, specializations etc., just a quick hack

    const ipr::Function* ft = ipr::util::view<ipr::Function>(f.type());  // dynamic_cast alternative
    assert(ft);

    // Print the name and return type when applicable
    if (!ipr::is_constructor(f) && !ipr::is_destructor(f) && !ipr::is_conversion(f))
        result << eval_type(ft->target(), declarator);
    else
        result << declarator;

    // Check if this is a const member function
    if (is_const_member_function(f))
        result << " const";

    // Print throw specification if the function has it
    if (!can_throw_everything(*ft)) // we skip throw specification when any exception can be thrown
        result << " throw(" + eval_type(ft->throws()) + ')';

    // Check if this is a pure virtual function (abstract method)
    if (f.specifiers() & ipr::Decl::Pure)
        result << " = 0";

    // Print body if present
    if (f.has_initializer())
    {
        const ipr::Stmt* body = ipr::util::view<ipr::Stmt>(f.initializer());
        assert(body);
        evaluate_stmt(*body,*this);
    }
    else
    if (!m_part_of_expression)
        result << ';';

    after(f,true); // Print comment after the function definition
}

//------------------------------------------------------------------------------

void DeclarationPrintVisitor::visit(const ipr::Alias& s)
{
    before(s,true) << prefix_specifiers(s);

    assert(s.has_initializer());

    const ipr::Expr& init = s.initializer();

	// This is the case of: typedef T U;
    if (const ipr::Type* p = ipr::util::view<ipr::Type>(init))
    {
        result << /*"typedef " <<*/ eval_type(*p,/*get_declarator(s)*/eval_name(s.name())) << ';'; // Since EDG front-end now sets up the Typedef specifier on ipr::Decl
        return;
    }

	// This is the case of: namespace N = A::B::C::D;
    if (const ipr::Id_expr* id = ipr::util::view<ipr::Id_expr>(init))
    {
        if (const ipr::Typedecl* td = ipr::util::view<ipr::Typedecl>(id->resolution()))
            if (ipr::is_namespace(*td))
            {
                result << "namespace " << eval_name(s.name()) << '=' << eval_name(id->name()) << ';';
                return;
            }
    }

	// This is the case of: using namespace A::B::C::D;
    if (const ipr::Typedecl* d = ipr::util::view<ipr::Typedecl>(init))
    {
        if (is_namespace(*d))
        {
            result << "using namespace " << eval_expr(s.initializer()) << ';';
            return;
        }
    }

	result << "using " << get_declarator(s,true) << ';';
    after(s);
}

//------------------------------------------------------------------------------

void DeclarationPrintVisitor::visit(const ipr::Bitfield& s)
{
    before(s) << prefix_specifiers(s) << eval_type(s.type(), eval_name(s.name())) << ':' << eval_classic_ex(s.precision());

    if (s.has_initializer())
        result << " = " << eval_classic_ex(s.initializer());

    if (!m_part_of_expression)
        result << ';';

    after(s);
}

//------------------------------------------------------------------------------

void DeclarationPrintVisitor::visit(const ipr::Enumerator& s)
{
    before(s) << prefix_specifiers(s) << eval_name(s.name());

    if (s.has_initializer())
        result << " = " << eval_classic_ex(s.initializer());

    after(s);
}

//------------------------------------------------------------------------------

void DeclarationPrintVisitor::visit(const ipr::Named_map& d)
{
    const ipr::Decl::Specifier prefix_mask = ipr::Decl::AccessProtection
                          | ipr::Decl::StorageClass
                          | ipr::Decl::Friend
                          | ipr::Decl::Typedef
                          | ipr::Decl::Constexpr
                          | ipr::Decl::Export;

    const ipr::Parameter_list& parameters = d.params();

    //indent() << template_parameters_stack.size() << ": Pushing: " << eval_params(parameters) << std::endl;

    //template_parameters_stack.push_back(&parameters); // Make current set of parameters available for referring to by Rname nodes

    // Print comment before the function definition
    before(d,true) << prefix_specifiers(d, prefix_mask)/* 
                   << "template <" << eval_params(parameters) << '>';
    indent()       << prefix_specifiers(d, ipr::Decl::FunctionSpecifier) 
                   << eval_type(d.mapping().result_type(), eval_name(d.name()))*/;

    //if (d.has_initializer())
    //    forward_to(d.initializer(), false);

    evaluate_map(d.mapping(), d, result, m_indent_level);

    //template_parameters_stack.pop_back(); // FIX: Make this RAII

    //indent() << template_parameters_stack.size() << ": Popping: " << eval_params(parameters) << std::endl;

    after(d,true); // Print comment after the function definition
}

//------------------------------------------------------------------------------

void DeclarationPrintVisitor::visit(const ipr::Global_scope& t)
{
    sequence_forward_to(t.members(), false);
}

//------------------------------------------------------------------------------

void DeclarationPrintVisitor::visit(const ipr::Namespace& t)
{
    indent() << '{';
    sequence_forward_to(t.members());
    indent() << '}';
}

//------------------------------------------------------------------------------

void DeclarationPrintVisitor::visit(const ipr::Class & t)
{
    // Print base classes
    const ipr::Sequence<ipr::Base_type>& bases = t.bases();

    if (bases.size())
    {
        result << " : ";

        for (ipr::Sequence<ipr::Base_type>::iterator p = bases.begin(); p != bases.end(); ++p)
        {
            if (p != bases.begin())
                result << ", ";

            result << eval_name(p->name());
        }
    }

    indent() << '{';
    sequence_forward_to(t.members());
    indent() << '}';

    if (!m_part_of_expression)
        result << ';';
}

//------------------------------------------------------------------------------

void DeclarationPrintVisitor::visit(const ipr::Union & t)
{
    indent() << '{';
    sequence_forward_to(t.members());
    indent() << '}';

    if (!m_part_of_expression)
        result << ';';
}

//------------------------------------------------------------------------------

void DeclarationPrintVisitor::visit(const ipr::Enum & t)
{
    indent() << '{';

    const ipr::Sequence<ipr::Enumerator>& members = t.members();

    for (ipr::Sequence<ipr::Enumerator>::iterator p = members.begin(); p != members.end(); ++p)
    {
        if (p != members.begin())
            result << ',';

        forward_to(*p);
    }

    indent() << '}';

    if (!m_part_of_expression)
        result << ';';
}

//==============================================================================
// ExprPrintVisitor
//==============================================================================

// Visitor that dispatches to the corresponding kind of generalized expression
class ExprPrintVisitor : public IndentationBasedVisitor
{
public:

    ExprPrintVisitor(IndentationBasedVisitor& ii) : IndentationBasedVisitor(ii) {}
    ExprPrintVisitor(std::ostream& os, bool part_of_expression) : IndentationBasedVisitor(os,part_of_expression) {}
    ExprPrintVisitor(std::ostream& os, int indent_level = 0) : IndentationBasedVisitor(os,indent_level) {}
    ExprPrintVisitor(std::ostream& os, comments_interface& c, int indent_level = 0) : IndentationBasedVisitor(os,c,indent_level) {}

    virtual void visit(const ipr::Mapping&     s)
    {
        if (ipr::util::node_has_member(s, &ipr::Mapping::result))
            evaluate_expr(s.result(),*this);
        else
            result << ';';
    }
    virtual void visit(const ipr::Overload&    s) { }
    virtual void visit(const ipr::Scope&       s)
    {
        // Scope can be in place of expression when expression is also a declaration
        // e.g. if (int i = foo()) {} or for (int i = 0, j = 0; ;);

        // This is a tricky case because first we have to generate initializers and
        // then also compute the operand that initializer resolves to as for example:
        // if (int i = 0, j = 1) ...

        const ipr::Sequence<ipr::Decl>& decls = s.members();

        for (ipr::Sequence<ipr::Decl>::iterator p = decls.begin(); p != decls.end(); ++p)
        {
            std::string s = eval_decl(*p); // Evaluate declaration without trailing ;

            if (p != decls.begin())
                result << ',';

            result << s;
        }
    }

    virtual void visit(const ipr::Classic&     s) { result << eval_classic(s); }
    virtual void visit(const ipr::Name&        s) { result << eval_name(s); }
    virtual void visit(const ipr::Type&        s) { result << eval_type(s); }
    virtual void visit(const ipr::Stmt&        s) { evaluate_stmt(s,*this); }
    virtual void visit(const ipr::Phantom&     s) { }
    virtual void visit(const ipr::Expr_list&   s) { result << '{' << eval_sqnc(s.operand()) << '}'; }
    virtual void visit(const ipr::Label&       s) { }
    virtual void visit(const ipr::Member_init& s) 
    {
        // FIX: This is workaround for now as it should become Expr_list statically
        if (const ipr::Expr_list* p = ipr::util::view<ipr::Expr_list>(s.initializer()))
            result << eval_expr(s.member())+'('+eval_sqnc(p->elements())+')';
        else
            result << eval_expr(s.member())+'('+eval_classic_ex(s.initializer())+')'; 
    }

    // The following kinds of nodes are not directly inherited from Expr so 
    // technically they shouldn't be handled here. However they may happen in 
    // context were traditional expressions are expected and those cases are
    // covered here.

    // Declaration might happen in expression context when IPR refers to the use
    // of the name but whoever generated IPR forgot to mark such use properly with
    // Id_expr and pointed directly to the declaration instead.
    virtual void visit(const ipr::Decl&        s) { forward_to(s.name(),false); }

    // Expr_stmt can happen in the initializer of the following loop:
    // int i; for (i=1; i < 10; i++)
    // because the scope won't be introduced. It seems to be easier at the
    // moment to simply handle this case here rather then all of the
    // cases of ClassicExprPrintVisitor inside StatementPrintVisitor when
    // forwarding evaluation to the initializer.
    virtual void visit(const ipr::Expr_stmt&   s) { forward_to(s.expr(),false); }

    // This shouldn't be here at all, but it's a quick hack right now to handle
    // template class definitions as mapping will forward here.
    virtual void visit(const ipr::Udt&         s) { evaluate_udt(s, *this); }

    // And this hack counter measure the bugs introduced by the above Udt hack
    // as references to Namespace should be just treated as names and not dumping
    // of their content that the above hack will do.
    virtual void visit(const ipr::Namespace&   s) { forward_to(s.name(),false); }

};

//==============================================================================
// NodePrintVisitor
//==============================================================================

// Visitor capable of printing any part of the program
class NodePrintVisitor : public ipr::assertive_visitor_base<std::ostream&>
{
private:

    typedef ipr::assertive_visitor_base<std::ostream&> base_type;

public:

    NodePrintVisitor(std::ostream& os) : base_type(os), m_commenter(0) {}
    NodePrintVisitor(std::ostream& os, comments_interface& c) : base_type(os), m_commenter(&c) {}

    void visit(const ipr::Name& n) { result << eval_name(n); }
    void visit(const ipr::Expr& n) { evaluate_expr(n, result, *m_commenter); }
    void visit(const ipr::Type& n) { result << eval_type(n); }
    void visit(const ipr::Stmt& n) { evaluate_stmt(n, result, *m_commenter); }
    void visit(const ipr::Decl& n) { evaluate_decl(n, result, *m_commenter); }

    void visit(const ipr::Udt&  n) { evaluate_udt (n, result, *m_commenter); }
    void visit(const ipr::Unit& n) { evaluate_udt (n.get_global_scope(), result, *m_commenter); } // This should forward to visitation of Global_scope, which is a Namespace

private:

    comments_interface* m_commenter;

};

//------------------------------------------------------------------------------

void print_cpp(const ipr::Node& n, std::ostream& os)
{
    ipr::execute<NodePrintVisitor>(n,os);
    os << std::endl;
}

//------------------------------------------------------------------------------

void print_cpp(const ipr::Node& n, std::ostream& os, comments_interface& c)
{
    ipr::execute<NodePrintVisitor>(n,os,c);
    os << std::endl;
}

//------------------------------------------------------------------------------

std::string eval_name(const ipr::Name& n) { return ipr::execute<NamePrintVisitor>(n); }

//------------------------------------------------------------------------------

std::string eval_expr(const ipr::Expr& n)                                          
{
    // Because in IPR everything is Expr, we have to figure out first what are 
    // we dealing with. To do this, we forward it to general dispatcher of Expr,
    // and we assume here that only the cases that won't print newlines etc. can
    // happen there.

    std::stringstream ss;
    evaluate_expr(n,ss);
    return ss.str();
}

//------------------------------------------------------------------------------

std::string eval_type(const ipr::Type& n, const std::string& declarator, size_t precedence) 
{
    std::string result = ipr::execute<TypePrintVisitor>(n,declarator,precedence);

    if (ipr::is_template_dependent_type(n))
        result = "typename " + result;

    return result;
}

//------------------------------------------------------------------------------

std::string eval_classic(const ipr::Classic& n) { return ipr::execute<ClassicExprPrintVisitor>(n); }

//------------------------------------------------------------------------------

std::string eval_decl(const ipr::Decl& n)
{
    std::stringstream ss;
    evaluate_decl(n,ss,true);
    return ss.str();
}

//------------------------------------------------------------------------------

std::string eval_params(const ipr::Parameter_list& parameters)
{
    std::string result;
    bool        print_comma = false;

    for (ipr::Parameter_list::iterator p = parameters.begin(); p != parameters.end(); ++p)
    {
        if (print_comma)
            result += ", ";

        if (ipr::is_this_pointer(*p)) // parameter represent implicit this pointer, skip it
            print_comma = false;
        else
        if (ipr::is_ellipsis(*p))     // parameter represents variable argument list ...
            result += "...";
        else
        {
            result += eval_type(p->type(), eval_name(p->name()));

            if (p->has_initializer())
                result += " = " + eval_classic_ex(p->default_value());

            print_comma = true;
        }
    }

    return result;
}

//------------------------------------------------------------------------------

void evaluate_expr(const ipr::Expr& n, IndentationBasedVisitor& ii)                               { ipr::execute<ExprPrintVisitor>(n, ii); }
void evaluate_expr(const ipr::Expr& n, std::ostream& os, int indent_level)                        { ipr::execute<ExprPrintVisitor>(n, os, indent_level); }
void evaluate_expr(const ipr::Expr& n, std::ostream& os, comments_interface& c, int indent_level) { ipr::execute<ExprPrintVisitor>(n, os, c, indent_level); }
void evaluate_stmt(const ipr::Stmt& n, IndentationBasedVisitor& ii)                               { ipr::execute<StatementPrintVisitor>(n, ii); }
void evaluate_stmt(const ipr::Stmt& n, std::ostream& os, int indent_level)                        { ipr::execute<StatementPrintVisitor>(n, os, indent_level); }
void evaluate_stmt(const ipr::Stmt& n, std::ostream& os, comments_interface& c, int indent_level) { ipr::execute<StatementPrintVisitor>(n, os, c, indent_level); }
void evaluate_decl(const ipr::Decl& n, IndentationBasedVisitor& ii)                               { ipr::execute<DeclarationPrintVisitor>(n, ii); }
void evaluate_decl(const ipr::Decl& n, std::ostream& os, bool part_of_expression)                 { ipr::execute<DeclarationPrintVisitor>(n, os, part_of_expression); }
void evaluate_decl(const ipr::Decl& n, std::ostream& os, int indent_level)                        { ipr::execute<DeclarationPrintVisitor>(n, os, indent_level); }
void evaluate_decl(const ipr::Decl& n, std::ostream& os, comments_interface& c, int indent_level) { ipr::execute<DeclarationPrintVisitor>(n, os, c, indent_level); }
void evaluate_udt (const ipr::Udt&  n, IndentationBasedVisitor& ii)                               { ipr::execute<DeclarationPrintVisitor>(n, ii); }
void evaluate_udt (const ipr::Udt&  n, std::ostream& os, int indent_level)                        { ipr::execute<DeclarationPrintVisitor>(n, os, indent_level); }
void evaluate_udt (const ipr::Udt&  n, std::ostream& os, comments_interface& c, int indent_level) { ipr::execute<DeclarationPrintVisitor>(n, os, c, indent_level); }

//------------------------------------------------------------------------------

bool evaluate_map (const ipr::Mapping& n, const ipr::Named_map& d, std::ostream& os, int indent_level)
{
    std::ostream& result = os; //indent(os, indent_level);

    if (const ipr::Function* ft = ipr::util::view<ipr::Function>(n.type()))
    {
        // Mapping represents regular function

        result << prefix_specifiers(d, ipr::Decl::FunctionSpecifier);

        const ipr::Parameter_list& parameters = n.params();

        // NOTE: For some reason currently virtual member functions are not marked with
        //       virtual specifier on the Fundecl but instead with the virtual specifier
        //       on the first parameter to account for later support of multi-methods.
        if (is_virtual_member_function(n) && is_in_class_declaration(d))
            result << "virtual ";

        // Evaluate declarator that will be used to output the result type that can
        // be complicated as function can return arrays or function pointers
        std::string declarator = eval_name(d.name()) + '(' + eval_params(parameters) + ')';

        //std::string declarator = get_declarator(d) + '(' + eval_params(parameters) + ')';

        // Prepend class name to the name when declaration/definition is made outside
        // of class and refers to a member function
        // Apparently we don't need this for templated functions
        //if (is_member_function(n) && !is_in_class_declaration(d))
        //    declarator = eval_name(parameters[0].type().name()) + "::" + declarator; // FIX: This won't work for general case of namespaces, specializations etc., just a quick hack

        // Print the name and return type when applicable
        if (!ipr::is_constructor(d) && !ipr::is_destructor(d) && !ipr::is_conversion(d))
            result << eval_type(ft->target(), declarator);
        else
            result << declarator;

        // Check if this is a const member function
        if (is_const_member_function(n))
            result << " const";

        // Print throw specification if the function has it
        if (!can_throw_everything(*ft)) // we skip throw specification when any exception can be thrown
            result << " throw(" + eval_type(ft->throws()) + ')';

        // Check if this is a pure virtual function (abstract method)
        if (d.specifiers() & ipr::Decl::Pure)
            result << " = 0";
    }
    else
    if (ipr::util::view<ipr::Template>(n.type()))
    {
        // Mapping represents a template

        const ipr::Parameter_list& parameters = n.params();

        template_parameters_stack.push_back(&parameters); // Make current set of parameters available for referring to by Rname nodes

        //std::cout << template_parameters_stack.size() << ": Pushing: " << eval_params(parameters) << std::endl;
        //indent(result, indent_level);

        std::string args = eval_params(parameters);

        if (args.length() > 0 && args[args.length()-1] == '>')
            args += ' ';

        // Print name and formal arguments
        result << "template <" << args << '>';

        if (ipr::util::node_has_member(n, &ipr::Mapping::result))
        {
            if (const ipr::Mapping* q = ipr::util::view<ipr::Mapping>(n.result()))
            {
                // The whole thing is a parametrized function
                indent(result, indent_level);

                // Forward to ourselves to print declaration
                // NOTE: In case of nested templates, all their result will point
                //       to the same body, but only the last one is allowed to
                //       print as only it know all the Rnames
                // We essentially check here if body has already been printed by
                // the recursive call.
                if (!evaluate_map(*q, d, result, indent_level))
                    if (ipr::util::node_has_member(*q, &ipr::Mapping::result))
                        evaluate_expr(q->result(), result, indent_level); // The result is going to be the body of the parameterized function
                    else
                        result << ';'; // Parameterized function forward declaration

                //indent(result, indent_level) << template_parameters_stack.size() << ": Popping: " << eval_params(parameters) << std::endl;

                template_parameters_stack.pop_back(); // FIX: Make this RAII
                return true;
            }
            else
            if (const ipr::Classic* q = ipr::util::view<ipr::Classic>(n.result()))
            {
                // The whole thing is a parameterized variable declaration with initializer
                // FIX: For some reasons result points to initializer
                indent(result, indent_level) << eval_type(n.result_type(), eval_name(d.name())) << " = " << eval_classic(*q) << ';';
            }
            else
            {
                // The whole thing is a parametrized class declaration
                indent(result, indent_level) << eval_type(n.result_type(), eval_name(d.name()));
                evaluate_expr(n.result(),result,indent_level);
            }
        }
        else
        {
            indent(result, indent_level) << eval_type(n.result_type(), eval_name(d.name()));

            if (d.has_initializer())
                result << " = " << eval_expr(d.initializer());

            //if (!m_part_of_expression)
                result << ';';
        }

        //indent(result, indent_level) << template_parameters_stack.size() << ": Popping: " << eval_params(parameters) << std::endl;

        template_parameters_stack.pop_back(); // FIX: Make this RAII
    }
    else
        assert(!"Invalid mapping");

    return false;
}

//------------------------------------------------------------------------------

} // of namespace cxxv
