///
/// \file guard.hpp
///
/// This file defines guard pattern support by our library.
///
/// \autor Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of the XTL framework (http://parasol.tamu.edu/xtl/).
/// Copyright (C) 2005-2012 Texas A&M University.
/// All rights reserved.
///

#pragma once

#include "primitive.hpp"

//------------------------------------------------------------------------------

/// Guard pattern
template <typename E1, typename E2>
struct guard
{
    static_assert(is_pattern<E1>::value,    "Argument E1 of a guard-pattern must be a pattern");
    static_assert(is_expression<E2>::value, "Argument E2 of a guard-pattern must be a lazy expression");

    guard(const E1&  e1, const E2&  e2) noexcept : m_e1(e1), m_e2(e2) {}
    guard(const E1&  e1,       E2&& e2) noexcept : m_e1(e1), m_e2(std::move(e2)) {}
    guard(      E1&& e1, const E2&  e2) noexcept : m_e1(std::move(e1)), m_e2(e2) {}
    guard(      E1&& e1,       E2&& e2) noexcept : m_e1(std::move(e1)), m_e2(std::move(e2)) {}
    guard(guard&& e) noexcept : m_e1(std::move(e.m_e1)), m_e2(std::move(e.m_e2)) {}

    typedef typename E1::result_type result_type;

    template <typename U>
    bool operator()(const U& u) const { return m_e1(u) && eval(m_e2); }

    E1 m_e1;
    E2 m_e2;
};

//------------------------------------------------------------------------------

template <typename E1, typename E2>
    auto make_guard(E1&& e1, E2&& e2) noexcept -> XTL_RETURN
    (
        guard<typename underlying<E1>::type,typename underlying<E2>::type>(std::forward<E1>(e1),std::forward<E2>(e2))
    )

//------------------------------------------------------------------------------

template <typename E, typename T>
auto operator|=(variable<T>& v, E&& e) noexcept -> XTL_RETURN_ENABLE_IF
(
    is_expression<E>::value,
    make_guard(var_ref<variable<T>>(v),filter(std::forward<E>(e)))
)

template <typename P, typename E>
auto operator|=(P&& p, E&& e) noexcept -> XTL_RETURN_ENABLE_IF
(
    is_pattern<P>::value && is_expression<E>::value,
    make_guard(std::forward<P>(p),filter(std::forward<E>(e)))
)

//------------------------------------------------------------------------------

/// @is_pattern_ is a helper meta-predicate capable of distinguishing all our patterns
template <typename E1, typename E2> struct is_pattern_<guard<E1,E2>> { enum { value = true }; };
