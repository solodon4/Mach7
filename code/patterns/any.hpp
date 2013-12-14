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

//------------------------------------------------------------------------------

/// One-of pattern
template <typename T>
struct one_of
{
    one_of(std::initializer_list<T>&& init) : m_elements(std::move(init)) {}
    typedef T accepted_type; ///< Type accepted by the pattern. Requirement of #Pattern concept
    bool operator()(const T& s) const noexcept 
    {
        return std::find(m_elements.begin(),m_elements.end(),s) != m_elements.end(); 
    }
    std::initializer_list<T> m_elements;
};

//------------------------------------------------------------------------------

/// #is_pattern_ is a helper meta-predicate capable of distinguishing all our patterns
template <typename T> struct is_pattern_<one_of<T>> { enum { value = true }; };

//------------------------------------------------------------------------------

/// Convenience function for creating any pattern out of initializer list holding 
/// the set of acceptable values.
template <typename T>
inline one_of<T> any(std::initializer_list<T>&& init) noexcept 
{
    return one_of<T>(std::move(init));
}

//------------------------------------------------------------------------------
