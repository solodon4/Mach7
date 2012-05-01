///
/// \file common.hpp
///
/// Common definitions for pattern support by our library.
///
/// \autor Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of the XTL framework (http://parasol.tamu.edu/xtl/).
/// Copyright (C) 2005-2012 Texas A&M University.
/// All rights reserved.
///

#pragma once

#include "../metatools.hpp"
#include <utility>           // All our patterns define move-constructors and use std::move

//------------------------------------------------------------------------------

/// @is_pattern_ is a helper meta-predicate capable of distinguishing all our patterns
template <typename T> struct is_pattern_ { enum { value = false }; };

/// @is_pattern is a helper meta-predicate capable of distinguishing all our patterns
template <typename T> struct is_pattern : is_pattern_<typename underlying<T>::type> {};

//------------------------------------------------------------------------------

/// @is_expression_ is a helper meta-predicate that separates lazily evaluatable expressions we support
template <typename T> struct is_expression_ { enum { value = false }; };

/// @is_expression is a helper meta-predicate that separates lazily evaluatable expressions we support
template <typename T> struct is_expression : is_expression_<typename underlying<T>::type> {};

/// @either_is_expression is a only used to workaround a compiler stack overflow 
/// problem in MSVC when we were overloading operator||(E1&&,E2&&) and had || in
/// enabling condition for that overload. Now we use either_is_expression there 
/// instead.
#ifdef _MSC_VER
/// FIX: Fails to compile from MSVC IDE, but ok from command line
/// TODO: This is not even needed for tests on type switch - separate code
template <typename E1, typename E2> struct either_is_expression { enum { value = is_expression<E1>::value /*|| is_expression<E2>::value*/ }; };
#else
template <typename E1, typename E2> struct either_is_expression { enum { value = is_expression<E1>::value || is_expression<E2>::value }; };
#endif

//------------------------------------------------------------------------------
