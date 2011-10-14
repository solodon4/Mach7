#include <algorithm>
#include <iostream>
#include <iomanip>
#include <numeric>
#include <vector>
#include <windows.h>
#include "match.hpp"

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
int do_match(Shape& s)
{
    SWITCH(s)
    {
    default:
    #define FOR_EACH_N(N) CASE(shape_kind<N>) return N;
    #include "loop_over_numbers.hpp"
    #undef  FOR_EACH_N
    }
    //assert(!"Inexhaustive search");
    return -1;
}
#else
int do_match(Shape& s)
{
    static vtbl2lines __vtbl2lines_map; 
    decltype(s)& __selector_var = s; 
    int choice = __vtbl2lines_map.get(&__selector_var);
    switch (choice)
    {
    default:
        __vtbl2lines_map.update(10, &__selector_var, memoized_cast<shape_kind<0>*>(&__selector_var));
    case 2*10:
        if (!match<shape_kind<0>>()(__selector_var))
        {
    case 2*10+1: ;
        }
        else
            return 0;
        __vtbl2lines_map.update(13, &__selector_var, memoized_cast<shape_kind<1>*>(&__selector_var));
    case 2*13:
        if (!match<shape_kind<1>>()(__selector_var))
        {
    case 2*13+1: ;
        }
        else
            return 1;
        __vtbl2lines_map.update(16, &__selector_var, memoized_cast<shape_kind<2>*>(&__selector_var));
    case 2*16:
        if (!match<shape_kind<2>>()(__selector_var))
        {
    case 2*16+1: ;
        }
        else
            return 2;
        __vtbl2lines_map.update(19, &__selector_var, memoized_cast<shape_kind<3>*>(&__selector_var));
    case 2*19:
        if (!match<shape_kind<3>>()(__selector_var))
        {
    case 2*19+1: ;
        }
        else
            return 3;
        __vtbl2lines_map.update(22, &__selector_var, memoized_cast<shape_kind<4>*>(&__selector_var));
    case 2*22:
        if (!match<shape_kind<4>>()(__selector_var))
        {
    case 2*22+1: ;
        }
        else
            return 4;
        __vtbl2lines_map.update(25, &__selector_var, memoized_cast<shape_kind<5>*>(&__selector_var));
    case 2*25:
        if (!match<shape_kind<5>>()(__selector_var))
        {
    case 2*25+1: ;
        }
        else
            return 5;
        __vtbl2lines_map.update(28, &__selector_var, memoized_cast<shape_kind<6>*>(&__selector_var));
    case 2*28:
        if (!match<shape_kind<6>>()(__selector_var))
        {
    case 2*28+1: ;
        }
        else
            return 6;
        __vtbl2lines_map.update(31, &__selector_var, memoized_cast<shape_kind<7>*>(&__selector_var));
    case 2*31:
        if (!match<shape_kind<7>>()(__selector_var))
        {
    case 2*31+1: ;
        }
        else
            return 7;
        __vtbl2lines_map.update(34, &__selector_var, memoized_cast<shape_kind<8>*>(&__selector_var));
    case 2*34:
        if (!match<shape_kind<8>>()(__selector_var))
        {
    case 2*34+1: ;
        }
        else
            return 8;
        __vtbl2lines_map.update(37, &__selector_var, memoized_cast<shape_kind<9>*>(&__selector_var));
    case 2*37:
        if (!match<shape_kind<9>>()(__selector_var))
        {
    case 2*37+1: ;
        }
        else
            return 9;
        __vtbl2lines_map.update(41, &__selector_var, memoized_cast<shape_kind<10>*>(&__selector_var));
    case 2*41:
        if (!match<shape_kind<10>>()(__selector_var))
        {
    case 2*41+1: ;
        }
        else
            return 10;
        __vtbl2lines_map.update(44, &__selector_var, memoized_cast<shape_kind<11>*>(&__selector_var));
    case 2*44:
        if (!match<shape_kind<11>>()(__selector_var))
        {
    case 2*44+1: ;
        }
        else
            return 11;
        __vtbl2lines_map.update(47, &__selector_var, memoized_cast<shape_kind<12>*>(&__selector_var));
    case 2*47:
        if (!match<shape_kind<12>>()(__selector_var))
        {
    case 2*47+1: ;
        }
        else
            return 12;
        __vtbl2lines_map.update(50, &__selector_var, memoized_cast<shape_kind<13>*>(&__selector_var));
    case 2*50:
        if (!match<shape_kind<13>>()(__selector_var))
        {
    case 2*50+1: ;
        }
        else
            return 13;
        __vtbl2lines_map.update(53, &__selector_var, memoized_cast<shape_kind<14>*>(&__selector_var));
    case 2*53:
        if (!match<shape_kind<14>>()(__selector_var))
        {
    case 2*53+1: ;
        }
        else
            return 14;
        __vtbl2lines_map.update(56, &__selector_var, memoized_cast<shape_kind<15>*>(&__selector_var));
    case 2*56:
        if (!match<shape_kind<15>>()(__selector_var))
        {
    case 2*56+1: ;
        }
        else
            return 15;
        __vtbl2lines_map.update(59, &__selector_var, memoized_cast<shape_kind<16>*>(&__selector_var));
    case 2*59:
        if (!match<shape_kind<16>>()(__selector_var))
        {
    case 2*59+1: ;
        }
        else
            return 16;
        __vtbl2lines_map.update(62, &__selector_var, memoized_cast<shape_kind<17>*>(&__selector_var));
    case 2*62:
        if (!match<shape_kind<17>>()(__selector_var))
        {
    case 2*62+1: ;
        }
        else
            return 17;
        __vtbl2lines_map.update(65, &__selector_var, memoized_cast<shape_kind<18>*>(&__selector_var));
    case 2*65:
        if (!match<shape_kind<18>>()(__selector_var))
        {
    case 2*65+1: ;
        }
        else
            return 18;
        __vtbl2lines_map.update(68, &__selector_var, memoized_cast<shape_kind<19>*>(&__selector_var));
    case 2*68:
        if (!match<shape_kind<19>>()(__selector_var))
        {
    case 2*68+1: ;
        }
        else
            return 19;
        __vtbl2lines_map.update(72, &__selector_var, memoized_cast<shape_kind<20>*>(&__selector_var));
    case 2*72:
        if (!match<shape_kind<20>>()(__selector_var))
        {
    case 2*72+1: ;
        }
        else
            return 20;
        __vtbl2lines_map.update(75, &__selector_var, memoized_cast<shape_kind<21>*>(&__selector_var));
    case 2*75:
        if (!match<shape_kind<21>>()(__selector_var))
        {
    case 2*75+1: ;
        }
        else
            return 21;
        __vtbl2lines_map.update(78, &__selector_var, memoized_cast<shape_kind<22>*>(&__selector_var));
    case 2*78:
        if (!match<shape_kind<22>>()(__selector_var))
        {
    case 2*78+1: ;
        }
        else
            return 22;
        __vtbl2lines_map.update(81, &__selector_var, memoized_cast<shape_kind<23>*>(&__selector_var));
    case 2*81:
        if (!match<shape_kind<23>>()(__selector_var))
        {
    case 2*81+1: ;
        }
        else
            return 23;
        __vtbl2lines_map.update(84, &__selector_var, memoized_cast<shape_kind<24>*>(&__selector_var));
    case 2*84:
        if (!match<shape_kind<24>>()(__selector_var))
        {
    case 2*84+1: ;
        }
        else
            return 24;
        __vtbl2lines_map.update(87, &__selector_var, memoized_cast<shape_kind<25>*>(&__selector_var));
    case 2*87:
        if (!match<shape_kind<25>>()(__selector_var))
        {
    case 2*87+1: ;
        }
        else
            return 25;
        __vtbl2lines_map.update(90, &__selector_var, memoized_cast<shape_kind<26>*>(&__selector_var));
    case 2*90:
        if (!match<shape_kind<26>>()(__selector_var))
        {
    case 2*90+1: ;
        }
        else
            return 26;
        __vtbl2lines_map.update(93, &__selector_var, memoized_cast<shape_kind<27>*>(&__selector_var));
    case 2*93:
        if (!match<shape_kind<27>>()(__selector_var))
        {
    case 2*93+1: ;
        }
        else
            return 27;
        __vtbl2lines_map.update(96, &__selector_var, memoized_cast<shape_kind<28>*>(&__selector_var));
    case 2*96:
        if (!match<shape_kind<28>>()(__selector_var))
        {
    case 2*96+1: ;
        }
        else
            return 28;
        __vtbl2lines_map.update(99, &__selector_var, memoized_cast<shape_kind<29>*>(&__selector_var));
    case 2*99:
        if (!match<shape_kind<29>>()(__selector_var))
        {
    case 2*99+1: ;
        }
        else
            return 29;
        __vtbl2lines_map.update(103, &__selector_var, memoized_cast<shape_kind<30>*>(&__selector_var));
    case 2*103:
        if (!match<shape_kind<30>>()(__selector_var))
        {
    case 2*103+1: ;
        }
        else
            return 30;
        __vtbl2lines_map.update(106, &__selector_var, memoized_cast<shape_kind<31>*>(&__selector_var));
    case 2*106:
        if (!match<shape_kind<31>>()(__selector_var))
        {
    case 2*106+1: ;
        }
        else
            return 31;
        __vtbl2lines_map.update(109, &__selector_var, memoized_cast<shape_kind<32>*>(&__selector_var));
    case 2*109:
        if (!match<shape_kind<32>>()(__selector_var))
        {
    case 2*109+1: ;
        }
        else
            return 32;
        __vtbl2lines_map.update(112, &__selector_var, memoized_cast<shape_kind<33>*>(&__selector_var));
    case 2*112:
        if (!match<shape_kind<33>>()(__selector_var))
        {
    case 2*112+1: ;
        }
        else
            return 33;
        __vtbl2lines_map.update(115, &__selector_var, memoized_cast<shape_kind<34>*>(&__selector_var));
    case 2*115:
        if (!match<shape_kind<34>>()(__selector_var))
        {
    case 2*115+1: ;
        }
        else
            return 34;
        __vtbl2lines_map.update(118, &__selector_var, memoized_cast<shape_kind<35>*>(&__selector_var));
    case 2*118:
        if (!match<shape_kind<35>>()(__selector_var))
        {
    case 2*118+1: ;
        }
        else
            return 35;
        __vtbl2lines_map.update(121, &__selector_var, memoized_cast<shape_kind<36>*>(&__selector_var));
    case 2*121:
        if (!match<shape_kind<36>>()(__selector_var))
        {
    case 2*121+1: ;
        }
        else
            return 36;
        __vtbl2lines_map.update(124, &__selector_var, memoized_cast<shape_kind<37>*>(&__selector_var));
    case 2*124:
        if (!match<shape_kind<37>>()(__selector_var))
        {
    case 2*124+1: ;
        }
        else
            return 37;
        __vtbl2lines_map.update(127, &__selector_var, memoized_cast<shape_kind<38>*>(&__selector_var));
    case 2*127:
        if (!match<shape_kind<38>>()(__selector_var))
        {
    case 2*127+1: ;
        }
        else
            return 38;
        __vtbl2lines_map.update(130, &__selector_var, memoized_cast<shape_kind<39>*>(&__selector_var));
    case 2*130:
        if (!match<shape_kind<39>>()(__selector_var))
        {
    case 2*130+1: ;
        }
        else
            return 39;
        __vtbl2lines_map.update(134, &__selector_var, memoized_cast<shape_kind<40>*>(&__selector_var));
    case 2*134:
        if (!match<shape_kind<40>>()(__selector_var))
        {
    case 2*134+1: ;
        }
        else
            return 40;
        __vtbl2lines_map.update(137, &__selector_var, memoized_cast<shape_kind<41>*>(&__selector_var));
    case 2*137:
        if (!match<shape_kind<41>>()(__selector_var))
        {
    case 2*137+1: ;
        }
        else
            return 41;
        __vtbl2lines_map.update(140, &__selector_var, memoized_cast<shape_kind<42>*>(&__selector_var));
    case 2*140:
        if (!match<shape_kind<42>>()(__selector_var))
        {
    case 2*140+1: ;
        }
        else
            return 42;
        __vtbl2lines_map.update(143, &__selector_var, memoized_cast<shape_kind<43>*>(&__selector_var));
    case 2*143:
        if (!match<shape_kind<43>>()(__selector_var))
        {
    case 2*143+1: ;
        }
        else
            return 43;
        __vtbl2lines_map.update(146, &__selector_var, memoized_cast<shape_kind<44>*>(&__selector_var));
    case 2*146:
        if (!match<shape_kind<44>>()(__selector_var))
        {
    case 2*146+1: ;
        }
        else
            return 44;
        __vtbl2lines_map.update(149, &__selector_var, memoized_cast<shape_kind<45>*>(&__selector_var));
    case 2*149:
        if (!match<shape_kind<45>>()(__selector_var))
        {
    case 2*149+1: ;
        }
        else
            return 45;
        __vtbl2lines_map.update(152, &__selector_var, memoized_cast<shape_kind<46>*>(&__selector_var));
    case 2*152:
        if (!match<shape_kind<46>>()(__selector_var))
        {
    case 2*152+1: ;
        }
        else
            return 46;
        __vtbl2lines_map.update(155, &__selector_var, memoized_cast<shape_kind<47>*>(&__selector_var));
    case 2*155:
        if (!match<shape_kind<47>>()(__selector_var))
        {
    case 2*155+1: ;
        }
        else
            return 47;
        __vtbl2lines_map.update(158, &__selector_var, memoized_cast<shape_kind<48>*>(&__selector_var));
    case 2*158:
        if (!match<shape_kind<48>>()(__selector_var))
        {
    case 2*158+1: ;
        }
        else
            return 48;
        __vtbl2lines_map.update(161, &__selector_var, memoized_cast<shape_kind<49>*>(&__selector_var));
    case 2*161:
        if (!match<shape_kind<49>>()(__selector_var))
        {
    case 2*161+1: ;
        }
        else
            return 49;
        __vtbl2lines_map.update(165, &__selector_var, memoized_cast<shape_kind<50>*>(&__selector_var));
    case 2*165:
        if (!match<shape_kind<50>>()(__selector_var))
        {
    case 2*165+1: ;
        }
        else
            return 50;
        __vtbl2lines_map.update(168, &__selector_var, memoized_cast<shape_kind<51>*>(&__selector_var));
    case 2*168:
        if (!match<shape_kind<51>>()(__selector_var))
        {
    case 2*168+1: ;
        }
        else
            return 51;
        __vtbl2lines_map.update(171, &__selector_var, memoized_cast<shape_kind<52>*>(&__selector_var));
    case 2*171:
        if (!match<shape_kind<52>>()(__selector_var))
        {
    case 2*171+1: ;
        }
        else
            return 52;
        __vtbl2lines_map.update(174, &__selector_var, memoized_cast<shape_kind<53>*>(&__selector_var));
    case 2*174:
        if (!match<shape_kind<53>>()(__selector_var))
        {
    case 2*174+1: ;
        }
        else
            return 53;
        __vtbl2lines_map.update(177, &__selector_var, memoized_cast<shape_kind<54>*>(&__selector_var));
    case 2*177:
        if (!match<shape_kind<54>>()(__selector_var))
        {
    case 2*177+1: ;
        }
        else
            return 54;
        __vtbl2lines_map.update(180, &__selector_var, memoized_cast<shape_kind<55>*>(&__selector_var));
    case 2*180:
        if (!match<shape_kind<55>>()(__selector_var))
        {
    case 2*180+1: ;
        }
        else
            return 55;
        __vtbl2lines_map.update(183, &__selector_var, memoized_cast<shape_kind<56>*>(&__selector_var));
    case 2*183:
        if (!match<shape_kind<56>>()(__selector_var))
        {
    case 2*183+1: ;
        }
        else
            return 56;
        __vtbl2lines_map.update(186, &__selector_var, memoized_cast<shape_kind<57>*>(&__selector_var));
    case 2*186:
        if (!match<shape_kind<57>>()(__selector_var))
        {
    case 2*186+1: ;
        }
        else
            return 57;
        __vtbl2lines_map.update(189, &__selector_var, memoized_cast<shape_kind<58>*>(&__selector_var));
    case 2*189:
        if (!match<shape_kind<58>>()(__selector_var))
        {
    case 2*189+1: ;
        }
        else
            return 58;
        __vtbl2lines_map.update(192, &__selector_var, memoized_cast<shape_kind<59>*>(&__selector_var));
    case 2*192:
        if (!match<shape_kind<59>>()(__selector_var))
        {
    case 2*192+1: ;
        }
        else
            return 59;
        __vtbl2lines_map.update(196, &__selector_var, memoized_cast<shape_kind<60>*>(&__selector_var));
    case 2*196:
        if (!match<shape_kind<60>>()(__selector_var))
        {
    case 2*196+1: ;
        }
        else
            return 60;
        __vtbl2lines_map.update(199, &__selector_var, memoized_cast<shape_kind<61>*>(&__selector_var));
    case 2*199:
        if (!match<shape_kind<61>>()(__selector_var))
        {
    case 2*199+1: ;
        }
        else
            return 61;
        __vtbl2lines_map.update(202, &__selector_var, memoized_cast<shape_kind<62>*>(&__selector_var));
    case 2*202:
        if (!match<shape_kind<62>>()(__selector_var))
        {
    case 2*202+1: ;
        }
        else
            return 62;
        __vtbl2lines_map.update(205, &__selector_var, memoized_cast<shape_kind<63>*>(&__selector_var));
    case 2*205:
        if (!match<shape_kind<63>>()(__selector_var))
        {
    case 2*205+1: ;
        }
        else
            return 63;
        __vtbl2lines_map.update(208, &__selector_var, memoized_cast<shape_kind<64>*>(&__selector_var));
    case 2*208:
        if (!match<shape_kind<64>>()(__selector_var))
        {
    case 2*208+1: ;
        }
        else
            return 64;
        __vtbl2lines_map.update(211, &__selector_var, memoized_cast<shape_kind<65>*>(&__selector_var));
    case 2*211:
        if (!match<shape_kind<65>>()(__selector_var))
        {
    case 2*211+1: ;
        }
        else
            return 65;
        __vtbl2lines_map.update(214, &__selector_var, memoized_cast<shape_kind<66>*>(&__selector_var));
    case 2*214:
        if (!match<shape_kind<66>>()(__selector_var))
        {
    case 2*214+1: ;
        }
        else
            return 66;
        __vtbl2lines_map.update(217, &__selector_var, memoized_cast<shape_kind<67>*>(&__selector_var));
    case 2*217:
        if (!match<shape_kind<67>>()(__selector_var))
        {
    case 2*217+1: ;
        }
        else
            return 67;
        __vtbl2lines_map.update(220, &__selector_var, memoized_cast<shape_kind<68>*>(&__selector_var));
    case 2*220:
        if (!match<shape_kind<68>>()(__selector_var))
        {
    case 2*220+1: ;
        }
        else
            return 68;
        __vtbl2lines_map.update(223, &__selector_var, memoized_cast<shape_kind<69>*>(&__selector_var));
    case 2*223:
        if (!match<shape_kind<69>>()(__selector_var))
        {
    case 2*223+1: ;
        }
        else
            return 69;
        __vtbl2lines_map.update(227, &__selector_var, memoized_cast<shape_kind<70>*>(&__selector_var));
    case 2*227:
        if (!match<shape_kind<70>>()(__selector_var))
        {
    case 2*227+1: ;
        }
        else
            return 70;
        __vtbl2lines_map.update(230, &__selector_var, memoized_cast<shape_kind<71>*>(&__selector_var));
    case 2*230:
        if (!match<shape_kind<71>>()(__selector_var))
        {
    case 2*230+1: ;
        }
        else
            return 71;
        __vtbl2lines_map.update(233, &__selector_var, memoized_cast<shape_kind<72>*>(&__selector_var));
    case 2*233:
        if (!match<shape_kind<72>>()(__selector_var))
        {
    case 2*233+1: ;
        }
        else
            return 72;
        __vtbl2lines_map.update(236, &__selector_var, memoized_cast<shape_kind<73>*>(&__selector_var));
    case 2*236:
        if (!match<shape_kind<73>>()(__selector_var))
        {
    case 2*236+1: ;
        }
        else
            return 73;
        __vtbl2lines_map.update(239, &__selector_var, memoized_cast<shape_kind<74>*>(&__selector_var));
    case 2*239:
        if (!match<shape_kind<74>>()(__selector_var))
        {
    case 2*239+1: ;
        }
        else
            return 74;
        __vtbl2lines_map.update(242, &__selector_var, memoized_cast<shape_kind<75>*>(&__selector_var));
    case 2*242:
        if (!match<shape_kind<75>>()(__selector_var))
        {
    case 2*242+1: ;
        }
        else
            return 75;
        __vtbl2lines_map.update(245, &__selector_var, memoized_cast<shape_kind<76>*>(&__selector_var));
    case 2*245:
        if (!match<shape_kind<76>>()(__selector_var))
        {
    case 2*245+1: ;
        }
        else
            return 76;
        __vtbl2lines_map.update(248, &__selector_var, memoized_cast<shape_kind<77>*>(&__selector_var));
    case 2*248:
        if (!match<shape_kind<77>>()(__selector_var))
        {
    case 2*248+1: ;
        }
        else
            return 77;
        __vtbl2lines_map.update(251, &__selector_var, memoized_cast<shape_kind<78>*>(&__selector_var));
    case 2*251:
        if (!match<shape_kind<78>>()(__selector_var))
        {
    case 2*251+1: ;
        }
        else
            return 78;
        __vtbl2lines_map.update(254, &__selector_var, memoized_cast<shape_kind<79>*>(&__selector_var));
    case 2*254:
        if (!match<shape_kind<79>>()(__selector_var))
        {
    case 2*254+1: ;
        }
        else
            return 79;
        __vtbl2lines_map.update(258, &__selector_var, memoized_cast<shape_kind<80>*>(&__selector_var));
    case 2*258:
        if (!match<shape_kind<80>>()(__selector_var))
        {
    case 2*258+1: ;
        }
        else
            return 80;
        __vtbl2lines_map.update(261, &__selector_var, memoized_cast<shape_kind<81>*>(&__selector_var));
    case 2*261:
        if (!match<shape_kind<81>>()(__selector_var))
        {
    case 2*261+1: ;
        }
        else
            return 81;
        __vtbl2lines_map.update(264, &__selector_var, memoized_cast<shape_kind<82>*>(&__selector_var));
    case 2*264:
        if (!match<shape_kind<82>>()(__selector_var))
        {
    case 2*264+1: ;
        }
        else
            return 82;
        __vtbl2lines_map.update(267, &__selector_var, memoized_cast<shape_kind<83>*>(&__selector_var));
    case 2*267:
        if (!match<shape_kind<83>>()(__selector_var))
        {
    case 2*267+1: ;
        }
        else
            return 83;
        __vtbl2lines_map.update(270, &__selector_var, memoized_cast<shape_kind<84>*>(&__selector_var));
    case 2*270:
        if (!match<shape_kind<84>>()(__selector_var))
        {
    case 2*270+1: ;
        }
        else
            return 84;
        __vtbl2lines_map.update(273, &__selector_var, memoized_cast<shape_kind<85>*>(&__selector_var));
    case 2*273:
        if (!match<shape_kind<85>>()(__selector_var))
        {
    case 2*273+1: ;
        }
        else
            return 85;
        __vtbl2lines_map.update(276, &__selector_var, memoized_cast<shape_kind<86>*>(&__selector_var));
    case 2*276:
        if (!match<shape_kind<86>>()(__selector_var))
        {
    case 2*276+1: ;
        }
        else
            return 86;
        __vtbl2lines_map.update(279, &__selector_var, memoized_cast<shape_kind<87>*>(&__selector_var));
    case 2*279:
        if (!match<shape_kind<87>>()(__selector_var))
        {
    case 2*279+1: ;
        }
        else
            return 87;
        __vtbl2lines_map.update(282, &__selector_var, memoized_cast<shape_kind<88>*>(&__selector_var));
    case 2*282:
        if (!match<shape_kind<88>>()(__selector_var))
        {
    case 2*282+1: ;
        }
        else
            return 88;
        __vtbl2lines_map.update(285, &__selector_var, memoized_cast<shape_kind<89>*>(&__selector_var));
    case 2*285:
        if (!match<shape_kind<89>>()(__selector_var))
        {
    case 2*285+1: ;
        }
        else
            return 89;
        __vtbl2lines_map.update(289, &__selector_var, memoized_cast<shape_kind<90>*>(&__selector_var));
    case 2*289:
        if (!match<shape_kind<90>>()(__selector_var))
        {
    case 2*289+1: ;
        }
        else
            return 90;
        __vtbl2lines_map.update(292, &__selector_var, memoized_cast<shape_kind<91>*>(&__selector_var));
    case 2*292:
        if (!match<shape_kind<91>>()(__selector_var))
        {
    case 2*292+1: ;
        }
        else
            return 91;
        __vtbl2lines_map.update(295, &__selector_var, memoized_cast<shape_kind<92>*>(&__selector_var));
    case 2*295:
        if (!match<shape_kind<92>>()(__selector_var))
        {
    case 2*295+1: ;
        }
        else
            return 92;
        __vtbl2lines_map.update(298, &__selector_var, memoized_cast<shape_kind<93>*>(&__selector_var));
    case 2*298:
        if (!match<shape_kind<93>>()(__selector_var))
        {
    case 2*298+1: ;
        }
        else
            return 93;
        __vtbl2lines_map.update(301, &__selector_var, memoized_cast<shape_kind<94>*>(&__selector_var));
    case 2*301:
        if (!match<shape_kind<94>>()(__selector_var))
        {
    case 2*301+1: ;
        }
        else
            return 94;
        __vtbl2lines_map.update(304, &__selector_var, memoized_cast<shape_kind<95>*>(&__selector_var));
    case 2*304:
        if (!match<shape_kind<95>>()(__selector_var))
        {
    case 2*304+1: ;
        }
        else
            return 95;
        __vtbl2lines_map.update(307, &__selector_var, memoized_cast<shape_kind<96>*>(&__selector_var));
    case 2*307:
        if (!match<shape_kind<96>>()(__selector_var))
        {
    case 2*307+1: ;
        }
        else
            return 96;
        __vtbl2lines_map.update(310, &__selector_var, memoized_cast<shape_kind<97>*>(&__selector_var));
    case 2*310:
        if (!match<shape_kind<97>>()(__selector_var))
        {
    case 2*310+1: ;
        }
        else
            return 97;
        __vtbl2lines_map.update(313, &__selector_var, memoized_cast<shape_kind<98>*>(&__selector_var));
    case 2*313:
        if (!match<shape_kind<98>>()(__selector_var))
        {
    case 2*313+1: ;
        }
        else
            return 98;
        __vtbl2lines_map.update(316, &__selector_var, memoized_cast<shape_kind<99>*>(&__selector_var));
    case 2*316:
        if (!match<shape_kind<99>>()(__selector_var))
        {
    case 2*316+1: ;
        }
        else
            return 99;
    }

    return -1;
}
#endif
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
const int M = 100;   // The amount of times time measuring is done

