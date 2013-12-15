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

namespace mch ///< Mach7 library namespace
{

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
template <typename F, typename E1, typename E2> struct is_pattern_<expr<F,E1,E2>>    { static const bool value = true; };

/// #is_expression_ is a helper meta-predicate that separates lazily evaluatable expressions we support
template <typename F, typename E1, typename E2> struct is_expression_<expr<F,E1,E2>> { static const bool value = true; };

//------------------------------------------------------------------------------

/// Expression pattern for unary operation
template <typename F, typename E1>
struct expr<F,E1>
{
    static_assert(is_expression<E1>::value, "Argument E1 of a unary expression-pattern must be a lazy expression");
    static_assert(!is_var<E1>::value,       "Attempting to host var<> directly. Use filter() to wrap it into ref2<>");

    explicit expr(const E1&  e1) noexcept : m_e1(          e1 ) {}
    explicit expr(      E1&& e1) noexcept : m_e1(std::move(e1)) {}
    expr(const expr&  e) noexcept : m_e1(          e.m_e1 ) {} ///< Copy constructor    
    expr(      expr&& e) noexcept : m_e1(std::move(e.m_e1)) {} ///< Move constructor
    expr& operator=(const expr&); ///< Assignment is not allowed for this class

    typedef typename std::remove_const<decltype(F()(std::declval<typename E1::result_type>()))>::type result_type;    ///< Type of result when used in expression. Requirement of #LazyExpression concept // We needed to add remove_const here as MSVC was returning const T

    /// Type function returning a type that will be accepted by the pattern for
    /// a given subject type S. We use type function instead of an associated 
    /// type, because there is no a single accepted type for a #wildcard pattern
    /// for example. Requirement of #Pattern concept.
    template <typename S> struct accepted_type_for { typedef result_type type; };

    operator result_type() const { return eval(*this); } // FIX: avoid implicit conversion in lazy expressions

    template <typename U>
    bool operator()(const U& u) const { return solve(*this,u); }

    E1 m_e1; ///< Expression template with the 1st operand
};

//------------------------------------------------------------------------------

/// Expression pattern for binary operation
template <typename F, typename E1, typename E2>
struct expr
{
    static_assert(is_expression<E1>::value, "Argument E1 of a binary expression-pattern must be a lazy expression");
    static_assert(is_expression<E2>::value, "Argument E2 of a binary expression-pattern must be a lazy expression");
    static_assert(!is_var<E1>::value,       "Attempting to host var<> directly. Use filter() to wrap it into ref2<>");
    static_assert(!is_var<E2>::value,       "Attempting to host var<> directly. Use filter() to wrap it into ref2<>");

    expr(const E1&  e1, const E2&  e2) noexcept : m_e1(          e1 ), m_e2(          e2 ) {}
    expr(      E1&& e1, const E2&  e2) noexcept : m_e1(std::move(e1)), m_e2(          e2 ) {}
    expr(const E1&  e1,       E2&& e2) noexcept : m_e1(          e1 ), m_e2(std::move(e2)) {}
    expr(      E1&& e1,       E2&& e2) noexcept : m_e1(std::move(e1)), m_e2(std::move(e2)) {}
    expr(const expr&  e) noexcept : m_e1(          e.m_e1 ), m_e2(          e.m_e2 ) {} ///< Copy constructor
    expr(      expr&& e) noexcept : m_e1(std::move(e.m_e1)), m_e2(std::move(e.m_e2)) {} ///< Move constructor
    expr& operator=(const expr&); ///< Assignment is not allowed for this class

    typedef typename std::remove_const<decltype(F()(std::declval<typename E1::result_type>(),std::declval<typename E2::result_type>()))>::type result_type;    ///< Type of result when used in expression. Requirement of #LazyExpression concept // We needed to add remove_const here as MSVC was returning const T

    /// Type function returning a type that will be accepted by the pattern for
    /// a given subject type S. We use type function instead of an associated 
    /// type, because there is no a single accepted type for a #wildcard pattern
    /// for example. Requirement of #Pattern concept.
    template <typename S> struct accepted_type_for { typedef result_type type; };

    operator result_type() const { return eval(*this); }// FIX: avoid implicit conversion in lazy expressions

    template <typename U>
    bool operator()(const U& u) const { return solve(*this,u); }

