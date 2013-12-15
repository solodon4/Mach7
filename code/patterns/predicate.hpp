///
/// \file predicate.hpp
///
/// This file defines predicate patterns supported by our library.
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

template <typename A>
struct predicate
{
    explicit predicate(bool (&g)(A)) noexcept : f(g) {}
    predicate(const predicate&  e) noexcept : f(e.f) {} ///< Copy constructor    
    predicate& operator=(const predicate&); ///< Assignment is not allowed for this class

    /// Type function returning a type that will be accepted by the pattern for
    /// a given subject type S. We use type function instead of an associated 
    /// type, because there is no a single accepted type for a #wildcard pattern
    /// for example. Requirement of #Pattern concept.
    template <typename S> struct accepted_type_for { typedef A type; };

    bool operator()(const A& a) const { return f(a); }

    bool (&f)(A);
};

//------------------------------------------------------------------------------

/// #is_pattern_ is a helper meta-predicate capable of distinguishing all our patterns
template <typename A> struct is_pattern_<predicate<A>>    { static const bool value = true; };

//------------------------------------------------------------------------------

template <typename A> inline predicate<A> filter(bool (&f)(A)) noexcept { return predicate<A>(f); }

} // of namespace mch

//------------------------------------------------------------------------------
