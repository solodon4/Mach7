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

#define NUMBER_OF_VFUNCS  1
#define NUMBER_OF_DERIVED 100

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
    #define FOR_EACH_MAX NUMBER_OF_VFUNCS-1
    #define FOR_EACH_N(N) virtual void foo ## N() {}
    #include "loop_over_numbers.hpp"
    #undef  FOR_EACH_N
    #undef  FOR_EACH_MAX
};

template <int N>
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

#define FOR_EACH_MAX NUMBER_OF_DERIVED-1

struct ShapeVisitor
{
    #define FOR_EACH_N(N) virtual void visit(const shape_kind<N>&) {}
    #include "loop_over_numbers.hpp"
    #undef  FOR_EACH_N
};

template <int N> void shape_kind<N>::accept(ShapeVisitor& v) const { v.visit(*this); }

#if 1
DO_NOT_INLINE_BEGIN
int do_match(Shape& s)
{
    SWITCH_N(s,FOR_EACH_MAX+1)
    {
    CASES_BEGIN
    #define FOR_EACH_N(N) CASE(shape_kind<N>) return N;
    #include "loop_over_numbers.hpp"
    #undef  FOR_EACH_N
    CASES_END
    }
    //XTL_ASSERT(!"Inexhaustive search");
    return -1;
}
DO_NOT_INLINE_END
#else
DO_NOT_INLINE_BEGIN
int do_match(Shape& s)
{
    //static vtblmap<type_switch_info&> __vtbl2lines_map;
    auto const   __selector_ptr = addr(s);
    const void*  __casted_ptr; 
    type_switch_info& __switch_info = preallocated<vtblmap<type_switch_info&,requires_bits<99+1>::value>,__LINE__>::value.get(__selector_ptr); 
    switch (__switch_info.line)
    {
    default: {
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<0>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 10; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 10:
                 auto const matched_object = adjust_ptr<shape_kind<0>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<0>>()(matched_object)))
                     return 0;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<1>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 13; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 13:
                 auto const matched_object = adjust_ptr<shape_kind<1>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<1>>()(matched_object)))
                     return 1;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<2>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 16; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 16:
                 auto const matched_object = adjust_ptr<shape_kind<2>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<2>>()(matched_object)))
                     return 2;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<3>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 19; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 19:
                 auto const matched_object = adjust_ptr<shape_kind<3>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<3>>()(matched_object)))
                     return 3;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<4>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 22; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 22:
                 auto const matched_object = adjust_ptr<shape_kind<4>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<4>>()(matched_object)))
                     return 4;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<5>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 25; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 25:
                 auto const matched_object = adjust_ptr<shape_kind<5>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<5>>()(matched_object)))
                     return 5;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<6>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 28; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 28:
                 auto const matched_object = adjust_ptr<shape_kind<6>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<6>>()(matched_object)))
                     return 6;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<7>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 31; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 31:
                 auto const matched_object = adjust_ptr<shape_kind<7>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<7>>()(matched_object)))
                     return 7;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<8>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 34; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 34:
                 auto const matched_object = adjust_ptr<shape_kind<8>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<8>>()(matched_object)))
                     return 8;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<9>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 37; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 37:
                 auto const matched_object = adjust_ptr<shape_kind<9>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<9>>()(matched_object)))
                     return 9;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<10>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 40; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 40:
                 auto const matched_object = adjust_ptr<shape_kind<10>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<10>>()(matched_object)))
                     return 10;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<11>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 43; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 43:
                 auto const matched_object = adjust_ptr<shape_kind<11>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<11>>()(matched_object)))
                     return 11;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<12>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 46; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 46:
                 auto const matched_object = adjust_ptr<shape_kind<12>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<12>>()(matched_object)))
                     return 12;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<13>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 49; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 49:
                 auto const matched_object = adjust_ptr<shape_kind<13>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<13>>()(matched_object)))
                     return 13;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<14>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 52; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 52:
                 auto const matched_object = adjust_ptr<shape_kind<14>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<14>>()(matched_object)))
                     return 14;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<15>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 55; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 55:
                 auto const matched_object = adjust_ptr<shape_kind<15>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<15>>()(matched_object)))
                     return 15;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<16>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 58; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 58:
                 auto const matched_object = adjust_ptr<shape_kind<16>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<16>>()(matched_object)))
                     return 16;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<17>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 61; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 61:
                 auto const matched_object = adjust_ptr<shape_kind<17>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<17>>()(matched_object)))
                     return 17;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<18>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 64; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 64:
                 auto const matched_object = adjust_ptr<shape_kind<18>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<18>>()(matched_object)))
                     return 18;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<19>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 67; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 67:
                 auto const matched_object = adjust_ptr<shape_kind<19>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<19>>()(matched_object)))
                     return 19;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<20>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 70; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 70:
                 auto const matched_object = adjust_ptr<shape_kind<20>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<20>>()(matched_object)))
                     return 20;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<21>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 73; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 73:
                 auto const matched_object = adjust_ptr<shape_kind<21>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<21>>()(matched_object)))
                     return 21;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<22>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 76; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 76:
                 auto const matched_object = adjust_ptr<shape_kind<22>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<22>>()(matched_object)))
                     return 22;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<23>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 79; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 79:
                 auto const matched_object = adjust_ptr<shape_kind<23>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<23>>()(matched_object)))
                     return 23;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<24>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 82; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 82:
                 auto const matched_object = adjust_ptr<shape_kind<24>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<24>>()(matched_object)))
                     return 24;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<25>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 85; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 85:
                 auto const matched_object = adjust_ptr<shape_kind<25>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<25>>()(matched_object)))
                     return 25;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<26>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 88; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 88:
                 auto const matched_object = adjust_ptr<shape_kind<26>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<26>>()(matched_object)))
                     return 26;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<27>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 91; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 91:
                 auto const matched_object = adjust_ptr<shape_kind<27>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<27>>()(matched_object)))
                     return 27;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<28>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 94; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 94:
                 auto const matched_object = adjust_ptr<shape_kind<28>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<28>>()(matched_object)))
                     return 28;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<29>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 97; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 97:
                 auto const matched_object = adjust_ptr<shape_kind<29>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<29>>()(matched_object)))
                     return 29;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<30>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 100; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 100:
                 auto const matched_object = adjust_ptr<shape_kind<30>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<30>>()(matched_object)))
                     return 30;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<31>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 103; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 103:
                 auto const matched_object = adjust_ptr<shape_kind<31>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<31>>()(matched_object)))
                     return 31;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<32>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 106; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 106:
                 auto const matched_object = adjust_ptr<shape_kind<32>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<32>>()(matched_object)))
                     return 32;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<33>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 109; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 109:
                 auto const matched_object = adjust_ptr<shape_kind<33>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<33>>()(matched_object)))
                     return 33;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<34>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 112; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 112:
                 auto const matched_object = adjust_ptr<shape_kind<34>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<34>>()(matched_object)))
                     return 34;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<35>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 115; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 115:
                 auto const matched_object = adjust_ptr<shape_kind<35>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<35>>()(matched_object)))
                     return 35;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<36>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 118; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 118:
                 auto const matched_object = adjust_ptr<shape_kind<36>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<36>>()(matched_object)))
                     return 36;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<37>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 121; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 121:
                 auto const matched_object = adjust_ptr<shape_kind<37>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<37>>()(matched_object)))
                     return 37;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<38>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 124; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 124:
                 auto const matched_object = adjust_ptr<shape_kind<38>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<38>>()(matched_object)))
                     return 38;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<39>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 127; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 127:
                 auto const matched_object = adjust_ptr<shape_kind<39>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<39>>()(matched_object)))
                     return 39;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<40>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 130; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 130:
                 auto const matched_object = adjust_ptr<shape_kind<40>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<40>>()(matched_object)))
                     return 40;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<41>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 133; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 133:
                 auto const matched_object = adjust_ptr<shape_kind<41>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<41>>()(matched_object)))
                     return 41;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<42>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 136; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 136:
                 auto const matched_object = adjust_ptr<shape_kind<42>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<42>>()(matched_object)))
                     return 42;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<43>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 139; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 139:
                 auto const matched_object = adjust_ptr<shape_kind<43>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<43>>()(matched_object)))
                     return 43;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<44>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 142; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 142:
                 auto const matched_object = adjust_ptr<shape_kind<44>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<44>>()(matched_object)))
                     return 44;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<45>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 145; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 145:
                 auto const matched_object = adjust_ptr<shape_kind<45>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<45>>()(matched_object)))
                     return 45;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<46>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 148; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 148:
                 auto const matched_object = adjust_ptr<shape_kind<46>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<46>>()(matched_object)))
                     return 46;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<47>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 151; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 151:
                 auto const matched_object = adjust_ptr<shape_kind<47>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<47>>()(matched_object)))
                     return 47;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<48>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 154; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 154:
                 auto const matched_object = adjust_ptr<shape_kind<48>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<48>>()(matched_object)))
                     return 48;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<49>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 157; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 157:
                 auto const matched_object = adjust_ptr<shape_kind<49>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<49>>()(matched_object)))
                     return 49;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<50>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 160; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 160:
                 auto const matched_object = adjust_ptr<shape_kind<50>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<50>>()(matched_object)))
                     return 50;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<51>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 163; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 163:
                 auto const matched_object = adjust_ptr<shape_kind<51>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<51>>()(matched_object)))
                     return 51;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<52>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 166; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 166:
                 auto const matched_object = adjust_ptr<shape_kind<52>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<52>>()(matched_object)))
                     return 52;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<53>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 169; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 169:
                 auto const matched_object = adjust_ptr<shape_kind<53>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<53>>()(matched_object)))
                     return 53;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<54>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 172; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 172:
                 auto const matched_object = adjust_ptr<shape_kind<54>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<54>>()(matched_object)))
                     return 54;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<55>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 175; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 175:
                 auto const matched_object = adjust_ptr<shape_kind<55>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<55>>()(matched_object)))
                     return 55;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<56>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 178; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 178:
                 auto const matched_object = adjust_ptr<shape_kind<56>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<56>>()(matched_object)))
                     return 56;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<57>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 181; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 181:
                 auto const matched_object = adjust_ptr<shape_kind<57>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<57>>()(matched_object)))
                     return 57;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<58>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 184; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 184:
                 auto const matched_object = adjust_ptr<shape_kind<58>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<58>>()(matched_object)))
                     return 58;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<59>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 187; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 187:
                 auto const matched_object = adjust_ptr<shape_kind<59>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<59>>()(matched_object)))
                     return 59;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<60>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 190; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 190:
                 auto const matched_object = adjust_ptr<shape_kind<60>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<60>>()(matched_object)))
                     return 60;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<61>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 193; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 193:
                 auto const matched_object = adjust_ptr<shape_kind<61>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<61>>()(matched_object)))
                     return 61;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<62>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 196; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 196:
                 auto const matched_object = adjust_ptr<shape_kind<62>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<62>>()(matched_object)))
                     return 62;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<63>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 199; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 199:
                 auto const matched_object = adjust_ptr<shape_kind<63>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<63>>()(matched_object)))
                     return 63;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<64>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 202; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 202:
                 auto const matched_object = adjust_ptr<shape_kind<64>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<64>>()(matched_object)))
                     return 64;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<65>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 205; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 205:
                 auto const matched_object = adjust_ptr<shape_kind<65>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<65>>()(matched_object)))
                     return 65;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<66>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 208; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 208:
                 auto const matched_object = adjust_ptr<shape_kind<66>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<66>>()(matched_object)))
                     return 66;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<67>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 211; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 211:
                 auto const matched_object = adjust_ptr<shape_kind<67>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<67>>()(matched_object)))
                     return 67;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<68>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 214; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 214:
                 auto const matched_object = adjust_ptr<shape_kind<68>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<68>>()(matched_object)))
                     return 68;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<69>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 217; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 217:
                 auto const matched_object = adjust_ptr<shape_kind<69>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<69>>()(matched_object)))
                     return 69;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<70>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 220; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 220:
                 auto const matched_object = adjust_ptr<shape_kind<70>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<70>>()(matched_object)))
                     return 70;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<71>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 223; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 223:
                 auto const matched_object = adjust_ptr<shape_kind<71>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<71>>()(matched_object)))
                     return 71;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<72>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 226; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 226:
                 auto const matched_object = adjust_ptr<shape_kind<72>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<72>>()(matched_object)))
                     return 72;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<73>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 229; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 229:
                 auto const matched_object = adjust_ptr<shape_kind<73>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<73>>()(matched_object)))
                     return 73;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<74>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 232; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 232:
                 auto const matched_object = adjust_ptr<shape_kind<74>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<74>>()(matched_object)))
                     return 74;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<75>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 235; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 235:
                 auto const matched_object = adjust_ptr<shape_kind<75>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<75>>()(matched_object)))
                     return 75;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<76>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 238; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 238:
                 auto const matched_object = adjust_ptr<shape_kind<76>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<76>>()(matched_object)))
                     return 76;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<77>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 241; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 241:
                 auto const matched_object = adjust_ptr<shape_kind<77>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<77>>()(matched_object)))
                     return 77;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<78>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 244; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 244:
                 auto const matched_object = adjust_ptr<shape_kind<78>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<78>>()(matched_object)))
                     return 78;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<79>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 247; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 247:
                 auto const matched_object = adjust_ptr<shape_kind<79>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<79>>()(matched_object)))
                     return 79;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<80>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 250; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 250:
                 auto const matched_object = adjust_ptr<shape_kind<80>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<80>>()(matched_object)))
                     return 80;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<81>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 253; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 253:
                 auto const matched_object = adjust_ptr<shape_kind<81>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<81>>()(matched_object)))
                     return 81;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<82>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 256; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 256:
                 auto const matched_object = adjust_ptr<shape_kind<82>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<82>>()(matched_object)))
                     return 82;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<83>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 259; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 259:
                 auto const matched_object = adjust_ptr<shape_kind<83>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<83>>()(matched_object)))
                     return 83;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<84>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 262; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 262:
                 auto const matched_object = adjust_ptr<shape_kind<84>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<84>>()(matched_object)))
                     return 84;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<85>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 265; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 265:
                 auto const matched_object = adjust_ptr<shape_kind<85>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<85>>()(matched_object)))
                     return 85;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<86>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 268; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 268:
                 auto const matched_object = adjust_ptr<shape_kind<86>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<86>>()(matched_object)))
                     return 86;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<87>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 271; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 271:
                 auto const matched_object = adjust_ptr<shape_kind<87>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<87>>()(matched_object)))
                     return 87;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<88>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 274; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 274:
                 auto const matched_object = adjust_ptr<shape_kind<88>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<88>>()(matched_object)))
                     return 88;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<89>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 277; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 277:
                 auto const matched_object = adjust_ptr<shape_kind<89>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<89>>()(matched_object)))
                     return 89;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<90>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 280; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 280:
                 auto const matched_object = adjust_ptr<shape_kind<90>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<90>>()(matched_object)))
                     return 90;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<91>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 283; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 283:
                 auto const matched_object = adjust_ptr<shape_kind<91>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<91>>()(matched_object)))
                     return 91;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<92>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 286; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 286:
                 auto const matched_object = adjust_ptr<shape_kind<92>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<92>>()(matched_object)))
                     return 92;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<93>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 289; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 289:
                 auto const matched_object = adjust_ptr<shape_kind<93>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<93>>()(matched_object)))
                     return 93;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<94>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 292; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 292:
                 auto const matched_object = adjust_ptr<shape_kind<94>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<94>>()(matched_object)))
                     return 94;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<95>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 295; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 295:
                 auto const matched_object = adjust_ptr<shape_kind<95>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<95>>()(matched_object)))
                     return 95;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<96>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 298; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 298:
                 auto const matched_object = adjust_ptr<shape_kind<96>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<96>>()(matched_object)))
                     return 96;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<97>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 301; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 301:
                 auto const matched_object = adjust_ptr<shape_kind<97>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<97>>()(matched_object)))
                     return 97;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<98>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 304; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 304:
                 auto const matched_object = adjust_ptr<shape_kind<98>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<98>>()(matched_object)))
                     return 98;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<99>*>(__selector_ptr)))
             {
                 if (__switch_info.line == 0) { __switch_info.line = 307; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); }
    case 307:
                 auto const matched_object = adjust_ptr<shape_kind<99>>(__selector_ptr,__switch_info.offset);
                 if ((match<shape_kind<99>>()(matched_object)))
                     return 99;
             }
             if (__switch_info.line == 0) { __switch_info.line = 57; }
    case 57: ;
    }
    return -1;
}
DO_NOT_INLINE_END
#endif

