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

template <typename S, typename C = void>
struct dynamic_cast_when_polymorphic_helper
{
    template <typename T>
    static inline const S* go(const S* s) { return s; }
};

template <typename S>
struct dynamic_cast_when_polymorphic_helper<S, typename std::enable_if<std::is_polymorphic<S>::value>::type> 
{
    template <typename T>
    static inline T go(const S* s) { return dynamic_cast<T>(s); }
};
/*
/// Behaves as dynamic_cast on pointers when argument is polymorphic.
template <typename T, typename S>
inline auto dynamic_cast_when_polymorphic(const S* s) -> std::enable_if< std::is_polymorphic<S>::value,T>
{
    return dynamic_cast<T>(s);
}

/// Otherwises behaves as identity on pointers when the argument is not polymorphic.
template <typename T, typename S>
inline auto dynamic_cast_when_polymorphic(const S* s) -> std::enable_if<!std::is_polymorphic<S>::value,T>
{
    return s;
}
*/

template <typename T, typename S>
inline auto dynamic_cast_when_polymorphic(const S* s) -> XTL_RETURN
(
    dynamic_cast_when_polymorphic_helper<S>::template go<T>(s)
)

} // of namespace mch

#define XTL_GET_TYPES_NUM_ESTIMATE   (mch::deferred_constant<mch::vtbl_count_t>::get<match_uid_type>::value)
#define XTL_SET_TYPES_NUM_ESTIMATE(N) mch::deferred_constant<mch::vtbl_count_t>::set<match_uid_type,(N)>::value_ptr

//------------------------------------------------------------------------------

/// Predefined value representing a layout used by default when none is specified.
//enum { default_layout = size_t(~0) };

//------------------------------------------------------------------------------

