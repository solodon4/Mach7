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

#include "common.hpp"

namespace mch ///< Mach7 library namespace
{

//------------------------------------------------------------------------------

template <typename E1>
struct equivalence
{
    static_assert(is_expression<E1>::value, "Argument E of an equivalence pattern must be a lazy expression");

    explicit equivalence(const E1& e1) : m_e1(e1) {}
    explicit equivalence(E1&& e1) noexcept : m_e1(std::move(e1)) {}
    equivalence(equivalence&& e) noexcept : m_e1(std::move(e.m_e1)) {}

    typedef typename E1::result_type result_type;
    typedef result_type accepted_type; ///< Type accepted by the pattern. Requirement of #Pattern concept

    operator result_type() const { return eval(m_e1); } // FIX: avoid implicit conversion in lazy expressions

    bool operator()(const accepted_type& u) const { return u == eval(m_e1); }

    E1 m_e1;
};

//------------------------------------------------------------------------------

/// #is_pattern_ is a helper meta-predicate capable of distinguishing all our patterns
template <typename E1> struct is_pattern_<equivalence<E1>>    { enum { value = true }; };

/// #is_expression_ is a helper meta-predicate that separates lazily evaluatable expressions we support
template <typename E1> struct is_expression_<equivalence<E1>> { enum { value = true }; };

//------------------------------------------------------------------------------

} // of namespace mch

//------------------------------------------------------------------------------

/// \note This operator has to be in the global namespace as E is unrestricted
///       and ADL won't work to find it!
template <typename E>
inline auto operator+(E&& e) throw() 
        -> typename std::enable_if<mch::is_expression<E>::value, mch::equivalence<typename mch::underlying<E>::type>>::type 
{
    return mch::equivalence<typename mch::underlying<E>::type>(std::forward<E>(e));
}

//------------------------------------------------------------------------------
