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
#include "testutils.hpp"
#include "match.hpp"

//------------------------------------------------------------------------------

#if !defined(NUMBER_OF_BASES)
  #define NUMBER_OF_BASES  5
#endif
#if NUMBER_OF_BASES > NUMBER_OF_DERIVED
  #error Visitor Forwarding only makes sense when number of base classes is smaller than the number of all derived classes.
#endif
#if !defined(XTL_VISITOR_FORWARDING)
  #define XTL_VISITOR_FORWARDING 0
#endif

//------------------------------------------------------------------------------

#if XTL_VISITOR_FORWARDING

template <size_t N>
struct shape_kind;

#define FOR_EACH_MAX NUMBER_OF_BASES-1
#if XTL_DEFAULT_SYNTAX == 'K' || XTL_DEFAULT_SYNTAX == 'k' || XTL_DEFAULT_SYNTAX == 'F' || XTL_DEFAULT_SYNTAX == 'f'
#define FOR_EACH_N(N) template <> struct shape_kind<N> : OtherBase, Shape { typedef Shape base_type; void accept(ShapeVisitor&) const; shape_kind(size_t k = tag<N>::value) : base_type(k) {} };
#else
#define FOR_EACH_N(N) template <> struct shape_kind<N> : OtherBase, Shape { typedef Shape base_type; void accept(ShapeVisitor&) const; };
#endif
#include "loop_over_numbers.hpp"
#undef  FOR_EACH_N
#undef  FOR_EACH_MAX

//------------------------------------------------------------------------------

template <size_t N>
struct shape_kind : shape_kind<N % NUMBER_OF_BASES>
{
    typedef shape_kind<N % NUMBER_OF_BASES> base_type;
#if XTL_DEFAULT_SYNTAX == 'K' || XTL_DEFAULT_SYNTAX == 'k' || XTL_DEFAULT_SYNTAX == 'F' || XTL_DEFAULT_SYNTAX == 'f'
    shape_kind() : base_type(tag<N>::value) {}
#endif
    void accept(ShapeVisitor&) const;
};

#else // !XTL_VISITOR_FORWARDING

template <size_t N>
struct shape_kind : OtherBase, Shape
{
#if XTL_DEFAULT_SYNTAX == 'K' || XTL_DEFAULT_SYNTAX == 'k' || XTL_DEFAULT_SYNTAX == 'F' || XTL_DEFAULT_SYNTAX == 'f'
    shape_kind() : Shape(tag<N>::value) {}
#endif
    void accept(ShapeVisitor& v) const;
};

#endif // XTL_VISITOR_FORWARDING

//------------------------------------------------------------------------------

struct ShapeVisitor
{
    virtual void visit(const Shape&) {}
    #define FOR_EACH_MAX NUMBER_OF_DERIVED-1
    #if XTL_VISITOR_FORWARDING
    #define FOR_EACH_N(N) virtual void visit(const shape_kind<N>& s) { visit(static_cast<const shape_kind<N>::base_type&>(s)); }
    #else
    #define FOR_EACH_N(N) virtual void visit(const shape_kind<N>&) {}
    #endif
    #include "loop_over_numbers.hpp"
    #undef  FOR_EACH_N
    #undef  FOR_EACH_MAX
};

//------------------------------------------------------------------------------

#if XTL_VISITOR_FORWARDING
  #define FOR_EACH_MAX NUMBER_OF_BASES-1
  #define FOR_EACH_N(N) void shape_kind<N>::accept(ShapeVisitor& v) const { v.visit(*this); }
  #include "loop_over_numbers.hpp"
  #undef  FOR_EACH_N
  #undef  FOR_EACH_MAX
#endif // XTL_VISITOR_FORWARDING

//------------------------------------------------------------------------------

template <size_t N> void shape_kind<N>::accept(ShapeVisitor& v) const { v.visit(*this); }

//------------------------------------------------------------------------------

#if XTL_DEFAULT_SYNTAX == 'K' || XTL_DEFAULT_SYNTAX == 'k'

namespace mch ///< Mach7 library namespace
{
template <>         struct bindings<Shape>         { KS(Shape::m_kind); };
template <size_t N> struct bindings<shape_kind<N>> { KV(Shape,tag<N>::value); CM(0,shape_kind<N>::m_member0); CM(1,shape_kind<N>::m_member1); };
} // of namespace mch

#elif XTL_DEFAULT_SYNTAX == 'F' || XTL_DEFAULT_SYNTAX == 'f'

SKV(Shape,0); // Declare the smallest kind value for Shape hierarchy

