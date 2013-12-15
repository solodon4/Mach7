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
    wildcard() {}
    /// Type function returning a type that will be accepted by the pattern for
    /// a given subject type S. We use type function instead of an associated 
    /// type, because there is no a single accepted type for a #wildcard pattern
    /// for example. Requirement of #Pattern concept.
    template <typename S> struct accepted_type_for { typedef S type; };

    // NOTE: We don't need the below application anymore since we have a
    //       specialization that never applies the actual member before
    //       passing it to this meta variable that matches everything.
    // NOTE: We add it anyways for unlikely cases when user uses wildcard in the
    //       left hand side of a guard
#if defined(__GNUC__)
    template <typename T>
    bool operator()(const T&) const noexcept { return true; }
#else
    bool operator()(...) const noexcept { return true; }
#endif
};

//------------------------------------------------------------------------------

/// Declare a static instance of wildcard pattern with commonly used name _ 
static wildcard _;

//------------------------------------------------------------------------------

/// #is_pattern_ is a helper meta-predicate capable of distinguishing all our patterns
template <> struct is_pattern_<wildcard> { static const bool value = true; };

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
    /// Type function returning a type that will be accepted by the pattern for
    /// a given subject type S. We use type function instead of an associated 
    /// type, because there is no a single accepted type for a #wildcard pattern
    /// for example. Requirement of #Pattern concept.
    template <typename S> struct accepted_type_for { typedef T type; };

    typedef T result_type;   ///< Type of result when used in expression. Requirement of #LazyExpression concept
    explicit value(const T& t) : m_value(t) {}
    explicit value(T&& t) noexcept : m_value(std::move(t)) {}
    value(value&& v) noexcept : m_value(std::move(v.m_value)) {}
    bool operator()(const T& t) const noexcept { return m_value == t; }
    operator const result_type&() const noexcept { return m_value; }// FIX: avoid implicit conversion in lazy expressions
    T m_value;
};

//------------------------------------------------------------------------------

// GCC before 4.6 doesn't support nullptr and nullptr_t, while many will live 
// without this specialization
#if !defined(__GNUC__) || (XTL_GCC_VERSION >= 40600)

/// Specialization of the value pattern to be able to compare with nullptr
template <>
struct value<std::nullptr_t>
{
    /// Type function returning a type that will be accepted by the pattern for
    /// a given subject type S. We use type function instead of an associated 
    /// type, because there is no a single accepted type for a #wildcard pattern
    /// for example. Requirement of #Pattern concept.
    template <typename S> struct accepted_type_for; // Intentionally no definition
    template <typename S> struct accepted_type_for<S*> { typedef const S* type; };

    typedef std::nullptr_t result_type;   ///< Type of result when used in expression. Requirement of #LazyExpression concept
    explicit value(const std::nullptr_t&) {}
    bool operator()(const void* p) const noexcept { return p == nullptr; }
    operator result_type() const noexcept { return nullptr; }// FIX: avoid implicit conversion in lazy expressions
};

#endif

//------------------------------------------------------------------------------

/// #is_pattern_ is a helper meta-predicate capable of distinguishing all our patterns
template <typename T> struct is_pattern_<value<T>>    { static const bool value = true; };

/// #is_expression_ is a helper meta-predicate that separates lazily evaluatable expressions we support
template <typename T> struct is_expression_<value<T>> { static const bool value = true; };

//------------------------------------------------------------------------------

/// Convenience function for creating value patterns
template <class T> inline value<T> val(T&& t) { return value<T>(std::forward<T>(t)); }

//------------------------------------------------------------------------------

/// Variable binding for a value type
template <class T>
struct var
{
    var() : m_value() {}
    /*explicit*/ var(const T& t) : m_value(t) {}
    /*explicit*/ var(T&& t) noexcept : m_value(std::move(t)) {}
    var(var&& v) noexcept : m_value(std::move(v.m_value)) {}

    /// Type function returning a type that will be accepted by the pattern for
    /// a given subject type S. We use type function instead of an associated 
    /// type, because there is no a single accepted type for a #wildcard pattern
    /// for example. Requirement of #Pattern concept.
    template <typename S> struct accepted_type_for { typedef T type; };

    typedef T result_type;   ///< Type of result when used in expression. Requirement of #LazyExpression concept

    /// We report that matching succeeded and bind the value
    bool operator()(const T& t) const
    {
        m_value = t;
        return true;
    }

    var& operator=(const T& t) { m_value = t; return *this; }

    /// Helper conversion operator to let the variable be used in some places
    /// where T was allowed
    operator const result_type&() const noexcept { return m_value; }

    /// Member that will hold matching value in case of successful matching
    mutable T m_value;
};

