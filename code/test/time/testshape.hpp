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
/// Shape class hierarchy used in our synthetic tests.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// \see https://parasol.tamu.edu/mach7/
/// \see https://github.com/solodon4/Mach7
/// \see https://github.com/solodon4/SELL
///
/// \note Unlike match.hpp, use of this file does not require member matching
///       definitions or any other definitions.
///
///

#pragma once

#include "rnd.hpp"
#include "testutils.hpp"

//------------------------------------------------------------------------------

#if !defined(XTL_RANDOMIZE_TAGS)
#define XTL_RANDOMIZE_TAGS 0
#endif

#if XTL_RANDOMIZE_TAGS
/// When tag randomization is enabled, #tag<>::value metafunction returns some
/// unique pseudo-random numbers.
template <size_t N> struct tag : rnd<N> {};
/// A run-time equivalent of the above.
inline size_t get_tag(size_t n) { return mch::get_rnd(n); }
#else
/// By default #tag<>::value metafunction behaves as identity thus assigning
/// a class number N a tag N
template <size_t N> struct tag { static const size_t value = N; };
/// A run-time equivalent of the above.
inline size_t get_tag(size_t n) { return n; }
#endif

//------------------------------------------------------------------------------

#if !defined(NUMBER_OF_VFUNCS)
#define NUMBER_OF_VFUNCS  1
#endif

#if !defined(NUMBER_OF_DERIVED)
#define NUMBER_OF_DERIVED 100
#endif

//------------------------------------------------------------------------------

const size_t invalid = size_t(-1);

//------------------------------------------------------------------------------

struct ShapeVisitor;

//------------------------------------------------------------------------------

struct OtherBase
{
    OtherBase() : m_foo(0xAAAAAAAA) {}
    virtual int foo() { return m_foo; }

    int m_foo;
};

//------------------------------------------------------------------------------

struct Shape
{
    size_t        m_kind;
    const size_t* m_all_kinds;
    size_t        m_fdc_id; // Fast dynamic cast ID

    Shape(size_t n = -1, size_t fdc_id = -1) : m_kind(n), m_all_kinds(0), m_fdc_id(fdc_id),
        m_member0(mch::get_rnd(n+0)),
        m_member1(mch::get_rnd(n+1)),
        m_member2(mch::get_rnd(n+2)),
        m_member3(mch::get_rnd(n+3)),
        m_member4(mch::get_rnd(n+4)),
        m_member5(mch::get_rnd(n+5)),
        m_member6(mch::get_rnd(n+6)),
        m_member7(mch::get_rnd(n+7)),
        m_member8(mch::get_rnd(n+8)),
        m_member9(mch::get_rnd(n+9))
    {}

    virtual ~Shape() {}
    virtual void accept(ShapeVisitor&) const = 0;
    virtual void raise() const {} // Polymorphic exception idiom: http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Polymorphic_Exception
    virtual void raise_ptr() const {} // Polymorphic exception idiom: http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Polymorphic_Exception
    #define FOR_EACH_MAX NUMBER_OF_VFUNCS-2
    #define FOR_EACH_N(N) virtual void foo ## N() {}
    #include "loop_over_numbers.hpp"
    #undef  FOR_EACH_N
    #undef  FOR_EACH_MAX

    size_t m_member0;
    size_t m_member1;
    size_t m_member2;
    size_t m_member3;
    size_t m_member4;
    size_t m_member5;
    size_t m_member6;
    size_t m_member7;
    size_t m_member8;
    size_t m_member9;
};

//------------------------------------------------------------------------------
