///
/// \file testshape.hpp
///
/// Shape class hierarchy used in our synthetic tests.
///
/// \autor Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of the XTL framework (http://parasol.tamu.edu/xtl/).
/// Copyright (C) 2005-2012 Texas A&M University.
/// All rights reserved.
///

#pragma once

#include "rnd.hpp"

//------------------------------------------------------------------------------

#if !defined(XTL_RANDOMIZE_TAGS)
#define XTL_RANDOMIZE_TAGS 0
#endif

#if XTL_RANDOMIZE_TAGS
/// When tag randomization is enabled, @tag<>::value metafunction returns some
/// unique pseudo-random numbers.
template <size_t N> struct tag : rnd<N> {};
/// A run-time equivalent of the above.
inline size_t get_tag(size_t n) { return get_rnd(n); }
#else
/// By default @tag<>::value metafunction behaves as identity thus assigning
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

#if !defined(XTL_INLINE_TIMED_FUNCS)
#define XTL_INLINE_TIMED_FUNCS 0
#endif

#if XTL_INLINE_TIMED_FUNCS
#define XTL_TIMED_FUNC_BEGIN XTL_FORCE_INLINE_BEGIN
#define XTL_TIMED_FUNC_END   XTL_FORCE_INLINE_END
#else
#define XTL_TIMED_FUNC_BEGIN XTL_DO_NOT_INLINE_BEGIN
#define XTL_TIMED_FUNC_END   XTL_DO_NOT_INLINE_END
#endif

//------------------------------------------------------------------------------

const size_t invalid = size_t(-1);

//------------------------------------------------------------------------------

struct ShapeVisitor;

//------------------------------------------------------------------------------

struct OtherBase
{
    OtherBase() : m_foo(0xAAAAAAAA) {}
    virtual int foo() { return m_foo; };

    int m_foo;
};

//------------------------------------------------------------------------------

struct Shape
{
    size_t        m_kind;
    const size_t* m_all_kinds;
    size_t        m_fdc_id; // Fast dynamic cast ID

    Shape(size_t n = -1, size_t fdc_id = -1) : m_kind(n), m_all_kinds(0), m_fdc_id(fdc_id),
        m_member0(get_rnd(n+0)),
        m_member1(get_rnd(n+1)),
        m_member2(get_rnd(n+2)),
        m_member3(get_rnd(n+3)),
        m_member4(get_rnd(n+4)),
        m_member5(get_rnd(n+5)),
        m_member6(get_rnd(n+6)),
        m_member7(get_rnd(n+7)),
        m_member8(get_rnd(n+8)),
        m_member9(get_rnd(n+9))
    {}

    virtual ~Shape() {}
    virtual void accept(ShapeVisitor&) const = 0;
    virtual void raise() const {} // Polymorphic exception idiom: http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Polymorphic_Exception
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
