///
/// \file match_ipr.hpp
///
/// This file defines pattern matching bindings for Pivot's IPR hierarchy.
///
/// \author Yuriy Solodkyy
/// Copyright (C) 2011, Texas A&M University.  All rights reserved.
///
/// \note Commented out lines are not used since we decided not to provide 
///       any bindings for corresponding nodes. They can be uncommented or 
///       deleted without changes in meaning. We only keep them here to 
///       maintain a placeholder for corresponding nodes in case we decide
///       to provide bindings for them later.

#pragma once

#include "match.hpp"
#include <ipr/interface>  // Pivot interfaces

using namespace ipr;

template <> struct bindings<Node>             { CM(0,Node::category);   CM(1,Node::node_id); };

template <> struct bindings<Annotation>       { CM(0,Annotation::name); CM(1,Annotation::value); };
template <> struct bindings<Region>           { CM(0,Region::owner);    CM(1,Region::enclosing); CM(2,Region::bindings); CM(3,Region::span); };
template <> struct bindings<Comment>          { CM(0,Comment::text); };
template <> struct bindings<String>           { CM(0,String::begin);    CM(1,String::size); };
template <> struct bindings<Expr>             { CM(0,Expr::type); };
template <> struct bindings<Mapping>          { CM(0,Mapping::params);  CM(1,Mapping::result_type); };

//template <> struct bindings<Overload>         { };
//template <> struct bindings<Parameter_list>   { };
template <> struct bindings<Scope>            { CM(0,Scope::members); };

// ------------------------------------------
// -- general constructors                 --
// ------------------------------------------
template <> struct bindings<Linkage>          { CM(0,Linkage::language); };
template <> struct bindings<Classic>          { CM(0,Expr::type); CM(1,Classic::has_impl_decl); CM(2,Classic::impl_decl); };
template <> struct bindings<Name>             { CM(0,Expr::type); };
template <> struct bindings<Type>             { CM(0,Type::name); };
template <> struct bindings<Udt>              { CM(0,Type::name); CM(1,Udt::region); CM(2,Udt::scope);};
//template <> struct bindings<Stmt>             { };
template <> struct bindings<Decl>             { CM(0,Decl::name); CM(1,Decl::type);  CM(2,Decl::initializer); };

// ------------------------------------------
// -- results of name constructor constants --
// ------------------------------------------
template <> struct bindings<Identifier>       { CM(0,Identifier::string); };
template <> struct bindings<Id_expr>          { CM(0,Id_expr::name); CM(1,Id_expr::resolution); };
template <> struct bindings<Operator>         { CM(0,Operator::opname); };
template <> struct bindings<Conversion>       { CM(0,Conversion::target); };
template <> struct bindings<Scope_ref>        { CM(0,Scope_ref::scope); CM(1,Scope_ref::member); };
template <> struct bindings<Template_id>      { CM(0,Template_id::template_name); CM(1,Template_id::args); };
template <> struct bindings<Type_id>          { CM(0,Type_id::type_expr); };
template <> struct bindings<Ctor_name>        { CM(0,Ctor_name::object_type); };
template <> struct bindings<Dtor_name>        { CM(0,Dtor_name::object_type); };
template <> struct bindings<Rname>            { CM(0,Rname::level); CM(1,Rname::position); };

// -------------------------------------------
// -- results of type constructor constants --
// -------------------------------------------
template <> struct bindings<Array>            { CM(0,Array::element_type); CM(1,Array::bound); };
template <> struct bindings<As_type>          { CM(0,As_type::expr); };
template <> struct bindings<Class>            { CM(0,Class::members);      CM(1,Class::bases); };
template <> struct bindings<Decltype>         { CM(0,Decltype::expr); };
template <> struct bindings<Enum>             { CM(0,Enum::members); };
template <> struct bindings<Function>         { CM(0,Function::source);    CM(1,Function::target); CM(2,Function::throws); };
template <> struct bindings<Namespace>        { CM(0,Namespace::members); };
template <> struct bindings<Pointer>          { CM(0,Pointer::points_to); };
template <> struct bindings<Ptr_to_member>    { CM(0,Ptr_to_member::containing_type); CM(1,Ptr_to_member::member_type); };
template <> struct bindings<Product>          { CM(0,Product::elements); };
template <> struct bindings<Qualified>        { CM(0,Qualified::main_variant); CM(1,Qualified::qualifiers); };
template <> struct bindings<Reference>        { CM(0,Reference::refers_to); };
template <> struct bindings<Rvalue_reference> { CM(0,Rvalue_reference::refers_to); };
template <> struct bindings<Sum>              { CM(0,Sum::elements); };
template <> struct bindings<Template>         { CM(0,Template::source); CM(1,Template::target); };
template <> struct bindings<Union>            { CM(0,Union::members); };

