///
/// \file equivalence.hpp
///
/// This file defines equivalence patterns supported by our library.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of Mach7 library (http://parasol.tamu.edu/mach7/).
/// Copyright (C) 2011-2012 Texas A&M University.
/// All rights reserved.
///

#pragma once

#include "primitive.hpp" // FIX: Ideally this should be common.hpp, but GCC seem to disagree: http://gcc.gnu.org/bugzilla/show_bug.cgi?id=55460

namespace mch ///< Mach7 library namespace
{

//------------------------------------------------------------------------------

template <typename E1>
struct equivalence
{
    static_assert(is_expression<E1>::value, "Argument E of an equivalence pattern must be a lazy expression");
    static_assert(!is_var<E1>::value,       "Attempting to host var<> directly. Use filter() to wrap it into ref2<>");

    explicit equivalence(const E1&  e) noexcept : m_e1(          e ) {}
    explicit equivalence(      E1&& e) noexcept : m_e1(std::move(e)) {}
    equivalence(const equivalence&  e) noexcept : m_e1(          e.m_e1 ) {} ///< Copy constructor    
    equivalence(      equivalence&& e) noexcept : m_e1(std::move(e.m_e1)) {} ///< Move constructor
    equivalence& operator=(const equivalence&); ///< Assignment is not allowed for this class

    typedef typename E1::result_type result_type;   ///< Type of result when used in expression. Requirement of #LazyExpression concept

    /// Type function returning a type that will be accepted by the pattern for
    /// a given subject type S. We use type function instead of an associated 
    /// type, because there is no a single accepted type for a #wildcard pattern
    /// for example. Requirement of #Pattern concept.
    template <typename S> struct accepted_type_for { typedef result_type type; };

    operator result_type() const { return eval(m_e1); } // FIX: avoid implicit conversion in lazy expressions

    bool operator()(const result_type& u) const { return u == eval(m_e1); }
//    bool operator()(      result_type& u) const { return u == eval(m_e1); }

    E1 m_e1;
};

//------------------------------------------------------------------------------

/// #is_pattern_ is a helper meta-predicate capable of distinguishing all our patterns
template <typename E1> struct is_pattern_<equivalence<E1>>    { static const bool value = true; };

/// #is_expression_ is a helper meta-predicate that separates lazily evaluatable expressions we support
template <typename E1> struct is_expression_<equivalence<E1>> { static const bool value = true; };

//------------------------------------------------------------------------------

} // of namespace mch

//------------------------------------------------------------------------------

/// \note This operator has to be in the global namespace as E is unrestricted
///       and ADL won't work to find it!
template <typename E1>
inline auto operator+(E1&& e1) noexcept
        -> typename std::enable_if<
                        mch::is_expression<E1>::value,
                        mch::equivalence<
                            typename mch::underlying<decltype(mch::filter(std::forward<E1>(e1)))>::type
                        >
                    >::type
{
    return mch::equivalence<
                            typename mch::underlying<decltype(mch::filter(std::forward<E1>(e1)))>::type
                        >(
                            mch::filter(std::forward<E1>(e1))
                         );
}

//------------------------------------------------------------------------------
