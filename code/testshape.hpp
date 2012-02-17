///
/// \file testshape.hpp
///
/// Shape class hierarchy used in our synthetic tests.
///
/// \autor Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of the XTL framework (http://parasol.tamu.edu/xtl/).
/// Copyright (C) 2005-2011 Texas A&M University.
/// All rights reserved.
///

#pragma once

#include <cstddef>

//------------------------------------------------------------------------------

#if !defined(NUMBER_OF_VFUNCS)
#define NUMBER_OF_VFUNCS  1
#endif

#if !defined(NUMBER_OF_DERIVED)
#define NUMBER_OF_DERIVED 100
#endif

#if 1
#define XTL_TIMED_FUNC_BEGIN XTL_FORCE_INLINE_BEGIN
#define XTL_TIMED_FUNC_END   XTL_FORCE_INLINE_END
#else
#define XTL_TIMED_FUNC_BEGIN XTL_DO_NOT_INLINE_BEGIN
#define XTL_TIMED_FUNC_END   XTL_DO_NOT_INLINE_END
#endif

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
        m_member0(n+0),
        m_member1(n+1),
        m_member2(n+2),
        m_member3(n+3),
        m_member4(n+4),
        m_member5(n+5),
        m_member6(n+6),
        m_member7(n+7),
        m_member8(n+8),
        m_member9(n+9)
    {}

    virtual ~Shape() {}
    virtual void accept(ShapeVisitor&) const = 0;
    virtual void raise() const {} // Polymorphic exception idiom: http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Polymorphic_Exception
    #define FOR_EACH_MAX NUMBER_OF_VFUNCS-2
    #define FOR_EACH_N(N) virtual void foo ## N() {}
    #include "loop_over_numbers.hpp"
    #undef  FOR_EACH_N
    #undef  FOR_EACH_MAX

    int m_member0;
    int m_member1;
    int m_member2;
    int m_member3;
    int m_member4;
    int m_member5;
    int m_member6;
    int m_member7;
    int m_member8;
    int m_member9;
};

//------------------------------------------------------------------------------
