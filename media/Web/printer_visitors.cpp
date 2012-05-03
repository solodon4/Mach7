///
/// \file printer_visitors.cpp
///
/// Implementation of a C++ pretty-printer for Pivot based on visitor design pattern
///
/// \author Yuriy Solodkyy
/// Copyright (C) 2011, Texas A&M University.  All rights reserved.
///

#include "printer_visitors.hpp"
#include "precedence.hpp"
#include "visitor_base.hpp"      // various IPR visitor base classes

/// Visitors version of C++ printer
namespace cxxv
{

//------------------------------------------------------------------------------

std::string eval_name(const ipr::Name& name)
{ 
    struct NamePrintVisitor : ipr::assertive_visitor_base<std::string> ///< Visitor that prints a name.
    {
        virtual void visit(const ipr::Identifier&  e) { result = to_str(e.string()); }
        virtual void visit(const ipr::Id_expr&     e) { result = eval_name(e.name()); }
        virtual void visit(const ipr::Operator&    e) { result = "operator" + postfix(to_str(e.opname())); }
        virtual void visit(const ipr::Conversion&  e) { result = "operator" + postfix(eval_type(e.target())); }
        virtual void visit(const ipr::Scope_ref&   e) 
        {
            result = eval_expr(e.scope()) + "::";

            if (const ipr::Template_id* p = ipr::util::view<ipr::Template_id>(e.member()))
                if (ipr::is_template_dependent(e.scope()))
                    result += "template ";

            result += eval_expr(e.member());
        }
        virtual void visit(const ipr::Template_id& e) 
        {
            std::string args = eval_sqnc(e.args().elements());

            if (args.length() > 0 && args[args.length()-1] == '>')
                args += ' ';

            result = eval_name(e.template_name()) + '<' + args + '>';
        }
        virtual void visit(const ipr::Ctor_name&   e) { result = eval_type(e.object_type()); }
        virtual void visit(const ipr::Dtor_name&   e) { result = '~' + eval_type(e.object_type()); }
        virtual void visit(const ipr::Rname&       e) 
        {
            if (e.level()    < template_parameters_stack.size() &&
                e.position() < template_parameters_stack[e.level()]->size())
                result = eval_name(template_parameters_stack[e.level()]->operator[](e.position()).name());
            else
                result = "BUGGY_IPR_RNAME_" + to_str(e.level()) + '_' + to_str(e.position());
        }
        virtual void visit(const ipr::Type_id&     e) { result = eval_type(e.type_expr()); }
    };

    return ipr::execute<NamePrintVisitor>(name); 
}

//------------------------------------------------------------------------------

std::string eval_type(const ipr::Type& n, const std::string& declarator, size_t precedence) 
{
    struct TypePrintVisitor : ipr::assertive_visitor_base<std::string> ///< Visitor that prints a type expression.
    {
        virtual void visit(const ipr::Array&         t) 
            { result = eval_type(t.element_type(), declarator(t) + '[' + eval_expr(t.bound())+ ']', ipr::precedence<ipr::Array>::value); }
        virtual void visit(const ipr::Void&          t) { result = "void"               + postfix(t); }
        virtual void visit(const ipr::Bool&          t) { result = "bool"               + postfix(t); }
        virtual void visit(const ipr::Char&          t) { result = "char"               + postfix(t); }
        virtual void visit(const ipr::sChar&         t) { result = "signed char"        + postfix(t); }
        virtual void visit(const ipr::uChar&         t) { result = "unsigned char"      + postfix(t); }
        virtual void visit(const ipr::Wchar_t&       t) { result = "wchar_t"            + postfix(t); }
        virtual void visit(const ipr::Short&         t) { result = "short"              + postfix(t); }
        virtual void visit(const ipr::uShort&        t) { result = "unsigned short"     + postfix(t); }
        virtual void visit(const ipr::Int&           t) { result = "int"                + postfix(t); }
        virtual void visit(const ipr::uInt&          t) { result = "unsigned int"       + postfix(t); }
        virtual void visit(const ipr::Long&          t) { result = "long"               + postfix(t); }
        virtual void visit(const ipr::uLong&         t) { result = "unsigned long"      + postfix(t); }
        virtual void visit(const ipr::Long_long&     t) { result = "long long"          + postfix(t); }
        virtual void visit(const ipr::uLong_long&    t) { result = "unsigned long long" + postfix(t); }
        virtual void visit(const ipr::Float&         t) { result = "float"              + postfix(t); }
        virtual void visit(const ipr::Double&        t) { result = "double"             + postfix(t); }
        virtual void visit(const ipr::Long_double&   t) { result = "long double"        + postfix(t); }
        virtual void visit(const ipr::Ellipsis&      t) { result = "..."; }
        virtual void visit(const ipr::Decltype&      t) { result = "decltype(" + eval_expr(t.expr()) + ')' + postfix(t); }
        virtual void visit(const ipr::Function&      t) 
        { 
            result = eval_type(t.target(), declarator(t) + '(' + eval_expr(t.source()) + ')', ipr::precedence<ipr::Function>::value);

            if (!can_throw_everything(t)) // we skip throw specification when any exception can be thrown
                result += " throw(" + eval_expr(t.throws()) + ')';
        }
        virtual void visit(const ipr::Pointer&       t) { result = eval_type(t.points_to(), '*' + postfix(t), ipr::precedence<ipr::Pointer>::value); }
        virtual void visit(const ipr::Reference&     t) { result = eval_type(t.refers_to(), '&' + postfix(t), ipr::precedence<ipr::Reference>::value); }
        virtual void visit(const ipr::Ptr_to_member& t) 
            { result = eval_type(t.member_type(), eval_type(t.containing_type()) + "::*" + postfix(t), ipr::precedence<ipr::Ptr_to_member>::value); }
        virtual void visit(const ipr::Qualified&     t)
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
 
        virtual void visit(const ipr::Product&       t) { result = eval_sqnc(t.elements()); }
        virtual void visit(const ipr::Sum&           t) { result = eval_sqnc(t.elements()); }
        virtual void visit(const ipr::Template&      t) { result = "template <" + eval_type(t.source()) + "> " + eval_type(t.target(), postfix(t)); }
        virtual void visit(const ipr::As_type&       t) { result = eval_expr(t.expr()) + postfix(t); } // This may represent user-defined class
        virtual void visit(const ipr::Udt&           t) { result = eval_name(t.name()) + postfix(t); } // This is analog of Decl handling in expression context - we only refer the name

        typedef ipr::assertive_visitor_base<std::string> base_type;

        TypePrintVisitor(const std::string& declarator = "", size_t precedence = 0) : base_type(declarator), m_precedence(precedence) {}

        std::string declarator(const ipr::Type& result_type) const { return ::precedence(result_type) < m_precedence ? '(' + result + ')' : result; }
        std::string    postfix(const ipr::Type& result_type) const { return ::postfix(declarator(result_type)); }
        std::string     prefix(const ipr::Type& result_type) const { return ::prefix(declarator(result_type)); }

        size_t m_precedence; ///< Declarator's precedence

    };

    std::string result = ipr::execute<TypePrintVisitor>(n,declarator,precedence);

    if (ipr::is_template_dependent_type(n))
        result = "typename " + result;

    return result;
}

//------------------------------------------------------------------------------

std::string eval_classic(const ipr::Classic& n) 
{ 
    struct ClassicExprPrintVisitor : ipr::assertive_visitor_base<std::string> ///< Visitor that prints an expression in the classic C++ meaning of it.
    {
        virtual void visit(const ipr::Literal&          e) { result = to_str(e.string()); }

        // Unary Operations

        virtual void visit(const ipr::Address&          e) { result = '&'         + eval_expr(e.operand()); }
        virtual void visit(const ipr::Array_delete&     e) { result = "delete[] " + eval_expr(e.operand()); }
        virtual void visit(const ipr::Complement&       e) { result = '~'         + eval_expr(e.operand()); }
        virtual void visit(const ipr::Delete&           e) { result = "delete "   + eval_expr(e.operand()); }
        virtual void visit(const ipr::Deref&            e) { result = '*'         + eval_expr(e.operand()); }
        virtual void visit(const ipr::Expr_sizeof&      e) { result = "sizeof("   + eval_expr(e.operand()) + ')'; }
        virtual void visit(const ipr::Expr_typeid&      e) { result = "typeid("   + eval_expr(e.operand()) + ')'; }
        virtual void visit(const ipr::Not&              e) { result = '!'         + eval_expr(e.operand()); }
        virtual void visit(const ipr::Paren_expr&       e) { result = '('         + eval_expr(e.operand()) + ')'; }
        virtual void visit(const ipr::Post_decrement&   e) { result =               eval_expr(e.operand()) + "--"; }
        virtual void visit(const ipr::Post_increment&   e) { result =               eval_expr(e.operand()) + "++"; }
        virtual void visit(const ipr::Pre_decrement&    e) { result = "--"        + eval_expr(e.operand()); }
        virtual void visit(const ipr::Pre_increment&    e) { result = "++"        + eval_expr(e.operand()); }
        virtual void visit(const ipr::Throw&            e) { result = "throw "    + eval_expr(e.operand()); }
        virtual void visit(const ipr::Type_sizeof&      e) { result = "sizeof("   + eval_expr(e.operand()) + ')'; }
        virtual void visit(const ipr::Type_typeid&      e) { result = "typeid("   + eval_expr(e.operand()) + ')'; }
        virtual void visit(const ipr::Unary_minus&      e) { result = '-'         + eval_expr(e.operand()); }
        virtual void visit(const ipr::Unary_plus&       e) { result = '+'         + eval_expr(e.operand()); }

        // Binary Operations

        virtual void visit(const ipr::Plus&             e) { result = eval_expr(e.first()) + " + "   + eval_expr(e.second()); }
        virtual void visit(const ipr::Minus&            e) { result = eval_expr(e.first()) + " - "   + eval_expr(e.second()); }
        virtual void visit(const ipr::Mul&              e) { result = eval_expr(e.first()) + " * "   + eval_expr(e.second()); }
        virtual void visit(const ipr::Div&              e) { result = eval_expr(e.first()) + " / "   + eval_expr(e.second()); }
        virtual void visit(const ipr::Modulo&           e) { result = eval_expr(e.first()) + " % "   + eval_expr(e.second()); }
        virtual void visit(const ipr::Bitand&           e) { result = eval_expr(e.first()) + " & "   + eval_expr(e.second()); }
        virtual void visit(const ipr::Bitor&            e) { result = eval_expr(e.first()) + " | "   + eval_expr(e.second()); }
        virtual void visit(const ipr::Bitxor&           e) { result = eval_expr(e.first()) + " ^ "   + eval_expr(e.second()); }
        virtual void visit(const ipr::Lshift&           e) { result = eval_expr(e.first()) + " << "  + eval_expr(e.second()); }
        virtual void visit(const ipr::Rshift&           e) { result = eval_expr(e.first()) + " >> "  + eval_expr(e.second()); }

        virtual void visit(const ipr::Assign&           e) { result = eval_expr(e.first()) + " = "   + eval_expr(e.second()); }
        virtual void visit(const ipr::Plus_assign&      e) { result = eval_expr(e.first()) + " += "  + eval_expr(e.second()); }
        virtual void visit(const ipr::Minus_assign&     e) { result = eval_expr(e.first()) + " -= "  + eval_expr(e.second()); }
        virtual void visit(const ipr::Mul_assign&       e) { result = eval_expr(e.first()) + " *= "  + eval_expr(e.second()); }
        virtual void visit(const ipr::Div_assign&       e) { result = eval_expr(e.first()) + " /= "  + eval_expr(e.second()); }
        virtual void visit(const ipr::Modulo_assign&    e) { result = eval_expr(e.first()) + " %= "  + eval_expr(e.second()); }
        virtual void visit(const ipr::Bitand_assign&    e) { result = eval_expr(e.first()) + " &= "  + eval_expr(e.second()); }
        virtual void visit(const ipr::Bitor_assign&     e) { result = eval_expr(e.first()) + " |= "  + eval_expr(e.second()); }
        virtual void visit(const ipr::Bitxor_assign&    e) { result = eval_expr(e.first()) + " ^= "  + eval_expr(e.second()); }
        virtual void visit(const ipr::Lshift_assign&    e) { result = eval_expr(e.first()) + " <<= " + eval_expr(e.second()); }
        virtual void visit(const ipr::Rshift_assign&    e) { result = eval_expr(e.first()) + " >>= " + eval_expr(e.second()); }

        virtual void visit(const ipr::And&              e) { result = eval_expr(e.first()) + " && "  + eval_expr(e.second()); }
        virtual void visit(const ipr::Or&               e) { result = eval_expr(e.first()) + " || "  + eval_expr(e.second()); }
        virtual void visit(const ipr::Equal&            e) { result = eval_expr(e.first()) + " == "  + eval_expr(e.second()); }
        virtual void visit(const ipr::Not_equal&        e) { result = eval_expr(e.first()) + " != "  + eval_expr(e.second()); }
        virtual void visit(const ipr::Greater&          e) { result = eval_expr(e.first()) + " > "   + eval_expr(e.second()); }
        virtual void visit(const ipr::Greater_equal&    e) { result = eval_expr(e.first()) + " >= "  + eval_expr(e.second()); }
        virtual void visit(const ipr::Less&             e) { result = eval_expr(e.first()) + " < "   + eval_expr(e.second()); }
        virtual void visit(const ipr::Less_equal&       e) { result = eval_expr(e.first()) + " <= "  + eval_expr(e.second()); }

        virtual void visit(const ipr::Comma&            e) { result = eval_expr(e.first()) + " , "   + eval_expr(e.second()); }
        virtual void visit(const ipr::Datum&            e) { result = eval_expr(e.first()) + '('     + eval_sqnc(e.args().elements()) + ')'; }

        // Binary Operations: Casts

        virtual void visit(const ipr::Cast&             e) { result =                 '(' + eval_type(e.type()) + ')'  + eval_expr(e.expr()); }
        virtual void visit(const ipr::Const_cast&       e) { result =       "const_cast<" + eval_type(e.type()) + ">(" + eval_expr(e.expr()) + ')'; }
        virtual void visit(const ipr::Dynamic_cast&     e) { result =     "dynamic_cast<" + eval_type(e.type()) + ">(" + eval_expr(e.expr()) + ')'; }
        virtual void visit(const ipr::Reinterpret_cast& e) { result = "reinterpret_cast<" + eval_type(e.type()) + ">(" + eval_expr(e.expr()) + ')'; }
        virtual void visit(const ipr::Static_cast&      e) { result =      "static_cast<" + eval_type(e.type()) + ">(" + eval_expr(e.expr()) + ')'; }

        // Binary Operations: Member selection

        virtual void visit(const ipr::Array_ref&        e) { result = eval_expr(e.base()) + '['  + eval_expr(e.member()) + ']'; } 
        virtual void visit(const ipr::Arrow&            e) 
        {
            if (e.has_impl_decl())
            {
                result = eval_expr(e.base()) + '.' + eval_name(e.impl_decl().name()) + "()";

                if (!is_phantom(e.member()))
                    result += "->" + eval_expr(e.member());
            }
            else
                result = eval_expr(e.base()) + "->" + eval_expr(e.member()); 
        }
        virtual void visit(const ipr::Dot&              e) { result = eval_expr(e.base()) + '.'   + eval_expr(e.member()); }
        virtual void visit(const ipr::Arrow_star&       e) { result = eval_expr(e.base()) + "->*" + eval_expr(e.member()); }
        virtual void visit(const ipr::Dot_star&         e) { result = eval_expr(e.base()) + ".*"  + eval_expr(e.member()); }

        // Ternary operations

        virtual void visit(const ipr::New&              e) { result = "new " + eval_type(e.allocated_type()); } // FIX: Take placement into account
        virtual void visit(const ipr::Conditional&      e) { result = eval_expr(e.condition()) + " ? "  + eval_expr(e.then_expr()) + " : " + eval_expr(e.else_expr()); }

        // Other operations

        virtual void visit(const ipr::Call&             e)
        {
            std::string func_name = eval_expr(e.function());

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
                        result = eval_expr(addr->operand()) + '.';
                    }
                    else
                    {
                        // The actual call is through this pointer
                        result = eval_expr(e.args()[0]) + "->";
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
                            result = eval_expr(addr->operand()) + '.';
                        }
                        else
                        {
                            // The actual call is through this pointer
                            result = eval_expr(e.args()[0]) + "->";
                        }

                        // Note that using target->name() instead of func_name might omit scope etc.
                        result += func_name + '(' + eval_sqnc(e.args().elements(), 1) + ')';
                        return;
                    }

                result = func_name + '(' + eval_sqnc(e.args().elements()) + ')';
            }
        }
    };

    return ipr::execute<ClassicExprPrintVisitor>(n); 
}

