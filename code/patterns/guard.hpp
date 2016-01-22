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
/// This file defines guard pattern support by our library.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// \see https://parasol.tamu.edu/mach7/
/// \see https://github.com/solodon4/Mach7
/// \see https://github.com/solodon4/SELL
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

    constexpr guard(const P1&  p1, const E2&  e2) noexcept_when(std::is_nothrow_copy_constructible<P1>::value && std::is_nothrow_copy_constructible<E2>::value) : m_p1(          p1 ), m_e2(          e2 ) {}
    constexpr guard(      P1&& p1, const E2&  e2) noexcept_when(std::is_nothrow_move_constructible<P1>::value && std::is_nothrow_copy_constructible<E2>::value) : m_p1(std::move(p1)), m_e2(          e2 ) {}
    constexpr guard(const P1&  p1,       E2&& e2) noexcept_when(std::is_nothrow_copy_constructible<P1>::value && std::is_nothrow_move_constructible<E2>::value) : m_p1(          p1 ), m_e2(std::move(e2)) {}
    constexpr guard(      P1&& p1,       E2&& e2) noexcept_when(std::is_nothrow_move_constructible<P1>::value && std::is_nothrow_move_constructible<E2>::value) : m_p1(std::move(p1)), m_e2(std::move(e2)) {}

    constexpr guard(const guard&  e)              noexcept_when(std::is_nothrow_copy_constructible<P1>::value && std::is_nothrow_copy_constructible<E2>::value) : m_p1(          e.m_p1 ), m_e2(          e.m_e2 ) {} ///< Copy constructor
    constexpr guard(      guard&& e)              noexcept_when(std::is_nothrow_move_constructible<P1>::value && std::is_nothrow_move_constructible<E2>::value) : m_p1(std::move(e.m_p1)), m_e2(std::move(e.m_e2)) {} ///< Move constructor

    guard& operator=(const guard&) XTL_DELETED; ///< Assignment is not allowed for this class

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
