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

#include "testshape.hpp"
#include "match.hpp"

//------------------------------------------------------------------------------

template <size_t N>
struct shape_kind : OtherBase, Shape
{
    shape_kind() : Shape(N) { this->m_all_kinds = (const size_t*)get_kinds<Shape>(original2remapped<Shape>(tag_type(N))); }
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

SKV(Shape,0); // Declare the smallest kind value for Shape hierarchy
template <>         struct match_members<Shape>         { KS(Shape::m_kind); };
template <size_t N> struct match_members<shape_kind<N>> { KV(N); CM(0,shape_kind<N>::m_member0); CM(1,shape_kind<N>::m_member1); };

#if 1
XTL_TIMED_FUNC_BEGIN
size_t do_match(const Shape& s, size_t)
{
    MatchF(s)
    {
    #define FOR_EACH_MAX      NUMBER_OF_DERIVED-1
    #define FOR_EACH_N(N)     CaseF(shape_kind<N>) return N;
    #include "loop_over_numbers.hpp"
    #undef  FOR_EACH_N
    #undef  FOR_EACH_MAX
    }
    EndMatchF
    return -1;
}
XTL_TIMED_FUNC_END
#else
#error No preprocessed version provided
#endif

//------------------------------------------------------------------------------

XTL_TIMED_FUNC_BEGIN
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
XTL_TIMED_FUNC_END

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

#include "testutils.hpp"    // Utilities for timing tests

//------------------------------------------------------------------------------

int main()
{
    int pp = test_repetitive();
    int ps = test_sequential();
    int pr = test_randomized();
    std::cout << "OVERALL: "
              << "Repetitive: " << abs(pp) << (pp >= 0 ? "% slower" : "% faster") << "; "
              << "Sequential: " << abs(ps) << (ps >= 0 ? "% slower" : "% faster") << "; "
              << "Random: "     << abs(pr) << (pr >= 0 ? "% slower" : "% faster") 
              << std::endl; 
}

//------------------------------------------------------------------------------
