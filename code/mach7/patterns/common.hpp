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
/// Common definitions for pattern support by our library.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// \see https://parasol.tamu.edu/mach7/
/// \see https://github.com/solodon4/Mach7
/// \see https://github.com/solodon4/SELL
///

#pragma once

#include "../metatools.hpp"
#include <utility>           // All our patterns define move-constructors and use std::move

namespace mch ///< Mach7 library namespace
{

//------------------------------------------------------------------------------

///
/// \interface Pattern
///
/// \brief Concept for all the patterns in the library
///

/// #is_pattern_ is a helper meta-predicate capable of distinguishing all our patterns
template <typename T> struct is_pattern_ { static const bool value = false; };

/// #is_pattern is a helper meta-predicate capable of distinguishing all our patterns
template <typename T> struct is_pattern : is_pattern_<typename underlying<T>::type> {};

//------------------------------------------------------------------------------

///
/// \interface LazyExpression
///
/// \brief Concept for all the lazy expressions in the library
///

/// #is_expression_ is a helper meta-predicate that separates lazily evaluatable expressions we support
template <typename T> struct is_expression_ { static const bool value = false; };

/// #is_expression is a helper meta-predicate that separates lazily evaluatable expressions we support
template <typename T> struct is_expression : is_expression_<typename underlying<T>::type> {};

//------------------------------------------------------------------------------

/// #either_is_expression is a only used to workaround a compiler stack overflow 
/// problem in MSVC when we were overloading operator||(E1&&,E2&&) and had || in
/// enabling condition for that overload. Now we use either_is_expression there 
/// instead.
template <typename P1, typename P2> struct either_is_pattern    { static const bool value =    is_pattern<P1>::value ||    is_pattern<P2>::value; };
template <typename E1, typename E2> struct either_is_expression { static const bool value = is_expression<E1>::value || is_expression<E2>::value; };

//------------------------------------------------------------------------------

/// eval represents a set of overloads defined on our #LazyExpression.
/// \note This generic function should never be called, instead, you must 
///       include header file of the specific lazy expression, which will
///       define required overload.
/// FIX: Somehow this doesn't work as intended - only assert when this 
///      overload gets chosen, which should only happen when you didn't
///      include headers wtih other overloads of eval. Hence commented body.
template <typename E>
inline auto eval(const E&) noexcept
        -> typename std::enable_if<is_expression<E>::value, typename underlying<E>::type::result_type>::type;
//{
//    static_assert(false,"You must include header file with overload of eval(E) for lazy expression of kind E");
//}

//------------------------------------------------------------------------------

///@{
/// Helper function that is used by the library to indicate that a value is used as
/// a subject i.e. in RHS and thus should not be treated as a lazy expression, but
/// rather as an eager one. Users seem to be doing this in nested Match-statements
/// where the subject of the inner Match-statement is a variable bound in the 
/// outer one.
/// 
/// In most cases it simply forwards to the underlying value...
template <typename T>
inline typename std::enable_if<!is_expression<T>::value, T&&>::type subject(T&& t) noexcept
{
    return std::forward<T>(t);
}

//------------------------------------------------------------------------------

/// ... but in the case of a LazyExpression it evaluates it first.
/// \note We couldn't have simply called eval always because it is not defined 
///       for non-LazyExpression's
template <typename T>
inline auto subject(T&& t) noexcept -> typename std::enable_if<is_expression<T>::value, decltype(eval(std::forward<T>(t)))>::type
{
    return eval(std::forward<T>(t));
}
///@}

//------------------------------------------------------------------------------

} // of namespace mch