namespace mch ///< Mach7 library namespace
{
template <>         struct bindings<Shape>         { KS(Shape::m_kind); KV(Shape,tag<NUMBER_OF_DERIVED+1>::value); };

// NOTE: We need to explicitly instantiate all bindings as otherwise our kinds 
//       associations don't get instantiated and recorded. This happens because
//       now that bindings with specific N > base cases is never instantiated
// FIX:  Wonder though why it didn't instantiate them for base cases at least since
//       we instantiate them explicitly in Case statements
#define FOR_EACH_MAX NUMBER_OF_BASES-1
#define FOR_EACH_N(N) template <> struct bindings<shape_kind<N>> { KV(Shape,tag<N>::value);  BCS(shape_kind<N>,Shape); CM(0,shape_kind<N>::m_member0); CM(1,shape_kind<N>::m_member1); };
#include "loop_over_numbers.hpp"
#undef  FOR_EACH_N
#undef  FOR_EACH_MAX

// NOTE: We need to explicitly instantiate all bindings as otherwise our kinds 
//       associations don't get instantiated and recorded. This happens because
//       now that bindings with specific N > base cases is never instantiated
#define FOR_EACH_MAX NUMBER_OF_DERIVED-NUMBER_OF_BASES-1
#define FOR_EACH_N(N) template <> struct bindings<shape_kind<NUMBER_OF_BASES+N>> { KV(Shape,tag<NUMBER_OF_BASES+N>::value);  BCS(shape_kind<NUMBER_OF_BASES+N>,shape_kind<NUMBER_OF_BASES+N>::base_type,Shape); CM(0,shape_kind<NUMBER_OF_BASES+N>::m_member0); CM(1,shape_kind<NUMBER_OF_BASES+N>::m_member1); };
#include "loop_over_numbers.hpp"
#undef  FOR_EACH_N
#undef  FOR_EACH_MAX

} // of namespace mch

#endif

//------------------------------------------------------------------------------

XTL_TIMED_FUNC_BEGIN
size_t do_match(const Shape& s, size_t)
{
    #if XTL_VISITOR_FORWARDING
        #define FOR_EACH_MAX      NUMBER_OF_BASES-1
        #define FOR_EACH_N(N)     Case(shape_kind<NUMBER_OF_BASES-1-N>)   return NUMBER_OF_BASES-1-N;
    #else
        #define FOR_EACH_MAX      NUMBER_OF_DERIVED-1
        #define FOR_EACH_N(N)     Case(shape_kind<NUMBER_OF_DERIVED-1-N>) return NUMBER_OF_DERIVED-1-N;
    #endif

    Match(s)
    {
        #include "loop_over_numbers.hpp"
    }
    EndMatch
    
    #undef  FOR_EACH_N
    #undef  FOR_EACH_MAX

    return invalid;
}
XTL_TIMED_FUNC_END

//------------------------------------------------------------------------------

XTL_TIMED_FUNC_BEGIN
size_t do_visit(const Shape& s, size_t)
{
    struct Visitor : ShapeVisitor
    {
        #if XTL_VISITOR_FORWARDING
        #define FOR_EACH_MAX  NUMBER_OF_BASES-1
        #else
        #define FOR_EACH_MAX  NUMBER_OF_DERIVED-1
        #endif
        #define FOR_EACH_N(N) virtual void visit(const shape_kind<N>&) { result = N; }
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

#if XTL_VISITOR_FORWARDING
    #define XTL_FORWARDING_STR "Yes"
#else
    #define XTL_FORWARDING_STR "No "
#endif

//------------------------------------------------------------------------------

#if XTL_DEFAULT_SYNTAX == 'P' || XTL_DEFAULT_SYNTAX == 'K' || XTL_DEFAULT_SYNTAX == 'F' || XTL_DEFAULT_SYNTAX == 'U' || XTL_DEFAULT_SYNTAX == 'E'
    #define XTL_SYNTAX_STR "Special"
#else
    #define XTL_SYNTAX_STR "Generic"
#endif

//------------------------------------------------------------------------------

#if   XTL_DEFAULT_SYNTAX == 'P' || XTL_DEFAULT_SYNTAX == 'p'
    #define XTL_ENCODING_STR "Polymorphic"
#elif XTL_DEFAULT_SYNTAX == 'K' || XTL_DEFAULT_SYNTAX == 'k'
    #define XTL_ENCODING_STR "Kind       "
#elif XTL_DEFAULT_SYNTAX == 'F' || XTL_DEFAULT_SYNTAX == 'f'
    #define XTL_ENCODING_STR "KindAdvanced"
#elif XTL_DEFAULT_SYNTAX == 'U' || XTL_DEFAULT_SYNTAX == 'u'
    #define XTL_ENCODING_STR "Union      "
#elif XTL_DEFAULT_SYNTAX == 'E' || XTL_DEFAULT_SYNTAX == 'e'
    #define XTL_ENCODING_STR "Exception  "
#else
    #define XTL_ENCODING_STR "Unknown    "
#endif

//------------------------------------------------------------------------------

#define XTL_PREFIX_STR "SYNTAX: " XTL_SYNTAX_STR " ENCODING: " XTL_ENCODING_STR " FORWARDING: " XTL_FORWARDING_STR " TEST: "

//------------------------------------------------------------------------------

#include "testvismat1.hpp"    // Utilities for timing tests

//------------------------------------------------------------------------------

int main()
{
#if   defined(XTL_REP_TEST)
    mch::verdict pr = mch::test_repetitive();
    std::cout << XTL_PREFIX_STR "Repetitive: " << pr << std::endl; 
#elif defined(XTL_SEQ_TEST)
    mch::verdict ps = mch::test_sequential();
    std::cout << XTL_PREFIX_STR "Sequential: " << ps << std::endl; 
#elif defined(XTL_RND_TEST)
    mch::verdict pn = mch::test_randomized();
    std::cout << XTL_PREFIX_STR "Randomized: " << pn << std::endl; 
#else
    #error Test scenario REP, SEQ or RND has not been chosen.
#endif
}

//------------------------------------------------------------------------------
