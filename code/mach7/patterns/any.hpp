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
/// \file any.hpp
///
/// This file defines any pattern (one-of pattern).
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// \see https://parasol.tamu.edu/mach7/
/// \see https://github.com/solodon4/Mach7
/// \see https://github.com/solodon4/SELL
///

#pragma once

#include "common.hpp"
#include <algorithm>
#if XTL_SUPPORT(initializer_list)
#include <initializer_list>
#else
#include <vector>
#endif

// TODO: Make 3 versions of one_of pattern: initializer_list, variadic and array based, each enabled independently based on compiler support via any() overloading

namespace mch ///< Mach7 library namespace
{

//------------------------------------------------------------------------------

#if XTL_SUPPORT(initializer_list)
/// One-of pattern
template <typename T>
struct one_of
{
    one_of(std::initializer_list<T>&& init) noexcept : m_elements(std::move(init)) {}

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
#else
/// One-of pattern
template <typename T>
struct one_of
{
    one_of(const T* b, const T* e) noexcept : m_elements(b,e) {}

    /// Type function returning a type that will be accepted by the pattern for
    /// a given subject type S. We use type function instead of an associated 
    /// type, because there is no a single accepted type for a #wildcard pattern
    /// for example. Requirement of #Pattern concept.
    template <typename S> struct accepted_type_for { typedef T type; };

    bool operator()(const T& s) const noexcept 
    {
        return std::find(m_elements.begin(),m_elements.end(),s) != m_elements.end(); 
    }

    std::vector<T> m_elements;
};
#endif

//------------------------------------------------------------------------------

/// #is_pattern_ is a helper meta-predicate capable of distinguishing all our patterns
template <typename T> struct is_pattern_<one_of<T>> { static const bool value = true; };

//------------------------------------------------------------------------------

#if XTL_SUPPORT(initializer_list)
/// Convenience function for creating any pattern out of initializer list holding 
/// the set of acceptable values.
template <typename T>
inline one_of<T> any(std::initializer_list<T>&& init) noexcept 
{
    return one_of<T>(std::move(init));
}
#else
/// Convenience function for creating any pattern out of initializer list holding 
/// the set of acceptable values.
template <typename T>
inline one_of<T> any(const T* b, const T* e) noexcept 
{
    return one_of<T>(b,e);
}
#endif

//------------------------------------------------------------------------------

} // of namespace mch
