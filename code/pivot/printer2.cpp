#include <deque>
#include <iomanip>
#include <ipr/utility>           // Some Pivot utilities

#include "printer.hpp"
#include "predicates.hpp"        // Helper predicates on IPR nodes
#include "strutils.hpp"          // String utilities
#include "precedence.hpp"
#include "match_ipr.hpp"
#include "timing.hpp"            // XTL timing library

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

class cxx_printer;

//------------------------------------------------------------------------------

cxx_printer& operator<<(cxx_printer& pr, const ipr::Stmt&);
cxx_printer& operator<<(cxx_printer& pr, const ipr::Decl&);
cxx_printer& operator<<(cxx_printer& pr, const ipr::Udt&);
cxx_printer& operator<<(cxx_printer& pr, const ipr::Expr&);
cxx_printer& operator<<(cxx_printer& pr, const ipr::Node&);
template <typename T> 
inline cxx_printer& operator<<(cxx_printer& pr, const ipr::Sequence<T>& s) 
{
    return pr << eval_sqnc(s); 
}

bool evaluate_map(const ipr::Mapping&, const ipr::Named_map&, cxx_printer&);

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

inline std::string postfix(const std::string& declarator)
{
    return !declarator.empty() && isid(declarator[0]) ? ' '+declarator : declarator;
}

