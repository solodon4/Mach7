///
/// \file printer_matching.cpp
///
/// Implementation of a C++ pretty-printer for Pivot based on pattern matching
///
/// \author Yuriy Solodkyy
/// Copyright (C) 2011, Texas A&M University.  All rights reserved.
///

#include "printer_matching.hpp"
#include "precedence.hpp"
#include "match_ipr.hpp"         // Pattern-matching bindings for IPR hierarchy.

/// Pattern-matching version of C++ printer
namespace cxxm
{

//------------------------------------------------------------------------------

std::string eval_name(const ipr::Name& name)
{
    Match(name)
    {
        Case(ipr::Identifier,        str) return to_str(str);
        Case(ipr::Id_expr,          name) return eval_name(name);
        Case(ipr::Operator,       opname) return "operator" + postfix(to_str(opname));
        Case(ipr::Conversion,     target) return "operator" + postfix(eval_type(target));
        Case(ipr::Scope_ref, scope, member) 
        {
            std::string result = eval_expr(scope) + "::";

            if (const ipr::Template_id* p = ipr::util::view<ipr::Template_id>(member))
                if (ipr::is_template_dependent(scope))
                    result += "template ";

            return result + eval_expr(member);
        }
        Case(ipr::Template_id, name, arguments)
        {
            std::string args = eval_sqnc(arguments.elements());

            if (args.length() > 0 && args[args.length()-1] == '>')
                args += ' ';

            return eval_name(name) + '<' + args + '>';
        }
        Case(ipr::Ctor_name, object_type) return eval_type(object_type);
        Case(ipr::Dtor_name, object_type) return '~' + eval_type(object_type);
        Case(ipr::Rname, level, position)
        {
            if (level    < template_parameters_stack.size() &&
                position < template_parameters_stack[level]->size())
                return eval_name(template_parameters_stack[level]->operator[](position).name());
            else
                return "BUGGY_IPR_RNAME_" + to_str(level) + '_' + to_str(position);
        }
        Case(ipr::Type_id,     type_expr) return eval_type(type_expr);
    }
    EndMatch
}

//------------------------------------------------------------------------------

std::string eval_type(const ipr::Type& n, const std::string& declarator, size_t decl_precedence) 
{
    const std::string& t = ::precedence(n) < decl_precedence ? '(' + declarator + ')' : declarator;
    std::string result;

    Match(n)
    {
        Case(ipr::Array,        element_type, bound) 
            result = eval_type(element_type, t + '[' + eval_expr(bound)+ ']', ipr::precedence<ipr::Array>::value); 
            break;
        Case(ipr::Void          )  result = "void"               + postfix(t); break;
        Case(ipr::Bool          )  result = "bool"               + postfix(t); break;
        Case(ipr::Char          )  result = "char"               + postfix(t); break;
        Case(ipr::sChar         )  result = "signed char"        + postfix(t); break;
        Case(ipr::uChar         )  result = "unsigned char"      + postfix(t); break;
        Case(ipr::Wchar_t       )  result = "wchar_t"            + postfix(t); break;
        Case(ipr::Short         )  result = "short"              + postfix(t); break;
        Case(ipr::uShort        )  result = "unsigned short"     + postfix(t); break;
        Case(ipr::Int           )  result = "int"                + postfix(t); break;
        Case(ipr::uInt          )  result = "unsigned int"       + postfix(t); break;
        Case(ipr::Long          )  result = "long"               + postfix(t); break;
        Case(ipr::uLong         )  result = "unsigned long"      + postfix(t); break;
        Case(ipr::Long_long     )  result = "long long"          + postfix(t); break;
        Case(ipr::uLong_long    )  result = "unsigned long long" + postfix(t); break;
        Case(ipr::Float         )  result = "float"              + postfix(t); break;
        Case(ipr::Double        )  result = "double"             + postfix(t); break;
        Case(ipr::Long_double   )  result = "long double"        + postfix(t); break;
        Case(ipr::Ellipsis      )  result = "..."; break;
        Case(ipr::Decltype,     expr)  result = "decltype(" + eval_expr(expr) + ')'; break;
        Case(ipr::Function,     source, target, throws) 
        { 
            result = eval_type(target, t + '(' + eval_expr(source) + ')', ipr::precedence<ipr::Function>::value);

            if (!can_throw_everything(*matched)) // we skip throw specification when any exception can be thrown
                result += " throw(" + eval_expr(throws) + ')';

            break;
        }
        Case(ipr::Pointer,      points_to)  result = eval_type(points_to, '*' + postfix(t), ipr::precedence<ipr::Pointer>::value);   break;
        Case(ipr::Reference,    refers_to)  result = eval_type(refers_to, '&' + postfix(t), ipr::precedence<ipr::Reference>::value); break;
        Case(ipr::Ptr_to_member,containing_type, member_type) 
            result = eval_type(member_type, eval_type(containing_type) + "::*" + postfix(t), ipr::precedence<ipr::Ptr_to_member>::value); 
            break;
        Case(ipr::Qualified,    main_variant, quals)
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

            result = eval_type(main_variant, qualifiers + postfix(t), ::precedence(main_variant)); 
            break;
        }
 
        Case(ipr::Product,      elements)       result = eval_sqnc(elements); break;
        Case(ipr::Sum,          elements)       result = eval_sqnc(elements); break;
        Case(ipr::Template,     source, target) result = "template <" + eval_type(source) + "> " + eval_type(target, postfix(t)); break;
        Case(ipr::As_type,      expr)           result = eval_expr(expr) + postfix(t); break; // This may represent user-defined class
        Case(ipr::Udt,          name)           result = eval_name(name) + postfix(t); break; // This is analog of Decl handling in expression context - we only refer the name
    }
    EndMatch

