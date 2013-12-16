///
/// \file quantifiers.hpp
///
/// This file defines existential patterns supported by our library.
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

template <typename P1>
struct existential
{
    static_assert(is_pattern<P1>::value, "Argument P1 of an existential quantifier pattern must be a pattern");

    explicit existential(const P1&  p) noexcept : m_p1(          p ) {}
    explicit existential(      P1&& p) noexcept : m_p1(std::move(p)) {}
    existential(const existential&  e) noexcept : m_p1(          e.m_p1 ) {} ///< Copy constructor
    existential(      existential&& e) noexcept : m_p1(std::move(e.m_p1)) {} ///< Move constructor
    existential& operator=(const existential&); ///< Assignment is not allowed for this class

    /// Type function returning a type that will be accepted by the pattern for
    /// a given subject type S. We use type function instead of an associated 
    /// type, because there is no a single accepted type for a #wildcard pattern
    /// for example. Requirement of #Pattern concept.
    template <typename S> struct accepted_type_for { typedef S type; };

    template <typename C>
    bool operator()(const C& c) const 
    {
        for (typename C::const_iterator p = c.begin(); p != c.end(); ++p)
            if (m_p1(*p))
                return true;
        return false; 
    }

    P1 m_p1;
};

//------------------------------------------------------------------------------

/// #is_pattern_ is a helper meta-predicate capable of distinguishing all our patterns
template <typename P1> struct is_pattern_<existential<P1>> { static const bool value = true; };

//------------------------------------------------------------------------------

template <typename P1>
inline auto exist(P1&& p1) noexcept 
        -> existential<
                typename underlying<decltype(filter(std::forward<P1>(p1)))>::type
           >
{
    return existential<
                typename underlying<decltype(filter(std::forward<P1>(p1)))>::type
           >(
                filter(std::forward<P1>(p1))
            );
}

//------------------------------------------------------------------------------

template <typename P1>
struct universal
{
    static_assert(is_pattern<P1>::value, "Argument P1 of an universal quantifier pattern must be a pattern");

    explicit universal(const P1&  p1) noexcept : m_p1(          p1 ) {}
    explicit universal(      P1&& p1) noexcept : m_p1(std::move(p1)) {}
    universal(const universal&  e) noexcept : m_p1(          e.m_p1 ) {} ///< Copy constructor
    universal(      universal&& e) noexcept : m_p1(std::move(e.m_p1)) {} ///< Move constructor
    universal& operator=(const universal&); ///< Assignment is not allowed for this class

    /// Type function returning a type that will be accepted by the pattern for
    /// a given subject type S. We use type function instead of an associated 
    /// type, because there is no a single accepted type for a #wildcard pattern
    /// for example. Requirement of #Pattern concept.
    template <typename S> struct accepted_type_for { typedef S type; };

    template <typename C>
    bool operator()(const C& c) const 
    {
        for (typename C::const_iterator p = c.begin(); p != c.end(); ++p)
            if (!m_p1(*p))
                return false;
        return true; 
    }

    P1 m_p1;
};

//------------------------------------------------------------------------------

/// #is_pattern_ is a helper meta-predicate capable of distinguishing all our patterns
template <typename P1> struct is_pattern_<universal<P1>> { static const bool value = true; };

//------------------------------------------------------------------------------

template <typename P1>
inline auto all(P1&& p1) noexcept
        -> universal<
                typename underlying<decltype(filter(std::forward<P1>(p1)))>::type
           >
{
    return universal<
                typename underlying<decltype(filter(std::forward<P1>(p1)))>::type
           >(
                filter(std::forward<P1>(p1))
            );
}

//------------------------------------------------------------------------------

} // of namespace mch

//------------------------------------------------------------------------------
