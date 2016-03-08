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
/// This file defines address patterns supported by our library.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// \see https://parasol.tamu.edu/mach7/
/// \see https://github.com/solodon4/Mach7
/// \see https://github.com/solodon4/SELL
///

#pragma once

#include <xtl/xtl.hpp>   // XTL subtyping definitions
#include "primitive.hpp" // FIX: Ideally this should be common.hpp, but GCC seem to disagree: http://gcc.gnu.org/bugzilla/show_bug.cgi?id=55460

namespace mch ///< Mach7 library namespace
{

//------------------------------------------------------------------------------

template <typename P1>
struct address
{
    static_assert(is_pattern<P1>::value, "Argument P1 of an address pattern must be a pattern");
    static_assert(!is_var<P1>::value,    "Attempting to host var<> directly. Use filter() to wrap it into ref2<>");

    constexpr explicit address(const P1&       p) noexcept_when(std::is_nothrow_copy_constructible<P1>::value) : m_p1(          p ) {}
    constexpr explicit address(      P1&&      p) noexcept_when(std::is_nothrow_move_constructible<P1>::value) : m_p1(std::move(p)) {}
    constexpr          address(const address&  e) noexcept_when(std::is_nothrow_copy_constructible<P1>::value) : m_p1(          e.m_p1 ) {} ///< Copy constructor
    constexpr          address(      address&& e) noexcept_when(std::is_nothrow_move_constructible<P1>::value) : m_p1(std::move(e.m_p1)) {} ///< Move constructor

    address& operator=(const address&) XTL_DELETED; ///< Assignment is not allowed for this class

    /// Type function returning a type that will be accepted by the pattern for
    /// a given subject type S. We use type function instead of an associated 
    /// type, because there is no a single accepted type for a #wildcard pattern
    /// for example. Requirement of #Pattern concept.
    template <typename S> struct accepted_type_for; // Intentionally no definition
    template <typename S> struct accepted_type_for<S*> { typedef typename P1::template accepted_type_for<S>::type* type; };

    // Address pattern only accepts pointer-like types. We don't use enable_if on this function because there is no
    // really other overloads to pick from, so we instead assert the relationship under which this method is enabled
    // in order to generate a more meaningful error message to the user.
    template <typename T>
    bool operator()(T&& t) const
    {
        static_assert(xtl::is_subtype<typename underlying<T>::type, const void*>::value,
                      "The accepted type of an address pattern combinator should be a pointer-like type. "
                      "Establish an XTL subtyping relation with void* for your smart pointers. "
                      "Use std::unique_ptr from mach7/adapters/std/memory.hpp as an example.");
        return t && m_p1(*std::forward<T>(t));
    }

    P1 m_p1;
};

//------------------------------------------------------------------------------

/// #is_pattern_ is a helper meta-predicate capable of distinguishing all our patterns
template <typename P1> struct is_pattern_<address<P1>> { static const bool value = true; };

//------------------------------------------------------------------------------

template <typename P1>
struct deref
{
    static_assert(is_pattern<P1>::value, "Argument P1 of an deref pattern must be a pattern");
    static_assert(!is_var<P1>::value,    "Attempting to host var<> directly. Use filter() to wrap it into ref2<>");

    constexpr explicit deref(const P1&     p) noexcept_when(std::is_nothrow_copy_constructible<P1>::value) : m_p1(          p ) {}
    constexpr explicit deref(      P1&&    p) noexcept_when(std::is_nothrow_move_constructible<P1>::value) : m_p1(std::move(p)) {}
    constexpr          deref(const deref&  e) noexcept_when(std::is_nothrow_copy_constructible<P1>::value) : m_p1(          e.m_p1 ) {} ///< Copy constructor
    constexpr          deref(      deref&& e) noexcept_when(std::is_nothrow_move_constructible<P1>::value) : m_p1(std::move(e.m_p1)) {} ///< Move constructor

    deref& operator=(const deref&) XTL_DELETED; ///< Assignment is not allowed for this class

    /// Type function returning a type that will be accepted by the pattern for
    /// a given subject type S. We use type function instead of an associated 
    /// type, because there is no a single accepted type for a #wildcard pattern
    /// for example. Requirement of #Pattern concept.
    template <typename S> struct accepted_type_for; // Intentionally no definition
    template <typename S> struct accepted_type_for<S&> { typedef typename P1::template accepted_type_for<S>::type& type; };

    template <typename T> bool operator()(T&& t) const { return m_p1(&std::forward<T>(t)); }

    P1 m_p1;
};

//------------------------------------------------------------------------------

/// #is_pattern_ is a helper meta-predicate capable of distinguishing all our patterns
template <typename P1> struct is_pattern_<deref<P1>> { static const bool value = true; };

//------------------------------------------------------------------------------

} // of namespace mch

//------------------------------------------------------------------------------

/// \note This operator has to be in the global namespace as E is unrestricted
///       and ADL won't work to find it!
template <typename P1>
inline auto operator&(P1&& p1) noexcept
        -> typename std::enable_if<
                        mch::is_pattern<P1>::value,
                        mch::address<
                            typename mch::underlying<decltype(mch::filter(std::forward<P1>(p1)))>::type
                        >
                    >::type
{
    return mch::address<
                        typename mch::underlying<decltype(mch::filter(std::forward<P1>(p1)))>::type
                    >(
                        mch::filter(std::forward<P1>(p1))
                    );
}

//------------------------------------------------------------------------------

/// \note This operator has to be in the global namespace as E is unrestricted
///       and ADL won't work to find it!
template <typename P1>
inline auto operator*(P1&& p1) noexcept
        -> typename std::enable_if<
                        mch::is_pattern<P1>::value,
                        mch::deref<
                            typename mch::underlying<decltype(mch::filter(std::forward<P1>(p1)))>::type
                        >
                    >::type
{
    return mch::deref<
                        typename mch::underlying<decltype(mch::filter(std::forward<P1>(p1)))>::type
                    >(
                        mch::filter(std::forward<P1>(p1))
                    );
}

//------------------------------------------------------------------------------
