// MyExecRefsDll.cpp
// compile with: /EHsc /link MathFuncsDll.lib

#include <iostream>
#include "common.hpp"
#include "SharedDll1.hpp"

using namespace std;

//------------------------------------------------------------------------------

#if 1
XTL_DO_NOT_INLINE_BEGIN
size_t do_match(const Shape& s)
{
    MatchP(s)
    {
        #define FOR_EACH_MAX  NUMBER_OF_DERIVED-1
        #define FOR_EACH_N(N) CaseP(shape_kind<N>) return N;
        #include "loop_over_numbers.hpp"
        #undef  FOR_EACH_N
        #undef  FOR_EACH_MAX
    }
    EndMatchP
    return -1;
}
XTL_DO_NOT_INLINE_END
#else
XTL_DO_NOT_INLINE_BEGIN
size_t do_match(const Shape& s)
{
    {
        static vtblmap<type_switch_info&,requires_bits<100>::value> __vtbl2lines_map;
        auto const __selector_ptr = addr(s);
        ;
        const void* __casted_ptr;
        type_switch_info& __switch_info = __vtbl2lines_map.get(__selector_ptr);
        switch (__switch_info.line) 
        {
        default:
            {
                {
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<0>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 2;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 2:
                typedef shape_kind<0> target_type;
                auto matched = adjust_ptr<shape_kind<0>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 0;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<1>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 3;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 3:
                typedef shape_kind<1> target_type;
                auto matched = adjust_ptr<shape_kind<1>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 1;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<2>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 4;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 4:
                typedef shape_kind<2> target_type;
                auto matched = adjust_ptr<shape_kind<2>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 2;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<3>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 5;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 5:
                typedef shape_kind<3> target_type;
                auto matched = adjust_ptr<shape_kind<3>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 3;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<4>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 6;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 6:
                typedef shape_kind<4> target_type;
                auto matched = adjust_ptr<shape_kind<4>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 4;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<5>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 7;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 7:
                typedef shape_kind<5> target_type;
                auto matched = adjust_ptr<shape_kind<5>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 5;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<6>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 8;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 8:
                typedef shape_kind<6> target_type;
                auto matched = adjust_ptr<shape_kind<6>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 6;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<7>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 9;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 9:
                typedef shape_kind<7> target_type;
                auto matched = adjust_ptr<shape_kind<7>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 7;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<8>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 10;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 10:
                typedef shape_kind<8> target_type;
                auto matched = adjust_ptr<shape_kind<8>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 8;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<9>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 11;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 11:
                typedef shape_kind<9> target_type;
                auto matched = adjust_ptr<shape_kind<9>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 9;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<10>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 12;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 12:
                typedef shape_kind<10> target_type;
                auto matched = adjust_ptr<shape_kind<10>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 10;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<11>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 13;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 13:
                typedef shape_kind<11> target_type;
                auto matched = adjust_ptr<shape_kind<11>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 11;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<12>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 14;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 14:
                typedef shape_kind<12> target_type;
                auto matched = adjust_ptr<shape_kind<12>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 12;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<13>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 15;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 15:
                typedef shape_kind<13> target_type;
                auto matched = adjust_ptr<shape_kind<13>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 13;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<14>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 16;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 16:
                typedef shape_kind<14> target_type;
                auto matched = adjust_ptr<shape_kind<14>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 14;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<15>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 17;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 17:
                typedef shape_kind<15> target_type;
                auto matched = adjust_ptr<shape_kind<15>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 15;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<16>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 18;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 18:
                typedef shape_kind<16> target_type;
                auto matched = adjust_ptr<shape_kind<16>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 16;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<17>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 19;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 19:
                typedef shape_kind<17> target_type;
                auto matched = adjust_ptr<shape_kind<17>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 17;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<18>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 20;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 20:
                typedef shape_kind<18> target_type;
                auto matched = adjust_ptr<shape_kind<18>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 18;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<19>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 21;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 21:
                typedef shape_kind<19> target_type;
                auto matched = adjust_ptr<shape_kind<19>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 19;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<20>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 22;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 22:
                typedef shape_kind<20> target_type;
                auto matched = adjust_ptr<shape_kind<20>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 20;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<21>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 23;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 23:
                typedef shape_kind<21> target_type;
                auto matched = adjust_ptr<shape_kind<21>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 21;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<22>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 24;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 24:
                typedef shape_kind<22> target_type;
                auto matched = adjust_ptr<shape_kind<22>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 22;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<23>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 25;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 25:
                typedef shape_kind<23> target_type;
                auto matched = adjust_ptr<shape_kind<23>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 23;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<24>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 26;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 26:
                typedef shape_kind<24> target_type;
                auto matched = adjust_ptr<shape_kind<24>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 24;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<25>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 27;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 27:
                typedef shape_kind<25> target_type;
                auto matched = adjust_ptr<shape_kind<25>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 25;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<26>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 28;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 28:
                typedef shape_kind<26> target_type;
                auto matched = adjust_ptr<shape_kind<26>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 26;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<27>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 29;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 29:
                typedef shape_kind<27> target_type;
                auto matched = adjust_ptr<shape_kind<27>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 27;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<28>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 30;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 30:
                typedef shape_kind<28> target_type;
                auto matched = adjust_ptr<shape_kind<28>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 28;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<29>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 31;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 31:
                typedef shape_kind<29> target_type;
                auto matched = adjust_ptr<shape_kind<29>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 29;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<30>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 32;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 32:
                typedef shape_kind<30> target_type;
                auto matched = adjust_ptr<shape_kind<30>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 30;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<31>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 33;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 33:
                typedef shape_kind<31> target_type;
                auto matched = adjust_ptr<shape_kind<31>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 31;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<32>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 34;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 34:
                typedef shape_kind<32> target_type;
                auto matched = adjust_ptr<shape_kind<32>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 32;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<33>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 35;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 35:
                typedef shape_kind<33> target_type;
                auto matched = adjust_ptr<shape_kind<33>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 33;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<34>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 36;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 36:
                typedef shape_kind<34> target_type;
                auto matched = adjust_ptr<shape_kind<34>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 34;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<35>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 37;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 37:
                typedef shape_kind<35> target_type;
                auto matched = adjust_ptr<shape_kind<35>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 35;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<36>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 38;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 38:
                typedef shape_kind<36> target_type;
                auto matched = adjust_ptr<shape_kind<36>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 36;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<37>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 39;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 39:
                typedef shape_kind<37> target_type;
                auto matched = adjust_ptr<shape_kind<37>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 37;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<38>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 40;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 40:
                typedef shape_kind<38> target_type;
                auto matched = adjust_ptr<shape_kind<38>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 38;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<39>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 41;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 41:
                typedef shape_kind<39> target_type;
                auto matched = adjust_ptr<shape_kind<39>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 39;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<40>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 42;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 42:
                typedef shape_kind<40> target_type;
                auto matched = adjust_ptr<shape_kind<40>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 40;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<41>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 43;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 43:
                typedef shape_kind<41> target_type;
                auto matched = adjust_ptr<shape_kind<41>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 41;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<42>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 44;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 44:
                typedef shape_kind<42> target_type;
                auto matched = adjust_ptr<shape_kind<42>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 42;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<43>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 45;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 45:
                typedef shape_kind<43> target_type;
                auto matched = adjust_ptr<shape_kind<43>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 43;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<44>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 46;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 46:
                typedef shape_kind<44> target_type;
                auto matched = adjust_ptr<shape_kind<44>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 44;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<45>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 47;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 47:
                typedef shape_kind<45> target_type;
                auto matched = adjust_ptr<shape_kind<45>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 45;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<46>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 48;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 48:
                typedef shape_kind<46> target_type;
                auto matched = adjust_ptr<shape_kind<46>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 46;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<47>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 49;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 49:
                typedef shape_kind<47> target_type;
                auto matched = adjust_ptr<shape_kind<47>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 47;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<48>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 50;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 50:
                typedef shape_kind<48> target_type;
                auto matched = adjust_ptr<shape_kind<48>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 48;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<49>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 51;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 51:
                typedef shape_kind<49> target_type;
                auto matched = adjust_ptr<shape_kind<49>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 49;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<50>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 52;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 52:
                typedef shape_kind<50> target_type;
                auto matched = adjust_ptr<shape_kind<50>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 50;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<51>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 53;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 53:
                typedef shape_kind<51> target_type;
                auto matched = adjust_ptr<shape_kind<51>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 51;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<52>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 54;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 54:
                typedef shape_kind<52> target_type;
                auto matched = adjust_ptr<shape_kind<52>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 52;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<53>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 55;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 55:
                typedef shape_kind<53> target_type;
                auto matched = adjust_ptr<shape_kind<53>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 53;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<54>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 56;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 56:
                typedef shape_kind<54> target_type;
                auto matched = adjust_ptr<shape_kind<54>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 54;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<55>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 57;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 57:
                typedef shape_kind<55> target_type;
                auto matched = adjust_ptr<shape_kind<55>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 55;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<56>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 58;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 58:
                typedef shape_kind<56> target_type;
                auto matched = adjust_ptr<shape_kind<56>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 56;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<57>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 59;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 59:
                typedef shape_kind<57> target_type;
                auto matched = adjust_ptr<shape_kind<57>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 57;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<58>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 60;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 60:
                typedef shape_kind<58> target_type;
                auto matched = adjust_ptr<shape_kind<58>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 58;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<59>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 61;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 61:
                typedef shape_kind<59> target_type;
                auto matched = adjust_ptr<shape_kind<59>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 59;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<60>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 62;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 62:
                typedef shape_kind<60> target_type;
                auto matched = adjust_ptr<shape_kind<60>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 60;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<61>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 63;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 63:
                typedef shape_kind<61> target_type;
                auto matched = adjust_ptr<shape_kind<61>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 61;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<62>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 64;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 64:
                typedef shape_kind<62> target_type;
                auto matched = adjust_ptr<shape_kind<62>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 62;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<63>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 65;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 65:
                typedef shape_kind<63> target_type;
                auto matched = adjust_ptr<shape_kind<63>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 63;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<64>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 66;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 66:
                typedef shape_kind<64> target_type;
                auto matched = adjust_ptr<shape_kind<64>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 64;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<65>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 67;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 67:
                typedef shape_kind<65> target_type;
                auto matched = adjust_ptr<shape_kind<65>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 65;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<66>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 68;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 68:
                typedef shape_kind<66> target_type;
                auto matched = adjust_ptr<shape_kind<66>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 66;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<67>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 69;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 69:
                typedef shape_kind<67> target_type;
                auto matched = adjust_ptr<shape_kind<67>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 67;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<68>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 70;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 70:
                typedef shape_kind<68> target_type;
                auto matched = adjust_ptr<shape_kind<68>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 68;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<69>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 71;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 71:
                typedef shape_kind<69> target_type;
                auto matched = adjust_ptr<shape_kind<69>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 69;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<70>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 72;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 72:
                typedef shape_kind<70> target_type;
                auto matched = adjust_ptr<shape_kind<70>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 70;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<71>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 73;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 73:
                typedef shape_kind<71> target_type;
                auto matched = adjust_ptr<shape_kind<71>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 71;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<72>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 74;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 74:
                typedef shape_kind<72> target_type;
                auto matched = adjust_ptr<shape_kind<72>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 72;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<73>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 75;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 75:
                typedef shape_kind<73> target_type;
                auto matched = adjust_ptr<shape_kind<73>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 73;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<74>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 76;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 76:
                typedef shape_kind<74> target_type;
                auto matched = adjust_ptr<shape_kind<74>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 74;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<75>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 77;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 77:
                typedef shape_kind<75> target_type;
                auto matched = adjust_ptr<shape_kind<75>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 75;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<76>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 78;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 78:
                typedef shape_kind<76> target_type;
                auto matched = adjust_ptr<shape_kind<76>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 76;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<77>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 79;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 79:
                typedef shape_kind<77> target_type;
                auto matched = adjust_ptr<shape_kind<77>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 77;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<78>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 80;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 80:
                typedef shape_kind<78> target_type;
                auto matched = adjust_ptr<shape_kind<78>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 78;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<79>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 81;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 81:
                typedef shape_kind<79> target_type;
                auto matched = adjust_ptr<shape_kind<79>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 79;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<80>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 82;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 82:
                typedef shape_kind<80> target_type;
                auto matched = adjust_ptr<shape_kind<80>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 80;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<81>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 83;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 83:
                typedef shape_kind<81> target_type;
                auto matched = adjust_ptr<shape_kind<81>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 81;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<82>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 84;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 84:
                typedef shape_kind<82> target_type;
                auto matched = adjust_ptr<shape_kind<82>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 82;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<83>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 85;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 85:
                typedef shape_kind<83> target_type;
                auto matched = adjust_ptr<shape_kind<83>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 83;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<84>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 86;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 86:
                typedef shape_kind<84> target_type;
                auto matched = adjust_ptr<shape_kind<84>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 84;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<85>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 87;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 87:
                typedef shape_kind<85> target_type;
                auto matched = adjust_ptr<shape_kind<85>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 85;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<86>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 88;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 88:
                typedef shape_kind<86> target_type;
                auto matched = adjust_ptr<shape_kind<86>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 86;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<87>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 89;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 89:
                typedef shape_kind<87> target_type;
                auto matched = adjust_ptr<shape_kind<87>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 87;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<88>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 90;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 90:
                typedef shape_kind<88> target_type;
                auto matched = adjust_ptr<shape_kind<88>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 88;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<89>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 91;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 91:
                typedef shape_kind<89> target_type;
                auto matched = adjust_ptr<shape_kind<89>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 89;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<90>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 92;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 92:
                typedef shape_kind<90> target_type;
                auto matched = adjust_ptr<shape_kind<90>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 90;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<91>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 93;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 93:
                typedef shape_kind<91> target_type;
                auto matched = adjust_ptr<shape_kind<91>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 91;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<92>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 94;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 94:
                typedef shape_kind<92> target_type;
                auto matched = adjust_ptr<shape_kind<92>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 92;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<93>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 95;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 95:
                typedef shape_kind<93> target_type;
                auto matched = adjust_ptr<shape_kind<93>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 93;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<94>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 96;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 96:
                typedef shape_kind<94> target_type;
                auto matched = adjust_ptr<shape_kind<94>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 94;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<95>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 97;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 97:
                typedef shape_kind<95> target_type;
                auto matched = adjust_ptr<shape_kind<95>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 95;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<96>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 98;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 98:
                typedef shape_kind<96> target_type;
                auto matched = adjust_ptr<shape_kind<96>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 96;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<97>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 99;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 99:
                typedef shape_kind<97> target_type;
                auto matched = adjust_ptr<shape_kind<97>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 97;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<98>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 100;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 100:
                typedef shape_kind<98> target_type;
                auto matched = adjust_ptr<shape_kind<98>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 98;
                }
            }
            if ((__casted_ptr = dynamic_cast<const shape_kind<99>*>(__selector_ptr))) 
            {
                if (((__switch_info.line == 0))) 
                {
                    __switch_info.line = 101;
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);
                }
        case 101:
                typedef shape_kind<99> target_type;
                auto matched = adjust_ptr<shape_kind<99>>(__selector_ptr,__switch_info.offset);
                (void)matched;
                ;
                {
                    return 99;
                }
            }
            if (((__switch_info.line == 0))) 
            {
                __switch_info.line = 102;
            }
        case 102:
                ;
        }
    }
    return -1;
}
XTL_DO_NOT_INLINE_END
#endif