//------------------------------------------------------------------------------

// Common implementation base class for visitors that require indentation support
struct IndentationBasedVisitor : public ipr::assertive_visitor_base<cxx_printer&>
{
    typedef ipr::assertive_visitor_base<cxx_printer&> base_type;
    IndentationBasedVisitor(cxx_printer& cxx) : base_type(cxx) {}
    cxx_printer& beforeT(const ipr::Stmt& s) const { return result.beforeT(s); }
    cxx_printer& beforeF(const ipr::Stmt& s) const { return result.beforeF(s); }
    void           after(const ipr::Stmt& s, bool   new_line = false) const { return result.after(s,new_line); }
};

//------------------------------------------------------------------------------

cxx_printer& cxx_printer::operator<<(const ipr::Stmt& stmt)
{
    struct StatementPrintVisitor : IndentationBasedVisitor ///< Visitor that prints executable code.
    {
        StatementPrintVisitor(cxx_printer& cxx) : IndentationBasedVisitor(cxx) {}

        virtual void visit(const ipr::Labeled_stmt&  matched) { beforeF(matched) << (is_case_label(matched.label()) ? "case " : "") << matched.label() << ": " << matched.stmt(); after(matched); }
        virtual void visit(const ipr::Block&         matched)
        {
            if (matched.handlers().size())
            {
                beforeF(matched)  << "try" <<
                indentation << '{';
            }
            else
                beforeF(matched)  << '{';

            result << increase << matched.body() << decrease // Print statements in the body
                   << indentation << '}' 
                   << matched.handlers();                    // Print catch handlers

            after(matched,true);
        }
        virtual void visit(const ipr::Ctor_body&     matched) { if (matched.inits().size()) result << " : " << eval_sqnc(matched.inits().elements()); result << matched.block(); }
        virtual void visit(const ipr::If_then&       matched) { beforeT(matched) << "if ("     << matched.condition() << ')' << increase << matched.then_stmt() << decrease; after(matched,true); }
        virtual void visit(const ipr::If_then_else&  matched) { beforeT(matched) << "if ("     << matched.condition() << ')' << increase << matched.then_stmt() << decrease << indentation << "else" << increase << matched.else_stmt() << decrease; after(matched,true); }
        virtual void visit(const ipr::Switch&        matched) { beforeT(matched) << "switch (" << matched.condition() << ')' << matched.body(); after(matched,true); }
        virtual void visit(const ipr::While&         matched) { beforeT(matched) << "while ("  << matched.condition() << ')' << increase << matched.body() << decrease; after(matched,true); }
        virtual void visit(const ipr::Do&            matched) { beforeT(matched) << "do "      << increase << matched.body() << decrease << indentation << "while (" << matched.condition() << ");"; after(matched,true); }
        virtual void visit(const ipr::For&           matched) { beforeT(matched) << "for ("    << matched.initializer() << ';' << matched.condition() << ';' << matched.increment() << ')' << increase << matched.body() << decrease; after(matched,true); }
        virtual void visit(const ipr::Break&         matched) { beforeF(matched) << "break;";    after(matched); }
        virtual void visit(const ipr::Continue&      matched) { beforeF(matched) << "continue;"; after(matched); }
        virtual void visit(const ipr::Goto&          matched)
        {
            const ipr::Identifier* p = ipr::util::view<ipr::Identifier>(matched.target());
            assert(p); // When this asserts, ipr::Goto stopped pointing to just Identifier
            beforeF(matched) << "goto " << (p ? eval_name(*p) : "???") << ';';
            after(matched);
        }
        virtual void visit(const ipr::Return&        matched) { beforeF(matched) << "return " << matched.value() << ';'; after(matched); }
        virtual void visit(const ipr::Handler&       matched) { beforeF(matched) << "catch(" << eval_decl(matched.exception()) << ')' << matched.body(); after(matched); }
        virtual void visit(const ipr::Expr_stmt&     matched) { beforeF(matched) << matched.expr() << ';'; after(matched); }
        virtual void visit(const ipr::Empty_stmt&    matched) { beforeF(matched) << ';'; after(matched); }

        // Declarations
        virtual void visit(const ipr::Decl&          matched) { result << matched; }
    };

    return ipr::execute<StatementPrintVisitor>(stmt, *this); 
}

