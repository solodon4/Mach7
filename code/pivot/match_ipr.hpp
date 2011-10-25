///
/// \file match_ipr.hpp
///
/// This file defines pattern matching bindings for Pivot's IPR hierarchy.
///
/// \author Yuriy Solodkyy
/// Copyright (C) 2011, Texas A&M University.  All rights reserved.
///

#pragma once

#include "match_generic.hpp"
#include <ipr/interface>  // Pivot interfaces

using namespace ipr;

//template <Category_code c, class T>
//Category<c,T> get_category_helper(const Category<c,T>&);
//
//template <typename T>                    struct get_category_ex;
//template <Category_code c, class T> struct get_category_ex<Category<c,T>> { enum { value = c }; };
//template <typename T>                    struct get_category : get_category_ex<decltype(get_category_helper(*static_cast<const T*>(0)))> {};

template <> struct match_members<Node>             { BCS(Node);                                           KV(unknown_cat); KS(Node::category); CM(0,Node::category);   CM(1,Node::node_id); };

template <> struct match_members<Annotation>       { BCS(Annotation,                               Node); KV(annotation_cat);        CM(0,Annotation::name); CM(1,Annotation::value); };
template <> struct match_members<Region>           { BCS(Region,                                   Node); KV(region_cat);            CM(0,Region::owner);    CM(1,Region::enclosing); CM(2,Region::bindings); CM(3,Region::span); };
template <> struct match_members<Comment>          { BCS(Comment,                                  Node); KV(comment_cat);           CM(0,Comment::text); };
template <> struct match_members<String>           { BCS(String,                                   Node); KV(string_cat);            CM(0,String::begin);    CM(1,String::size); };
template <> struct match_members<Expr>             { BCS(Expr,                                     Node); KV(expr_cat); KS(Expr::category); CM(0,Expr::type); };
template <> struct match_members<Mapping>          { BCS(Mapping,                            Expr, Node); KV(mapping_cat);           CM(0,Mapping::params);  CM(1,Mapping::result_type); };

template <> struct match_members<Overload>         { BCS(Overload,                           Expr, Node); KV(overload_cat);         };
template <> struct match_members<Parameter_list>   { BCS(Parameter_list,                   Region, Node); KV(parameter_list_cat);   };
template <> struct match_members<Scope>            { BCS(Scope,                              Expr, Node); KV(scope_cat);             CM(0,Scope::members); };

// ------------------------------------------
// -- general constructors                 --
// ------------------------------------------
template <> struct match_members<Linkage>          { BCS(Linkage,                                  Node); KV(linkage_cat);           CM(0,Linkage::language); };
//template <> struct match_members<Expr>           { BCS(Expr,                                     Node); KV(expr_cat); KS(Expr::category); CM(0,Expr::type); };
template <> struct match_members<Classic>          { BCS(Classic,                            Expr, Node); KV(expr_cat); KS(Classic::category); CM(0,Expr::type); CM(1,Classic::has_impl_decl); CM(2,Classic::impl_decl); };
template <> struct match_members<Name>             { BCS(Name,                               Expr, Node); KV(name_cat); KS(Name::category); CM(0,Expr::type); };
template <> struct match_members<Type>             { BCS(Type,                               Expr, Node); KV(type_cat); KS(Type::category); CM(0,Type::name); };
template <> struct match_members<Udt>              { BCS(Udt,                          Type, Expr, Node); KV(udt_cat);  KS(Udt::category);  CM(0,Type::name); CM(1,Udt::region); CM(2,Udt::scope);};
template <> struct match_members<Stmt>             { BCS(Stmt,                               Expr, Node); KV(stmt_cat); KS(Stmt::category); };
template <> struct match_members<Decl>             { BCS(Decl,                         Stmt, Expr, Node); KV(decl_cat); KS(Decl::category); CM(0,Decl::name); CM(1,Decl::type);  CM(2,Decl::initializer); };