//------------------------------------------------------------------------------

XTL_DO_NOT_INLINE_BEGIN
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
XTL_DO_NOT_INLINE_END

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

const size_t N = 10000; // The amount of times visitor and matching procedure is invoked in one time measuring
const size_t M = 101;   // The amount of times time measuring is done
const size_t K = NUMBER_OF_DERIVED; // The amount of cases we have in hierarchy

//------------------------------------------------------------------------------

template <typename Container>
typename Container::value_type mean(const Container& c)
{
    return std::accumulate(c.begin(),c.end(),typename Container::value_type())/c.size();
}

//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------

int relative_performance(long long v, long long m)
{
    if (XTL_UNLIKELY(v <= 0 || m <= 0))
    {
        std::cout << "ERROR: Insufficient timer resolution. Increase number of iterations N" << std::endl;
        exit(42);
    }
    else
    if (XTL_UNLIKELY(v <= m))
    {
        int percent = int(m*100/v-100);
        std::cout << "\t\t" << percent << "% slower" << std::endl;
        return +percent; // Positive number indicates how many percents slower we are 
    }
    else
    {
        int percent = int(v*100/m-100);
        std::cout << "\t\t" << percent << "% faster" << std::endl;
        return -percent; // Negative number indicates how many percents faster we are 
    }
}

