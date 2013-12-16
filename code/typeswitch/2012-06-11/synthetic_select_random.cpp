///
/// \file
///
/// This file is a part of pattern matching testing suite.
///
/// \autor Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of the XTL framework (http://parasol.tamu.edu/xtl/).
/// Copyright (C) 2005-2012 Texas A&M University.
/// All rights reserved.
///

#include "testshape.hpp"
#include "match.hpp"

//------------------------------------------------------------------------------

template <size_t N>
struct shape_kind : OtherBase, Shape
{
    void accept(ShapeVisitor&) const;
};

//------------------------------------------------------------------------------

struct ShapeVisitor
{
    #define FOR_EACH_MAX NUMBER_OF_DERIVED-1
    #define FOR_EACH_N(N) virtual void visit(const shape_kind<N>&) {}
    #include "loop_over_numbers.hpp"
    #undef  FOR_EACH_N
    #undef  FOR_EACH_MAX
};

//------------------------------------------------------------------------------

template <size_t N> void shape_kind<N>::accept(ShapeVisitor& v) const { v.visit(*this); }

//------------------------------------------------------------------------------

size_t frequency(const Shape& s)
{
    MatchP(s)
    {
        #define FOR_EACH_MAX  NUMBER_OF_DERIVED-1
        #define FOR_EACH_N(N) CaseP(shape_kind<N>) return 10*N+7;
        #include "loop_over_numbers.hpp"
        #undef  FOR_EACH_N
        #undef  FOR_EACH_MAX
    }
    EndMatchP
    return invalid;
}

//size_t frequency(intptr_t vtbl) { return frequency(*reinterpret_cast<const Shape*>(&vtbl)); }
template <> struct bindings<Shape> { FQS(frequency); };

//------------------------------------------------------------------------------

#if 1
XTL_TIMED_FUNC_BEGIN
size_t do_match(const Shape& s, size_t)
{
    MatchP(s)
    {
        #define FOR_EACH_MAX  NUMBER_OF_DERIVED-1
        #define FOR_EACH_N(N) CaseP(shape_kind<N>) return N;
        #include "loop_over_numbers.hpp"
        #undef  FOR_EACH_N
        #undef  FOR_EACH_MAX
    }
    EndMatchP
    return invalid;
}
XTL_TIMED_FUNC_END
#else
#error No preprocessed version provided
#endif

//------------------------------------------------------------------------------

    struct Visitor : ShapeVisitor
    {
        Visitor(size_t r) : result(r) {}
        #define FOR_EACH_MAX  NUMBER_OF_DERIVED-1
        #define FOR_EACH_N(N) virtual void visit(const shape_kind<N>&) { result = N; }
        #include "loop_over_numbers.hpp"
        #undef  FOR_EACH_N
        #undef  FOR_EACH_MAX
        size_t result;
    };


XTL_TIMED_FUNC_BEGIN
size_t do_visit(const Shape& s, size_t m)
{
    Visitor v(m);
    //v.result = invalid;
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

#include "testvismat.hpp"    // Utilities for timing tests

//------------------------------------------------------------------------------

int main()
{
//    verdict pp = test_repetitive();
    verdict ps = test_sequential();
    verdict pr = test_randomized();
    std::cout << "OVERALL: "
//              << "Repetitive: " << pp << "; "
              << "Sequential: " << ps << "; "
              << "Random: "     << pr 
              << std::endl; 
}

//------------------------------------------------------------------------------