// --------------------------------------------------------
// -- results of nullar expression constructor constants --
// --------------------------------------------------------
//template <> struct bindings<Phantom>          { };

// -------------------------------------------------------
// -- results of unary expression constructor constants --
// -------------------------------------------------------
template <> struct bindings<Address>          { CM(0,Address::operand); };
template <> struct bindings<Array_delete>     { CM(0,Array_delete::operand); };
template <> struct bindings<Complement>       { CM(0,Complement::operand); };
template <> struct bindings<Delete>           { CM(0,Delete::operand); };
template <> struct bindings<Deref>            { CM(0,Deref::operand); };
template <> struct bindings<Expr_list>        { CM(0,Expr_list::operand); };
template <> struct bindings<Expr_sizeof>      { CM(0,Expr_sizeof::operand); };
template <> struct bindings<Expr_typeid>      { CM(0,Expr_typeid::operand); };
template <> struct bindings<Label>            { CM(0,Label::operand); };
template <> struct bindings<Not>              { CM(0,Not::operand); };
template <> struct bindings<Paren_expr>       { CM(0,Paren_expr::operand); };
template <> struct bindings<Post_decrement>   { CM(0,Post_decrement::operand); };
template <> struct bindings<Post_increment>   { CM(0,Post_increment::operand); };
template <> struct bindings<Pre_decrement>    { CM(0,Pre_decrement::operand); };
template <> struct bindings<Pre_increment>    { CM(0,Pre_increment::operand); };
template <> struct bindings<Throw>            { CM(0,Throw::operand); };
template <> struct bindings<Type_sizeof>      { CM(0,Type_sizeof::operand); };
template <> struct bindings<Type_typeid>      { CM(0,Type_typeid::operand); };
template <> struct bindings<Unary_minus>      { CM(0,Unary_minus::operand); };
template <> struct bindings<Unary_plus>       { CM(0,Unary_plus::operand); };

