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
    static vtbl2lines<7> __vtbl2lines_map; 
    decltype(s)& __selector_var = s; 
    int choice = __vtbl2lines_map.get(addr(__selector_var));
    switch (choice)
    {
    default:{
            }
            if (memoized_cast<const shape_kind<0>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(10, addr(__selector_var));
    case 10:
                if (match<shape_kind<0>>()(__selector_var))
                    return 0; else return -42;
            } 
            if (memoized_cast<const shape_kind<1>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(13, addr(__selector_var));
    case 13:
                if (match<shape_kind<1>>()(__selector_var))
                    return 1; else return -42;
            } 
            if (memoized_cast<const shape_kind<2>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(16, addr(__selector_var));
    case 16:
                if (match<shape_kind<2>>()(__selector_var))
                    return 2; else return -42;
            } 
            if (memoized_cast<const shape_kind<3>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(19, addr(__selector_var));
    case 19:
                if (match<shape_kind<3>>()(__selector_var))
                    return 3; else return -42;
            } 
            if (memoized_cast<const shape_kind<4>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(22, addr(__selector_var));
    case 22:
                if (match<shape_kind<4>>()(__selector_var))
                    return 4; else return -42;
            } 
            if (memoized_cast<const shape_kind<5>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(25, addr(__selector_var));
    case 25:
                if (match<shape_kind<5>>()(__selector_var))
                    return 5; else return -42;
            } 
            if (memoized_cast<const shape_kind<6>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(28, addr(__selector_var));
    case 28:
                if (match<shape_kind<6>>()(__selector_var))
                    return 6; else return -42;
            } 
            if (memoized_cast<const shape_kind<7>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(31, addr(__selector_var));
    case 31:
                if (match<shape_kind<7>>()(__selector_var))
                    return 7; else return -42;
            } 
            if (memoized_cast<const shape_kind<8>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(34, addr(__selector_var));
    case 34:
                if (match<shape_kind<8>>()(__selector_var))
                    return 8; else return -42;
            } 
            if (memoized_cast<const shape_kind<9>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(37, addr(__selector_var));
    case 37:
                if (match<shape_kind<9>>()(__selector_var))
                    return 9; else return -42;
            } 
            if (memoized_cast<const shape_kind<10>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(41, addr(__selector_var));
    case 41:
                if (match<shape_kind<10>>()(__selector_var))
                    return 10; else return -42;
            } 
            if (memoized_cast<const shape_kind<11>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(44, addr(__selector_var));
    case 44:
                if (match<shape_kind<11>>()(__selector_var))
                    return 11; else return -42;
            } 
            if (memoized_cast<const shape_kind<12>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(47, addr(__selector_var));
    case 47:
                if (match<shape_kind<12>>()(__selector_var))
                    return 12; else return -42;
            } 
            if (memoized_cast<const shape_kind<13>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(50, addr(__selector_var));
    case 50:
                if (match<shape_kind<13>>()(__selector_var))
                    return 13; else return -42;
            } 
            if (memoized_cast<const shape_kind<14>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(53, addr(__selector_var));
    case 53:
                if (match<shape_kind<14>>()(__selector_var))
                    return 14; else return -42;
            } 
            if (memoized_cast<const shape_kind<15>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(56, addr(__selector_var));
    case 56:
                if (match<shape_kind<15>>()(__selector_var))
                    return 15; else return -42;
            } 
            if (memoized_cast<const shape_kind<16>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(59, addr(__selector_var));
    case 59:
                if (match<shape_kind<16>>()(__selector_var))
                    return 16; else return -42;
            } 
            if (memoized_cast<const shape_kind<17>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(62, addr(__selector_var));
    case 62:
                if (match<shape_kind<17>>()(__selector_var))
                    return 17; else return -42;
            } 
            if (memoized_cast<const shape_kind<18>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(65, addr(__selector_var));
    case 65:
                if (match<shape_kind<18>>()(__selector_var))
                    return 18; else return -42;
            } 
            if (memoized_cast<const shape_kind<19>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(68, addr(__selector_var));
    case 68:
                if (match<shape_kind<19>>()(__selector_var))
                    return 19; else return -42;
            } 
            if (memoized_cast<const shape_kind<20>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(72, addr(__selector_var));
    case 72:
                if (match<shape_kind<20>>()(__selector_var))
                    return 20; else return -42;
            } 
            if (memoized_cast<const shape_kind<21>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(75, addr(__selector_var));
    case 75:
                if (match<shape_kind<21>>()(__selector_var))
                    return 21; else return -42;
            } 
            if (memoized_cast<const shape_kind<22>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(78, addr(__selector_var));
    case 78:
                if (match<shape_kind<22>>()(__selector_var))
                    return 22; else return -42;
            } 
            if (memoized_cast<const shape_kind<23>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(81, addr(__selector_var));
    case 81:
                if (match<shape_kind<23>>()(__selector_var))
                    return 23; else return -42;
            } 
            if (memoized_cast<const shape_kind<24>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(84, addr(__selector_var));
    case 84:
                if (match<shape_kind<24>>()(__selector_var))
                    return 24; else return -42;
            } 
            if (memoized_cast<const shape_kind<25>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(87, addr(__selector_var));
    case 87:
                if (match<shape_kind<25>>()(__selector_var))
                    return 25; else return -42;
            } 
            if (memoized_cast<const shape_kind<26>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(90, addr(__selector_var));
    case 90:
                if (match<shape_kind<26>>()(__selector_var))
                    return 26; else return -42;
            } 
            if (memoized_cast<const shape_kind<27>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(93, addr(__selector_var));
    case 93:
                if (match<shape_kind<27>>()(__selector_var))
                    return 27; else return -42;
            } 
            if (memoized_cast<const shape_kind<28>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(96, addr(__selector_var));
    case 96:
                if (match<shape_kind<28>>()(__selector_var))
                    return 28; else return -42;
            } 
            if (memoized_cast<const shape_kind<29>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(99, addr(__selector_var));
    case 99:
                if (match<shape_kind<29>>()(__selector_var))
                    return 29; else return -42;
            } 
            if (memoized_cast<const shape_kind<30>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(103, addr(__selector_var));
    case 103:
                if (match<shape_kind<30>>()(__selector_var))
                    return 30; else return -42;
            } 
            if (memoized_cast<const shape_kind<31>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(106, addr(__selector_var));
    case 106:
                if (match<shape_kind<31>>()(__selector_var))
                    return 31; else return -42;
            } 
            if (memoized_cast<const shape_kind<32>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(109, addr(__selector_var));
    case 109:
                if (match<shape_kind<32>>()(__selector_var))
                    return 32; else return -42;
            } 
            if (memoized_cast<const shape_kind<33>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(112, addr(__selector_var));
    case 112:
                if (match<shape_kind<33>>()(__selector_var))
                    return 33; else return -42;
            } 
            if (memoized_cast<const shape_kind<34>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(115, addr(__selector_var));
    case 115:
                if (match<shape_kind<34>>()(__selector_var))
                    return 34; else return -42;
            } 
            if (memoized_cast<const shape_kind<35>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(118, addr(__selector_var));
    case 118:
                if (match<shape_kind<35>>()(__selector_var))
                    return 35; else return -42;
            } 
            if (memoized_cast<const shape_kind<36>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(121, addr(__selector_var));
    case 121:
                if (match<shape_kind<36>>()(__selector_var))
                    return 36; else return -42;
            } 
            if (memoized_cast<const shape_kind<37>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(124, addr(__selector_var));
    case 124:
                if (match<shape_kind<37>>()(__selector_var))
                    return 37; else return -42;
            } 
            if (memoized_cast<const shape_kind<38>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(127, addr(__selector_var));
    case 127:
                if (match<shape_kind<38>>()(__selector_var))
                    return 38; else return -42;
            } 
            if (memoized_cast<const shape_kind<39>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(130, addr(__selector_var));
    case 130:
                if (match<shape_kind<39>>()(__selector_var))
                    return 39; else return -42;
            } 
            if (memoized_cast<const shape_kind<40>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(134, addr(__selector_var));
    case 134:
                if (match<shape_kind<40>>()(__selector_var))
                    return 40; else return -42;
            } 
            if (memoized_cast<const shape_kind<41>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(137, addr(__selector_var));
    case 137:
                if (match<shape_kind<41>>()(__selector_var))
                    return 41; else return -42;
            } 
            if (memoized_cast<const shape_kind<42>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(140, addr(__selector_var));
    case 140:
                if (match<shape_kind<42>>()(__selector_var))
                    return 42; else return -42;
            } 
            if (memoized_cast<const shape_kind<43>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(143, addr(__selector_var));
    case 143:
                if (match<shape_kind<43>>()(__selector_var))
                    return 43; else return -42;
            } 
            if (memoized_cast<const shape_kind<44>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(146, addr(__selector_var));
    case 146:
                if (match<shape_kind<44>>()(__selector_var))
                    return 44; else return -42;
            } 
            if (memoized_cast<const shape_kind<45>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(149, addr(__selector_var));
    case 149:
                if (match<shape_kind<45>>()(__selector_var))
                    return 45; else return -42;
            } 
            if (memoized_cast<const shape_kind<46>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(152, addr(__selector_var));
    case 152:
                if (match<shape_kind<46>>()(__selector_var))
                    return 46; else return -42;
            } 
            if (memoized_cast<const shape_kind<47>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(155, addr(__selector_var));
    case 155:
                if (match<shape_kind<47>>()(__selector_var))
                    return 47; else return -42;
            } 
            if (memoized_cast<const shape_kind<48>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(158, addr(__selector_var));
    case 158:
                if (match<shape_kind<48>>()(__selector_var))
                    return 48; else return -42;
            } 
            if (memoized_cast<const shape_kind<49>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(161, addr(__selector_var));
    case 161:
                if (match<shape_kind<49>>()(__selector_var))
                    return 49; else return -42;
            } 
            if (memoized_cast<const shape_kind<50>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(165, addr(__selector_var));
    case 165:
                if (match<shape_kind<50>>()(__selector_var))
                    return 50; else return -42;
            } 
            if (memoized_cast<const shape_kind<51>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(168, addr(__selector_var));
    case 168:
                if (match<shape_kind<51>>()(__selector_var))
                    return 51; else return -42;
            } 
            if (memoized_cast<const shape_kind<52>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(171, addr(__selector_var));
    case 171:
                if (match<shape_kind<52>>()(__selector_var))
                    return 52; else return -42;
            } 
            if (memoized_cast<const shape_kind<53>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(174, addr(__selector_var));
    case 174:
                if (match<shape_kind<53>>()(__selector_var))
                    return 53; else return -42;
            } 
            if (memoized_cast<const shape_kind<54>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(177, addr(__selector_var));
    case 177:
                if (match<shape_kind<54>>()(__selector_var))
                    return 54; else return -42;
            } 
            if (memoized_cast<const shape_kind<55>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(180, addr(__selector_var));
    case 180:
                if (match<shape_kind<55>>()(__selector_var))
                    return 55; else return -42;
            } 
            if (memoized_cast<const shape_kind<56>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(183, addr(__selector_var));
    case 183:
                if (match<shape_kind<56>>()(__selector_var))
                    return 56; else return -42;
            } 
            if (memoized_cast<const shape_kind<57>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(186, addr(__selector_var));
    case 186:
                if (match<shape_kind<57>>()(__selector_var))
                    return 57; else return -42;
            } 
            if (memoized_cast<const shape_kind<58>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(189, addr(__selector_var));
    case 189:
                if (match<shape_kind<58>>()(__selector_var))
                    return 58; else return -42;
            } 
            if (memoized_cast<const shape_kind<59>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(192, addr(__selector_var));
    case 192:
                if (match<shape_kind<59>>()(__selector_var))
                    return 59; else return -42;
            } 
            if (memoized_cast<const shape_kind<60>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(196, addr(__selector_var));
    case 196:
                if (match<shape_kind<60>>()(__selector_var))
                    return 60; else return -42;
            } 
            if (memoized_cast<const shape_kind<61>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(199, addr(__selector_var));
    case 199:
                if (match<shape_kind<61>>()(__selector_var))
                    return 61; else return -42;
            } 
            if (memoized_cast<const shape_kind<62>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(202, addr(__selector_var));
    case 202:
                if (match<shape_kind<62>>()(__selector_var))
                    return 62; else return -42;
            } 
            if (memoized_cast<const shape_kind<63>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(205, addr(__selector_var));
    case 205:
                if (match<shape_kind<63>>()(__selector_var))
                    return 63; else return -42;
            } 
            if (memoized_cast<const shape_kind<64>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(208, addr(__selector_var));
    case 208:
                if (match<shape_kind<64>>()(__selector_var))
                    return 64; else return -42;
            } 
            if (memoized_cast<const shape_kind<65>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(211, addr(__selector_var));
    case 211:
                if (match<shape_kind<65>>()(__selector_var))
                    return 65; else return -42;
            } 
            if (memoized_cast<const shape_kind<66>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(214, addr(__selector_var));
    case 214:
                if (match<shape_kind<66>>()(__selector_var))
                    return 66; else return -42;
            } 
            if (memoized_cast<const shape_kind<67>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(217, addr(__selector_var));
    case 217:
                if (match<shape_kind<67>>()(__selector_var))
                    return 67; else return -42;
            } 
            if (memoized_cast<const shape_kind<68>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(220, addr(__selector_var));
    case 220:
                if (match<shape_kind<68>>()(__selector_var))
                    return 68; else return -42;
            } 
            if (memoized_cast<const shape_kind<69>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(223, addr(__selector_var));
    case 223:
                if (match<shape_kind<69>>()(__selector_var))
                    return 69; else return -42;
            } 
            if (memoized_cast<const shape_kind<70>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(227, addr(__selector_var));
    case 227:
                if (match<shape_kind<70>>()(__selector_var))
                    return 70; else return -42;
            } 
            if (memoized_cast<const shape_kind<71>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(230, addr(__selector_var));
    case 230:
                if (match<shape_kind<71>>()(__selector_var))
                    return 71; else return -42;
            } 
            if (memoized_cast<const shape_kind<72>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(233, addr(__selector_var));
    case 233:
                if (match<shape_kind<72>>()(__selector_var))
                    return 72; else return -42;
            } 
            if (memoized_cast<const shape_kind<73>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(236, addr(__selector_var));
    case 236:
                if (match<shape_kind<73>>()(__selector_var))
                    return 73; else return -42;
            } 
            if (memoized_cast<const shape_kind<74>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(239, addr(__selector_var));
    case 239:
                if (match<shape_kind<74>>()(__selector_var))
                    return 74; else return -42;
            } 
            if (memoized_cast<const shape_kind<75>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(242, addr(__selector_var));
    case 242:
                if (match<shape_kind<75>>()(__selector_var))
                    return 75; else return -42;
            } 
            if (memoized_cast<const shape_kind<76>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(245, addr(__selector_var));
    case 245:
                if (match<shape_kind<76>>()(__selector_var))
                    return 76; else return -42;
            } 
            if (memoized_cast<const shape_kind<77>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(248, addr(__selector_var));
    case 248:
                if (match<shape_kind<77>>()(__selector_var))
                    return 77; else return -42;
            } 
            if (memoized_cast<const shape_kind<78>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(251, addr(__selector_var));
    case 251:
                if (match<shape_kind<78>>()(__selector_var))
                    return 78; else return -42;
            } 
            if (memoized_cast<const shape_kind<79>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(254, addr(__selector_var));
    case 254:
                if (match<shape_kind<79>>()(__selector_var))
                    return 79; else return -42;
            } 
            if (memoized_cast<const shape_kind<80>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(258, addr(__selector_var));
    case 258:
                if (match<shape_kind<80>>()(__selector_var))
                    return 80; else return -42;
            } 
            if (memoized_cast<const shape_kind<81>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(261, addr(__selector_var));
    case 261:
                if (match<shape_kind<81>>()(__selector_var))
                    return 81; else return -42;
            } 
            if (memoized_cast<const shape_kind<82>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(264, addr(__selector_var));
    case 264:
                if (match<shape_kind<82>>()(__selector_var))
                    return 82; else return -42;
            } 
            if (memoized_cast<const shape_kind<83>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(267, addr(__selector_var));
    case 267:
                if (match<shape_kind<83>>()(__selector_var))
                    return 83; else return -42;
            } 
            if (memoized_cast<const shape_kind<84>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(270, addr(__selector_var));
    case 270:
                if (match<shape_kind<84>>()(__selector_var))
                    return 84; else return -42;
            } 
            if (memoized_cast<const shape_kind<85>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(273, addr(__selector_var));
    case 273:
                if (match<shape_kind<85>>()(__selector_var))
                    return 85; else return -42;
            } 
            if (memoized_cast<const shape_kind<86>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(276, addr(__selector_var));
    case 276:
                if (match<shape_kind<86>>()(__selector_var))
                    return 86; else return -42;
            } 
            if (memoized_cast<const shape_kind<87>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(279, addr(__selector_var));
    case 279:
                if (match<shape_kind<87>>()(__selector_var))
                    return 87; else return -42;
            } 
            if (memoized_cast<const shape_kind<88>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(282, addr(__selector_var));
    case 282:
                if (match<shape_kind<88>>()(__selector_var))
                    return 88; else return -42;
            } 
            if (memoized_cast<const shape_kind<89>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(285, addr(__selector_var));
    case 285:
                if (match<shape_kind<89>>()(__selector_var))
                    return 89; else return -42;
            } 
            if (memoized_cast<const shape_kind<90>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(289, addr(__selector_var));
    case 289:
                if (match<shape_kind<90>>()(__selector_var))
                    return 90; else return -42;
            } 
            if (memoized_cast<const shape_kind<91>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(292, addr(__selector_var));
    case 292:
                if (match<shape_kind<91>>()(__selector_var))
                    return 91; else return -42;
            } 
            if (memoized_cast<const shape_kind<92>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(295, addr(__selector_var));
    case 295:
                if (match<shape_kind<92>>()(__selector_var))
                    return 92; else return -42;
            } 
            if (memoized_cast<const shape_kind<93>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(298, addr(__selector_var));
    case 298:
                if (match<shape_kind<93>>()(__selector_var))
                    return 93; else return -42;
            } 
            if (memoized_cast<const shape_kind<94>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(301, addr(__selector_var));
    case 301:
                if (match<shape_kind<94>>()(__selector_var))
                    return 94; else return -42;
            } 
            if (memoized_cast<const shape_kind<95>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(304, addr(__selector_var));
    case 304:
                if (match<shape_kind<95>>()(__selector_var))
                    return 95; else return -42;
            } 
            if (memoized_cast<const shape_kind<96>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(307, addr(__selector_var));
    case 307:
                if (match<shape_kind<96>>()(__selector_var))
                    return 96; else return -42;
            } 
            if (memoized_cast<const shape_kind<97>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(310, addr(__selector_var));
    case 310:
                if (match<shape_kind<97>>()(__selector_var))
                    return 97; else return -42;
            } 
            if (memoized_cast<const shape_kind<98>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(313, addr(__selector_var));
    case 313:
                if (match<shape_kind<98>>()(__selector_var))
                    return 98; else return -42;
            } 
            if (memoized_cast<const shape_kind<99>*>(addr(__selector_var)))
            {
                __vtbl2lines_map.update(316, addr(__selector_var));
    case 316:
                if (match<shape_kind<99>>()(__selector_var))
                    return 99; else return -42;
            } 
            __vtbl2lines_map.update(319, addr(__selector_var));
    case 319: ;

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

            XTL_VERIFY(a1==a2);

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

        XTL_VERIFY(a1==a2);
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
