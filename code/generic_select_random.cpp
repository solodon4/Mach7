#include <algorithm>
#include <cmath>
#include <fstream>
#include <iterator>
#include <iostream>
#include <iomanip>
#include <numeric>
#include <string>
#include <vector>
#include "match_generic.hpp"
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

template <size_t N> struct match_members<shape_kind<N>> { CM(0,shape_kind<N>::m_member0); CM(1,shape_kind<N>::m_member1); };

#if 1
XTL_DO_NOT_INLINE_BEGIN
size_t do_match(const Shape& s)
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
    return -1;
}
XTL_DO_NOT_INLINE_END
#else
XTL_DO_NOT_INLINE_BEGIN
size_t do_match(const Shape& s)
{
    {
        auto const __selector_ptr = addr(s);
        ;
        enum 
        {
            __base_line = 1 
        }
        ;
        typedef generic_switch<decltype(*__selector_ptr)> switch_traits;
        static switch_traits::static_data_type static_data;
        switch_traits::local_data_type local_data;
        switch (switch_traits::choose(__selector_ptr,static_data,local_data)) 
        {
        case switch_traits::CaseLabel<1-__base_line>::entry:
            {
                {
                    {
                    }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<0>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<0>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 2-__base_line);
        case target_specific::CaseLabel<2-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 0;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<1>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<1>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 3-__base_line);
        case target_specific::CaseLabel<3-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 1;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<2>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<2>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 4-__base_line);
        case target_specific::CaseLabel<4-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 2;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<3>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<3>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 5-__base_line);
        case target_specific::CaseLabel<5-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 3;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<4>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<4>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 6-__base_line);
        case target_specific::CaseLabel<6-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 4;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<5>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<5>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 7-__base_line);
        case target_specific::CaseLabel<7-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 5;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<6>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<6>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 8-__base_line);
        case target_specific::CaseLabel<8-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 6;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<7>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<7>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 9-__base_line);
        case target_specific::CaseLabel<9-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 7;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<8>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<8>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 10-__base_line);
        case target_specific::CaseLabel<10-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 8;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<9>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<9>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 11-__base_line);
        case target_specific::CaseLabel<11-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 9;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<10>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<10>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 12-__base_line);
        case target_specific::CaseLabel<12-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 10;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<11>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<11>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 13-__base_line);
        case target_specific::CaseLabel<13-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 11;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<12>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<12>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 14-__base_line);
        case target_specific::CaseLabel<14-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 12;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<13>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<13>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 15-__base_line);
        case target_specific::CaseLabel<15-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 13;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<14>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<14>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 16-__base_line);
        case target_specific::CaseLabel<16-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 14;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<15>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<15>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 17-__base_line);
        case target_specific::CaseLabel<17-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 15;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<16>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<16>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 18-__base_line);
        case target_specific::CaseLabel<18-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 16;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<17>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<17>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 19-__base_line);
        case target_specific::CaseLabel<19-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 17;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<18>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<18>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 20-__base_line);
        case target_specific::CaseLabel<20-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 18;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<19>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<19>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 21-__base_line);
        case target_specific::CaseLabel<21-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 19;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<20>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<20>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 22-__base_line);
        case target_specific::CaseLabel<22-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 20;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<21>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<21>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 23-__base_line);
        case target_specific::CaseLabel<23-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 21;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<22>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<22>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 24-__base_line);
        case target_specific::CaseLabel<24-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 22;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<23>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<23>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 25-__base_line);
        case target_specific::CaseLabel<25-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 23;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<24>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<24>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 26-__base_line);
        case target_specific::CaseLabel<26-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 24;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<25>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<25>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 27-__base_line);
        case target_specific::CaseLabel<27-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 25;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<26>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<26>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 28-__base_line);
        case target_specific::CaseLabel<28-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 26;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<27>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<27>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 29-__base_line);
        case target_specific::CaseLabel<29-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 27;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<28>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<28>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 30-__base_line);
        case target_specific::CaseLabel<30-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 28;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<29>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<29>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 31-__base_line);
        case target_specific::CaseLabel<31-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 29;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<30>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<30>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 32-__base_line);
        case target_specific::CaseLabel<32-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 30;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<31>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<31>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 33-__base_line);
        case target_specific::CaseLabel<33-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 31;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<32>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<32>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 34-__base_line);
        case target_specific::CaseLabel<34-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 32;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<33>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<33>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 35-__base_line);
        case target_specific::CaseLabel<35-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 33;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<34>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<34>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 36-__base_line);
        case target_specific::CaseLabel<36-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 34;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<35>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<35>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 37-__base_line);
        case target_specific::CaseLabel<37-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 35;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<36>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<36>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 38-__base_line);
        case target_specific::CaseLabel<38-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 36;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<37>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<37>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 39-__base_line);
        case target_specific::CaseLabel<39-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 37;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<38>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<38>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 40-__base_line);
        case target_specific::CaseLabel<40-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 38;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<39>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<39>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 41-__base_line);
        case target_specific::CaseLabel<41-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 39;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<40>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<40>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 42-__base_line);
        case target_specific::CaseLabel<42-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 40;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<41>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<41>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 43-__base_line);
        case target_specific::CaseLabel<43-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 41;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<42>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<42>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 44-__base_line);
        case target_specific::CaseLabel<44-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 42;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<43>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<43>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 45-__base_line);
        case target_specific::CaseLabel<45-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 43;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<44>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<44>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 46-__base_line);
        case target_specific::CaseLabel<46-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 44;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<45>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<45>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 47-__base_line);
        case target_specific::CaseLabel<47-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 45;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<46>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<46>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 48-__base_line);
        case target_specific::CaseLabel<48-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 46;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<47>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<47>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 49-__base_line);
        case target_specific::CaseLabel<49-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 47;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<48>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<48>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 50-__base_line);
        case target_specific::CaseLabel<50-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 48;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<49>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<49>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 51-__base_line);
        case target_specific::CaseLabel<51-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 49;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<50>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<50>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 52-__base_line);
        case target_specific::CaseLabel<52-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 50;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<51>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<51>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 53-__base_line);
        case target_specific::CaseLabel<53-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 51;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<52>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<52>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 54-__base_line);
        case target_specific::CaseLabel<54-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 52;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<53>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<53>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 55-__base_line);
        case target_specific::CaseLabel<55-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 53;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<54>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<54>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 56-__base_line);
        case target_specific::CaseLabel<56-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 54;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<55>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<55>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 57-__base_line);
        case target_specific::CaseLabel<57-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 55;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<56>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<56>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 58-__base_line);
        case target_specific::CaseLabel<58-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 56;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<57>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<57>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 59-__base_line);
        case target_specific::CaseLabel<59-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 57;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<58>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<58>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 60-__base_line);
        case target_specific::CaseLabel<60-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 58;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<59>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<59>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 61-__base_line);
        case target_specific::CaseLabel<61-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 59;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<60>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<60>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 62-__base_line);
        case target_specific::CaseLabel<62-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 60;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<61>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<61>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 63-__base_line);
        case target_specific::CaseLabel<63-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 61;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<62>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<62>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 64-__base_line);
        case target_specific::CaseLabel<64-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 62;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<63>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<63>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 65-__base_line);
        case target_specific::CaseLabel<65-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 63;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<64>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<64>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 66-__base_line);
        case target_specific::CaseLabel<66-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 64;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<65>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<65>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 67-__base_line);
        case target_specific::CaseLabel<67-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 65;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<66>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<66>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 68-__base_line);
        case target_specific::CaseLabel<68-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 66;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<67>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<67>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 69-__base_line);
        case target_specific::CaseLabel<69-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 67;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<68>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<68>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 70-__base_line);
        case target_specific::CaseLabel<70-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 68;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<69>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<69>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 71-__base_line);
        case target_specific::CaseLabel<71-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 69;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<70>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<70>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 72-__base_line);
        case target_specific::CaseLabel<72-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 70;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<71>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<71>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 73-__base_line);
        case target_specific::CaseLabel<73-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 71;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<72>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<72>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 74-__base_line);
        case target_specific::CaseLabel<74-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 72;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<73>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<73>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 75-__base_line);
        case target_specific::CaseLabel<75-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 73;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<74>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<74>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 76-__base_line);
        case target_specific::CaseLabel<76-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 74;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<75>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<75>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 77-__base_line);
        case target_specific::CaseLabel<77-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 75;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<76>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<76>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 78-__base_line);
        case target_specific::CaseLabel<78-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 76;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<77>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<77>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 79-__base_line);
        case target_specific::CaseLabel<79-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 77;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<78>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<78>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 80-__base_line);
        case target_specific::CaseLabel<80-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 78;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<79>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<79>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 81-__base_line);
        case target_specific::CaseLabel<81-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 79;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<80>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<80>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 82-__base_line);
        case target_specific::CaseLabel<82-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 80;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<81>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<81>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 83-__base_line);
        case target_specific::CaseLabel<83-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 81;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<82>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<82>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 84-__base_line);
        case target_specific::CaseLabel<84-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 82;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<83>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<83>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 85-__base_line);
        case target_specific::CaseLabel<85-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 83;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<84>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<84>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 86-__base_line);
        case target_specific::CaseLabel<86-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 84;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<85>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<85>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 87-__base_line);
        case target_specific::CaseLabel<87-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 85;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<86>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<86>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 88-__base_line);
        case target_specific::CaseLabel<88-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 86;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<87>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<87>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 89-__base_line);
        case target_specific::CaseLabel<89-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 87;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<88>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<88>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 90-__base_line);
        case target_specific::CaseLabel<90-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 88;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<89>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<89>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 91-__base_line);
        case target_specific::CaseLabel<91-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 89;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<90>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<90>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 92-__base_line);
        case target_specific::CaseLabel<92-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 90;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<91>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<91>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 93-__base_line);
        case target_specific::CaseLabel<93-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 91;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<92>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<92>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 94-__base_line);
        case target_specific::CaseLabel<94-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 92;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<93>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<93>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 95-__base_line);
        case target_specific::CaseLabel<95-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 93;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<94>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<94>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 96-__base_line);
        case target_specific::CaseLabel<96-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 94;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<95>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<95>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 97-__base_line);
        case target_specific::CaseLabel<97-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 95;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<96>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<96>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 98-__base_line);
        case target_specific::CaseLabel<98-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 96;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<97>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<97>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 99-__base_line);
        case target_specific::CaseLabel<99-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 97;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<98>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<98>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 100-__base_line);
        case target_specific::CaseLabel<100-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 98;
            }
                }
            }
            {
                typedef switch_traits::disambiguate<sizeof(shape_kind<99>)<sizeof(switch_traits::selector_type)>::parameter<shape_kind<99>> target_specific;
                if (target_specific::main_condition(__selector_ptr, local_data)) 
                {
                    switch_traits::on_first_pass(__selector_ptr, local_data, 101-__base_line);
        case target_specific::CaseLabel<101-__base_line>::value:
            auto matched = target_specific::get_matched(__selector_ptr,local_data);
            ;
            {
                return 99;
            }
                }
            }
            switch_traits::on_end(__selector_ptr, local_data, 102-__base_line);
        case switch_traits::CaseLabel<102-__base_line>::exit:
            ;
        }
    }
    return -1;
}
XTL_DO_NOT_INLINE_END
#endif

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
            shapes[i] = make_shape(n);
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

int main()
{
    int ps = test_sequential();
    int pr = test_randomized();
    std::cout << "OVERALL: Sequential: " 
              << abs(ps) << (ps >= 0 ? "% slower" : "% faster") << "; Random: " 
              << abs(pr) << (pr >= 0 ? "% slower" : "% faster") << std::endl; 
}