    if (ipr::is_template_dependent_type(n))
        result = "typename " + result;

    return result;
}

//------------------------------------------------------------------------------

std::string eval_classic(const ipr::Classic& n)
{
    Match(n)
    {
        Case(ipr::Literal,                _, str) return to_str(str);

        // Unary Operations

        Case(ipr::Address,               operand) return '&'         + eval_expr(operand);
        Case(ipr::Array_delete,          operand) return "delete[] " + eval_expr(operand);
        Case(ipr::Complement,            operand) return '~'         + eval_expr(operand);
        Case(ipr::Delete,                operand) return "delete "   + eval_expr(operand);
        Case(ipr::Deref,                 operand) return '*'         + eval_expr(operand);
        Case(ipr::Expr_sizeof,           operand) return "sizeof("   + eval_expr(operand) + ')';
        Case(ipr::Expr_typeid,           operand) return "typeid("   + eval_expr(operand) + ')';
        Case(ipr::Not,                   operand) return '!'         + eval_expr(operand);
        Case(ipr::Paren_expr,            operand) return '('         + eval_expr(operand) + ')';
        Case(ipr::Post_decrement,        operand) return               eval_expr(operand) + "--";
        Case(ipr::Post_increment,        operand) return               eval_expr(operand) + "++";
        Case(ipr::Pre_decrement,         operand) return "--"        + eval_expr(operand);
        Case(ipr::Pre_increment,         operand) return "++"        + eval_expr(operand);
        Case(ipr::Throw,                 operand) return "throw "    + eval_expr(operand);
        Case(ipr::Type_sizeof,           operand) return "sizeof("   + eval_expr(operand) + ')';
        Case(ipr::Type_typeid,           operand) return "typeid("   + eval_expr(operand) + ')';
        Case(ipr::Unary_minus,           operand) return '-'         + eval_expr(operand);
        Case(ipr::Unary_plus,            operand) return '+'         + eval_expr(operand);

        // Binary Operations

        Case(ipr::Plus,            first, second) return eval_expr(first) + " + "   + eval_expr(second);
        Case(ipr::Minus,           first, second) return eval_expr(first) + " - "   + eval_expr(second);
        Case(ipr::Mul,             first, second) return eval_expr(first) + " * "   + eval_expr(second);
        Case(ipr::Div,             first, second) return eval_expr(first) + " / "   + eval_expr(second);
        Case(ipr::Modulo,          first, second) return eval_expr(first) + " % "   + eval_expr(second);
        Case(ipr::Bitand,          first, second) return eval_expr(first) + " & "   + eval_expr(second);
        Case(ipr::Bitor,           first, second) return eval_expr(first) + " | "   + eval_expr(second);
        Case(ipr::Bitxor,          first, second) return eval_expr(first) + " ^ "   + eval_expr(second);
        Case(ipr::Lshift,          first, second) return eval_expr(first) + " << "  + eval_expr(second);
        Case(ipr::Rshift,          first, second) return eval_expr(first) + " >> "  + eval_expr(second);

        Case(ipr::Assign,          first, second) return eval_expr(first) + " = "   + eval_expr(second);
        Case(ipr::Plus_assign,     first, second) return eval_expr(first) + " += "  + eval_expr(second);
        Case(ipr::Minus_assign,    first, second) return eval_expr(first) + " -= "  + eval_expr(second);
        Case(ipr::Mul_assign,      first, second) return eval_expr(first) + " *= "  + eval_expr(second);
        Case(ipr::Div_assign,      first, second) return eval_expr(first) + " /= "  + eval_expr(second);
        Case(ipr::Modulo_assign,   first, second) return eval_expr(first) + " %= "  + eval_expr(second);
        Case(ipr::Bitand_assign,   first, second) return eval_expr(first) + " &= "  + eval_expr(second);
        Case(ipr::Bitor_assign,    first, second) return eval_expr(first) + " |= "  + eval_expr(second);
        Case(ipr::Bitxor_assign,   first, second) return eval_expr(first) + " ^= "  + eval_expr(second);
        Case(ipr::Lshift_assign,   first, second) return eval_expr(first) + " <<= " + eval_expr(second);
        Case(ipr::Rshift_assign,   first, second) return eval_expr(first) + " >>= " + eval_expr(second);

        Case(ipr::And,             first, second) return eval_expr(first) + " && "  + eval_expr(second);
        Case(ipr::Or,              first, second) return eval_expr(first) + " || "  + eval_expr(second);
        Case(ipr::Equal,           first, second) return eval_expr(first) + " == "  + eval_expr(second);
        Case(ipr::Not_equal,       first, second) return eval_expr(first) + " != "  + eval_expr(second);
        Case(ipr::Greater,         first, second) return eval_expr(first) + " > "   + eval_expr(second);
        Case(ipr::Greater_equal,   first, second) return eval_expr(first) + " >= "  + eval_expr(second);
        Case(ipr::Less,            first, second) return eval_expr(first) + " < "   + eval_expr(second);
        Case(ipr::Less_equal,      first, second) return eval_expr(first) + " <= "  + eval_expr(second);

        Case(ipr::Comma,           first, second) return eval_expr(first) + " , "   + eval_expr(second);
        Case(ipr::Datum,                    c, s) return eval_expr(c) + '('   + eval_sqnc(s.elements()) + ')';

        // Binary Operations: Casts

        Case(ipr::Cast,               type, expr) return                 '(' + eval_type(type) + ')'  + eval_expr(expr);
        Case(ipr::Const_cast,         type, expr) return       "const_cast<" + eval_type(type) + ">(" + eval_expr(expr) + ')';
        Case(ipr::Dynamic_cast,       type, expr) return     "dynamic_cast<" + eval_type(type) + ">(" + eval_expr(expr) + ')';
        Case(ipr::Reinterpret_cast,   type, expr) return "reinterpret_cast<" + eval_type(type) + ">(" + eval_expr(expr) + ')';
        Case(ipr::Static_cast,        type, expr) return      "static_cast<" + eval_type(type) + ">(" + eval_expr(expr) + ')';

        // Binary Operations: Member selection

        Case(ipr::Array_ref,        base, member) return eval_expr(base) + '['  + eval_expr(member) + ']'; 
        Case(ipr::Arrow,            base, member) 
        {
            if (matched->has_impl_decl())
            {
                std::string result = eval_expr(base) + '.' + eval_name(matched->impl_decl().name()) + "()";

                if (!is_phantom(member))
                    result += "->" + eval_expr(member);

                return result;
            }
            else
                return eval_expr(base) + "->" + eval_expr(member); 
        }
        Case(ipr::Dot,              base, member) return eval_expr(base) + '.'   + eval_expr(member);
        Case(ipr::Arrow_star,       base, member) return eval_expr(base) + "->*" + eval_expr(member);
        Case(ipr::Dot_star,         base, member) return eval_expr(base) + ".*"  + eval_expr(member);

        // Ternary operations

        Case(ipr::New,   placement, allocated_type, initializer) return "new " + eval_type(allocated_type); // FIX: Take placement into account
        Case(ipr::Conditional,  condition, then_expr, else_expr) return eval_expr(condition) + " ? "  + eval_expr(then_expr) + " : " + eval_expr(else_expr);

        // Other operations

        Case(ipr::Call, function, args)
        {
            std::string func_name = eval_expr(function);

            if (const ipr::Fundecl* target = call_target(*matched)) // Call target is known
            {
                const ipr::Parameter_list& parameters = target->parameters();

                if (is_member_function(*target))
                {
                    // Call to a member function
                    assert(args.size() > 0); // There must be argument that represents this pointer

                    std::string result;

                    // Because of function call unification, calls to member functions
                    // are still represented as regular calls with this pointer passed
                    // explicitly as the first argument. The original call might have
                    // been on reference to object, in which case an ipr::Address node
                    // is artificially inserted. Here we are trying to undo that 
                    // unnecessary address to print the dot when possible.
                    if (const ipr::Address* addr = ipr::util::view<ipr::Address>(args[0]))
                    {
                        // The address is artifical and the actual call is through dot
                        result = eval_expr(addr->operand()) + '.';
                    }
                    else
                    {
                        // The actual call is through this pointer
                        result = eval_expr(args[0]) + "->";
                    }

                    // Note that using target->name() instead of func_name might omit scope etc.
                    return result + func_name + '(' + eval_sqnc(args.elements(), 1) + ')';
                }
                else // not a member function
                {
                    // Call to a regular function
                    return func_name + '(' + eval_sqnc(args.elements()) + ')';
                }
            }
            else // Call target is not known
            {
                if (const ipr::Function* ft = ipr::util::view<ipr::Function>(function.type()))
                    if (is_member_function_type(*ft))
                    {
                        // Call to a member function
                        assert(args.size() > 0); // There must be argument that represents this pointer

                        std::string result;

                        // Because of function call unification, calls to member functions
                        // are still represented as regular calls with this pointer passed
                        // explicitly as the first argument. The original call might have
                        // been on reference to object, in which case an ipr::Address node
                        // is artificially inserted. Here we are trying to undo that 
                        // unnecessary address to print the dot when possible.
                        if (const ipr::Address* addr = ipr::util::view<ipr::Address>(args[0]))
                        {
                            // The address is artifical and the actual call is through dot
                            result = eval_expr(addr->operand()) + '.';
                        }
                        else
                        {
                            // The actual call is through this pointer
                            result = eval_expr(args[0]) + "->";
                        }

                        // Note that using target->name() instead of func_name might omit scope etc.
                        return result + func_name + '(' + eval_sqnc(args.elements(), 1) + ')';
                    }

                return func_name + '(' + eval_sqnc(args.elements()) + ')';
            }
        }
    }
    EndMatch
}

