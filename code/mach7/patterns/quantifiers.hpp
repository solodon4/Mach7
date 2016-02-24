//
//  Mach7: Pattern Matching Library for C++
//
//  Copyright 2011-2013, Texas A&M University.
//  Copyright 2014 Yuriy Solodkyy.
//  All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//
//      * Redistributions of source code must retain the above copyright
//        notice, this list of conditions and the following disclaimer.
//
//      * Redistributions in binary form must reproduce the above copyright
//        notice, this list of conditions and the following disclaimer in the
//        documentation and/or other materials provided with the distribution.
//
//      * Neither the names of Mach7 project nor the names of its contributors
//        may be used to endorse or promote products derived from this software
//        without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
//  IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY
//  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
//  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

///
/// \file
///
/// This file defines existential patterns supported by our library.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// \see https://parasol.tamu.edu/mach7/
/// \see https://github.com/solodon4/Mach7
/// \see https://github.com/solodon4/SELL
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

    constexpr explicit existential(const P1&  p)          noexcept_when(std::is_nothrow_copy_constructible<P1>::value) : m_p1(          p ) {}
    constexpr explicit existential(      P1&& p)          noexcept_when(std::is_nothrow_move_constructible<P1>::value) : m_p1(std::move(p)) {}
    constexpr          existential(const existential&  e) noexcept_when(std::is_nothrow_copy_constructible<P1>::value) : m_p1(          e.m_p1 ) {} ///< Copy constructor
    constexpr          existential(      existential&& e) noexcept_when(std::is_nothrow_move_constructible<P1>::value) : m_p1(std::move(e.m_p1)) {} ///< Move constructor
    existential& operator=(const existential&) XTL_DELETED; ///< Assignment is not allowed for this class

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

    constexpr explicit universal(const P1&  p1)       noexcept_when(std::is_nothrow_copy_constructible<P1>::value) : m_p1(          p1 ) {}
    constexpr explicit universal(      P1&& p1)       noexcept_when(std::is_nothrow_move_constructible<P1>::value) : m_p1(std::move(p1)) {}
    constexpr          universal(const universal&  e) noexcept_when(std::is_nothrow_copy_constructible<P1>::value) : m_p1(          e.m_p1 ) {} ///< Copy constructor
    constexpr          universal(      universal&& e) noexcept_when(std::is_nothrow_move_constructible<P1>::value) : m_p1(std::move(e.m_p1)) {} ///< Move constructor
    universal& operator=(const universal&) XTL_DELETED; ///< Assignment is not allowed for this class

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
