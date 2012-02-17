///
/// \file patterns.hpp
///
/// This file defines various patterns supported by our library.
///
/// \autor Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of the XTL framework (http://parasol.tamu.edu/xtl/).
/// Copyright (C) 2005-2011 Texas A&M University.
/// All rights reserved.
///

#pragma once

#include "exprtmpl.hpp"
#include "config.hpp"
#include <ostream>
#include <type_traits>
#include <utility>

//------------------------------------------------------------------------------

/// Helper meta-function to remove reference ans const qualification from a type.
/// Used to get an underlying selector type of Match' argument.
template <typename T>
struct underlying : std::remove_const<typename std::remove_reference<T>::type> {};

//------------------------------------------------------------------------------

// Forward declarations of various patterns supported by our library.
                                                                                    struct wildcard; ///< Wildcard pattern
template <class T>                                                                  struct value;    ///< Value pattern
template <class T>                                                                  struct variable; ///< Variable pattern
template <class T>                                                                  struct var_ref;  ///< Variable pattern (internal)
template <typename F, typename E1 = void, typename E2 = void>                       struct expr;     ///< Expression pattern
template <typename E1, typename E2>                                                 struct guard;    ///< Guard pattern
template <typename T, size_t L>                                                     struct constr0;  ///< Constructor/Type pattern of 0 arguments
template <typename T, size_t L, typename E1, typename Condition = void>             struct constr1;  ///< Constructor/Type pattern of 1 arguments
template <typename T, size_t L, typename E1, typename E2>                           struct constr2;  ///< Constructor/Type pattern of 2 arguments
template <typename T, size_t L, typename E1, typename E2, typename E3>              struct constr3;  ///< Constructor/Type pattern of 3 arguments
template <typename T, size_t L, typename E1, typename E2, typename E3, typename E4> struct constr4;  ///< Constructor/Type pattern of 4 arguments

//------------------------------------------------------------------------------

/// @is_pattern_ is a helper meta-predicate capable of distinguishing all our patterns
template <typename T>                                                               struct is_pattern_                            { enum { value = false }; };
template <>                                                                         struct is_pattern_<wildcard>                  { enum { value = true }; };
template <typename T>                                                               struct is_pattern_<value<T>>                  { enum { value = true }; };
template <typename T>                                                               struct is_pattern_<variable<T>>               { enum { value = true }; };
template <typename T>                                                               struct is_pattern_<var_ref<T>>                { enum { value = true }; };
template <typename F, typename E1, typename E2>                                     struct is_pattern_<expr<F,E1,E2>>             { enum { value = true }; };
template <typename E1, typename E2>                                                 struct is_pattern_<guard<E1,E2>>              { enum { value = true }; };
template <typename T, size_t L>                                                     struct is_pattern_<constr0<T,L>>              { enum { value = true }; };
template <typename T, size_t L, typename E1>                                        struct is_pattern_<constr1<T,L,E1>>           { enum { value = true }; };
template <typename T, size_t L, typename E1, typename E2>                           struct is_pattern_<constr2<T,L,E1,E2>>        { enum { value = true }; };
template <typename T, size_t L, typename E1, typename E2, typename E3>              struct is_pattern_<constr3<T,L,E1,E2,E3>>     { enum { value = true }; };
template <typename T, size_t L, typename E1, typename E2, typename E3, typename E4> struct is_pattern_<constr4<T,L,E1,E2,E3,E4>>  { enum { value = true }; };

/// @is_pattern is a helper meta-predicate capable of distinguishing all our patterns
template <typename T> struct is_pattern : is_pattern_<typename underlying<T>::type> {};

/// @is_expression is a helper meta-predicate that separates lazily evaluatable expressions we support
template <typename T> struct is_expression { enum { value = is_pattern<T>::value && !std::is_same<T,wildcard>::value }; };

//------------------------------------------------------------------------------

/// Predefined value representing a layout used by default when none is specified.
enum { default_layout = size_t(~0) };

/// Traits like structure that will define which members should be matched at
/// which positions. It is intentionally left undefined as user will have to
/// provide specializations for his hierarchy.
template <typename type_being_matched, size_t layout = default_layout>
struct match_members;

//------------------------------------------------------------------------------

/// A wrapper class that lets one pack a given layout and type into a view.
template <typename type_being_matched, size_t layout = default_layout>
struct view
{
    typedef type_being_matched target_type;
    static const size_t        target_layout = layout;
};

//------------------------------------------------------------------------------

/// Helper meta-function to extract target type and layout from a given type
/// that can be view.
template <typename T>
struct target_of
{
    typedef T type;
    enum { layout = default_layout };
};

/// The special case of view
template <typename T, size_t L>
struct target_of<view<T,L>>
{
    typedef T type;
    enum { layout = L };
};

//------------------------------------------------------------------------------

/// Class that describes a meta variable that matches everything.
/// This variable will match any type returned by a member, but the main
/// difference from just using a variable whose value is ignored is that
/// use of this variable will make sure the actual member is never invoked!
struct wildcard
{
    //typedef void result_type;

    // NOTE: We don't need the below application anymore since we have a
    //       specialization that never applies the actual member before
    //       passing it to this meta variable that matches everything.
    // NOTE: We add it anyways for unlikely cases when user uses wildcard in the
    //       left hand side of a guard
    template <typename U>
    bool operator()(const U& u) const noexcept { return true; }
};

//------------------------------------------------------------------------------

/// Value pattern
template <class T>
struct value
{
    typedef T result_type;
    value(const T& t) : m_value(t) {}
    value(T&& t) noexcept : m_value(std::move(t)) {}
    value(value&& v) noexcept : m_value(std::move(v.m_value)) {}
    bool operator()(const T& t) const noexcept { return m_value == t; }
    operator result_type() const noexcept { return m_value; }
    T m_value;
};

template <class T> inline value<T> val(T&& t) { return value<T>(std::forward<T>(t)); }

//------------------------------------------------------------------------------

/// Variable binding for a value type
template <class T>
struct variable
{
    variable() : m_value() {}
    variable(const T& t) : m_value(t) {}
    variable(T&& t) noexcept : m_value(std::move(t)) {}
    variable(variable&& v) noexcept : m_value(std::move(v.m_value)) {}

    typedef T result_type;

    /// We report that matching succeeded and bind the value
    bool operator()(const T& t) const
    {
        m_value = t;
        return true;
    }

    variable& operator=(const T& t) { m_value = t; return *this; }