//------------------------------------------------------------------------------

cxx_printer& cxx_printer::operator<<(const ipr::Decl& decl)
{ 
    struct DeclarationPrintVisitor : IndentationBasedVisitor ///< Visitor that prints definitions in the program.
    {
        DeclarationPrintVisitor(cxx_printer& cxx) : IndentationBasedVisitor(cxx) {}

        virtual void visit(const ipr::Typedecl& matched)
        {
            beforeT(matched) << prefix_specifiers(matched) << eval_type(matched.type(), eval_name(matched.name()));

            if (is_namespace(matched))
            {
                assert(matched.has_initializer()); // Namespaces cannot be forward declared
                // initializer() would be a Namespace node, to which we forward traversal
                const ipr::Namespace* ns = ipr::util::view<ipr::Namespace>(matched.initializer());  // dynamic_cast alternative
                assert(ns);
                result << *ns;
            }
            else
            {
                if (matched.has_initializer())
                    result << matched.initializer();
                else
                    result << semicolon;
            }

            after(matched,true); // Print comment after the type declaration/namespace
        }

        virtual void visit(const ipr::Fundecl& matched)
        {
            beforeT(matched) << prefix_specifiers(matched);

            // NOTE: For some reason currently virtual member functions are not marked with
            //       virtual specifier on the Fundecl but instead with the virtual specifier
            //       on the first parameter to account for later support of multi-methods.
            if (is_virtual_member_function(matched) && is_in_class_declaration(matched))
                result << "virtual ";

            const ipr::Parameter_list& parameters = matched.parameters();

            // Evaluate declarator that will be used to output the result type that can
            // be complicated as function can return arrays or function pointers

            std::string declarator = get_declarator(matched) + '(' + eval_params(parameters) + ')';

            // Prepend class name to the name when declaration/definition is made outside
            // of class and refers to a member function

            const ipr::Function* ft = ipr::util::view<ipr::Function>(matched.type());  // dynamic_cast alternative
            assert(ft);

            // Print the name and return type when applicable
            if (!ipr::is_constructor(matched) && !ipr::is_destructor(matched) && !ipr::is_conversion(matched))
                result << eval_type(ft->target(), declarator);
            else
                result << declarator;

            // Check if this is a const member function
            if (is_const_member_function(matched))
                result << " const";

            // Print throw specification if the function has it
            if (!can_throw_everything(*ft)) // we skip throw specification when any exception can be thrown
                result << " throw(" + eval_type(ft->throws()) + ')';

            // Check if this is a pure virtual function (abstract method)
            if (matched.specifiers() & ipr::Decl::Pure)
                result << " = 0";

            // Print body if present
            if (matched.has_initializer())
            {
                const ipr::Stmt* body = ipr::util::view<ipr::Stmt>(matched.initializer());
                assert(body);
                result << *body;
            }
            else
                result << semicolon;

            after(matched,true); // Print comment after the function definition
        }

        virtual void visit(const ipr::Alias& matched)
        {
            beforeT(matched) << prefix_specifiers(matched);

            assert(matched.has_initializer());

            const ipr::Expr& init = matched.initializer();

	        // This is the case of: typedef T U;
            if (const ipr::Type* p = ipr::util::view<ipr::Type>(init))
            {
                result << eval_type(*p,eval_name(matched.name())) << ';'; // Since EDG front-end now sets up the Typedef specifier on ipr::Decl
                return;
            }

	        // This is the case of: namespace N = A::B::C::D;
            if (const ipr::Id_expr* id = ipr::util::view<ipr::Id_expr>(init))
            {
                if (const ipr::Typedecl* td = ipr::util::view<ipr::Typedecl>(id->resolution()))
                    if (ipr::is_namespace(*td))
                    {
                        result << "namespace " << matched.name() << '=' << id->name() << ';';
                        return;
                    }
            }

	        // This is the case of: using namespace A::B::C::D;
            if (const ipr::Typedecl* d = ipr::util::view<ipr::Typedecl>(init))
            {
                if (is_namespace(*d))
                {
                    result << "using namespace " << matched.initializer() << ';';
                    return;
                }
            }

            result << "using " << get_declarator(matched,true) << ';';
            after(matched);
        }

        virtual void visit(const ipr::Bitfield& matched)
        {
            beforeF(matched) << prefix_specifiers(matched) << eval_type(matched.type(), eval_name(matched.name())) << ':' << matched.precision();

            if (matched.has_initializer())
                result << " = " << matched.initializer();

            result << semicolon;
            after(matched);
        }

        virtual void visit(const ipr::Enumerator& matched)
        {
            beforeF(matched) << prefix_specifiers(matched) << matched.name();

            if (matched.has_initializer())
                result << " = " << matched.initializer();

            after(matched);
        }

        virtual void visit(const ipr::Named_map& matched)
        {
            extern bool evaluate_map (const ipr::Mapping&, const ipr::Named_map&, cxx_printer&);

            const ipr::Decl::Specifier prefix_mask = ipr::Decl::AccessProtection
                                                   | ipr::Decl::StorageClass
                                                   | ipr::Decl::Friend
                                                   | ipr::Decl::Typedef
                                                   | ipr::Decl::Constexpr
                                                   | ipr::Decl::Export;

            const ipr::Parameter_list& parameters = matched.params();

            // Print comment before the function definition
            beforeT(matched) << prefix_specifiers(matched, prefix_mask);
            evaluate_map(matched.mapping(), matched, result);
            after(matched,true); // Print comment after the function definition
        }

        virtual void visit(const ipr::Decl& matched)
        {
            std::string declarator = get_declarator(matched);
            beforeF(matched) << prefix_specifiers(matched) << eval_type(matched.type(), declarator);

            if (matched.has_initializer())
                result << " = " << matched.initializer();

            result << semicolon;
            after(matched);
        }

        virtual void visit(const ipr::Expr& matched) { result << matched; }
    };

    return ipr::execute<DeclarationPrintVisitor>(decl, *this); 
}

