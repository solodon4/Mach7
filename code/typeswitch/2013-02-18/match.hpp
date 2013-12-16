///
/// \file match.hpp
///
/// This file defines the necessary infrastructure for ML-like pattern matching
/// against trees in C++.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of Mach7 library (http://parasol.tamu.edu/mach7/).
/// Copyright (C) 2011-2013 Texas A&M University.
/// All rights reserved.
///

#pragma once

//------------------------------------------------------------------------------

//  FIX: Big question: should null pointers match pointer variables??? NO
//  n/a: Replace all MatchX_N versions of the macro with variadic macro 
//       taking either 1 or 2 arguments
// done: Make non-null version of matchers to call from match statement and avoid checks
// TODO: Make a test case that tests all features in template context
// TODO: Provide implementation when variadic templates are supported
//  FIX: Remove const currently added on auto-declared references to allow modification
// done: Preallocate cache for number of case clauses
// done: Add probabilities on classes to be taken into account for collisions
// TODO: Add support for multiple scrutinies
// done: Add static assert that verifies types in clauses to be related to source type
//       in the same way dynamic_cast does. This cannot always be check that target type
//       is derived from source type because we may have cross cast, but we can add
//       a configuration macro that says that hierarchy is not using multiple inheritance
//       in which case such assertion will be valid. This can be made default too.
// done: Define logic with dedicated value used by closed matching and the way user
//       can override that value when it conflicts with one of its values.
// n/a:  Add static_asserts that check that case constants are not equal to dedicate value
// done: Implement Otherwise for closed cases
// TODO: Test cases should test:
//        - template vs. non-template context
//        - presense and absense of Otherwise as well as when user disabled its use
//        - const vs. non-const subject
//        - presence and absense of forwarding to base classes
//        - usage of fall-through behavior
//        - use of implicitly declared variables in Case and explicitly declared in Qua
//        - read and write of explicitly declared variables
//        - subject by reference and by pointer
//        - use of generic and specialized syntax
//        - multiple match statements within the same function (to check labels and instantiations)
//        - presense of multiple inheritance: both repeated and virtual
//        - presense of ambiguous bases and casts
//        - use of exceptions in: code associated with clauses, bound members
//        - use of separate translation units, libraries and DLLs
//        - passing null-pointer to Match in release/debug builds
//        - use of layouts and views
// FIX: Add support of guards |= on other patterns e.g. matcherN
// TODO: Check if implicit breaks render better code than code with ifs for fall-through behavior
// TODO: Make a configuration option that uses static_cast directly instead of 
//       our ptr_adjust. It is an option as static_cast won't work for virtual
//       inheritance or ambiguous base classes, while in typical case we won't 
//       need to store and will simply inline adjustment.
// TODO: Overload comma operator on patterns to handle multiple subjects
// TODO: Make the following two syntaxes work:
//        - if (match(c).with(re,im)) ...
//        - if (cons<complex<float>>(re,im) == c) ...
//        - if (Po(P1,...,Pn).matches(v))
// FIX:  number of clauses is only used for MatchP at the moment, not for MatchG

// NOTE: If Visual C++ gives you any of the following errors:
//       error C2051: case expression not constant
//       error C2057: expected constant expression
//       on any macro that uses __LINE__ in them, just change the Debug Format 
//       in project settings:
//       Project -> Properties -> C/C++ -> General -> Debug Information Format 
//       from "Program Database for Edit And Continue (/ZI)" 
//       to   "Program Database (/Zi)", which is the default in Release builds,
//       but not in Debug. This is a known bug of Visual C++ described here:
//       http://connect.microsoft.com/VisualStudio/feedback/details/375836/-line-not-seen-as-compile-time-constant

//------------------------------------------------------------------------------

#include "unisyn.hpp"

#if defined(_MSC_VER) && !defined(_CPPRTTI)
    /// Disabling RTTI in MSVC is known to enable compiler optimizations that
    /// may render type switching on a polymorphic object type unsafe.
    XTL_MESSAGE("WARNING: Polymorphic type switch is potentially unsafe when RTTI is disabled!")
#endif

//------------------------------------------------------------------------------

#if XTL_USE_MEMOIZED_CAST
  #include "memoized_cast.hpp"
  #define dynamic_cast memoized_cast
#else
  #define memoized_cast dynamic_cast
#endif

//------------------------------------------------------------------------------

/// FIX: This specialization doesn't actually help/work as expected as get<UID>
///      at the time of this instantiation is not yet set<UID>.
template <typename T, typename UID>
struct preallocated<vtblmap<T&>,UID>
{
    static vtblmap<T&> value;
};

template <typename T, typename UID>
vtblmap<T&> preallocated<vtblmap<T&>,UID>::value(
    deferred_constant<vtbl_count_t>::get<UID>::value 
        ? deferred_constant<vtbl_count_t>::get<UID>::value 
        : min_expected_size
);

//------------------------------------------------------------------------------

/// Helper function that is enabled when user provided frequency function for the
/// source type.
template <typename T>
inline typename std::enable_if<has_member_frequency<bindings<T>>::value, size_t>::type
get_frequency_ex(intptr_t vtbl)
{
    return bindings<T>::frequency(vtbl);
}

/// Helper function that is enabled when user did not provide frequency function
/// for source type.
template <typename T>
inline typename std::enable_if<!has_member_frequency<bindings<T>>::value, size_t>::type
get_frequency_ex(intptr_t)
{
    // FIX: Make this function warn user
    return 1;
}

/// We need this extra wrapper to be able to forward declare it from @vtblmap's header
template <typename T> inline size_t get_frequency(intptr_t vtbl) { return get_frequency_ex<T>(vtbl); }

//------------------------------------------------------------------------------

template <class T> inline void ignore_unused_warning(T const&) {}

//------------------------------------------------------------------------------

/// A helper macro to access kind value of a class
#define BCK(D,B) (ignore_unused_warning(associate_kinds<D,B>::kinds),remapped<B>::lbl)