inline std::string prefix(const std::string& declarator)
{
    return !declarator.empty() && isid(declarator[declarator.length()-1]) ? declarator+' ' : declarator;
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

//------------------------------------------------------------------------------

std::string eval_name(const ipr::Name& name)
{
    Match(name)
    {
        Case(ipr::Identifier,  str)    { return to_str(str); }
        Case(ipr::Id_expr,     name)   { return eval_name(name); }
        Case(ipr::Operator,    opname) { return "operator"  + to_str(opname); }
        Case(ipr::Conversion,  target) { return "operator " + eval_type(target); }
        Case(ipr::Scope_ref,   scope, member)    
        {
            std::string result = eval_expr(scope) + "::";

            if (const ipr::Template_id* p = ipr::util::view<ipr::Template_id>(member))
                if (ipr::is_template_dependent(scope))
                    result += "template ";

            result += eval_expr(member);
            return result;
        }
        Case(ipr::Template_id, name, arguments)
        {
            std::string args = eval_sqnc(arguments.elements());

            if (args.length() > 0 && args[args.length()-1] == '>')
                args += ' ';

            return eval_name(name) + '<' + args + '>';
        }
        Case(ipr::Ctor_name,   object_type) { return eval_type(object_type); }
        Case(ipr::Dtor_name,   object_type) { return "~" + eval_type(object_type); }
        Case(ipr::Rname,       level, position)
        {
            //std::cout << template_parameters_stack.size() << ": Accessing: R[" << i << ',' << j << ']' << std::endl;
            //assert(level    < template_parameters_stack.size()); // We should have processed corresponding Named_map and put parameters on the stack
            //assert(position < template_parameters_stack[level]->size()); // Parameter list of that level should be long enough to have such position
            // FIX: The above assertion crashes on use_stl_vector.cpp for probably an IPR bug
            //      Temporarily removed with the below condition:
            if (level    < template_parameters_stack.size() &&
                position < template_parameters_stack[level]->size())
                return eval_name(template_parameters_stack[level]->operator[](position).name());
            else
                return "BUGGY_IPR_RNAME_" + to_str(level) + '_' + to_str(position);
        }
        Case(ipr::Type_id,     type_expr)    { return eval_type(type_expr); }
    }
    EndMatch

    assert(!"Inexhaustive match"); // Your case clauses don't cover all the cases!
}

//------------------------------------------------------------------------------

std::string eval_type_ex(const ipr::Type& n, const std::string& declarator) 
{
    MatchP(n)
    {
        CaseP(ipr::Array,        element_type, bound) { return eval_type(element_type, declarator + '[' + eval_expr(bound)+ ']', ipr::precedence<ipr::Array>::value); }
        CaseP(ipr::Void          )  { return "void"               + postfix(declarator); }
        CaseP(ipr::Bool          )  { return "bool"               + postfix(declarator); }
        CaseP(ipr::Char          )  { return "char"               + postfix(declarator); }
        CaseP(ipr::sChar         )  { return "signed char"        + postfix(declarator); }
        CaseP(ipr::uChar         )  { return "unsigned char"      + postfix(declarator); }
        CaseP(ipr::Wchar_t       )  { return "wchar_t"            + postfix(declarator); }
        CaseP(ipr::Short         )  { return "short"              + postfix(declarator); }
        CaseP(ipr::uShort        )  { return "unsigned short"     + postfix(declarator); }
        CaseP(ipr::Int           )  { return "int"                + postfix(declarator); }
        CaseP(ipr::uInt          )  { return "unsigned int"       + postfix(declarator); }
        CaseP(ipr::Long          )  { return "long"               + postfix(declarator); }
        CaseP(ipr::uLong         )  { return "unsigned long"      + postfix(declarator); }
        CaseP(ipr::Long_long     )  { return "long long"          + postfix(declarator); }
        CaseP(ipr::uLong_long    )  { return "unsigned long long" + postfix(declarator); }
        CaseP(ipr::Float         )  { return "float"              + postfix(declarator); }
        CaseP(ipr::Double        )  { return "double"             + postfix(declarator); }
        CaseP(ipr::Long_double   )  { return "long double"        + postfix(declarator); }
        CaseP(ipr::Ellipsis      )  { return "..."; }
        CaseP(ipr::Decltype,     expr)  { return "decltype(" + eval_expr(expr) + ')'; }
        CaseP(ipr::Function,     source, target, throws)  
        { 
            std::string result = eval_type(target, declarator + '(' + eval_expr(source) + ')', ipr::precedence<ipr::Function>::value);

            if (!can_throw_everything(*matched)) // we skip throw specification when any exception can be thrown
                result += " throw(" + eval_expr(throws) + ')';

            return result;
        }
        CaseP(ipr::Pointer,      points_to)  { return eval_type(points_to, '*' + postfix(declarator), ipr::precedence<ipr::Pointer>::value); }
        CaseP(ipr::Reference,    refers_to)  { return eval_type(refers_to, '&' + postfix(declarator), ipr::precedence<ipr::Reference>::value); }
        CaseP(ipr::Ptr_to_member,containing_type, member_type) { return eval_type(member_type, eval_type(containing_type) + "::*" + postfix(declarator), ipr::precedence<ipr::Ptr_to_member>::value); }
        CaseP(ipr::Qualified,    main_variant, quals)
        {
            std::string qualifiers;

            if (quals & ipr::Type::Const)
                qualifiers += "const ";

            if (quals & ipr::Type::Volatile)
                qualifiers += "volatile ";

            if (quals & ipr::Type::Restrict)
    #ifdef _MSC_VER
                qualifiers += "__restrict ";
    #else
                qualifiers += "__restrict__ ";
    #endif

            return eval_type(main_variant, qualifiers + postfix(/*matched->main_variant()*/declarator), ::precedence(main_variant));
        }
    
        CaseP(ipr::Product,      elements)       { return eval_sqnc(elements); }
        CaseP(ipr::Sum,          elements)       { return eval_sqnc(elements); }
        CaseP(ipr::Template,     source, target) { return "template <" + eval_type(source) + "> " + eval_type(target, postfix(declarator)); }
        CaseP(ipr::As_type,      expr)           { return eval_expr(expr) + postfix(declarator); } // This may represent user-defined class
        CaseP(ipr::Udt,          name)           { return eval_name(name) + postfix(declarator); } // This is analog of Decl handling in expression context - we only refer the name
    }
    EndMatchP

    assert(!"Inexhaustive match"); // Your case clauses don't cover all the cases!
}

//------------------------------------------------------------------------------

inline std::string eval_type(const ipr::Type& n, const std::string& declarator, size_t decl_precedence) 
{
    std::string result = eval_type_ex(n, ::precedence(n) < decl_precedence ? '(' + declarator + ')' : declarator);

    if (ipr::is_template_dependent_type(n))
        result = "typename " + result;

    return result;
}

//------------------------------------------------------------------------------

std::string eval_classic(const ipr::Classic& n)
{
    Match(n)
    {
        Case(ipr::Literal, _, s)      { return to_str(s); }

        // Unary Operations

        Case(ipr::Address, e)         { return "&" + eval_classic_ex(e); }
        Case(ipr::Array_delete, e)    { return "delete[] " + eval_classic_ex(e); }
        Case(ipr::Complement, e)      { return "~" + eval_classic_ex(e); }
        Case(ipr::Delete, e)          { return "delete " + eval_classic_ex(e); }
        Case(ipr::Deref, e)           { return "*" + eval_classic_ex(e); }
        Case(ipr::Expr_sizeof, e)     { return "sizeof(" + eval_classic_ex(e) + ")"; }
        Case(ipr::Expr_typeid, e)     { return "typeid(" + eval_classic_ex(e) + ")"; }
        Case(ipr::Not, e)             { return "!" + eval_classic_ex(e); }
        Case(ipr::Paren_expr, e)      { return "(" + eval_classic_ex(e) + ")"; }
        Case(ipr::Post_decrement, e)  { return eval_classic_ex(e) + "--"; }
        Case(ipr::Post_increment, e)  { return eval_classic_ex(e) + "++"; }
        Case(ipr::Pre_decrement, e)   { return "--" + eval_classic_ex(e); }
        Case(ipr::Pre_increment, e)   { return "++" + eval_classic_ex(e); }
        Case(ipr::Throw, e)           { return "throw " + eval_classic_ex(e); }
        Case(ipr::Type_sizeof, e)     { return "sizeof(" + eval_classic_ex(e) + ")"; }
        Case(ipr::Type_typeid, e)     { return "typeid(" + eval_classic_ex(e) + ")"; }
        Case(ipr::Unary_minus, e)     { return "-" + eval_classic_ex(e); }
        Case(ipr::Unary_plus, e)      { return "+" + eval_classic_ex(e); }

        // Binary Operations

        Case(ipr::Plus, a, b)            { return eval_classic_ex(a) + " + "  + eval_classic_ex(b); }
        Case(ipr::Minus, a, b)           { return eval_classic_ex(a) + " - "  + eval_classic_ex(b); }
        Case(ipr::Mul, a, b)             { return eval_classic_ex(a) + " * "  + eval_classic_ex(b); }
        Case(ipr::Div, a, b)             { return eval_classic_ex(a) + " / "  + eval_classic_ex(b); }
        Case(ipr::Modulo, a, b)          { return eval_classic_ex(a) + " % "  + eval_classic_ex(b); }
        Case(ipr::Bitand, a, b)          { return eval_classic_ex(a) + " & "  + eval_classic_ex(b); }
        Case(ipr::Bitor, a, b)           { return eval_classic_ex(a) + " | "  + eval_classic_ex(b); }
        Case(ipr::Bitxor, a, b)          { return eval_classic_ex(a) + " ^ "  + eval_classic_ex(b); }
        Case(ipr::Lshift, a, b)          { return eval_classic_ex(a) + " << " + eval_classic_ex(b); }
        Case(ipr::Rshift, a, b)          { return eval_classic_ex(a) + " >> " + eval_classic_ex(b); }

        Case(ipr::Assign, a, b)          { return eval_classic_ex(a) + " = "   + eval_classic_ex(b); }
        Case(ipr::Plus_assign, a, b)     { return eval_classic_ex(a) + " += "  + eval_classic_ex(b); }
        Case(ipr::Minus_assign, a, b)    { return eval_classic_ex(a) + " -= "  + eval_classic_ex(b); }
        Case(ipr::Mul_assign, a, b)      { return eval_classic_ex(a) + " *= "  + eval_classic_ex(b); }
        Case(ipr::Div_assign, a, b)      { return eval_classic_ex(a) + " /= "  + eval_classic_ex(b); }
        Case(ipr::Modulo_assign, a, b)   { return eval_classic_ex(a) + " %= "  + eval_classic_ex(b); }
        Case(ipr::Bitand_assign, a, b)   { return eval_classic_ex(a) + " &= "  + eval_classic_ex(b); }
        Case(ipr::Bitor_assign, a, b)    { return eval_classic_ex(a) + " |= "  + eval_classic_ex(b); }
        Case(ipr::Bitxor_assign, a, b)   { return eval_classic_ex(a) + " ^= "  + eval_classic_ex(b); }
        Case(ipr::Lshift_assign, a, b)   { return eval_classic_ex(a) + " <<= " + eval_classic_ex(b); }
        Case(ipr::Rshift_assign, a, b)   { return eval_classic_ex(a) + " >>= " + eval_classic_ex(b); }

        Case(ipr::And, a, b)             { return eval_classic_ex(a) + " && "  + eval_classic_ex(b); }
        Case(ipr::Or, a, b)              { return eval_classic_ex(a) + " || "  + eval_classic_ex(b); }
        Case(ipr::Equal, a, b)           { return eval_classic_ex(a) + " == "  + eval_classic_ex(b); }
        Case(ipr::Not_equal, a, b)       { return eval_classic_ex(a) + " != "  + eval_classic_ex(b); }
        Case(ipr::Greater, a, b)         { return eval_classic_ex(a) + " > "   + eval_classic_ex(b); }
        Case(ipr::Greater_equal, a, b)   { return eval_classic_ex(a) + " >= "  + eval_classic_ex(b); }
        Case(ipr::Less, a, b)            { return eval_classic_ex(a) + " < "   + eval_classic_ex(b); }
        Case(ipr::Less_equal, a, b)      { return eval_classic_ex(a) + " <= "  + eval_classic_ex(b); }

        Case(ipr::Comma, a, b)           { return eval_classic_ex(a) + " , " + eval_classic_ex(b); }
        Case(ipr::Datum, c, s)           { return eval_classic_ex(c) + '('   + eval_sqnc(s.elements()) + ')'; }

        // Binary Operations: Casts

        Case(ipr::Cast, t, e)            { return                 "(" + eval_type(t) + ")"  + eval_classic_ex(e); }
        Case(ipr::Const_cast, t, e)      { return       "const_cast<" + eval_type(t) + ">(" + eval_classic_ex(e) + ")"; }
        Case(ipr::Dynamic_cast, t, e)    { return     "dynamic_cast<" + eval_type(t) + ">(" + eval_classic_ex(e) + ")"; }
        Case(ipr::Reinterpret_cast, t, e){ return "reinterpret_cast<" + eval_type(t) + ">(" + eval_classic_ex(e) + ")"; }
        Case(ipr::Static_cast, t, e)     { return      "static_cast<" + eval_type(t) + ">(" + eval_classic_ex(e) + ")"; }

        // Binary Operations: Member selection

        Case(ipr::Array_ref, b, m)       { return eval_classic_ex(b) + "["  + eval_classic_ex(m) + "]"; }    
        Case(ipr::Arrow, b, m)           
        {
            if (matched->has_impl_decl())
            {
                std::string result = eval_classic_ex(b) + '.' + eval_name(matched->impl_decl().name()) + "()";

                if (!is_phantom(m))
                    result += "->" + eval_classic_ex(m);

                return result;
            }
            else
                return eval_classic_ex(b) + "->" + eval_classic_ex(m); 
        }
        Case(ipr::Dot, b, m)             { return eval_classic_ex(b) + "."   + eval_classic_ex(m); }
        Case(ipr::Arrow_star, b, m)      { return eval_classic_ex(b) + "->*" + eval_classic_ex(m); }
        Case(ipr::Dot_star, b, m)        { return eval_classic_ex(b) + ".*"  + eval_classic_ex(m); }

        // Ternary operations

        Case(ipr::New, p, t, i)          { return "new " + eval_type(t); } // FIX: Take placement into account
        Case(ipr::Conditional, c, t, e)  { return eval_classic_ex(c) + " ? "  + eval_classic_ex(t) + " : " + eval_classic_ex(e); }

        // Other operations

        Case(ipr::Call, f, a)
        {
            std::string func_name = eval_classic_ex(f);

            if (const ipr::Fundecl* target = call_target(*matched)) // Call target is known
            {
                const ipr::Parameter_list& parameters = target->parameters();

                if (is_member_function(*target))
                {
                    // Call to a member function
                    assert(a.size() > 0); // There must be argument that represents this pointer

                    std::string result;

                    // Because of function call unification, calls to member functions
                    // are still represented as regular calls with this pointer passed
                    // explicitly as the first argument. The original call might have
                    // been on reference to object, in which case an ipr::Address node
                    // is artificially inserted. Here we are trying to undo that 
                    // unnecessary address to print the dot when possible.
                    if (const ipr::Address* addr = ipr::util::view<ipr::Address>(a[0]))
                    {
                        // The address is artifical and the actual call is through dot
                        result = eval_classic_ex(addr->operand()) + '.';
                    }
                    else
                    {
                        // The actual call is through this pointer
                        result = eval_classic_ex(a[0]) + "->";
                    }

                    // Note that using target->name() instead of func_name might omit scope etc.
                    result += func_name + '(' + eval_sqnc(a.elements(), 1) + ')';
                    return result;
                }
                else // not a member function
                {
                    // Call to a regular function
                    return func_name + '(' + eval_sqnc(a.elements()) + ')';
                }
            }
            else // Call target is not known
            {
                if (const ipr::Function* ft = ipr::util::view<ipr::Function>(f.type()))
                    if (is_member_function_type(*ft))
                    {
                        // Call to a member function
                        assert(a.size() > 0); // There must be argument that represents this pointer

                        std::string result;

                        // Because of function call unification, calls to member functions
                        // are still represented as regular calls with this pointer passed
                        // explicitly as the first argument. The original call might have
                        // been on reference to object, in which case an ipr::Address node
                        // is artificially inserted. Here we are trying to undo that 
                        // unnecessary address to print the dot when possible.
                        if (const ipr::Address* addr = ipr::util::view<ipr::Address>(a[0]))
                        {
                            // The address is artifical and the actual call is through dot
                            result = eval_classic_ex(addr->operand()) + '.';
                        }
                        else
                        {
                            // The actual call is through this pointer
                            result = eval_classic_ex(a[0]) + "->";
                        }

                        // Note that using target->name() instead of func_name might omit scope etc.
                        result += func_name + '(' + eval_sqnc(a.elements(), 1) + ')';
                        return result;
                    }

                return func_name + '(' + eval_sqnc(a.elements()) + ')';
            }
        }
    }
    EndMatch

    assert(!"Inexhaustive match"); // Your case clauses don't cover all the cases!
}

//==============================================================================
// cxx_printer
//==============================================================================

/// Class that holds context needed to pass around while printing IPR
class cxx_printer
{
    const std::ostream& base() const { return m_os; }
          std::ostream& base()       { return m_os; }
public:

    cxx_printer(
        std::ostream&       os, 
        bool                part_of_expression
    ) :
        m_os(os),
        m_part_of_expression(part_of_expression),
        m_indent_level(0),
        m_commenter(0)
    {}

    cxx_printer(
        std::ostream&       os,
        int                 indent_level = 0
    ) :
        m_os(os),
        m_part_of_expression(false),
        m_indent_level(indent_level),
        m_commenter(0)
    {}
    
    cxx_printer(
        std::ostream&       os,
        comments_interface& c,
        int                 indent_level = 0
    ) :
        m_os(os),
        m_part_of_expression(false),
        m_indent_level(indent_level),
        m_commenter(&c)
    {}

    //cxx_printer& operator<<(cxx_printer& (*pfn)(cxx_printer&)) { base() << pfn; return *this; }
    //cxx_printer& operator<<(basic_ios<char_type, traits_type>& (*pfn)(basic_ios<char_type, traits_type>&)) { base() << pfn; return *this; }
    //cxx_printer& operator<<(ios_base& (*pfn)(ios_base&)) { base() << pfn; return *this; }

    friend cxx_printer& operator<<(cxx_printer& pr, char c)                 { pr.base() << c;   return pr; }
    friend cxx_printer& operator<<(cxx_printer& pr, const char* str)        { pr.base() << str; return pr; }
    friend cxx_printer& operator<<(cxx_printer& pr, const std::string& str) { pr.base() << str; return pr; }

    //template <typename T> friend cxx_printer& operator<<(cxx_printer& pr, const T& t) { pr.base() << t; return pr; }

    friend cxx_printer& operator<<(cxx_printer& pr, cxx_printer& (&manipulator)(cxx_printer&)) { return manipulator(pr); }

    bool is_part_of_expression() const { return m_part_of_expression; }

    friend cxx_printer&    increase(cxx_printer& cxx) { ++cxx.m_indent_level; return cxx; }
    friend cxx_printer&    decrease(cxx_printer& cxx) { --cxx.m_indent_level; return cxx; }
    friend cxx_printer& indentation(cxx_printer& cxx) { return cxx.indent(); }

//protected:

    // A syntactic sugar to show the intent that we would like to forward
    // evaluation to a subcomponent.
    template <typename IPRNode>
    void forward_to(const IPRNode& n, bool indent = true)
    {
        if (indent)
            ++m_indent_level;

        *this << n;

        if (indent)
            --m_indent_level;
    }

    template <class T>
    void sequence_forward_to(const ipr::Sequence<T>& s, bool indent = true)
    {
        for (typename ipr::Sequence<T>::iterator p = s.begin(); p != s.end(); ++p)
            forward_to(*p, indent);
    }

    cxx_printer& indent(bool empty_line = false)
    {
        if (!m_part_of_expression)
        {
            base() << std::endl;

            if (empty_line)
                base() << std::endl;

            if (m_indent_level)
                base() << std::setw(m_indent_level*4) << ' ';
        }

        return *this;
    }

    cxx_printer& before(const ipr::Stmt& s, bool empty_line = false)
    {
        // FIX: Add support for printing a line that shows which file the source
        //      is coming from till that file becomes different.
        if (m_part_of_expression)
            return *this;

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

    void after(const ipr::Stmt& s, bool new_line = false)
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

                base() << (new_line ? "//<- " : " //<- ") << *p;
                new_line = true;
            }
        }
    }

