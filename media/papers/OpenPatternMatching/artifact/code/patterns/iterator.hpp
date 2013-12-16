///
/// \file iterator.hpp
///
/// This file defines iterator pattern supported by our library.
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

template <typename C>
struct iterator_increment_suffix
{
    iterator_increment_suffix(const iterator_increment_suffix& i) noexcept : m_begin(i.m_begin) {} ///< Copy constructor
    iterator_increment_suffix& operator=(const iterator_increment_suffix&); ///< Assignment is not allowed for this class

    /// Type function returning a type that will be accepted by the pattern for
    /// a given subject type S. We use type function instead of an associated 
    /// type, because there is no a single accepted type for a #wildcard pattern
    /// for example. Requirement of #Pattern concept.
    template <typename S> struct accepted_type_for { typedef C type; };

    typename C::const_iterator m_begin;
    typename C::const_iterator m_end;
};

template <typename C>
struct iterator
{
    iterator(const iterator& i) noexcept : m_begin(i.m_begin) {} ///< Copy constructor
    iterator& operator=(const iterator&); ///< Assignment is not allowed for this class

    /// Type function returning a type that will be accepted by the pattern for
    /// a given subject type S. We use type function instead of an associated 
    /// type, because there is no a single accepted type for a #wildcard pattern
    /// for example. Requirement of #Pattern concept.
    template <typename S> struct accepted_type_for { typedef C type; };

    bool operator()(const C& c) const 
    {
        m_begin = c.begin(); 
        m_end   = c.end();
        return true;
    }

    R T::operator++(int);

    typename C::const_iterator m_begin;
    typename C::const_iterator m_end;
};

//------------------------------------------------------------------------------

/// #is_pattern_ is a helper meta-predicate capable of distinguishing all our patterns
template <typename P1> struct is_pattern_<iterator<P1>> { static const bool value = true; };

//------------------------------------------------------------------------------

} // of namespace mch

//------------------------------------------------------------------------------