//------------------------------------------------------------------------------

cxx_printer& cxx_printer::operator<<(const ipr::Stmt& stmt)
{
    Match(stmt)
    {
        Case(ipr::Labeled_stmt, lbl, stm)         beforeF(*matched) << (is_case_label(lbl) ? "case " : "") << lbl << ": " << stm; after(*matched); break;
        Case(ipr::Block, body, handlers)
        {
            if (handlers.size())
            {
                beforeF(*matched) << "try" <<
                indentation      << '{';
            }
            else
                beforeF(*matched) << '{';

            *this << increase << body << decrease // Print statements in the body
                  << indentation << '}' 
                  << handlers;                    // Print catch handlers

            after(*matched,true);
            break;
        }
        Case(ipr::Ctor_body,    inits, block)               if (inits.size()) *this << " : " << eval_sqnc(inits.elements()); *this << block; break;
        Case(ipr::If_then,      cond, then_stmt)            beforeT(*matched) << "if ("     << cond << ')' << increase << then_stmt << decrease; after(*matched,true); break;
        Case(ipr::If_then_else, cond, then_stmt, else_stmt) beforeT(*matched) << "if ("     << cond << ')' << increase << then_stmt << decrease << indentation << "else" << increase << else_stmt << decrease; after(*matched,true); break;
        Case(ipr::Switch,       cond, body)                 beforeT(*matched) << "switch (" << cond << ')' << body; after(*matched,true); break;
        Case(ipr::While,        cond, body)                 beforeT(*matched) << "while ("  << cond << ')' << increase << body << decrease; after(*matched,true); break;
        Case(ipr::Do,           cond, body)                 beforeT(*matched) << "do "      << increase << body << decrease << indentation << "while (" << cond << ");"; after(*matched,true); break;
        Case(ipr::For,          init, cond, incr, body)     beforeT(*matched) << "for ("    << init << ';' << cond << ';' << incr << ')' << increase << body << decrease; after(*matched,true); break;
        Case(ipr::Break)                                    beforeF(*matched) << "break;";    after(*matched); break;
        Case(ipr::Continue)                                 beforeF(*matched) << "continue;"; after(*matched); break;
        Case(ipr::Goto,         target)
        {
            const ipr::Identifier* p = ipr::util::view<ipr::Identifier>(target);
            assert(p); // When this asserts, ipr::Goto stopped pointing to just Identifier
            beforeF(*matched) << "goto " << (p ? eval_name(*p) : "???") << ';';
            after(*matched);
            break;
        }
        Case(ipr::Return,       value)                      beforeF(*matched) << "return " << value << ';'; after(*matched); break;
        Case(ipr::Handler,      expt, body)                 beforeF(*matched) << "catch(" << eval_decl(expt) << ')' << body; after(*matched); break;
        Case(ipr::Empty_stmt)                               beforeF(*matched) << ';'; after(*matched); break;
        Case(ipr::Expr_stmt,    expr)                       beforeF(*matched) << expr << ';'; after(*matched); break;

        // Declarations
        Case(ipr::Decl)                                     *this << *matched; break;
    }
    EndMatch

    return *this;
}