private:

    /// The actual stream we are wrapping
    std::ostream& m_os;

    /// Indicates that corresponding statement is printed as part of expression
    /// and thus trailing ; as well as new-lines and comments should be avoided.
    const bool m_part_of_expression;

    /// A level of indentation currently used.
    int m_indent_level;

    /// A user-supplied object used to generate comments to the source
    comments_interface* m_commenter;

};

//------------------------------------------------------------------------------

cxx_printer& operator<<(cxx_printer& pr, const ipr::Stmt& stmt)
{
    Match(stmt)
    {
        Case(ipr::Labeled_stmt, lbl, stm)         { pr.before(*matched) << (is_case_label(lbl) ? "case " : "") << eval_expr(lbl) << ": "; pr.forward_to(stm, false/*, !ipr::util::view<ipr::Labeled_stmt>(stm)*/); pr.after(*matched); break; }
        Case(ipr::Block, body, handlers)
        {
            if (handlers.size())
            {
                pr.before(*matched) << "try";
                pr.indent()  << '{';
            }
            else
                pr.before(*matched) << '{';

            // Print statements in the body
            pr.sequence_forward_to(body);

            pr.indent() << '}';

            // Print catch handlers
            pr.sequence_forward_to(handlers, false);

            pr.after(*matched,true);
            break;
        }
        Case(ipr::Ctor_body,    inits, block)               { if (inits.size()) pr << " : " << eval_sqnc(inits.elements()); pr.forward_to(block,false); break; }
        Case(ipr::If_then,      cond, then_stmt)            { pr.before(*matched,true) << "if (" << eval_classic_ex(cond) << ")"; pr.forward_to(then_stmt); pr.after(*matched,true); break; }
        Case(ipr::If_then_else, cond, then_stmt, else_stmt) { pr.before(*matched,true) << "if (" << eval_classic_ex(cond) << ")"; pr.forward_to(then_stmt); pr.indent() << "else"; pr.forward_to(else_stmt); pr.after(*matched,true); break; }
        Case(ipr::Switch,       cond, body)                 { pr.before(*matched,true) << "switch (" << eval_classic_ex(cond) << ')'; pr.forward_to(body,false); pr.after(*matched,true); break; }
        Case(ipr::While,        cond, body)                 { pr.before(*matched,true) << "while (" << eval_classic_ex(cond) << ")"; pr.forward_to(body); pr.after(*matched,true); break; }
        Case(ipr::Do,           cond, body)                 { pr.before(*matched,true) << "do "; pr.forward_to(body); pr.indent() << "while (" << eval_classic_ex(cond) << ");"; pr.after(*matched,true); break; }
        Case(ipr::For,          init, cond, incr, body)     { pr.before(*matched,true) << "for (" << eval_classic_ex(init) << ";" << eval_classic_ex(cond) << ";" << eval_classic_ex(incr) << ")"; pr.forward_to(body); pr.after(*matched,true); break; }
        Case(ipr::Break)                                    { pr.before(*matched)      << "break;"; pr.after(*matched); break; }
        Case(ipr::Continue)                                 { pr.before(*matched)      << "continue;"; pr.after(*matched); break; }
        Case(ipr::Goto,         target)
        {
            const ipr::Identifier* q = ipr::util::view<ipr::Identifier>(target);
            assert(q); // When this asserts, ipr::Goto stopped pointing to just Identifier
            pr.before(*matched) << "goto " << (q ? eval_name(*q) : "???") << ";";
            pr.after(*matched);
            break;
        }
        Case(ipr::Return,       value)                      { pr.before(*matched) << "return " << eval_classic_ex(value) << ";"; pr.after(*matched); break; }
        Case(ipr::Handler,      expt, body)                 { pr.before(*matched) << "catch(" << eval_decl(expt) << ')'; pr.forward_to(body,false); pr.after(*matched); break; }
        Case(ipr::Empty_stmt)                               { pr.before(*matched) << ";"; pr.after(*matched); break; }
        Case(ipr::Expr_stmt,    expr)                       { pr.before(*matched) << eval_classic_ex(expr) << ";"; pr.after(*matched); break; }

        // Declarations
        Case(ipr::Decl)                                     { pr << *matched; break; }
    }
    EndMatch

    return pr;
}

