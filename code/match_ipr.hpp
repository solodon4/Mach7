///
/// \file match_ipr.hpp
///
/// This file defines pattern matching bindings for Pivot's IPR hierarchy.
///
/// \author Yuriy Solodkyy
/// Copyright (C) 2011, Texas A&M University.  All rights reserved.
///

#pragma once

#include "match.hpp"
#include <ipr/interface>  // Pivot interfaces

using namespace ipr;

template <> struct match_members<Node>             { CM(0,Node,category);   CM(1,Node,node_id); };

template <> struct match_members<Annotation>       { CM(0,Annotation,name); CM(1,Annotation,value); };
template <> struct match_members<Region>           { CM(0,Region,owner);    CM(1,Region,enclosing); CM(2,Region,bindings); CM(3,Region,span); };
template <> struct match_members<Comment>          { CM(0,Comment,text); };
template <> struct match_members<String>           { CM(0,String,begin);    CM(1,String,size); };

template <> struct match_members<Mapping>          {};
template <> struct match_members<Overload>         {};
template <> struct match_members<Parameter_list>   {};
template <> struct match_members<Scope>            {};

// ------------------------------------------
// -- general constructors                 --
// ------------------------------------------
template <> struct match_members<Linkage>          { CM(0,Linkage,language); };
template <> struct match_members<Expr>             { CM(0,Expr,type); };
template <> struct match_members<Classic>          { CM(0,Expr,type); CM(1,Classic,has_impl_decl); CM(2,Classic,impl_decl); };
template <> struct match_members<Name>             { CM(0,Expr,type); };
template <> struct match_members<Type>             { CM(0,Type,name); };
template <> struct match_members<Udt>              {};
template <> struct match_members<Stmt>             {};
template <> struct match_members<Decl>             { CM(0,Decl,name); CM(1,Decl,type); CM(2,Decl,initializer); };

// ------------------------------------------
// -- results of name constructor constants --
// ------------------------------------------
template <> struct match_members<Identifier>       { CM(0,Identifier,string); };
template <> struct match_members<Id_expr>          { CM(0,Id_expr,name); CM(1,Id_expr,resolution); };
template <> struct match_members<Operator>         { CM(0,Operator,opname); };
template <> struct match_members<Conversion>       { CM(0,Conversion,target); };
template <> struct match_members<Scope_ref>        { CM(0,Scope_ref,scope); CM(1,Scope_ref,member); };
template <> struct match_members<Template_id>      { CM(0,Template_id,template_name); CM(1,Template_id,args); };
template <> struct match_members<Type_id>          {};
template <> struct match_members<Ctor_name>        { CM(0,Ctor_name,object_type); };
template <> struct match_members<Dtor_name>        { CM(0,Dtor_name,object_type); };
template <> struct match_members<Rname>            { CM(0,Rname,level); CM(1,Rname,position); };

// -------------------------------------------
// -- results of type constructor constants --
// -------------------------------------------
template <> struct match_members<Array>            { CM(0,Array,element_type); CM(1,Array,bound); };
template <> struct match_members<As_type>          { CM(0,As_type,expr); };
template <> struct match_members<Class>            { CM(0,Class,members);      CM(1,Class,bases); };
template <> struct match_members<Decltype>         { CM(0,Decltype,expr); };
template <> struct match_members<Enum>             { CM(0,Enum,members); };
template <> struct match_members<Function>         { CM(0,Function,source);    CM(1,Function,target); CM(2,Function,throws); };
template <> struct match_members<Namespace>        { CM(0,Namespace,members); };
template <> struct match_members<Pointer>          { CM(0,Pointer,points_to); };
template <> struct match_members<Ptr_to_member>    { CM(0,Ptr_to_member,containing_type); CM(1,Ptr_to_member,member_type); };
template <> struct match_members<Product>          { CM(0,Product,elements); };
template <> struct match_members<Qualified>        { CM(0,Qualified,main_variant); CM(1,Qualified,qualifiers); };
template <> struct match_members<Reference>        { CM(0,Reference,refers_to); };
template <> struct match_members<Rvalue_reference> { CM(0,Rvalue_reference,refers_to); };
template <> struct match_members<Sum>              { CM(0,Sum,elements); };
template <> struct match_members<Template>         { CM(0,Template,source); CM(1,Template,target); };
template <> struct match_members<Union>            { CM(0,Union,members); };