    /// Helper conversion operator to let the variable be used in some places
    /// where T was allowed
    operator const T&() const noexcept { return m_value; }

    /// Member that will hold matching value in case of successful matching
    mutable T m_value;
};

//------------------------------------------------------------------------------

/// Variable binding for a pointer type
template <class T>
struct variable<const T*>
{
    variable() : m_value() {}
    variable(variable&& v) noexcept : m_value(std::move(v.m_value)) {}

    typedef const T* result_type;

    /// We may be applied to a value of a base type, so first we have to figure
    /// out whether they dynamic type is actually T. We report match only if it is
    template <typename U>
    bool operator()(const U* u) const
    {
        if (const T* t = dynamic_cast<const T*>(u))
        {
            m_value = t;
            return true;
        }
        else
            return false;
    }

    variable& operator=(const T* t) { m_value = t; return *this; }

    /// This distinguishes the case when type of the variable matches type of the member
    bool operator()(const T* t) const
    {
        // NOTE: This will also assign the null pointer. Should it?
        m_value = t;
        return true;
    }

    /// Helper conversion operator to let the variable be used in some places
    /// where T* is expected
    operator const T*() const noexcept { return m_value; }

    /// A helper member to let the wrapping variable be used as a pointer
    const T* operator->() const noexcept { return m_value; }

    /// A helper member to let the wrapping variable be used as a pointer
    const T& operator*()  const noexcept { return *m_value; }

    /// Member that will hold matching value in case of successful matching
    mutable const T* m_value;
};

//------------------------------------------------------------------------------

/// Variable binding for a pointer type
template <class T>
struct variable<const T&>
{
    variable() : m_value() {}
    variable(variable&& v) noexcept : m_value(std::move(v.m_value)) {}

    typedef const T& result_type;

    /// We may be applied to a value of a base type, so first we have to figure
    /// out whether they dynamic type is actually T. We report match only if it is
    template <typename U>
    bool operator()(const U& u) const
    {
        if (const T* t = dynamic_cast<const T*>(&u))
        {
            m_value = t;
            return true;
        }
        else
            return false;
    }

    /// This distinguishes the case when type of the variable matches type of the member
    bool operator()(const T& t) const
    {
        // NOTE: This will also assign the null pointer. Should it?
        m_value = &t;
        return true;
    }

    /// Helper conversion operator to let the variable be used in some places
    /// where T was allowed
    operator const T&() const noexcept { XTL_ASSERT(m_value); return *m_value; }

    /// Member that will hold matching value in case of successful matching
    mutable const T* m_value;
};

//------------------------------------------------------------------------------

/// A reference to a user provided variable
template <class T>
struct var_ref
{
    var_ref(T& var) : m_var(&var) {}
    var_ref(var_ref&& v) noexcept : m_var(v.m_var) {}

    typedef T result_type;
    operator result_type() const noexcept { return *m_var; }

    /// We report that matching succeeded and bind the value
    bool operator()(const T& t) const
    {
        *m_var = t;
        return true;
    }

    /// Member that will hold matching value in case of successful matching
    T* m_var;
};

/// A reference to a user provided variable
template <class T>
struct var_ref<variable<T>>
{
    var_ref(variable<T>& var) : m_var(&var) {}
    var_ref(var_ref&& v) noexcept : m_var(v.m_var) {}

    typedef T result_type;
    operator result_type() const noexcept { return *m_var; }

    /// We report that matching succeeded and bind the value
    bool operator()(const T& t) const
    {
        *m_var = t;
        return true;
    }

    /// Member that will hold matching value in case of successful matching
    variable<T>* m_var;
};

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
template <typename F, typename E1>
struct expr<F,E1>
{
    static_assert(is_expression<E1>::value, "Argument E1 of a unary expression-pattern must be a lazy expression");

    expr(const E1& e1) : m_e1(e1) {}
    expr(E1&& e1) noexcept : m_e1(std::move(e1)) {}
    expr(expr&& e) noexcept : m_e1(std::move(e.m_e1)) {}

    typedef typename std::remove_const<decltype(F()(std::declval<typename E1::result_type>()))>::type result_type; // We needed to add remove_const here as MSVC was returning const T
    operator result_type() const { return eval(*this); }
    //bool operator()(const result_type& t) const { return solve(*this,t); }
    template <typename U>
    bool operator()(const U& u) const { return solve(*this,u); }
    //bool operator()(const result_type& t) const 
    //{
    //    typename E1::result_type v;
    //    return solver<F>()(v,t) && m_e1(v);
    //}

    const E1 m_e1;
};

template <typename F, typename E1, typename E2>
struct expr
{
    static_assert(is_expression<E1>::value, "Argument E1 of a binary expression-pattern must be a lazy expression");
    static_assert(is_expression<E2>::value, "Argument E2 of a binary expression-pattern must be a lazy expression");

    expr(const E1& e1, const E2& e2) : m_e1(e1), m_e2(e2) {}
    expr(E1&& e1, E2&& e2) noexcept : m_e1(std::move(e1)), m_e2(std::move(e2)) {}
    expr(expr&& e) noexcept : m_e1(std::move(e.m_e1)), m_e2(std::move(e.m_e2)) {}

    typedef typename std::remove_const<decltype(F()(std::declval<typename E1::result_type>(),std::declval<typename E2::result_type>()))>::type result_type; // We needed to add remove_const here as MSVC was returning const T
    operator result_type() const { return eval(*this); }
    //bool operator()(const result_type& t) const { return solve(*this,t); }
    template <typename U>
    bool operator()(const U& u) const { return solve(*this,u); }
    //bool operator()(const result_type& t) const 
    //{
    //    typename E1::result_type v;
    //    return solver<F>()(v,eval(m_e2),t) && m_e1(v);
    //}

    const E1 m_e1;
    const E2 m_e2;
};

template <typename F, typename E1>
    auto make_expr(E1&& e1) noexcept -> XTL_RETURN
    (
        expr<F,typename underlying<E1>::type>(std::forward<E1>(e1))
    )

template <typename F, typename E1, typename E2>
    auto make_expr(E1&& e1, E2&& e2) noexcept -> XTL_RETURN
    (
        expr<F,typename underlying<E1>::type,typename underlying<E2>::type>(std::forward<E1>(e1),std::forward<E2>(e2))
    )

//------------------------------------------------------------------------------