//------------------------------------------------------------------------------

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
              << std::setw(4) << microseconds(dev)
#if   defined(XTL_TIMING_METHOD_1)
              // FIX: 1000 is heuristic here for my laptop. QueryPerformanceCounter doesn't specify how it is related to cycles!
              << " Cycles/iteration: ["
              << std::setw(4) << min*1000/N << " -- " 
              << std::setw(4) << avg*1000/N << "/" 
              << std::setw(4) << med*1000/N << " -- "
              << std::setw(4) << max*1000/N << "]"
#elif defined(XTL_TIMING_METHOD_2)
              << " Cycles/iteration: ["
              << std::setw(4) << min/N << " -- " 
              << std::setw(4) << avg/N << "/" 
              << std::setw(4) << med/N << " -- "
              << std::setw(4) << max/N << "]"
#endif
              << std::endl;

    return med;
}

//------------------------------------------------------------------------------

int test_sequential()
{
    std::cout << "=================== Sequential Test ===================" << std::endl;

    size_t a1 = 0,a2 = 0;
    std::vector<int> percentages(K); // Final verdict of medians for each of the K experiments

    for (size_t n = 0; n < K; ++n)
    {
        std::vector<Shape*> shapes(N);

        for (size_t i = 0; i < N; ++i)
        {
            size_t n = i%K;
            shapes[i] = i%2 ? make_shape(n) : make_shape_dll(n);
        }

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

        long long avgV = display("AreaVisSeq", timingsV);
        long long avgM = display("AreaMatSeq", timingsM);
        percentages[n] = relative_performance(avgV, avgM);

        for (size_t i = 0; i < N; ++i)
        {
            delete shapes[i];
            shapes[i] = 0;
        }
    }

    if (a1 != a2)
    {
        std::cout << "ERROR: Invariant " << a1 << "==" << a2 << " doesn't hold." << std::endl;
        exit(42);
    }

    std::sort(percentages.begin(), percentages.end());
    return percentages[percentages.size()/2];
}

