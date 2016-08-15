#pragma once

#include <ipr/interface>  // Pivot interfaces

// Precedence of operation has been copied from 
// http://en.wikipedia.org/wiki/Operators_in_C_and_C%2B%2B

namespace ipr
{
    static const size_t unspecified_precedence = 0;

    template <class T> struct traits            { typedef Node parent_type; static const size_t precedence =  unspecified_precedence; };
    
    template <> struct traits<Node>             { enum { category = unknown_cat};           typedef void parent_type; static const size_t precedence =  unspecified_precedence; };

    template <> struct traits<Annotation>       { enum { category = annotation_cat};        typedef Node parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Region>           { enum { category = region_cat};            typedef Node parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Comment>          { enum { category = comment_cat};           typedef Node parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<String>           { enum { category = string_cat};            typedef Node parent_type; static const size_t precedence =  unspecified_precedence; };

    template <> struct traits<Mapping>          { enum { category = mapping_cat};           typedef Expr parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Overload>         { enum { category = overload_cat};          typedef Expr parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Parameter_list>   { enum { category = parameter_list_cat};    typedef Region parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Scope>            { enum { category = scope_cat};             typedef Expr parent_type; static const size_t precedence =  unspecified_precedence; };

    // ------------------------------------------
    // -- general constructors                 --
    // ------------------------------------------
    template <> struct traits<Linkage>          { enum { category = linkage_cat};           typedef Node parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Expr>             { enum { category = expr_cat};              typedef Node parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Classic>          { enum { category = expr_cat};              typedef Expr parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Name>             { enum { category = name_cat};              typedef Expr parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Type>             { enum { category = type_cat};              typedef Expr parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Udt>              { enum { category = udt_cat};               typedef Type parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Stmt>             { enum { category = stmt_cat};              typedef Expr parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Decl>             { enum { category = decl_cat};              typedef Stmt parent_type; static const size_t precedence =  unspecified_precedence; };

    // ------------------------------------------
    // -- results of name constructor constants --
    // ------------------------------------------
    template <> struct traits<Identifier>       { enum { category = identifier_cat};        typedef Name parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Id_expr>          { enum { category = id_expr_cat};           typedef Name parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Operator>         { enum { category = operator_cat};          typedef Name parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Conversion>       { enum { category = conversion_cat};        typedef Name parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Scope_ref>        { enum { category = scope_ref_cat};         typedef Name parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Template_id>      { enum { category = template_id_cat};       typedef Name parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Type_id>          { enum { category = type_id_cat};           typedef Name parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Ctor_name>        { enum { category = ctor_name_cat};         typedef Name parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Dtor_name>        { enum { category = dtor_name_cat};         typedef Name parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Rname>            { enum { category = rname_cat};             typedef Name parent_type; static const size_t precedence =  unspecified_precedence; };

    // -------------------------------------------
    // -- results of type constructor constants --
    // -------------------------------------------
    template <> struct traits<Array>            { enum { category = array_cat};             typedef Type parent_type; static const size_t precedence =  2; };
    template <> struct traits<As_type>          { enum { category = as_type_cat};           typedef Type parent_type; static const size_t precedence = 20; };
    template <> struct traits<Class>            { enum { category = class_cat};             typedef Udt  parent_type; static const size_t precedence = 20; };
    template <> struct traits<Decltype>         { enum { category = decltype_cat};          typedef Type parent_type; static const size_t precedence = 20; };
    template <> struct traits<Enum>             { enum { category = enum_cat};              typedef Udt  parent_type; static const size_t precedence = 20; };
    template <> struct traits<Function>         { enum { category = function_cat};          typedef Type parent_type; static const size_t precedence =  2; };
    template <> struct traits<Namespace>        { enum { category = namespace_cat};         typedef Udt  parent_type; static const size_t precedence =  0; };
    template <> struct traits<Pointer>          { enum { category = pointer_cat};           typedef Type parent_type; static const size_t precedence =  3; };
    template <> struct traits<Ptr_to_member>    { enum { category = ptr_to_member_cat};     typedef Type parent_type; static const size_t precedence =  4; };
    template <> struct traits<Product>          { enum { category = product_cat};           typedef Type parent_type; static const size_t precedence =  0; };
    template <> struct traits<Qualified>        { enum { category = qualified_cat};         typedef Type parent_type; static const size_t precedence = 20; };
    template <> struct traits<Reference>        { enum { category = reference_cat};         typedef Type parent_type; static const size_t precedence =  3; };
    template <> struct traits<Rvalue_reference> { enum { category = rvalue_reference_cat};  typedef Type parent_type; static const size_t precedence =  3; };
    template <> struct traits<Sum>              { enum { category = sum_cat};               typedef Type parent_type; static const size_t precedence =  0; };
    template <> struct traits<Template>         { enum { category = template_cat};          typedef Type parent_type; static const size_t precedence = 20; };
    template <> struct traits<Union>            { enum { category = union_cat};             typedef Udt  parent_type; static const size_t precedence = 20; };

    // --------------------------------------------------------
    // -- results of nullar expression constructor constants --
    // --------------------------------------------------------
    template <> struct traits<Phantom>          { enum { category = phantom_cat};           typedef Expr parent_type; static const size_t precedence =  unspecified_precedence; };

    // -------------------------------------------------------
    // -- results of unary expression constructor constants --
    // -------------------------------------------------------
    template <> struct traits<Address>          { enum { category = address_cat};           typedef Classic parent_type; static const size_t precedence =  3; };
    template <> struct traits<Array_delete>     { enum { category = array_delete_cat};      typedef Classic parent_type; static const size_t precedence =  3; };
    template <> struct traits<Complement>       { enum { category = complement_cat};        typedef Classic parent_type; static const size_t precedence =  3; };
    template <> struct traits<Delete>           { enum { category = delete_cat};            typedef Classic parent_type; static const size_t precedence =  3; };
    template <> struct traits<Deref>            { enum { category = deref_cat};             typedef Classic parent_type; static const size_t precedence =  3; };
    template <> struct traits<Expr_list>        { enum { category = expr_list_cat};         typedef Expr    parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Expr_sizeof>      { enum { category = expr_sizeof_cat};       typedef Classic parent_type; static const size_t precedence =  3; };
    template <> struct traits<Expr_typeid>      { enum { category = expr_typeid_cat};       typedef Classic parent_type; static const size_t precedence =  2; };
    template <> struct traits<Label>            { enum { category = label_cat};             typedef Expr    parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Not>              { enum { category = not_cat};               typedef Classic parent_type; static const size_t precedence =  3; };
    template <> struct traits<Paren_expr>       { enum { category = paren_expr_cat};        typedef Classic parent_type; static const size_t precedence =  2; };
    template <> struct traits<Post_decrement>   { enum { category = post_decrement_cat};    typedef Classic parent_type; static const size_t precedence =  2; };
    template <> struct traits<Post_increment>   { enum { category = post_increment_cat};    typedef Classic parent_type; static const size_t precedence =  2; };
    template <> struct traits<Pre_decrement>    { enum { category = pre_decrement_cat};     typedef Classic parent_type; static const size_t precedence =  3; };
    template <> struct traits<Pre_increment>    { enum { category = pre_increment_cat};     typedef Classic parent_type; static const size_t precedence =  3; };
    template <> struct traits<Throw>            { enum { category = throw_cat};             typedef Classic parent_type; static const size_t precedence = 17; };
    template <> struct traits<Type_sizeof>      { enum { category = type_sizeof_cat};       typedef Classic parent_type; static const size_t precedence =  3; };
    template <> struct traits<Type_typeid>      { enum { category = type_typeid_cat};       typedef Classic parent_type; static const size_t precedence =  2; };
    template <> struct traits<Unary_minus>      { enum { category = unary_minus_cat};       typedef Classic parent_type; static const size_t precedence =  3; };
    template <> struct traits<Unary_plus>       { enum { category = unary_plus_cat};        typedef Classic parent_type; static const size_t precedence =  3; };

    // --------------------------------------------------------
    // -- results of binary expression constructor constants --
    // --------------------------------------------------------
    template <> struct traits<And>              { enum { category = and_cat};               typedef Classic parent_type; static const size_t precedence = 13; };
    template <> struct traits<Array_ref>        { enum { category = array_ref_cat};         typedef Classic parent_type; static const size_t precedence =  2; };
    template <> struct traits<Arrow>            { enum { category = arrow_cat};             typedef Classic parent_type; static const size_t precedence =  2; };
    template <> struct traits<Arrow_star>       { enum { category = arrow_star_cat};        typedef Classic parent_type; static const size_t precedence =  4; };
    template <> struct traits<Assign>           { enum { category = assign_cat};            typedef Classic parent_type; static const size_t precedence = 16; };
    template <> struct traits<Bitand>           { enum { category = bitand_cat};            typedef Classic parent_type; static const size_t precedence = 10; };
    template <> struct traits<Bitand_assign>    { enum { category = bitand_assign_cat};     typedef Classic parent_type; static const size_t precedence = 16; };
    template <> struct traits<Bitor>            { enum { category = bitor_cat};             typedef Classic parent_type; static const size_t precedence = 12; };
    template <> struct traits<Bitor_assign>     { enum { category = bitor_assign_cat};      typedef Classic parent_type; static const size_t precedence = 16; };
    template <> struct traits<Bitxor>           { enum { category = bitxor_cat};            typedef Classic parent_type; static const size_t precedence = 11; };
    template <> struct traits<Bitxor_assign>    { enum { category = bitxor_assign_cat};     typedef Classic parent_type; static const size_t precedence = 16; };
    template <> struct traits<Cast>             { enum { category = cast_cat};              typedef Classic parent_type; static const size_t precedence =  3; };
    template <> struct traits<Call>             { enum { category = call_cat};              typedef Classic parent_type; static const size_t precedence =  2; };
    template <> struct traits<Comma>            { enum { category = comma_cat};             typedef Classic parent_type; static const size_t precedence = 18; };
    template <> struct traits<Const_cast>       { enum { category = const_cast_cat};        typedef Classic parent_type; static const size_t precedence =  2; };
    template <> struct traits<Datum>            { enum { category = datum_cat};             typedef Classic parent_type; static const size_t precedence =  2; };
    template <> struct traits<Div>              { enum { category = div_cat};               typedef Classic parent_type; static const size_t precedence =  5; };
    template <> struct traits<Div_assign>       { enum { category = div_assign_cat};        typedef Classic parent_type; static const size_t precedence = 16; };
    template <> struct traits<Dot>              { enum { category = dot_cat};               typedef Classic parent_type; static const size_t precedence =  2; };
    template <> struct traits<Dot_star>         { enum { category = dot_star_cat};          typedef Classic parent_type; static const size_t precedence =  4; };
    template <> struct traits<Dynamic_cast>     { enum { category = dynamic_cast_cat};      typedef Classic parent_type; static const size_t precedence =  2; };
    template <> struct traits<Equal>            { enum { category = equal_cat};             typedef Classic parent_type; static const size_t precedence =  9; };
    template <> struct traits<Greater>          { enum { category = greater_cat};           typedef Classic parent_type; static const size_t precedence =  8; };
    template <> struct traits<Greater_equal>    { enum { category = greater_equal_cat};     typedef Classic parent_type; static const size_t precedence =  8; };
    template <> struct traits<Less>             { enum { category = less_cat};              typedef Classic parent_type; static const size_t precedence =  8; };
    template <> struct traits<Less_equal>       { enum { category = less_equal_cat};        typedef Classic parent_type; static const size_t precedence =  8; };
    template <> struct traits<Literal>          { enum { category = literal_cat};           typedef Classic parent_type; static const size_t precedence =  0; };
    template <> struct traits<Lshift>           { enum { category = lshift_cat};            typedef Classic parent_type; static const size_t precedence =  7; };
    template <> struct traits<Lshift_assign>    { enum { category = lshift_assign_cat};     typedef Classic parent_type; static const size_t precedence = 16; };
    template <> struct traits<Member_init>      { enum { category = member_init_cat};       typedef Expr    parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Minus>            { enum { category = minus_cat};             typedef Classic parent_type; static const size_t precedence =  6; };
    template <> struct traits<Minus_assign>     { enum { category = minus_assign_cat};      typedef Classic parent_type; static const size_t precedence = 16; };
    template <> struct traits<Modulo>           { enum { category = modulo_cat};            typedef Classic parent_type; static const size_t precedence =  5; };
    template <> struct traits<Modulo_assign>    { enum { category = modulo_assign_cat};     typedef Classic parent_type; static const size_t precedence = 16; };
    template <> struct traits<Mul>              { enum { category = mul_cat};               typedef Classic parent_type; static const size_t precedence =  5; };
    template <> struct traits<Mul_assign>       { enum { category = mul_assign_cat};        typedef Classic parent_type; static const size_t precedence = 16; };
    template <> struct traits<Not_equal>        { enum { category = not_equal_cat};         typedef Classic parent_type; static const size_t precedence =  9; };
    template <> struct traits<Or>               { enum { category = or_cat};                typedef Classic parent_type; static const size_t precedence = 14; };
    template <> struct traits<Plus>             { enum { category = plus_cat};              typedef Classic parent_type; static const size_t precedence =  6; };
    template <> struct traits<Plus_assign>      { enum { category = plus_assign_cat};       typedef Classic parent_type; static const size_t precedence = 16; };
    template <> struct traits<Reinterpret_cast> { enum { category = reinterpret_cast_cat};  typedef Classic parent_type; static const size_t precedence =  2; };
    template <> struct traits<Rshift>           { enum { category = rshift_cat};            typedef Classic parent_type; static const size_t precedence =  7; };
    template <> struct traits<Rshift_assign>    { enum { category = rshift_assign_cat};     typedef Classic parent_type; static const size_t precedence = 16; };
    template <> struct traits<Static_cast>      { enum { category = static_cast_cat};       typedef Classic parent_type; static const size_t precedence =  2; };

    // --------------------------------------------------------
    // -- result of trinary expression constructor constants --
    // --------------------------------------------------------
    template <> struct traits<Conditional>      { enum { category = conditional_cat};       typedef Classic parent_type; static const size_t precedence = 15; };
    template <> struct traits<New>              { enum { category = new_cat};               typedef Classic parent_type; static const size_t precedence =  3; };

    // -----------------------------------------------
    // -- result of statement constructor constants --
    // -----------------------------------------------
    template <> struct traits<Block>            { enum { category = block_cat};             typedef Stmt parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Break>            { enum { category = break_cat};             typedef Stmt parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Continue>         { enum { category = continue_cat};          typedef Stmt parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Ctor_body>        { enum { category = ctor_body_cat};         typedef Stmt parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Do>               { enum { category = do_cat};                typedef Stmt parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Expr_stmt>        { enum { category = expr_stmt_cat};         typedef Stmt parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Empty_stmt>       { enum { category = empty_stmt_cat};        typedef Expr_stmt parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<For>              { enum { category = for_cat};               typedef Stmt parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<For_in>           { enum { category = for_in_cat};            typedef Stmt parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Goto>             { enum { category = goto_cat};              typedef Stmt parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Handler>          { enum { category = handler_cat};           typedef Stmt parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<If_then>          { enum { category = if_then_cat};           typedef Stmt parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<If_then_else>     { enum { category = if_then_else_cat};      typedef Stmt parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Labeled_stmt>     { enum { category = labeled_stmt_cat};      typedef Stmt parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Return>           { enum { category = return_cat};            typedef Stmt parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Switch>           { enum { category = switch_cat};            typedef Stmt parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<While>            { enum { category = while_cat};             typedef Stmt parent_type; static const size_t precedence =  unspecified_precedence; };

    // -------------------------------------------------
    // -- result of declaration constructor constants --
    // -------------------------------------------------
    template <> struct traits<Alias>            { enum { category = alias_cat};             typedef Decl parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Asm>              { enum { category = asm_cat};               typedef Decl parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Base_type>        { enum { category = base_type_cat};         typedef Decl parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Enumerator>       { enum { category = enumerator_cat};        typedef Decl parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Field>            { enum { category = field_cat};             typedef Decl parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Bitfield>         { enum { category = bitfield_cat};          typedef Decl parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Fundecl>          { enum { category = fundecl_cat};           typedef Decl parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Named_map>        { enum { category = named_map_cat};         typedef Decl parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Parameter>        { enum { category = parameter_cat};         typedef Decl parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Typedecl>         { enum { category = typedecl_cat};          typedef Decl parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Var>              { enum { category = var_cat};               typedef Decl parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Using_directive>  { enum { category = using_directive};       typedef Decl parent_type; static const size_t precedence =  unspecified_precedence; };

    // ------------------------
    // -- distinguished node --
    // ------------------------
    template <> struct traits<Unit>             { enum { category = unit_cat};              typedef Node parent_type; static const size_t precedence =  unspecified_precedence; };
    
    // --------------------------------
    // -- Special built-in constants --
    // --------------------------------
    template <> struct traits<Void>             { enum { category = as_type_cat};           typedef As_type parent_type; static const size_t precedence = 20; };
    template <> struct traits<Bool>             { enum { category = as_type_cat};           typedef As_type parent_type; static const size_t precedence = 20; };
    template <> struct traits<Char>             { enum { category = as_type_cat};           typedef As_type parent_type; static const size_t precedence = 20; };
    template <> struct traits<sChar>            { enum { category = as_type_cat};           typedef As_type parent_type; static const size_t precedence = 20; };
    template <> struct traits<uChar>            { enum { category = as_type_cat};           typedef As_type parent_type; static const size_t precedence = 20; };
    template <> struct traits<Wchar_t>          { enum { category = as_type_cat};           typedef As_type parent_type; static const size_t precedence = 20; };
    template <> struct traits<Short>            { enum { category = as_type_cat};           typedef As_type parent_type; static const size_t precedence = 20; };
    template <> struct traits<uShort>           { enum { category = as_type_cat};           typedef As_type parent_type; static const size_t precedence = 20; };
    template <> struct traits<Int>              { enum { category = as_type_cat};           typedef As_type parent_type; static const size_t precedence = 20; };
    template <> struct traits<uInt>             { enum { category = as_type_cat};           typedef As_type parent_type; static const size_t precedence = 20; };
    template <> struct traits<Long>             { enum { category = as_type_cat};           typedef As_type parent_type; static const size_t precedence = 20; };
    template <> struct traits<uLong>            { enum { category = as_type_cat};           typedef As_type parent_type; static const size_t precedence = 20; };
    template <> struct traits<Long_long>        { enum { category = as_type_cat};           typedef As_type parent_type; static const size_t precedence = 20; };
    template <> struct traits<uLong_long>       { enum { category = as_type_cat};           typedef As_type parent_type; static const size_t precedence = 20; };
    template <> struct traits<Float>            { enum { category = as_type_cat};           typedef As_type parent_type; static const size_t precedence = 20; };
    template <> struct traits<Double>           { enum { category = as_type_cat};           typedef As_type parent_type; static const size_t precedence = 20; };
    template <> struct traits<Long_double>      { enum { category = as_type_cat};           typedef As_type parent_type; static const size_t precedence = 20; };
    template <> struct traits<Ellipsis>         { enum { category = as_type_cat};           typedef As_type parent_type; static const size_t precedence = 20; };

    template <> struct traits<C_linkage>        { enum { category = linkage_cat};           typedef Linkage parent_type; static const size_t precedence =  unspecified_precedence; };
    template <> struct traits<Cxx_linkage>      { enum { category = linkage_cat};           typedef Linkage parent_type; static const size_t precedence =  unspecified_precedence; };

    // ----------------------------------
    // -- built-ins, but not constants --
    // ----------------------------------
    template <> struct traits<Global_scope>     { enum { category = global_scope_cat};      typedef Namespace parent_type; static const size_t precedence =  unspecified_precedence; };

}

namespace ipr
{
    template <class T, class C = void>
    struct precedence 
    {
        static const size_t value = traits<T>::precedence; 
    };
}
