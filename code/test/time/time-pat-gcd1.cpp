//
//  Mach7: Pattern Matching Library for C++
//
//  Copyright 2011-2013, Texas A&M University.
//  Copyright 2014 Yuriy Solodkyy.
//  All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//
//      * Redistributions of source code must retain the above copyright
//        notice, this list of conditions and the following disclaimer.
//
//      * Redistributions in binary form must reproduce the above copyright
//        notice, this list of conditions and the following disclaimer in the
//        documentation and/or other materials provided with the distribution.
//
//      * Neither the names of Mach7 project nor the names of its contributors
//        may be used to endorse or promote products derived from this software
//        without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
//  IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY
//  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
//  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

///
/// \file
///
/// This file is a part of Mach7 library test suite.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// \see https://parasol.tamu.edu/mach7/
/// \see https://github.com/solodon4/Mach7
/// \see https://github.com/solodon4/SELL
///

#include <iostream>
#include <mach7/type_switchN-patterns.hpp> // Support for N-ary Match statement on patterns
#include <mach7/patterns/equivalence.hpp>  // Equivalence combinator +
#include <mach7/patterns/n+k.hpp>          // Generalized n+k patterns
#include <mach7/patterns/primitive.hpp>    // Wildcard, variable and value patterns
#include "testutils.hpp"

//------------------------------------------------------------------------------

using namespace mch;

//------------------------------------------------------------------------------

typedef std::pair<unsigned int,unsigned int> arg_type;

//------------------------------------------------------------------------------

extern unsigned int gcd1(const unsigned int, const unsigned int);

inline unsigned int gcd1(const arg_type a) { return gcd1(a.first,a.second); }

//------------------------------------------------------------------------------

XTL_TIMED_FUNC_BEGIN
unsigned int gcd1(const unsigned int a, const unsigned int b)
{
    if (a > b)
        return gcd1(a-b,b);
    else
    if (a < b)
        return gcd1(a,b-a);
    else
        return a;
}
XTL_TIMED_FUNC_END

//------------------------------------------------------------------------------

extern unsigned int gcd2(const unsigned int, const unsigned int);

inline unsigned int gcd2(const arg_type a) { return gcd2(a.first,a.second); }

//------------------------------------------------------------------------------