//------------------------------------------------------------------------------

cxx_printer& operator<<(cxx_printer& pr, const ipr::Decl& decl)
{
    Match(decl)
    {
        Case(ipr::Typedecl, name, type)
        {
            pr.before(*matched,true) << prefix_specifiers(*matched) << eval_type(type, eval_name(name));

            if (is_namespace(*matched))
            {
                assert(matched->has_initializer()); // Namespaces cannot be forward declared
                // initializer() would be a Namespace node, to which we forward traversal
                const ipr::Namespace* ns = ipr::util::view<ipr::Namespace>(matched->initializer());  // dynamic_cast alternative
                assert(ns);
                pr.forward_to(*ns, false);
            }
            else
            {
                if (matched->has_initializer())
                    pr.forward_to(matched->initializer(), false);
                else
                if (!pr.is_part_of_expression())
                    pr << ';';
            }

            pr.after(*matched,true); // Print comment after the type declaration/namespace
            break;
        }

        Case(ipr::Fundecl, name, type, membership, parameters) // FIX: membership is not used here!
        {
            pr.before(*matched,true) << prefix_specifiers(*matched);

            // NOTE: For some reason currently virtual member functions are not marked with
            //       virtual specifier on the Fundecl but instead with the virtual specifier
            //       on the first parameter to account for later support of multi-methods.
            if (is_virtual_member_function(*matched) && is_in_class_declaration(*matched))
                pr << "virtual ";

            // Evaluate declarator that will be used to output the p type that can
            // be complicated as function can return arrays or function pointers
            //std::string declarator = eval_name(name) + '(' + eval_params(parameters) + ')';

            std::string declarator = get_declarator(*matched) + '(' + eval_params(parameters) + ')';

            // Prepend class name to the name when declaration/definition is made outside
            // of class and refers to a member function
            //if (is_member(*matched) && !is_in_class_declaration(*matched))
            //    declarator = eval_name(membership.name()) + "::" + declarator; // FIX: This won't work for general case of namespaces, specializations etc., just a quick hack
            //if (is_member_function(*matched) && !is_in_class_declaration(*matched))
            //    declarator = eval_name(membership.name()) + "::" + declarator; // FIX: This won't work for general case of namespaces, specializations etc., just a quick hack

            const ipr::Function* ft = ipr::util::view<ipr::Function>(type);  // dynamic_cast alternative
            assert(ft);

            // Print the name and return type when applicable
            if (!ipr::is_constructor(*matched) && !ipr::is_destructor(*matched) && !ipr::is_conversion(*matched))
                pr << eval_type(ft->target(), declarator);
            else
                pr << declarator;

            // Check if this is a const member function
            if (is_const_member_function(*matched))
                pr << " const";

            // Print throw specification if the function has it
            if (!can_throw_everything(*ft)) // we skip throw specification when any exception can be thrown
                pr << " throw(" + eval_type(ft->throws()) + ')';

            // Check if this is a pure virtual function (abstract method)
            if (matched->specifiers() & ipr::Decl::Pure)
                pr << " = 0";

            // Print body if present
            if (matched->has_initializer())
            {
                const ipr::Stmt* body = ipr::util::view<ipr::Stmt>(matched->initializer());
                assert(body);
                pr << *body;
            }
            else
            if (!pr.is_part_of_expression())
                pr << ';';

            pr.after(*matched,true); // Print comment after the function definition
            break;
        }

        Case(ipr::Alias, name, type)
        {
            pr.before(*matched,true) << prefix_specifiers(*matched);

            assert(matched->has_initializer());

            const ipr::Expr& init = matched->initializer();

	        // This is the case of: typedef T U;
            if (const ipr::Type* q = ipr::util::view<ipr::Type>(init))
            {
                pr << /*"typedef " <<*/ eval_type(*q,/*get_declarator(*matched)*/eval_name(name)) << ';'; // Since EDG front-end now sets up the Typedef specifier on ipr::Decl
                return pr;
            }

	        // This is the case of: namespace N = A::B::C::D;
            if (const ipr::Id_expr* id = ipr::util::view<ipr::Id_expr>(init))
            {
                if (const ipr::Typedecl* td = ipr::util::view<ipr::Typedecl>(id->resolution()))
                    if (ipr::is_namespace(*td))
                    {
                        pr << "namespace " << eval_name(name) << '=' << eval_name(id->name()) << ';';
                        return pr;
                    }
            }

	        // This is the case of: using namespace A::B::C::D;
            if (const ipr::Typedecl* d = ipr::util::view<ipr::Typedecl>(init))
            {
                if (is_namespace(*d))
                {
                    pr << "using namespace " << eval_expr(matched->initializer()) << ';';
                    return pr;
                }
            }

	        pr << "using " << get_declarator(*matched,true) << ';';
            pr.after(*matched);
            break;
        }

        Case(ipr::Bitfield, name, type, membership, precision) // FIX: membership is not used here!
        {
            pr.before(*matched) << prefix_specifiers(*matched) << eval_type(type, eval_name(name)) << ':' << eval_classic_ex(precision);

            if (matched->has_initializer())
                pr << " = " << eval_classic_ex(matched->initializer());

            if (!pr.is_part_of_expression())
                pr << ';';

            pr.after(*matched);
            break;
        }

        Case(ipr::Enumerator, name)
        {
            pr.before(*matched) << prefix_specifiers(*matched) << eval_name(name);

            if (matched->has_initializer())
                pr << " = " << eval_classic_ex(matched->initializer());

            pr.after(*matched);
            break;
        }

        Case(ipr::Named_map, name, type, parameters, mapping) // FIX: Only parameters is used here
        {
            const ipr::Decl::Specifier prefix_mask = ipr::Decl::AccessProtection
                                  | ipr::Decl::StorageClass
                                  | ipr::Decl::Friend
                                  | ipr::Decl::Typedef
                                  | ipr::Decl::Constexpr
                                  | ipr::Decl::Export;

            //pr.indent() << template_parameters_stack.size() << ": Pushing: " << eval_params(parameters) << std::endl;

            //template_parameters_stack.push_back(&parameters); // Make current set of parameters available for referring to by Rname nodes

            // Print comment before the function definition
            pr.before(*matched,true) << prefix_specifiers(*matched, prefix_mask)/* 
                           << "template <" << eval_params(parameters) << '>';
            pr.indent()       << prefix_specifiers(*matched, ipr::Decl::FunctionSpecifier) 
                           << eval_type(mapping.result_type(), eval_name(name))*/;

            //if (matched->has_initializer())
            //    pr.forward_to(matched->initializer(), false);

            evaluate_map(mapping, *matched, pr);

            //template_parameters_stack.pop_back(); // FIX: Make this RAII

            //pr.indent() << template_parameters_stack.size() << ": Popping: " << eval_params(parameters) << std::endl;

            pr.after(*matched,true); // Print comment after the function definition
            break;
        }

        Case(ipr::Decl)
        {
            std::string declarator = get_declarator(*matched); //eval_name(matched->name());
        /*
            if (is_member(*matched) && !is_in_class_declaration(*matched))
            {
                const ipr::Udt* udt = ipr::util::view<ipr::Udt>(matched->home_region().owner());
                assert(udt); // Because is_member implies this
                declarator = eval_name(udt->name()) + "::" + declarator; // FIX: This won't work for general case of namespaces, specializations etc., just a quick hack
            }
        */
            pr.before(*matched) << prefix_specifiers(*matched) << eval_type(matched->type(), declarator);

            if (matched->has_initializer())
                pr << " = " << eval_expr(matched->initializer());

            if (!pr.is_part_of_expression())
                pr << ';';

            pr.after(*matched);
            break;
        }

        Case(ipr::Expr)         { pr << eval_expr(*matched); break; } // TODO: Discuss why we had it in visitors!
    }
    EndMatch

    return pr;
}

