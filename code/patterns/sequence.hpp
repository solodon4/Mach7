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
/// This file defines sequence patterns supported by our library.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// \see https://parasol.tamu.edu/mach7/
/// \see https://github.com/solodon4/Mach7
/// \see https://github.com/solodon4/SELL
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
    constexpr sequence(const I&  b, const I&  e) noexcept_when(std::is_nothrow_copy_constructible<I>::value) : m_begin(          b ), m_end(          e ) {}
    constexpr sequence(      I&& b,       I&& e) noexcept_when(std::is_nothrow_move_constructible<I>::value) : m_begin(std::move(b)), m_end(std::move(e)) {}

    constexpr sequence(const sequence&  src) noexcept_when(std::is_nothrow_copy_constructible<I>::value) : m_begin(          src.m_begin ), m_end(          src.m_end ) {} ///< Copy constructor
    constexpr sequence(      sequence&& src) noexcept_when(std::is_nothrow_move_constructible<I>::value) : m_begin(std::move(src.m_begin)), m_end(std::move(src.m_end)) {} ///< Move constructor

    sequence& operator=(const sequence&) XTL_DELETED; ///< Assignment is not allowed for this class

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