/// Guard pattern
template <typename E1, typename E2>
struct guard
{
    static_assert(is_pattern<E1>::value,    "Argument E1 of a guard-pattern must be a pattern");
    static_assert(is_expression<E2>::value, "Argument E2 of a guard-pattern must be a lazy expression");

    guard(const E1& e1, const E2& e2) : m_e1(e1), m_e2(e2) {}
    guard(E1&& e1, E2&& e2) noexcept : m_e1(std::move(e1)), m_e2(std::move(e2)) {}
    guard(guard&& e) noexcept : m_e1(std::move(e.m_e1)), m_e2(std::move(e.m_e2)) {}

    template <typename U>
    bool operator()(const U& u) const { return m_e1(u) && eval(m_e2); }

    const E1 m_e1;
    const E2 m_e2;
};

template <typename E1, typename E2>
    auto make_guard(E1&& e1, E2&& e2) noexcept -> XTL_RETURN
    (
        guard<typename underlying<E1>::type,typename underlying<E2>::type>(std::forward<E1>(e1),std::forward<E2>(e2))
    )

//------------------------------------------------------------------------------

template <typename E1, typename E2>
struct expr_or
{
    static_assert(is_expression<E1>::value, "Argument E1 of an or-pattern must be a lazy expression");
    static_assert(is_expression<E2>::value, "Argument E2 of an or-pattern must be a lazy expression");

    expr_or(const E1& e1, const E2& e2) : m_e1(e1), m_e2(e2) {}
    expr_or(E1&& e1, E2&& e2) noexcept : m_e1(std::move(e1)), m_e2(std::move(e2)) {}
    expr_or(expr_or&& e) noexcept : m_e1(std::move(e.m_e1)), m_e2(std::move(e.m_e2)) {}

    template <typename U> bool operator()(const U* u) const { return m_e1(u) || m_e2(u); }
    template <typename U> bool operator()(      U* u) const { return m_e1(u) || m_e2(u); }
    template <typename U> bool operator()(const U& u) const { return operator()(&u); }
    template <typename U> bool operator()(      U& u) const { return operator()(&u); }

    const E1 m_e1;
    const E2 m_e2;
};

//------------------------------------------------------------------------------

template <typename T>                                             var_ref<variable<T>> filter(variable<T>& t) noexcept { return var_ref<variable<T>>(t); }
template <typename P> typename std::enable_if< is_pattern<P>::value,         P >::type filter(    const P& p) noexcept { return p; }
template <typename T> typename std::enable_if<!is_pattern<T>::value,   value<T>>::type filter(    const T& t) noexcept { return value<T>(t); }
template <typename T> typename std::enable_if<!is_pattern<T>::value, var_ref<T>>::type filter(          T& t) noexcept { return var_ref<T>(t); }

//------------------------------------------------------------------------------

template <typename E, typename T>
auto operator|=(variable<T>& v, E&& e) noexcept -> XTL_RETURN
(
    make_guard(var_ref<variable<T>>(v),filter(std::forward<E>(e)))
)

template <typename P, typename E>
auto operator|=(P&& p, E&& e) noexcept -> XTL_RETURN_ENABLE_IF
(
    is_pattern<P>::value && is_expression<E>::value,
    make_guard(std::forward<P>(p),filter(std::forward<E>(e)))
)

//------------------------------------------------------------------------------

// NOTE: We need otherwise redundant non-const overloads to make sure that our 
//       overloads creating expression templates based on the second argument are 
//       not chosen!
template <typename T> std::ostream& operator<<(std::ostream& os, const variable<T >& v) { return os <<  v.m_value; }
template <typename T> std::ostream& operator<<(std::ostream& os,       variable<T >& v) { return os <<  v.m_value; }
template <typename T> std::ostream& operator<<(std::ostream& os, const variable<T*>& v) { return os << *v.m_value; }
template <typename T> std::ostream& operator<<(std::ostream& os,       variable<T*>& v) { return os << *v.m_value; }

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
    template <typename T, typename E>                            inline auto XTL_CONCATENATE(operator,S)(      variable<T>&    v,       E&&             e) noexcept -> XTL_RETURN(make_expr<FF>(var_ref<variable<T>>(v),filter(std::forward<E>(e)))) \
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
#include "loop_over_operators.hpp"
#undef  FOR_EACH_BINARY_OPERATOR
#undef  FOR_EACH_UNARY_OPERATOR
///@}

//------------------------------------------------------------------------------

template <typename T> T inline eval(const value<T>& e)    { return e.m_value; }
template <typename T> T inline eval(const variable<T>& e) { return e.m_value; }
template <typename T> T inline eval(const var_ref<T>& e)  { return *e.m_var; }
template <typename T> T inline eval(const var_ref<variable<T>>& e) { return e.m_var->m_value; }
template <typename F, typename E1>              typename expr<F,E1>::result_type    inline eval(const expr<F,E1>&    e) { return F()(eval(e.m_e1)); }
template <typename F, typename E1, typename E2> typename expr<F,E1,E2>::result_type inline eval(const expr<F,E1,E2>& e) { return F()(eval(e.m_e1),eval(e.m_e2)); }

//------------------------------------------------------------------------------

#define XTL_DEBUG_APPLY_MEMBER(what, c, f) //XTL_DEBUG_ONLY(std::clog << "\nApplying " what << c << " of type " << typeid(*c).name() << std::endl)

//------------------------------------------------------------------------------

template <class C, class T, typename R>
inline R apply_member(const C* c, R (T::*method)() const)
{
    XTL_DEBUG_APPLY_MEMBER("const member function to const instance ", c, method);
    return (c->*method)();
}

//------------------------------------------------------------------------------

template <class C, class T, typename R>
inline R apply_member(      C* c, R (T::*method)() const)
{
    XTL_DEBUG_APPLY_MEMBER("const member function to non-const instance ", c, method);
    return (c->*method)();
}

//------------------------------------------------------------------------------

template <class C, class T, typename R>
inline R apply_member(      C* c, R (T::*method)()      )
{
    XTL_DEBUG_APPLY_MEMBER("non-const member function to non-const instance ", c, method);
    return (c->*method)();
}

//------------------------------------------------------------------------------

template <class C, class T, typename R>
inline const R& apply_member(const C* c, R T::*field) noexcept
{
    XTL_DEBUG_APPLY_MEMBER("data member to const instance ", c, field);
    return c->*field;
}

//------------------------------------------------------------------------------

template <class C, class T, typename R>
inline       R& apply_member(      C* c, R T::*field) noexcept
{
    XTL_DEBUG_APPLY_MEMBER("data member to non-const instance ", c, field);
    return c->*field;
}