//------------------------------------------------------------------------------

cxx_printer& cxx_printer::operator<<(const ipr::Decl& decl)
{
    Match(decl)
    {
        Case(ipr::Typedecl, name, type)
        {
            beforeT(*matched) << prefix_specifiers(*matched) << eval_type(type, eval_name(name));

            if (is_namespace(*matched))
            {
                assert(matched->has_initializer()); // Namespaces cannot be forward declared
                // initializer() would be a Namespace node, to which we forward traversal
                const ipr::Namespace* ns = ipr::util::view<ipr::Namespace>(matched->initializer());  // dynamic_cast alternative
                assert(ns);
                *this << *ns;
            }
            else
            {
                if (matched->has_initializer())
                    *this << matched->initializer();
                else
                    *this << semicolon;
            }

            after(*matched,true); // Print comment after the type declaration/namespace
            break;
        }

        Case(ipr::Fundecl, name, type, membership, parameters) // FIX: membership is not used here!
        {
            beforeT(*matched) << prefix_specifiers(*matched);

            // NOTE: For some reason currently virtual member functions are not marked with
            //       virtual specifier on the Fundecl but instead with the virtual specifier
            //       on the first parameter to account for later support of multi-methods.
            if (is_virtual_member_function(*matched) && is_in_class_declaration(*matched))
                *this << "virtual ";

            // Evaluate declarator that will be used to output the result type that can
            // be complicated as function can return arrays or function pointers

            std::string declarator = get_declarator(*matched) + '(' + eval_params(parameters) + ')';

            // Prepend class name to the name when declaration/definition is made outside
            // of class and refers to a member function
            const ipr::Function* ft = ipr::util::view<ipr::Function>(type);  // dynamic_cast alternative
            assert(ft);

            // Print the name and return type when applicable
            if (!ipr::is_constructor(*matched) && !ipr::is_destructor(*matched) && !ipr::is_conversion(*matched))
                *this << eval_type(ft->target(), declarator);
            else
                *this << declarator;

            // Check if this is a const member function
            if (is_const_member_function(*matched))
                *this << " const";

            // Print throw specification if the function has it
            if (!can_throw_everything(*ft)) // we skip throw specification when any exception can be thrown
                *this << " throw(" + eval_type(ft->throws()) + ')';

            // Check if this is a pure virtual function (abstract method)
            if (matched->specifiers() & ipr::Decl::Pure)
                *this << " = 0";

            // Print body if present
            if (matched->has_initializer())
            {
                const ipr::Stmt* body = ipr::util::view<ipr::Stmt>(matched->initializer());
                assert(body);
                *this << *body;
            }
            else
                *this << semicolon;

            after(*matched,true); // Print comment after the function definition
            break;
        }

        Case(ipr::Alias, name, type)
        {
            beforeT(*matched) << prefix_specifiers(*matched);

            assert(matched->has_initializer());

            const ipr::Expr& init = matched->initializer();

	        // This is the case of: typedef T U;
            if (const ipr::Type* p = ipr::util::view<ipr::Type>(init))
            {
                *this << eval_type(*p,eval_name(name)) << ';'; // Since EDG front-end now sets up the Typedef specifier on ipr::Decl
                return *this;
            }

	        // This is the case of: namespace N = A::B::C::D;
            if (const ipr::Id_expr* id = ipr::util::view<ipr::Id_expr>(init))
            {
                if (const ipr::Typedecl* td = ipr::util::view<ipr::Typedecl>(id->resolution()))
                    if (ipr::is_namespace(*td))
                    {
                        *this << "namespace " << name << '=' << id->name() << ';';
                        return *this;
                    }
            }

	        // This is the case of: using namespace A::B::C::D;
            if (const ipr::Typedecl* d = ipr::util::view<ipr::Typedecl>(init))
            {
                if (is_namespace(*d))
                {
                    *this << "using namespace " << matched->initializer() << ';';
                    return *this;
                }
            }

            *this << "using " << get_declarator(*matched,true) << ';';
            after(*matched);
            break;
        }

        Case(ipr::Bitfield, name, type, membership, precision) // FIX: membership is not used here!
        {
            beforeF(*matched) << prefix_specifiers(*matched) << eval_type(type, eval_name(name)) << ':' << precision;

            if (matched->has_initializer())
                *this << " = " << matched->initializer();

            *this << semicolon;
            after(*matched);
            break;
        }

        Case(ipr::Enumerator, name)
        {
            beforeF(*matched) << prefix_specifiers(*matched) << name;

            if (matched->has_initializer())
                *this << " = " << matched->initializer();

            after(*matched);
            break;
        }

        Case(ipr::Named_map, name, type, parameters, mapping) // FIX: Only parameters is used here
        {
            extern bool evaluate_map (const ipr::Mapping&, const ipr::Named_map&, cxx_printer&);

            const ipr::Decl::Specifier prefix_mask = ipr::Decl::AccessProtection
                                                   | ipr::Decl::StorageClass
                                                   | ipr::Decl::Friend
                                                   | ipr::Decl::Typedef
                                                   | ipr::Decl::Constexpr
                                                   | ipr::Decl::Export;

            // Print comment before the function definition
            beforeT(*matched) << prefix_specifiers(*matched, prefix_mask);
            evaluate_map(mapping, *matched, *this);
            after(*matched,true); // Print comment after the function definition
            break;
        }

        Case(ipr::Decl)
        {
            std::string declarator = get_declarator(*matched); //eval_name(matched->name());
            beforeF(*matched) << prefix_specifiers(*matched) << eval_type(matched->type(), declarator);

            if (matched->has_initializer())
                *this << " = " << matched->initializer();

            *this << semicolon;
            after(*matched);
            break;
        }

        Case(ipr::Expr)         *this << *matched; break; // TODO: Discuss why we had it in visitors!
    }
    EndMatch

    return *this;
}