/// A set of macros handling various amount of base classes passed to BCS macro.
#define BCS0()                        static const lbl_type* get_kinds() { static const lbl_type kinds[] = { lbl_type(0) }; return kinds; }
#define BCS1(x0)                      static const lbl_type* get_kinds() { static const lbl_type kinds[] = { BCK(D,x0), lbl_type(0) }; return kinds; }
#define BCS2(x0,x1)                   static const lbl_type* get_kinds() { static const lbl_type kinds[] = { BCK(D,x0), BCK(D,x1), lbl_type(0) }; return kinds; }
#define BCS3(x0,x1,x2)                static const lbl_type* get_kinds() { static const lbl_type kinds[] = { BCK(D,x0), BCK(D,x1), BCK(D,x2), lbl_type(0) }; return kinds; }
#define BCS4(x0,x1,x2,x3)             static const lbl_type* get_kinds() { static const lbl_type kinds[] = { BCK(D,x0), BCK(D,x1), BCK(D,x2), BCK(D,x3), lbl_type(0) }; return kinds; }
#define BCS5(x0,x1,x2,x3,x4)          static const lbl_type* get_kinds() { static const lbl_type kinds[] = { BCK(D,x0), BCK(D,x1), BCK(D,x2), BCK(D,x3), BCK(D,x4), lbl_type(0) }; return kinds; }
#define BCS6(x0,x1,x2,x3,x4,x5)       static const lbl_type* get_kinds() { static const lbl_type kinds[] = { BCK(D,x0), BCK(D,x1), BCK(D,x2), BCK(D,x3), BCK(D,x4), BCK(D,x5), lbl_type(0) }; return kinds; }
#define BCS7(x0,x1,x2,x3,x4,x5,x6)    static const lbl_type* get_kinds() { static const lbl_type kinds[] = { BCK(D,x0), BCK(D,x1), BCK(D,x2), BCK(D,x3), BCK(D,x4), BCK(D,x5), BCK(D,x6), lbl_type(0) }; return kinds; }
#define BCS8(x0,x1,x2,x3,x4,x5,x6,x7) static const lbl_type* get_kinds() { static const lbl_type kinds[] = { BCK(D,x0), BCK(D,x1), BCK(D,x2), BCK(D,x3), BCK(D,x4), BCK(D,x5), BCK(D,x6), BCK(D,x7), lbl_type(0) }; return kinds; }

/// Helper macro for the one below
#define BCS_(N, ...) XTL_CONCAT(BCS, N)(__VA_ARGS__)
/// A macro that will be passed arguments to case statement. This should include
/// the first type parameter that will be used as a dummy. This is required to
/// be able to handle 0 non-type parameters.
#define  BCS(...) typedef          get_param<bindings>::type D; BCS_(XTL_NARG(__VA_ARGS__),##__VA_ARGS__)
#define TBCS(...) typedef typename get_param<bindings>::type D; BCS_(XTL_NARG(__VA_ARGS__),##__VA_ARGS__)

//------------------------------------------------------------------------------

/// A macro to declare implicitly a reference variable with name V bound to 
/// a value in position P of the target type.
/// FIX: Try without const to bind also for modification
#define XTL_VAR_DECL(P,V) const auto& V = apply_member(matched, bindings<target_type,target_layout>::XTL_CONCAT(member,P)())

/// A set of macros handling various amount of arguments passed to case statement.
/// FIX: The reason macro for 0 arguments doesn't take Dummy but ... is because 
///      apparently Dummy is not passed in case of 0 (only) and MSVC gives us:
///      warning C4003: not enough actual parameters for macro 'XTL_DECL_BOUND_VAR_0'
#define XTL_DECL_BOUND_VAR_0(...)                           XTL_UNUSED(matched);
#define XTL_DECL_BOUND_VAR_1(Dummy,x0)                      XTL_VAR_DECL(0,x0);
#define XTL_DECL_BOUND_VAR_2(Dummy,x0,x1)                   XTL_VAR_DECL(0,x0); XTL_VAR_DECL(1,x1);
#define XTL_DECL_BOUND_VAR_3(Dummy,x0,x1,x2)                XTL_VAR_DECL(0,x0); XTL_VAR_DECL(1,x1); XTL_VAR_DECL(2,x2);
#define XTL_DECL_BOUND_VAR_4(Dummy,x0,x1,x2,x3)             XTL_VAR_DECL(0,x0); XTL_VAR_DECL(1,x1); XTL_VAR_DECL(2,x2); XTL_VAR_DECL(3,x3);
#define XTL_DECL_BOUND_VAR_5(Dummy,x0,x1,x2,x3,x4)          XTL_VAR_DECL(0,x0); XTL_VAR_DECL(1,x1); XTL_VAR_DECL(2,x2); XTL_VAR_DECL(3,x3); XTL_VAR_DECL(4,x4);
#define XTL_DECL_BOUND_VAR_6(Dummy,x0,x1,x2,x3,x4,x5)       XTL_VAR_DECL(0,x0); XTL_VAR_DECL(1,x1); XTL_VAR_DECL(2,x2); XTL_VAR_DECL(3,x3); XTL_VAR_DECL(4,x4); XTL_VAR_DECL(5,x5);
#define XTL_DECL_BOUND_VAR_7(Dummy,x0,x1,x2,x3,x4,x5,x6)    XTL_VAR_DECL(0,x0); XTL_VAR_DECL(1,x1); XTL_VAR_DECL(2,x2); XTL_VAR_DECL(3,x3); XTL_VAR_DECL(4,x4); XTL_VAR_DECL(5,x5); XTL_VAR_DECL(6,x6);
#define XTL_DECL_BOUND_VAR_8(Dummy,x0,x1,x2,x3,x4,x5,x6,x7) XTL_VAR_DECL(0,x0); XTL_VAR_DECL(1,x1); XTL_VAR_DECL(2,x2); XTL_VAR_DECL(3,x3); XTL_VAR_DECL(4,x4); XTL_VAR_DECL(5,x5); XTL_VAR_DECL(6,x6); XTL_VAR_DECL(7,x7);

/// Helper macro for the one below
#define XTL_DECL_BOUND_VAR_(N, ...) XTL_CONCAT(XTL_DECL_BOUND_VAR_, N)(__VA_ARGS__)
/// A macro that will be passed arguments to case statement. This should include
/// the first type parameter that will be used as a dummy. This is required to
/// be able to handle 0 non-type parameters.
#define XTL_DECL_BOUND_VARS(...) XTL_DECL_BOUND_VAR_(XTL_NARG_EX(__VA_ARGS__),##__VA_ARGS__)

//------------------------------------------------------------------------------