//------------------------------------------------------------------------------

cxx_printer& operator<<(cxx_printer& pr, const ipr::Udt& udt)
{
    Match(udt)
    {
        Case(ipr::Global_scope, members)
        {
            pr.sequence_forward_to(members, false);
            break;
        }

        Case(ipr::Namespace, members)
        {
            pr.indent() << '{';
            pr.sequence_forward_to(members);
            pr.indent() << '}';
            break;
        }

        Case(ipr::Class, members, bases)
        {
            // Print base classes
            if (bases.size())
            {
                pr << " : ";

                for (ipr::Sequence<ipr::Base_type>::iterator p = bases.begin(); p != bases.end(); ++p)
                {
                    if (p != bases.begin())
                        pr << ", ";

                    pr << eval_name(p->name());
                }
            }

            pr.indent() << '{';
            pr.sequence_forward_to(members);
            pr.indent() << '}';

            if (!pr.is_part_of_expression())
                pr << ';';
            break;
        }

        Case(ipr::Union, members)
        {
            pr.indent() << '{';
            pr.sequence_forward_to(members);
            pr.indent() << '}';

            if (!pr.is_part_of_expression())
                pr << ';';
            break;
        }

        Case(ipr::Enum, members)
        {
            pr.indent() << '{';

            for (ipr::Sequence<ipr::Enumerator>::iterator p = members.begin(); p != members.end(); ++p)
            {
                if (p != members.begin())
                    pr << ',';

                pr.forward_to(*p);
            }

            pr.indent() << '}';

            if (!pr.is_part_of_expression())
                pr << ';';
            break;
        }
    }
    EndMatch

    return pr;
}