template <typename Container>
typename Container::value_type mean(const Container& c)
{
    return std::accumulate(c.begin(),c.end(),typename Container::value_type())/c.size();
}

template <typename Container>
typename Container::value_type deviation(const Container& c)
{
    typename Container::value_type m = mean(c);
    typename Container::value_type d = 0;

    for (typename Container::const_iterator p = c.begin(); p != c.end(); ++p)
        d += (*p-m)*(*p-m);

    return std::sqrt(double(d)/c.size());
}

long long display(const char* name, std::vector<long long>& timings, const LARGE_INTEGER& Freq)
{
    std::sort(timings.begin(), timings.end());
    long long min = timings.front();
    long long max = timings.back();
    long long avg = mean(timings);
    long long dev = deviation(timings);
    std::cout << name << " Time: ["
              << std::setw(4) << min*1000000/Freq.QuadPart << " -- " 
              << std::setw(4) << avg*1000000/Freq.QuadPart << " -- " 
              << std::setw(4) << max*1000000/Freq.QuadPart 
              << "] Dev = " << std::setw(4) << dev << std::endl;
    return avg;
}

#include <bitset> // For print out purposes only

void test_sequential()
{
#ifdef TRACE_PERFORMANCE
    cache_hits   = 0;
    cache_misses = 0;
#endif
    std::cout << "=================== Sequential Test ===================" << std::endl;
    LARGE_INTEGER Freq;

    QueryPerformanceFrequency(&Freq);

    for (int n = 0; n <= FOR_EACH_MAX; ++n)
    {
        Shape* s = make_shape(n);
        std::vector<long long> timingsV(M);
        std::vector<long long> timingsM(M);

        for (int m = 0; m < M; ++m)
        {
            LARGE_INTEGER liStart1, liFinish1, liStart2, liFinish2;
            int a1 = 0,a2 = 0;

            QueryPerformanceCounter(&liStart1);

            for (int i = 0; i < N; ++i)
                a1 += do_visit(*s);

            QueryPerformanceCounter(&liFinish1);

            QueryPerformanceCounter(&liStart2);

            for (int i = 0; i < N; ++i)
                a2 += do_match(*s);

            QueryPerformanceCounter(&liFinish2);

            assert(a1==a2);

            timingsV[m] = liFinish1.QuadPart-liStart1.QuadPart;
            timingsM[m] = liFinish2.QuadPart-liStart2.QuadPart;
        }

        long long avgV = display("AreaVis", timingsV, Freq);
        long long avgM = display("AreaMat", timingsM, Freq);
        std::cout << avgM*100/avgV-100 << "% slower" << std::endl;
#ifdef TRACE_PERFORMANCE
        std::cout << "Cache hits: " << cache_hits << "\tCache misses: " << cache_misses << std::endl;
        std::cout << "Common: " << std::bitset<32>(common) << std::endl
                  << "Differ: " << std::bitset<32>(differ) << std::endl;
#endif

        delete s;
    }
}

