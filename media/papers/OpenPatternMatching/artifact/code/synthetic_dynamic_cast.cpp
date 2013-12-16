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

#if !XTL_USE_MEMOIZED_CAST
    #define dynamic_cast fast_dynamic_cast
#endif

//------------------------------------------------------------------------------

static size_t fdc_id(size_t n);

//------------------------------------------------------------------------------

template <size_t N>
struct shape_kind : OtherBase, Shape
{
    typedef Shape base_class;
    shape_kind(size_t n = N) : base_class(n,fdc_id(n)) {}
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

static const size_t primes[] = {
   2,   3,   5,   7,  11,  13,  17,  19,  23,  29, 
  31,  37,  41,  43,  47,  53,  59,  61,  67,  71, 
  73,  79,  83,  89,  97, 101, 103, 107, 109, 113, 
 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 
 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 
 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 
 283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 
 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 
 419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 
 467, 479, 487, 491, 499, 503, 509, 521, 523, 541, 
 547, 557, 563, 569, 571, 577, 587, 593, 599, 601, 
 607, 613, 617, 619, 631, 641, 643, 647, 653, 659, 
};

//------------------------------------------------------------------------------

static size_t fdc_id(size_t n)
{
    return 2*primes[n+1];
}

//------------------------------------------------------------------------------

inline size_t id(size_t n) { return fdc_id(n); }

const size_t shape_ids[100] =
{
     id( 0), id( 1), id( 2), id( 3), id( 4), id( 5), id( 6), id( 7), id( 8), id( 9),
     id(10), id(11), id(12), id(13), id(14), id(15), id(16), id(17), id(18), id(19),
     id(20), id(21), id(22), id(23), id(24), id(25), id(26), id(27), id(28), id(29),
     id(30), id(31), id(32), id(33), id(34), id(35), id(36), id(37), id(38), id(39),
     id(40), id(41), id(42), id(43), id(44), id(45), id(46), id(47), id(48), id(49),
     id(50), id(51), id(52), id(53), id(54), id(55), id(56), id(57), id(58), id(59),
     id(60), id(61), id(62), id(63), id(64), id(65), id(66), id(67), id(68), id(69),
     id(70), id(71), id(72), id(73), id(74), id(75), id(76), id(77), id(78), id(79),
     id(80), id(81), id(82), id(83), id(84), id(85), id(86), id(87), id(88), id(89),
     id(90), id(91), id(92), id(93), id(94), id(95), id(96), id(97), id(98), id(99),
};

//------------------------------------------------------------------------------

template <size_t N>
inline const shape_kind<N>* fast_dynamic_cast_ex(const shape_kind<N>*, const Shape* u)
{
    return u->m_fdc_id % shape_ids[N] == 0
                ? static_cast<const shape_kind<N>*>(u) 
                : 0;
}

template <typename T>
inline T fast_dynamic_cast(const Shape* u)
{
    return fast_dynamic_cast_ex(static_cast<T>(0), u);
}

//------------------------------------------------------------------------------

#if 1
XTL_TIMED_FUNC_BEGIN
size_t do_match(const Shape& s, size_t)
{
    #define FOR_EACH_MAX  NUMBER_OF_DERIVED-1
    #define FOR_EACH_N(N) if (/*const shape_kind<N>* p =*/ dynamic_cast<const shape_kind<N>*>(&s)) return N/*+p->m_member1*/;
    #include "loop_over_numbers.hpp"
    #undef  FOR_EACH_N
    #undef  FOR_EACH_MAX
    return invalid;
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
    using namespace mch; // Mach7's library namespace

    verdict pp = test_repetitive();
//    verdict ps = test_sequential();
//    verdict pr = test_randomized();
    std::cout << "OVERALL: "
              << "Repetitive: " << pp << "; "
//              << "Sequential: " << ps << "; "
//              << "Random: "     << pr 
              << std::endl; 
}

//------------------------------------------------------------------------------
