///
/// \file guard.hpp
///
/// This file defines guard pattern support by our library.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of Mach7 library (http://parasol.tamu.edu/mach7/).
/// Copyright (C) 2011-2012 Texas A&M University.
/// All rights reserved.
///

#pragma once

#include "primitive.hpp"

namespace mch ///< Mach7 library namespace
{

//------------------------------------------------------------------------------

/// Guard pattern
template <typename P1, typename E2>
struct guard
{
    static_assert(is_pattern<P1>::value,    "Argument P1 of a guard-pattern must be a pattern");
    static_assert(is_expression<E2>::value, "Argument E2 of a guard-pattern must be a lazy expression");

    guard(const P1&  p1, const E2&  e2) noexcept : m_p1(p1), m_e2(e2) {}
    guard(const P1&  p1,       E2&& e2) noexcept : m_p1(p1), m_e2(std::move(e2)) {}
    guard(      P1&& p1, const E2&  e2) noexcept : m_p1(std::move(p1)), m_e2(e2) {}
    guard(      P1&& p1,       E2&& e2) noexcept : m_p1(std::move(p1)), m_e2(std::move(e2)) {}
    guard(const guard&  e) noexcept : m_p1(          e.m_p1 ), m_e2(          e.m_e2 ) {} ///< Copy constructor
    guard(      guard&& e) noexcept : m_p1(std::move(e.m_p1)), m_e2(std::move(e.m_e2)) {} ///< Move constructor
    guard& operator=(const guard&); ///< Assignment is not allowed for this class

    /// Type function returning a type that will be accepted by the pattern for
    /// a given subject type S. We use type function instead of an associated 
    /// type, because there is no a single accepted type for a #wildcard pattern
    /// for example. Requirement of #Pattern concept.
    template <typename S> struct accepted_type_for : P1::template accepted_type_for<S> {};

    template <typename U>
    bool operator()(const U& u) const { return m_p1(u) && eval(m_e2); }

    P1 m_p1;
    E2 m_e2;
};

//------------------------------------------------------------------------------

/// #is_pattern_ is a helper meta-predicate capable of distinguishing all our patterns
template <typename P1, typename E2> struct is_pattern_<guard<P1,E2>> { static const bool value = true; };

//------------------------------------------------------------------------------

} // of namespace mch

//------------------------------------------------------------------------------

template <typename P, typename E>
auto operator|=(P&& p, E&& e) noexcept -> XTL_RETURN_ENABLE_IF
(
    mch::is_pattern<P>::value && mch::is_expression<E>::value,
    mch::guard<
        typename mch::underlying<decltype(mch::filter(std::forward<P>(p)))>::type,
        typename mch::underlying<decltype(mch::filter(std::forward<E>(e)))>::type
    >(
        mch::filter(std::forward<P>(p)),
        mch::filter(std::forward<E>(e))
    )
)

//------------------------------------------------------------------------------
