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

#define FOR_EACH_MAX 99

struct ShapeVisitor;

struct Shape
{
    virtual void accept(ShapeVisitor&) const = 0;
};

template <int N>
struct shape_kind : Shape
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
    #define FOR_EACH_N(N) virtual void visit(const shape_kind<N>&) {}
    #include "loop_over_numbers.hpp"
    #undef  FOR_EACH_N
};

template <int N> void shape_kind<N>::accept(ShapeVisitor& v) const { v.visit(*this); }

#if 0
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
    static vtbl2lines<requires_bits<99+1>::value> __vtbl2lines_map; 
    decltype(s)& __selector_var = s; 
    const void* __casted_ptr; 
    line_offset& __switch_info = __vtbl2lines_map.get(addr(__selector_var)); 
    switch (__switch_info.line)
    {
    default: {
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<0>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(10, addr(__selector_var), __casted_ptr);
    case 10:
                 auto matched_object = adjust_ptr<shape_kind<0>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<0>>()(matched_object)))
                     return 0;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<1>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(13, addr(__selector_var), __casted_ptr);
    case 13:
                 auto matched_object = adjust_ptr<shape_kind<1>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<1>>()(matched_object)))
                     return 1;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<2>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(16, addr(__selector_var), __casted_ptr);
    case 16:
                 auto matched_object = adjust_ptr<shape_kind<2>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<2>>()(matched_object)))
                     return 2;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<3>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(19, addr(__selector_var), __casted_ptr);
    case 19:
                 auto matched_object = adjust_ptr<shape_kind<3>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<3>>()(matched_object)))
                     return 3;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<4>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(22, addr(__selector_var), __casted_ptr);
    case 22:
                 auto matched_object = adjust_ptr<shape_kind<4>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<4>>()(matched_object)))
                     return 4;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<5>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(25, addr(__selector_var), __casted_ptr);
    case 25:
                 auto matched_object = adjust_ptr<shape_kind<5>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<5>>()(matched_object)))
                     return 5;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<6>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(28, addr(__selector_var), __casted_ptr);
    case 28:
                 auto matched_object = adjust_ptr<shape_kind<6>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<6>>()(matched_object)))
                     return 6;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<7>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(31, addr(__selector_var), __casted_ptr);
    case 31:
                 auto matched_object = adjust_ptr<shape_kind<7>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<7>>()(matched_object)))
                     return 7;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<8>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(34, addr(__selector_var), __casted_ptr);
    case 34:
                 auto matched_object = adjust_ptr<shape_kind<8>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<8>>()(matched_object)))
                     return 8;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<9>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(37, addr(__selector_var), __casted_ptr);
    case 37:
                 auto matched_object = adjust_ptr<shape_kind<9>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<9>>()(matched_object)))
                     return 9;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<10>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(40, addr(__selector_var), __casted_ptr);
    case 40:
                 auto matched_object = adjust_ptr<shape_kind<10>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<10>>()(matched_object)))
                     return 10;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<11>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(43, addr(__selector_var), __casted_ptr);
    case 43:
                 auto matched_object = adjust_ptr<shape_kind<11>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<11>>()(matched_object)))
                     return 11;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<12>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(46, addr(__selector_var), __casted_ptr);
    case 46:
                 auto matched_object = adjust_ptr<shape_kind<12>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<12>>()(matched_object)))
                     return 12;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<13>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(49, addr(__selector_var), __casted_ptr);
    case 49:
                 auto matched_object = adjust_ptr<shape_kind<13>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<13>>()(matched_object)))
                     return 13;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<14>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(52, addr(__selector_var), __casted_ptr);
    case 52:
                 auto matched_object = adjust_ptr<shape_kind<14>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<14>>()(matched_object)))
                     return 14;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<15>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(55, addr(__selector_var), __casted_ptr);
    case 55:
                 auto matched_object = adjust_ptr<shape_kind<15>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<15>>()(matched_object)))
                     return 15;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<16>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(58, addr(__selector_var), __casted_ptr);
    case 58:
                 auto matched_object = adjust_ptr<shape_kind<16>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<16>>()(matched_object)))
                     return 16;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<17>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(61, addr(__selector_var), __casted_ptr);
    case 61:
                 auto matched_object = adjust_ptr<shape_kind<17>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<17>>()(matched_object)))
                     return 17;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<18>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(64, addr(__selector_var), __casted_ptr);
    case 64:
                 auto matched_object = adjust_ptr<shape_kind<18>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<18>>()(matched_object)))
                     return 18;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<19>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(67, addr(__selector_var), __casted_ptr);
    case 67:
                 auto matched_object = adjust_ptr<shape_kind<19>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<19>>()(matched_object)))
                     return 19;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<20>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(70, addr(__selector_var), __casted_ptr);
    case 70:
                 auto matched_object = adjust_ptr<shape_kind<20>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<20>>()(matched_object)))
                     return 20;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<21>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(73, addr(__selector_var), __casted_ptr);
    case 73:
                 auto matched_object = adjust_ptr<shape_kind<21>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<21>>()(matched_object)))
                     return 21;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<22>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(76, addr(__selector_var), __casted_ptr);
    case 76:
                 auto matched_object = adjust_ptr<shape_kind<22>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<22>>()(matched_object)))
                     return 22;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<23>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(79, addr(__selector_var), __casted_ptr);
    case 79:
                 auto matched_object = adjust_ptr<shape_kind<23>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<23>>()(matched_object)))
                     return 23;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<24>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(82, addr(__selector_var), __casted_ptr);
    case 82:
                 auto matched_object = adjust_ptr<shape_kind<24>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<24>>()(matched_object)))
                     return 24;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<25>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(85, addr(__selector_var), __casted_ptr);
    case 85:
                 auto matched_object = adjust_ptr<shape_kind<25>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<25>>()(matched_object)))
                     return 25;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<26>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(88, addr(__selector_var), __casted_ptr);
    case 88:
                 auto matched_object = adjust_ptr<shape_kind<26>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<26>>()(matched_object)))
                     return 26;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<27>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(91, addr(__selector_var), __casted_ptr);
    case 91:
                 auto matched_object = adjust_ptr<shape_kind<27>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<27>>()(matched_object)))
                     return 27;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<28>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(94, addr(__selector_var), __casted_ptr);
    case 94:
                 auto matched_object = adjust_ptr<shape_kind<28>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<28>>()(matched_object)))
                     return 28;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<29>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(97, addr(__selector_var), __casted_ptr);
    case 97:
                 auto matched_object = adjust_ptr<shape_kind<29>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<29>>()(matched_object)))
                     return 29;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<30>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(100, addr(__selector_var), __casted_ptr);
    case 100:
                 auto matched_object = adjust_ptr<shape_kind<30>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<30>>()(matched_object)))
                     return 30;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<31>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(103, addr(__selector_var), __casted_ptr);
    case 103:
                 auto matched_object = adjust_ptr<shape_kind<31>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<31>>()(matched_object)))
                     return 31;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<32>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(106, addr(__selector_var), __casted_ptr);
    case 106:
                 auto matched_object = adjust_ptr<shape_kind<32>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<32>>()(matched_object)))
                     return 32;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<33>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(109, addr(__selector_var), __casted_ptr);
    case 109:
                 auto matched_object = adjust_ptr<shape_kind<33>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<33>>()(matched_object)))
                     return 33;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<34>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(112, addr(__selector_var), __casted_ptr);
    case 112:
                 auto matched_object = adjust_ptr<shape_kind<34>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<34>>()(matched_object)))
                     return 34;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<35>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(115, addr(__selector_var), __casted_ptr);
    case 115:
                 auto matched_object = adjust_ptr<shape_kind<35>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<35>>()(matched_object)))
                     return 35;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<36>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(118, addr(__selector_var), __casted_ptr);
    case 118:
                 auto matched_object = adjust_ptr<shape_kind<36>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<36>>()(matched_object)))
                     return 36;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<37>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(121, addr(__selector_var), __casted_ptr);
    case 121:
                 auto matched_object = adjust_ptr<shape_kind<37>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<37>>()(matched_object)))
                     return 37;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<38>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(124, addr(__selector_var), __casted_ptr);
    case 124:
                 auto matched_object = adjust_ptr<shape_kind<38>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<38>>()(matched_object)))
                     return 38;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<39>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(127, addr(__selector_var), __casted_ptr);
    case 127:
                 auto matched_object = adjust_ptr<shape_kind<39>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<39>>()(matched_object)))
                     return 39;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<40>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(130, addr(__selector_var), __casted_ptr);
    case 130:
                 auto matched_object = adjust_ptr<shape_kind<40>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<40>>()(matched_object)))
                     return 40;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<41>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(133, addr(__selector_var), __casted_ptr);
    case 133:
                 auto matched_object = adjust_ptr<shape_kind<41>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<41>>()(matched_object)))
                     return 41;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<42>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(136, addr(__selector_var), __casted_ptr);
    case 136:
                 auto matched_object = adjust_ptr<shape_kind<42>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<42>>()(matched_object)))
                     return 42;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<43>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(139, addr(__selector_var), __casted_ptr);
    case 139:
                 auto matched_object = adjust_ptr<shape_kind<43>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<43>>()(matched_object)))
                     return 43;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<44>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(142, addr(__selector_var), __casted_ptr);
    case 142:
                 auto matched_object = adjust_ptr<shape_kind<44>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<44>>()(matched_object)))
                     return 44;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<45>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(145, addr(__selector_var), __casted_ptr);
    case 145:
                 auto matched_object = adjust_ptr<shape_kind<45>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<45>>()(matched_object)))
                     return 45;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<46>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(148, addr(__selector_var), __casted_ptr);
    case 148:
                 auto matched_object = adjust_ptr<shape_kind<46>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<46>>()(matched_object)))
                     return 46;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<47>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(151, addr(__selector_var), __casted_ptr);
    case 151:
                 auto matched_object = adjust_ptr<shape_kind<47>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<47>>()(matched_object)))
                     return 47;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<48>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(154, addr(__selector_var), __casted_ptr);
    case 154:
                 auto matched_object = adjust_ptr<shape_kind<48>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<48>>()(matched_object)))
                     return 48;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<49>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(157, addr(__selector_var), __casted_ptr);
    case 157:
                 auto matched_object = adjust_ptr<shape_kind<49>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<49>>()(matched_object)))
                     return 49;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<50>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(160, addr(__selector_var), __casted_ptr);
    case 160:
                 auto matched_object = adjust_ptr<shape_kind<50>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<50>>()(matched_object)))
                     return 50;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<51>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(163, addr(__selector_var), __casted_ptr);
    case 163:
                 auto matched_object = adjust_ptr<shape_kind<51>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<51>>()(matched_object)))
                     return 51;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<52>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(166, addr(__selector_var), __casted_ptr);
    case 166:
                 auto matched_object = adjust_ptr<shape_kind<52>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<52>>()(matched_object)))
                     return 52;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<53>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(169, addr(__selector_var), __casted_ptr);
    case 169:
                 auto matched_object = adjust_ptr<shape_kind<53>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<53>>()(matched_object)))
                     return 53;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<54>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(172, addr(__selector_var), __casted_ptr);
    case 172:
                 auto matched_object = adjust_ptr<shape_kind<54>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<54>>()(matched_object)))
                     return 54;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<55>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(175, addr(__selector_var), __casted_ptr);
    case 175:
                 auto matched_object = adjust_ptr<shape_kind<55>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<55>>()(matched_object)))
                     return 55;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<56>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(178, addr(__selector_var), __casted_ptr);
    case 178:
                 auto matched_object = adjust_ptr<shape_kind<56>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<56>>()(matched_object)))
                     return 56;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<57>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(181, addr(__selector_var), __casted_ptr);
    case 181:
                 auto matched_object = adjust_ptr<shape_kind<57>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<57>>()(matched_object)))
                     return 57;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<58>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(184, addr(__selector_var), __casted_ptr);
    case 184:
                 auto matched_object = adjust_ptr<shape_kind<58>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<58>>()(matched_object)))
                     return 58;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<59>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(187, addr(__selector_var), __casted_ptr);
    case 187:
                 auto matched_object = adjust_ptr<shape_kind<59>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<59>>()(matched_object)))
                     return 59;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<60>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(190, addr(__selector_var), __casted_ptr);
    case 190:
                 auto matched_object = adjust_ptr<shape_kind<60>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<60>>()(matched_object)))
                     return 60;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<61>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(193, addr(__selector_var), __casted_ptr);
    case 193:
                 auto matched_object = adjust_ptr<shape_kind<61>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<61>>()(matched_object)))
                     return 61;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<62>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(196, addr(__selector_var), __casted_ptr);
    case 196:
                 auto matched_object = adjust_ptr<shape_kind<62>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<62>>()(matched_object)))
                     return 62;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<63>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(199, addr(__selector_var), __casted_ptr);
    case 199:
                 auto matched_object = adjust_ptr<shape_kind<63>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<63>>()(matched_object)))
                     return 63;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<64>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(202, addr(__selector_var), __casted_ptr);
    case 202:
                 auto matched_object = adjust_ptr<shape_kind<64>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<64>>()(matched_object)))
                     return 64;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<65>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(205, addr(__selector_var), __casted_ptr);
    case 205:
                 auto matched_object = adjust_ptr<shape_kind<65>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<65>>()(matched_object)))
                     return 65;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<66>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(208, addr(__selector_var), __casted_ptr);
    case 208:
                 auto matched_object = adjust_ptr<shape_kind<66>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<66>>()(matched_object)))
                     return 66;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<67>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(211, addr(__selector_var), __casted_ptr);
    case 211:
                 auto matched_object = adjust_ptr<shape_kind<67>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<67>>()(matched_object)))
                     return 67;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<68>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(214, addr(__selector_var), __casted_ptr);
    case 214:
                 auto matched_object = adjust_ptr<shape_kind<68>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<68>>()(matched_object)))
                     return 68;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<69>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(217, addr(__selector_var), __casted_ptr);
    case 217:
                 auto matched_object = adjust_ptr<shape_kind<69>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<69>>()(matched_object)))
                     return 69;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<70>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(220, addr(__selector_var), __casted_ptr);
    case 220:
                 auto matched_object = adjust_ptr<shape_kind<70>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<70>>()(matched_object)))
                     return 70;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<71>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(223, addr(__selector_var), __casted_ptr);
    case 223:
                 auto matched_object = adjust_ptr<shape_kind<71>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<71>>()(matched_object)))
                     return 71;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<72>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(226, addr(__selector_var), __casted_ptr);
    case 226:
                 auto matched_object = adjust_ptr<shape_kind<72>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<72>>()(matched_object)))
                     return 72;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<73>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(229, addr(__selector_var), __casted_ptr);
    case 229:
                 auto matched_object = adjust_ptr<shape_kind<73>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<73>>()(matched_object)))
                     return 73;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<74>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(232, addr(__selector_var), __casted_ptr);
    case 232:
                 auto matched_object = adjust_ptr<shape_kind<74>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<74>>()(matched_object)))
                     return 74;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<75>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(235, addr(__selector_var), __casted_ptr);
    case 235:
                 auto matched_object = adjust_ptr<shape_kind<75>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<75>>()(matched_object)))
                     return 75;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<76>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(238, addr(__selector_var), __casted_ptr);
    case 238:
                 auto matched_object = adjust_ptr<shape_kind<76>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<76>>()(matched_object)))
                     return 76;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<77>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(241, addr(__selector_var), __casted_ptr);
    case 241:
                 auto matched_object = adjust_ptr<shape_kind<77>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<77>>()(matched_object)))
                     return 77;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<78>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(244, addr(__selector_var), __casted_ptr);
    case 244:
                 auto matched_object = adjust_ptr<shape_kind<78>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<78>>()(matched_object)))
                     return 78;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<79>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(247, addr(__selector_var), __casted_ptr);
    case 247:
                 auto matched_object = adjust_ptr<shape_kind<79>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<79>>()(matched_object)))
                     return 79;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<80>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(250, addr(__selector_var), __casted_ptr);
    case 250:
                 auto matched_object = adjust_ptr<shape_kind<80>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<80>>()(matched_object)))
                     return 80;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<81>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(253, addr(__selector_var), __casted_ptr);
    case 253:
                 auto matched_object = adjust_ptr<shape_kind<81>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<81>>()(matched_object)))
                     return 81;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<82>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(256, addr(__selector_var), __casted_ptr);
    case 256:
                 auto matched_object = adjust_ptr<shape_kind<82>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<82>>()(matched_object)))
                     return 82;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<83>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(259, addr(__selector_var), __casted_ptr);
    case 259:
                 auto matched_object = adjust_ptr<shape_kind<83>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<83>>()(matched_object)))
                     return 83;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<84>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(262, addr(__selector_var), __casted_ptr);
    case 262:
                 auto matched_object = adjust_ptr<shape_kind<84>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<84>>()(matched_object)))
                     return 84;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<85>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(265, addr(__selector_var), __casted_ptr);
    case 265:
                 auto matched_object = adjust_ptr<shape_kind<85>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<85>>()(matched_object)))
                     return 85;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<86>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(268, addr(__selector_var), __casted_ptr);
    case 268:
                 auto matched_object = adjust_ptr<shape_kind<86>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<86>>()(matched_object)))
                     return 86;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<87>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(271, addr(__selector_var), __casted_ptr);
    case 271:
                 auto matched_object = adjust_ptr<shape_kind<87>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<87>>()(matched_object)))
                     return 87;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<88>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(274, addr(__selector_var), __casted_ptr);
    case 274:
                 auto matched_object = adjust_ptr<shape_kind<88>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<88>>()(matched_object)))
                     return 88;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<89>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(277, addr(__selector_var), __casted_ptr);
    case 277:
                 auto matched_object = adjust_ptr<shape_kind<89>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<89>>()(matched_object)))
                     return 89;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<90>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(280, addr(__selector_var), __casted_ptr);
    case 280:
                 auto matched_object = adjust_ptr<shape_kind<90>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<90>>()(matched_object)))
                     return 90;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<91>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(283, addr(__selector_var), __casted_ptr);
    case 283:
                 auto matched_object = adjust_ptr<shape_kind<91>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<91>>()(matched_object)))
                     return 91;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<92>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(286, addr(__selector_var), __casted_ptr);
    case 286:
                 auto matched_object = adjust_ptr<shape_kind<92>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<92>>()(matched_object)))
                     return 92;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<93>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(289, addr(__selector_var), __casted_ptr);
    case 289:
                 auto matched_object = adjust_ptr<shape_kind<93>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<93>>()(matched_object)))
                     return 93;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<94>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(292, addr(__selector_var), __casted_ptr);
    case 292:
                 auto matched_object = adjust_ptr<shape_kind<94>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<94>>()(matched_object)))
                     return 94;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<95>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(295, addr(__selector_var), __casted_ptr);
    case 295:
                 auto matched_object = adjust_ptr<shape_kind<95>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<95>>()(matched_object)))
                     return 95;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<96>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(298, addr(__selector_var), __casted_ptr);
    case 298:
                 auto matched_object = adjust_ptr<shape_kind<96>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<96>>()(matched_object)))
                     return 96;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<97>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(301, addr(__selector_var), __casted_ptr);
    case 301:
                 auto matched_object = adjust_ptr<shape_kind<97>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<97>>()(matched_object)))
                     return 97;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<98>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(304, addr(__selector_var), __casted_ptr);
    case 304:
                 auto matched_object = adjust_ptr<shape_kind<98>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<98>>()(matched_object)))
                     return 98;
             }
             if ((__casted_ptr = memoized_cast<const shape_kind<99>*>(addr(__selector_var))))
             {
                 __vtbl2lines_map.update(307, addr(__selector_var), __casted_ptr);
    case 307:
                 auto matched_object = adjust_ptr<shape_kind<99>>(addr(__selector_var),__switch_info.offset);
                 if ((match<shape_kind<99>>()(matched_object)))
                     return 99;
             } __vtbl2lines_map.update(57, addr(__selector_var), 0);
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
#ifdef TRACE_PERFORMANCE
    cache_hits   = 0;
    cache_misses = 0;