//------------------------------------------------------------------------------

cxx_printer& operator<<(cxx_printer& pr, const ipr::Expr& e)
{
    Match(e)
    {
        Case(ipr::Mapping)
        {
            if (ipr::util::node_has_member(*matched, &ipr::Mapping::result))
                pr << matched->result();
            else
                pr << ';';
            break;
        }
        Case(ipr::Overload) { break; }
        Case(ipr::Scope, decls)
        {
            // Scope can be in place of expression when expression is also a declaration
            // e.g. if (int i = foo()) {} or for (int i = 0, j = 0; ;);

            // This is a tricky case because first we have to generate initializers and
            // then also compute the operand that initializer resolves to as for example:
            // if (int i = 0, j = 1) ...

            for (ipr::Sequence<ipr::Decl>::iterator p = decls.begin(); p != decls.end(); ++p)
            {
                std::string s = eval_decl(*p); // Evaluate declaration without trailing ;

                if (p != decls.begin())
                    pr << ',';

                pr << s;
            }

            break;
        }

        Case(ipr::Classic)   { pr << eval_classic(*matched); break; }
        Case(ipr::Name)      { pr << eval_name(*matched); break; }
        Case(ipr::Phantom)   { break; }
        Case(ipr::Expr_list, operand) { pr << '{' << eval_sqnc(operand) << '}'; break; }
        Case(ipr::Label)     { break; }
        Case(ipr::Member_init, member, initializer) 
        {
            // FIX: This is workaround for now as it should become Expr_list statically
            if (const ipr::Expr_list* p = ipr::util::view<ipr::Expr_list>(initializer))
                pr << eval_expr(member)+'('+eval_sqnc(p->elements())+')';
            else
                pr << eval_expr(member)+'('+eval_classic_ex(initializer)+')'; 
            break;
        }

        // The following kinds of nodes are not directly inherited from Expr so 
        // technically they shouldn't be handled here. However they may happen in 
        // context were traditional expressions are expected and those cases are
        // covered here.

        // Declaration might happen in expression context when IPR refers to the use
        // of the name but whoever generated IPR forgot to mark such use properly with
        // Id_expr and pointed directly to the declaration instead.
        Case(ipr::Decl, name) { pr.forward_to(name,false); break; }

        // Expr_stmt can happen in the initializer of the following loop:
        // int i; for (i=1; i < 10; i++)
        // because the scope won't be introduced. It seems to be easier at the
        // moment to simply handle this case here rather then all of the
        // cases of ClassicExprPrintVisitor inside StatementPrintVisitor when
        // forwarding evaluation to the initializer.
        Case(ipr::Expr_stmt, expr) { pr.forward_to(expr,false); break; }

        // And this hack counter measure the bugs introduced by the above Udt hack
        // as references to Namespace should be just treated as names and not dumping
        // of their content that the above hack will do.
        Case(ipr::Namespace) { pr.forward_to(matched->name(),false); break; }

        // This shouldn't be here at all, but it'*matched a quick hack right now to handle
        // template class definitions as mapping will forward here.
        Case(ipr::Udt) { pr << *matched; break; }

        Case(ipr::Type)      { pr << eval_type(*matched); break; }
        Case(ipr::Stmt)      { pr << *matched; break; }
    }
    EndMatch

    return pr;
}

