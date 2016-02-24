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
/// This file defines equivalence patterns supported by our library.
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

template <typename E1>
struct equivalence
{
    static_assert(is_expression<E1>::value, "Argument E1 of an equivalence pattern must be a lazy expression");
    static_assert(!is_var<E1>::value,       "Attempting to host var<> directly. Use filter() to wrap it into ref2<>");

    constexpr explicit equivalence(const E1&           e) noexcept_when(std::is_nothrow_copy_constructible<E1>::value) : m_e1(          e ) {}
    constexpr explicit equivalence(      E1&&          e) noexcept_when(std::is_nothrow_move_constructible<E1>::value) : m_e1(std::move(e)) {}
    constexpr          equivalence(const equivalence&  e) noexcept_when(std::is_nothrow_copy_constructible<E1>::value) : m_e1(          e.m_e1 ) {} ///< Copy constructor
    constexpr          equivalence(      equivalence&& e) noexcept_when(std::is_nothrow_move_constructible<E1>::value) : m_e1(std::move(e.m_e1)) {} ///< Move constructor

    equivalence& operator=(const equivalence&) XTL_DELETED; ///< Assignment is not allowed for this class

    typedef typename E1::result_type result_type;   ///< Type of result when used in expression. Requirement of #LazyExpression concept

    /// Type function returning a type that will be accepted by the pattern for
    /// a given subject type S. We use type function instead of an associated 
    /// type, because there is no a single accepted type for a #wildcard pattern
    /// for example. Requirement of #Pattern concept.
    template <typename S> struct accepted_type_for { typedef result_type type; };

    operator result_type() const { return eval(m_e1); } // FIX: avoid implicit conversion in lazy expressions

    bool operator()(const result_type& u) const { return u == eval(m_e1); }

    E1 m_e1;
};

//------------------------------------------------------------------------------

/// #is_pattern_ is a helper meta-predicate capable of distinguishing all our patterns
template <typename E1> struct is_pattern_<equivalence<E1>>    { static const bool value = true; };

/// #is_expression_ is a helper meta-predicate that separates lazily evaluatable expressions we support
template <typename E1> struct is_expression_<equivalence<E1>> { static const bool value = true; };

//------------------------------------------------------------------------------

template <typename E1>              
inline typename equivalence<E1>::result_type eval(const equivalence<E1>& e) { return eval(e.m_e1); }

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
