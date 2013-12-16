///
/// \file common.hpp
///
/// Common definitions for pattern support by our library.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of Mach7 library (http://parasol.tamu.edu/mach7/).
/// Copyright (C) 2011-2012 Texas A&M University.
/// All rights reserved.
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

} // of namespace mch