//------------------------------------------------------------------------------

template <class C, class T, typename R>
inline R apply_member(const C* c, R (*func)(const T*))
{
    XTL_DEBUG_APPLY_MEMBER("external function taking const pointer to const instance ", c, func);
    return (*func)(c);
}

//------------------------------------------------------------------------------

template <class C, class T, typename R>
inline R apply_member(      C* c, R (*func)(const T*))
{
    XTL_DEBUG_APPLY_MEMBER("external function taking const pointer to non-const instance ", c, func);
    return (*func)(c);
}

//------------------------------------------------------------------------------

template <class C, class T, typename R>
inline R apply_member(      C* c, R (*func)(      T*))
{
    XTL_DEBUG_APPLY_MEMBER("external function taking non-const pointer to non-const instance ", c, func);
    return (*func)(c);
}

//------------------------------------------------------------------------------

template <class C, class T, typename R>
inline R apply_member(const C* c, R (*func)(const T&))
{
    XTL_DEBUG_APPLY_MEMBER("external function taking const reference to const instance ", c, func);
    return (*func)(*c);
}

//------------------------------------------------------------------------------

template <class C, class T, typename R>
inline R apply_member(      C* c, R (*func)(const T&))
{
    XTL_DEBUG_APPLY_MEMBER("external function taking const reference to non-const instance ", c, func);
    return (*func)(*c);
}

//------------------------------------------------------------------------------

template <class C, class T, typename R>
inline R apply_member(      C* c, R (*func)(      T&))
{
    XTL_DEBUG_APPLY_MEMBER("external function taking non-const reference to non-const instance ", c, func);
    return (*func)(*c);
}

//------------------------------------------------------------------------------

/// We need this extra indirection to be able to intercept when we are trying to
/// match a meta variable _ of type wildcard, that matches everything of
/// any type. In this case we don't even want to invoke the underlain member!
template <typename E, typename C, typename M>
inline bool apply_expression(const E& e, const C* c, M m)
{
    #ifdef _MSC_VER
    #pragma warning( disable : 4800 )
    #endif

    return e(apply_member(c, m));
}

template <typename E, typename C, typename M>
inline bool apply_expression(const E& e,       C* c, M m)
{
    #ifdef _MSC_VER
    #pragma warning( disable : 4800 )
    #endif

    return e(apply_member(c, m));
}

/// This is the specialization that makes the member not to be invoked when we
/// are matching against the meta variable _ that matches everything.
template <typename C, typename M>
inline bool apply_expression(const wildcard&, const C*, M) noexcept
{
    return true;
}

template <typename C, typename M>
inline bool apply_expression(const wildcard&,       C*, M) noexcept
{
    return true;
}

//------------------------------------------------------------------------------

/// Constructor/Type pattern of 0 arguments
template <typename T, size_t layout>
struct constr0
{
    ///@{
    /// Constructor patterns can be uniformly matched against pointers and 
    /// references to polymorphic objects. This uniformity is needed for 
    /// composability of constructor patterns, since application operators
    /// return a pointer to indicate whether pattern was accepted or refuted.
    /// The following set of application operators distinguishes:
    ///  - the arguments passed by reference from those passed by pointers
    ///  - whether the target type matches the subject type (to avoid dynamic_cast)
    ///  - const from non-const arguments to propagate constness further.
    template <typename U> const T* operator()(const U* u) const noexcept { return dynamic_cast<const T*>(u); }
    template <typename U>       T* operator()(      U* u) const noexcept { return dynamic_cast<      T*>(u); }
    template <typename U> const T* operator()(const U& u) const noexcept { return operator()(&u); }
    template <typename U>       T* operator()(      U& u) const noexcept { return operator()(&u); }
                          const T* operator()(const T* t) const noexcept { return t; }
                                T* operator()(      T* t) const noexcept { return t; }
    ///@}

    template <typename E> expr_or<constr0,E> operator|(const E& e) const noexcept { return expr_or<constr0,E>(*this,e); }
};

//------------------------------------------------------------------------------

/// Constructor/Type pattern of 1 arguments
template <typename T, size_t layout, typename E1, typename Condition>
struct constr1
{
    static_assert(is_pattern<E1>::value, "Argument E1 of constructor-pattern must be a pattern");

    constr1(const E1& e1) : m_e1(e1) {}
    constr1(E1&& e1) noexcept : m_e1(std::move(e1)) {}
    constr1(constr1&& src) noexcept : m_e1(std::move(src.m_e1)) {}

    /// Helper function that does the actual structural matching once we have
    /// uncovered a value of the target type. Applies to a const argument!
    const T* match_structure(const T* t) const
    {
        XTL_ASSERT(t); // This helper function assumes t cannot be a nullptr
        return apply_expression(m_e1, t, match_members<T,layout>::member0()) // error C2027: use of undefined type 'match_members<type_being_matched,layout>'
             ? t                                                             // here means you did not provide bindings for type_being_matched and layout
             : 0;                                                            // described in the details of error message. See @match_members and @CM
                                                                             // error: incomplete type 'match_members<type_being_matched, layout>' used in nested name specifier (see above description for Visual C++)
    }
    /// Helper function that does the actual structural matching once we have
    /// uncovered a value of the target type. Applies to a non-const argument!
          T* match_structure(      T* t) const
    {
        XTL_ASSERT(t); // This helper function assumes t cannot be a nullptr
        return apply_expression(m_e1, t, match_members<T,layout>::member0()) // error C2027: use of undefined type 'match_members<type_being_matched,layout>'
             ? t                                                             // here means you did not provide bindings for type_being_matched and layout
             : 0;                                                            // described in the details of error message. See @match_members and @CM
                                                                             // error: incomplete type 'match_members<type_being_matched, layout>' used in nested name specifier (see above description for Visual C++)
    }

    ///@{
    /// Constructor patterns can be uniformly matched against pointers and 
    /// references to polymorphic objects. This uniformity is needed for 
    /// composability of constructor patterns, since application operators
    /// return a pointer to indicate whether pattern was accepted or refuted.
    /// The following set of application operators distinguishes:
    ///  - the arguments passed by reference from those passed by pointers
    ///  - whether the target type matches the subject type (to avoid dynamic_cast)
    ///  - const from non-const arguments to propagate constness further.
    template <typename U> const T* operator()(const U* u) const { return operator()(dynamic_cast<const T*>(u)); }
    template <typename U>       T* operator()(      U* u) const { return operator()(dynamic_cast<      T*>(u)); }
    template <typename U> const T* operator()(const U& u) const { return operator()(&u); }
    template <typename U>       T* operator()(      U& u) const { return operator()(&u); }
                          const T* operator()(const T* t) const { return t ? match_structure(t) : 0; }
                                T* operator()(      T* t) const { return t ? match_structure(t) : 0; }
    ///@}

