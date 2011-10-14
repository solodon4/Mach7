#include <algorithm>
#include <cmath>
#include <fstream>
#include <iterator>
#include <iostream>
#include <iomanip>
#include <numeric>
#include <string>
#include <vector>
#include "match.hpp"
#include "timing.hpp"

#if !defined(NUMBER_OF_VFUNCS)
#define NUMBER_OF_VFUNCS  1
#endif
#if !defined(NUMBER_OF_DERIVED)
#define NUMBER_OF_DERIVED 100
#endif

struct ShapeVisitor;

struct OtherBase
{
    OtherBase() : m_foo(0xAAAAAAAA) {}
    virtual int foo() { return m_foo; };

    int m_foo;
};

struct Shape
{
    virtual ~Shape() {}
    virtual void accept(ShapeVisitor&) const = 0;
    #define FOR_EACH_MAX NUMBER_OF_VFUNCS-2
    #define FOR_EACH_N(N) virtual void foo ## N() {}
    #include "loop_over_numbers.hpp"
    #undef  FOR_EACH_N
    #undef  FOR_EACH_MAX
};

template <size_t N>
struct shape_kind : OtherBase, Shape
{
    void accept(ShapeVisitor& v) const;
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

struct ShapeVisitor
{
    #define FOR_EACH_MAX NUMBER_OF_DERIVED-1
    #define FOR_EACH_N(N) virtual void visit(const shape_kind<N>&) {}
    #include "loop_over_numbers.hpp"
    #undef  FOR_EACH_N
    #undef  FOR_EACH_MAX
};

template <size_t N> void shape_kind<N>::accept(ShapeVisitor& v) const { v.visit(*this); }

#if 1
DO_NOT_INLINE_BEGIN
size_t do_match(const Shape& s)
{
    SWITCH_N(s,NUMBER_OF_DERIVED)
    {
    #define FOR_EACH_MAX      NUMBER_OF_DERIVED-1
    #define FOR_EACH_PRELUDE  CASES_BEGIN
    #define FOR_EACH_N(N)     CASE(shape_kind<N>) return N;
    #define FOR_EACH_POSTLUDE CASES_END
    #include "loop_over_numbers.hpp"
    #undef  FOR_EACH_POSTLUDE
    #undef  FOR_EACH_N
    #undef  FOR_EACH_PRELUDE
    #undef  FOR_EACH_MAX
    }
    return -1;
}
DO_NOT_INLINE_END
#else
DO_NOT_INLINE_BEGIN
size_t do_match(const Shape& s)
{
    static vtblmap<type_switch_info&,requires_bits<100-1+1>::value> __vtbl2lines_map; 
    auto const        __selector_ptr = addr(s); 
    const void*       __casted_ptr; 
    type_switch_info& __switch_info = __vtbl2lines_map.get(__selector_ptr); 
    switch (__switch_info.line)
    {
    default: {
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<0>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 10; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 10:
                 if ((match<shape_kind<0>>()(adjust_ptr<shape_kind<0>>(__selector_ptr,__switch_info.offset))))
                     return 0;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<1>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 13; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 13:
                 if ((match<shape_kind<1>>()(adjust_ptr<shape_kind<1>>(__selector_ptr,__switch_info.offset))))
                     return 1;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<2>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 16; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 16:
                 if ((match<shape_kind<2>>()(adjust_ptr<shape_kind<2>>(__selector_ptr,__switch_info.offset))))
                     return 2;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<3>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 19; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 19:
                 if ((match<shape_kind<3>>()(adjust_ptr<shape_kind<3>>(__selector_ptr,__switch_info.offset))))
                     return 3;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<4>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 22; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 22:
                 if ((match<shape_kind<4>>()(adjust_ptr<shape_kind<4>>(__selector_ptr,__switch_info.offset))))
                     return 4;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<5>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 25; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 25:
                 if ((match<shape_kind<5>>()(adjust_ptr<shape_kind<5>>(__selector_ptr,__switch_info.offset))))
                     return 5;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<6>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 28; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 28:
                 if ((match<shape_kind<6>>()(adjust_ptr<shape_kind<6>>(__selector_ptr,__switch_info.offset))))
                     return 6;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<7>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 31; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 31:
                 if ((match<shape_kind<7>>()(adjust_ptr<shape_kind<7>>(__selector_ptr,__switch_info.offset))))
                     return 7;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<8>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 34; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 34:
                 if ((match<shape_kind<8>>()(adjust_ptr<shape_kind<8>>(__selector_ptr,__switch_info.offset))))
                     return 8;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<9>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 37; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 37:
                 if ((match<shape_kind<9>>()(adjust_ptr<shape_kind<9>>(__selector_ptr,__switch_info.offset))))
                     return 9;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<10>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 40; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 40:
                 if ((match<shape_kind<10>>()(adjust_ptr<shape_kind<10>>(__selector_ptr,__switch_info.offset))))
                     return 10;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<11>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 43; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 43:
                 if ((match<shape_kind<11>>()(adjust_ptr<shape_kind<11>>(__selector_ptr,__switch_info.offset))))
                     return 11;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<12>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 46; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 46:
                 if ((match<shape_kind<12>>()(adjust_ptr<shape_kind<12>>(__selector_ptr,__switch_info.offset))))
                     return 12;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<13>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 49; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 49:
                 if ((match<shape_kind<13>>()(adjust_ptr<shape_kind<13>>(__selector_ptr,__switch_info.offset))))
                     return 13;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<14>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 52; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 52:
                 if ((match<shape_kind<14>>()(adjust_ptr<shape_kind<14>>(__selector_ptr,__switch_info.offset))))
                     return 14;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<15>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 55; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 55:
                 if ((match<shape_kind<15>>()(adjust_ptr<shape_kind<15>>(__selector_ptr,__switch_info.offset))))
                     return 15;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<16>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 58; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 58:
                 if ((match<shape_kind<16>>()(adjust_ptr<shape_kind<16>>(__selector_ptr,__switch_info.offset))))
                     return 16;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<17>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 61; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 61:
                 if ((match<shape_kind<17>>()(adjust_ptr<shape_kind<17>>(__selector_ptr,__switch_info.offset))))
                     return 17;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<18>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 64; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 64:
                 if ((match<shape_kind<18>>()(adjust_ptr<shape_kind<18>>(__selector_ptr,__switch_info.offset))))
                     return 18;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<19>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 67; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 67:
                 if ((match<shape_kind<19>>()(adjust_ptr<shape_kind<19>>(__selector_ptr,__switch_info.offset))))
                     return 19;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<20>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 70; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 70:
                 if ((match<shape_kind<20>>()(adjust_ptr<shape_kind<20>>(__selector_ptr,__switch_info.offset))))
                     return 20;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<21>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 73; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 73:
                 if ((match<shape_kind<21>>()(adjust_ptr<shape_kind<21>>(__selector_ptr,__switch_info.offset))))
                     return 21;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<22>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 76; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 76:
                 if ((match<shape_kind<22>>()(adjust_ptr<shape_kind<22>>(__selector_ptr,__switch_info.offset))))
                     return 22;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<23>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 79; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 79:
                 if ((match<shape_kind<23>>()(adjust_ptr<shape_kind<23>>(__selector_ptr,__switch_info.offset))))
                     return 23;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<24>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 82; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 82:
                 if ((match<shape_kind<24>>()(adjust_ptr<shape_kind<24>>(__selector_ptr,__switch_info.offset))))
                     return 24;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<25>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 85; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 85:
                 if ((match<shape_kind<25>>()(adjust_ptr<shape_kind<25>>(__selector_ptr,__switch_info.offset))))
                     return 25;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<26>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 88; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 88:
                 if ((match<shape_kind<26>>()(adjust_ptr<shape_kind<26>>(__selector_ptr,__switch_info.offset))))
                     return 26;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<27>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 91; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 91:
                 if ((match<shape_kind<27>>()(adjust_ptr<shape_kind<27>>(__selector_ptr,__switch_info.offset))))
                     return 27;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<28>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 94; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 94:
                 if ((match<shape_kind<28>>()(adjust_ptr<shape_kind<28>>(__selector_ptr,__switch_info.offset))))
                     return 28;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<29>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 97; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 97:
                 if ((match<shape_kind<29>>()(adjust_ptr<shape_kind<29>>(__selector_ptr,__switch_info.offset))))
                     return 29;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<30>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 100; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 100:
                 if ((match<shape_kind<30>>()(adjust_ptr<shape_kind<30>>(__selector_ptr,__switch_info.offset))))
                     return 30;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<31>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 103; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 103:
                 if ((match<shape_kind<31>>()(adjust_ptr<shape_kind<31>>(__selector_ptr,__switch_info.offset))))
                     return 31;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<32>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 106; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 106:
                 if ((match<shape_kind<32>>()(adjust_ptr<shape_kind<32>>(__selector_ptr,__switch_info.offset))))
                     return 32;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<33>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 109; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 109:
                 if ((match<shape_kind<33>>()(adjust_ptr<shape_kind<33>>(__selector_ptr,__switch_info.offset))))
                     return 33;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<34>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 112; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 112:
                 if ((match<shape_kind<34>>()(adjust_ptr<shape_kind<34>>(__selector_ptr,__switch_info.offset))))
                     return 34;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<35>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 115; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 115:
                 if ((match<shape_kind<35>>()(adjust_ptr<shape_kind<35>>(__selector_ptr,__switch_info.offset))))
                     return 35;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<36>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 118; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 118:
                 if ((match<shape_kind<36>>()(adjust_ptr<shape_kind<36>>(__selector_ptr,__switch_info.offset))))
                     return 36;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<37>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 121; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 121:
                 if ((match<shape_kind<37>>()(adjust_ptr<shape_kind<37>>(__selector_ptr,__switch_info.offset))))
                     return 37;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<38>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 124; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 124:
                 if ((match<shape_kind<38>>()(adjust_ptr<shape_kind<38>>(__selector_ptr,__switch_info.offset))))
                     return 38;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<39>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 127; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 127:
                 if ((match<shape_kind<39>>()(adjust_ptr<shape_kind<39>>(__selector_ptr,__switch_info.offset))))
                     return 39;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<40>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 130; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 130:
                 if ((match<shape_kind<40>>()(adjust_ptr<shape_kind<40>>(__selector_ptr,__switch_info.offset))))
                     return 40;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<41>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 133; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 133:
                 if ((match<shape_kind<41>>()(adjust_ptr<shape_kind<41>>(__selector_ptr,__switch_info.offset))))
                     return 41;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<42>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 136; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 136:
                 if ((match<shape_kind<42>>()(adjust_ptr<shape_kind<42>>(__selector_ptr,__switch_info.offset))))
                     return 42;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<43>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 139; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 139:
                 if ((match<shape_kind<43>>()(adjust_ptr<shape_kind<43>>(__selector_ptr,__switch_info.offset))))
                     return 43;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<44>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 142; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 142:
                 if ((match<shape_kind<44>>()(adjust_ptr<shape_kind<44>>(__selector_ptr,__switch_info.offset))))
                     return 44;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<45>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 145; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 145:
                 if ((match<shape_kind<45>>()(adjust_ptr<shape_kind<45>>(__selector_ptr,__switch_info.offset))))
                     return 45;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<46>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 148; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 148:
                 if ((match<shape_kind<46>>()(adjust_ptr<shape_kind<46>>(__selector_ptr,__switch_info.offset))))
                     return 46;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<47>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 151; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 151:
                 if ((match<shape_kind<47>>()(adjust_ptr<shape_kind<47>>(__selector_ptr,__switch_info.offset))))
                     return 47;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<48>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 154; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 154:
                 if ((match<shape_kind<48>>()(adjust_ptr<shape_kind<48>>(__selector_ptr,__switch_info.offset))))
                     return 48;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<49>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 157; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 157:
                 if ((match<shape_kind<49>>()(adjust_ptr<shape_kind<49>>(__selector_ptr,__switch_info.offset))))
                     return 49;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<50>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 160; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 160:
                 if ((match<shape_kind<50>>()(adjust_ptr<shape_kind<50>>(__selector_ptr,__switch_info.offset))))
                     return 50;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<51>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 163; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 163:
                 if ((match<shape_kind<51>>()(adjust_ptr<shape_kind<51>>(__selector_ptr,__switch_info.offset))))
                     return 51;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<52>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 166; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 166:
                 if ((match<shape_kind<52>>()(adjust_ptr<shape_kind<52>>(__selector_ptr,__switch_info.offset))))
                     return 52;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<53>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 169; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 169:
                 if ((match<shape_kind<53>>()(adjust_ptr<shape_kind<53>>(__selector_ptr,__switch_info.offset))))
                     return 53;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<54>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 172; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 172:
                 if ((match<shape_kind<54>>()(adjust_ptr<shape_kind<54>>(__selector_ptr,__switch_info.offset))))
                     return 54;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<55>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 175; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 175:
                 if ((match<shape_kind<55>>()(adjust_ptr<shape_kind<55>>(__selector_ptr,__switch_info.offset))))
                     return 55;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<56>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 178; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 178:
                 if ((match<shape_kind<56>>()(adjust_ptr<shape_kind<56>>(__selector_ptr,__switch_info.offset))))
                     return 56;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<57>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 181; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 181:
                 if ((match<shape_kind<57>>()(adjust_ptr<shape_kind<57>>(__selector_ptr,__switch_info.offset))))
                     return 57;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<58>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 184; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 184:
                 if ((match<shape_kind<58>>()(adjust_ptr<shape_kind<58>>(__selector_ptr,__switch_info.offset))))
                     return 58;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<59>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 187; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 187:
                 if ((match<shape_kind<59>>()(adjust_ptr<shape_kind<59>>(__selector_ptr,__switch_info.offset))))
                     return 59;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<60>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 190; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 190:
                 if ((match<shape_kind<60>>()(adjust_ptr<shape_kind<60>>(__selector_ptr,__switch_info.offset))))
                     return 60;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<61>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 193; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 193:
                 if ((match<shape_kind<61>>()(adjust_ptr<shape_kind<61>>(__selector_ptr,__switch_info.offset))))
                     return 61;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<62>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 196; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 196:
                 if ((match<shape_kind<62>>()(adjust_ptr<shape_kind<62>>(__selector_ptr,__switch_info.offset))))
                     return 62;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<63>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 199; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 199:
                 if ((match<shape_kind<63>>()(adjust_ptr<shape_kind<63>>(__selector_ptr,__switch_info.offset))))
                     return 63;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<64>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 202; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 202:
                 if ((match<shape_kind<64>>()(adjust_ptr<shape_kind<64>>(__selector_ptr,__switch_info.offset))))
                     return 64;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<65>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 205; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 205:
                 if ((match<shape_kind<65>>()(adjust_ptr<shape_kind<65>>(__selector_ptr,__switch_info.offset))))
                     return 65;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<66>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 208; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 208:
                 if ((match<shape_kind<66>>()(adjust_ptr<shape_kind<66>>(__selector_ptr,__switch_info.offset))))
                     return 66;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<67>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 211; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 211:
                 if ((match<shape_kind<67>>()(adjust_ptr<shape_kind<67>>(__selector_ptr,__switch_info.offset))))
                     return 67;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<68>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 214; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 214:
                 if ((match<shape_kind<68>>()(adjust_ptr<shape_kind<68>>(__selector_ptr,__switch_info.offset))))
                     return 68;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<69>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 217; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 217:
                 if ((match<shape_kind<69>>()(adjust_ptr<shape_kind<69>>(__selector_ptr,__switch_info.offset))))
                     return 69;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<70>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 220; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 220:
                 if ((match<shape_kind<70>>()(adjust_ptr<shape_kind<70>>(__selector_ptr,__switch_info.offset))))
                     return 70;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<71>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 223; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 223:
                 if ((match<shape_kind<71>>()(adjust_ptr<shape_kind<71>>(__selector_ptr,__switch_info.offset))))
                     return 71;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<72>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 226; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 226:
                 if ((match<shape_kind<72>>()(adjust_ptr<shape_kind<72>>(__selector_ptr,__switch_info.offset))))
                     return 72;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<73>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 229; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 229:
                 if ((match<shape_kind<73>>()(adjust_ptr<shape_kind<73>>(__selector_ptr,__switch_info.offset))))
                     return 73;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<74>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 232; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 232:
                 if ((match<shape_kind<74>>()(adjust_ptr<shape_kind<74>>(__selector_ptr,__switch_info.offset))))
                     return 74;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<75>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 235; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 235:
                 if ((match<shape_kind<75>>()(adjust_ptr<shape_kind<75>>(__selector_ptr,__switch_info.offset))))
                     return 75;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<76>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 238; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 238:
                 if ((match<shape_kind<76>>()(adjust_ptr<shape_kind<76>>(__selector_ptr,__switch_info.offset))))
                     return 76;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<77>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 241; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 241:
                 if ((match<shape_kind<77>>()(adjust_ptr<shape_kind<77>>(__selector_ptr,__switch_info.offset))))
                     return 77;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<78>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 244; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 244:
                 if ((match<shape_kind<78>>()(adjust_ptr<shape_kind<78>>(__selector_ptr,__switch_info.offset))))
                     return 78;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<79>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 247; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 247:
                 if ((match<shape_kind<79>>()(adjust_ptr<shape_kind<79>>(__selector_ptr,__switch_info.offset))))
                     return 79;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<80>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 250; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 250:
                 if ((match<shape_kind<80>>()(adjust_ptr<shape_kind<80>>(__selector_ptr,__switch_info.offset))))
                     return 80;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<81>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 253; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 253:
                 if ((match<shape_kind<81>>()(adjust_ptr<shape_kind<81>>(__selector_ptr,__switch_info.offset))))
                     return 81;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<82>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 256; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 256:
                 if ((match<shape_kind<82>>()(adjust_ptr<shape_kind<82>>(__selector_ptr,__switch_info.offset))))
                     return 82;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<83>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 259; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 259:
                 if ((match<shape_kind<83>>()(adjust_ptr<shape_kind<83>>(__selector_ptr,__switch_info.offset))))
                     return 83;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<84>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 262; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 262:
                 if ((match<shape_kind<84>>()(adjust_ptr<shape_kind<84>>(__selector_ptr,__switch_info.offset))))
                     return 84;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<85>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 265; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 265:
                 if ((match<shape_kind<85>>()(adjust_ptr<shape_kind<85>>(__selector_ptr,__switch_info.offset))))
                     return 85;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<86>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 268; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 268:
                 if ((match<shape_kind<86>>()(adjust_ptr<shape_kind<86>>(__selector_ptr,__switch_info.offset))))
                     return 86;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<87>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 271; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 271:
                 if ((match<shape_kind<87>>()(adjust_ptr<shape_kind<87>>(__selector_ptr,__switch_info.offset))))
                     return 87;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<88>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 274; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 274:
                 if ((match<shape_kind<88>>()(adjust_ptr<shape_kind<88>>(__selector_ptr,__switch_info.offset))))
                     return 88;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<89>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 277; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 277:
                 if ((match<shape_kind<89>>()(adjust_ptr<shape_kind<89>>(__selector_ptr,__switch_info.offset))))
                     return 89;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<90>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 280; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 280:
                 if ((match<shape_kind<90>>()(adjust_ptr<shape_kind<90>>(__selector_ptr,__switch_info.offset))))
                     return 90;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<91>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 283; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 283:
                 if ((match<shape_kind<91>>()(adjust_ptr<shape_kind<91>>(__selector_ptr,__switch_info.offset))))
                     return 91;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<92>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 286; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 286:
                 if ((match<shape_kind<92>>()(adjust_ptr<shape_kind<92>>(__selector_ptr,__switch_info.offset))))
                     return 92;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<93>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 289; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 289:
                 if ((match<shape_kind<93>>()(adjust_ptr<shape_kind<93>>(__selector_ptr,__switch_info.offset))))
                     return 93;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<94>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 292; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 292:
                 if ((match<shape_kind<94>>()(adjust_ptr<shape_kind<94>>(__selector_ptr,__switch_info.offset))))
                     return 94;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<95>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 295; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 295:
                 if ((match<shape_kind<95>>()(adjust_ptr<shape_kind<95>>(__selector_ptr,__switch_info.offset))))
                     return 95;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<96>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 298; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 298:
                 if ((match<shape_kind<96>>()(adjust_ptr<shape_kind<96>>(__selector_ptr,__switch_info.offset))))
                     return 96;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<97>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 301; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 301:
                 if ((match<shape_kind<97>>()(adjust_ptr<shape_kind<97>>(__selector_ptr,__switch_info.offset))))
                     return 97;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<98>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 304; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 304:
                 if ((match<shape_kind<98>>()(adjust_ptr<shape_kind<98>>(__selector_ptr,__switch_info.offset))))
                     return 98;
             }
             if ((__casted_ptr = dynamic_cast<const shape_kind<99>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 307; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 307:
                 if ((match<shape_kind<99>>()(adjust_ptr<shape_kind<99>>(__selector_ptr,__switch_info.offset))))
                     return 99;
             }
             if (__switch_info.line == 0) { __switch_info.line = 78; }
    case 78: ;
    }

    return -1;
}
DO_NOT_INLINE_END
#endif

DO_NOT_INLINE_BEGIN
size_t do_visit(const Shape& s)
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
DO_NOT_INLINE_END

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

const size_t N = 10000; // The amount of times visitor and matching procedure is invoked in one time measuring
const size_t M = 101;   // The amount of times time measuring is done
const size_t K = NUMBER_OF_DERIVED; // The amount of cases we have in hierarchy

template <typename Container>
typename Container::value_type mean(const Container& c)
{
    return std::accumulate(c.begin(),c.end(),typename Container::value_type())/c.size();
}

template <typename Container>
typename Container::value_type deviation(const Container& c)
{
    typedef typename Container::value_type value_type;
    value_type m = mean(c);
    value_type d = 0;

    for (typename Container::const_iterator p = c.begin(); p != c.end(); ++p)
        d += (*p-m)*(*p-m);

    return value_type(std::sqrt(double(d)/c.size()));
}

template <typename T>
void statistics(std::vector<T>& measurements, T& min, T& max, T& avg, T& med, T& dev)
{
    std::sort(measurements.begin(), measurements.end());
    min = measurements.front();
    max = measurements.back();
    avg = mean(measurements);
    med = measurements[measurements.size()/2];
    dev = deviation(measurements);
}

int relative_performance(long long v, long long m)
{
    if (UNLIKELY_BRANCH(v <= 0 || m <= 0))
    {
        std::cout << "ERROR: Insufficient timer resolution. Increase number of iterations N" << std::endl;
        exit(42);
    }
    else
    if (UNLIKELY_BRANCH(v <= m))
    {
        int percent = m*100/v-100;
        std::cout << "\t\t" << percent << "% slower" << std::endl;
        return +percent; // Positive number indicates how many percents slower we are 
    }
    else
    {
        int percent = v*100/m-100;
        std::cout << "\t\t" << percent << "% faster" << std::endl;
        return -percent; // Negative number indicates how many percents faster we are 
    }
}

long long display(const char* name, std::vector<long long>& timings)
{
    long long min, max, avg, med, dev;

    statistics(timings, min, max, avg, med, dev); // Get statistics from timings

    std::fstream file;
   
    file.open(std::string(name)+".csv", std::fstream::out | std::fstream::app);

    if (file)
    {
        std::copy(timings.begin(), timings.end(), std::ostream_iterator<long long>(file, ", "));
        file << "End" << std::endl;
    }

    file.close();

    std::cout << name << " Time: ["
              << std::setw(4) << microseconds(min) << " -- " 
              << std::setw(4) << microseconds(avg) << "/" 
              << std::setw(4) << microseconds(med) << " -- "
              << std::setw(4) << microseconds(max) << "] Dev = " 
              << std::setw(4) << dev << std::endl;
    return med;
}

int test_repetitive()
{
    std::cout << "=================== Repetitive Test ===================" << std::endl;

    size_t a1 = 0,a2 = 0;
    std::vector<int> percentages(K); // Final verdict of medians for each of the K experiments

    for (size_t n = 0; n < K; ++n)
    {
        Shape* s = make_shape(n);
        std::vector<long long> timingsV(M);
        std::vector<long long> timingsM(M);

        for (size_t m = 0; m < M; ++m)
        {
            time_stamp liStart1 = get_time_stamp();

            for (size_t i = 0; i < N; ++i)
                a1 += do_visit(*s);

            time_stamp liFinish1 = get_time_stamp();

            time_stamp liStart2 = get_time_stamp();

            for (size_t i = 0; i < N; ++i)
                a2 += do_match(*s);

            time_stamp liFinish2 = get_time_stamp();

            XTL_ASSERT(a1==a2);

            timingsV[m] = liFinish1-liStart1;
            timingsM[m] = liFinish2-liStart2;
        }

        long long avgV = display("AreaVisSeq", timingsV);
        long long avgM = display("AreaMatSeq", timingsM);
        percentages[n] = relative_performance(avgV, avgM);
        delete s;
    }

    if (a1 != a2)
    {
        std::cout << "ERROR: Invariant " << a1 << "==" << a2 << " doesn't hold." << std::endl;
        exit(42);
    }

    std::sort(percentages.begin(), percentages.end());
    return percentages[percentages.size()/2];
}

int test_randomized()
{
#if !defined(NO_RANDOMIZATION)
    srand (get_time_stamp()/get_frequency()); // Randomize pseudo random number generator
#endif
    std::cout << "=================== Randomized Test ===================" << std::endl;

    size_t a1 = 0,a2 = 0;

    std::vector<Shape*> shapes(N);
    TRACE_PERFORMANCE_ONLY(std::vector<size_t> distribution(K));

    for (size_t i = 0; i < N; ++i)
    {
        size_t n = rand()%K;
        TRACE_PERFORMANCE_ONLY(distribution[n]++);
        shapes[i] = make_shape(n);
    }
#if defined(TRACE_PERFORMANCE)
    size_t min, max, avg, med, dev;
    statistics(distribution, min, max, avg, med, dev);
    //std::copy(distribution.begin(), distribution.end(), std::ostream_iterator<size_t>(std::cout, ":"));
    std::cout << "Shape kind distribution: ["
              << std::setw(4) << min << " -- " 
              << std::setw(4) << avg << "/" 
              << std::setw(4) << med << " -- "
              << std::setw(4) << max << "] Dev = " 
              << std::setw(4) << dev << std::endl;
#endif

    std::vector<long long> timingsV(M);
    std::vector<long long> timingsM(M);

    for (size_t m = 0; m < M; ++m)
    {
        time_stamp liStart1 = get_time_stamp();

        for (size_t i = 0; i < N; ++i)
            a1 += do_visit(*shapes[i]);

        time_stamp liFinish1 = get_time_stamp();

        time_stamp liStart2 = get_time_stamp();

        for (size_t i = 0; i < N; ++i)
            a2 += do_match(*shapes[i]);

        time_stamp liFinish2 = get_time_stamp();

        XTL_ASSERT(a1==a2);
        timingsV[m] = liFinish1-liStart1;
        timingsM[m] = liFinish2-liStart2;
    }

    long long avgV = display("AreaVisRnd", timingsV);
    long long avgM = display("AreaMatRnd", timingsM);

    if (a1 != a2)
    {
        std::cout << "ERROR: Invariant " << a1 << "==" << a2 << " doesn't hold." << std::endl;
        exit(42);
    }

    return relative_performance(avgV, avgM);
}

int main()
{
    int ps = test_repetitive();
    int pr = test_randomized();
    std::cout << "OVERALL: Repetitive: " 
              << abs(ps) << (ps >= 0 ? "% slower" : "% faster") << "; Random: " 
              << abs(pr) << (pr >= 0 ? "% slower" : "% faster") << std::endl; 
}
