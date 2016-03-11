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
/// A stripped down version of match.hpp that can be used for type switching,
/// without support for binding or other pattern matching.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// \see https://parasol.tamu.edu/mach7/
/// \see https://github.com/solodon4/Mach7
/// \see https://github.com/solodon4/SELL
///
/// \note Unlike match.hpp, use of this file does not require member matching
///       definitions or any other definitions.
///
///

#pragma once

#include "vtblmap3st-3.hpp"
#include <mach7/metatools.hpp>             // Support for various meta-programming tools used by Mach7

//------------------------------------------------------------------------------

/// Predefined value representing a layout used by default when none is specified.
enum { default_layout = size_t(~0) };

//------------------------------------------------------------------------------

/// Common definitions generated at the beginning of #Match statement for a 
/// subject s in position N
#define XTL_MATCH_SUBJECT(N,s)                                                 \
        auto&&     subject_ref##N = s;                                         \
        auto const subject_ptr##N = mch::addr(subject_ref##N);                 \
        typedef XTL_CPP0X_TYPENAME mch::underlying<decltype(*subject_ptr##N)>::type source_type##N; \
        typedef source_type##N target_type##N XTL_UNUSED_TYPEDEF;              \
        enum { target_layout##N = default_layout };                            \
        XTL_ASSERT(xtl_failure("Trying to match against a nullptr",subject_ptr##N));\
        auto const match##N = subject_ptr##N;                                  \
        XTL_UNUSED(match##N);

//------------------------------------------------------------------------------

/// Common definitions generated at the beginning of #Match statement for a 
/// polymorphic subject s in position N
#define XTL_MATCH_SUBJECT_POLYMORPHIC(N,s)                                     \
        XTL_MATCH_SUBJECT(N,s)                                                 \
        static_assert(std::is_polymorphic<source_type##N>::value, "Type of subject " #N " should be polymorphic when you use Match2");\
        register const void* __casted_ptr##N = 0;

//------------------------------------------------------------------------------

/// Macro that starts the switch on dynamic type of a variable s that can be 
/// either pointer or reference to a polymorphic type.
/// \note case 0: instead of default: will work in the same way because we 
///       initialize cache with 0, however through experiments we can see
///       that having default here is quite a bit faster than having case 0
///       because one less branch should be generated
#define Match3(s0,s1,s2) {                                                     \
        XTL_WARNING_PUSH                                                       \
        XTL_WARNING_IGNORE_NAME_HIDING                                         \
        struct match_uid_type {};                                              \
        enum { is_inside_case_clause = 0 };                                    \
        enum { __base_counter = XTL_COUNTER };                                 \
        XTL_MATCH_SUBJECT_POLYMORPHIC(0,s0)                                    \
        XTL_MATCH_SUBJECT_POLYMORPHIC(1,s1)                                    \
        XTL_MATCH_SUBJECT_POLYMORPHIC(2,s2)                                    \
        XTL_PRELOADABLE_LOCAL_STATIC(mch::vtblmap<mch::type_switch_info>,__vtbl2lines_map,match_uid_type,XTL_DUMP_PERFORMANCE_ONLY(__FILE__,__LINE__,XTL_FUNCTION));\
        mch::type_switch_info& __switch_info = __vtbl2lines_map.get(subject_ptr0,subject_ptr1,subject_ptr2); \
        switch (__switch_info.target) {                                        \
        default: {

/// Macro that defines the case statement for the above switch
/// NOTE: If Visual C++ gives you error C2051: case expression not constant
///       on this TypeCase label, just change the Debug Format in project setting 
///       Project -> Properties -> C/C++ -> General -> Debug Information Format 
///       from "Program Database for Edit And Continue (/ZI)" 
///       to   "Program Database (/Zi)", which is the default in Release builds,
///       but not in Debug. This is a known bug of Visual C++ described here:
///       http://connect.microsoft.com/VisualStudio/feedback/details/375836/-line-not-seen-as-compile-time-constant
#define Case3(C0,C1,C2)                                                        \
        }                                                                      \
        if (XTL_UNLIKELY((__casted_ptr0 = dynamic_cast<const C0*>(subject_ptr0)) != 0) && \
            XTL_UNLIKELY((__casted_ptr1 = dynamic_cast<const C1*>(subject_ptr1)) != 0) && \
            XTL_UNLIKELY((__casted_ptr2 = dynamic_cast<const C2*>(subject_ptr2)) != 0))   \
        {                                                                      \
            enum { target_label = XTL_COUNTER-__base_counter, is_inside_case_clause = 1 }; \
            if (XTL_LIKELY(__switch_info.target == 0))                         \
            {                                                                  \
                __switch_info.target = target_label;                           \
                __switch_info.offset[0] = intptr_t(__casted_ptr0)-intptr_t(subject_ptr0); \
                __switch_info.offset[1] = intptr_t(__casted_ptr1)-intptr_t(subject_ptr1); \
                __switch_info.offset[2] = intptr_t(__casted_ptr2)-intptr_t(subject_ptr2); \
            }                                                                  \
        case target_label:                                                     \
            auto& match0 = *mch::adjust_ptr<C0>(subject_ptr0,__switch_info.offset[0]); \
            auto& match1 = *mch::adjust_ptr<C1>(subject_ptr1,__switch_info.offset[1]); \
            auto& match2 = *mch::adjust_ptr<C2>(subject_ptr2,__switch_info.offset[2]); \
            XTL_UNUSED(match0);                                                \
            XTL_UNUSED(match1);                                                \
            XTL_UNUSED(match2);

#define Otherwise3()                                                           \
        static_assert(is_inside_case_clause, "Otherwise() must follow actual clauses! If you are trying to use it as a default sub-clause, use When() instead"); \
        Case3(source_type0,source_type1,source_type2)

#define EndMatch3                                                              \
        }                                                                      \
        if (XTL_UNLIKELY((__casted_ptr0 == 0 && __casted_ptr1 == 0 && __casted_ptr2 == 0 && __switch_info.target == 0))) \
        {                                                                      \
            enum { target_label = XTL_COUNTER-__base_counter };                \
            __switch_info.target = target_label;                               \
            case target_label: ;                                               \
        }                                                                      \
        }                                                                      \
        XTL_WARNING_POP                                                        \
        }

//------------------------------------------------------------------------------

