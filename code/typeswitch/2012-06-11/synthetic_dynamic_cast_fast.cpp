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
#include "config.hpp"
#include "ptrtools.hpp"

//------------------------------------------------------------------------------

#if !XTL_USE_MEMOIZED_CAST
    #define dynamic_cast constant_time_dynamic_cast
#endif

//------------------------------------------------------------------------------

static size_t fdc_id(size_t n);

//------------------------------------------------------------------------------

template <size_t N>
struct shape_kind : shape_kind<N/2>
{
    typedef shape_kind<N/2> base_class;
    shape_kind(size_t n = N) : base_class(n) {}
    void accept(ShapeVisitor&) const;
};

template <>
struct shape_kind<0> : OtherBase, Shape
{
    typedef Shape base_class;
    shape_kind(size_t n = 0) : base_class(n,fdc_id(n)) {}
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

enum { fdc_size = 10 };

/// Primes numbers for each level of the binary hierarchy
const size_t constant_time_dynamic_cast_primes[fdc_size][2] = 
{
    { 2, 2}, // Because the root is 2
    { 3, 5},
    { 7,11},
    {13,17},
    {19,23},
    {29,31},
    {37,41},
    {43,47},
    {53,59},
    {61,67}
};

//------------------------------------------------------------------------------

static size_t fdc_id(size_t n)
{
    XTL_ASSERT(req_bits(n) < fdc_size);
    size_t id = 1;

    if (n)
        for (size_t m = req_bits(n), i = m; i; --i)
            id *= constant_time_dynamic_cast_primes[m-i][(n & (1 << (i-1))) != 0];
    //std::cout << n << "->" << id << std::endl;
    return id;
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
inline const shape_kind<N>* constant_time_dynamic_cast_ex(const shape_kind<N>*, const Shape* u)
{
    return u->m_fdc_id % shape_ids[N] == 0
                ? static_cast<const shape_kind<N>*>(u) 
                : 0;
}

template <typename T>
inline T constant_time_dynamic_cast(const Shape* u)
{
    return constant_time_dynamic_cast_ex(static_cast<T>(0), u);
}

//------------------------------------------------------------------------------

XTL_TIMED_FUNC_BEGIN
size_t do_match(const Shape& s, size_t)
{
    if (const shape_kind< 0>* p0 = dynamic_cast<const shape_kind< 0>*>(&s))
    {
        if (const shape_kind< 1>* p1 = dynamic_cast<const shape_kind< 1>*>(p0)) 
            if (const shape_kind< 2>* p2 = dynamic_cast<const shape_kind< 2>*>(p1)) 
                if (const shape_kind< 4>* p4 = dynamic_cast<const shape_kind< 4>*>(p2)) 
                    if (const shape_kind< 8>* p8  = dynamic_cast<const shape_kind< 8>*>(p4)) 
                        if (const shape_kind<16>* p16 = dynamic_cast<const shape_kind<16>*>(p8)) 
                            if (const shape_kind<32>* p32 = dynamic_cast<const shape_kind<32>*>(p16))
                                if (const shape_kind<64>* p64 = dynamic_cast<const shape_kind<64>*>(p32))
                                    return p64->m_member7 + 64 ;
                                else
                                if (const shape_kind<65>* p65 = dynamic_cast<const shape_kind<65>*>(p32))
                                    return p65->m_member7 + 65 ;
                                else
                                    return p32->m_member7 + 32 ;
                            else
                            if (const shape_kind<33>* p33 = dynamic_cast<const shape_kind<33>*>(p16))
                                if (const shape_kind<66>* p66 = dynamic_cast<const shape_kind<66>*>(p33))
                                    return p66->m_member7 + 66 ;
                                else
                                if (const shape_kind<67>* p67 = dynamic_cast<const shape_kind<67>*>(p33))
                                    return p67->m_member7 + 67 ;
                                else
                                    return p33->m_member7 + 33 ;
                            else
                                return p16->m_member7 + 16 ;
                        else
                        if (const shape_kind<17>* p17 = dynamic_cast<const shape_kind<17>*>(p8)) 
                            if (const shape_kind<34>* p34 = dynamic_cast<const shape_kind<34>*>(p17))
                                if (const shape_kind<68>* p68 = dynamic_cast<const shape_kind<68>*>(p34))
                                    return p68->m_member7 + 68 ;
                                else
                                if (const shape_kind<69>* p69 = dynamic_cast<const shape_kind<69>*>(p34))
                                    return p69->m_member7 + 69 ;
                                else
                                    return p34->m_member7 + 34 ;
                            else
                            if (const shape_kind<35>* p35 = dynamic_cast<const shape_kind<35>*>(p17))
                                if (const shape_kind<70>* p70 = dynamic_cast<const shape_kind<70>*>(p35))
                                    return p70->m_member7 + 70 ;
                                else
                                if (const shape_kind<71>* p71 = dynamic_cast<const shape_kind<71>*>(p35))
                                    return p71->m_member7 + 71 ;
                                else
                                    return p35->m_member7 + 35 ;
                            else
                                return p17->m_member7 + 17 ;
                        else
                            return p8->m_member7 + 8 ;
                    else
                    if (const shape_kind< 9>* p9  = dynamic_cast<const shape_kind< 9>*>(p4)) 
                        if (const shape_kind<18>* p18 = dynamic_cast<const shape_kind<18>*>(p9))
                            if (const shape_kind<36>* p36 = dynamic_cast<const shape_kind<36>*>(p18))
                                if (const shape_kind<72>* p72 = dynamic_cast<const shape_kind<72>*>(p36))
                                    return p72->m_member7 + 72 ;
                                else
                                if (const shape_kind<73>* p73 = dynamic_cast<const shape_kind<73>*>(p36))
                                    return p73->m_member7 + 73 ;
                                else
                                    return p36->m_member7 + 36 ;
                            else
                            if (const shape_kind<37>* p37 = dynamic_cast<const shape_kind<37>*>(p18))
                                if (const shape_kind<74>* p74 = dynamic_cast<const shape_kind<74>*>(p37))
                                    return p74->m_member7 + 74 ;
                                else
                                if (const shape_kind<75>* p75 = dynamic_cast<const shape_kind<75>*>(p37))
                                    return p75->m_member7 + 75 ;
                                else
                                    return p37->m_member7 + 37 ;
                            else
                                return p18->m_member7 + 18 ;
                        else
                        if (const shape_kind<19>* p19 = dynamic_cast<const shape_kind<19>*>(p9)) 
                            if (const shape_kind<38>* p38 = dynamic_cast<const shape_kind<38>*>(p19))
                                if (const shape_kind<76>* p76 = dynamic_cast<const shape_kind<76>*>(p38))
                                    return p76->m_member7 + 76 ;
                                else
                                if (const shape_kind<77>* p77 = dynamic_cast<const shape_kind<77>*>(p38))
                                    return p77->m_member7 + 77 ;
                                else
                                    return p38->m_member7 + 38 ;
                            else
                            if (const shape_kind<39>* p39 = dynamic_cast<const shape_kind<39>*>(p19))
                                if (const shape_kind<78>* p78 = dynamic_cast<const shape_kind<78>*>(p39))
                                    return p78->m_member7 + 78 ;
                                else
                                if (const shape_kind<79>* p79 = dynamic_cast<const shape_kind<79>*>(p39))
                                    return p79->m_member7 + 79 ;
                                else
                                    return p39->m_member7 + 39 ;
                            else
                                return p19->m_member7 + 19 ;
                        else
                            return p9->m_member7 + 9 ;
                    else
                        return p4->m_member7 + 4 ;
                else
                if (const shape_kind< 5>* p5 = dynamic_cast<const shape_kind< 5>*>(p2)) 
                    if (const shape_kind<10>* p10 = dynamic_cast<const shape_kind<10>*>(p5)) 
                        if (const shape_kind<20>* p20 = dynamic_cast<const shape_kind<20>*>(p10))
                            if (const shape_kind<40>* p40 = dynamic_cast<const shape_kind<40>*>(p20))
                                if (const shape_kind<80>* p80 = dynamic_cast<const shape_kind<80>*>(p40))
                                    return p80->m_member7 + 80 ;
                                else
                                if (const shape_kind<81>* p81 = dynamic_cast<const shape_kind<81>*>(p40))
                                    return p81->m_member7 + 81 ;
                                else
                                    return p40->m_member7 + 40 ;
                            else
                            if (const shape_kind<41>* p41 = dynamic_cast<const shape_kind<41>*>(p20))
                                if (const shape_kind<82>* p82 = dynamic_cast<const shape_kind<82>*>(p41))
                                    return p82->m_member7 + 82 ;
                                else
                                if (const shape_kind<83>* p83 = dynamic_cast<const shape_kind<83>*>(p41))
                                    return p83->m_member7 + 83 ;
                                else
                                    return p41->m_member7 + 41 ;
                            else
                                return p20->m_member7 + 20 ;
                        else
                        if (const shape_kind<21>* p21 = dynamic_cast<const shape_kind<21>*>(p10))
                            if (const shape_kind<42>* p42 = dynamic_cast<const shape_kind<42>*>(p21))
                                if (const shape_kind<84>* p84 = dynamic_cast<const shape_kind<84>*>(p42))
                                    return p84->m_member7 + 84 ;
                                else
                                if (const shape_kind<85>* p85 = dynamic_cast<const shape_kind<85>*>(p42))
                                    return p85->m_member7 + 85 ;
                                else
                                    return p42->m_member7 + 42 ;
                            else
                            if (const shape_kind<43>* p43 = dynamic_cast<const shape_kind<43>*>(p21))
                                if (const shape_kind<86>* p86 = dynamic_cast<const shape_kind<86>*>(p43))
                                    return p86->m_member7 + 86 ;
                                else
                                if (const shape_kind<87>* p87 = dynamic_cast<const shape_kind<87>*>(p43))
                                    return p87->m_member7 + 87 ;
                                else
                                    return p43->m_member7 + 43 ;
                            else
                                return p21->m_member7 + 21 ;
                        else
                            return p10->m_member7 + 10 ;
                    else
                    if (const shape_kind<11>* p11 = dynamic_cast<const shape_kind<11>*>(p5)) 
                        if (const shape_kind<22>* p22 = dynamic_cast<const shape_kind<22>*>(p11))
                            if (const shape_kind<44>* p44 = dynamic_cast<const shape_kind<44>*>(p22))
                                if (const shape_kind<88>* p88 = dynamic_cast<const shape_kind<88>*>(p44))
                                    return p88->m_member7 + 88 ;
                                else
                                if (const shape_kind<89>* p89 = dynamic_cast<const shape_kind<89>*>(p44))
                                    return p89->m_member7 + 89 ;
                                else
                                    return p44->m_member7 + 44 ;
                            else
                            if (const shape_kind<45>* p45 = dynamic_cast<const shape_kind<45>*>(p22))
                                if (const shape_kind<90>* p90 = dynamic_cast<const shape_kind<90>*>(p45))
                                    return p90->m_member7 + 90 ;
                                else
                                if (const shape_kind<91>* p91 = dynamic_cast<const shape_kind<91>*>(p45))
                                    return p91->m_member7 + 91 ;
                                else
                                    return p45->m_member7 + 45 ;
                            else
                                return p22->m_member7 + 22 ;
                        else
                        if (const shape_kind<23>* p23 = dynamic_cast<const shape_kind<23>*>(p11))
                            if (const shape_kind<46>* p46 = dynamic_cast<const shape_kind<46>*>(p23))
                                if (const shape_kind<92>* p92 = dynamic_cast<const shape_kind<92>*>(p46))
                                    return p92->m_member7 + 92 ;
                                else
                                if (const shape_kind<93>* p93 = dynamic_cast<const shape_kind<93>*>(p46))
                                    return p93->m_member7 + 93 ;
                                else
                                    return p46->m_member7 + 46 ;
                            else
                            if (const shape_kind<47>* p47 = dynamic_cast<const shape_kind<47>*>(p23))
                                if (const shape_kind<94>* p94 = dynamic_cast<const shape_kind<94>*>(p47))
                                    return p94->m_member7 + 94 ;
                                else
                                if (const shape_kind<95>* p95 = dynamic_cast<const shape_kind<95>*>(p47))
                                    return p95->m_member7 + 95 ;
                                else
                                    return p47->m_member7 + 47 ;
                            else
                                return p23->m_member7 + 23 ;
                        else
                            return p11->m_member7 + 11 ;
                    else
                        return p5->m_member7 + 5 ;
                else
                    return p2->m_member7 + 2 ;
            else
            if (const shape_kind< 3>* p3 = dynamic_cast<const shape_kind< 3>*>(p1)) 
                if (const shape_kind< 6>* p6 = dynamic_cast<const shape_kind< 6>*>(p3)) 
                    if (const shape_kind<12>* p12 = dynamic_cast<const shape_kind<12>*>(p6)) 
                        if (const shape_kind<24>* p24 = dynamic_cast<const shape_kind<24>*>(p12))
                            if (const shape_kind<48>* p48 = dynamic_cast<const shape_kind<48>*>(p24))
                                if (const shape_kind<96>* p96 = dynamic_cast<const shape_kind<96>*>(p48))
                                    return p96->m_member7 + 96 ;
                                else
                                if (const shape_kind<97>* p97 = dynamic_cast<const shape_kind<97>*>(p48))
                                    return p97->m_member7 + 97 ;
                                else
                                    return p48->m_member7 + 48 ;
                            else
                            if (const shape_kind<49>* p49 = dynamic_cast<const shape_kind<49>*>(p24))
                                if (const shape_kind<98>* p98 = dynamic_cast<const shape_kind<98>*>(p49))
                                    return p98->m_member7 + 98 ;
                                else
                                if (const shape_kind<99>* p99 = dynamic_cast<const shape_kind<99>*>(p49))
                                    return p99->m_member7 + 99 ;
                                else
                                    return p49->m_member7 + 49 ;
                            else
                                return p24->m_member7 + 24 ;
                        else
                        if (const shape_kind<25>* p25 = dynamic_cast<const shape_kind<25>*>(p12))
                            if (const shape_kind<50>* p50 = dynamic_cast<const shape_kind<50>*>(p25))
                                return p50->m_member7 + 50 ;
                            else
                            if (const shape_kind<51>* p51 = dynamic_cast<const shape_kind<51>*>(p25))
                                return p51->m_member7 + 51 ;
                            else
                                return p25->m_member7 + 25 ;
                        else
                            return p12->m_member7 + 12 ;
                    else
                    if (const shape_kind<13>* p13 = dynamic_cast<const shape_kind<13>*>(p6)) 
                        if (const shape_kind<26>* p26 = dynamic_cast<const shape_kind<26>*>(p13))
                            if (const shape_kind<52>* p52 = dynamic_cast<const shape_kind<52>*>(p26))
                                return p52->m_member7 + 52 ;
                            else
                            if (const shape_kind<53>* p53 = dynamic_cast<const shape_kind<53>*>(p26))
                                return p53->m_member7 + 53 ;
                            else
                                return p26->m_member7 + 26 ;
                        else
                        if (const shape_kind<27>* p27 = dynamic_cast<const shape_kind<27>*>(p13))
                            if (const shape_kind<54>* p54 = dynamic_cast<const shape_kind<54>*>(p27))
                                return p54->m_member7 + 54 ;
                            else
                            if (const shape_kind<55>* p55 = dynamic_cast<const shape_kind<55>*>(p27))
                                return p55->m_member7 + 55 ;
                            else
                                return p27->m_member7 + 27 ;
                        else
                            return p13->m_member7 + 13 ;
                    else
                        return p6->m_member7 + 6 ;
                else
                if (const shape_kind< 7>* p7 = dynamic_cast<const shape_kind< 7>*>(p3)) 
                    if (const shape_kind<14>* p14 = dynamic_cast<const shape_kind<14>*>(p7)) 
                        if (const shape_kind<28>* p28 = dynamic_cast<const shape_kind<28>*>(p14))
                            if (const shape_kind<56>* p56 = dynamic_cast<const shape_kind<56>*>(p28))
                                return p56->m_member7 + 56 ;
                            else
                            if (const shape_kind<57>* p57 = dynamic_cast<const shape_kind<57>*>(p28))
                                return p57->m_member7 + 57 ;
                            else
                                return p28->m_member7 + 28 ;
                        else
                        if (const shape_kind<29>* p29 = dynamic_cast<const shape_kind<29>*>(p14))
                            if (const shape_kind<58>* p58 = dynamic_cast<const shape_kind<58>*>(p29))
                                return p58->m_member7 + 58 ;
                            else
                            if (const shape_kind<59>* p59 = dynamic_cast<const shape_kind<59>*>(p29))
                                return p59->m_member7 + 59 ;
                            else
                                return p29->m_member7 + 29 ;
                        else
                            return p14->m_member7 + 14 ;
                    else
                    if (const shape_kind<15>* p15 = dynamic_cast<const shape_kind<15>*>(p7)) 
                        if (const shape_kind<30>* p30 = dynamic_cast<const shape_kind<30>*>(p15))
                            if (const shape_kind<60>* p60 = dynamic_cast<const shape_kind<60>*>(p30))
                                return p60->m_member7 + 60 ;
                            else
                            if (const shape_kind<61>* p61 = dynamic_cast<const shape_kind<61>*>(p30))
                                return p61->m_member7 + 61 ;
                            else
                                return p30->m_member7 + 30 ;
                        else
                        if (const shape_kind<31>* p31 = dynamic_cast<const shape_kind<31>*>(p15))
                            if (const shape_kind<62>* p62 = dynamic_cast<const shape_kind<62>*>(p31))
                                return p62->m_member7 + 62 ;
                            else
                            if (const shape_kind<63>* p63 = dynamic_cast<const shape_kind<63>*>(p31))
                                return p63->m_member7 + 63 ;
                            else
                                return p31->m_member7 + 31 ;
                        else
                            return p15->m_member7 + 15 ;
                    else
                        return p7->m_member7 + 7 ;
                else
                    return p3->m_member7 + 3 ;
            else
                return p1->m_member7 + 1 ;
        else
            return p0->m_member7 + 0 ;
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

#include "testvismat.hpp"    // Utilities for timing tests

//------------------------------------------------------------------------------

int main()
{
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