/// Common definitions generated at the beginning of #Match statement for a 
/// subject s in position N
#define XTL_MATCH_SUBJECT(N,s)                                                 \
        auto&&     subject_ref##N = s;                                         \
        auto const subject_ptr##N = mch::addr(subject_ref##N);                 \
        typedef XTL_CPP0X_TYPENAME mch::underlying<decltype(*subject_ptr##N)>::type source_type##N; \
        typedef source_type##N target_type##N;                                 \
        XTL_ASSERT(("Trying to match against a nullptr",subject_ptr##N));      \
        enum { is_polymorphic##N = std::is_polymorphic<source_type##N>::value, \
               polymorphic_index##N = XTL_CONCAT(polymorphic_index,XTL_PREV(N)) + is_polymorphic##N }; \
        auto& match##N = *subject_ptr##N;                                      \
        XTL_UNUSED(match##N);

//------------------------------------------------------------------------------

/// Common definitions generated at the beginning of #Match statement for a 
/// polymorphic subject s in position N
#define XTL_MATCH_SUBJECT_POLYMORPHIC(N,s)                                     \
        XTL_MATCH_SUBJECT(N,s)                                                 \
        /*static_assert(std::is_polymorphic<source_type##N>::value, "Type of subject " #N " should be polymorphic when you use Match");*/\
        register const void* __casted_ptr##N = 0;

/// Extension of #XTL_MATCH_SUBJECT_POLYMORPHIC where a list of subjects is 
/// passed and we have to pick up i-th subject. Used in repetitions.
#define XTL_MATCH_SUBJECT_POLYMORPHIC_FROM(i,...) XTL_MATCH_SUBJECT_POLYMORPHIC(i,XTL_SELECT_ARG(i,__VA_ARGS__))

//------------------------------------------------------------------------------

/// A common macro used in repetitions to prefix the iterator with given token
#define XTL_PREFIX(n,...) __VA_ARGS__##n

#define XTL_GET_VTLB_OF_SUBJECT(N,...) mch::vtbl_of(subject_ptr##N)
#define XTL_POLY_INDEX(N,...) 

//------------------------------------------------------------------------------

/// Helper macro for #Match
#define MatchN(N, ...) {                                                       \
        struct match_uid_type {};                                              \
        enum {                                                                 \
            is_inside_case_clause = 0,                                         \
            number_of_subjects = N,                                            \
            polymorphic_index00 = -1,                                          \
            __base_counter = XTL_COUNTER                                       \
        };                                                                     \
        XTL_REPEAT(N,XTL_MATCH_SUBJECT_POLYMORPHIC_FROM,__VA_ARGS__)           \
        enum { number_of_polymorphic_subjects = XTL_REPEAT_WITH(+,N, XTL_PREFIX, is_polymorphic) }; \
        /*const intptr_t __vtbl[N] = {XTL_ENUM(N,XTL_GET_VTLB_OF_SUBJECT)};*/      \
        typedef mch::vtbl_map<number_of_polymorphic_subjects,mch::type_switch_info<number_of_polymorphic_subjects>> vtbl_map_type; \
        XTL_PRELOADABLE_LOCAL_STATIC(vtbl_map_type,__vtbl2case_map,match_uid_type,XTL_DUMP_PERFORMANCE_ONLY(__FILE__,__LINE__,XTL_FUNCTION,)XTL_GET_TYPES_NUM_ESTIMATE);\
        mch::type_switch_info<number_of_polymorphic_subjects>& __switch_info = __vtbl2case_map.get(XTL_ENUM(N,XTL_PREFIX,subject_ptr)); \
        switch (number_of_polymorphic_subjects ? __switch_info.target : 0) {                                        \
        default: {{{

#if defined(_MSC_VER)
    /// FIX: For some reason we need to make this extra hoop to make MSVC preprocessor do what we want
    #define Match_(N,...) XTL_APPLY_VARIADIC_MACRO(MatchN,(N,__VA_ARGS__))
    /// General Match statement supporting multiple scrutiny (subjects)
    #define Match(...) Match_(XTL_NARG(__VA_ARGS__),__VA_ARGS__)
#else
    /// General Match statement supporting multiple scrutiny (subjects)
    #define Match(...) MatchN(XTL_NARG(__VA_ARGS__),__VA_ARGS__)
#endif

#define XTL_DECLARE_TARGET_TYPES(i,...)                                        \
        typedef XTL_CPP0X_TYPENAME mch::underlying<decltype(mch::filter(XTL_SELECT_ARG(i,__VA_ARGS__)))>::type type_of_pattern##i; \
        static_assert(mch::is_pattern<type_of_pattern##i>::value,"Case-clause expects patterns as its arguments"); \
        typedef XTL_CPP0X_TYPENAME mch::underlying<type_of_pattern##i>::type::/*XTL_CPP0X_TEMPLATE*/ accepted_type_for<source_type##i>::type target_type##i;
#define XTL_DYN_CAST_FROM(i,...) (__casted_ptr##i = mch::dynamic_cast_when_polymorphic<const target_type##i*>(subject_ptr##i)) != 0
#define XTL_ASSIGN_OFFSET(i,...) XTL_STATIC_IF(is_polymorphic##i) __switch_info.offset[polymorphic_index##i] = intptr_t(__casted_ptr##i)-intptr_t(subject_ptr##i);
#define XTL_ADJUST_PTR_FROM(i,...) auto& match##i = *mch::adjust_ptr_if_polymorphic<target_type##i>(subject_ptr##i,__switch_info.offset[polymorphic_index##i]);
#define XTL_MATCH_PATTERN_TO_TARGET(i,...) mch::filter(XTL_SELECT_ARG(i,__VA_ARGS__))(match##i)

/// Helper macro for #Case
/// NOTE: It is possible to have if conditions sequenced instead of &&, but that
///       doesn't seem to help compiler figuring out it got same dynamic_cast calls.
/// FIX: Neither mentioning only type nor omitting variable name works.
#define CaseN(N, ...)                                                          \
        }}}                                                                    \
        {                                                                      \
        XTL_REPEAT(N, XTL_DECLARE_TARGET_TYPES, __VA_ARGS__)                   \
        if (XTL_REPEAT_WITH(&&, N, XTL_DYN_CAST_FROM, __VA_ARGS__))            \
        {                                                                      \
            static_assert(number_of_subjects == N, "Number of targets in the case clause must be the same as the number of subjects in the Match statement"); \
            enum { target_label = XTL_COUNTER-__base_counter, is_inside_case_clause = 1 }; \
            XTL_STATIC_IF(number_of_polymorphic_subjects)                      \
            if (XTL_LIKELY(__switch_info.target == 0))                         \
            {                                                                  \
                __switch_info.target = target_label;                           \
                XTL_REPEAT(N, XTL_ASSIGN_OFFSET)                               \
            }                                                                  \
        case target_label:                                                     \
            XTL_REPEAT(N, XTL_ADJUST_PTR_FROM, __VA_ARGS__)                    \
            if (XTL_REPEAT_WITH(&&, N, XTL_MATCH_PATTERN_TO_TARGET, __VA_ARGS__)) {
            

#if defined(_MSC_VER)
    /// FIX: For some reason we need to make this extra hoop to make MSVC preprocessor do what we want
    #define Case_(N,...) XTL_APPLY_VARIADIC_MACRO(CaseN,(N,__VA_ARGS__))
    /// General Match statement supporting multiple scrutiny (subjects)
    #define Case(...) Case_(XTL_NARG(__VA_ARGS__),__VA_ARGS__)
#else
    /// General Case statement supporting multiple targets
    #define Case(...) CaseN(XTL_NARG(__VA_ARGS__), __VA_ARGS__);
#endif

#define Otherwise()                                                            \
            static_assert(is_inside_case_clause, "Otherwise() must follow actual clauses! If you are trying to use it as a default sub-clause, use When() instead"); \
        }}}                                                                    \
        {{{                                                                    \
            enum { target_label = XTL_COUNTER-__base_counter, is_inside_case_clause = 1 }; \
            if (XTL_LIKELY(__switch_info.target == 0))                         \
                __switch_info.target = target_label;                           \
        case target_label:

/// General EndMatch statement
#define EndMatch                                                               \
        }}}                                                                    \
        XTL_STATIC_IF(number_of_polymorphic_subjects)                          \
        if (XTL_UNLIKELY((__switch_info.target == 0)))                         \
        {                                                                      \
            enum { target_label = XTL_COUNTER-__base_counter };                \
            XTL_SET_TYPES_NUM_ESTIMATE(target_label-1);                        \
            __switch_info.target = target_label;                               \
            case target_label: ;                                               \
        }                                                                      \
        }}

//------------------------------------------------------------------------------