DO_NOT_INLINE_BEGIN
int do_visit(const Shape& s)
{
    struct Visitor : ShapeVisitor
    {
        #define FOR_EACH_N(N) virtual void visit(const shape_kind<N>&) { result = N; }
        #include "loop_over_numbers.hpp"
        #undef  FOR_EACH_N
        int result;
    };

    Visitor v;
    v.result = -1;
    s.accept(v);
    return v.result;
}
DO_NOT_INLINE_END

Shape* make_shape(int i)
{
    switch (i)
    {
    #define FOR_EACH_N(N) case N: return new shape_kind<N>;
    #include "loop_over_numbers.hpp"
    #undef  FOR_EACH_N
    }
    return 0;
}

const int N = 10000; // The amount of times visitor and matching procedure is invoked in one time measuring
const int M = 101;   // The amount of times time measuring is done
const int K = FOR_EACH_MAX+1; // The amount of cases we have in hierarchy

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

#ifdef TRACE_PERFORMANCE
#include <bitset> // For print out purposes only
#endif

void test_sequential()
{
#ifdef TRACE_PERFORMANCE_XXX
    cache_hits   = 0;
    cache_misses = 0;
#endif
    std::cout << "=================== Sequential Test ===================" << std::endl;

    int a1 = 0,a2 = 0;

    for (int n = 0; n < K; ++n)
    {
        std::vector<Shape*> shapes(N);

        for (int i = 0; i < N; ++i)
        {
            int n = i%K;
            shapes[i] = make_shape(n);
        }

        std::vector<long long> timingsV(M);
        std::vector<long long> timingsM(M);

        for (int m = 0; m < M; ++m)
        {
            time_stamp liStart1 = get_time_stamp();

            for (int i = 0; i < N; ++i)
                a1 += do_visit(*shapes[i]);

            time_stamp liFinish1 = get_time_stamp();

            time_stamp liStart2 = get_time_stamp();

            for (int i = 0; i < N; ++i)
                a2 += do_match(*shapes[i]);

            time_stamp liFinish2 = get_time_stamp();

            XTL_ASSERT(a1==a2);

            timingsV[m] = liFinish1-liStart1;
            timingsM[m] = liFinish2-liStart2;
        }

        long long avgV = display("AreaVisSeq", timingsV);
        long long avgM = display("AreaMatSeq", timingsM);
        //if (avgV)
            std::cout << "\t\t" << avgM*100/avgV-100 << "% slower" << std::endl;
        //else
        //    std::cout << "Insufficient timer resolution" << std::endl;
        std::cout << "\t\tThe following 2 numbers should be the same " << a1 << "==" << a2 << " ? " << std::boolalpha << (a1==a2) << std::endl;
        for (int i = 0; i < N; ++i)
        {
            delete shapes[i];
            shapes[i] = 0;
        }
    }

#ifdef TRACE_PERFORMANCE_XXX
    std::cout << "Vtbl cache hits: " << cache_hits << "\tVtbl cache misses: " << cache_misses << std::endl;

    int i = 0;

    for (; (differ >> i) << i == differ; ++i);

    if (i-1 != VTBL_IRRELEVANT_BITS)
    {
        std::cout << "WARNING: Empirically computed irrelevant_bits " << i-1 
                  << " differs from the predefined one " STRING_LITERAL(VTBL_IRRELEVANT_BITS) 
                  << ". See vtbl patterns below: " << std::endl;
    std::cout << "Common: " << std::bitset<8*sizeof(intptr_t)>((unsigned long long)common) << std::endl
              << "Differ: " << std::bitset<8*sizeof(intptr_t)>((unsigned long long)differ) << std::endl;
    }
#endif
}

