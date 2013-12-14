///
/// \file n+k.hpp
///
/// This file defines generalized n+k pattern support by our library.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of Mach7 library (http://parasol.tamu.edu/mach7/).
/// Copyright (C) 2011-2012 Texas A&M University.
/// All rights reserved.
///

#pragma once

#include "primitive.hpp"
#include "exprtmpl.hpp"
#include <ostream>

//------------------------------------------------------------------------------

/// This is the most generic solver that essentially just reports an error at
/// compile time to let the user know that she forgot to provide the semantics
/// for an algebraic decomposition she is using in the program.
template <typename E, typename S>
inline bool solve(const E&, const S&) noexcept
{
    // NOTE: The condition is dummy that is surely false. We need it as putting
    //       just false there fails compilation without trying to instantiate solve.
    static_assert(std::is_same<E,S>::value,"There is no solver for matching expression E against a value of type S");
    // When you get the above error, look at the structure of type E in the error
    // message - somewhere in your program you are trying to match an expression
    // corresponding to this type against a value of type S, however you did not
    // provide a solver for the corresponding expression. To fix this, add a function
    //   bool solve(const E& e, const S& r) {...} (or function template if needed)
    // that implements the semantics of such an algebraic decomposition.
    return false;
}

//------------------------------------------------------------------------------

/// Expression pattern
template <typename F, typename E1 = void, typename E2 = void> 
struct expr;

//------------------------------------------------------------------------------

/// #is_pattern_ is a helper meta-predicate capable of distinguishing all our patterns
template <typename F, typename E1, typename E2> struct is_pattern_<expr<F,E1,E2>>    { enum { value = true }; };

/// #is_expression_ is a helper meta-predicate that separates lazily evaluatable expressions we support
template <typename F, typename E1, typename E2> struct is_expression_<expr<F,E1,E2>> { enum { value = true }; };

//------------------------------------------------------------------------------

/// Expression pattern for unary operation
template <typename F, typename E1>
struct expr<F,E1>
{
    static_assert(is_expression<E1>::value, "Argument E1 of a unary expression-pattern must be a lazy expression");

    explicit expr(const E1& e1) : m_e1(e1) {}
    explicit expr(E1&& e1) noexcept : m_e1(std::move(e1)) {}
    expr(expr&& e) noexcept : m_e1(std::move(e.m_e1)) {}

    typedef typename std::remove_const<decltype(F()(std::declval<typename E1::result_type>()))>::type result_type; // We needed to add remove_const here as MSVC was returning const T
    typedef result_type accepted_type; ///< Type accepted by the pattern. Requirement of #Pattern concept
    operator result_type() const { return eval(*this); } // FIX: avoid implicit conversion in lazy expressions

    template <typename U>
    bool operator()(const U& u) const { return solve(*this,u); }

    E1 m_e1;
};

/// Expression pattern for binary operation
template <typename F, typename E1, typename E2>
struct expr
{
    static_assert(is_expression<E1>::value, "Argument E1 of a binary expression-pattern must be a lazy expression");
    static_assert(is_expression<E2>::value, "Argument E2 of a binary expression-pattern must be a lazy expression");

    expr(const E1& e1, const E2& e2) : m_e1(e1), m_e2(e2) {}
    expr(E1&& e1, E2&& e2) noexcept : m_e1(std::move(e1)), m_e2(std::move(e2)) {}
    expr(expr&& e) noexcept : m_e1(std::move(e.m_e1)), m_e2(std::move(e.m_e2)) {}

    typedef typename std::remove_const<decltype(F()(std::declval<typename E1::result_type>(),std::declval<typename E2::result_type>()))>::type result_type; // We needed to add remove_const here as MSVC was returning const T
    typedef result_type accepted_type; ///< Type accepted by the pattern. Requirement of #Pattern concept
    operator result_type() const { return eval(*this); }// FIX: avoid implicit conversion in lazy expressions

    template <typename U>
    bool operator()(const U& u) const { return solve(*this,u); }

    E1 m_e1;
    E2 m_e2;
};