    template <typename E> expr_or<constr1,E> operator|(const E& e) const noexcept { return expr_or<constr1,E>(*this,e); }

    const E1 m_e1; ///< Expression representing 1st operand
};

/// This is a specialization of the general one-argument construction pattern
/// for the case when the type of that argument matches the target type. This
/// is needed to be able to apply algebraic patterns to the entire object for
/// those cases when object already provides its structural decomposition.
/// For example std::complex<T> can be decomposed into two subcomponents and
/// accordingly to our logic when second argument is omitted, we should only 
/// match against the first one. Now if that first argument is of type 
/// complex<T> and not just T, we are trying to match against the whole object,
/// not against its first subcomponent! One can imagine a recursive data 
/// structure however where the type of the first subcomponent also matches
/// the actual type (e.g. list), so we must give preference in such case to
/// the structurally matched subcomponent instead of matching against the whole
/// object. 
/// FIX: Add extra condition that makes sure the type of first subcomponent is
///      not the same as the type itself!
template <typename T, size_t layout, typename E1>
struct constr1<T,layout,E1,typename std::enable_if<std::is_same<T,typename E1::result_type>::value>::type>
{
    static_assert(is_pattern<E1>::value, "Argument E1 of constructor-pattern must be a pattern");

    constr1(const E1& e1) : m_e1(e1) {}
    constr1(E1&& e1) noexcept : m_e1(std::move(e1)) {}
    constr1(constr1&& src) noexcept : m_e1(std::move(src.m_e1)) {}

    /// Helper function that does the actual structural matching once we have
    /// uncovered a value of the target type. Applies to a const argument!
    const T* match_structure(const T* t) const
    {
        XTL_ASSERT(t); // This helper function assumes t cannot be a nullptr
        return solve(m_e1,*t) ? t : 0;
    }
    /// Helper function that does the actual structural matching once we have
    /// uncovered a value of the target type. Applies to a non-const argument!
          T* match_structure(      T* t) const
    {
        XTL_ASSERT(t); // This helper function assumes t cannot be a nullptr
        return solve(m_e1,*t) ? t : 0;
    }

    ///@{
    /// Constructor patterns can be uniformly matched against pointers and 
    /// references to polymorphic objects. This uniformity is needed for 
    /// composability of constructor patterns, since application operators
    /// return a pointer to indicate whether pattern was accepted or refuted.
    /// The following set of application operators distinguishes:
    ///  - the arguments passed by reference from those passed by pointers
    ///  - whether the target type matches the subject type (to avoid dynamic_cast)
    ///  - const from non-const arguments to propagate constness further.
    template <typename U> const T* operator()(const U* u) const { return operator()(dynamic_cast<const T*>(u)); }
    template <typename U>       T* operator()(      U* u) const { return operator()(dynamic_cast<      T*>(u)); }
    template <typename U> const T* operator()(const U& u) const { return operator()(&u); }
    template <typename U>       T* operator()(      U& u) const { return operator()(&u); }
                          const T* operator()(const T* t) const { return t ? match_structure(t) : 0; }
                                T* operator()(      T* t) const { return t ? match_structure(t) : 0; }
    ///@}

    template <typename E> expr_or<constr1,E> operator|(const E& e) const noexcept { return expr_or<constr1,E>(*this,e); }

    const E1 m_e1; ///< Expression representing 1st operand
};

//------------------------------------------------------------------------------

/// Constructor/Type pattern of 2 arguments
template <typename T, size_t layout, typename E1, typename E2>
struct constr2
{
    static_assert(is_pattern<E1>::value, "Argument E1 of constructor-pattern must be a pattern");
    static_assert(is_pattern<E2>::value, "Argument E2 of constructor-pattern must be a pattern");

    constr2(const E1& e1, const E2& e2) : m_e1(e1), m_e2(e2) {}
    constr2(E1&& e1, E2&& e2) noexcept : m_e1(std::move(e1)), m_e2(std::move(e2)) {}
    constr2(constr2&& src) noexcept : m_e1(std::move(src.m_e1)), m_e2(std::move(src.m_e2)) {}

    /// Helper function that does the actual structural matching once we have
    /// uncovered a value of the target type. Applies to a const argument!
    const T* match_structure(const T* t) const
    {
        XTL_ASSERT(t); // This helper function assumes t cannot be a nullptr
        return apply_expression(m_e1, t, match_members<T,layout>::member0()) // error C2027: use of undefined type 'match_members<type_being_matched,layout>'
            && apply_expression(m_e2, t, match_members<T,layout>::member1()) // here means you did not provide bindings for type_being_matched and layout
             ? t                                                             // described in the details of error message. See @match_members and @CM
             : 0;                                                            // error: incomplete type 'match_members<type_being_matched, layout>' used in nested name specifier (see above description for Visual C++)
    }
    /// Helper function that does the actual structural matching once we have
    /// uncovered a value of the target type. Applies to a non-const argument!
          T* match_structure(      T* t) const
    {
        XTL_ASSERT(t); // This helper function assumes t cannot be a nullptr
        return apply_expression(m_e1, t, match_members<T,layout>::member0()) // error C2027: use of undefined type 'match_members<type_being_matched,layout>'
            && apply_expression(m_e2, t, match_members<T,layout>::member1()) // here means you did not provide bindings for type_being_matched and layout
             ? t                                                             // described in the details of error message. See @match_members and @CM
             : 0;                                                            // error: incomplete type 'match_members<type_being_matched, layout>' used in nested name specifier (see above description for Visual C++)
    }

