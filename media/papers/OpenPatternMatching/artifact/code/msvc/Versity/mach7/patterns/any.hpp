///
/// \file any.hpp
///
/// This file defines any pattern (one-of pattern).
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of Mach7 library (http://parasol.tamu.edu/mach7/).
/// Copyright (C) 2011-2012 Texas A&M University.
/// All rights reserved.
///

#pragma once

#include "common.hpp"
#include <algorithm>

namespace mch ///< Mach7 library namespace
{

//------------------------------------------------------------------------------

/// One-of pattern
template <typename T>
struct one_of
{
    one_of(std::initializer_list<T>&& init) : m_elements(std::move(init)) {}

    /// Type function returning a type that will be accepted by the pattern for
    /// a given subject type S. We use type function instead of an associated 
    /// type, because there is no a single accepted type for a #wildcard pattern
    /// for example. Requirement of #Pattern concept.
    template <typename S> struct accepted_type_for { typedef T type; };

    bool operator()(const T& s) const noexcept 
    {
        return std::find(m_elements.begin(),m_elements.end(),s) != m_elements.end(); 
    }

    std::initializer_list<T> m_elements;
};

//------------------------------------------------------------------------------

/// #is_pattern_ is a helper meta-predicate capable of distinguishing all our patterns
template <typename T> struct is_pattern_<one_of<T>> { static const bool value = true; };

//------------------------------------------------------------------------------

/// Convenience function for creating any pattern out of initializer list holding 
/// the set of acceptable values.
template <typename T>
inline one_of<T> any(std::initializer_list<T>&& init) noexcept 
{
    return one_of<T>(std::move(init));
}

//------------------------------------------------------------------------------

} // of namespace mch
