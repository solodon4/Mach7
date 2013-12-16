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

#include <iostream>
#include "type_switchN-patterns.hpp"
#include "patterns/all.hpp"
#include "testutils.hpp"

//------------------------------------------------------------------------------

using namespace mch;

//------------------------------------------------------------------------------

template <typename T>
inline T sqr(const T& x) { return x*x; }

//------------------------------------------------------------------------------

XTL_TIMED_FUNC_BEGIN
int fib1(int n)
{
	if (n == 1 || n == 2) 
        return 1;
	if (n % 2 == 0) 
        return sqr(fib1(n/2+1)) - sqr(fib1(n/2-1));
	if (n % 2 == 1) 
        return sqr(fib1(n/2+1)) + sqr(fib1(n/2));
}
XTL_TIMED_FUNC_END

//------------------------------------------------------------------------------

int fib_opt(int n)
{
	if (n == 1 || n == 2) 
        return 1;

    return 
        n%2 == 0 
            ? sqr(fib_opt(n/2+1)) - sqr(fib_opt(n/2-1))
            : sqr(fib_opt(n/2+1)) + sqr(fib_opt(n/2));
}

//------------------------------------------------------------------------------

XTL_TIMED_FUNC_BEGIN
int fib2(int n)
{
    var<int> m;

    Match(n)
    {
      Case(1)     return 1;
      Case(2)     return 1;
      Case(2*m)   return sqr(fib2(m+1)) - sqr(fib2(m-1));
      Case(2*m+1) return sqr(fib2(m+1)) + sqr(fib2(m));
    }
    EndMatch
}
//int fib2(int n)
//{
//    var<int> m;
//    {
//        struct match_uid_type 
//        {
//        }
//        ;
//        enum 
//        {
//            is_inside_case_clause = 0, number_of_subjects = 1, polymorphic_index00 = -1, __base_counter = 0 
//        }
//        ;
//        auto&& subject_ref0 = n;
//        auto const subject_ptr0 = mch::addr(subject_ref0);
//        typedef  mch::underlying<decltype(*subject_ptr0)>::type source_type0;
//        typedef source_type0 target_type0;
//        ;
//        enum 
//        {
//            is_polymorphic0 = std::is_polymorphic<source_type0>::value, polymorphic_index0 = polymorphic_index00 + is_polymorphic0 
//        }
//        ;
//        auto& match0 = *subject_ptr0;
//        (void)match0;
//        ;
//        register const void* __casted_ptr0 = 0;
//        enum 
//        {
//            number_of_polymorphic_subjects = is_polymorphic0 
//        }
//        ;
//        typedef mch::vtbl_map<number_of_polymorphic_subjects,mch::type_switch_info<number_of_polymorphic_subjects>> vtbl_map_type;
//        vtbl_map_type& __vtbl2case_map = mch::preallocated<vtbl_map_type,match_uid_type>::value;
//        mch::type_switch_info<number_of_polymorphic_subjects>& __switch_info = __vtbl2case_map.get(subject_ptr0);
//        switch (number_of_polymorphic_subjects ? __switch_info.target : 0)
//        {
//        default:
//            {
//                {
//                    {
//                        {
//                        }
//                    }
//                }
//                {
//                    typedef  mch::underlying<decltype(mch::filter(1))>::type type_of_pattern0;
//                    static_assert(mch::is_pattern<type_of_pattern0>::value,"Case-clause expects patterns as its arguments");
//                    typedef  mch::underlying<type_of_pattern0>::type::
//                        accepted_type_for<source_type0>::type target_type0;
//                    if ((__casted_ptr0 = mch::dynamic_cast_when_polymorphic<const target_type0*>(subject_ptr0)) != 0) 
//                    {
//                        static_assert(number_of_subjects == 1, "Number of targets in the case clause must be the same as the number of subjects in the Match statement");
//                        enum 
//                        {
//                            target_label = 1-__base_counter, is_inside_case_clause = 1 
//                        }
//                        ;
//                        if (number_of_polymorphic_subjects && (__switch_info.target == 0)) 
//                        {
//                            __switch_info.target = target_label;
//                            if ((is_polymorphic0)) __switch_info.offset[polymorphic_index0] = intptr_t(__casted_ptr0)-intptr_t(subject_ptr0);
//                        }
//        case target_label:
//                        auto& match0 = *mch::adjust_ptr_if_polymorphic<target_type0>(subject_ptr0,__switch_info.offset[polymorphic_index0]);
//                        if (mch::filter(1)(match0)) 
//                        {
//                            return 1;
//                        }
//                    }
//                }
//                {
//                    typedef  mch::underlying<decltype(mch::filter(2))>::type type_of_pattern0;
//                    static_assert(mch::is_pattern<type_of_pattern0>::value,"Case-clause expects patterns as its arguments");
//                    typedef  mch::underlying<type_of_pattern0>::type::
//                        accepted_type_for<source_type0>::type target_type0;
//                    if ((__casted_ptr0 = mch::dynamic_cast_when_polymorphic<const target_type0*>(subject_ptr0)) != 0) 
//                    {
//                        static_assert(number_of_subjects == 1, "Number of targets in the case clause must be the same as the number of subjects in the Match statement");
//                        enum 
//                        {
//                            target_label = 2-__base_counter, is_inside_case_clause = 1 
//                        }
//                        ;
//                        if (number_of_polymorphic_subjects && (__switch_info.target == 0)) 
//                        {
//                            __switch_info.target = target_label;
//                            if ((is_polymorphic0)) __switch_info.offset[polymorphic_index0] = intptr_t(__casted_ptr0)-intptr_t(subject_ptr0);
//                        }
//        case target_label:
//                        auto& match0 = *mch::adjust_ptr_if_polymorphic<target_type0>(subject_ptr0,__switch_info.offset[polymorphic_index0]);
//                        if (mch::filter(2)(match0)) 
//                        {
//                            return 1;
//                        }
//                    }
//                }
//                {
//                    typedef  mch::underlying<decltype(mch::filter(2*m))>::type type_of_pattern0;
//                    static_assert(mch::is_pattern<type_of_pattern0>::value,"Case-clause expects patterns as its arguments");
//                    typedef  mch::underlying<type_of_pattern0>::type::
//                        accepted_type_for<source_type0>::type target_type0;
//                    if ((__casted_ptr0 = mch::dynamic_cast_when_polymorphic<const target_type0*>(subject_ptr0)) != 0) 
//                    {
//                        static_assert(number_of_subjects == 1, "Number of targets in the case clause must be the same as the number of subjects in the Match statement");
//                        enum 
//                        {
//                            target_label = 3-__base_counter, is_inside_case_clause = 1 
//                        }
//                        ;
//                        if (number_of_polymorphic_subjects && (__switch_info.target == 0)) 
//                        {
//                            __switch_info.target = target_label;
//                            if ((is_polymorphic0)) __switch_info.offset[polymorphic_index0] = intptr_t(__casted_ptr0)-intptr_t(subject_ptr0);
//                        }
//        case target_label:
//                        auto& match0 = *mch::adjust_ptr_if_polymorphic<target_type0>(subject_ptr0,__switch_info.offset[polymorphic_index0]);
//                        if (mch::filter(2*m)(match0)) 
//                        {
//                            return sqr(fib2(m.m_value+1)) - sqr(fib2(m.m_value-1));
//                        }
//                    }
//                }
//                {
//                    typedef  mch::underlying<decltype(mch::filter(2*m+1))>::type type_of_pattern0;
//                    static_assert(mch::is_pattern<type_of_pattern0>::value,"Case-clause expects patterns as its arguments");
//                    typedef  mch::underlying<type_of_pattern0>::type::
//                        accepted_type_for<source_type0>::type target_type0;
//                    if ((__casted_ptr0 = mch::dynamic_cast_when_polymorphic<const target_type0*>(subject_ptr0)) != 0) 
//                    {
//                        static_assert(number_of_subjects == 1, "Number of targets in the case clause must be the same as the number of subjects in the Match statement");
//                        enum 
//                        {
//                            target_label = 4-__base_counter, is_inside_case_clause = 1 
//                        }
//                        ;
//                        if (number_of_polymorphic_subjects && (__switch_info.target == 0)) 
//                        {
//                            __switch_info.target = target_label;
//                            if ((is_polymorphic0)) __switch_info.offset[polymorphic_index0] = intptr_t(__casted_ptr0)-intptr_t(subject_ptr0);
//                        }
//        case target_label:
//                        auto& match0 = *mch::adjust_ptr_if_polymorphic<target_type0>(subject_ptr0,__switch_info.offset[polymorphic_index0]);
//                        if (mch::filter(2*m+1)(match0)) 
//                        {
//                            return sqr(fib2(m+1)) + sqr(fib2(m));
//                        }
//                    }
//                }
//            }
//            if ((number_of_polymorphic_subjects && (__switch_info.target == 0))) 
//            {
//                enum 
//                {
//                    target_label = 5-__base_counter 
//                }
//                ;
//                mch::deferred_constant<mch::vtbl_count_t>::set<match_uid_type,(target_label-1)>::value_ptr;
//                __switch_info.target = target_label;
//        case target_label:
//            ;
//            }
//        }
//    }
//}
XTL_TIMED_FUNC_END

//------------------------------------------------------------------------------


int main()
{
    std::vector<int> arguments(N);

    for (size_t i = 0; i < N; ++i)
        arguments[i] = rand() % 20 + 1;

    verdict v = get_timings1<int,int,fib1,fib2>(arguments);
    std::cout << "Verdict: \t" << v << std::endl;
}

//------------------------------------------------------------------------------