//------------------------------------------------------------------------------

cxx_printer& operator<<(cxx_printer& pr, const ipr::Node& n)
{
    Match(n)
    {
        Case(ipr::Unit, global_scope) { pr << global_scope; break; } // This should forward to visitation of Global_scope, which is a Namespace
        Case(ipr::Udt)  { pr << *matched; break; }
        Case(ipr::Decl) { pr << *matched; break; }
        Case(ipr::Stmt) { pr << *matched; break; }
        Case(ipr::Type) { pr << eval_type(*matched); break; }
        Case(ipr::Name) { pr << eval_name(*matched); break; }
        Case(ipr::Expr) { pr << *matched; break; }
    }
    EndMatch

    return pr;
}

//------------------------------------------------------------------------------

void print_cpp(const ipr::Node& n, std::ostream& os)
{
    time_stamp liStart  = get_time_stamp();

    cxx_printer cxx(os);
    cxx << n << "\n"; //std::endl; FIX: Make iomanipulators work with printer

    time_stamp liFinish = get_time_stamp();
    std::clog << "Time: " << liFinish-liStart << std::endl;
}

//------------------------------------------------------------------------------

void print_cpp(const ipr::Node& n, std::ostream& os, comments_interface& c)
{
    time_stamp liStart  = get_time_stamp();

    cxx_printer cxx(os,c);
    cxx << n << "\n"; //std::endl; FIX: Make iomanipulators work with printer

    time_stamp liFinish = get_time_stamp();
    std::clog << "Time: " << liFinish-liStart << std::endl;
}