// --------------------------------------------------------
// -- results of nullar expression constructor constants --
// --------------------------------------------------------
template <> struct match_members<Phantom>          {};

// -------------------------------------------------------
// -- results of unary expression constructor constants --
// -------------------------------------------------------
template <> struct match_members<Address>          { CM(0,Address,operand); };
template <> struct match_members<Array_delete>     { CM(0,Array_delete,operand); };
template <> struct match_members<Complement>       { CM(0,Complement,operand); };
template <> struct match_members<Delete>           { CM(0,Delete,operand); };
template <> struct match_members<Deref>            { CM(0,Deref,operand); };
template <> struct match_members<Expr_list>        { CM(0,Expr_list,operand); };
template <> struct match_members<Expr_sizeof>      { CM(0,Expr_sizeof,operand); };
template <> struct match_members<Expr_typeid>      { CM(0,Expr_typeid,operand); };
template <> struct match_members<Label>            { CM(0,Label,operand); };
template <> struct match_members<Not>              { CM(0,Not,operand); };
template <> struct match_members<Paren_expr>       { CM(0,Paren_expr,operand); };
template <> struct match_members<Post_decrement>   { CM(0,Post_decrement,operand); };
template <> struct match_members<Post_increment>   { CM(0,Post_increment,operand); };
template <> struct match_members<Pre_decrement>    { CM(0,Pre_decrement,operand); };
template <> struct match_members<Pre_increment>    { CM(0,Pre_increment,operand); };
template <> struct match_members<Throw>            { CM(0,Throw,operand); };
template <> struct match_members<Type_sizeof>      { CM(0,Type_sizeof,operand); };
template <> struct match_members<Type_typeid>      { CM(0,Type_typeid,operand); };
template <> struct match_members<Unary_minus>      { CM(0,Unary_minus,operand); };
template <> struct match_members<Unary_plus>       { CM(0,Unary_plus,operand); };