void test_randomized()
{
    srand (get_time_stamp()/get_frequency()); // Randomize pseudo random number generator

#ifdef TRACE_PERFORMANCE_XXX
    cache_hits   = 0;
    cache_misses = 0;
#endif
    std::cout << "=================== Randomized Test ===================" << std::endl;

    for (int n = 0; n < K; ++n)
    {
        std::vector<Shape*> shapes(N);
        TRACE_PERFORMANCE_ONLY(std::vector<int> distribution(K));

        for (int i = 0; i < N; ++i)
        {
            int n = rand()%K;
            TRACE_PERFORMANCE_ONLY(distribution[n]++);
            shapes[i] = make_shape(n);
        }
#if defined(TRACE_PERFORMANCE)
        int min, max, avg, med, dev;
        statistics(distribution, min, max, avg, med, dev);
        //std::copy(distribution.begin(), distribution.end(), std::ostream_iterator<int>(std::cout, ":"));
        std::cout << "Shape kind distribution: ["
                  << std::setw(4) << min << " -- " 
                  << std::setw(4) << avg << "/" 
                  << std::setw(4) << med << " -- "
                  << std::setw(4) << max << "] Dev = " 
                  << std::setw(4) << dev << std::endl;
#endif

        std::vector<long long> timingsV(M);
        std::vector<long long> timingsM(M);

        int a1 = 0,a2 = 0;

        for (int m = 0; m < M; ++m)
        {
            time_stamp liStart1 = get_time_stamp();

            for (int i = 0; i < N; ++i)
                a1 += do_visit(*shapes[i]);

            time_stamp liFinish1 = get_time_stamp();

            time_stamp liStart2 = get_time_stamp();

            for (int i = 0; i < N; ++i)
                a2 += do_match(*shapes[i]);

            time_stamp liFinish2 = get_time_stamp();

            XTL_ASSERT(a1==a2);
            timingsV[m] = liFinish1-liStart1;
            timingsM[m] = liFinish2-liStart2;
        }

        long long avgV = display("AreaVisRnd", timingsV);
        long long avgM = display("AreaMatRnd", timingsM);
        //if (avgV)
            std::cout << "\t\t" << avgM*100/avgV-100 << "% slower" << std::endl;
        //else
            //std::cout << "Insufficient timer resolution" << std::endl;
        std::cout << "\t\tThe following 2 numbers should be the same " << a1 << "==" << a2 << " ? " << std::boolalpha << (a1==a2) << std::endl;

        for (int i = 0; i < N; ++i)
        {
            delete shapes[i];
            shapes[i] = 0;
        }
    }

#ifdef TRACE_PERFORMANCE_XXX
    std::cout << "Vtbl cache hits: " << cache_hits << "\tVtbl cache misses: " << cache_misses << std::endl;

    int i = 0;

    for (; (differ >> i) << i == differ; ++i);

    if (i-1 != VTBL_IRRELEVANT_BITS)
    {
        std::cout << "WARNING: Empirically computed irrelevant_bits " << i-1 
                  << " differs from the predefined one " STRING_LITERAL(VTBL_IRRELEVANT_BITS) 
                  << ". See vtbl patterns below: " << std::endl;
        std::cout << "Common: " << std::bitset<8*sizeof(intptr_t)>((unsigned long long)common) << std::endl
                  << "Differ: " << std::bitset<8*sizeof(intptr_t)>((unsigned long long)differ) << std::endl;
    }
#endif
}

int main()
{
    //test_sequential();
    test_randomized();
}

#undef  FOR_EACH_MAX