// ------------------------------------------
// -- results of name constructor constants --
// ------------------------------------------
template <> struct match_members<Identifier>       { BCS(Identifier,                   Name, Expr, Node); KV(identifier_cat);        CM(0,Identifier::string); };
template <> struct match_members<Id_expr>          { BCS(Id_expr,                      Name, Expr, Node); KV(id_expr_cat);           CM(0,Id_expr::name); CM(1,Id_expr::resolution); };
template <> struct match_members<Operator>         { BCS(Operator,                     Name, Expr, Node); KV(operator_cat);          CM(0,Operator::opname); };
template <> struct match_members<Conversion>       { BCS(Conversion,                   Name, Expr, Node); KV(conversion_cat);        CM(0,Conversion::target); };
template <> struct match_members<Scope_ref>        { BCS(Scope_ref,                    Name, Expr, Node); KV(scope_ref_cat);         CM(0,Scope_ref::scope); CM(1,Scope_ref::member); };
template <> struct match_members<Template_id>      { BCS(Template_id,                  Name, Expr, Node); KV(template_id_cat);       CM(0,Template_id::template_name); CM(1,Template_id::args); };
template <> struct match_members<Type_id>          { BCS(Type_id,                      Name, Expr, Node); KV(type_id_cat);           CM(0,Type_id::type_expr); };
template <> struct match_members<Ctor_name>        { BCS(Ctor_name,                    Name, Expr, Node); KV(ctor_name_cat);         CM(0,Ctor_name::object_type); };
template <> struct match_members<Dtor_name>        { BCS(Dtor_name,                    Name, Expr, Node); KV(dtor_name_cat);         CM(0,Dtor_name::object_type); };
template <> struct match_members<Rname>            { BCS(Rname,                        Name, Expr, Node); KV(rname_cat);             CM(0,Rname::level); CM(1,Rname::position); };

// -------------------------------------------
// -- results of type constructor constants --
// -------------------------------------------
template <> struct match_members<Array>            { BCS(Array,                        Type, Expr, Node); KV(array_cat);             CM(0,Array::element_type); CM(1,Array::bound); };
template <> struct match_members<As_type>          { BCS(As_type,                      Type, Expr, Node); KV(as_type_cat);           CM(0,As_type::expr); };
template <> struct match_members<Class>            { BCS(Class,                   Udt, Type, Expr, Node); KV(class_cat);             CM(0,Class::members);      CM(1,Class::bases); };
template <> struct match_members<Decltype>         { BCS(Decltype,                     Type, Expr, Node); KV(decltype_cat);          CM(0,Decltype::expr); };
template <> struct match_members<Enum>             { BCS(Enum,                    Udt, Type, Expr, Node); KV(enum_cat);              CM(0,Enum::members); };
template <> struct match_members<Function>         { BCS(Function,                     Type, Expr, Node); KV(function_cat);          CM(0,Function::source);    CM(1,Function::target); CM(2,Function::throws); };
template <> struct match_members<Namespace>        { BCS(Namespace,               Udt, Type, Expr, Node); KV(namespace_cat);         CM(0,Namespace::members); };
template <> struct match_members<Pointer>          { BCS(Pointer,                      Type, Expr, Node); KV(pointer_cat);           CM(0,Pointer::points_to); };
template <> struct match_members<Ptr_to_member>    { BCS(Ptr_to_member,                Type, Expr, Node); KV(ptr_to_member_cat);     CM(0,Ptr_to_member::containing_type); CM(1,Ptr_to_member::member_type); };
template <> struct match_members<Product>          { BCS(Product,                      Type, Expr, Node); KV(product_cat);           CM(0,Product::elements); };
template <> struct match_members<Qualified>        { BCS(Qualified,                    Type, Expr, Node); KV(qualified_cat);         CM(0,Qualified::main_variant); CM(1,Qualified::qualifiers); };
template <> struct match_members<Reference>        { BCS(Reference,                    Type, Expr, Node); KV(reference_cat);         CM(0,Reference::refers_to); };
template <> struct match_members<Rvalue_reference> { BCS(Rvalue_reference,             Type, Expr, Node); KV(rvalue_reference_cat);  CM(0,Rvalue_reference::refers_to); };
template <> struct match_members<Sum>              { BCS(Sum,                          Type, Expr, Node); KV(sum_cat);               CM(0,Sum::elements); };
template <> struct match_members<Template>         { BCS(Template,                     Type, Expr, Node); KV(template_cat);          CM(0,Template::source); CM(1,Template::target); };
template <> struct match_members<Union>            { BCS(Union,                   Udt, Type, Expr, Node); KV(union_cat);             CM(0,Union::members); };

// --------------------------------------------------------
// -- results of nullar expression constructor constants --
// --------------------------------------------------------
template <> struct match_members<Phantom>          { BCS(Phantom,                            Expr, Node); KV(phantom_cat);     };