    ///@{
    /// Constructor patterns can be uniformly matched against pointers and 
    /// references to polymorphic objects. This uniformity is needed for 
    /// composability of constructor patterns, since application operators
    /// return a pointer to indicate whether pattern was accepted or refuted.
    /// The following set of application operators distinguishes:
    ///  - the arguments passed by reference from those passed by pointers
    ///  - whether the target type matches the subject type (to avoid dynamic_cast)
    ///  - const from non-const arguments to propagate constness further.
    template <typename U> const T* operator()(const U* u) const { return operator()(dynamic_cast<const T*>(u)); }
    template <typename U>       T* operator()(      U* u) const { return operator()(dynamic_cast<      T*>(u)); }
    template <typename U> const T* operator()(const U& u) const { return operator()(&u); }
    template <typename U>       T* operator()(      U& u) const { return operator()(&u); }
                          const T* operator()(const T* t) const { return t ? match_structure(t) : 0; }
                                T* operator()(      T* t) const { return t ? match_structure(t) : 0; }
    ///@}

    template <typename E> expr_or<constr2,E> operator|(const E& e) const noexcept { return expr_or<constr2,E>(*this,e); }

    const E1 m_e1; ///< Expression representing 1st operand
    const E2 m_e2; ///< Expression representing 2nd operand
};

//------------------------------------------------------------------------------

/// Constructor/Type pattern of 3 arguments
template <typename T, size_t layout, typename E1, typename E2, typename E3>
struct constr3
{
    static_assert(is_pattern<E1>::value, "Argument E1 of constructor-pattern must be a pattern");
    static_assert(is_pattern<E2>::value, "Argument E2 of constructor-pattern must be a pattern");
    static_assert(is_pattern<E3>::value, "Argument E3 of constructor-pattern must be a pattern");

    constr3(const E1& e1, const E2& e2, const E3& e3) : m_e1(e1), m_e2(e2), m_e3(e3) {}
    constr3(E1&& e1, E2&& e2, E3&& e3) noexcept : m_e1(std::move(e1)), m_e2(std::move(e2)), m_e3(std::move(e3)) {}
    constr3(constr3&& src) noexcept : m_e1(std::move(src.m_e1)), m_e2(std::move(src.m_e2)), m_e3(std::move(src.m_e3)) {}

    /// Helper function that does the actual structural matching once we have
    /// uncovered a value of the target type. Applies to a const argument!
    const T* match_structure(const T* t) const
    {
        XTL_ASSERT(t); // This helper function assumes t cannot be a nullptr
        return apply_expression(m_e1, t, match_members<T,layout>::member0()) // error C2027: use of undefined type 'match_members<type_being_matched,layout>'
            && apply_expression(m_e2, t, match_members<T,layout>::member1()) // here means you did not provide bindings for type_being_matched and layout
            && apply_expression(m_e3, t, match_members<T,layout>::member2()) // described in the details of error message. See @match_members and @CM
             ? t                                                             // error: incomplete type 'match_members<type_being_matched, layout>' used in nested name specifier (see above description for Visual C++)
             : 0;
    }
    /// Helper function that does the actual structural matching once we have
    /// uncovered a value of the target type. Applies to a non-const argument!
          T* match_structure(      T* t) const
    {
        XTL_ASSERT(t); // This helper function assumes t cannot be a nullptr
        return apply_expression(m_e1, t, match_members<T,layout>::member0()) // error C2027: use of undefined type 'match_members<type_being_matched,layout>'
            && apply_expression(m_e2, t, match_members<T,layout>::member1()) // here means you did not provide bindings for type_being_matched and layout
            && apply_expression(m_e3, t, match_members<T,layout>::member2()) // described in the details of error message. See @match_members and @CM
             ? t                                                             // error: incomplete type 'match_members<type_being_matched, layout>' used in nested name specifier (see above description for Visual C++)
             : 0;
    }

    ///@{
    /// Constructor patterns can be uniformly matched against pointers and 
    /// references to polymorphic objects. This uniformity is needed for 
    /// composability of constructor patterns, since application operators
    /// return a pointer to indicate whether pattern was accepted or refuted.
    /// The following set of application operators distinguishes:
    ///  - the arguments passed by reference from those passed by pointers
    ///  - whether the target type matches the subject type (to avoid dynamic_cast)
    ///  - const from non-const arguments to propagate constness further.
    template <typename U> const T* operator()(const U* u) const { return operator()(dynamic_cast<const T*>(u)); }
    template <typename U>       T* operator()(      U* u) const { return operator()(dynamic_cast<      T*>(u)); }
    template <typename U> const T* operator()(const U& u) const { return operator()(&u); }
    template <typename U>       T* operator()(      U& u) const { return operator()(&u); }
                          const T* operator()(const T* t) const { return t ? match_structure(t) : 0; }
                                T* operator()(      T* t) const { return t ? match_structure(t) : 0; }
    ///@}

    template <typename E> expr_or<constr3,E> operator|(const E& e) const noexcept { return expr_or<constr3,E>(*this,e); }

    const E1 m_e1; ///< Expression representing 1st operand
    const E2 m_e2; ///< Expression representing 2nd operand
    const E3 m_e3; ///< Expression representing 3rd operand
};

//------------------------------------------------------------------------------

/// Constructor/Type pattern of 4 arguments
template <typename T, size_t layout, typename E1, typename E2, typename E3, typename E4>
struct constr4
{
    static_assert(is_pattern<E1>::value, "Argument E1 of constructor-pattern must be a pattern");
    static_assert(is_pattern<E2>::value, "Argument E2 of constructor-pattern must be a pattern");
    static_assert(is_pattern<E3>::value, "Argument E3 of constructor-pattern must be a pattern");
    static_assert(is_pattern<E4>::value, "Argument E4 of constructor-pattern must be a pattern");

    constr4(const E1& e1, const E2& e2, const E3& e3, const E4& e4) : m_e1(e1), m_e2(e2), m_e3(e3), m_e4(e4) {}
    constr4(E1&& e1, E2&& e2, E3&& e3, E4&& e4) noexcept : m_e1(std::move(e1)), m_e2(std::move(e2)), m_e3(std::move(e3)), m_e4(std::move(e4)) {}
    constr4(constr4&& src) noexcept : m_e1(std::move(src.m_e1)), m_e2(std::move(src.m_e2)), m_e3(std::move(src.m_e3)), m_e4(std::move(src.m_e4)) {}

