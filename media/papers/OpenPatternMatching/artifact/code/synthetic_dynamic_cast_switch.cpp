///
/// \file
///
/// This file is a part of pattern matching testing suite.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of Mach7 library (http://parasol.tamu.edu/mach7/).
/// Copyright (C) 2011-2013 Texas A&M University.
/// All rights reserved.
///

#include "testshape.hpp"
#include "config.hpp"

//------------------------------------------------------------------------------

size_t values[NUMBER_OF_DERIVED] = {};

//------------------------------------------------------------------------------

template <size_t N>
struct shape_kind : shape_kind<N/2>
{
    typedef shape_kind<N/2> base_class;
    shape_kind(size_t n = tag<N>::value) : base_class(n) {}
    void accept(ShapeVisitor&) const;
};

template <>
struct shape_kind<0> : OtherBase, Shape
{
    typedef Shape base_class;
    shape_kind(size_t n = tag<0>::value) : base_class(n) {}
    void accept(ShapeVisitor&) const;
};

//------------------------------------------------------------------------------

struct ShapeVisitor
{
    virtual void visit(const shape_kind<0>&) {}
    #define FOR_EACH_MAX NUMBER_OF_DERIVED-2
    #define FOR_EACH_N(N) virtual void visit(const shape_kind<N+1>& s) { visit(static_cast<const shape_kind<N+1>::base_class&>(s)); }
    #include "loop_over_numbers.hpp"
    #undef  FOR_EACH_N
    #undef  FOR_EACH_MAX
};

//------------------------------------------------------------------------------

template <size_t N> void shape_kind<N>::accept(ShapeVisitor& v) const { v.visit(*this); }
                    void shape_kind<0>::accept(ShapeVisitor& v) const { v.visit(*this); }

//------------------------------------------------------------------------------

XTL_TIMED_FUNC_BEGIN
size_t do_match(const Shape& s, size_t)
{
    switch (s.m_kind)
    {
        #define FOR_EACH_MAX  NUMBER_OF_DERIVED-1
        #define FOR_EACH_N(N) case N: return s.m_member7 + N; // values[N]; // return N;
        #include "loop_over_numbers.hpp"
        #undef  FOR_EACH_N
        #undef  FOR_EACH_MAX
    }
    return invalid;
}
XTL_TIMED_FUNC_END

//------------------------------------------------------------------------------

XTL_TIMED_FUNC_BEGIN
size_t do_visit(const Shape& s, size_t)
{
    struct Visitor : ShapeVisitor
    {
        #define FOR_EACH_MAX  NUMBER_OF_DERIVED-1
        #define FOR_EACH_N(N) virtual void visit(const shape_kind<N>& s) { result = s.m_member7 + N; }
        #include "loop_over_numbers.hpp"
        #undef  FOR_EACH_N
        #undef  FOR_EACH_MAX
        size_t result;
    };

    Visitor v;
    v.result = invalid;
    s.accept(v);
    return v.result;
}
XTL_TIMED_FUNC_END

//------------------------------------------------------------------------------

Shape* make_shape(size_t i)
{
    switch (i % NUMBER_OF_DERIVED)
    {
        #define FOR_EACH_MAX  NUMBER_OF_DERIVED-1
        #define FOR_EACH_N(N) case N: return new shape_kind<N>;
        #include "loop_over_numbers.hpp"
        #undef  FOR_EACH_N
        #undef  FOR_EACH_MAX
    }
    return 0;
}

//------------------------------------------------------------------------------

#include "testvismat1.hpp"    // Utilities for timing tests

//------------------------------------------------------------------------------

int main()
{
    for (int i = 0; i < NUMBER_OF_DERIVED; ++i)
        values[i] = i;

    using namespace mch; // Mach7's library namespace

    verdict pp = test_repetitive();
    verdict ps = test_sequential();
    verdict pr = test_randomized();
    std::cout << "OVERALL: "
              << "Repetitive: " << pp << "; "
              << "Sequential: " << ps << "; "
              << "Random: "     << pr 
              << std::endl; 
}

//------------------------------------------------------------------------------