// -------------------------------------------------------
// -- results of unary expression constructor constants --
// -------------------------------------------------------
template <> struct match_members<Address>          { BCS(Address,                   Classic, Expr, Node); KV(address_cat);           CM(0,Address::operand); };
template <> struct match_members<Array_delete>     { BCS(Array_delete,              Classic, Expr, Node); KV(array_delete_cat);      CM(0,Array_delete::operand); };
template <> struct match_members<Complement>       { BCS(Complement,                Classic, Expr, Node); KV(complement_cat);        CM(0,Complement::operand); };
template <> struct match_members<Delete>           { BCS(Delete,                    Classic, Expr, Node); KV(delete_cat);            CM(0,Delete::operand); };
template <> struct match_members<Deref>            { BCS(Deref,                     Classic, Expr, Node); KV(deref_cat);             CM(0,Deref::operand); };
template <> struct match_members<Expr_list>        { BCS(Expr_list,                          Expr, Node); KV(expr_list_cat);         CM(0,Expr_list::operand); };
template <> struct match_members<Expr_sizeof>      { BCS(Expr_sizeof,               Classic, Expr, Node); KV(expr_sizeof_cat);       CM(0,Expr_sizeof::operand); };
template <> struct match_members<Expr_typeid>      { BCS(Expr_typeid,               Classic, Expr, Node); KV(expr_typeid_cat);       CM(0,Expr_typeid::operand); };
template <> struct match_members<Label>            { BCS(Label,                              Expr, Node); KV(label_cat);             CM(0,Label::operand); };
template <> struct match_members<Not>              { BCS(Not,                       Classic, Expr, Node); KV(not_cat);               CM(0,Not::operand); };
template <> struct match_members<Paren_expr>       { BCS(Paren_expr,                Classic, Expr, Node); KV(paren_expr_cat);        CM(0,Paren_expr::operand); };
template <> struct match_members<Post_decrement>   { BCS(Post_decrement,            Classic, Expr, Node); KV(post_decrement_cat);    CM(0,Post_decrement::operand); };
template <> struct match_members<Post_increment>   { BCS(Post_increment,            Classic, Expr, Node); KV(post_increment_cat);    CM(0,Post_increment::operand); };
template <> struct match_members<Pre_decrement>    { BCS(Pre_decrement,             Classic, Expr, Node); KV(pre_decrement_cat);     CM(0,Pre_decrement::operand); };
template <> struct match_members<Pre_increment>    { BCS(Pre_increment,             Classic, Expr, Node); KV(pre_increment_cat);     CM(0,Pre_increment::operand); };
template <> struct match_members<Throw>            { BCS(Throw,                     Classic, Expr, Node); KV(throw_cat);             CM(0,Throw::operand); };
template <> struct match_members<Type_sizeof>      { BCS(Type_sizeof,               Classic, Expr, Node); KV(type_sizeof_cat);       CM(0,Type_sizeof::operand); };
template <> struct match_members<Type_typeid>      { BCS(Type_typeid,               Classic, Expr, Node); KV(type_typeid_cat);       CM(0,Type_typeid::operand); };
template <> struct match_members<Unary_minus>      { BCS(Unary_minus,               Classic, Expr, Node); KV(unary_minus_cat);       CM(0,Unary_minus::operand); };
template <> struct match_members<Unary_plus>       { BCS(Unary_plus,                Classic, Expr, Node); KV(unary_plus_cat);        CM(0,Unary_plus::operand); };

