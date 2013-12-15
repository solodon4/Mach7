///
/// \file sequence.hpp
///
/// This file defines sequence patterns supported by our library.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of Mach7 library (http://parasol.tamu.edu/mach7/).
/// Copyright (C) 2011-2012 Texas A&M University.
/// All rights reserved.
///

#pragma once

#include "common.hpp"
#include <vector>

namespace mch ///< Mach7 library namespace
{

//------------------------------------------------------------------------------

template <typename I>
struct sequence
{
    sequence(const I& b, const I& e) : m_begin(b), m_end(e) {}
    sequence(I&& b, I&& e) noexcept : m_begin(std::move(b)), m_end(std::move(e)) {}
    sequence(sequence&& src) noexcept : m_begin(std::move(src.m_begin)), m_end(std::move(src.m_end)) {}

    /// Type function returning a type that will be accepted by the pattern for
    /// a given subject type S. We use type function instead of an associated 
    /// type, because there is no a single accepted type for a #wildcard pattern
    /// for example. Requirement of #Pattern concept.
    template <typename S> struct accepted_type_for; // Intentionally no definition

    template <typename C>
    bool operator()(const C& c) const 
    {
        m_begin = c.begin();
        m_end   = c.end();
        return true; 
    }

    I m_begin;
    I m_end;
};

//------------------------------------------------------------------------------

/// #is_pattern_ is a helper meta-predicate capable of distinguishing all our patterns
template <typename I> struct is_pattern_<sequence<I>> { static const bool value = true; };

//------------------------------------------------------------------------------

template <typename T, typename A>
sequence<typename std::vector<T,A>::const_iterator> seq(const std::vector<T,A>& v)
{
    return sequence<typename std::vector<T,A>::const_iterator>(v.begin(),v.end());
}

//------------------------------------------------------------------------------

} // of namespace mch

//------------------------------------------------------------------------------