// --------------------------------------------------------
// -- results of binary expression constructor constants --
// --------------------------------------------------------
template <> struct bindings<And>              { CM(0,And::first);              CM(1,And::second); };
template <> struct bindings<Array_ref>        { CM(0,Array_ref::first);        CM(1,Array_ref::second); };
template <> struct bindings<Arrow>            { CM(0,Arrow::first);            CM(1,Arrow::second); };
template <> struct bindings<Arrow_star>       { CM(0,Arrow_star::first);       CM(1,Arrow_star::second); };
template <> struct bindings<Assign>           { CM(0,Assign::first);           CM(1,Assign::second); };
template <> struct bindings<Bitand>           { CM(0,Bitand::first);           CM(1,Bitand::second); };
template <> struct bindings<Bitand_assign>    { CM(0,Bitand_assign::first);    CM(1,Bitand_assign::second); };
template <> struct bindings<Bitor>            { CM(0,Bitor::first);            CM(1,Bitor::second); };
template <> struct bindings<Bitor_assign>     { CM(0,Bitor_assign::first);     CM(1,Bitor_assign::second); };
template <> struct bindings<Bitxor>           { CM(0,Bitxor::first);           CM(1,Bitxor::second); };
template <> struct bindings<Bitxor_assign>    { CM(0,Bitxor_assign::first);    CM(1,Bitxor_assign::second); };
template <> struct bindings<Cast>             { CM(0,Cast::first);             CM(1,Cast::second); };
template <> struct bindings<Call>             { CM(0,Call::first);             CM(1,Call::second); };
template <> struct bindings<Comma>            { CM(0,Comma::first);            CM(1,Comma::second); };
template <> struct bindings<Const_cast>       { CM(0,Const_cast::first);       CM(1,Const_cast::second); };
template <> struct bindings<Datum>            { CM(0,Datum::first);            CM(1,Datum::second); };
template <> struct bindings<Div>              { CM(0,Div::first);              CM(1,Div::second); };
template <> struct bindings<Div_assign>       { CM(0,Div_assign::first);       CM(1,Div_assign::second); };
template <> struct bindings<Dot>              { CM(0,Dot::first);              CM(1,Dot::second); };
template <> struct bindings<Dot_star>         { CM(0,Dot_star::first);         CM(1,Dot_star::second); };
template <> struct bindings<Dynamic_cast>     { CM(0,Dynamic_cast::first);     CM(1,Dynamic_cast::second); };
template <> struct bindings<Equal>            { CM(0,Equal::first);            CM(1,Equal::second); };
template <> struct bindings<Greater>          { CM(0,Greater::first);          CM(1,Greater::second); };
template <> struct bindings<Greater_equal>    { CM(0,Greater_equal::first);    CM(1,Greater_equal::second); };
template <> struct bindings<Less>             { CM(0,Less::first);             CM(1,Less::second); };
template <> struct bindings<Less_equal>       { CM(0,Less_equal::first);       CM(1,Less_equal::second); };
template <> struct bindings<Literal>          { CM(0,Literal::first);          CM(1,Literal::second); };
template <> struct bindings<Lshift>           { CM(0,Lshift::first);           CM(1,Lshift::second); };
template <> struct bindings<Lshift_assign>    { CM(0,Lshift_assign::first);    CM(1,Lshift_assign::second); };
template <> struct bindings<Member_init>      { CM(0,Member_init::member);     CM(1,Member_init::initializer); };
template <> struct bindings<Minus>            { CM(0,Minus::first);            CM(1,Minus::second); };
template <> struct bindings<Minus_assign>     { CM(0,Minus_assign::first);     CM(1,Minus_assign::second); };
template <> struct bindings<Modulo>           { CM(0,Modulo::first);           CM(1,Modulo::second); };
template <> struct bindings<Modulo_assign>    { CM(0,Modulo_assign::first);    CM(1,Modulo_assign::second); };
template <> struct bindings<Mul>              { CM(0,Mul::first);              CM(1,Mul::second); };
template <> struct bindings<Mul_assign>       { CM(0,Mul_assign::first);       CM(1,Mul_assign::second); };
template <> struct bindings<Not_equal>        { CM(0,Not_equal::first);        CM(1,Not_equal::second); };
template <> struct bindings<Or>               { CM(0,Or::first);               CM(1,Or::second); };
template <> struct bindings<Plus>             { CM(0,Plus::first);             CM(1,Plus::second); };
template <> struct bindings<Plus_assign>      { CM(0,Plus_assign::first);      CM(1,Plus_assign::second); };
template <> struct bindings<Reinterpret_cast> { CM(0,Reinterpret_cast::first); CM(1,Reinterpret_cast::second); };
template <> struct bindings<Rshift>           { CM(0,Rshift::first);           CM(1,Rshift::second); };
template <> struct bindings<Rshift_assign>    { CM(0,Rshift_assign::first);    CM(1,Rshift_assign::second); };
template <> struct bindings<Static_cast>      { CM(0,Static_cast::first);      CM(1,Static_cast::second); };

// --------------------------------------------------------
// -- result of trinary expression constructor constants --
// --------------------------------------------------------
template <> struct bindings<Conditional>      { CM(0,Conditional::condition);  CM(1,Conditional::then_expr);  CM(2,Conditional::else_expr); };
template <> struct bindings<New>              { CM(0,New::placement);          CM(1,New::allocated_type);     CM(2,New::initializer); };