/// When this macro is defined, our library will generate additional code that 
/// will trigger compiler to check case clauses for redundancy.
/// \warning Do not define this macro in actual builds as the generated code 
///          will effectively have a switch statement whose body is never evaluated!
/// In type-checking builds we sprinkle in try to start a try-catch block 
/// that will trigger redundancy warnings by a compiler. The case clauses will
/// be wrapped with a catch handler on target type to let the compiler
/// check case-clauses for redundancy by checking catch-handlers for redundancy.
/// We need to hide case labels during redundancy checking as those will skip 
/// variable initialization inside the catch handler. This is what makes the 
/// underlain switch statement essentially empty as there won't be any case 
/// labels in it, unless user provided a default.
#if XTL_REDUNDANCY_CHECKING
  /// During redundancy checking mode all case labels are removed from the 
  /// underlying switch statement, making it effectively empty. Your code will
  /// still compile, but during execution no switching will happen until you
  /// disable redundancy checking macro again.
  /// You should also be ready that in this mode, case clauses with the same
  /// target type (which is allowed for open case) will report an error in MSVC
  XTL_MESSAGE("WARNING: Redundancy checking is enabled. Match statements will not be executed until redundancy checking is disabled again!")
  /// Even though normal case labels are agnostic of the block structure around them,
  /// which lets us use or not use {} around case clauses, the try-catch structure
  /// used for redundancy checking does not have this flexibility as catch handlers
  /// should correspond to the try block. To assure generation of proper try-catch 
  /// structure, the use of {} inside #Match statement should correspond to the 
  /// #XTL_USE_BRACES library configuration flag.
  XTL_MESSAGE("WARNING: The use of { and } within Match-statement must match XTL_USE_BRACES setting for redundancy checking mode!")
  /// In type-checking builds we resolve it to try to start a try-catch block 
  /// that will trigger redundancy warnings by a compiler.
  #define XTL_REDUNDANCY_TRY       try
  /// Wraps case clause with a catch handler on target type to let the compiler
  /// check case-clauses for redundancy by checking catch-handlers for redundancy.
  #define XTL_REDUNDANCY_CATCH(C)  catch(C*)
  /// We need to hide case labels during redundancy checking as those will skip 
  /// variable initialization inside the catch handler. This is what makes the 
  /// underlain switch statement essentially empty as there won't be any case 
  /// labels in it, unless user provided a default.
  #define XTL_REDUNDANCY_LABEL(lbl)
#else
  /// We do not generate try statement in release builds, they were only needed 
  /// for redundancy checking.
  /// FIX: We still need to discuss behavior of our mechanism when exceptions are presents
  #define XTL_REDUNDANCY_TRY
  /// We do not generate catch handlers either in release builds
  #define XTL_REDUNDANCY_CATCH(C)
  /// Case labels are now visible again.
  #define XTL_REDUNDANCY_LABEL(lbl) lbl
#endif

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
/// - matched refers the subject by default (used for When sub-clauses)
/// - the subject cannot be a nullptr - we assert at run-time (debug) if it is
#define XTL_MATCH_PREAMBULA(s)                                                 \
        struct match_uid_type {};                                              \
        auto&&     subject_ref = s;                                            \
        auto const subject_ptr = addr(subject_ref);                            \
        typedef XTL_CPP0X_TYPENAME underlying<decltype(*subject_ptr)>::type source_type; \
        typedef source_type target_type;                                       \
        enum { target_layout = default_layout, is_inside_case_clause = 0 };    \
        XTL_ASSERT(("Trying to match against a nullptr",subject_ptr));         \
        auto const matched = subject_ptr;                                      \
        XTL_UNUSED(matched);

#define XTL_SUBCLAUSE_FIRST           XTL_NON_FALL_THROUGH_ONLY(XTL_STATIC_IF(false)) XTL_NON_USE_BRACES_ONLY({)
#define XTL_SUBCLAUSE_OPEN(...)                                       XTL_STATIC_IF(UCL_PP_IF(UCL_PP_IS_EMPTY(__VA_ARGS__), true,   XTL_LIKELY(cons<target_type,target_layout>(__VA_ARGS__).match_structure(matched)))) {
#define XTL_SUBCLAUSE_CONTINUE(...) } XTL_NON_FALL_THROUGH_ONLY(else) XTL_STATIC_IF(UCL_PP_IF(UCL_PP_IS_EMPTY(__VA_ARGS__), true, XTL_UNLIKELY(cons<target_type,target_layout>(__VA_ARGS__).match_structure(matched)))) {
#define XTL_SUBCLAUSE_PATTERN(...)  } XTL_NON_FALL_THROUGH_ONLY(else) XTL_STATIC_IF(UCL_PP_IF(UCL_PP_IS_EMPTY(__VA_ARGS__), true, XTL_UNLIKELY(filter(__VA_ARGS__)(*matched)))) {
#define XTL_SUBCLAUSE_CLOSE         }                            XTL_NON_FALL_THROUGH_ONLY(XTL_STATIC_IF(is_inside_case_clause) break;)
#define XTL_SUBCLAUSE_LAST            XTL_NON_USE_BRACES_ONLY(}) XTL_NON_FALL_THROUGH_ONLY(XTL_STATIC_IF(is_inside_case_clause) break;)


/// FIX: Workaround
#if XTL_CLAUSE_DECL
#define XTL_CLAUSE_OTHERWISE(CaseClause,...)                                   \
        static_assert(is_inside_case_clause, "Otherwise() must follow actual clauses! If you are trying to use it as a default sub-clause, use When() instead"); \
        CaseClause(const source_type& _dummy_ UCL_PP_IF(UCL_PP_IS_EMPTY(__VA_ARGS__), UCL_PP_EMPTY(), ,) __VA_ARGS__)
#else
#define XTL_CLAUSE_OTHERWISE(CaseClause,...)                                   \
        static_assert(is_inside_case_clause, "Otherwise() must follow actual clauses! If you are trying to use it as a default sub-clause, use When() instead"); \
        CaseClause(source_type UCL_PP_IF(UCL_PP_IS_EMPTY(__VA_ARGS__), UCL_PP_EMPTY(), ,) __VA_ARGS__)
#endif

/// This is a common part of most clause implementations, which:
/// - Introduces named constant into the scope to indicate we are in clause's scope
/// - Extracts type from C when it is allowed to be a declaration
#define XTL_CLAUSE_COMMON(C)                                                   \
        enum { is_inside_case_clause = 1 };                                    \
        XTL_NON_CLAUSE_DECL_ONLY(typedef C target_type);                       \
        XTL_CLAUSE_DECL_ONLY(extern int decl_helper(C));                       \
        XTL_CLAUSE_DECL_ONLY(typedef XTL_CPP0X_TYPENAME underlying<XTL_CPP0X_TYPENAME get_first_param<decltype(decl_helper)>::type>::type target_type);

