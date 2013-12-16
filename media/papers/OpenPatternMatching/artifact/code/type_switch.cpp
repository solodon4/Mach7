///
/// \file
///
/// This file is a part of pattern matching testing suite.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of Mach7 library (http://parasol.tamu.edu/mach7/).
/// Copyright (C) 2011-2012 Texas A&M University.
/// All rights reserved.
///

#include "testshape.hpp"
#include "type_switch.hpp"        // Slower by now than the above

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

XTL_TIMED_FUNC_BEGIN
size_t do_match(const Shape& s, size_t)
{
    Match(s)
    {
        #define FOR_EACH_MAX  NUMBER_OF_DERIVED-1
        #define FOR_EACH_N(N) Case(shape_kind<N>) return N;
        #include "loop_over_numbers.hpp"
        #undef  FOR_EACH_N
        #undef  FOR_EACH_MAX
    }
    EndMatch
    return invalid;
}
XTL_TIMED_FUNC_END

//------------------------------------------------------------------------------
#if 1
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
#else
size_t do_visit(const Shape& s, size_t)
{
    struct Visitor : ShapeVisitor
    {
        virtual void visit(const shape_kind<0>&) 
        {
            result = 0;
        }
        virtual void visit(const shape_kind<1>&) 
        {
            result = 1;
        }
        virtual void visit(const shape_kind<2>&) 
        {
            result = 2;
        }
        virtual void visit(const shape_kind<3>&) 
        {
            result = 3;
        }
        virtual void visit(const shape_kind<4>&) 
        {
            result = 4;
        }
        virtual void visit(const shape_kind<5>&) 
        {
            result = 5;
        }
        virtual void visit(const shape_kind<6>&) 
        {
            result = 6;
        }
        virtual void visit(const shape_kind<7>&) 
        {
            result = 7;
        }
        virtual void visit(const shape_kind<8>&) 
        {
            result = 8;
        }
        virtual void visit(const shape_kind<9>&) 
        {
            result = 9;
        }
        virtual void visit(const shape_kind<10>&) 
        {
            result = 10;
        }
        virtual void visit(const shape_kind<11>&) 
        {
            result = 11;
        }
        virtual void visit(const shape_kind<12>&) 
        {
            result = 12;
        }
        virtual void visit(const shape_kind<13>&) 
        {
            result = 13;
        }
        virtual void visit(const shape_kind<14>&) 
        {
            result = 14;
        }
        virtual void visit(const shape_kind<15>&) 
        {
            result = 15;
        }
        virtual void visit(const shape_kind<16>&) 
        {
            result = 16;
        }
        virtual void visit(const shape_kind<17>&) 
        {
            result = 17;
        }
        virtual void visit(const shape_kind<18>&) 
        {
            result = 18;
        }
        virtual void visit(const shape_kind<19>&) 
        {
            result = 19;
        }
        virtual void visit(const shape_kind<20>&) 
        {
            result = 20;
        }
        virtual void visit(const shape_kind<21>&) 
        {
            result = 21;
        }
        virtual void visit(const shape_kind<22>&) 
        {
            result = 22;
        }
        virtual void visit(const shape_kind<23>&) 
        {
            result = 23;
        }
        virtual void visit(const shape_kind<24>&) 
        {
            result = 24;
        }
        virtual void visit(const shape_kind<25>&) 
        {
            result = 25;
        }
        virtual void visit(const shape_kind<26>&) 
        {
            result = 26;
        }
        virtual void visit(const shape_kind<27>&) 
        {
            result = 27;
        }
        virtual void visit(const shape_kind<28>&) 
        {
            result = 28;
        }
        virtual void visit(const shape_kind<29>&) 
        {
            result = 29;
        }
        virtual void visit(const shape_kind<30>&) 
        {
            result = 30;
        }
        virtual void visit(const shape_kind<31>&) 
        {
            result = 31;
        }
        virtual void visit(const shape_kind<32>&) 
        {
            result = 32;
        }
        virtual void visit(const shape_kind<33>&) 
        {
            result = 33;
        }
        virtual void visit(const shape_kind<34>&) 
        {
            result = 34;
        }
        virtual void visit(const shape_kind<35>&) 
        {
            result = 35;
        }
        virtual void visit(const shape_kind<36>&) 
        {
            result = 36;
        }
        virtual void visit(const shape_kind<37>&) 
        {
            result = 37;
        }
        virtual void visit(const shape_kind<38>&) 
        {
            result = 38;
        }
        virtual void visit(const shape_kind<39>&) 
        {
            result = 39;
        }
        virtual void visit(const shape_kind<40>&) 
        {
            result = 40;
        }
        virtual void visit(const shape_kind<41>&) 
        {
            result = 41;
        }
        virtual void visit(const shape_kind<42>&) 
        {
            result = 42;
        }
        virtual void visit(const shape_kind<43>&) 
        {
            result = 43;
        }
        virtual void visit(const shape_kind<44>&) 
        {
            result = 44;
        }
        virtual void visit(const shape_kind<45>&) 
        {
            result = 45;
        }
        virtual void visit(const shape_kind<46>&) 
        {
            result = 46;
        }
        virtual void visit(const shape_kind<47>&) 
        {
            result = 47;
        }
        virtual void visit(const shape_kind<48>&) 
        {
            result = 48;
        }
        virtual void visit(const shape_kind<49>&) 
        {
            result = 49;
        }
        virtual void visit(const shape_kind<50>&) 
        {
            result = 50;
        }
        virtual void visit(const shape_kind<51>&) 
        {
            result = 51;
        }
        virtual void visit(const shape_kind<52>&) 
        {
            result = 52;
        }
        virtual void visit(const shape_kind<53>&) 
        {
            result = 53;
        }
        virtual void visit(const shape_kind<54>&) 
        {
            result = 54;
        }
        virtual void visit(const shape_kind<55>&) 
        {
            result = 55;
        }
        virtual void visit(const shape_kind<56>&) 
        {
            result = 56;
        }
        virtual void visit(const shape_kind<57>&) 
        {
            result = 57;
        }
        virtual void visit(const shape_kind<58>&) 
        {
            result = 58;
        }
        virtual void visit(const shape_kind<59>&) 
        {
            result = 59;
        }
        virtual void visit(const shape_kind<60>&) 
        {
            result = 60;
        }
        virtual void visit(const shape_kind<61>&) 
        {
            result = 61;
        }
        virtual void visit(const shape_kind<62>&) 
        {
            result = 62;
        }
        virtual void visit(const shape_kind<63>&) 
        {
            result = 63;
        }
        virtual void visit(const shape_kind<64>&) 
        {
            result = 64;
        }
        virtual void visit(const shape_kind<65>&) 
        {
            result = 65;
        }
        virtual void visit(const shape_kind<66>&) 
        {
            result = 66;
        }
        virtual void visit(const shape_kind<67>&) 
        {
            result = 67;
        }
        virtual void visit(const shape_kind<68>&) 
        {
            result = 68;
        }
        virtual void visit(const shape_kind<69>&) 
        {
            result = 69;
        }
        virtual void visit(const shape_kind<70>&) 
        {
            result = 70;
        }
        virtual void visit(const shape_kind<71>&) 
        {
            result = 71;
        }
        virtual void visit(const shape_kind<72>&) 
        {
            result = 72;
        }
        virtual void visit(const shape_kind<73>&) 
        {
            result = 73;
        }
        virtual void visit(const shape_kind<74>&) 
        {
            result = 74;
        }
        virtual void visit(const shape_kind<75>&) 
        {
            result = 75;
        }
        virtual void visit(const shape_kind<76>&) 
        {
            result = 76;
        }
        virtual void visit(const shape_kind<77>&) 
        {
            result = 77;
        }
        virtual void visit(const shape_kind<78>&) 
        {
            result = 78;
        }
        virtual void visit(const shape_kind<79>&) 
        {
            result = 79;
        }
        virtual void visit(const shape_kind<80>&) 
        {
            result = 80;
        }
        virtual void visit(const shape_kind<81>&) 
        {
            result = 81;
        }
        virtual void visit(const shape_kind<82>&) 
        {
            result = 82;
        }
        virtual void visit(const shape_kind<83>&) 
        {
            result = 83;
        }
        virtual void visit(const shape_kind<84>&) 
        {
            result = 84;
        }
        virtual void visit(const shape_kind<85>&) 
        {
            result = 85;
        }
        virtual void visit(const shape_kind<86>&) 
        {
            result = 86;
        }
        virtual void visit(const shape_kind<87>&) 
        {
            result = 87;
        }
        virtual void visit(const shape_kind<88>&) 
        {
            result = 88;
        }
        virtual void visit(const shape_kind<89>&) 
        {
            result = 89;
        }
        virtual void visit(const shape_kind<90>&) 
        {
            result = 90;
        }
        virtual void visit(const shape_kind<91>&) 
        {
            result = 91;
        }
        virtual void visit(const shape_kind<92>&) 
        {
            result = 92;
        }
        virtual void visit(const shape_kind<93>&) 
        {
            result = 93;
        }
        virtual void visit(const shape_kind<94>&) 
        {
            result = 94;
        }
        virtual void visit(const shape_kind<95>&) 
        {
            result = 95;
        }
        virtual void visit(const shape_kind<96>&) 
        {
            result = 96;
        }
        virtual void visit(const shape_kind<97>&) 
        {
            result = 97;
        }
        virtual void visit(const shape_kind<98>&) 
        {
            result = 98;
        }
        virtual void visit(const shape_kind<99>&) 
        {
            result = 99;
        }
        size_t result;
    };
    Visitor v;
    v.result = invalid;
    s.accept(v);
    return v.result;
}
#endif
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
    verdict ps = test_sequential();
    verdict pr = test_randomized();
    std::cout << "OVERALL: "
              << "Repetitive: " << pp << "; "
              << "Sequential: " << ps << "; "
              << "Random: "     << pr 
              << std::endl; 
}

//------------------------------------------------------------------------------