//------------------------------------------------------------------------------

/// Variable binding for a pointer type
template <class T>
struct var<T&>
{
    var() : m_value() {}
    var(var&& v) noexcept : m_value(std::move(v.m_value)) {}

    /// Type function returning a type that will be accepted by the pattern for
    /// a given subject type S. We use type function instead of an associated 
    /// type, because there is no a single accepted type for a #wildcard pattern
    /// for example. Requirement of #Pattern concept.
    template <typename S> struct accepted_type_for { typedef T type; };

    typedef typename std::remove_const<T>::type result_type;   ///< Type of result when used in expression. Requirement of #LazyExpression concept

    /// We may be applied to a value of a base type, so first we have to figure
    /// out whether they dynamic type is actually T. We report match only if it is
    template <typename U>
    bool operator()(const U& u) const
    {
        if (const T* t = dynamic_cast<const T*>(&u))
            return operator()(*t);
        else
            return false;
    }

    /// This distinguishes the case when type of the variable matches type of the member
    bool operator()(const result_type& t) const
    {
        // NOTE: This will also assign the null pointer. Should it?
        m_value = &t;
        return true;
    }

    /// This distinguishes the case when type of the variable matches type of the member
    bool operator()(result_type& t) const
    {
        // NOTE: This will also assign the null pointer. Should it?
        m_value = &t;
        return true;
    }

    /// Helper conversion operator to let the variable be used in some places
    /// where T was allowed
    /// \note If you get assertion here, it means you are trying to use the 
    ///       value of this reference variable before it was bound (i.e. used 
    ///       in pattern matching context to get its value).
    operator result_type&() const noexcept { XTL_ASSERT(m_value); return *m_value; }

    /// Member that will hold matching value in case of successful matching
    mutable T* m_value;
};

//------------------------------------------------------------------------------

/// #is_var is a helper meta-predicate that is true only when P=var<T>
/// We use it to check that patterns that take other patterns as arguments do 
/// not accidentally host var<> directly as it will create a different var object.
/// Every var<> pattern must be wrapped into ref<> with the help of #filter()!
template <typename P> struct is_var         { static const bool value = false; };
template <typename T> struct is_var<var<T>> { static const bool value = true;  };

/// #is_pattern_ is a helper meta-predicate capable of distinguishing all our patterns
template <typename T> struct is_pattern_<var<T>>    { static const bool value = true; };

/// #is_expression_ is a helper meta-predicate that separates lazily evaluatable expressions we support
template <typename T> struct is_expression_<var<T>> { static const bool value = true; };

//------------------------------------------------------------------------------

/// A reference to a user provided variable
template <class T>
struct ref
{
    explicit ref(T& var) : m_var(var) {}
    ref(ref&& v) noexcept : m_var(v.m_var) {}
    ref& operator=(const ref&); ///< Assignment is not allowed for this class

    /// Type function returning a type that will be accepted by the pattern for
    /// a given subject type S. We use type function instead of an associated 
    /// type, because there is no a single accepted type for a #wildcard pattern
    /// for example. Requirement of #Pattern concept.
    template <typename S> struct accepted_type_for { typedef T type; };

    typedef T result_type;   ///< Type of result when used in expression. Requirement of #LazyExpression concept
    operator const result_type&() const noexcept { return m_var; } // FIX: avoid implicit conversion in lazy expressions
    //operator       result_type&() const noexcept { return m_var; }

    /// We report that matching succeeded and bind the value
    bool operator()(const T& t) const
    {
        m_var = t;
        return true;
    }

    /// Member that will hold matching value in case of successful matching
    T& m_var;
};

//------------------------------------------------------------------------------

/// A reference to a user provided variable
template <class T>
struct ref<var<T>>
{
    explicit ref(var<T>& var) : m_var(var) {}
    ref(const ref&  v) noexcept : m_var(v.m_var) {} // Copy constructor
    ref(      ref&& v) noexcept : m_var(v.m_var) {} // Move constructor
    ref& operator=(const ref&); ///< Assignment is not allowed for this class

    typedef typename var<T>::result_type result_type;   ///< Type of result when used in expression. Requirement of #LazyExpression concept

    /// Type function returning a type that will be accepted by the pattern for
    /// a given subject type S. We use type function instead of an associated 
    /// type, because there is no a single accepted type for a #wildcard pattern
    /// for example. Requirement of #Pattern concept.
    template <typename S> struct accepted_type_for { typedef result_type type; };

    operator const result_type&() const noexcept { return (const result_type&)m_var; } // FIX: avoid implicit conversion in lazy expressions
    //operator       result_type&() const noexcept { return       (result_type&)m_var; }