    E1 m_e1; ///< Expression template with the 1st operand
    E2 m_e2; ///< Expression template with the 2nd operand
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
/// \note See header files of other patterns for more overloads!
template <typename F, typename E1>              
inline typename expr<F,E1>::result_type    eval(const expr<F,E1>&    e) { return F()(eval(e.m_e1)); }
template <typename F, typename E1, typename E2> 
inline typename expr<F,E1,E2>::result_type eval(const expr<F,E1,E2>& e) { return F()(eval(e.m_e1),eval(e.m_e2)); }
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

} // of namespace mch

//------------------------------------------------------------------------------

#if defined(__GNUC__)

/// \note We need otherwise redundant non-const overloads to make sure that our 
///       overloads creating expression templates based on the second argument are 
///       not chosen!
/// \fix  These operators should also work when declared in mch namespace AFAIK since 
///       their arguments are from mch, however if we put them there, GCC for some
///       reason cannot find then a proper overload for os << v.m_value when it is 
///       declared in the global scope for the type decltype(v.m_value) from the 
///       global scope. To workaround this, we keep these overloads in the global scope!
template <typename T> std::ostream& operator<<(std::ostream& os, const mch::var<T >& v) { return os <<  v.m_value; }
template <typename T> std::ostream& operator<<(std::ostream& os,       mch::var<T >& v) { return os <<  v.m_value; }
template <typename T> std::ostream& operator<<(std::ostream& os, const mch::var<T*>& v) { return os << *v.m_value; }
template <typename T> std::ostream& operator<<(std::ostream& os,       mch::var<T*>& v) { return os << *v.m_value; }

#else

/// \note This operator has to be in the global namespace as E is unrestricted
///       and ADL won't work to find it!
template <typename E>
inline auto operator<<(std::ostream& os, E&& e) throw() 
        -> typename std::enable_if<mch::is_expression<E>::value, std::ostream&>::type 
{
    return os << mch::eval(e);
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
/// \note These operators have to be in the global namespace as Ei is unrestricted
///       and ADL won't work to find it!
/// FIX: GCC (versions 4.5.2 and 4.6.1 at least) crashes on the following definition
///      that tries to mimic concept overloading based approach with enable_if. We thus
///      provide an overload based workaround for GCC.
#if !defined(__GNUC__)
#define FOR_EACH_UNARY_OPERATOR(FF,S)                                           \
    template <typename E1>                                                      \
    inline auto XTL_CONCATENATE(operator,S)(E1&& e1) noexcept ->                \
    typename std::enable_if<                                                    \
                mch::is_expression<E1>::value,                                  \
                mch::expr<FF,decltype(mch::filter(std::forward<E1>(e1)))>       \
             >::type                                                            \
    {                                                                           \
        return mch::make_expr<FF>(mch::filter(std::forward<E1>(e1)));           \
    }
#define FOR_EACH_BINARY_OPERATOR(FF,S)                                          \
    template <typename E1, typename E2>                                         \
    inline auto XTL_CONCATENATE(operator,S)(E1&& e1, E2&& e2) noexcept ->       \
    typename std::enable_if<                                                    \
                mch::either_is_expression<E1,E2>::value,                        \
                mch::expr<FF,                                                   \
                          decltype(mch::filter(std::forward<E1>(e1))),          \
                          decltype(mch::filter(std::forward<E2>(e2)))           \
                         >                                                      \
             >::type                                                            \
    {                                                                           \
        return mch::make_expr<FF>(                                              \
                    mch::filter(std::forward<E1>(e1)),                          \
                    mch::filter(std::forward<E2>(e2)));                         \
    }
#else
/// FIX: As you can see one of the overloads is commented and is replaced with a
///      different one that follows. This is because of the bug in GCC 4.5.2 where
///      failure in decltype after -> does not behave as SFINAE, but fails compilation.
///      This was choosing the wrong overload, so maybe we should replace all operators
///      in that way, of rely on later GCC version where this (presumably) is fixed.
#define FOR_EACH_UNARY_OPERATOR(FF,S)                                           \
    template <typename T>                                         inline auto XTL_CONCATENATE(operator,S)(      mch::var<T>&         v) noexcept -> XTL_RETURN(mch::make_expr<FF>(mch::ref2<mch::var<T>>(v))) \
    template <typename T>                                         inline auto XTL_CONCATENATE(operator,S)(const mch::value<T>&       v) noexcept -> XTL_RETURN(mch::make_expr<FF>(v))                        \
    template <typename F1, typename E1>                           inline auto XTL_CONCATENATE(operator,S)(const mch::expr<F1,E1>&    v) noexcept -> XTL_RETURN(mch::make_expr<FF>(v))                        \
    template <typename F1, typename E1, typename E2>              inline auto XTL_CONCATENATE(operator,S)(const mch::expr<F1,E1,E2>& v) noexcept -> XTL_RETURN(mch::make_expr<FF>(v))
#define FOR_EACH_BINARY_OPERATOR(FF,S)                                          \
  /*template <typename T,  typename E>                            inline auto XTL_CONCATENATE(operator,S)(      mch::var<T>&         v,       E&&                  e) noexcept -> XTL_RETURN(mch::make_expr<FF>(mch::ref2<mch::var<T>>(v),mch::filter(std::forward<E>(e))))*/ \
    template <typename T,  typename E>                            inline auto XTL_CONCATENATE(operator,S)(      mch::var<T>&         v,       E&&                  e) noexcept -> mch::expr<FF,mch::ref2<mch::var<T>>,decltype(mch::filter(std::forward<E>(e)))> { return mch::make_expr<FF>(mch::ref2<mch::var<T>>(v),mch::filter(std::forward<E>(e))); } \
  /*template <typename T,  typename E>                            inline auto XTL_CONCATENATE(operator,S)(      E&&                  e,            mch::var<T>&    v) noexcept -> XTL_RETURN(mch::make_expr<FF>(mch::filter(std::forward<E>(e)),mch::ref2<mch::var<T>>(v)))*/ \
    template <typename T,  typename E>                            inline auto XTL_CONCATENATE(operator,S)(      E&&                  e,            mch::var<T>&    v) noexcept -> mch::expr<FF,decltype(mch::filter(std::forward<E>(e))),mch::ref2<mch::var<T>>> { return mch::make_expr<FF>(mch::filter(std::forward<E>(e)),mch::ref2<mch::var<T>>(v)); } \
    template <typename T,  typename E>                            inline auto XTL_CONCATENATE(operator,S)(const mch::value<T>&       v,       E&&                  e) noexcept -> XTL_RETURN(mch::make_expr<FF>(v,mch::filter(std::forward<E>(e)))) \
    template <typename T,  typename E>                            inline auto XTL_CONCATENATE(operator,S)(      E&&                  e, const mch::value<T>&       v) noexcept -> XTL_RETURN(mch::make_expr<FF>(mch::filter(std::forward<E>(e)),v)) \
    template <typename F1, typename E1, typename E>               inline auto XTL_CONCATENATE(operator,S)(const mch::expr<F1,E1>&    v,       E&&                  e) noexcept -> XTL_RETURN(mch::make_expr<FF>(v,mch::filter(std::forward<E>(e)))) \
    template <typename F1, typename E1, typename E>               inline auto XTL_CONCATENATE(operator,S)(      E&&                  e, const mch::expr<F1,E1>&    v) noexcept -> XTL_RETURN(mch::make_expr<FF>(mch::filter(std::forward<E>(e)),v)) \
    template <typename F1, typename E1, typename E2, typename E>  inline auto XTL_CONCATENATE(operator,S)(const mch::expr<F1,E1,E2>& v,       E&&                  e) noexcept -> XTL_RETURN(mch::make_expr<FF>(v,mch::filter(std::forward<E>(e)))) \
    template <typename F1, typename E1, typename E2, typename E>  inline auto XTL_CONCATENATE(operator,S)(      E&&                  e, const mch::expr<F1,E1,E2>& v) noexcept -> XTL_RETURN(mch::make_expr<FF>(mch::filter(std::forward<E>(e)),v)) \
    template <typename T,  typename U>                            inline auto XTL_CONCATENATE(operator,S)(const mch::value<T>&       v, const mch::value<U>&       c) noexcept -> XTL_RETURN(mch::make_expr<FF>(v,c)) \
    template <typename T,  typename U>                            inline auto XTL_CONCATENATE(operator,S)(      mch::var<T>&         v,       mch::var<U>&         w) noexcept -> XTL_RETURN(mch::make_expr<FF>(mch::ref2<mch::var<T>>(v),mch::ref2<mch::var<T>>(w))) \
    template <typename F1, typename E1, typename E2,                           \
              typename F2, typename E3, typename E4>              inline auto XTL_CONCATENATE(operator,S)(const mch::expr<F1,E1,E2>& a, const mch::expr<F2,E3,E4>& b) noexcept -> XTL_RETURN(mch::make_expr<FF>(a,b)) \
    template <typename T,  typename U>                            inline auto XTL_CONCATENATE(operator,S)(      mch::var<T>&         v, const mch::value<U>&       c) noexcept -> XTL_RETURN(mch::make_expr<FF>(mch::ref2<mch::var<T>>(v),c)) \
    template <typename T,  typename U>                            inline auto XTL_CONCATENATE(operator,S)(const mch::value<U>&       c,       mch::var<T>&         v) noexcept -> XTL_RETURN(mch::make_expr<FF>(c,mch::ref2<mch::var<T>>(v))) \
    template <typename T,  typename F1, typename E1, typename E2> inline auto XTL_CONCATENATE(operator,S)(const mch::expr<F1,E1,E2>& e,       mch::var<T>&         v) noexcept -> XTL_RETURN(mch::make_expr<FF>(e,mch::ref2<mch::var<T>>(v))) \
    template <typename T,  typename F1, typename E1, typename E2> inline auto XTL_CONCATENATE(operator,S)(      mch::var<T>&         v, const mch::expr<F1,E1,E2>& e) noexcept -> XTL_RETURN(mch::make_expr<FF>(mch::ref2<mch::var<T>>(v),e)) \
    template <typename T,  typename F1, typename E1, typename E2> inline auto XTL_CONCATENATE(operator,S)(const mch::expr<F1,E1,E2>& e, const mch::value<T>&       c) noexcept -> XTL_RETURN(mch::make_expr<FF>(e,c))                       \
    template <typename T,  typename F1, typename E1, typename E2> inline auto XTL_CONCATENATE(operator,S)(const mch::value<T>&       c, const mch::expr<F1,E1,E2>& e) noexcept -> XTL_RETURN(mch::make_expr<FF>(c,e))
#endif
//#include "../loop_over_operators.hpp"
#include "../operators_preprocessed.hpp"
#undef  FOR_EACH_BINARY_OPERATOR
#undef  FOR_EACH_UNARY_OPERATOR
///@}

//------------------------------------------------------------------------------

#include "solvers.hpp" // Include all our solvers by default for testing purposes. Remove eventually

//------------------------------------------------------------------------------
