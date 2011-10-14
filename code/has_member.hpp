#pragma once

///
/// We use here code by Jason Shirk and Alexey Gurtovoy for detecting presence
/// of a given member. See the following discussion for details:
/// http://lists.boost.org/Archives/boost/2002/03/27251.php
///

typedef char (& no_tag)[1];
typedef char (&yes_tag)[2]; 

#ifdef _MSC_VER
// In the version for MSVC the member name has to be taken into ()
#define HAS_MEMBER_XXX(name,member_type,member_name) \
template < typename T, member_type > struct ptmf_##name##_helper {}; \
template< typename T > no_tag  has_member_##name##_helper(...); \
template< typename T > yes_tag has_member_##name##_helper(ptmf_##name##_helper<T, &(T::member_name)>* p); \
template< typename T > \
struct has_member_##name \
{ \
    static const bool value = sizeof(has_member_##name##_helper<T>(0)) == sizeof(yes_tag); \
}
#else
// In the version for GCC () around member name don't work for some reason, but note to keep space before >
#define HAS_MEMBER_XXX(name,member_type,member_name) \
template < typename T, member_type > struct ptmf_##name##_helper {}; \
template< typename T > no_tag  has_member_##name##_helper(...); \
template< typename T > yes_tag has_member_##name##_helper(ptmf_##name##_helper<T, &T::member_name >* p); \
template< typename T > \
struct has_member_##name \
{ \
    static const bool value = sizeof(has_member_##name##_helper<T>(0)) == sizeof(yes_tag); \
}
#endif

//template < typename T, T& (T::*)() > struct ptmf_operator_pre_increment_helper {};
//template< typename T > no_tag  has_member_operator_pre_increment_helper(...);
//template< typename T > yes_tag has_member_operator_pre_increment_helper(ptmf_operator_pre_increment_helper<T, &T::operator++ >* p);
//template< typename T >
//struct has_member_operator_pre_increment
//{
//    static const bool value = sizeof(has_member_operator_pre_increment_helper<T>(0)) == sizeof(yes_tag);
//}


// Helpers to detect some unary operators

HAS_MEMBER_XXX(operator_pre_increment , T& (T::*)()             , operator++);
HAS_MEMBER_XXX(operator_pre_decrement , T& (T::*)()             , operator--);
HAS_MEMBER_XXX(operator_post_increment, T& (T::*)(int)          , operator++);
HAS_MEMBER_XXX(operator_post_decrement, T& (T::*)(int)          , operator--);

// Helpers to detect some binary operators

HAS_MEMBER_XXX(operator_plus          , T (T::*)(const T&) const, operator+);
HAS_MEMBER_XXX(operator_minus         , T (T::*)(const T&) const, operator-);
HAS_MEMBER_XXX(operator_mul           , T (T::*)(const T&) const, operator*);
HAS_MEMBER_XXX(operator_div           , T (T::*)(const T&) const, operator/);
HAS_MEMBER_XXX(operator_modulo        , T (T::*)(const T&) const, operator%);
HAS_MEMBER_XXX(operator_bitand        , T (T::*)(const T&) const, operator&);
HAS_MEMBER_XXX(operator_bitor         , T (T::*)(const T&) const, operator|);
HAS_MEMBER_XXX(operator_bitxor        , T (T::*)(const T&) const, operator^);
HAS_MEMBER_XXX(operator_lshift        , T (T::*)(const T&) const, operator<<);
HAS_MEMBER_XXX(operator_rshift        , T (T::*)(const T&) const, operator>>);

// Helpers to detect assignment operators

HAS_MEMBER_XXX(operator_assign        , T& (T::*)(const T&)     , operator=);
HAS_MEMBER_XXX(operator_plus_assign   , T& (T::*)(const T&)     , operator+=);
HAS_MEMBER_XXX(operator_minus_assign  , T& (T::*)(const T&)     , operator-=);
HAS_MEMBER_XXX(operator_mul_assign    , T& (T::*)(const T&)     , operator*=);
HAS_MEMBER_XXX(operator_div_assign    , T& (T::*)(const T&)     , operator/=);
HAS_MEMBER_XXX(operator_modulo_assign , T& (T::*)(const T&)     , operator%=);
HAS_MEMBER_XXX(operator_bitand_assign , T& (T::*)(const T&)     , operator&=);
HAS_MEMBER_XXX(operator_bitor_assign  , T& (T::*)(const T&)     , operator|=);
HAS_MEMBER_XXX(operator_bitxor_assign , T& (T::*)(const T&)     , operator^=);
HAS_MEMBER_XXX(operator_lshift_assign , T& (T::*)(const T&)     , operator<<=);
HAS_MEMBER_XXX(operator_rshift_assign , T& (T::*)(const T&)     , operator>>=);

#include "type_descriptor.hpp"

HAS_MEMBER_XXX(dependent   , typename T::dependent_type (*)(const type_descriptor_container::type_descriptor&), dependent);
HAS_MEMBER_XXX(addr_top    ,          T                 (*)(const address&)      , top);
HAS_MEMBER_XXX(addr_bot    ,          T                 (*)(const address&)      , bot);
HAS_MEMBER_XXX(dep_addr_top, typename T::actual_type (T::*)(const address&) const, top);
HAS_MEMBER_XXX(dep_addr_bot, typename T::actual_type (T::*)(const address&) const, bot);
