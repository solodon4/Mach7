///
/// \file type_switch.hpp
///
/// A stripped down version of match.hpp that can be used for type switching,
/// without support for binding or other pattern matching. 
///
/// \note Unlike match.hpp, use of this file does not require member matching
/// definitions or any other definitions.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of Mach7 library (http://parasol.tamu.edu/mach7/).
/// Copyright (C) 2011-2012 Texas A&M University.
/// All rights reserved.
///

#pragma once

#include "vtblmap4.hpp"
#include "metatools.hpp"

namespace mch ///< Mach7 library namespace
{

//------------------------------------------------------------------------------

/// FIX: This specialization doesn't actually help/work as expected as get<UID>
///      at the time of this instantiation is not yet set<UID>.
template <size_t N, typename T, typename UID>
struct preallocated<vtbl_map<N,T>,UID>
{
    static vtbl_map<N,T> value;
};

template <size_t N, typename T, typename UID>
vtbl_map<N,T> preallocated<vtbl_map<N,T>,UID>::value(deferred_constant<vtbl_count_t>::get<UID>::value);

} // of namespace mch

#define XTL_GET_TYPES_NUM_ESTIMATE   (mch::deferred_constant<mch::vtbl_count_t>::get<match_uid_type>::value)
#define XTL_SET_TYPES_NUM_ESTIMATE(N) mch::deferred_constant<mch::vtbl_count_t>::set<match_uid_type,(N)>::value_ptr

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
        typedef source_type##N target_type##N;                                 \
        enum { target_layout##N = default_layout };                            \
        XTL_ASSERT(("Trying to match against a nullptr",subject_ptr##N));      \
        auto& match##N = *subject_ptr##N;                                      \
        XTL_UNUSED(match##N);

//------------------------------------------------------------------------------

/// Common definitions generated at the beginning of #Match statement for a 
/// polymorphic subject s in position N
#define XTL_MATCH_SUBJECT_POLYMORPHIC(N,s)                                     \
        XTL_MATCH_SUBJECT(N,s)                                                 \
        static_assert(std::is_polymorphic<source_type##N>::value, "Type of subject " #N " should be polymorphic when you use Match");\
        register const void* __casted_ptr##N = 0;

/// Extension of #XTL_MATCH_SUBJECT_POLYMORPHIC where a list of subjects is 
/// passed and we have to pick up i-th subject. Used in repetitions.
#define XTL_MATCH_SUBJECT_POLYMORPHIC_FROM(i,...) XTL_MATCH_SUBJECT_POLYMORPHIC(i,XTL_SELECT_ARG(i,__VA_ARGS__))

//------------------------------------------------------------------------------

/// A common macro used in repetitions to prefix the iterator with given token
#define XTL_PREFIX(n,...) __VA_ARGS__##n

#define XTL_GET_VTLB_OF_SUBJECT(N,...) mch::vtbl_of(subject_ptr##N)

//------------------------------------------------------------------------------

/// Helper macro for #Match
#define MatchN(N, ...) {                                                       \
        struct match_uid_type {};                                              \
        enum { is_inside_case_clause = 0, number_of_subjects = N };            \
        enum { __base_counter = XTL_COUNTER };                                 \
        XTL_REPEAT(N,XTL_MATCH_SUBJECT_POLYMORPHIC_FROM,__VA_ARGS__)           \
        const intptr_t __vtbl[N] = {XTL_ENUM(N,XTL_GET_VTLB_OF_SUBJECT)};      \
        typedef mch::vtbl_map<N,mch::type_switch_info<N>> vtbl_map_type;       \
        XTL_PRELOADABLE_LOCAL_STATIC(vtbl_map_type,__vtbl2case_map,match_uid_type,XTL_DUMP_PERFORMANCE_ONLY(__FILE__,__LINE__,XTL_FUNCTION,)XTL_GET_TYPES_NUM_ESTIMATE);\
        mch::type_switch_info<N>& __switch_info = __vtbl2case_map.get(__vtbl); \
        switch (__switch_info.target) {                                        \
        default: {

#if defined(_MSC_VER)
    /// FIX: For some reason we need to make this extra hoop to make MSVC preprocessor do what we want
    #define Match_(N,...) XTL_APPLY_VARIADIC_MACRO(MatchN,(N,__VA_ARGS__))
    /// General Match statement supporting multiple scrutiny (subjects)
    #define Match(...) Match_(XTL_NARG(__VA_ARGS__),__VA_ARGS__)
#else
    /// General Match statement supporting multiple scrutiny (subjects)
    #define Match(...) MatchN(XTL_NARG(__VA_ARGS__),__VA_ARGS__)
#endif

/// A set of macros handling specific number of subjects passed to #Match(...).
//#define Match0()                        static_assert(false,"Match statement has to have at least 1 scrutiny");
//#define Match1(x0)                      MatchN(1,x0)
//#define Match2(x0,x1)                   MatchN(2,x0,x1)                  
//#define Match3(x0,x1,x2)                MatchN(3,x0,x1,x2)               
//#define Match4(x0,x1,x2,x3)             MatchN(4,x0,x1,x2,x3)            
//#define Match5(x0,x1,x2,x3,x4)          MatchN(5,x0,x1,x2,x3,x4)         
//#define Match6(x0,x1,x2,x3,x4,x5)       MatchN(6,x0,x1,x2,x3,x4,x5)      
//#define Match7(x0,x1,x2,x3,x4,x5,x6)    MatchN(7,x0,x1,x2,x3,x4,x5,x6)   
//#define Match8(x0,x1,x2,x3,x4,x5,x6,x7) MatchN(8,x0,x1,x2,x3,x4,x5,x6,x7)

#define XTL_DYN_CAST_FROM(i,...) (__casted_ptr##i = dynamic_cast<const XTL_SELECT_ARG(i,__VA_ARGS__)*>(subject_ptr##i)) != 0
#define XTL_ASSIGN_OFFSET(i,...) __switch_info.offset[i] = intptr_t(__casted_ptr##i)-intptr_t(subject_ptr##i);
#define XTL_ADJUST_PTR_FROM(i,...) auto& match##i = *mch::adjust_ptr<XTL_SELECT_ARG(i,__VA_ARGS__)>(subject_ptr##i,__switch_info.offset[i]); XTL_UNUSED(match##i)

/// Helper macro for #Case
/// NOTE: It is possible to have if conditions sequenced instead of &&, but that
///       doesn't seem to help compiler figuring out it got same dynamic_cast calls.
#define CaseN(N, ...)                                                          \
        }                                                                      \
        if (XTL_REPEAT_WITH(&&, N, XTL_DYN_CAST_FROM, __VA_ARGS__))            \
        {                                                                      \
            static_assert(number_of_subjects == N, "Number of targets in the case clause must be the same as the number of subjects in the Match statement"); \
            enum { target_label = XTL_COUNTER-__base_counter, is_inside_case_clause = 1 }; \
            if (XTL_LIKELY(__switch_info.target == 0))                         \
            {                                                                  \
                __switch_info.target = target_label;                           \
                XTL_REPEAT(N, XTL_ASSIGN_OFFSET)                               \
            }                                                                  \
        case target_label:                                                     \
            XTL_REPEAT(N, XTL_ADJUST_PTR_FROM, __VA_ARGS__)

#if defined(_MSC_VER)
    /// FIX: For some reason we need to make this extra hoop to make MSVC preprocessor do what we want
    #define Case_(N,...) XTL_APPLY_VARIADIC_MACRO(CaseN,(N,__VA_ARGS__))
    /// General Match statement supporting multiple scrutiny (subjects)
    #define Case(...) Case_(XTL_NARG(__VA_ARGS__),__VA_ARGS__)
#else
    /// General Case statement supporting multiple targets
    #define Case(...) CaseN(XTL_NARG(__VA_ARGS__), __VA_ARGS__);
#endif

/// A set of macros handling specific number of subjects passed to #Match(...).
//#define Case0()                        static_assert(false,"Case clause has to have at least 1 target");
//#define Case1(x0)                      CaseN(1,x0)
//#define Case2(x0,x1)                   CaseN(2,x0,x1)                  
//#define Case3(x0,x1,x2)                CaseN(3,x0,x1,x2)               
//#define Case4(x0,x1,x2,x3)             CaseN(4,x0,x1,x2,x3)            
//#define Case5(x0,x1,x2,x3,x4)          CaseN(5,x0,x1,x2,x3,x4)         
//#define Case6(x0,x1,x2,x3,x4,x5)       CaseN(6,x0,x1,x2,x3,x4,x5)      
//#define Case7(x0,x1,x2,x3,x4,x5,x6)    CaseN(7,x0,x1,x2,x3,x4,x5,x6)   
//#define Case8(x0,x1,x2,x3,x4,x5,x6,x7) CaseN(8,x0,x1,x2,x3,x4,x5,x6,x7)

#define Otherwise()                                                            \
            static_assert(is_inside_case_clause, "Otherwise() must follow actual clauses! If you are trying to use it as a default sub-clause, use When() instead"); \
        }                                                                      \
        {                                                                      \
            enum { target_label = XTL_COUNTER-__base_counter, is_inside_case_clause = 1 }; \
            if (XTL_LIKELY(__switch_info.target == 0))                         \
                __switch_info.target = target_label;                           \
        case target_label:

//#define Otherwise0() static_assert(false,"Otherwise clause has to have at least 1 target");
//#define Otherwise1() OtherwiseN(1)
//#define Otherwise2() OtherwiseN(2)
//#define Otherwise3() OtherwiseN(3)
//#define Otherwise4() OtherwiseN(4)
//#define Otherwise5() OtherwiseN(5)
//#define Otherwise6() OtherwiseN(6)
//#define Otherwise7() OtherwiseN(7)
//#define Otherwise8() OtherwiseN(8)

/// General EndMatch statement
#define EndMatch                                                               \
        }                                                                      \
        if (XTL_UNLIKELY((__switch_info.target == 0)))                         \
        {                                                                      \
            enum { target_label = XTL_COUNTER-__base_counter };                \
            XTL_SET_TYPES_NUM_ESTIMATE(target_label-1);                        \
            __switch_info.target = target_label;                               \
            case target_label: ;                                               \
        }                                                                      \
        }}

//#define EndMatch0 static_assert(false,"Invalid EndMatch");
//#define EndMatch1 EndMatchN(1)
//#define EndMatch2 EndMatchN(2)
//#define EndMatch3 EndMatchN(3)
//#define EndMatch4 EndMatchN(4)
//#define EndMatch5 EndMatchN(5)
//#define EndMatch6 EndMatchN(6)
//#define EndMatch7 EndMatchN(7)
//#define EndMatch8 EndMatchN(8)

//------------------------------------------------------------------------------