//------------------------------------------------------------------------------

/// Convenience function to create unary expression
template <typename F, typename E1>
    auto make_expr(E1&& e1) noexcept -> XTL_RETURN
    (
        expr<F,typename underlying<E1>::type>(std::forward<E1>(e1))
    )

/// Convenience function to create binary expression
template <typename F, typename E1, typename E2>
    auto make_expr(E1&& e1, E2&& e2) noexcept -> XTL_RETURN
    (
        expr<F,typename underlying<E1>::type,typename underlying<E2>::type>(std::forward<E1>(e1),std::forward<E2>(e2))
    )

//------------------------------------------------------------------------------

///@{
/// Set of overloads capable of decomposing an expression template that models
/// an Expression concept and evaluating it.
template <typename T> T inline eval(const value<T>& e)             { return e.m_value; }
template <typename T> T inline eval(const variable<T>& e)          { return e.m_value; }
template <typename T> T inline eval(const var_ref<T>& e)           { return *e.m_var; }
template <typename T> T inline eval(const var_ref<variable<T>>& e) { return e.m_var->m_value; }
template <typename F, typename E1>              
    typename expr<F,E1>::result_type    inline eval(const expr<F,E1>&    e) { return F()(eval(e.m_e1)); }
template <typename F, typename E1, typename E2> 
    typename expr<F,E1,E2>::result_type inline eval(const expr<F,E1,E2>& e) { return F()(eval(e.m_e1),eval(e.m_e2)); }
///@}

//------------------------------------------------------------------------------

#if defined(__GNUC__)
///@{
/// This is a helper structure to define a result type of an operation.
/// We need it because GCC chokes on decltype(filter(...)) in the enable_if's 
/// result type when condition is false
template <typename F, typename E1 = void, typename E2 = void, bool C = either_is_expression<E1,E2>::value>
struct filtered_result;

template <typename F, typename E1>
struct filtered_result<F,E1,void,true>
{
    typedef expr<F,decltype(filter(std::declval<E1>()))> type;
};

template <typename F, typename E1, typename E2>
struct filtered_result<F,E1,E2,true>
{
    typedef expr<F,decltype(filter(std::declval<E1>())), decltype(filter(std::declval<E2>()))> type;
};

template <typename F, typename E1, typename E2>
struct filtered_result<F,E1,E2,false>
{
    struct  disabled_operation_result_type {};
    typedef disabled_operation_result_type type;
};
///@}
#endif

//------------------------------------------------------------------------------

#if defined(__GNUC__)
// NOTE: We need otherwise redundant non-const overloads to make sure that our 
//       overloads creating expression templates based on the second argument are 
//       not chosen!
template <typename T> std::ostream& operator<<(std::ostream& os, const variable<T >& v) { return os <<  v.m_value; }
template <typename T> std::ostream& operator<<(std::ostream& os,       variable<T >& v) { return os <<  v.m_value; }
template <typename T> std::ostream& operator<<(std::ostream& os, const variable<T*>& v) { return os << *v.m_value; }
template <typename T> std::ostream& operator<<(std::ostream& os,       variable<T*>& v) { return os << *v.m_value; }
#else
template <typename E>
inline auto operator<<(std::ostream& os, E&& e) throw() -> typename std::enable_if<is_expression<E>::value, std::ostream&>::type 
{
    return os << eval(e);
}
#endif

//------------------------------------------------------------------------------

///@{
/// The following definitions are needed to create the expression templates
/// supporting our patterns and lazy expressions. They define:
/// - all unary operators on variable as well as all binary operators with
///   first argument being variable and second whatever
/// - all unary operators on value as well as all binary operators with
///   first argument being value and second whatever
/// - all unary operators on unary expressions as well as all binary
///   operators with first argument being unary expression and second whatever
/// - all unary operators on binary expressions as well as all binary
///   operators with first argument being binary expression and second whatever
/// - the combinations making the above ones ambiguous
/// FIX: GCC (versions 4.5.2 and 4.6.1 at least) crashes on the following definition
///      that tries to mimic concept overloading based approach with enable_if. We thus
///      provide an overload based workaround for GCC.
#if !defined(__GNUC__)
#define FOR_EACH_UNARY_OPERATOR(FF,S)                                           \
    template <typename E1>                                                      \
    inline auto XTL_CONCATENATE(operator,S)(E1&& e1) noexcept ->                \
    typename std::enable_if<is_expression<E1>::value,                           \
                            expr<FF,decltype(filter(std::forward<E1>(e1)))>     \
                           >::type                                              \
    {                                                                           \
        return make_expr<FF>(filter(std::forward<E1>(e1)));                     \
    }