//------------------------------------------------------------------------------

int test_randomized()
{
#if !defined(NO_RANDOMIZATION)
    srand (unsigned(get_time_stamp()/get_frequency())); // Randomize pseudo random number generator
#endif
    std::cout << "=================== Randomized Test ===================" << std::endl;

    size_t a1 = 0,a2 = 0;
    std::vector<int> percentages(K); // Final verdict of medians for each of the K experiments

    for (size_t n = 0; n < K; ++n)
    {
        std::vector<Shape*> shapes(N);
        XTL_TRACE_PERFORMANCE_ONLY(std::vector<size_t> distribution(K));

        for (size_t i = 0; i < N; ++i)
        {
            size_t n = rand()%K;
            XTL_TRACE_PERFORMANCE_ONLY(distribution[n]++);
            shapes[i] = make_shape(n);
        }
#if defined(XTL_TRACE_PERFORMANCE)
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
        percentages[n] = relative_performance(avgV, avgM);

        for (size_t i = 0; i < N; ++i)
        {
            delete shapes[i];
            shapes[i] = 0;
        }
    }

    if (a1 != a2)
    {
        std::cout << "ERROR: Invariant " << a1 << "==" << a2 << " doesn't hold." << std::endl;
        exit(42);
    }

    std::sort(percentages.begin(), percentages.end());
    return percentages[percentages.size()/2];
}

//------------------------------------------------------------------------------

int main()
{
    int ps = test_sequential();
    int pr = test_randomized();
    std::cout << "OVERALL: Sequential: " 
              << abs(ps) << (ps >= 0 ? "% slower" : "% faster") << "; Random: " 
              << abs(pr) << (pr >= 0 ? "% slower" : "% faster") << std::endl; 
}

//int main()
//{
//    double a = 7.4;
//    int b = 99;
//
//    cout << "a + b = " <<
//        MathFuncs::MyMathFuncs::Add(a, b) << endl;
//    cout << "a - b = " <<
//        MathFuncs::MyMathFuncs::Subtract(a, b) << endl;
//    cout << "a * b = " <<
//        MathFuncs::MyMathFuncs::Multiply(a, b) << endl;
//    cout << "a / b = " <<
//        MathFuncs::MyMathFuncs::Divide(a, b) << endl;
//
//    return 0;
//}
//