#endif
    std::cout << "=================== Sequential Test ===================" << std::endl;

    for (int n = 0; n < K; ++n)
    {
        Shape* s = make_shape(n);
        std::vector<long long> timingsV(M);
        std::vector<long long> timingsM(M);

        for (int m = 0; m < M; ++m)
        {
            int a1 = 0,a2 = 0;

            time_stamp liStart1 = get_time_stamp();

            for (int i = 0; i < N; ++i)
                a1 += do_visit(*s);

            time_stamp liFinish1 = get_time_stamp();

            time_stamp liStart2 = get_time_stamp();

            for (int i = 0; i < N; ++i)
                a2 += do_match(*s);

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

        delete s;
    }

#ifdef TRACE_PERFORMANCE
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

#ifdef TRACE_PERFORMANCE
    cache_hits   = 0;
    cache_misses = 0;
#endif
    std::cout << "=================== Randomized Test ===================" << std::endl;

    std::vector<Shape*> shapes(N);

#ifdef TRACE_PERFORMANCE
    std::vector<int> distribution(K);
#endif

    for (int i = 0; i < N; ++i)
    {
        int n = rand()%K;
        shapes[i] = make_shape(n);
#ifdef TRACE_PERFORMANCE
        distribution[n]++;
#endif
    }

#ifdef TRACE_PERFORMANCE
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

    for (int m = 0; m < M; ++m)
    {
        int a1 = 0,a2 = 0;

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
    //    std::cout << "Insufficient timer resolution" << std::endl;

#ifdef TRACE_PERFORMANCE
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
    test_sequential();
    test_randomized();
}

#undef  FOR_EACH_MAX