//------------------------------------------------------------------------------

cxx_printer& cxx_printer::operator<<(const ipr::Udt& udt)
{ 
    struct UdtPrintVisitor : IndentationBasedVisitor // Visitor that prints user data type definitions in the program.
    {
        UdtPrintVisitor(cxx_printer& cxx) : IndentationBasedVisitor(cxx) {}

        virtual void visit(const ipr::Global_scope& matched) { result << matched.members(); }
        virtual void visit(const ipr::Namespace&    matched) { result << indentation << '{' << increase << matched.members() << decrease << indentation << '}'; }
        virtual void visit(const ipr::Class&        matched)
        {
            // Print base classes
            const ipr::Sequence<ipr::Base_type>& bases = matched.bases();

            if (bases.size())
            {
                result << " : ";

                for (ipr::Sequence<ipr::Base_type>::iterator p = bases.begin(); p != bases.end(); ++p)
                {
                    if (p != bases.begin())
                        result << ", ";

                    result << p->name();
                }
            }

            result << indentation << '{' << increase << matched.members() << decrease << indentation << '}' << semicolon;
        }
        virtual void visit(const ipr::Union&        matched) { result << indentation << '{' << increase << matched.members() << decrease << indentation << '}' << semicolon; }
        virtual void visit(const ipr::Enum&         matched)
        {
            result << indentation << '{' << increase;

            const ipr::Sequence<ipr::Enumerator>& members = matched.members();

            for (ipr::Sequence<ipr::Enumerator>::iterator p = members.begin(); p != members.end(); ++p)
            {
                if (p != members.begin())
                    result << ',';

                result << *p;
            }

            result << decrease << indentation << '}' << semicolon;
        }
    };

    return ipr::execute<UdtPrintVisitor>(udt, *this);
}