#if XTL_CLAUSES_NUM_ESTIMATES_TYPES_NUM
    #define XTL_GET_TYPES_NUM_ESTIMATE   (deferred_constant<vtbl_count_t>::get<match_uid_type>::value)
    #define XTL_SET_TYPES_NUM_ESTIMATE(N) ignore_unused_warning(deferred_constant<vtbl_count_t>::set<match_uid_type,(N)>::value_ptr)
#else
    #define XTL_GET_TYPES_NUM_ESTIMATE   (min_expected_size)
    #define XTL_SET_TYPES_NUM_ESTIMATE(N)
#endif

//------------------------------------------------------------------------------
/// Few general rules to understand behavior of various #Match statements below:
/// - Each Case, Qua and When clauses should close as many braces as they open
/// - When-clause should open less braces than Qua and Case to allow sub-clauses
/// - Case and Qua clauses should open the same amount of braces since they can
///   be mixed in the same statement e.g. Otherwise is resolved to Case clause.
/// - Match statement based on switch should introduce more open braces than
///   its Case-clauses to allow optional { } surrounding clauses.
/// - Scopes are generally as following:
///   * Match-level scope to introduce subject_ptr, source_type, __base_counter
///   * switch-level scope to group case labels and allow use of break statement
///   * Clause-level scope introduces target_type shared by all sub-clauses
///   * Sub-clause-level scope introduces target_label etc.
///   * Optional scope introduced by conditions for sequential execution of statement
///   * Scope of user's statement to separate from scope of matched declaration
//------------------------------------------------------------------------------

/// Macro that starts the switch on pattern
/// \note case 0: instead of default: will work in the same way because we 
///       initialize cache with 0, however through experiments we can see
///       that having default here is quite a bit faster than having case 0
///       because one less branch should be generated
#define MatchP(s) {                                                            \
        XTL_MATCH_PREAMBULA(s)                                                 \
        enum { __base_counter = XTL_COUNTER };                                 \
        static_assert(std::is_polymorphic<source_type>::value, "Type of subject should be polymorphic when you use MatchP");\
        XTL_PRELOADABLE_LOCAL_STATIC(vtblmap<type_switch_info&>,__vtbl2lines_map,match_uid_type,XTL_DUMP_PERFORMANCE_ONLY(__FILE__,__LINE__,XTL_FUNCTION,)XTL_GET_TYPES_NUM_ESTIMATE);\
        register const void* __casted_ptr = 0;                                 \
        type_switch_info& __switch_info = __vtbl2lines_map.get(subject_ptr);\
        switch (__switch_info.line)                                            \
        {                                                                      \
            XTL_REDUNDANCY_TRY                                                 \
            {                                                                  \
                {                                                              \
                    XTL_REDUNDANCY_LABEL(default:)                             \
                    XTL_SUBCLAUSE_FIRST

/// Macro that defines the case statement for the above switch
#define QuaP(C,...)                                                            \
        XTL_SUBCLAUSE_CLOSE }}                                                 \
        XTL_REDUNDANCY_CATCH(C)                                                \
        {                                                                      \
            XTL_CLAUSE_COMMON(C);                                              \
            enum { target_label = XTL_COUNTER-__base_counter };                \
            __casted_ptr = dynamic_cast<const target_type*>(subject_ptr);      \
            if (XTL_UNLIKELY(__casted_ptr))                                    \
            {                                                                  \
                if (XTL_LIKELY((__switch_info.line == 0)))                     \
                {                                                              \
                    __switch_info.line = target_label;                         \
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(subject_ptr); \
                }                                                              \
            XTL_REDUNDANCY_LABEL(case target_label:)                           \
                auto matched = adjust_ptr<target_type>(subject_ptr,__switch_info.offset);\
                XTL_CLAUSE_DECL_ONLY(C(*matched));                             \
                XTL_UNUSED(matched);                                           \
                XTL_SUBCLAUSE_OPEN(__VA_ARGS__)

/// NOTE: We need this extra indirection to properly handle 0 arguments as it
///       seems to be impossible to introduce dummy argument inside the Case 
///       directly, so we use the type argument as a dummy argument for XTL_DECL_BOUND_VARS
#define CaseP_(C,...)   QuaP(C)                                                                         
#define CaseP(...)      XTL_APPLY_VARIADIC_MACRO(CaseP_,(__VA_ARGS__)) XTL_APPLY_VARIADIC_MACRO(XTL_DECL_BOUND_VARS,(__VA_ARGS__))
#define WhenP(...)      XTL_SUBCLAUSE_CONTINUE(__VA_ARGS__) __casted_ptr = subject_ptr;
#define PatternP(...)   XTL_SUBCLAUSE_PATTERN(__VA_ARGS__)  __casted_ptr = subject_ptr;
#define OtherwiseP(...) XTL_CLAUSE_OTHERWISE(CaseP,__VA_ARGS__)
#define EndMatchP                                                              \
        XTL_SUBCLAUSE_LAST }}                                                  \
        enum { target_label = XTL_COUNTER-__base_counter };                    \
        XTL_SET_TYPES_NUM_ESTIMATE(target_label-1);                            \
        if (XTL_UNLIKELY((__casted_ptr == 0 && __switch_info.line == 0))) { __switch_info.line = target_label; } \
        case target_label: ; }}

//------------------------------------------------------------------------------