// -----------------------------------------------
// -- result of statement constructor constants --
// -----------------------------------------------
template <> struct bindings<Block>            { CM(0,Block::body);             CM(1,Block::handlers);         CM(2,Scope::members); };
template <> struct bindings<Break>            { CM(0,Break::from); };
template <> struct bindings<Continue>         { CM(0,Continue::iteration); };
template <> struct bindings<Ctor_body>        { CM(0,Ctor_body::inits);        CM(1,Ctor_body::block); };
template <> struct bindings<Do>               { CM(0,Do::condition);           CM(1,Do::body); };
template <> struct bindings<Expr_stmt>        { CM(0,Expr_stmt::expr); };
//template <> struct bindings<Empty_stmt>       { };
template <> struct bindings<For>              { CM(0,For::initializer);        CM(1,For::condition);          CM(2,For::increment); CM(3,For::body); };
template <> struct bindings<For_in>           { CM(0,For_in::variable);        CM(1,For_in::sequence);        CM(2,For_in::body); };
template <> struct bindings<Goto>             { CM(0,Goto::target); };
template <> struct bindings<Handler>          { CM(0,Handler::exception);      CM(1,Handler::body); };
template <> struct bindings<If_then>          { CM(0,If_then::condition);      CM(1,If_then::then_stmt); };
template <> struct bindings<If_then_else>     { CM(0,If_then_else::condition); CM(1,If_then_else::then_stmt); CM(2,If_then_else::else_stmt); };
template <> struct bindings<Labeled_stmt>     { CM(0,Labeled_stmt::label);     CM(1,Labeled_stmt::stmt); };
template <> struct bindings<Return>           { CM(0,Return::value); };
template <> struct bindings<Switch>           { CM(0,Switch::condition);       CM(1,Switch::body); };
template <> struct bindings<While>            { CM(0,While::condition);        CM(1,While::body); };

// -------------------------------------------------
// -- result of declaration constructor constants --
// -------------------------------------------------
template <> struct bindings<Alias>            { CM(0,Decl::name); CM(1,Decl::type); };
//template <> struct bindings<Asm>              { };
template <> struct bindings<Base_type>        { CM(0,Decl::name); CM(1,Decl::type); };
template <> struct bindings<Enumerator>       { CM(0,Decl::name); CM(1,Decl::type); CM(2,Enumerator::membership); };
template <> struct bindings<Field>            { CM(0,Decl::name); CM(1,Decl::type); CM(2,Field::membership); };
template <> struct bindings<Bitfield>         { CM(0,Decl::name); CM(1,Decl::type); CM(2,Bitfield::membership);  CM(3,Bitfield::precision); };
template <> struct bindings<Fundecl>          { CM(0,Decl::name); CM(1,Decl::type); CM(2,Fundecl::membership);   CM(3,Fundecl::parameters); /*CM(2,Fundecl::definition); CM(3,Fundecl::mapping);*/ };
template <> struct bindings<Named_map>        { CM(0,Decl::name); CM(1,Decl::type); CM(2,Named_map::params);     CM(3,Named_map::mapping);/*CM(2,Named_map::definition); CM(2,Named_map::primary_named_map);*/ };
template <> struct bindings<Parameter>        { CM(0,Decl::name); CM(1,Decl::type); CM(2,Parameter::membership); CM(3,Parameter::default_value); };
template <> struct bindings<Typedecl>         { CM(0,Decl::name); CM(1,Decl::type); CM(2,Typedecl::membership);  CM(3,Typedecl::definition); };
template <> struct bindings<Var>              { CM(0,Decl::name); CM(1,Decl::type); };

// ------------------------
// -- distinguished node --
// ------------------------
template <> struct bindings<Unit>             { CM(0,Unit::get_global_scope); };
                                                     
// --------------------------------
// -- Special built-in constants --
// --------------------------------
//template <> struct bindings<Void>             { };
//template <> struct bindings<Bool>             { };
//template <> struct bindings<Char>             { };
//template <> struct bindings<sChar>            { };
//template <> struct bindings<uChar>            { };
//template <> struct bindings<Wchar_t>          { };
//template <> struct bindings<Short>            { };
//template <> struct bindings<uShort>           { };
//template <> struct bindings<Int>              { };
//template <> struct bindings<uInt>             { };
//template <> struct bindings<Long>             { };
//template <> struct bindings<uLong>            { };
//template <> struct bindings<Long_long>        { };
//template <> struct bindings<uLong_long>       { };
//template <> struct bindings<Float>            { };
//template <> struct bindings<Double>           { };
//template <> struct bindings<Long_double>      { };
//template <> struct bindings<Ellipsis>         { };

//template <> struct bindings<C_linkage>        { };
//template <> struct bindings<Cxx_linkage>      { };

// ----------------------------------
// -- built-ins, but not constants --
// ----------------------------------
template <> struct bindings<Global_scope>     { CM(0,Namespace::members); };

//------------------------------------------------------------------------------