// --------------------------------------------------------
// -- results of binary expression constructor constants --
// --------------------------------------------------------
template <> struct match_members<And>              { CM(0,And,first); CM(1,And,second); };
template <> struct match_members<Array_ref>        { CM(0,Array_ref,first); CM(1,Array_ref,second); };
template <> struct match_members<Arrow>            { CM(0,Arrow,first); CM(1,Arrow,second); };
template <> struct match_members<Arrow_star>       { CM(0,Arrow_star,first); CM(1,Arrow_star,second); };
template <> struct match_members<Assign>           { CM(0,Assign,first); CM(1,Assign,second); };
template <> struct match_members<Bitand>           { CM(0,Bitand,first); CM(1,Bitand,second); };
template <> struct match_members<Bitand_assign>    { CM(0,Bitand_assign,first); CM(1,Bitand_assign,second); };
template <> struct match_members<Bitor>            { CM(0,Bitor,first); CM(1,Bitor,second); };
template <> struct match_members<Bitor_assign>     { CM(0,Bitor_assign,first); CM(1,Bitor_assign,second); };
template <> struct match_members<Bitxor>           { CM(0,Bitxor,first); CM(1,Bitxor,second); };
template <> struct match_members<Bitxor_assign>    { CM(0,Bitxor_assign,first); CM(1,Bitxor_assign,second); };
template <> struct match_members<Cast>             { CM(0,Cast,first); CM(1,Cast,second); };
template <> struct match_members<Call>             { CM(0,Call,first); CM(1,Call,second); };
template <> struct match_members<Comma>            { CM(0,Comma,first); CM(1,Comma,second); };
template <> struct match_members<Const_cast>       { CM(0,Const_cast,first); CM(1,Const_cast,second); };
template <> struct match_members<Datum>            { CM(0,Datum,first); CM(1,Datum,second); };
template <> struct match_members<Div>              { CM(0,Div,first); CM(1,Div,second); };
template <> struct match_members<Div_assign>       { CM(0,Div_assign,first); CM(1,Div_assign,second); };
template <> struct match_members<Dot>              { CM(0,Dot,first); CM(1,Dot,second); };
template <> struct match_members<Dot_star>         { CM(0,Dot_star,first); CM(1,Dot_star,second); };
template <> struct match_members<Dynamic_cast>     { CM(0,Dynamic_cast,first); CM(1,Dynamic_cast,second); };
template <> struct match_members<Equal>            { CM(0,Equal,first); CM(1,Equal,second); };
template <> struct match_members<Greater>          { CM(0,Greater,first); CM(1,Greater,second); };
template <> struct match_members<Greater_equal>    { CM(0,Greater_equal,first); CM(1,Greater_equal,second); };
template <> struct match_members<Less>             { CM(0,Less,first); CM(1,Less,second); };
template <> struct match_members<Less_equal>       { CM(0,Less_equal,first); CM(1,Less_equal,second); };
template <> struct match_members<Literal>          { CM(0,Literal,first); CM(1,Literal,second); };
template <> struct match_members<Lshift>           { CM(0,Lshift,first); CM(1,Lshift,second); };
template <> struct match_members<Lshift_assign>    { CM(0,Lshift_assign,first); CM(1,Lshift_assign,second); };
template <> struct match_members<Member_init>      { CM(0,Member_init,first); CM(1,Member_init,second); };
template <> struct match_members<Minus>            { CM(0,Minus,first); CM(1,Minus,second); };
template <> struct match_members<Minus_assign>     { CM(0,Minus_assign,first); CM(1,Minus_assign,second); };
template <> struct match_members<Modulo>           { CM(0,Modulo,first); CM(1,Modulo,second); };
template <> struct match_members<Modulo_assign>    { CM(0,Modulo_assign,first); CM(1,Modulo_assign,second); };
template <> struct match_members<Mul>              { CM(0,Mul,first); CM(1,Mul,second); };
template <> struct match_members<Mul_assign>       { CM(0,Mul_assign,first); CM(1,Mul_assign,second); };
template <> struct match_members<Not_equal>        { CM(0,Not_equal,first); CM(1,Not_equal,second); };
template <> struct match_members<Or>               { CM(0,Or,first); CM(1,Or,second); };
template <> struct match_members<Plus>             { CM(0,Plus,first); CM(1,Plus,second); };
template <> struct match_members<Plus_assign>      { CM(0,Plus_assign,first); CM(1,Plus_assign,second); };
template <> struct match_members<Reinterpret_cast> { CM(0,Reinterpret_cast,first); CM(1,Reinterpret_cast,second); };
template <> struct match_members<Rshift>           { CM(0,Rshift,first); CM(1,Rshift,second); };
template <> struct match_members<Rshift_assign>    { CM(0,Rshift_assign,first); CM(1,Rshift_assign,second); };
template <> struct match_members<Static_cast>      { CM(0,Static_cast,first); CM(1,Static_cast,second); };

// --------------------------------------------------------
// -- result of trinary expression constructor constants --
// --------------------------------------------------------
template <> struct match_members<Conditional>      {};
template <> struct match_members<New>              {};