    /// Helper function that does the actual structural matching once we have
    /// uncovered a value of the target type. Applies to a const argument!
    const T* match_structure(const T* t) const
    {
        XTL_ASSERT(t); // This helper function assumes t cannot be a nullptr
        return apply_expression(m_e1, t, match_members<T,layout>::member0()) // error C2027: use of undefined type 'match_members<type_being_matched,layout>'
            && apply_expression(m_e2, t, match_members<T,layout>::member1()) // here means you did not provide bindings for type_being_matched and layout
            && apply_expression(m_e3, t, match_members<T,layout>::member2()) // described in the details of error message. See @match_members and @CM
            && apply_expression(m_e4, t, match_members<T,layout>::member3()) // error: incomplete type 'match_members<type_being_matched, layout>' used in nested name specifier (see above description for Visual C++)
             ? t
             : 0;
    }
    /// Helper function that does the actual structural matching once we have
    /// uncovered a value of the target type. Applies to a non-const argument!
          T* match_structure(      T* t) const
    {
        XTL_ASSERT(t); // This helper function assumes t cannot be a nullptr
        return apply_expression(m_e1, t, match_members<T,layout>::member0()) // error C2027: use of undefined type 'match_members<type_being_matched,layout>'
            && apply_expression(m_e2, t, match_members<T,layout>::member1()) // here means you did not provide bindings for type_being_matched and layout
            && apply_expression(m_e3, t, match_members<T,layout>::member2()) // described in the details of error message. See @match_members and @CM
            && apply_expression(m_e4, t, match_members<T,layout>::member3()) // error: incomplete type 'match_members<type_being_matched, layout>' used in nested name specifier (see above description for Visual C++)
             ? t
             : 0;
    }

    ///@{
    /// Constructor patterns can be uniformly matched against pointers and 
    /// references to polymorphic objects. This uniformity is needed for 
    /// composability of constructor patterns, since application operators
    /// return a pointer to indicate whether pattern was accepted or refuted.
    /// The following set of application operators distinguishes:
    ///  - the arguments passed by reference from those passed by pointers
    ///  - whether the target type matches the subject type (to avoid dynamic_cast)
    ///  - const from non-const arguments to propagate constness further.
    template <typename U> const T* operator()(const U* u) const { return operator()(dynamic_cast<const T*>(u)); }
    template <typename U>       T* operator()(      U* u) const { return operator()(dynamic_cast<      T*>(u)); }
    template <typename U> const T* operator()(const U& u) const { return operator()(&u); }
    template <typename U>       T* operator()(      U& u) const { return operator()(&u); }
                          const T* operator()(const T* t) const { return t ? match_structure(t) : 0; }
                                T* operator()(      T* t) const { return t ? match_structure(t) : 0; }
    ///@}

    template <typename E> expr_or<constr4,E> operator|(const E& e) const noexcept { return expr_or<constr4,E>(*this,e); }

    const E1 m_e1; ///< Expression representing 1st operand
    const E2 m_e2; ///< Expression representing 2nd operand
    const E3 m_e3; ///< Expression representing 3rd operand
    const E4 m_e4; ///< Expression representing 4th operand
};

//------------------------------------------------------------------------------

/// 0-argument version of a helper function to @match that accepts arguments that
/// have been already preprocessed with @filter to convert regular variables into
/// @var_ref and constants into @value.
/// \note This version will be called from @match with a non-@view target type
template <typename T, size_t layout>
inline constr0<T,layout> match_ex(const view<T,layout>&) noexcept
{
    return constr0<T,layout>();
}

/// 0-argument version of a helper function to @match that accepts arguments that
/// have been already preprocessed with @filter to convert regular variables into
/// @var_ref and constants into @value.
/// \note This version will be called from @match that had its target type a @view
template <typename T, size_t layout>
inline constr0<T,layout> match_ex(const view<view<T,layout>>&) noexcept
{
    return constr0<T,layout>();
}

/// A 0-argument version of a tree-pattern constructor. Target type is allowed
/// to be a @view here.
template <typename T>
inline auto match() noexcept -> XTL_RETURN(match_ex(view<T>()))

/// A 0-argument version of a tree-pattern constructor that takes layout in
/// addition to the target type.
/// \note @view is not supposed to be passed as a target type to this version
///       of the function because we will then have two potentially conflicting
///       layouts. Any layout different from @default_layout passed here will
///       result in a compile time error.
template <typename T, size_t layout>
inline auto match() noexcept -> XTL_RETURN(match_ex(view<T,layout>()))

//------------------------------------------------------------------------------

/// 1-argument version of a helper function to @match that accepts arguments that
/// have been already preprocessed with @filter to convert regular variables into
/// @var_ref and constants into @value.
/// \note This version will be called from @match with a non-@view target type
template <typename T, size_t layout, typename E1>
inline constr1<T,layout,E1> match_ex(const view<T,layout>&, E1&& e1) noexcept
{
    return constr1<T,layout,E1>(std::forward<E1>(e1));
}

/// 1-argument version of a helper function to @match that accepts arguments that
/// have been already preprocessed with @filter to convert regular variables into
/// @var_ref and constants into @value.
/// \note This version will be called from @match that had its target type a @view
template <typename T, size_t layout, typename E1>
inline constr1<T,layout,E1> match_ex(const view<view<T,layout>>&, E1&& e1) noexcept
{
    return constr1<T,layout,E1>(std::forward<E1>(e1));
}

/// A 1-argument version of a tree-pattern constructor. Target type is allowed
/// to be a @view here.
template <typename T, typename E1>
inline auto match(E1&& e1) noexcept -> XTL_RETURN
(
    match_ex(
        view<T>(),
        filter(std::forward<E1>(e1))
    )
)

/// A 1-argument version of a tree-pattern constructor that takes layout in
/// addition to the target type.
/// \note @view is not supposed to be passed as a target type to this version
///       of the function because we will then have two potentially conflicting
///       layouts. Any layout different from @default_layout passed here will
///       result in a compile time error.
template <typename T, size_t layout, typename E1>
inline auto match(E1&& e1) noexcept -> XTL_RETURN
(
    match_ex(
        view<T,layout>(),
        filter(std::forward<E1>(e1))
    )
)

//------------------------------------------------------------------------------

/// 2-argument version of a helper function to @match that accepts arguments that
/// have been already preprocessed with @filter to convert regular variables into
/// @var_ref and constants into @value.
/// \note This version will be called from @match with a non-@view target type
template <typename T, size_t layout, typename E1, typename E2>
inline constr2<T,layout,E1,E2> match_ex(const view<T,layout>&, E1&& e1, E2&& e2) noexcept
{
    return constr2<T,layout,E1,E2>(
            std::forward<E1>(e1),
            std::forward<E2>(e2)
           );
}

