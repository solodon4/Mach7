// DO NOT INCLUDE THIS FILE!

/// \note Content of this file is textually the same as the content of 
///       printer_visitors.hpp with the exception of the namespace's name.
///       This is intentional as we would like to have both visitors and 
///       pattern-matching implementations in the same library, without mixing
///       their implementations. 
///       The definitions in this file do not depend on whether the functions 
///       they call are using visitors or pattern matching, but in order to be
///       able to compare the two independently, we have to make sure only one
///       set of functions is called.

#include "printer_common.hpp"
#include <deque>
#include <ipr/utility>           // Some Pivot utilities

/// Pattern-matching version of C++ printer
namespace cxxm {

//------------------------------------------------------------------------------

/// Set of template parameters with corresponding nesting of the currently
/// processed template to be resolved for Rname nodes.
std::deque<const ipr::Parameter_list*> template_parameters_stack; // FIX: Make this thread local somehow

//------------------------------------------------------------------------------

std::string eval_name(const ipr::Name&);
std::string eval_type(const ipr::Type&, const std::string& = "", size_t = 0);
std::string eval_classic(const ipr::Classic&);
std::string eval_decl(const ipr::Decl&);
std::string eval_expr(const ipr::Expr&);
std::string eval_params(const ipr::Parameter_list& parameters);

//------------------------------------------------------------------------------

class cxx_printer : public cxx_printer_of<cxx_printer>
{
    typedef cxx_printer_of<cxx_printer> base_type;

public:

    /// Forward all constructors to the base class
    cxx_printer(std::ostream& os, bool part_of_expression)                     : base_type(os,part_of_expression) {}
    cxx_printer(std::ostream& os, int  indent_level = 0)                       : base_type(os,indent_level)       {}
    cxx_printer(std::ostream& os, comments_interface& c, int indent_level = 0) : base_type(os,c,indent_level)     {}

    using base_type::operator<<;

    cxx_printer& operator<<(const ipr::Classic&        n) { return *this << eval_classic(n); }
    cxx_printer& operator<<(const ipr::Name&           n) { return *this << eval_name(n);    }
    cxx_printer& operator<<(const ipr::Type&           n) { return *this << eval_type(n);    }
    cxx_printer& operator<<(const ipr::Parameter_list& n) { return *this << eval_params(n);  }
    cxx_printer& operator<<(const ipr::Stmt&);
    cxx_printer& operator<<(const ipr::Decl&);
    cxx_printer& operator<<(const ipr::Udt&);
    cxx_printer& operator<<(const ipr::Expr&);
    cxx_printer& operator<<(const ipr::Node&);

};

#if defined(__GNUC__)
/// FIX: For some reasone these are not picked from the base class, figure out why
    cxx_printer&    increase(cxx_printer& cxx) { ++cxx.m_indent_level; return cxx; }
    cxx_printer&    decrease(cxx_printer& cxx) { --cxx.m_indent_level; return cxx; }
    cxx_printer& indentation(cxx_printer& cxx) { return cxx.indent(); }
    cxx_printer&  empty_line(cxx_printer& cxx) { return cxx.indent(true); }
    cxx_printer&   semicolon(cxx_printer& cxx) { if (!cxx.is_part_of_expression()) cxx << ';'; return cxx; }
#endif

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

bool evaluate_map (const ipr::Mapping& n, const ipr::Named_map& d, cxx_printer& result)
{
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
        //result.indent();

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
                result.indent();

                // Forward to ourselves to print declaration
                // NOTE: In case of nested templates, all their result will point
                //       to the same body, but only the last one is allowed to
                //       print as only it know all the Rnames
                // We essentially check here if body has already been printed by
                // the recursive call.
                if (!evaluate_map(*q, d, result))
                    if (ipr::util::node_has_member(*q, &ipr::Mapping::result))
                        result << q->result(); // The result is going to be the body of the parameterized function
                    else
                        result << ';'; // Parameterized function forward declaration

                //result.indent() << template_parameters_stack.size() << ": Popping: " << eval_params(parameters) << std::endl;

                template_parameters_stack.pop_back(); // FIX: Make this RAII
                return true;
            }
            else
            if (const ipr::Classic* q = ipr::util::view<ipr::Classic>(n.result()))
            {
                // The whole thing is a parameterized variable declaration with initializer
                // FIX: For some reasons result points to initializer
                result.indent() << eval_type(n.result_type(), eval_name(d.name())) << " = " << eval_classic(*q) << ';';
            }
            else
            {
                // The whole thing is a parametrized class declaration
                result.indent() << eval_type(n.result_type(), eval_name(d.name()));
                result << n.result();
            }
        }
        else
        {
            result.indent() << eval_type(n.result_type(), eval_name(d.name()));

            if (d.has_initializer())
                result << " = " << eval_expr(d.initializer());

            //if (!m_part_of_expression)
                result << ';';
        }

        //result.indent() << template_parameters_stack.size() << ": Popping: " << eval_params(parameters) << std::endl;

        template_parameters_stack.pop_back(); // FIX: Make this RAII
    }
    else
        assert(!"Invalid mapping");

    return false;
}

//------------------------------------------------------------------------------

void print_cpp(const ipr::Node& n, std::ostream& os)
{
    cxx_printer cxx(os);
    cxx << n << "\n"; //std::endl; FIX: Make iomanipulators work with printer
}

//------------------------------------------------------------------------------

void print_cpp(const ipr::Node& n, std::ostream& os, comments_interface& c)
{
    cxx_printer cxx(os,c);
    cxx << n << "\n"; //std::endl; FIX: Make iomanipulators work with printer
}

//------------------------------------------------------------------------------

} // of namespace cxxm