//------------------------------------------------------------------------------

cxx_printer& cxx_printer::operator<<(const ipr::Expr& e)
{
    struct ExprPrintVisitor : public IndentationBasedVisitor ///< Visitor that dispatches to the corresponding kind of generalized expression
    {
        ExprPrintVisitor(cxx_printer& cxx) : IndentationBasedVisitor(cxx) {}

        virtual void visit(const ipr::Mapping&     matched)
        {
            if (ipr::util::node_has_member(matched, &ipr::Mapping::result))
                result << matched.result();
            else
                result << ';';
        }
        virtual void visit(const ipr::Overload&    matched) { }
        virtual void visit(const ipr::Scope&       matched)
        {
            // Scope can be in place of expression when expression is also a declaration
            // e.g. if (int i = foo()) {} or for (int i = 0, j = 0; ;);

            // This is a tricky case because first we have to generate initializers and
            // then also compute the operand that initializer resolves to as for example:
            // if (int i = 0, j = 1) ...

            const ipr::Sequence<ipr::Decl>& decls = matched.members();

            for (ipr::Sequence<ipr::Decl>::iterator p = decls.begin(); p != decls.end(); ++p)
            {
                std::string matched = eval_decl(*p); // Evaluate declaration without trailing ;

                if (p != decls.begin())
                    result << ',';

                result << matched;
            }
        }

        virtual void visit(const ipr::Classic&     matched) { result << matched; }
        virtual void visit(const ipr::Name&        matched) { result << matched; }
        virtual void visit(const ipr::Phantom&     matched) { }
        virtual void visit(const ipr::Expr_list&   matched) { result << '{' << eval_sqnc(matched.operand()) << '}'; }
        virtual void visit(const ipr::Label&       matched) { }
        virtual void visit(const ipr::Member_init& matched) 
        {
            // FIX: This is workaround for now as it should become Expr_list statically
            if (const ipr::Expr_list* p = ipr::util::view<ipr::Expr_list>(matched.initializer()))
                result << matched.member() << '(' << eval_sqnc(p->elements()) << ')';
            else
                result << matched.member() << '(' << matched.initializer() << ')'; 
        }

        // The following kinds of nodes are not directly inherited from Expr so 
        // technically they shouldn't be handled here. However they may happen in 
        // context were traditional expressions are expected and those cases are
        // covered here.

        // Declaration might happen in expression context when IPR refers to the use
        // of the name but whoever generated IPR forgot to mark such use properly with
        // Id_expr and pointed directly to the declaration instead.
        virtual void visit(const ipr::Decl&        matched) { result << matched.name(); }

        // Expr_stmt can happen in the initializer of the following loop:
        // int i; for (i=1; i < 10; i++)
        // because the scope won't be introduced. It seems to be easier at the
        // moment to simply handle this case here rather then all of the
        // cases of ClassicExprPrintVisitor inside StatementPrintVisitor when
        // forwarding evaluation to the initializer.
        virtual void visit(const ipr::Expr_stmt&   matched) { result << matched.expr(); }

        // And this hack counter measure the bugs introduced by the Udt hack below
        // as references to Namespace should be just treated as names and not dumping
        // of their content that the above hack will do.
        virtual void visit(const ipr::Namespace&   matched) { result << matched.name(); }

        // This shouldn't be here at all, but it's a quick hack right now to handle
        // template class definitions as mapping will forward here.
        virtual void visit(const ipr::Udt&         matched) { result << matched; }
        virtual void visit(const ipr::Type&        matched) { result << matched; }
        virtual void visit(const ipr::Stmt&        matched) { result << matched; }
    };

    return ipr::execute<ExprPrintVisitor>(e, *this);
}

//------------------------------------------------------------------------------

cxx_printer& cxx_printer::operator<<(const ipr::Node& n)
{
    struct NodePrintVisitor : IndentationBasedVisitor ///< Visitor capable of printing any part of the program
    {
        NodePrintVisitor(cxx_printer& cxx) : IndentationBasedVisitor(cxx) {}

        void visit(const ipr::Unit& matched) { result << matched.get_global_scope(); } // This should forward to visitation of Global_scope, which is a Namespace
        void visit(const ipr::Udt&  matched) { result << matched; }
        void visit(const ipr::Decl& matched) { result << matched; }
        void visit(const ipr::Stmt& matched) { result << matched; }
        void visit(const ipr::Type& matched) { result << matched; }
        void visit(const ipr::Name& matched) { result << matched; }
        void visit(const ipr::Expr& matched) { result << matched; }
    };

    return ipr::execute<NodePrintVisitor>(n,*this);
}

//------------------------------------------------------------------------------

} // of namespace cxxv