// --------------------------------------------------------
// -- results of binary expression constructor constants --
// --------------------------------------------------------
template <> struct match_members<And>              { BCS(And,                       Classic, Expr, Node); KV(and_cat);               CM(0,And::first);              CM(1,And::second); };
template <> struct match_members<Array_ref>        { BCS(Array_ref,                 Classic, Expr, Node); KV(array_ref_cat);         CM(0,Array_ref::first);        CM(1,Array_ref::second); };
template <> struct match_members<Arrow>            { BCS(Arrow,                     Classic, Expr, Node); KV(arrow_cat);             CM(0,Arrow::first);            CM(1,Arrow::second); };
template <> struct match_members<Arrow_star>       { BCS(Arrow_star,                Classic, Expr, Node); KV(arrow_star_cat);        CM(0,Arrow_star::first);       CM(1,Arrow_star::second); };
template <> struct match_members<Assign>           { BCS(Assign,                    Classic, Expr, Node); KV(assign_cat);            CM(0,Assign::first);           CM(1,Assign::second); };
template <> struct match_members<Bitand>           { BCS(Bitand,                    Classic, Expr, Node); KV(bitand_cat);            CM(0,Bitand::first);           CM(1,Bitand::second); };
template <> struct match_members<Bitand_assign>    { BCS(Bitand_assign,             Classic, Expr, Node); KV(bitand_assign_cat);     CM(0,Bitand_assign::first);    CM(1,Bitand_assign::second); };
template <> struct match_members<Bitor>            { BCS(Bitor,                     Classic, Expr, Node); KV(bitor_cat);             CM(0,Bitor::first);            CM(1,Bitor::second); };
template <> struct match_members<Bitor_assign>     { BCS(Bitor_assign,              Classic, Expr, Node); KV(bitor_assign_cat);      CM(0,Bitor_assign::first);     CM(1,Bitor_assign::second); };
template <> struct match_members<Bitxor>           { BCS(Bitxor,                    Classic, Expr, Node); KV(bitxor_cat);            CM(0,Bitxor::first);           CM(1,Bitxor::second); };
template <> struct match_members<Bitxor_assign>    { BCS(Bitxor_assign,             Classic, Expr, Node); KV(bitxor_assign_cat);     CM(0,Bitxor_assign::first);    CM(1,Bitxor_assign::second); };
template <> struct match_members<Cast>             { BCS(Cast,                      Classic, Expr, Node); KV(cast_cat);              CM(0,Cast::first);             CM(1,Cast::second); };
template <> struct match_members<Call>             { BCS(Call,                      Classic, Expr, Node); KV(call_cat);              CM(0,Call::first);             CM(1,Call::second); };
template <> struct match_members<Comma>            { BCS(Comma,                     Classic, Expr, Node); KV(comma_cat);             CM(0,Comma::first);            CM(1,Comma::second); };
template <> struct match_members<Const_cast>       { BCS(Const_cast,                Classic, Expr, Node); KV(const_cast_cat);        CM(0,Const_cast::first);       CM(1,Const_cast::second); };
template <> struct match_members<Datum>            { BCS(Datum,                     Classic, Expr, Node); KV(datum_cat);             CM(0,Datum::first);            CM(1,Datum::second); };
template <> struct match_members<Div>              { BCS(Div,                       Classic, Expr, Node); KV(div_cat);               CM(0,Div::first);              CM(1,Div::second); };
template <> struct match_members<Div_assign>       { BCS(Div_assign,                Classic, Expr, Node); KV(div_assign_cat);        CM(0,Div_assign::first);       CM(1,Div_assign::second); };
template <> struct match_members<Dot>              { BCS(Dot,                       Classic, Expr, Node); KV(dot_cat);               CM(0,Dot::first);              CM(1,Dot::second); };
template <> struct match_members<Dot_star>         { BCS(Dot_star,                  Classic, Expr, Node); KV(dot_star_cat);          CM(0,Dot_star::first);         CM(1,Dot_star::second); };
template <> struct match_members<Dynamic_cast>     { BCS(Dynamic_cast,              Classic, Expr, Node); KV(dynamic_cast_cat);      CM(0,Dynamic_cast::first);     CM(1,Dynamic_cast::second); };
template <> struct match_members<Equal>            { BCS(Equal,                     Classic, Expr, Node); KV(equal_cat);             CM(0,Equal::first);            CM(1,Equal::second); };
template <> struct match_members<Greater>          { BCS(Greater,                   Classic, Expr, Node); KV(greater_cat);           CM(0,Greater::first);          CM(1,Greater::second); };
template <> struct match_members<Greater_equal>    { BCS(Greater_equal,             Classic, Expr, Node); KV(greater_equal_cat);     CM(0,Greater_equal::first);    CM(1,Greater_equal::second); };
template <> struct match_members<Less>             { BCS(Less,                      Classic, Expr, Node); KV(less_cat);              CM(0,Less::first);             CM(1,Less::second); };
template <> struct match_members<Less_equal>       { BCS(Less_equal,                Classic, Expr, Node); KV(less_equal_cat);        CM(0,Less_equal::first);       CM(1,Less_equal::second); };
template <> struct match_members<Literal>          { BCS(Literal,                   Classic, Expr, Node); KV(literal_cat);           CM(0,Literal::first);          CM(1,Literal::second); };
template <> struct match_members<Lshift>           { BCS(Lshift,                    Classic, Expr, Node); KV(lshift_cat);            CM(0,Lshift::first);           CM(1,Lshift::second); };
template <> struct match_members<Lshift_assign>    { BCS(Lshift_assign,             Classic, Expr, Node); KV(lshift_assign_cat);     CM(0,Lshift_assign::first);    CM(1,Lshift_assign::second); };
template <> struct match_members<Member_init>      { BCS(Member_init,                        Expr, Node); KV(member_init_cat);       CM(0,Member_init::member);     CM(1,Member_init::initializer); };
template <> struct match_members<Minus>            { BCS(Minus,                     Classic, Expr, Node); KV(minus_cat);             CM(0,Minus::first);            CM(1,Minus::second); };
template <> struct match_members<Minus_assign>     { BCS(Minus_assign,              Classic, Expr, Node); KV(minus_assign_cat);      CM(0,Minus_assign::first);     CM(1,Minus_assign::second); };
template <> struct match_members<Modulo>           { BCS(Modulo,                    Classic, Expr, Node); KV(modulo_cat);            CM(0,Modulo::first);           CM(1,Modulo::second); };
template <> struct match_members<Modulo_assign>    { BCS(Modulo_assign,             Classic, Expr, Node); KV(modulo_assign_cat);     CM(0,Modulo_assign::first);    CM(1,Modulo_assign::second); };
template <> struct match_members<Mul>              { BCS(Mul,                       Classic, Expr, Node); KV(mul_cat);               CM(0,Mul::first);              CM(1,Mul::second); };
template <> struct match_members<Mul_assign>       { BCS(Mul_assign,                Classic, Expr, Node); KV(mul_assign_cat);        CM(0,Mul_assign::first);       CM(1,Mul_assign::second); };
template <> struct match_members<Not_equal>        { BCS(Not_equal,                 Classic, Expr, Node); KV(not_equal_cat);         CM(0,Not_equal::first);        CM(1,Not_equal::second); };
template <> struct match_members<Or>               { BCS(Or,                        Classic, Expr, Node); KV(or_cat);                CM(0,Or::first);               CM(1,Or::second); };
template <> struct match_members<Plus>             { BCS(Plus,                      Classic, Expr, Node); KV(plus_cat);              CM(0,Plus::first);             CM(1,Plus::second); };
template <> struct match_members<Plus_assign>      { BCS(Plus_assign,               Classic, Expr, Node); KV(plus_assign_cat);       CM(0,Plus_assign::first);      CM(1,Plus_assign::second); };
template <> struct match_members<Reinterpret_cast> { BCS(Reinterpret_cast,          Classic, Expr, Node); KV(reinterpret_cast_cat);  CM(0,Reinterpret_cast::first); CM(1,Reinterpret_cast::second); };
template <> struct match_members<Rshift>           { BCS(Rshift,                    Classic, Expr, Node); KV(rshift_cat);            CM(0,Rshift::first);           CM(1,Rshift::second); };
template <> struct match_members<Rshift_assign>    { BCS(Rshift_assign,             Classic, Expr, Node); KV(rshift_assign_cat);     CM(0,Rshift_assign::first);    CM(1,Rshift_assign::second); };
template <> struct match_members<Static_cast>      { BCS(Static_cast,               Classic, Expr, Node); KV(static_cast_cat);       CM(0,Static_cast::first);      CM(1,Static_cast::second); };

