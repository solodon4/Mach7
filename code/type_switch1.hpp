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

#include "vtblmap.hpp"
#include "metatools.hpp"

//------------------------------------------------------------------------------

/// This is a common prefix of every Match statement that ensures the following:
/// - a local type match_uid_type that uniquely identifies this match statement
///   is introduced. We use it to associate global resources with this statement.
/// - a (const) reference value subject_ref referencing the subject is 
///   introduced, in order to prolong the lifetime of a temporary, if such was
///   used as a subject, till the end of the Match statement.
/// - a pointer value subject_ptr referencing the subject is introduced, 
///   regardless of whether the actual subject was passed by pointer, reference
///   or value.
/// - the actual type of the subject without qualifiers etc. is source_type
/// - the default target_type is the source_type (used for When sub-clauses)
/// - the default target layout is the default layout (used for When sub-clauses)
/// - match1 refers the subject by default (used for When sub-clauses)
/// - the subject cannot be a nullptr - we assert at run-time (debug) if it is
#define XTL_MATCH_PREAMBULA(s)                                                 \
        struct match_uid_type {};                                              \
        auto&&     subject_ref = s;                                            \
        auto const subject_ptr = mch::addr(subject_ref);                       \
        typedef XTL_CPP0X_TYPENAME mch::underlying<decltype(*subject_ptr)>::type source_type; \
        typedef source_type target_type XTL_UNUSED_TYPEDEF;                    \
        enum { is_inside_case_clause = 0 };                                    \
        XTL_ASSERT(xtl_failure("Trying to match against a nullptr",subject_ptr)); \
        auto& match1 = *subject_ptr;                                           \
        XTL_UNUSED(match1);

//------------------------------------------------------------------------------

#if XTL_CLAUSES_NUM_ESTIMATES_TYPES_NUM
    #define XTL_GET_TYPES_NUM_ESTIMATE   (mch::deferred_constant<mch::vtbl_count_t>::get<match_uid_type>::value)
    #define XTL_SET_TYPES_NUM_ESTIMATE(N) mch::ignore_unused_warning(mch::deferred_constant<mch::vtbl_count_t>::set<match_uid_type,(N)>::value_ptr)
#else
    #define XTL_GET_TYPES_NUM_ESTIMATE   (min_expected_size)
    #define XTL_SET_TYPES_NUM_ESTIMATE(N)
#endif

//------------------------------------------------------------------------------

/// Macro that starts the switch on dynamic type of a variable s that can be 
/// either pointer or reference to a polymorphic type.
/// \note case 0: instead of default: will work in the same way because we 
///       initialize cache with 0, however through experiments we can see
///       that having default here is quite a bit faster than having case 0
///       because one less branch should be generated
#define Match(s) {                                                             \
        XTL_MATCH_PREAMBULA(s)                                                 \
        enum { __base_counter = XTL_COUNTER };                                 \
        static_assert(std::is_polymorphic<source_type>::value, "Type of subject should be polymorphic when you use MatchP");\
        XTL_PRELOADABLE_LOCAL_STATIC(mch::vtblmap<mch::type_switch_info>,__vtbl2lines_map,match_uid_type,XTL_DUMP_PERFORMANCE_ONLY(__FILE__,__LINE__,XTL_FUNCTION,)XTL_GET_TYPES_NUM_ESTIMATE);\
        register const void* __casted_ptr = 0;                                 \
        mch::type_switch_info& __switch_info = __vtbl2lines_map.get(subject_ptr); \
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
#define Case(C)                                                                \
        }                                                                      \
        if (XTL_UNLIKELY(__casted_ptr = dynamic_cast<const C*>(subject_ptr)))  \
        {                                                                      \
            enum { target_label = XTL_COUNTER-__base_counter };                \
            if (XTL_LIKELY(__switch_info.target == 0))                         \
            {                                                                  \
                __switch_info.target = target_label;                           \
                __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr); \
            }                                                                  \
        case target_label:                                                     \
            auto& match1 = *adjust_ptr<C>(subject_ptr,__switch_info.offset);   \
            XTL_UNUSED(match1);

#define EndMatch                                                               \
        }                                                                      \
        if (XTL_UNLIKELY((__casted_ptr == 0 && __switch_info.target == 0)))    \
        {                                                                      \
            enum { target_label = XTL_COUNTER-__base_counter };                \
            __switch_info.target = target_label;                               \
            case target_label: ;                                               \
        }                                                                      \
        }}

//------------------------------------------------------------------------------