XTL_TIMED_FUNC_BEGIN
unsigned int gcd2(const unsigned int a, const unsigned int b)
{
    var<unsigned int> x, y;

    Match(a,b)
    {
    Case(x,+x)   return x;
    Case(x,+x+y) return gcd2(x,y);
    Case(x,+x-y) return gcd2(x-y,y);
    }
    EndMatch

    XTL_UNREACHABLE; // To avoid warning that control may reach end of a non-void function
}
//unsigned int gcd2(const unsigned int a, const unsigned int b)
//{
//    var<unsigned int> x, y;
//    {
//        struct match_uid_type 
//        {
//        }
//        ;
//        enum 
//        {
//            is_inside_case_clause = 0, number_of_subjects = 2, polymorphic_index00 = -1, __base_counter = 0 
//        }
//        ;
//        auto&& subject_ref0 = a;
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
//        auto&& subject_ref1 = b;
//        auto const subject_ptr1 = mch::addr(subject_ref1);
//        typedef  mch::underlying<decltype(*subject_ptr1)>::type source_type1;
//        typedef source_type1 target_type1;
//        ;
//        enum 
//        {
//            is_polymorphic1 = std::is_polymorphic<source_type1>::value, polymorphic_index1 = polymorphic_index0 + is_polymorphic1 
//        }
//        ;
//        auto& match1 = *subject_ptr1;
//        (void)match1;
//        ;
//        register const void* __casted_ptr1 = 0;
//        enum 
//        {
//            number_of_polymorphic_subjects = is_polymorphic0 + is_polymorphic1 
//        }
//        ;
//        typedef mch::vtbl_map<number_of_polymorphic_subjects,mch::type_switch_info<number_of_polymorphic_subjects>> vtbl_map_type;
//        static vtbl_map_type __vtbl2case_map ((mch::deferred_constant<mch::vtbl_count_t>::get<match_uid_type>::value));
//        mch::type_switch_info<number_of_polymorphic_subjects>& __switch_info = __vtbl2case_map.get(subject_ptr0 , subject_ptr1);
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
//                    typedef  mch::underlying<decltype(mch::filter(x))>::type type_of_pattern0;
//                    static_assert(mch::is_pattern<type_of_pattern0>::value,"Case-clause expects patterns as its arguments");
//                    typedef  mch::underlying<type_of_pattern0>::type::accepted_type_for<source_type0>::type target_type0;
//                    typedef  mch::underlying<decltype(mch::filter(+x))>::type type_of_pattern1;
//                    static_assert(mch::is_pattern<type_of_pattern1>::value,"Case-clause expects patterns as its arguments");
//                    typedef  mch::underlying<type_of_pattern1>::type::accepted_type_for<source_type1>::type target_type1;
//                    if ((__casted_ptr0 = mch::dynamic_cast_when_polymorphic<const target_type0*>(subject_ptr0)) != 0 && (__casted_ptr1 = mch::dynamic_cast_when_polymorphic<const target_type1*>(subject_ptr1)) != 0) 
//                    {
//                        static_assert(number_of_subjects == 2, "Number of targets in the case clause must be the same as the number of subjects in the Match statement");
//                        enum 
//                        {
//                            target_label = 1-__base_counter, is_inside_case_clause = 1 
//                        }
//                        ;
//                        __pragma(warning(push)) __pragma(warning( disable : 4127 ))
//                            if ((number_of_polymorphic_subjects)) __pragma(warning(pop))
//                                if ((__switch_info.target == 0)) 
//                                {
//                                    __switch_info.target = target_label;
//                                    __pragma(warning(push)) __pragma(warning( disable : 4127 ))
//                                        if ((is_polymorphic0)) __pragma(warning(pop)) __switch_info.offset[polymorphic_index0] = intptr_t(__casted_ptr0)-intptr_t(subject_ptr0);
//                                    __pragma(warning(push)) __pragma(warning( disable : 4127 ))
//                                        if ((is_polymorphic1)) __pragma(warning(pop)) __switch_info.offset[polymorphic_index1] = intptr_t(__casted_ptr1)-intptr_t(subject_ptr1);
//                                }
//        case target_label:
//                        auto& match0 = *mch::adjust_ptr_if_polymorphic<target_type0>(subject_ptr0,__switch_info.offset[polymorphic_index0]);
//                        auto& match1 = *mch::adjust_ptr_if_polymorphic<target_type1>(subject_ptr1,__switch_info.offset[polymorphic_index1]);
//                        if (mch::filter(x)(match0) && mch::filter(+x)(match1)) 
//                        {
//                            return x;
//                        }
//                    }
//                }
//                {
//                    typedef  mch::underlying<decltype(mch::filter(x))>::type type_of_pattern0;
//                    static_assert(mch::is_pattern<type_of_pattern0>::value,"Case-clause expects patterns as its arguments");
//                    typedef  mch::underlying<type_of_pattern0>::type::accepted_type_for<source_type0>::type target_type0;
//                    typedef  mch::underlying<decltype(mch::filter(+x+y))>::type type_of_pattern1;
//                    static_assert(mch::is_pattern<type_of_pattern1>::value,"Case-clause expects patterns as its arguments");
//                    typedef  mch::underlying<type_of_pattern1>::type::accepted_type_for<source_type1>::type target_type1;
//                    if ((__casted_ptr0 = mch::dynamic_cast_when_polymorphic<const target_type0*>(subject_ptr0)) != 0 && (__casted_ptr1 = mch::dynamic_cast_when_polymorphic<const target_type1*>(subject_ptr1)) != 0) 
//                    {
//                        static_assert(number_of_subjects == 2, "Number of targets in the case clause must be the same as the number of subjects in the Match statement");
//                        enum 
//                        {
//                            target_label = 2-__base_counter, is_inside_case_clause = 1 
//                        }
//                        ;
//                        __pragma(warning(push)) __pragma(warning( disable : 4127 ))
//                            if ((number_of_polymorphic_subjects)) __pragma(warning(pop))
//                                if ((__switch_info.target == 0)) 
//                                {
//                                    __switch_info.target = target_label;
//                                    __pragma(warning(push)) __pragma(warning( disable : 4127 ))
//                                        if ((is_polymorphic0)) __pragma(warning(pop)) __switch_info.offset[polymorphic_index0] = intptr_t(__casted_ptr0)-intptr_t(subject_ptr0);
//                                    __pragma(warning(push)) __pragma(warning( disable : 4127 ))
//                                        if ((is_polymorphic1)) __pragma(warning(pop)) __switch_info.offset[polymorphic_index1] = intptr_t(__casted_ptr1)-intptr_t(subject_ptr1);
//                                }
//        case target_label:
//                        auto& match0 = *mch::adjust_ptr_if_polymorphic<target_type0>(subject_ptr0,__switch_info.offset[polymorphic_index0]);
//                        auto& match1 = *mch::adjust_ptr_if_polymorphic<target_type1>(subject_ptr1,__switch_info.offset[polymorphic_index1]);
//                        if (mch::filter(x)(match0) && mch::filter(+x+y)(match1)) 
//                        {
//                            return gcd2(x,y);
//                        }
//                    }
//                }
//                {
//                    typedef  mch::underlying<decltype(mch::filter(x))>::type type_of_pattern0;
//                    static_assert(mch::is_pattern<type_of_pattern0>::value,"Case-clause expects patterns as its arguments");
//                    typedef  mch::underlying<type_of_pattern0>::type::accepted_type_for<source_type0>::type target_type0;
//                    typedef  mch::underlying<decltype(mch::filter(+x-y))>::type type_of_pattern1;
//                    static_assert(mch::is_pattern<type_of_pattern1>::value,"Case-clause expects patterns as its arguments");
//                    typedef  mch::underlying<type_of_pattern1>::type::accepted_type_for<source_type1>::type target_type1;
//                    if ((__casted_ptr0 = mch::dynamic_cast_when_polymorphic<const target_type0*>(subject_ptr0)) != 0 && (__casted_ptr1 = mch::dynamic_cast_when_polymorphic<const target_type1*>(subject_ptr1)) != 0) 
//                    {
//                        static_assert(number_of_subjects == 2, "Number of targets in the case clause must be the same as the number of subjects in the Match statement");
//                        enum 
//                        {
//                            target_label = 3-__base_counter, is_inside_case_clause = 1 
//                        }
//                        ;
//                        __pragma(warning(push)) __pragma(warning( disable : 4127 ))
//                            if ((number_of_polymorphic_subjects)) __pragma(warning(pop))
//                                if ((__switch_info.target == 0)) 
//                                {
//                                    __switch_info.target = target_label;
//                                    __pragma(warning(push)) __pragma(warning( disable : 4127 ))
//                                        if ((is_polymorphic0)) __pragma(warning(pop)) __switch_info.offset[polymorphic_index0] = intptr_t(__casted_ptr0)-intptr_t(subject_ptr0);
//                                    __pragma(warning(push)) __pragma(warning( disable : 4127 ))
//                                        if ((is_polymorphic1)) __pragma(warning(pop)) __switch_info.offset[polymorphic_index1] = intptr_t(__casted_ptr1)-intptr_t(subject_ptr1);
//                                }
//        case target_label:
//                        auto& match0 = *mch::adjust_ptr_if_polymorphic<target_type0>(subject_ptr0,__switch_info.offset[polymorphic_index0]);
//                        auto& match1 = *mch::adjust_ptr_if_polymorphic<target_type1>(subject_ptr1,__switch_info.offset[polymorphic_index1]);
//                        if (mch::filter(x)(match0) && mch::filter(+x-y)(match1)) 
//                        {
//                            return gcd2(x-y,y);
//                        }
//                    }
//                }
//            }
//            __pragma(warning(push)) __pragma(warning( disable : 4127 ))
//                if ((number_of_polymorphic_subjects)) __pragma(warning(pop))
//                    if (((__switch_info.target == 0))) 
//                    {
//                        enum 
//                        {
//                            target_label = 4-__base_counter 
//                        }
//                        ;
//                        mch::deferred_constant<mch::vtbl_count_t>::set<match_uid_type,(target_label-1)>::value_ptr;
//                        __switch_info.target = target_label;
//        case target_label:
//            ;
//                    }
//        }
//    }
//}
XTL_TIMED_FUNC_END

//------------------------------------------------------------------------------

int main()
{
    const size_t MM = 10000; // Largest argument to GCD
    std::vector<arg_type> arguments(N);

    for (size_t i = 0; i < N; ++i)
        arguments[i] = arg_type(rand() % MM + 1, rand() % MM + 1);

    verdict v = get_timings1<unsigned int,arg_type,gcd1,gcd2>(arguments);
    std::cout << "Verdict: \t" << v << std::endl;
}

//------------------------------------------------------------------------------