// --------------------------------------------------------
// -- result of trinary expression constructor constants --
// --------------------------------------------------------
template <> struct match_members<Conditional>      { BCS(Conditional,               Classic, Expr, Node); KV(conditional_cat);       CM(0,Conditional::condition);  CM(1,Conditional::then_expr);  CM(2,Conditional::else_expr); };
template <> struct match_members<New>              { BCS(New,                       Classic, Expr, Node); KV(new_cat);               CM(0,New::placement);          CM(1,New::allocated_type);     CM(2,New::initializer); };

// -----------------------------------------------
// -- result of statement constructor constants --
// -----------------------------------------------
template <> struct match_members<Block>            { BCS(Block,                        Stmt, Expr, Node); KV(block_cat);             CM(0,Block::body);             CM(1,Block::handlers);         CM(2,Scope::members); };
template <> struct match_members<Break>            { BCS(Break,                        Stmt, Expr, Node); KV(break_cat);             CM(0,Break::from); };
template <> struct match_members<Continue>         { BCS(Continue,                     Stmt, Expr, Node); KV(continue_cat);          CM(0,Continue::iteration); };
template <> struct match_members<Ctor_body>        { BCS(Ctor_body,                    Stmt, Expr, Node); KV(ctor_body_cat);         CM(0,Ctor_body::inits);        CM(1,Ctor_body::block); };
template <> struct match_members<Do>               { BCS(Do,                           Stmt, Expr, Node); KV(do_cat);                CM(0,Do::condition);           CM(1,Do::body); };
template <> struct match_members<Expr_stmt>        { BCS(Expr_stmt,                    Stmt, Expr, Node); KV(expr_stmt_cat);         CM(0,Expr_stmt::expr); };
template <> struct match_members<Empty_stmt>       { BCS(Empty_stmt,        Expr_stmt, Stmt, Expr, Node); KV(empty_stmt_cat); };
template <> struct match_members<For>              { BCS(For,                          Stmt, Expr, Node); KV(for_cat);               CM(0,For::initializer);        CM(1,For::condition);          CM(2,For::increment); CM(3,For::body); };
template <> struct match_members<For_in>           { BCS(For_in,                       Stmt, Expr, Node); KV(for_in_cat);            CM(0,For_in::variable);        CM(1,For_in::sequence);        CM(2,For_in::body); };
template <> struct match_members<Goto>             { BCS(Goto,                         Stmt, Expr, Node); KV(goto_cat);              CM(0,Goto::target); };
template <> struct match_members<Handler>          { BCS(Handler,                      Stmt, Expr, Node); KV(handler_cat);           CM(0,Handler::exception);      CM(1,Handler::body); };
template <> struct match_members<If_then>          { BCS(If_then,                      Stmt, Expr, Node); KV(if_then_cat);           CM(0,If_then::condition);      CM(1,If_then::then_stmt); };
template <> struct match_members<If_then_else>     { BCS(If_then_else,                 Stmt, Expr, Node); KV(if_then_else_cat);      CM(0,If_then_else::condition); CM(1,If_then_else::then_stmt); CM(2,If_then_else::else_stmt); };
template <> struct match_members<Labeled_stmt>     { BCS(Labeled_stmt,                 Stmt, Expr, Node); KV(labeled_stmt_cat);      CM(0,Labeled_stmt::label);     CM(1,Labeled_stmt::stmt); };
template <> struct match_members<Return>           { BCS(Return,                       Stmt, Expr, Node); KV(return_cat);            CM(0,Return::value); };
template <> struct match_members<Switch>           { BCS(Switch,                       Stmt, Expr, Node); KV(switch_cat);            CM(0,Switch::condition);       CM(1,Switch::body); };
template <> struct match_members<While>            { BCS(While,                        Stmt, Expr, Node); KV(while_cat);             CM(0,While::condition);        CM(1,While::body); };