#define FOR_EACH_BINARY_OPERATOR(FF,S)                                          \
    template <typename E1, typename E2>                                         \
    inline auto XTL_CONCATENATE(operator,S)(E1&& e1, E2&& e2) noexcept ->       \
    typename std::enable_if<either_is_expression<E1,E2>::value,                 \
                            expr<FF,decltype(filter(std::forward<E1>(e1))),decltype(filter(std::forward<E2>(e2)))> \
                           >::type                                              \
    {                                                                           \
        return make_expr<FF>(filter(std::forward<E1>(e1)),filter(std::forward<E2>(e2))); \
    }
#else
/// FIX: As you can see one of the overloads is commented and is replaced with a
///      different one that follows. This is because of the bug in GCC 4.5.2 where
///      failure in decltype after -> does not behave as SFINAE, but fails compilation.
///      This was choosing the wrong overload, so maybe we should replace all operators
///      in that way, of rely on later GCC version where this (presumably) is fixed.
#define FOR_EACH_UNARY_OPERATOR(FF,S)                                           \
    template <typename T>                                        inline auto XTL_CONCATENATE(operator,S)(      variable<T>&    v) noexcept -> XTL_RETURN(make_expr<FF>(var_ref<variable<T>>(v))) \
    template <typename T>                                        inline auto XTL_CONCATENATE(operator,S)(const value<T>&       v) noexcept -> XTL_RETURN(make_expr<FF>(v))                       \
    template <typename F1, typename E1>                          inline auto XTL_CONCATENATE(operator,S)(const expr<F1,E1>&    v) noexcept -> XTL_RETURN(make_expr<FF>(v))                       \
    template <typename F1, typename E1, typename E2>             inline auto XTL_CONCATENATE(operator,S)(const expr<F1,E1,E2>& v) noexcept -> XTL_RETURN(make_expr<FF>(v))