//------------------------------------------------------------------------------

std::string eval_expr(const ipr::Expr& n)                                          
{
    // Because in IPR everything is Expr, we have to figure out first what are 
    // we dealing with. To do this, we forward it to general dispatcher of Expr,
    // and we assume here that only the cases that won't print newlines etc. can
    // happen there.

    std::stringstream ss;
    cxx_printer cxx(ss);
    cxx << n;
    return ss.str();
}

//------------------------------------------------------------------------------

std::string eval_decl(const ipr::Decl& n)
{
    std::stringstream ss;
    cxx_printer cxx(ss,true);
    cxx << n;
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

bool evaluate_map (const ipr::Mapping& n, const ipr::Named_map& d, cxx_printer& pr)
{
    if (const ipr::Function* ft = ipr::util::view<ipr::Function>(n.type()))
    {
        // Mapping represents regular function

        pr << prefix_specifiers(d, ipr::Decl::FunctionSpecifier);

        const ipr::Parameter_list& parameters = n.params();

        // NOTE: For some reason currently virtual member functions are not marked with
        //       virtual specifier on the Fundecl but instead with the virtual specifier
        //       on the first parameter to account for later support of multi-methods.
        if (is_virtual_member_function(n) && is_in_class_declaration(d))
            pr << "virtual ";

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
            pr << eval_type(ft->target(), declarator);
        else
            pr << declarator;

        // Check if this is a const member function
        if (is_const_member_function(n))
            pr << " const";

        // Print throw specification if the function has it
        if (!can_throw_everything(*ft)) // we skip throw specification when any exception can be thrown
            pr << " throw(" + eval_type(ft->throws()) + ')';

        // Check if this is a pure virtual function (abstract method)
        if (d.specifiers() & ipr::Decl::Pure)
            pr << " = 0";
    }
    else
    if (ipr::util::view<ipr::Template>(n.type()))
    {
        // Mapping represents a template

        const ipr::Parameter_list& parameters = n.params();

        template_parameters_stack.push_back(&parameters); // Make current set of parameters available for referring to by Rname nodes

        //std::cout << template_parameters_stack.size() << ": Pushing: " << eval_params(parameters) << std::endl;
        //pr.indent();

        std::string args = eval_params(parameters);

        if (args.length() > 0 && args[args.length()-1] == '>')
            args += ' ';

        // Print name and formal arguments
        pr << "template <" << args << '>';

        if (ipr::util::node_has_member(n, &ipr::Mapping::result))
        {
            if (const ipr::Mapping* q = ipr::util::view<ipr::Mapping>(n.result()))
            {
                // The whole thing is a parametrized function
                pr.indent();

                // Forward to ourselves to print declaration
                // NOTE: In case of nested templates, all their result will point
                //       to the same body, but only the last one is allowed to
                //       print as only it know all the Rnames
                // We essentially check here if body has already been printed by
                // the recursive call.
                if (!evaluate_map(*q, d, pr))
                    if (ipr::util::node_has_member(*q, &ipr::Mapping::result))
                        pr << q->result(); // The result is going to be the body of the parameterized function
                    else
                        pr << ';'; // Parameterized function forward declaration

                //pr.indent() << template_parameters_stack.size() << ": Popping: " << eval_params(parameters) << std::endl;

                template_parameters_stack.pop_back(); // FIX: Make this RAII
                return true;
            }
            else
            if (const ipr::Classic* q = ipr::util::view<ipr::Classic>(n.result()))
            {
                // The whole thing is a parameterized variable declaration with initializer
                // FIX: For some reasons result points to initializer
                pr.indent() << eval_type(n.result_type(), eval_name(d.name())) << " = " << eval_classic(*q) << ';';
            }
            else
            {
                // The whole thing is a parametrized class declaration
                pr.indent() << eval_type(n.result_type(), eval_name(d.name()));
                pr << n.result();
            }
        }
        else
        {
            pr.indent() << eval_type(n.result_type(), eval_name(d.name()));

            if (d.has_initializer())
                pr << " = " << eval_expr(d.initializer());

            //if (!m_part_of_expression)
                pr << ';';
        }

        //pr.indent() << template_parameters_stack.size() << ": Popping: " << eval_params(parameters) << std::endl;

        template_parameters_stack.pop_back(); // FIX: Make this RAII
    }
    else
        assert(!"Invalid mapping");

    return false;
}

//------------------------------------------------------------------------------