// -------------------------------------------------
// -- result of declaration constructor constants --
// -------------------------------------------------
template <> struct match_members<Alias>            { BCS(Alias,                  Decl, Stmt, Expr, Node); KV(alias_cat);             CM(0,Decl::name); CM(1,Decl::type); };
template <> struct match_members<Asm>              { BCS(Asm,                    Decl, Stmt, Expr, Node); KV(asm_cat);         };
template <> struct match_members<Base_type>        { BCS(Base_type,              Decl, Stmt, Expr, Node); KV(base_type_cat);         CM(0,Decl::name); CM(1,Decl::type); };
template <> struct match_members<Enumerator>       { BCS(Enumerator,             Decl, Stmt, Expr, Node); KV(enumerator_cat);        CM(0,Decl::name); CM(1,Decl::type); CM(2,Enumerator::membership); };
template <> struct match_members<Field>            { BCS(Field,                  Decl, Stmt, Expr, Node); KV(field_cat);             CM(0,Decl::name); CM(1,Decl::type); CM(2,Field::membership); };
template <> struct match_members<Bitfield>         { BCS(Bitfield,               Decl, Stmt, Expr, Node); KV(bitfield_cat);          CM(0,Decl::name); CM(1,Decl::type); CM(2,Bitfield::membership);  CM(3,Bitfield::precision); };
template <> struct match_members<Fundecl>          { BCS(Fundecl,                Decl, Stmt, Expr, Node); KV(fundecl_cat);           CM(0,Decl::name); CM(1,Decl::type); CM(2,Fundecl::membership);   CM(3,Fundecl::parameters); /*CM(2,Fundecl::definition); CM(3,Fundecl::mapping);*/ };
template <> struct match_members<Named_map>        { BCS(Named_map,              Decl, Stmt, Expr, Node); KV(named_map_cat);         CM(0,Decl::name); CM(1,Decl::type); CM(2,Named_map::params);     CM(3,Named_map::mapping);/*CM(2,Named_map::definition); CM(2,Named_map::primary_named_map);*/ };
template <> struct match_members<Parameter>        { BCS(Parameter,              Decl, Stmt, Expr, Node); KV(parameter_cat);         CM(0,Decl::name); CM(1,Decl::type); CM(2,Parameter::membership); CM(3,Parameter::default_value); };
template <> struct match_members<Typedecl>         { BCS(Typedecl,               Decl, Stmt, Expr, Node); KV(typedecl_cat);          CM(0,Decl::name); CM(1,Decl::type); CM(2,Typedecl::membership);  CM(3,Typedecl::definition); };
template <> struct match_members<Var>              { BCS(Var,                    Decl, Stmt, Expr, Node); KV(var_cat);               CM(0,Decl::name); CM(1,Decl::type); };
template <> struct match_members<Using_directive>  { BCS(Using_directive,        Decl, Stmt, Expr, Node); KV(using_directive); };

