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

#if 0
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
#else
//#error No preprocessed code yet
size_t do_match(const Shape& s, size_t)
{
    {
        struct match_uid_type 
        {
        }
        ;
        auto&& subject_ref = s;
        auto const subject_ptr = addr(subject_ref);
        typedef  underlying<decltype(*subject_ptr)>::type source_type;
        typedef source_type target_type;
        enum 
        {
            is_inside_case_clause = 0 
        }
        ;
        ;
        auto& match1 = *subject_ptr;
        (void)match1;
        ;
        enum 
        {
            __base_counter = 0 
        }
        ;
        static_assert(std::is_polymorphic<source_type>::value, "Type of subject should be polymorphic when you use MatchP");
        vtblmap<type_switch_info>& __vtbl2lines_map = preallocated<vtblmap<type_switch_info>,match_uid_type>::value;
        register const void* __casted_ptr = 0;
        type_switch_info& __switch_info = __vtbl2lines_map.get(subject_ptr);
        switch (__switch_info.target) 
        {
        default:
            {
                {

                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<0>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 1-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<0>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 0;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<1>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 2-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<1>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 1;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<2>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 3-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<2>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 2;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<3>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 4-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<3>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 3;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<4>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 5-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<4>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 4;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<5>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 6-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<5>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 5;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<6>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 7-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<6>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 6;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<7>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 8-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<7>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 7;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<8>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 9-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<8>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 8;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<9>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 10-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<9>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 9;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<10>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 11-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<10>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 10;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<11>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 12-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<11>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 11;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<12>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 13-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<12>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 12;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<13>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 14-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<13>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 13;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<14>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 15-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<14>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 14;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<15>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 16-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<15>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 15;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<16>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 17-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<16>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 16;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<17>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 18-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<17>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 17;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<18>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 19-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<18>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 18;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<19>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 20-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<19>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 19;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<20>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 21-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<20>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 20;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<21>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 22-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<21>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 21;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<22>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 23-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<22>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 22;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<23>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 24-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<23>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 23;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<24>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 25-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<24>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 24;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<25>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 26-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<25>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 25;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<26>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 27-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<26>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 26;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<27>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 28-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<27>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 27;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<28>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 29-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<28>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 28;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<29>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 30-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<29>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 29;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<30>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 31-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<30>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 30;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<31>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 32-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<31>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 31;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<32>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 33-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<32>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 32;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<33>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 34-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<33>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 33;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<34>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 35-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<34>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 34;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<35>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 36-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<35>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 35;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<36>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 37-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<36>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 36;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<37>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 38-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<37>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 37;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<38>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 39-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<38>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 38;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<39>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 40-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<39>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 39;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<40>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 41-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<40>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 40;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<41>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 42-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<41>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 41;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<42>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 43-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<42>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 42;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<43>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 44-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<43>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 43;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<44>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 45-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<44>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 44;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<45>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 46-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<45>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 45;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<46>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 47-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<46>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 46;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<47>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 48-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<47>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 47;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<48>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 49-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<48>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 48;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<49>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 50-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<49>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 49;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<50>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 51-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<50>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 50;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<51>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 52-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<51>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 51;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<52>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 53-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<52>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 52;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<53>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 54-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<53>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 53;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<54>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 55-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<54>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 54;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<55>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 56-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<55>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 55;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<56>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 57-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<56>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 56;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<57>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 58-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<57>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 57;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<58>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 59-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<58>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 58;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<59>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 60-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<59>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 59;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<60>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 61-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<60>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 60;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<61>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 62-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<61>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 61;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<62>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 63-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<62>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 62;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<63>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 64-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<63>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 63;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<64>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 65-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<64>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 64;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<65>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 66-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<65>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 65;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<66>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 67-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<66>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 66;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<67>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 68-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<67>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 67;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<68>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 69-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<68>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 68;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<69>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 70-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<69>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 69;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<70>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 71-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<70>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 70;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<71>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 72-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<71>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 71;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<72>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 73-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<72>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 72;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<73>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 74-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<73>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 73;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<74>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 75-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<74>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 74;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<75>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 76-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<75>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 75;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<76>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 77-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<76>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 76;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<77>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 78-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<77>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 77;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<78>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 79-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<78>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 78;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<79>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 80-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<79>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 79;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<80>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 81-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<80>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 80;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<81>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 82-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<81>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 81;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<82>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 83-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<82>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 82;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<83>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 84-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<83>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 83;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<84>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 85-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<84>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 84;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<85>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 86-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<85>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 85;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<86>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 87-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<86>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 86;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<87>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 88-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<87>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 87;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<88>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 89-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<88>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 88;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<89>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 90-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<89>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 89;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<90>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 91-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<90>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 90;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<91>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 92-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<91>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 91;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<92>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 93-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<92>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 92;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<93>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 94-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<93>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 93;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<94>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 95-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<94>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 94;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<95>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 96-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<95>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 95;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<96>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 97-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<96>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 96;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<97>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 98-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<97>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 97;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<98>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 99-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<98>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 98;
                }
                if ((__casted_ptr = dynamic_cast<const shape_kind<99>*>(subject_ptr))) 
                {
                    enum 
                    {
                        target_label = 100-__base_counter 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr);
                    }
        case target_label:
            auto& match1 = *adjust_ptr<shape_kind<99>>(subject_ptr,__switch_info.offset);
            (void)match1;
            ;
            return 99;
                }
            }
            if (((__casted_ptr == 0 && __switch_info.target == 0))) 
            {
                enum 
                {
                    target_label = 101-__base_counter 
                }
                ;
                __switch_info.target = target_label;
        case target_label:
            ;
            }
        }
    }
    return invalid;
}
#endif

//------------------------------------------------------------------------------
#if 0
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
