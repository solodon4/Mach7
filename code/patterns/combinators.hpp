///
/// \file combinators.hpp
///
/// This file defines pattern combinators supported by our library.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of Mach7 library (http://parasol.tamu.edu/mach7/).
/// Copyright (C) 2011-2012 Texas A&M University.
/// All rights reserved.
///

#pragma once

#include "common.hpp"

//------------------------------------------------------------------------------

/// Conjunction pattern combinator
template <typename P1, typename P2>
struct conjunction
{
    static_assert(is_pattern<P1>::value, "Argument P1 of a conjunction-pattern must be a pattern");
    static_assert(is_pattern<P2>::value, "Argument P2 of a conjunction-pattern must be a pattern");
    static_assert(std::is_same<
                      typename P1::accepted_type,
                      typename P2::accepted_type
                  >::value,"The type accepted by patterns in conjunction must be the same");

    conjunction(const P1& p1, const P2& p2) : m_p1(p1), m_p2(p2) {}
    conjunction(P1&& p1, P2&& p2) noexcept : m_p1(std::move(p1)), m_p2(std::move(p2)) {}
    conjunction(conjunction&& c)  noexcept : m_p1(std::move(c.m_p1)), m_p2(std::move(c.m_p2)) {}

    typedef typename P1::accepted_type accepted_type; ///< Type accepted by the pattern. Requirement of #Pattern concept

    bool operator()(const accepted_type& subject) const { return m_p1(subject) && m_p2(subject); }

    P1 m_p1; ///< The 1st pattern in conjunction
    P2 m_p2; ///< The 2nd pattern in conjunction
};

/// #is_pattern_ is a helper meta-predicate capable of distinguishing all our patterns
template <typename P1, typename P2> struct is_pattern_<conjunction<P1,P2>> { enum { value = true /*is_pattern<P1>::value && is_pattern<P2>::value*/ }; };

//------------------------------------------------------------------------------

/// Disjunction pattern combinator
template <typename P1, typename P2>
struct disjunction
{
    static_assert(is_pattern<P1>::value, "Argument P1 of a disjunction-pattern must be a pattern");
    static_assert(is_pattern<P2>::value, "Argument P2 of a disjunction-pattern must be a pattern");
    static_assert(std::is_same<
                      typename P1::accepted_type,
                      typename P2::accepted_type
                  >::value,"The type accepted by patterns in disjunction must be the same");

    disjunction(const P1& p1, const P2& p2) : m_p1(p1), m_p2(p2) {}
    disjunction(P1&& p1, P2&& p2) noexcept : m_p1(std::move(p1)), m_p2(std::move(p2)) {}
    disjunction(disjunction&& c)  noexcept : m_p1(std::move(c.m_p1)), m_p2(std::move(c.m_p2)) {}

    typedef typename P1::accepted_type accepted_type; ///< Type accepted by the pattern. Requirement of #Pattern concept

    bool operator()(const accepted_type& subject) const { return m_p1(subject) || m_p2(subject); }

    P1 m_p1; ///< The 1st pattern of disjunction combinator
    P2 m_p2; ///< The 2nd pattern of disjunction combinator
};


/// #is_pattern_ is a helper meta-predicate capable of distinguishing all our patterns
template <typename P1, typename P2> struct is_pattern_<disjunction<P1,P2>> { enum { value = true /*is_pattern<P1>::value && is_pattern<P2>::value*/ }; };

//------------------------------------------------------------------------------

/// Negation pattern combinator
template <typename P1>
struct negation
{
    static_assert(is_pattern<P1>::value, "Argument P1 of a negation-pattern must be a pattern");

    negation(const P1& p1) : m_p1(p1) {}
    negation(P1&& p1) noexcept : m_p1(std::move(p1)) {}
    negation(negation&& c)  noexcept : m_p1(std::move(c.m_p1)) {}

    typedef typename P1::accepted_type accepted_type; ///< Type accepted by the pattern. Requirement of #Pattern concept

    bool operator()(const accepted_type& subject) const { return !m_p1(subject); }

    P1 m_p1; ///< The argument pattern of negation combinator
};

/// #is_pattern_ is a helper meta-predicate capable of distinguishing all our patterns
template <typename P1> struct is_pattern_<negation<P1>> { enum { value = true /*is_pattern<P1>::value*/ }; };

//------------------------------------------------------------------------------

template <typename P1, typename P2>
inline auto operator&&(P1&& p1, P2&& p2) noexcept 
        -> typename std::enable_if<is_pattern<P1>::value && 
                                   is_pattern<P2>::value, 
                                   conjunction<typename underlying<P1>::type,
                                               typename underlying<P2>::type>
                                  >::type 
{
    return conjunction<
                typename underlying<P1>::type,
                typename underlying<P2>::type
           >(
                std::forward<P1>(p1),
                std::forward<P2>(p2)
            );
}

//------------------------------------------------------------------------------

template <typename P1, typename P2>
inline auto operator||(P1&& p1, P2&& p2) noexcept 
        -> typename std::enable_if<is_pattern<P1>::value && 
                                   is_pattern<P2>::value, 
                                   disjunction<typename underlying<P1>::type,
                                               typename underlying<P2>::type>
                                  >::type 
{
    return disjunction<
                typename underlying<P1>::type,
                typename underlying<P2>::type
           >(
                std::forward<P1>(p1),
                std::forward<P2>(p2)
            );
}

//------------------------------------------------------------------------------

template <typename P1>
inline auto operator!(P1&& p) noexcept 
        -> typename std::enable_if<is_pattern<P1>::value, negation<typename underlying<P1>::type>>::type 
{
    return negation<typename underlying<P1>::type>(std::forward<P1>(p));
}

//------------------------------------------------------------------------------