//------------------------------------------------------------------------------

cxx_printer& cxx_printer::operator<<(const ipr::Udt& udt)
{
    Match(udt)
    {
        Case(ipr::Global_scope, members) *this << members; break;
        Case(ipr::Namespace,    members) *this << indentation << '{' << increase << members << decrease << indentation << '}'; break;
        Case(ipr::Class, members, bases)
        {
            // Print base classes
            if (bases.size())
            {
                *this << " : ";

                for (ipr::Sequence<ipr::Base_type>::iterator p = bases.begin(); p != bases.end(); ++p)
                {
                    if (p != bases.begin())
                        *this << ", ";

                    *this << p->name();
                }
            }

            *this << indentation << '{' << increase << members << decrease << indentation << '}' << semicolon;
            break;
        }

        Case(ipr::Union,        members) *this << indentation << '{' << increase << members << decrease << indentation << '}' << semicolon; break;
        Case(ipr::Enum,         members)
        {
            *this << indentation << '{' << increase;

            for (ipr::Sequence<ipr::Enumerator>::iterator p = members.begin(); p != members.end(); ++p)
            {
                if (p != members.begin())
                    *this << ',';

                *this << *p;
            }

            *this << decrease << indentation << '}' << semicolon;
            break;
        }
    }
    EndMatch

    return *this;
}