/// Macro that starts the switch on types that carry their own dynamic type as
/// a distinct integral value in one of their members.
#define MatchK(s) {                                                            \
        XTL_MATCH_PREAMBULA(s)                                                 \
        static_assert(has_member_kind_selector<bindings<source_type>>::value, "Before using MatchK, you have to specify kind selector on the source type using KS macro");\
        auto const __kind_selector = kind_selector(subject_ptr);               \
        switch (__kind_selector) { { XTL_SUBCLAUSE_FIRST

/// Macro that defines the case statement for the above switch
#define QuaK(C,...)                                                            \
        XTL_SUBCLAUSE_CLOSE }                                                  \
        if (XTL_UNLIKELY((size_t(__kind_selector) == size_t(bindings<C>::kind_value)))) \
        {                                                                      \
        case bindings<C>::kind_value:                                          \
            XTL_CLAUSE_COMMON(C);                                              \
            auto matched = stat_cast<target_type>(subject_ptr);                \
            XTL_CLAUSE_DECL_ONLY(C(*matched));                                 \
            XTL_UNUSED(matched);                                               \
            XTL_SUBCLAUSE_OPEN(__VA_ARGS__)

/// NOTE: We need this extra indirection to properly handle 0 arguments as it
///       seems to be impossible to introduce dummy argument inside the Case 
///       directly, so we use the type argument as a dummy argument for XTL_DECL_BOUND_VARS
#define CaseK_(C,...)   QuaK(C)
#define CaseK(...)      XTL_APPLY_VARIADIC_MACRO(CaseK_,(__VA_ARGS__)) XTL_APPLY_VARIADIC_MACRO(XTL_DECL_BOUND_VARS,(__VA_ARGS__))
#define WhenK(...)      XTL_SUBCLAUSE_CONTINUE(__VA_ARGS__)
#define PatternK(...)   XTL_SUBCLAUSE_PATTERN(__VA_ARGS__)
#define OtherwiseK(...)                                                        \
        } XTL_NON_FALL_THROUGH_ONLY(break;) }                                  \
        default: { XTL_APPLY_VARIADIC_MACRO(XTL_DECL_BOUND_VARS,(__VA_ARGS__)) {
#define EndMatchK       XTL_SUBCLAUSE_LAST } }}

//------------------------------------------------------------------------------

/// Macro that starts the switch on types that carry their own dynamic type as
/// a distinct integral value in one of their members.
#define MatchU(s) {                                                            \
        XTL_MATCH_PREAMBULA(s)                                                 \
        static_assert(has_member_kind_selector<bindings<source_type>>::value, "Before using MatchU, you have to specify kind selector on the subject type using KS macro");\
        auto const __kind_selector = kind_selector(subject_ptr);               \
        switch (__kind_selector) { { XTL_SUBCLAUSE_FIRST

/// Macro that defines the case statement for the above switch
#define QuaU(L,...)                                                            \
        XTL_SUBCLAUSE_CLOSE }                                                  \
        if (XTL_UNLIKELY((size_t(__kind_selector) == size_t(bindings<source_type,L>::kind_value)))) \
        {                                                                      \
        case bindings<source_type,L>::kind_value:                              \
            typedef source_type target_type;                                   \
            enum { target_layout = L, is_inside_case_clause = 1 };             \
            auto matched = subject_ptr;                                        \
            XTL_UNUSED(matched);                                               \
            XTL_SUBCLAUSE_OPEN(__VA_ARGS__)

#define CaseU_(L,...)   QuaU(L)
#define CaseU(...)      XTL_APPLY_VARIADIC_MACRO(CaseU_,(__VA_ARGS__)) XTL_APPLY_VARIADIC_MACRO(XTL_DECL_BOUND_VARS,(__VA_ARGS__))
#define WhenU(...)      XTL_SUBCLAUSE_CONTINUE(__VA_ARGS__)
#define PatternU(...)   XTL_SUBCLAUSE_PATTERN(__VA_ARGS__)
#define OtherwiseU(...)                                                        \
        } XTL_NON_FALL_THROUGH_ONLY(break;) }                                  \
        default: { XTL_APPLY_VARIADIC_MACRO(XTL_DECL_BOUND_VARS,(__VA_ARGS__)) {
#define EndMatchU       XTL_SUBCLAUSE_LAST } }}

//------------------------------------------------------------------------------

/// Macro that starts the switch on types that carry their own dynamic type as
/// a distinct integral value in one of their members.
/// \note Unlike the rest of our Match statements, MatchE does not allow {} 
///       around the case clauses.
#define MatchE(s) {                                                            \
        XTL_MATCH_PREAMBULA(s)                                                 \
        static_assert(has_member_raise_selector<bindings<source_type>>::value, "Before using MatchE, you have to specify raise selector on the subject type using RS macro");\
        XTL_MESSAGE("WARNING: The use of { and } within MatchE-statement must match XTL_USE_BRACES setting!") \
        try {                                                                  \
            raise_selector(subject_ptr);                                       \
            XTL_SUBCLAUSE_FIRST

/// Macro that defines the case statement for the above switch
#define QuaE(C,...) }}                                                         \
        catch (C& __matched)                                                   \
        {                                                                      \
            XTL_CLAUSE_COMMON(C);                                              \
            auto matched = &__matched;                                         \
            XTL_CLAUSE_DECL_ONLY(C(*matched));                                 \
            XTL_UNUSED(matched);                                               \
            XTL_SUBCLAUSE_OPEN(__VA_ARGS__)

#define CaseE_(C,...)   QuaE(C)
#define CaseE(...)      XTL_APPLY_VARIADIC_MACRO(CaseE_,(__VA_ARGS__)) XTL_APPLY_VARIADIC_MACRO(XTL_DECL_BOUND_VARS,(__VA_ARGS__))
#define WhenE(...)      XTL_SUBCLAUSE_CONTINUE(__VA_ARGS__)
#define PatternE(...)   XTL_SUBCLAUSE_PATTERN(__VA_ARGS__)
#define OtherwiseE(...) XTL_CLAUSE_OTHERWISE(CaseE,__VA_ARGS__)
#define EndMatchE       XTL_NON_USE_BRACES_ONLY(}) } catch (...) {} }

//------------------------------------------------------------------------------

/// Macro that starts the switch on types that carry their own dynamic type as
/// a distinct integral value in one of their members.
/// Non-forwarding: Sequential:  33% faster; Random: 34% faster
///     Forwarding: Sequential: 251% faster; Random: 33% faster
/// FIX: The use of vector and resize in it assumes at the moment small tags in 
///      region 0..k. Tag randomization will overuse memory!
#define MatchF(s) {                                                            \
        XTL_MATCH_PREAMBULA(s)                                                 \
        static_assert(has_member_kind_selector<bindings<source_type>>::value, "Before using MatchF, you have to specify kind selector on the subject type using KS macro");\
        lbl_type __kind_selector = original2remapped<source_type>(tag_type(kind_selector(subject_ptr))), __most_derived_kind_selector = __kind_selector;\
        XTL_UNUSED(__most_derived_kind_selector);                              \
        const lbl_type* __kinds = 0;                                           \
        XTL_CONCAT(ReMatch,__LINE__):                                          \
        switch (size_t(__kind_selector)) {                                     \
        default:                                                               \
            if (XTL_LIKELY(!__kinds))                                          \
            {                                                                  \
                XTL_PRELOADABLE_LOCAL_STATIC(std::vector<const lbl_type*>,__kinds_cache,match_uid_type);\
                if (XTL_UNLIKELY(size_t(__kind_selector) >= __kinds_cache.size()))\
                    __kinds_cache.resize(__kind_selector+1);                   \
                __kinds = __kinds_cache[__kind_selector];                      \
                if (!__kinds)                                                  \
                    __kinds = __kinds_cache[__kind_selector] = get_kinds<source_type>(__kind_selector);\
            }                                                                  \
            XTL_ASSERT(("Base classes for this kind were not specified",__kinds));\
            XTL_ASSERT(("Invalid list of kinds",*__kinds==__kind_selector));   \
            __kind_selector = __kinds ? *++__kinds : lbl_type(0);              \
            goto XTL_CONCAT(ReMatch,__LINE__);                                 \
        case 0: break; { XTL_SUBCLAUSE_FIRST

/// Macro that defines the case statement for the above switch
#define QuaF(C,...)                                                            \
        XTL_SUBCLAUSE_CLOSE }                                                  \
        if (XTL_UNLIKELY(is_base_and_derived_kinds<source_type>(remapped<C>::lbl, __most_derived_kind_selector))) \
        {                                                                      \
        case remapped<C>::lbl:                                                 \
            XTL_CLAUSE_COMMON(C);                                              \
            auto matched = stat_cast<target_type>(subject_ptr);                \
            XTL_CLAUSE_DECL_ONLY(C(*matched));                                 \
            XTL_UNUSED(matched);                                               \
            XTL_SUBCLAUSE_OPEN(__VA_ARGS__)

/// NOTE: We need this extra indirection to properly handle 0 arguments as it
///       seems to be impossible to introduce dummy argument inside the Case 
///       directly, so we use the type argument as a dummy argument for XTL_DECL_BOUND_VARS
#define CaseF_(C,...)   QuaF(C)
#define CaseF(...)      XTL_APPLY_VARIADIC_MACRO(CaseF_,(__VA_ARGS__)) XTL_APPLY_VARIADIC_MACRO(XTL_DECL_BOUND_VARS,(__VA_ARGS__))
#define WhenF(...)      XTL_SUBCLAUSE_CONTINUE(__VA_ARGS__)
#define PatternF(...)   XTL_SUBCLAUSE_PATTERN(__VA_ARGS__)
#define OtherwiseF(...) XTL_CLAUSE_OTHERWISE(CaseF,__VA_ARGS__)
#define EndMatchF       XTL_SUBCLAUSE_LAST }}}

//------------------------------------------------------------------------------

#if 0
/// Macro that starts the switch on types implemented as a sequential cascading-if
/// without any memoization. The idea is to provide a general syntax for cases not 
/// specifically interested in speed, but expressivty: e.g. algebraic decomposition
/// with n+k patterns or structural decomposition with nested patterns.
#define MatchS(s) {                                                            \
        XTL_MATCH_PREAMBULA(s)                                                 \
        enum { __base_counter = XTL_COUNTER };                                 \
        switch (1)                                                             \
        {                                                                      \
            default:                                                           \
            XTL_REDUNDANCY_TRY                                                 \
            {{ XTL_SUBCLAUSE_FIRST

/// Macro that defines the case statement for the above switch
#define QuaS(C,...)                                                            \
        XTL_SUBCLAUSE_CLOSE }}                                                 \
        XTL_REDUNDANCY_CATCH(C)                                                \
        { /* Clause */                                                         \
            XTL_CLAUSE_COMMON(C);                                              \
            XTL_REDUNDANCY_LABEL(case XTL_COUNTER-__base_counter:)             \
            if (auto matched = cons<C>()(subject_ptr))                         \
            { /* Sequential */                                                 \
                XTL_CLAUSE_DECL_ONLY(C(*matched));                             \
                XTL_UNUSED(matched);                                           \
                XTL_SUBCLAUSE_OPEN(__VA_ARGS__)

/// NOTE: We need this extra indirection to properly handle 0 arguments as it
///       seems to be impossible to introduce dummy argument inside the Case 
///       directly, so we use the type argument as a dummy argument for XTL_DECL_BOUND_VARS
#define CaseS_(C,...)   QuaS(C)
#define CaseS(...)      XTL_APPLY_VARIADIC_MACRO(CaseS_,(__VA_ARGS__)) XTL_APPLY_VARIADIC_MACRO(XTL_DECL_BOUND_VARS,(__VA_ARGS__))
#define WhenS(...)      XTL_SUBCLAUSE_CONTINUE(__VA_ARGS__)
#define PatternS(...)   XTL_SUBCLAUSE_PATTERN(__VA_ARGS__)
#define OtherwiseS(...) XTL_CLAUSE_OTHERWISE(CaseS,__VA_ARGS__)
#define EndMatchS       XTL_SUBCLAUSE_LAST }}}}

#else

///// Macro that starts the switch on types implemented as a sequential cascading-if
///// without any memoization. The idea is to provide a general syntax for cases not 
///// specifically interested in speed, but expressivty: e.g. algebraic decomposition
///// with n+k patterns or structural decomposition with nested patterns.
#define MatchS(s) do {                                                         \
        XTL_MATCH_PREAMBULA(s)                                                 \
        XTL_MESSAGE("WARNING: The use of { and } within MatchS-statement must match XTL_USE_BRACES setting!") \
        {{ XTL_SUBCLAUSE_FIRST

/// Macro that defines the case statement for the above switch
#define QuaS(C,...)                                                            \
        XTL_SUBCLAUSE_CLOSE }}{                                                \
        XTL_CLAUSE_COMMON(C);                                                  \
        if (auto matched = cons<C>()(subject_ptr))                             \
        {                                                                      \
            XTL_CLAUSE_DECL_ONLY(C(*matched));                                 \
            XTL_UNUSED(matched);                                               \
            XTL_SUBCLAUSE_OPEN(__VA_ARGS__)

/// NOTE: We need this extra indirection to properly handle 0 arguments as it
///       seems to be impossible to introduce dummy argument inside the Case 
///       directly, so we use the type argument as a dummy argument for XTL_DECL_BOUND_VARS
#define CaseS_(C,...)   QuaS(C)
#define CaseS(...)      XTL_APPLY_VARIADIC_MACRO(CaseS_,(__VA_ARGS__)) XTL_APPLY_VARIADIC_MACRO(XTL_DECL_BOUND_VARS,(__VA_ARGS__))
#define WhenS(...)      XTL_SUBCLAUSE_CONTINUE(__VA_ARGS__)
#define PatternS(...)   XTL_SUBCLAUSE_PATTERN(__VA_ARGS__)
#define OtherwiseS(...) XTL_CLAUSE_OTHERWISE(CaseS,__VA_ARGS__)
#define EndMatchS       XTL_SUBCLAUSE_LAST }}} XTL_STATIC_WHILE(false);

#endif

//------------------------------------------------------------------------------

/// This is a helper macro to be able to pass template instantiations as an
/// argument of a macro. The actual problem is that template instantiations
/// may contain commas in their argument list, which are treated as macro 
/// argument separator by the preprocessor. Enclosing such type argument in 
/// () directly will not work because (Type) is a conversion expression in C
/// and compiler reports an error when we try to user (Type) instead of a Type.
/// \code 
///       Case(TypeArg(MyMap<int,string>),x,y,z) ...
/// \endcode
/// The solution used here is based on the following discussion:
/// \see http://stackoverflow.com/questions/4295890/trouble-with-template-parameters-used-in-macros
#define  TypeArg_(X)  XTL_CPP0X_TYPENAME get_first_param<void X>::type
#define  TypeArg(...) TypeArg_((__VA_ARGS__))

//------------------------------------------------------------------------------

/// Macro that starts generic switch on types capable of figuring out by itself
/// which of the 3 cases presented above we are dealing with: open, closed or union.
/// \note This macro cannot be used in a template context! If you need to have 
///       it inside a template, please use @TMatch and corresponding @TCase and 
///       @TEndMatch. Unfortunately at the moment we are unaware how to unify 
///       these 2 macros as types and templates should only be annotated inside
///       a template context and not outside.
#define MatchQ(s) {                                                            \
        XTL_MATCH_PREAMBULA(s)                                                 \
        enum { __base_counter = XTL_COUNTER };                                 \
        typedef unified_switch<source_type> switch_traits;                     \
        XTL_PRELOADABLE_LOCAL_STATIC(XTL_CPP0X_TYPENAME switch_traits::static_data_type,static_data,match_uid_type); \
        XTL_CPP0X_TYPENAME switch_traits::local_data_type  local_data;         \
        register bool processed = false;                                       \
        size_t jump_target = switch_traits::choose(subject_ptr,static_data,local_data); \
        XTL_CONCAT(ReMatch,__LINE__):                                          \
        switch (jump_target)                                                   \
        {                                                                      \
            XTL_REDUNDANCY_LABEL(default:)                                     \
            { XTL_REDUNDANCY_TRY {{                                            \
            if (switch_traits::on_default(jump_target,local_data,static_data)) \
                goto XTL_CONCAT(ReMatch,__LINE__);                             \
            XTL_SUBCLAUSE_FIRST

/// Currently we have not found the way to distinguish all 3 possibilities 
/// i.e. type, layout and declaration so we just define a different macro when
/// the declaration support is expected.
#if XTL_CLAUSE_DECL
/// Macro that defines the case statement for the above switch
#define QuaQ(C,...)                                                            \
        XTL_SUBCLAUSE_CLOSE }}                                                 \
        XTL_REDUNDANCY_CATCH(C)                                                \
        {                                                                      \
            XTL_CLAUSE_COMMON(C);                                              \
            typedef XTL_CPP0X_TYPENAME switch_traits::                         \
                    XTL_CPP0X_TEMPLATE disambiguate<0>::                       \
                    XTL_CPP0X_TEMPLATE parameter<target_type> target_specific; \
            enum { target_layout = target_specific::layout, target_label = XTL_COUNTER-__base_counter };  \
            if (XTL_UNLIKELY(target_specific::main_condition(subject_ptr, local_data)))                   \
            {                                                                                             \
                switch_traits::on_first_pass(subject_ptr, local_data, target_label);                      \
            XTL_REDUNDANCY_LABEL(case target_specific::XTL_CPP0X_TEMPLATE CaseLabel<target_label>::value:)\
                auto matched = target_specific::get_matched(subject_ptr,local_data);                      \
                XTL_CLAUSE_DECL_ONLY(C(*matched));                             \
                XTL_UNUSED(matched);                                           \
                XTL_SUBCLAUSE_OPEN(__VA_ARGS__) processed = true;
#else
/// Macro that defines the case statement for the above switch
#define QuaQ(C,...)                                                            \
        XTL_SUBCLAUSE_CLOSE }}                                                 \
        XTL_REDUNDANCY_CATCH(C)                                                \
        {                                                                      \
            typedef XTL_CPP0X_TYPENAME switch_traits::                         \
                    XTL_CPP0X_TEMPLATE disambiguate<sizeof(C)<sizeof(XTL_CPP0X_TYPENAME switch_traits::source_type)>:: \
                    XTL_CPP0X_TEMPLATE parameter<C> target_specific;           \
            typedef XTL_CPP0X_TYPENAME target_specific::target_type target_type;                          \
            enum { target_layout = target_specific::layout, target_label = XTL_COUNTER-__base_counter, is_inside_case_clause = 1 };  \
            if (XTL_UNLIKELY(target_specific::main_condition(subject_ptr, local_data)))                   \
            {                                                                                             \
                switch_traits::on_first_pass(subject_ptr, local_data, target_label);                      \
            XTL_REDUNDANCY_LABEL(case target_specific::XTL_CPP0X_TEMPLATE CaseLabel<target_label>::value:)\
                auto matched = target_specific::get_matched(subject_ptr,local_data);                      \
                XTL_UNUSED(matched);                                           \
                XTL_SUBCLAUSE_OPEN(__VA_ARGS__) processed = true;
#endif // XTL_CLAUSE_DECL

/// NOTE: We need this extra indirection to properly handle 0 arguments as it
///       seems to be impossible to introduce dummy argument inside the Case 
///       directly, so we use the type argument as a dummy argument for XTL_DECL_BOUND_VARS
#define CaseQ_(C,...)   QuaQ(C)
#define CaseQ(...)      XTL_APPLY_VARIADIC_MACRO(CaseQ_,(__VA_ARGS__)) XTL_APPLY_VARIADIC_MACRO(XTL_DECL_BOUND_VARS,(__VA_ARGS__))
#define WhenQ(...)      XTL_SUBCLAUSE_CONTINUE(__VA_ARGS__) processed = true;
#define PatternQ(...)   XTL_SUBCLAUSE_PATTERN(__VA_ARGS__)
#define OtherwiseQ(...) XTL_CLAUSE_OTHERWISE(CaseQ,__VA_ARGS__)
#define EndMatchQ       XTL_SUBCLAUSE_LAST }}}                                 \
        enum { target_label = XTL_COUNTER-__base_counter };                    \
        if (!processed) switch_traits::on_end(subject_ptr, local_data, target_label); \
        case switch_traits::XTL_CPP0X_TEMPLATE CaseLabel<target_label>::exit: ; }}

//------------------------------------------------------------------------------

#if defined(Match) || defined(Case) || defined(Qua) || defined(When) || defined(Otherwise) || defined(EndMatch)
    #error One of the macros used by the pattern-matching library has already been defined
#endif

/// The table below lists capabilities of each (sub)clause in each encoding.
///
/// ----------+------+------+------+------+------+------+------+
/// Encoding=>| P    | S0   | S1   | F    | K    | U    | E    |
/// ----------+------+------+------+------+------+------+------+
/// Otherwise | A    | A    | A    | A    | A    | A    | A    | Specialized
///           | A    | A    | A    | A    | A    | A    | A    | Unified
/// Case      | ADTR | ADTR | ADT  | ADT  | ADT  | A  N | ADT  |
///           | A TR | A TR | A T  | A TN | A TN | A  N | A T  |    ^
/// Qua       | PDTR | PDTR | PDT  | PDT  | PDT  | P  N | PDT  | Syntax 
///           | P TR | P TR | P T  | P TN | P TN | P  N | P T  |
/// With      | P    | P    | P    |      |      |      | P    |
///           | P    | P    | P    |      |      |      | P    |
/// When      | P    | P    | P    | P    | P    | P    | P    |
///           | P    | P    | P    | P    | P    | P    | P    |
/// ----------+------+------+------+------+------+------+------+
///
/// The (sub)clauses are:
///
/// * Qua(T,p1,...,pn) - most general clause that takes target type or a tag 
///   value as the first argument and generalized patterns as subsequent arguments
/// * Case(T,v1,...,vn)- auto introduces v1 ... vn as variables of corresponding 
///   types and forwards the call to Qua
/// * Otherwise(v1,...,vn) - equivalent to Case-clause with subject type used
///   as a target type. It is an irrefutable clause and should be last.
/// * With(p) - takes a single pattern as an argument. With(C<T>(p1,...,pn))
///   is essentially the same as Qua(T,p1,...,pn), but With can take other patterns        
/// * When(p1,...,pn) - this is a subclause. They are executed sequentially on the
///   matched value of the last clause (or a subject if none).
///
/// The encodings are:
///
/// * P  - polymorphic encoding combined with type switch          - implements first-fit semantics
/// * S0 - polymorphic encoding combined with sequential execution - implements first-fit semantics (dummy switch)
/// * S1 - polymorphic encoding combined with sequential execution - implements first-fit semantics (cascading if)
/// * F  - tag encoding with    forwarding to base classes         - implements  best-fit semantics
/// * K  - tag encoding without forwarding to base classes         - implements exact-fit semantics
/// * U  - tag encoding for discriminated union types              - implements exact-fit semantics
/// * E  - dispatch based on exception handling mechanism of C++   - implements first-fit semantics
///
/// The obvious question is why bother with so many encodings? Once the match 
/// statement is written, one may wonder to experiment with it whether tag or 
/// polymorphic encoding would work best for his particular hierarchy and needs.
/// This was the case for our hierarchy that had both tags and virtual functions,
/// which is why we made it transparent to support both. Later we added other 
/// encodings just for the sake of experiment since we were timing them for the
/// paper and technical report to justify our choices.
///
/// The letters indicate:
///
/// * A - support of autodeclaration of variables in binding positions
/// * D - support of declaration syntax in the first parameter
/// * T - support of type in the first parameter
/// * N - support of number (tag) in the first parameter
/// * R - support of redundancy checking
/// * P - support of generalized patterns
///
/// The first  line of each clause indicates feature support for specialized syntax.
/// The second line of each clause indicates feature support for unified     syntax.

//------------------------------------------------------------------------------

/// Now based on user's preference we can set up default syntax to one of the above
#if   XTL_DEFAULT_SYNTAX == 'P'
  /// The user chooses polymorphic match statement to be the default
  XTL_MESSAGE("Default pattern matching syntax is: P")
  #define  Match      MatchP
  #define  Case       CaseP
  #define  Qua        QuaP
  #define  When       WhenP
  #define  Pattern    PatternP
  #define  Otherwise  OtherwiseP
  #define  EndMatch   EndMatchP
#elif XTL_DEFAULT_SYNTAX == 'K'
  /// The user chooses closed kind match statement to be the default
  XTL_MESSAGE("Default pattern matching syntax is: K")
  #define  Match      MatchK
  #define  Case       CaseK
  #define  Qua        QuaK
  #define  When       WhenK
  #define  Pattern    PatternK
  #define  Otherwise  OtherwiseK
  #define  EndMatch   EndMatchK
#elif XTL_DEFAULT_SYNTAX == 'U'
  /// The user chooses discriminated union statement to be the default
  XTL_MESSAGE("Default pattern matching syntax is: U")
  #define  Match      MatchU
  #define  Case       CaseU
  #define  Qua        QuaU
  #define  When       WhenU
  #define  Pattern    PatternU
  #define  Otherwise  OtherwiseU
  #define  EndMatch   EndMatchU
#elif XTL_DEFAULT_SYNTAX == 'E'
  /// The user chooses exception match statement to be the default
  XTL_MESSAGE("Default pattern matching syntax is: E")
  #define  Match      MatchE
  #define  Case       CaseE
  #define  Qua        QuaE
  #define  When       WhenE
  #define  Pattern    PatternE
  #define  Otherwise  OtherwiseE
  #define  EndMatch   EndMatchE
#elif XTL_DEFAULT_SYNTAX == 'F'
  /// The user chooses closed kind match statement to be the default
  XTL_MESSAGE("Default pattern matching syntax is: F")
  #define  Match      MatchF
  #define  Case       CaseF
  #define  Qua        QuaF
  #define  When       WhenF
  #define  Pattern    PatternF
  #define  Otherwise  OtherwiseF
  #define  EndMatch   EndMatchF
#elif XTL_DEFAULT_SYNTAX == 'S'
  /// The user chooses sequential cascading-if match statement to be the default
  XTL_MESSAGE("Default pattern matching syntax is: S")
  #define  Match      MatchS
  #define  Case       CaseS
  #define  Qua        QuaS
  #define  When       WhenS
  #define  Pattern    PatternS
  #define  Otherwise  OtherwiseS
  #define  EndMatch   EndMatchS
#else
  /// The user chooses generic match statement to be the default
  XTL_MESSAGE("Default pattern matching syntax is: G")
  #define  Match      MatchQ
  #define  Case       CaseQ
  #define  Qua        QuaQ
  #define  When       WhenQ
  #define  Pattern    PatternQ
  #define  Otherwise  OtherwiseQ
  #define  EndMatch   EndMatchQ
#endif

//------------------------------------------------------------------------------
