///
/// \file
///
/// This file is a part of pattern matching testing suite.
///
/// \autor Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of the XTL framework (http://parasol.tamu.edu/xtl/).
/// Copyright (C) 2005-2011 Texas A&M University.
/// All rights reserved.
///

#include "testutils.hpp"
#include "match_generic.hpp"

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
    return -1;
}
//size_t frequency(intptr_t vtbl) { return frequency(*reinterpret_cast<const Shape*>(&vtbl)); }
template <> struct match_members<Shape> { FQS(frequency); };

//------------------------------------------------------------------------------

#if 1
XTL_DO_NOT_INLINE_BEGIN
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
    return -1;
}
XTL_DO_NOT_INLINE_END
#else
XTL_DO_NOT_INLINE_BEGIN
#error No preprocessed version provided
XTL_DO_NOT_INLINE_END
#endif

//------------------------------------------------------------------------------

XTL_DO_NOT_INLINE_BEGIN
size_t do_visit(const Shape& s, size_t)
{
    struct Visitor : ShapeVisitor
    {
        #define FOR_EACH_MAX  NUMBER_OF_DERIVED-1
        #define FOR_EACH_N(N) virtual void visit(const shape_kind<N>&) { result = N; }
        #include "loop_over_numbers.hpp"
        #undef  FOR_EACH_N
        #undef  FOR_EACH_MAX
        size_t result;
    };

    Visitor v;
    v.result = -1;
    s.accept(v);
    return v.result;
}
XTL_DO_NOT_INLINE_END

//------------------------------------------------------------------------------

Shape* make_shape(size_t i)
{
    switch (i)
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

int main()
{
//  int pp = test_repetitive();
    int ps = test_sequential();
    int pr = test_randomized();
    std::cout << "OVERALL: "
//            << "Repetitive: " << abs(pp) << (pp >= 0 ? "% slower" : "% faster") << "; "
              << "Sequential: " << abs(ps) << (ps >= 0 ? "% slower" : "% faster") << "; "
              << "Random: "     << abs(pr) << (pr >= 0 ? "% slower" : "% faster") 
              << std::endl; 
}

//------------------------------------------------------------------------------