void test_randomized()
{
#ifdef TRACE_PERFORMANCE
    cache_hits   = 0;
    cache_misses = 0;
#endif
    std::cout << "=================== Randomized Test ===================" << std::endl;
    LARGE_INTEGER Freq;

    QueryPerformanceFrequency(&Freq);

    std::vector<Shape*> shapes(N);

    for (int i = 0; i < N; ++i)
    {
        int n = rand()%FOR_EACH_MAX;
        shapes[i] = make_shape(n);
    }

    std::vector<long long> timingsV(M);
    std::vector<long long> timingsM(M);

    for (int m = 0; m < M; ++m)
    {
        LARGE_INTEGER liStart1, liFinish1, liStart2, liFinish2;
        int a1 = 0,a2 = 0;

        QueryPerformanceCounter(&liStart1);

        for (int i = 0; i < N; ++i)
            a1 += do_visit(*shapes[i]);

        QueryPerformanceCounter(&liFinish1);

        QueryPerformanceCounter(&liStart2);

        for (int i = 0; i < N; ++i)
            a2 += do_match(*shapes[i]);

        QueryPerformanceCounter(&liFinish2);

        assert(a1==a2);
        timingsV[m] = liFinish1.QuadPart-liStart1.QuadPart;
        timingsM[m] = liFinish2.QuadPart-liStart2.QuadPart;
    }

    long long avgV = display("AreaVis", timingsV, Freq);
    long long avgM = display("AreaMat", timingsM, Freq);
    std::cout << avgM*100/avgV-100 << "% slower" << std::endl;
#ifdef TRACE_PERFORMANCE
    std::cout << "Cache hits: " << cache_hits << "\tCache misses: " << cache_misses << std::endl;
    std::cout << "Common: " << std::bitset<32>(common) << std::endl
              << "Differ: " << std::bitset<32>(differ) << std::endl;
#endif
}

int main()
{
    test_sequential();
    test_randomized();
}

#undef  FOR_EACH_MAX