    /// We report that matching succeeded and bind the value
    bool operator()(const result_type& t) const { return m_var(t); }
    bool operator()(      result_type& t) const { return m_var(t); }

    /// Member that will hold matching value in case of successful matching
    var<T>& m_var;
};

//------------------------------------------------------------------------------

/// A reference to a user provided variable
template <class T>
struct ref<const var<T>>
{
    explicit ref(const var<T>& var) : m_var(var) {}
    ref(const ref&  v) noexcept : m_var(v.m_var) {} // Copy constructor
    ref(      ref&& v) noexcept : m_var(v.m_var) {} // Move constructor
    ref& operator=(const ref&); ///< Assignment is not allowed for this class

    typedef typename var<T>::result_type result_type;   ///< Type of result when used in expression. Requirement of #LazyExpression concept

    /// Type function returning a type that will be accepted by the pattern for
    /// a given subject type S. We use type function instead of an associated 
    /// type, because there is no a single accepted type for a #wildcard pattern
    /// for example. Requirement of #Pattern concept.
    template <typename S> struct accepted_type_for { typedef result_type type; };

    operator const result_type&() const noexcept { return (const result_type&)m_var; } // FIX: avoid implicit conversion in lazy expressions
    //operator       result_type&() const noexcept { return       (result_type&)m_var; }

    /// We report that matching succeeded and bind the value
    bool operator()(const result_type& t) const { return m_var(t); }
    bool operator()(      result_type& t) const { return m_var(t); }

    /// Member that will hold matching value in case of successful matching
    const var<T>& m_var;
};

//------------------------------------------------------------------------------

/// #is_pattern_ is a helper meta-predicate capable of distinguishing all our patterns
template <typename T> struct is_pattern_<ref<T>>    { static const bool value = true; };

/// #is_expression_ is a helper meta-predicate that separates lazily evaluatable expressions we support
template <typename T> struct is_expression_<ref<T>> { static const bool value = true; };

//------------------------------------------------------------------------------

/// Convenience function for creating variable patterns out of existing variables
template <class T> inline ref<T> var_from(T& t) { return ref<T>(t); }

//------------------------------------------------------------------------------

template <typename T> inline                                        ref<const var<T>>       filter(const var<T>& t) noexcept { return ref<const var<T>>(t); } // We need this because some patterns may not need to modify the variable: e.g. equivalence
template <typename T> inline                                        ref<      var<T>>       filter(      var<T>& t) noexcept { return ref<      var<T>>(t); } // This one is needed for patterns that will modify the variable.
template <typename T> inline typename std::enable_if<!is_pattern<T>::value, value<T>>::type filter(const T& t) noexcept { return value<T>(t); }
template <typename T> inline typename std::enable_if<!is_pattern<T>::value,   ref<T>>::type filter(      T& t) noexcept { return ref<T>(t); }

//------------------------------------------------------------------------------

///@{
/// Set of overloads capable of decomposing an expression template that models
/// an Expression concept and evaluating it.
/// \note See header files of other patterns for more overloads!
template <typename T> inline const T& eval(const value<T>&           e) { return e.m_value; }
template <typename T> inline const T& eval(const var<T >&            e) { return e.m_value; }
template <typename T> inline       T& eval(      var<T >&            e) { return e.m_value; }
template <typename T> inline const T& eval(const var<T&>&            e) { XTL_ASSERT(e.m_value); return *e.m_value; } ///< \note If you get assertion here, it means you are trying to use the value of this reference variable before it was bound (i.e. used in pattern matching context to get its value).
template <typename T> inline       T& eval(      var<T&>&            e) { XTL_ASSERT(e.m_value); return *e.m_value; } ///< \note If you get assertion here, it means you are trying to use the value of this reference variable before it was bound (i.e. used in pattern matching context to get its value).
template <typename T> inline const T& eval(const ref<          T >&  e) { return e.m_var; }
template <typename T> inline       T& eval(      ref<          T >&  e) { return e.m_var; }
template <typename T> inline     auto eval(const ref<      var<T>>&  e) -> decltype(eval(e.m_var)) { return eval(e.m_var); }
template <typename T> inline     auto eval(      ref<      var<T>>&  e) -> decltype(eval(e.m_var)) { return eval(e.m_var); }
template <typename T> inline     auto eval(const ref<const var<T>>&  e) -> decltype(eval(e.m_var)) { return eval(e.m_var); }
template <typename T> inline     auto eval(      ref<const var<T>>&  e) -> decltype(eval(e.m_var)) { return eval(e.m_var); }
///@}

//------------------------------------------------------------------------------

} // of namespace mch