// -----------------------------------------------
// -- result of statement constructor constants --
// -----------------------------------------------
template <> struct match_members<Block>            { CM(0,Block,body);             CM(1,Block,handlers);         CM(2,Scope,members); };
template <> struct match_members<Break>            { CM(0,Break,from); };
template <> struct match_members<Continue>         { CM(0,Continue,iteration); };
template <> struct match_members<Ctor_body>        { CM(0,Ctor_body,inits);        CM(1,Ctor_body,block); };
template <> struct match_members<Do>               { CM(0,Do,condition);           CM(1,Do,body); };
template <> struct match_members<Expr_stmt>        { CM(0,Expr_stmt,expr); };
template <> struct match_members<Empty_stmt>       {};
template <> struct match_members<For>              { CM(0,For,initializer);        CM(1,For,condition);          CM(2,For,increment); CM(3,For,body); };
template <> struct match_members<For_in>           { CM(0,For_in,variable);        CM(1,For_in,sequence);        CM(2,For_in,body); };
template <> struct match_members<Goto>             { CM(0,Goto,target); };
template <> struct match_members<Handler>          { CM(0,Handler,exception);      CM(1,Handler,body); };
template <> struct match_members<If_then>          { CM(0,If_then,condition);      CM(1,If_then,then_stmt); };
template <> struct match_members<If_then_else>     { CM(0,If_then_else,condition); CM(1,If_then_else,then_stmt); CM(2,If_then_else,else_stmt); };
template <> struct match_members<Labeled_stmt>     { CM(0,Labeled_stmt,label);     CM(1,Labeled_stmt,stmt); };
template <> struct match_members<Return>           { CM(0,Return,value); };
template <> struct match_members<Switch>           { CM(0,Switch,condition);       CM(1,Switch,body); };
template <> struct match_members<While>            { CM(0,While,condition);        CM(1,While,body); };

// -------------------------------------------------
// -- result of declaration constructor constants --
// -------------------------------------------------
template <> struct match_members<Alias>            { CM(0,Decl,name); CM(1,Decl,type); };
template <> struct match_members<Asm>              {};
template <> struct match_members<Base_type>        { CM(0,Decl,name); CM(1,Decl,type); };
template <> struct match_members<Enumerator>       { CM(0,Decl,name); CM(1,Decl,type); CM(2,Enumerator,membership); };
template <> struct match_members<Field>            { CM(0,Decl,name); CM(1,Decl,type); CM(2,Field,membership); };
template <> struct match_members<Bitfield>         { CM(0,Decl,name); CM(1,Decl,type); CM(2,Bitfield,membership);  CM(3,Bitfield,precision); };
template <> struct match_members<Fundecl>          { CM(0,Decl,name); CM(1,Decl,type); CM(2,Fundecl,membership);   CM(3,Fundecl,parameters); /*CM(2,Fundecl,definition); CM(3,Fundecl,mapping);*/ };
template <> struct match_members<Named_map>        { CM(0,Decl,name); CM(1,Decl,type); CM(2,Named_map,primary_named_map); CM(3,Named_map,params); /*CM(2,Named_map,definition); CM(3,Named_map,mapping);*/ };
template <> struct match_members<Parameter>        { CM(0,Decl,name); CM(1,Decl,type); CM(2,Parameter,membership); CM(3,Parameter,default_value); };
template <> struct match_members<Typedecl>         { CM(0,Decl,name); CM(1,Decl,type); CM(2,Typedecl,membership);  CM(3,Typedecl,definition); };
template <> struct match_members<Var>              { CM(0,Decl,name); CM(1,Decl,type); };
template <> struct match_members<Using_directive>  {};

// ------------------------
// -- distinguished node --
// ------------------------
template <> struct match_members<Unit>             {};
    
// --------------------------------
// -- Special built-in constants --
// --------------------------------
template <> struct match_members<Void>             {};
template <> struct match_members<Bool>             {};
template <> struct match_members<Char>             {};
template <> struct match_members<sChar>            {};
template <> struct match_members<uChar>            {};
template <> struct match_members<Wchar_t>          {};
template <> struct match_members<Short>            {};
template <> struct match_members<uShort>           {};
template <> struct match_members<Int>              {};
template <> struct match_members<uInt>             {};
template <> struct match_members<Long>             {};
template <> struct match_members<uLong>            {};
template <> struct match_members<Long_long>        {};
template <> struct match_members<uLong_long>       {};
template <> struct match_members<Float>            {};
template <> struct match_members<Double>           {};
template <> struct match_members<Long_double>      {};
template <> struct match_members<Ellipsis>         {};

template <> struct match_members<C_linkage>        {};
template <> struct match_members<Cxx_linkage>      {};

// ----------------------------------
// -- built-ins, but not constants --
// ----------------------------------
template <> struct match_members<Global_scope>     {};

//------------------------------------------------------------------------------