#define FOR_EACH_BINARY_OPERATOR(FF,S)                                          \
  /*template <typename T, typename E>                            inline auto XTL_CONCATENATE(operator,S)(      variable<T>&    v,       E&&             e) noexcept -> XTL_RETURN(make_expr<FF>(var_ref<variable<T>>(v),filter(std::forward<E>(e))))*/ \
    template <typename T, typename E>                            inline auto XTL_CONCATENATE(operator,S)(      variable<T>&    v,       E&&             e) noexcept -> expr<FF,var_ref<variable<T>>,decltype(filter(std::forward<E>(e)))> { return make_expr<FF>(var_ref<variable<T>>(v),filter(std::forward<E>(e))); } \
  /*template <typename T, typename E>                            inline auto XTL_CONCATENATE(operator,S)(      E&&             e,       variable<T>&    v) noexcept -> XTL_RETURN(make_expr<FF>(filter(std::forward<E>(e)),var_ref<variable<T>>(v)))*/ \
    template <typename T, typename E>                            inline auto XTL_CONCATENATE(operator,S)(      E&&             e,       variable<T>&    v) noexcept -> expr<FF,decltype(filter(std::forward<E>(e))),var_ref<variable<T>>> { return make_expr<FF>(filter(std::forward<E>(e)),var_ref<variable<T>>(v)); } \
    template <typename T, typename E>                            inline auto XTL_CONCATENATE(operator,S)(const value<T>&       v,       E&&             e) noexcept -> XTL_RETURN(make_expr<FF>(v,filter(std::forward<E>(e)))) \
    template <typename T, typename E>                            inline auto XTL_CONCATENATE(operator,S)(      E&&             e, const value<T>&       v) noexcept -> XTL_RETURN(make_expr<FF>(filter(std::forward<E>(e)),v)) \
    template <typename F1, typename E1, typename E>              inline auto XTL_CONCATENATE(operator,S)(const expr<F1,E1>&    v,       E&&             e) noexcept -> XTL_RETURN(make_expr<FF>(v,filter(std::forward<E>(e)))) \
    template <typename F1, typename E1, typename E>              inline auto XTL_CONCATENATE(operator,S)(      E&&             e, const expr<F1,E1>&    v) noexcept -> XTL_RETURN(make_expr<FF>(filter(std::forward<E>(e)),v)) \
    template <typename F1, typename E1, typename E2, typename E> inline auto XTL_CONCATENATE(operator,S)(const expr<F1,E1,E2>& v,       E&&             e) noexcept -> XTL_RETURN(make_expr<FF>(v,filter(std::forward<E>(e)))) \
    template <typename F1, typename E1, typename E2, typename E> inline auto XTL_CONCATENATE(operator,S)(      E&&             e, const expr<F1,E1,E2>& v) noexcept -> XTL_RETURN(make_expr<FF>(filter(std::forward<E>(e)),v)) \
    template <typename T, typename U>                            inline auto XTL_CONCATENATE(operator,S)(const value<T>&       v, const value<U>&       c) noexcept -> XTL_RETURN(make_expr<FF>(v,c)) \
    template <typename T, typename U>                            inline auto XTL_CONCATENATE(operator,S)(      variable<T>&    v,       variable<U>&    w) noexcept -> XTL_RETURN(make_expr<FF>(var_ref<variable<T>>(v),var_ref<variable<T>>(w))) \
    template <typename F1, typename E1, typename E2,                           \
              typename F2, typename E3, typename E4>             inline auto XTL_CONCATENATE(operator,S)(const expr<F1,E1,E2>& a, const expr<F2,E3,E4>& b) noexcept -> XTL_RETURN(make_expr<FF>(a,b)) \
    template <typename T, typename U>                            inline auto XTL_CONCATENATE(operator,S)(      variable<T>&    v, const value<U>&       c) noexcept -> XTL_RETURN(make_expr<FF>(var_ref<variable<T>>(v),c)) \
    template <typename T, typename U>                            inline auto XTL_CONCATENATE(operator,S)(const value<U>&       c,       variable<T>&    v) noexcept -> XTL_RETURN(make_expr<FF>(c,var_ref<variable<T>>(v))) \
    template <typename T, typename F1, typename E1, typename E2> inline auto XTL_CONCATENATE(operator,S)(const expr<F1,E1,E2>& e,       variable<T>&    v) noexcept -> XTL_RETURN(make_expr<FF>(e,var_ref<variable<T>>(v))) \
    template <typename T, typename F1, typename E1, typename E2> inline auto XTL_CONCATENATE(operator,S)(      variable<T>&    v, const expr<F1,E1,E2>& e) noexcept -> XTL_RETURN(make_expr<FF>(var_ref<variable<T>>(v),e)) \
    template <typename T, typename F1, typename E1, typename E2> inline auto XTL_CONCATENATE(operator,S)(const expr<F1,E1,E2>& e, const value<T>&       c) noexcept -> XTL_RETURN(make_expr<FF>(e,c))                       \
    template <typename T, typename F1, typename E1, typename E2> inline auto XTL_CONCATENATE(operator,S)(const value<T>&       c, const expr<F1,E1,E2>& e) noexcept -> XTL_RETURN(make_expr<FF>(c,e))
#endif
//#include "../loop_over_operators.hpp"
#include "../operators_preprocessed.hpp"
#undef  FOR_EACH_BINARY_OPERATOR
#undef  FOR_EACH_UNARY_OPERATOR
///@}

//------------------------------------------------------------------------------

#include "solvers.hpp" // Include all our solvers by default for testing purposes. Remove eventually

//------------------------------------------------------------------------------