// ------------------------
// -- distinguished node --
// ------------------------
template <> struct match_members<Unit>             { BCS(Unit,                                     Node); KV(unit_cat);              CM(0,Unit::get_global_scope); };
                                                            
// --------------------------------
// -- Special built-in constants --
// --------------------------------
template <> struct match_members<Void>             { BCS(Void,                      As_type, Node); KV(as_type_cat); };
template <> struct match_members<Bool>             { BCS(Bool,                      As_type, Node); KV(as_type_cat); };
template <> struct match_members<Char>             { BCS(Char,                      As_type, Node); KV(as_type_cat); };
template <> struct match_members<sChar>            { BCS(sChar,                     As_type, Node); KV(as_type_cat); };
template <> struct match_members<uChar>            { BCS(uChar,                     As_type, Node); KV(as_type_cat); };
template <> struct match_members<Wchar_t>          { BCS(Wchar_t,                   As_type, Node); KV(as_type_cat); };
template <> struct match_members<Short>            { BCS(Short,                     As_type, Node); KV(as_type_cat); };
template <> struct match_members<uShort>           { BCS(uShort,                    As_type, Node); KV(as_type_cat); };
template <> struct match_members<Int>              { BCS(Int,                       As_type, Node); KV(as_type_cat); };
template <> struct match_members<uInt>             { BCS(uInt,                      As_type, Node); KV(as_type_cat); };
template <> struct match_members<Long>             { BCS(Long,                      As_type, Node); KV(as_type_cat); };
template <> struct match_members<uLong>            { BCS(uLong,                     As_type, Node); KV(as_type_cat); };
template <> struct match_members<Long_long>        { BCS(Long_long,                 As_type, Node); KV(as_type_cat); };
template <> struct match_members<uLong_long>       { BCS(uLong_long,                As_type, Node); KV(as_type_cat); };
template <> struct match_members<Float>            { BCS(Float,                     As_type, Node); KV(as_type_cat); };
template <> struct match_members<Double>           { BCS(Double,                    As_type, Node); KV(as_type_cat); };
template <> struct match_members<Long_double>      { BCS(Long_double,               As_type, Node); KV(as_type_cat); };
template <> struct match_members<Ellipsis>         { BCS(Ellipsis,                  As_type, Node); KV(as_type_cat); };

template <> struct match_members<C_linkage>        { BCS(C_linkage,                       Linkage, Node); KV(linkage_cat); };
template <> struct match_members<Cxx_linkage>      { BCS(Cxx_linkage,                     Linkage, Node); KV(linkage_cat); };

// ----------------------------------
// -- built-ins, but not constants --
// ----------------------------------
template <> struct match_members<Global_scope>     { BCS(Global_scope, Namespace, Udt, Type, Expr, Node); KV(global_scope_cat); CM(0,Namespace::members); };

//------------------------------------------------------------------------------