/// 2-argument version of a helper function to @match that accepts arguments that
/// have been already preprocessed with @filter to convert regular variables into
/// @var_ref and constants into @value.
/// \note This version will be called from @match that had its target type a @view
template <typename T, size_t layout, typename E1, typename E2>
inline constr2<T,layout,E1,E2> match_ex(const view<view<T,layout>>&, E1&& e1, E2&& e2) noexcept
{
    return constr2<T,layout,E1,E2>(
            std::forward<E1>(e1),
            std::forward<E2>(e2)
           );
}

/// A 2-argument version of a tree-pattern constructor. Target type is allowed
/// to be a @view here.
template <typename T, typename E1, typename E2>
inline auto match(E1&& e1, E2&& e2) noexcept -> XTL_RETURN
(
    match_ex(
        view<T>(),
        filter(std::forward<E1>(e1)),
        filter(std::forward<E2>(e2))
    )
)

/// A 2-argument version of a tree-pattern constructor that takes layout in
/// addition to the target type.
/// \note @view is not supposed to be passed as a target type to this version
///       of the function because we will then have two potentially conflicting
///       layouts. Any layout different from @default_layout passed here will
///       result in a compile time error.
template <typename T, size_t layout, typename E1, typename E2>
inline auto match(E1&& e1, E2&& e2) noexcept -> XTL_RETURN
(
    match_ex(
        view<T,layout>(),
        filter(std::forward<E1>(e1)),
        filter(std::forward<E2>(e2))
    )
)

//------------------------------------------------------------------------------

/// 3-argument version of a helper function to @match that accepts arguments that
/// have been already preprocessed with @filter to convert regular variables into
/// @var_ref and constants into @value.
/// \note This version will be called from @match with a non-@view target type
template <typename T, size_t layout, typename E1, typename E2, typename E3>
inline constr3<T,layout,E1,E2,E3> match_ex(const view<T,layout>&, E1&& e1, E2&& e2, E3&& e3) noexcept
{
    return constr3<T,layout,E1,E2,E3>(
            std::forward<E1>(e1),
            std::forward<E2>(e2),
            std::forward<E3>(e3)
           );
}

/// 3-argument version of a helper function to @match that accepts arguments that
/// have been already preprocessed with @filter to convert regular variables into
/// @var_ref and constants into @value.
/// \note This version will be called from @match that had its target type a @view
template <typename T, size_t layout, typename E1, typename E2, typename E3>
inline constr3<T,layout,E1,E2,E3> match_ex(const view<view<T,layout>>&, E1&& e1, E2&& e2, E3&& e3) noexcept
{
    return constr3<T,layout,E1,E2,E3>(
            std::forward<E1>(e1),
            std::forward<E2>(e2),
            std::forward<E3>(e3)
           );
}

/// A 3-argument version of a tree-pattern constructor. Target type is allowed
/// to be a @view here.
template <typename T, typename E1, typename E2, typename E3>
inline auto match(E1&& e1, E2&& e2, E3&& e3) noexcept -> XTL_RETURN
(
    match_ex(
        view<T>(),
        filter(std::forward<E1>(e1)),
        filter(std::forward<E2>(e2)),
        filter(std::forward<E3>(e3))
    )
)

/// A 3-argument version of a tree-pattern constructor that takes layout in
/// addition to the target type.
/// \note @view is not supposed to be passed as a target type to this version
///       of the function because we will then have two potentially conflicting
///       layouts. Any layout different from @default_layout passed here will
///       result in a compile time error.
template <typename T, size_t layout, typename E1, typename E2, typename E3>
inline auto match(E1&& e1, E2&& e2, E3&& e3) noexcept -> XTL_RETURN
(
    match_ex(
        view<T,layout>(),
        filter(std::forward<E1>(e1)),
        filter(std::forward<E2>(e2)),
        filter(std::forward<E3>(e3))
    )
)

//------------------------------------------------------------------------------

/// 4-argument version of a helper function to @match that accepts arguments that
/// have been already preprocessed with @filter to convert regular variables into
/// @var_ref and constants into @value.
/// \note This version will be called from @match with a non-@view target type
template <typename T, size_t layout, typename E1, typename E2, typename E3, typename E4>
inline constr4<T,layout,E1,E2,E3,E4> match_ex(const view<T,layout>&, E1&& e1, E2&& e2, E3&& e3, E4&& e4) noexcept
{
    return constr4<T,layout,E1,E2,E3,E4>(
            std::forward<E1>(e1),
            std::forward<E2>(e2),
            std::forward<E3>(e3),
            std::forward<E4>(e4)
           );
}

/// 4-argument version of a helper function to @match that accepts arguments that
/// have been already preprocessed with @filter to convert regular variables into
/// @var_ref and constants into @value.
/// \note This version will be called from @match that had its target type a @view
template <typename T, size_t layout, typename E1, typename E2, typename E3, typename E4>
inline constr4<T,layout,E1,E2,E3,E4> match_ex(const view<view<T,layout>>&, E1&& e1, E2&& e2, E3&& e3, E4&& e4) noexcept
{
    return constr4<T,layout,E1,E2,E3,E4>(
            std::forward<E1>(e1),
            std::forward<E2>(e2),
            std::forward<E3>(e3),
            std::forward<E4>(e4)
           );
}

/// A 4-argument version of a tree-pattern constructor. Target type is allowed
/// to be a @view here.
template <typename T, typename E1, typename E2, typename E3, typename E4>
inline auto match(E1&& e1, E2&& e2, E3&& e3, E4&& e4) noexcept -> XTL_RETURN
(
    match_ex(
        view<T>(),
        filter(std::forward<E1>(e1)),
        filter(std::forward<E2>(e2)),
        filter(std::forward<E3>(e3)),
        filter(std::forward<E4>(e4))
    )
)

/// A 4-argument version of a tree-pattern constructor that takes layout in
/// addition to the target type.
/// \note @view is not supposed to be passed as a target type to this version
///       of the function because we will then have two potentially conflicting
///       layouts. Any layout different from @default_layout passed here will
///       result in a compile time error.
template <typename T, size_t layout, typename E1, typename E2, typename E3, typename E4>
inline auto match(E1&& e1, E2&& e2, E3&& e3, E4&& e4) noexcept -> XTL_RETURN
(
    match_ex(
        view<T,layout>(),
        filter(std::forward<E1>(e1)),
        filter(std::forward<E2>(e2)),
        filter(std::forward<E3>(e3)),
        filter(std::forward<E4>(e4))
    )
)

#include "solvers.hpp" // Include all our solvers by default for testing purposes. Remove eventually
