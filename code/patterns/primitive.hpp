///
/// \file primitive.hpp
///
/// This file defines primitive patterns supported by our library.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of Mach7 library (http://parasol.tamu.edu/mach7/).
/// Copyright (C) 2011-2012 Texas A&M University.
/// All rights reserved.
///

#pragma once

#include "common.hpp"

namespace mch ///< Mach7 library namespace
{

//------------------------------------------------------------------------------

/// Class that describes a meta variable that matches everything.
/// This variable will match any type returned by a member, but the main
/// difference from just using a variable whose value is ignored is that
/// use of this variable will make sure the actual member is never invoked!
struct wildcard
{
    // NOTE: We don't need the below application anymore since we have a
    //       specialization that never applies the actual member before
    //       passing it to this meta variable that matches everything.
    // NOTE: We add it anyways for unlikely cases when user uses wildcard in the
    //       left hand side of a guard
    bool operator()(...) const noexcept { return true; }
};

//------------------------------------------------------------------------------

/// #is_pattern_ is a helper meta-predicate capable of distinguishing all our patterns
template <> struct is_pattern_<wildcard> { enum { value = true }; };

//------------------------------------------------------------------------------

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

/// Value pattern
template <class T>
struct value
{
    typedef T accepted_type; ///< Type accepted by the pattern. Requirement of #Pattern concept
    typedef T result_type;   ///< Type of result when used in expression. Requirement of #LazyExpression concept
    explicit value(const T& t) : m_value(t) {}
    explicit value(T&& t) noexcept : m_value(std::move(t)) {}
    value(value&& v) noexcept : m_value(std::move(v.m_value)) {}
    bool operator()(const T& t) const noexcept { return m_value == t; }
    operator result_type() const noexcept { return m_value; }// FIX: avoid implicit conversion in lazy expressions
    T m_value;
};

//------------------------------------------------------------------------------

/// #is_pattern_ is a helper meta-predicate capable of distinguishing all our patterns
template <typename T> struct is_pattern_<value<T>>    { enum { value = true }; };

/// #is_expression_ is a helper meta-predicate that separates lazily evaluatable expressions we support
template <typename T> struct is_expression_<value<T>> { enum { value = true }; };

//------------------------------------------------------------------------------

/// Convenience function for creating value patterns
template <class T> inline value<T> val(T&& t) { return value<T>(std::forward<T>(t)); }

//------------------------------------------------------------------------------

/// Variable binding for a value type
template <class T>
struct variable
{
    variable() : m_value() {}
    /*explicit*/ variable(const T& t) : m_value(t) {}
    /*explicit*/ variable(T&& t) noexcept : m_value(std::move(t)) {}
    variable(variable&& v) noexcept : m_value(std::move(v.m_value)) {}

    typedef T accepted_type; ///< Type accepted by the pattern. Requirement of #Pattern concept
    typedef T result_type;   ///< Type of result when used in expression. Requirement of #LazyExpression concept

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

    typedef const T* accepted_type; ///< Type accepted by the pattern. Requirement of #Pattern concept
    typedef const T* result_type;   ///< Type of result when used in expression. Requirement of #LazyExpression concept

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

    typedef T accepted_type; ///< Type accepted by the pattern. Requirement of #Pattern concept
    typedef T result_type;   ///< Type of result when used in expression. Requirement of #LazyExpression concept

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

/// #is_pattern_ is a helper meta-predicate capable of distinguishing all our patterns
template <typename T> struct is_pattern_<variable<T>>    { enum { value = true }; };

/// #is_expression_ is a helper meta-predicate that separates lazily evaluatable expressions we support
template <typename T> struct is_expression_<variable<T>> { enum { value = true }; };

//------------------------------------------------------------------------------

/// A reference to a user provided variable
template <class T>
struct var_ref
{
    explicit var_ref(T& var) : m_var(&var) {}
    var_ref(var_ref&& v) noexcept : m_var(v.m_var) {}

    typedef T accepted_type; ///< Type accepted by the pattern. Requirement of #Pattern concept
    typedef T result_type;   ///< Type of result when used in expression. Requirement of #LazyExpression concept
    operator result_type() const noexcept { return *m_var; }// FIX: avoid implicit conversion in lazy expressions

    /// We report that matching succeeded and bind the value
    bool operator()(const T& t) const
    {
        *m_var = t;
        return true;
    }

    /// Member that will hold matching value in case of successful matching
    T* m_var;
};

//------------------------------------------------------------------------------

/// A reference to a user provided variable
template <class T>
struct var_ref<variable<T>>
{
    explicit var_ref(variable<T>& var) : m_var(&var) {}
    var_ref(var_ref&& v) noexcept : m_var(v.m_var) {}

    typedef T accepted_type; ///< Type accepted by the pattern. Requirement of #Pattern concept
    typedef T result_type;   ///< Type of result when used in expression. Requirement of #LazyExpression concept
    operator result_type() const noexcept { return *m_var; }// FIX: avoid implicit conversion in lazy expressions

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

/// #is_pattern_ is a helper meta-predicate capable of distinguishing all our patterns
template <typename T> struct is_pattern_<var_ref<T>>    { enum { value = true }; };

/// #is_expression_ is a helper meta-predicate that separates lazily evaluatable expressions we support
template <typename T> struct is_expression_<var_ref<T>> { enum { value = true }; };

//------------------------------------------------------------------------------

/// Convenience function for creating variable patterns out of existing variables
template <class T> inline var_ref<T> var(T& t) { return var_ref<T>(t); }

//------------------------------------------------------------------------------

template <typename T> inline                                             var_ref<variable<T>> filter(variable<T>& t) noexcept { return var_ref<variable<T>>(t); }
template <typename T> inline typename std::enable_if<!is_pattern<T>::value,   value<T>>::type filter(    const T& t) noexcept { return value<T>(t); }
template <typename T> inline typename std::enable_if<!is_pattern<T>::value, var_ref<T>>::type filter(          T& t) noexcept { return var_ref<T>(t); }
template <typename P> inline typename std::enable_if< is_pattern<P>::value, typename std::remove_reference<P>::type>::type filter(P&& p) noexcept { return std::move(p); }

//------------------------------------------------------------------------------

///@{
/// Set of overloads capable of decomposing an expression template that models
/// an Expression concept and evaluating it.
/// \note See header files of other patterns for more overloads!
template <typename T> T inline eval(const value<T>& e)             { return e.m_value; }
template <typename T> T inline eval(const variable<T>& e)          { return e.m_value; }
template <typename T> T inline eval(const var_ref<T>& e)           { return *e.m_var; }
template <typename T> T inline eval(const var_ref<variable<T>>& e) { return e.m_var->m_value; }
///@}

//------------------------------------------------------------------------------

} // of namespace mch