//------------------------------------------------------------------------------

cxx_printer& cxx_printer::operator<<(const ipr::Expr& e)
{
    Match(e)
    {
        Case(ipr::Mapping)
        {
            if (ipr::util::node_has_member(*matched, &ipr::Mapping::result))
                *this << matched->result();
            else
                *this << ';';
            break;
        }
        Case(ipr::Overload) break;
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
                    *this << ',';

                *this << s;
            }

            break;
        }

        Case(ipr::Classic)   *this << *matched; break;
        Case(ipr::Name)      *this << *matched; break;
        Case(ipr::Phantom)   break;
        Case(ipr::Expr_list, operand) *this << '{' << eval_sqnc(operand) << '}'; break;
        Case(ipr::Label)     break;
        Case(ipr::Member_init, member, initializer) 
        {
            // FIX: This is workaround for now as it should become Expr_list statically
            if (const ipr::Expr_list* p = ipr::util::view<ipr::Expr_list>(initializer))
                *this << member << '(' << eval_sqnc(p->elements()) << ')';
            else
                *this << member << '(' << initializer << ')'; 
            break;
        }

        // The following kinds of nodes are not directly inherited from Expr so 
        // technically they shouldn't be handled here. However they may happen in 
        // context were traditional expressions are expected and those cases are
        // covered here.

        // Declaration might happen in expression context when IPR refers to the use
        // of the name but whoever generated IPR forgot to mark such use properly with
        // Id_expr and pointed directly to the declaration instead.
        Case(ipr::Decl, name)      *this << name; break;

        // Expr_stmt can happen in the initializer of the following loop:
        // int i; for (i=1; i < 10; i++)
        // because the scope won't be introduced. It seems to be easier at the
        // moment to simply handle this case here rather then all of the
        // cases of ClassicExprPrintVisitor inside StatementPrintVisitor when
        // forwarding evaluation to the initializer.
        Case(ipr::Expr_stmt, expr) *this << expr; break;

        // And this hack counter measure the bugs introduced by the Udt hack below
        // as references to Namespace should be just treated as names and not dumping
        // of their content that the above hack will do.
        Case(ipr::Namespace)       *this << matched->name(); break;

        // This shouldn't be here at all, but it's a quick hack right now to handle
        // template class definitions as mapping will forward here.
        Case(ipr::Udt)             *this << *matched; break;
        Case(ipr::Type)            *this << *matched; break;
        Case(ipr::Stmt)            *this << *matched; break;
    }
    EndMatch

    return *this;
}

//------------------------------------------------------------------------------

cxx_printer& cxx_printer::operator<<(const ipr::Node& n)
{
    Match(n)
    {
        Case(ipr::Unit, global_scope) *this << global_scope; break; // This should forward to visitation of Global_scope, which is a Namespace
        Case(ipr::Udt)                *this << *matched; break;
        Case(ipr::Decl)               *this << *matched; break;
        Case(ipr::Stmt)               *this << *matched; break;
        Case(ipr::Type)               *this << *matched; break;
        Case(ipr::Name)               *this << *matched; break;
        Case(ipr::Expr)               *this << *matched; break;
    }
    EndMatch

    return *this;
}

//------------------------------------------------------------------------------

} // of namespace cxxm
