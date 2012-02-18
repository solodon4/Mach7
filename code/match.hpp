///
/// \file match.hpp
///
/// This file defines the necessary infrastructure for ML-like pattern matching
/// against trees in C++.
///
/// \autor Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of the XTL framework (http://parasol.tamu.edu/xtl/).
/// Copyright (C) 2005-2012 Texas A&M University.
/// All rights reserved.
///

#pragma once

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
// done: Add static assert that verifies types in clauses to be related to selector type
//       in the same way dynamic_cast does. This cannot always be check that target type
//       is derived from selector type because we may have cross cast, but we can add
//       a configuration macro that says that hierarchy is not using multiple inheritance
//       in which case such assertion will be valid. This can be made default too.
// done: Define logic with dedicated value used by closed matching and the way user
//       can override that value when it conflicts with one of its values.
// n/a:  Add static_asserts that check that case constants are not equal to dedicate value
// done: Implement Otherwise for closed cases
// TODO: Test cases should test:
//        - template vs. non-template context
//        - presense and absense of Otherwise as well as when user disabled its use
//        - const vs. non-const selector
//        - presence and absense of forwarding to base classes
//        - usage of fall-through behavior
//        - use of implicitly declared variables in Case and explicitly declared in Que
//        - read and write of explicitly declared variables
//        - selector by reference and by pointer
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
//        - if (match<complex<float>>(re,im) == c) ...
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

#include "vtblmap.hpp"
#include "patterns.hpp"
#include "has_member.hpp"    // Meta-functions to check use of certain @match_members facilities
#include <unordered_map>
#include <vector>

//------------------------------------------------------------------------------

#if XTL_USE_MEMOIZED_CAST
  #include "memoized_cast.hpp"
  #define dynamic_cast memoized_cast
#else
  #define memoized_cast dynamic_cast
#endif

//------------------------------------------------------------------------------

#if !defined(CM)
    /// Macro to define member's position within decomposition of a given data type
    /// Example: CM(0,MyClass::member) or CM(1,external_func)
    /// \note Use this macro only inside specializations of @match_members
    /// \note We use variadic macro parameter here in order to be able to handle 
    ///       templates, which might have commas, otherwise juse a second argument
    ///       would be sufficient.
    /// \note @unary is used here as an identity to disambiguate the necessary
    ///       [member-]function when multiple exist. For example: complex<T> has
    ///       two member functions imag() - one with no arugment and one with one
    ///       argument. We are only interested with the one without argument and
    ///       putting @unary here around taking the address of it saves the user
    ///       from having to disambiguate explicitly.
    #define CM(Index,...)                                           \
        static inline decltype(unary(&__VA_ARGS__)) member##Index() noexcept \
        {                                                           \
            return unary(&__VA_ARGS__);                             \
        }
#else
    #error Macro CM used by the pattern-matching library has already been defined
#endif

#if !defined(KS)
    /// Macro to define a kind selector - a member of the common base class that 
    /// carries a distinct integral value that uniquely identifies the derived 
    /// type.  Used in the decomposition of the base class.
    /// \note Use this macro only inside specializations of @match_members
    /// \note We use variadic macro parameter here in order to be able to handle 
    ///       templates, which might have commas, otherwise juse a second argument
    ///       would be sufficient.
    /// FIX: KS doesn't accept now members qualified with base class, but CM does, check why.
    #define KS(...)                                                 \
        static inline decltype(unary(&__VA_ARGS__)) kind_selector() noexcept \
        {                                                           \
            return unary(&__VA_ARGS__);                             \
        }                                                           \
        bool kind_selector_dummy() const noexcept;
#else
    #error Macro KS used by the pattern-matching library has already been defined
#endif

#if !defined(KV)
    /// Macro to define an integral constant that uniquely identifies the derived 
    /// class. Used in the decomposition of a derived class.
    /// \note Use this macro only inside specializations of @match_members
    /// \note We use variadic macro parameter here in order to be able to handle 
    ///       templates, which might have commas, otherwise juse a second argument
    ///       would be sufficient.
    #define KV(...) enum { kind_value = __VA_ARGS__ };
#else
    #error Macro KS used by the pattern-matching library has already been defined
#endif

#if !defined(FQ)
    /// Macro to define an expected frequency of objects of a given class.
    /// The exact values are not important as they are turned into probabilities by
    /// dividing these numbers onto sum of frequencies of all classes, statically
    /// allowable by a given match statement (derived from a selector type).
    /// \note Use this macro only inside specializations of @match_members
    /// \note We use variadic macro parameter here in order to be able to handle 
    ///       templates, which might have commas, otherwise juse a second argument
    ///       would be sufficient.
    #define FQ(...) enum { fq = __VA_ARGS__ };
#else
    #error Macro FQ used by the pattern-matching library has already been defined
#endif

#if !defined(FQS)
    /// Macro to define a function that returns expected frequency of a given type.
    /// The syntax of the function should be: size_t f(const selector_type&);
    /// FIX: Make sure there is no re-entrancy problems when pattern matching is
    ///      used inside of the frequency function f to uncover selector type.
    #define FQS(f)                                                  \
        static size_t frequency(intptr_t vtbl) noexcept             \
        {                                                           \
            return ::f(*reinterpret_cast<const XTL_CPP0X_TYPENAME get_param<match_members>::type*>(&vtbl)); \
        }                                                           \
        bool frequency_dummy() const noexcept;
#else
    #error Macro FQS used by the pattern-matching library has already been defined
#endif

#if !defined(RS)
    /// Macro to define a raise selector - a virtual member function of the common
    /// base class that implements a polymorphic exception idiom (\see 
    /// http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Polymorphic_Exception). 
    /// Essentially it's a virtual member function (e.g. virtual void raise() const = 0;)
    /// that will be overriden in all subclasses in the following way:
    /// void SubClass::raise() const { throw *this; }
    /// \note Use this macro only inside specializations of @match_members
    /// \note We use variadic macro parameter here in order to be able to handle 
    ///       templates, which might have commas, otherwise juse a second argument
    ///       would be sufficient.
    #define RS(...)                                                 \
        static inline decltype(unary(&__VA_ARGS__)) raise_selector() noexcept\
        {                                                           \
            return unary(&__VA_ARGS__);                             \
        }                                                           \
        bool raise_selector_dummy() const noexcept;
#else
    #error Macro RS used by the pattern-matching library has already been defined
#endif

//------------------------------------------------------------------------------

/// By default a value of any type is not decomposable i.e. the whole value is 
/// the only value it can be decomposed into.
/// NOTE: We should not need this anymore since we provide a specialization of 
///       constr1 structure for exactly this case.
/// NOTE: We still need this at the moment as specialization of constr1 does
///       not handle different types e.g. when subject type is int but target is size_t.
template <typename type_being_matched, size_t layout>
struct match_members { CM(0,identity<type_being_matched>); };

//------------------------------------------------------------------------------

/// Helper function to access the value of the member specified with @KS macro 
/// on a given object.
template <typename T>
inline auto kind_selector(const T* p) -> XTL_RETURN
(
    apply_member(p, match_members<T>::kind_selector())
)

/// Helper function to call a function specified with @RS macro on a given object.
template <typename T>
inline auto raise_selector(const T* p) -> XTL_RETURN
(
    apply_member(p, match_members<T>::raise_selector())
)

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

/// Helper meta-function used to obtain the first argument of a single-argument
/// template instantiation.
/// \note Default template does not have implementation by design to fail on 
///       inappropriate application.
template <typename T>                         struct get_param;
/// This is the actual implementation that grabs the first parameter for any 
/// single-argument template Q instantiation.
template <class U, template<class X> class Q> struct get_param<Q<U>> { typedef U type; };
/// This specialization is for uses of @match_members that employ layouts.
template <class U, size_t L>                  struct get_param<match_members<U,L>> { typedef U type; };

//------------------------------------------------------------------------------

/// Helper function that is enabled when user provided frequency function for the
/// selector type.
template <typename T>
inline typename std::enable_if<has_member_frequency<match_members<T>>::value, size_t>::type
get_frequency_ex(intptr_t vtbl)
{
    return match_members<T>::frequency(vtbl);
}

/// Helper function that is enabled when user did not provide frequency function
/// for selector type.
template <typename T>
inline typename std::enable_if<!has_member_frequency<match_members<T>>::value, size_t>::type
get_frequency_ex(intptr_t)
{
    // FIX: Make this function warn user
    return 1;
}

/// We need this extra wrapper to be able to forward declare it from @vtblmap's header
template <typename T> inline size_t get_frequency(intptr_t vtbl) { return get_frequency_ex<T>(vtbl); }

//------------------------------------------------------------------------------

/// The type capable of holding any values of original tags in user's class 
/// hierarchy. We make it different from size_t to ensure we don't mix original 
/// and remapped tags.
enum tag_type { min_tag = size_t(0), max_tag = ~size_t(0) };

/// The type holding remapped original tags into the actual case labels.
/// We make this type distinct from tag_type to make sure we don't pass
/// original tags where remapped are expected and vice versa.
/// We need remapping in order to accomodate a distinct value, indicating 
/// the end of tag precedence list, as well as to make all the case labels
/// close to 0. Remapping mostly happens at compile time, so in very few
/// places we will have to do a run-time conversion.
enum lbl_type { min_lbl = size_t(0), max_lbl = ~size_t(0) };

namespace std 
{
    template <> struct hash<lbl_type> { size_t operator()(const lbl_type& l) const noexcept { return l; } };
};

/// Amount of extra values in the range of labels (lbl_type) we need for our purposes
const size_t reserved_extra_kinds = 1;

#if !defined(SKV)
    /// Macro to be used in global scope to set the smallest kind value N for the 
    /// class hierarchy rooted at C. When it is not used, 0 is assumed.
    #define SKV(C,N) smallest_kind_is<N> smallest_kind_value_helper(const C&);
#else
    #error Macro SKV used by the pattern-matching library has already been defined
#endif

/// Predefined structure that will be used when user did specify smallest kind with @SKV macro
template <size_t N> struct smallest_kind_is { enum { value = N }; };
/// When the user did not specify the smallest kind with @SKV macro, assume it is 0
smallest_kind_is<0> smallest_kind_value_helper(...);

/// Accessort to get the smallest kind for the hierarchy type T belongs to
template <typename T>
struct smallest_kind
{
    typedef decltype(smallest_kind_value_helper(std::declval<T>())) type;
    enum { value = type::value };
};

/// Convenience meta-function to get the original kind associated with the class T
template <typename T, size_t L> struct original                   { static const tag_type tag = tag_type(L); }; // FIX: This temporarily for test purposes assumes only union case. Split in two with enable_if
template <typename T>           struct original<T,default_layout> { static const tag_type tag = tag_type(match_members<T>::kind_value); };
/// Convenience meta-function to get the remapped kind associated with the class T
template <typename T, size_t L = default_layout> struct remapped  { static const lbl_type lbl = lbl_type(original<T,L>::tag - smallest_kind<T>::value + reserved_extra_kinds); };
/// Convenience function to convert remapped kind to original at run-time
template <typename T> inline tag_type remapped2original(lbl_type l) { return tag_type(l + smallest_kind<T>::value - reserved_extra_kinds); }
/// Convenience function to convert original kind to remapped at run-time
template <typename T> inline lbl_type original2remapped(tag_type t) { return lbl_type(t - smallest_kind<T>::value + reserved_extra_kinds); }

//------------------------------------------------------------------------------

typedef std::unordered_map<lbl_type, const lbl_type*> kind_to_kinds_map;

template <typename T>
inline kind_to_kinds_map& get_kind_to_kinds_map() noexcept 
{
    static kind_to_kinds_map k2k;
    return k2k;
}

/// Gets all the kinds of a class with static type T and dynamic type represented 
/// by kind. The first element of the returned list will always be equal to kind,
/// the last to a dedicated value and those in between to the kinds of base classes.
template <typename T>
inline const lbl_type* get_kinds(lbl_type kind) noexcept
{
    return get_kind_to_kinds_map<T>()[kind];
}

template <typename T>
inline const lbl_type* set_kinds(lbl_type kind, const lbl_type* kinds) noexcept
{
    return get_kind_to_kinds_map<T>()[kind] = kinds;
}

template <typename D, typename B>
struct associate_kinds
{
    static_assert(std::is_base_of<B,D>::value, "Not a base class");
    static const lbl_type* kinds;
};

template <typename D, typename B>
const lbl_type* associate_kinds<D,B>::kinds = set_kinds<B>(remapped<D>::lbl, match_members<D>::get_kinds());

/// Checks whether a given base_kind belongs to the tag precedence list of derived_kind
template <typename T>
inline bool is_base_and_derived_kinds(lbl_type base_kind, lbl_type derived_kind) noexcept
{
    const lbl_type* all_kinds = get_kinds<T>(derived_kind);

    if (!all_kinds)
        return base_kind == derived_kind;

    while (*all_kinds)
        if (*all_kinds++ == base_kind)
            return true;

    return false;
}

//------------------------------------------------------------------------------

/// A helper macro to access kind value of a class
#define BCK(D,B) (associate_kinds<D,B>::kinds,remapped<B>::lbl)

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
#define  BCS(...) typedef          get_param<match_members>::type D; BCS_(XTL_NARG(__VA_ARGS__),##__VA_ARGS__)
#define TBCS(...) typedef typename get_param<match_members>::type D; BCS_(XTL_NARG(__VA_ARGS__),##__VA_ARGS__)

//------------------------------------------------------------------------------

/// A macro to declare implicitly a reference variable with name V bound to 
/// a value in position P of the target type.
/// FIX: Try without const to bind also for modification
#define XTL_VAR_DECL(P,V) const auto& V = apply_member(matched, match_members<target_type,target_layout>::XTL_CONCAT(member,P)())

/// A set of macros handling various amount of arguments passed to case statement.
#define XTL_DECL_BOUND_VAR_0(Dummy)                         XTL_UNUSED(matched);
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
  /// structure, the use of {} inside @Match statement should correspond to the 
  /// @XTL_USE_BRACES library configuration flag.
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
/// - a pointer value __selector_ptr referencing the subject is introduced, 
///   regardless of whether the actual subject was passed by pointer, reference
///   or value.
/// - the actual type of the subject without qualifiers etc. is selector_type
/// - the default target_type is the selector_type (used for When sub-clauses)
/// - the default target layout is the default layout (used for When sub-clauses)
/// - matched refers the subject by default (used for When sub-clauses)
/// - the subject cannot be a nullptr - we assert at run-time (debug) if it is
#define XTL_MATCH_PREAMBULA(s)                                                 \
        struct match_uid_type {};                                              \
        auto const __selector_ptr = addr(s);                                   \
        typedef XTL_CPP0X_TYPENAME underlying<decltype(*__selector_ptr)>::type selector_type; \
        typedef selector_type target_type;                                     \
        enum { target_layout = default_layout, is_inside_case_clause = 0 };    \
        XTL_ASSERT(("Trying to match against a nullptr",__selector_ptr));      \
        auto const matched = __selector_ptr;                                   \
        XTL_UNUSED(matched);

#define XTL_SUBCLAUSE_FIRST         XTL_NON_FALL_THROUGH_ONLY(if (false)) XTL_NON_USE_BRACES_ONLY({)
#define XTL_SUBCLAUSE_OPEN(...)                                       if (UCL_PP_IF(UCL_PP_IS_EMPTY(__VA_ARGS__), true,   XTL_LIKELY(match<target_type,target_layout>(__VA_ARGS__).match_structure(matched)))) {
#define XTL_SUBCLAUSE_CONTINUE(...) } XTL_NON_FALL_THROUGH_ONLY(else) if (UCL_PP_IF(UCL_PP_IS_EMPTY(__VA_ARGS__), true, XTL_UNLIKELY(match<target_type,target_layout>(__VA_ARGS__).match_structure(matched)))) {
#define XTL_SUBCLAUSE_CLOSE         }                          XTL_NON_FALL_THROUGH_ONLY(if (is_inside_case_clause) break;)
#define XTL_SUBCLAUSE_LAST          XTL_NON_USE_BRACES_ONLY(}) XTL_NON_FALL_THROUGH_ONLY(if (is_inside_case_clause) break;)

#define XTL_CLAUSE_OTHERWISE(CaseClause,...)                                   \
        static_assert(is_inside_case_clause, "Otherwise() must follow actual clauses! If you are trying to use it as a default sub-clause, use When() instead"); \
        CaseClause(selector_type UCL_PP_IF(UCL_PP_IS_EMPTY(__VA_ARGS__), UCL_PP_EMPTY(), ,) __VA_ARGS__)

#if XTL_CLAUSES_NUM_ESTIMATES_TYPES_NUM
    #define XTL_GET_TYPES_NUM_ESTIMATE   (deferred_constant<vtbl_count_t>::get<match_uid_type>::value)
    #define XTL_SET_TYPES_NUM_ESTIMATE(N) deferred_constant<vtbl_count_t>::set<match_uid_type,(N)>::value_ptr
#else
    #define XTL_GET_TYPES_NUM_ESTIMATE   (min_expected_size)
    #define XTL_SET_TYPES_NUM_ESTIMATE(N)
#endif

//------------------------------------------------------------------------------
/// Few general rules to understand behavior of various @Match statements below:
/// - Each Case, Que and When clauses should close as many braces as they open
/// - When-clause should open less braces than Que and Case to allow sub-clauses
/// - Case and Que clauses should open the same amount of braces since they can
///   be mixed in the same statement e.g. Otherwise is resolved to Case clause.
/// - Match statement baseed on switch should introduce more open braces than
///   its Case-clauses to allow optional { } surrounding clauses.
/// - Scopes are generally as following:
///   * Match-level scope to introduce __selector_ptr, selector_type, __base_counter
///   * switch-level scope to group case labels and allow use of break statement
///   * Clause-level scope introduces target_type shared by all sub-clauses
///   * Sub-clause-level scope introduces target_label etc.
///   * Optional scope introduced by conditions for sequential execution of statement
///   * Scope of user's statement to separate from scope of matched declaration
//------------------------------------------------------------------------------

/// Macro that starts the switch on pattern
/// \note case 0: instead of default: will work in the same way because we 
///       initialize cache with 0, however through experiments we can see
///       that having default here is quite a bit faster than having case 0.
#define MatchP(s) {                                                            \
        XTL_MATCH_PREAMBULA(s)                                                 \
        enum { __base_counter = XTL_COUNTER };                                 \
        static_assert(std::is_polymorphic<selector_type>::value, "Type of selector should be polymorphic when you use MatchP");\
        XTL_PRELOADABLE_LOCAL_STATIC(vtblmap<type_switch_info&>,__vtbl2lines_map,match_uid_type,XTL_DUMP_PERFORMANCE_ONLY(__FILE__,__LINE__,)XTL_CLAUSES_NUM_ESTIMATES_TYPES_NUM);\
        register const void* __casted_ptr = 0;                                 \
        type_switch_info& __switch_info = __vtbl2lines_map.get(__selector_ptr);\
        switch (__switch_info.line)                                            \
        {                                                                      \
            XTL_REDUNDANCY_TRY                                                 \
            {                                                                  \
                {                                                              \
                    XTL_REDUNDANCY_LABEL(default:)                             \
                    XTL_SUBCLAUSE_FIRST

/// Macro that defines the case statement for the above switch
#define QueP(C,...)                                                            \
        XTL_SUBCLAUSE_CLOSE }}                                                 \
        XTL_REDUNDANCY_CATCH(C)                                                \
        {                                                                      \
            typedef C target_type;                                             \
            enum { target_label = XTL_COUNTER-__base_counter, is_inside_case_clause = 1 };     \
            if (XTL_UNLIKELY(__casted_ptr = dynamic_cast<const target_type*>(__selector_ptr))) \
            {                                                                  \
                if (XTL_LIKELY((__switch_info.line == 0)))                     \
                {                                                              \
                    __switch_info.line = target_label;                         \
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); \
                }                                                              \
            XTL_REDUNDANCY_LABEL(case target_label:)                           \
                auto matched = adjust_ptr<target_type>(__selector_ptr,__switch_info.offset);\
                XTL_UNUSED(matched);                                           \
                XTL_SUBCLAUSE_OPEN(__VA_ARGS__)

/// NOTE: We need this extra indirection to properly handle 0 arguments as it
///       seems to be impossible to introduce dummy argument inside the Case 
///       directly, so we use the type argument as a dummy argument for XTL_DECL_BOUND_VARS
#define CaseP_(C,...)   QueP(C)                                                                         
#define CaseP(...)      XTL_APPLY_VARIADIC_MACRO(CaseP_,(__VA_ARGS__)) XTL_APPLY_VARIADIC_MACRO(XTL_DECL_BOUND_VARS,(__VA_ARGS__))
#define WhenP(...)      XTL_SUBCLAUSE_CONTINUE(__VA_ARGS__) __casted_ptr = __selector_ptr;
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
        static_assert(has_member_kind_selector<match_members<selector_type>>::value, "Before using MatchK, you have to specify kind selector on the selector type using KS macro");\
        auto const __kind_selector = kind_selector(__selector_ptr);            \
        switch (__kind_selector) { { XTL_SUBCLAUSE_FIRST

/// Macro that defines the case statement for the above switch
#define QueK(C,...)                                                            \
        XTL_SUBCLAUSE_CLOSE }                                                  \
        if (XTL_UNLIKELY((__kind_selector == match_members<C>::kind_value)))   \
        {                                                                      \
        case match_members<C>::kind_value:                                     \
            typedef C target_type;                                             \
            enum { is_inside_case_clause = 1 };                                \
            auto matched = stat_cast<target_type>(__selector_ptr);             \
            XTL_UNUSED(matched);                                               \
            XTL_SUBCLAUSE_OPEN(__VA_ARGS__)

/// NOTE: We need this extra indirection to properly handle 0 arguments as it
///       seems to be impossible to introduce dummy argument inside the Case 
///       directly, so we use the type argument as a dummy argument for XTL_DECL_BOUND_VARS
#define CaseK_(C,...)   QueK(C)
#define CaseK(...)      XTL_APPLY_VARIADIC_MACRO(CaseK_,(__VA_ARGS__)) XTL_APPLY_VARIADIC_MACRO(XTL_DECL_BOUND_VARS,(__VA_ARGS__))
#define WhenK(...)      XTL_SUBCLAUSE_CONTINUE(__VA_ARGS__)
#define OtherwiseK(...)                                                        \
        } XTL_NON_FALL_THROUGH_ONLY(break;) }                                  \
        default: { XTL_APPLY_VARIADIC_MACRO(XTL_DECL_BOUND_VARS,(__VA_ARGS__)) {
#define EndMatchK       XTL_SUBCLAUSE_LAST } }}

//------------------------------------------------------------------------------

/// Macro that starts the switch on types that carry their own dynamic type as
/// a distinct integral value in one of their members.
#define MatchU(s) {                                                            \
        XTL_MATCH_PREAMBULA(s)                                                 \
        static_assert(has_member_kind_selector<match_members<selector_type>>::value, "Before using MatchU, you have to specify kind selector on the selector type using KS macro");\
        auto const __kind_selector = kind_selector(__selector_ptr);            \
        switch (__kind_selector) { { XTL_SUBCLAUSE_FIRST

/// Macro that defines the case statement for the above switch
#define QueU(L,...)                                                            \
        XTL_SUBCLAUSE_CLOSE }                                                  \
        if (XTL_UNLIKELY((__kind_selector == match_members<selector_type,L>::kind_value))) \
        {                                                                      \
        case match_members<selector_type,L>::kind_value:                       \
            typedef selector_type target_type;                                 \
            enum { target_layout = L, is_inside_case_clause = 1 };             \
            auto matched = __selector_ptr;                                     \
            XTL_UNUSED(matched);                                               \
            XTL_SUBCLAUSE_OPEN(__VA_ARGS__)

#define CaseU_(L,...)   QueU(L)
#define CaseU(...)      XTL_APPLY_VARIADIC_MACRO(CaseU_,(__VA_ARGS__)) XTL_APPLY_VARIADIC_MACRO(XTL_DECL_BOUND_VARS,(__VA_ARGS__))
#define WhenU(...)      XTL_SUBCLAUSE_CONTINUE(__VA_ARGS__)
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
        static_assert(has_member_raise_selector<match_members<selector_type>>::value, "Before using MatchE, you have to specify raise selector on the selector type using RS macro");\
        XTL_MESSAGE("WARNING: The use of { and } within MatchE-statement must match XTL_USE_BRACES setting!") \
        try { raise_selector(__selector_ptr); XTL_SUBCLAUSE_FIRST

/// Macro that defines the case statement for the above switch
#define QueE(C,...) }}                                                         \
        catch (C& __matched)                                                   \
        {                                                                      \
            typedef C target_type;                                             \
            enum { is_inside_case_clause = 1 };                                \
            auto matched = &__matched;                                         \
            XTL_UNUSED(matched);                                               \
            XTL_SUBCLAUSE_OPEN(__VA_ARGS__)

#define CaseE_(C,...)   QueE(C)
#define CaseE(...)      XTL_APPLY_VARIADIC_MACRO(CaseE_,(__VA_ARGS__)) XTL_APPLY_VARIADIC_MACRO(XTL_DECL_BOUND_VARS,(__VA_ARGS__))
#define WhenE(...)      XTL_SUBCLAUSE_CONTINUE(__VA_ARGS__)
#define OtherwiseE(...) XTL_CLAUSE_OTHERWISE(CaseE,__VA_ARGS__)
#define EndMatchE       XTL_NON_USE_BRACES_ONLY(}) } catch (...) {} }

//------------------------------------------------------------------------------

/// Macro that starts the switch on types that carry their own dynamic type as
/// a distinct integral value in one of their members.
/// Non-forwarding: Sequential:  33% faster; Random: 34% faster
///     Forwarding: Sequential: 251% faster; Random: 33% faster
#define MatchF(s) {                                                            \
        XTL_MATCH_PREAMBULA(s)                                                 \
        static_assert(has_member_kind_selector<match_members<selector_type>>::value, "Before using MatchF, you have to specify kind selector on the selector type using KS macro");\
        lbl_type __kind_selector = original2remapped<selector_type>(tag_type(kind_selector(__selector_ptr))), __most_derived_kind_selector = __kind_selector;\
        const lbl_type* __kinds = 0;                                           \
        size_t __attempt;                                                      \
        XTL_CONCAT(ReMatch,__LINE__):                                          \
        switch (__kind_selector) {                                             \
        default:                                                               \
            if (XTL_LIKELY(!__kinds))                                          \
            {                                                                  \
                __attempt = 0;                                                 \
                XTL_PRELOADABLE_LOCAL_STATIC(std::vector<const lbl_type*>,__kinds_cache,match_uid_type); \
                if (XTL_UNLIKELY(__kind_selector >= __kinds_cache.size()))     \
                    __kinds_cache.resize(__kind_selector+1);                   \
                if (!(__kinds = __kinds_cache[__kind_selector]))               \
                    __kinds = __kinds_cache[__kind_selector] = get_kinds<selector_type>(__kind_selector);\
            }                                                                  \
            XTL_ASSERT(("Base classes for this kind were not specified",__kinds));    \
            XTL_ASSERT(("Invalid list of kinds",__kinds[__attempt]==__kind_selector));\
            __kind_selector = __kinds ? __kinds[++__attempt] : lbl_type(0);    \
            goto XTL_CONCAT(ReMatch,__LINE__);                                 \
        case 0: break; { XTL_SUBCLAUSE_FIRST

/// Macro that defines the case statement for the above switch
#define QueF(C,...)                                                            \
        XTL_SUBCLAUSE_CLOSE }                                                  \
        if (XTL_UNLIKELY(is_base_and_derived_kinds<selector_type>(remapped<C>::lbl, __most_derived_kind_selector))) \
        {                                                                      \
        case remapped<C>::lbl:                                                 \
            typedef C target_type;                                             \
            enum { is_inside_case_clause = 1 };                                \
            auto matched = stat_cast<target_type>(__selector_ptr);             \
            XTL_UNUSED(matched);                                               \
            XTL_SUBCLAUSE_OPEN(__VA_ARGS__)

/// NOTE: We need this extra indirection to properly handle 0 arguments as it
///       seems to be impossible to introduce dummy argument inside the Case 
///       directly, so we use the type argument as a dummy argument for XTL_DECL_BOUND_VARS
#define CaseF_(C,...)   QueF(C)
#define CaseF(...)      XTL_APPLY_VARIADIC_MACRO(CaseF_,(__VA_ARGS__)) XTL_APPLY_VARIADIC_MACRO(XTL_DECL_BOUND_VARS,(__VA_ARGS__))
#define WhenF(...)      XTL_SUBCLAUSE_CONTINUE(__VA_ARGS__)
#define OtherwiseF(...) XTL_CLAUSE_OTHERWISE(CaseF,__VA_ARGS__)
#define EndMatchF       XTL_SUBCLAUSE_LAST } }}

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
#define QueS(C,...)                                                            \
        XTL_SUBCLAUSE_CLOSE }}                                                 \
        XTL_REDUNDANCY_CATCH(C)                                                \
        { /* Clause */                                                         \
            typedef C target_type;                                             \
            enum { is_inside_case_clause = 1 };                                \
            XTL_REDUNDANCY_LABEL(case XTL_COUNTER-__base_counter:)             \
            if (auto matched = match<C>()(__selector_ptr))                     \
            { /* Sequential */                                                 \
                XTL_UNUSED(matched);                                           \
                XTL_SUBCLAUSE_OPEN(__VA_ARGS__)

/// NOTE: We need this extra indirection to properly handle 0 arguments as it
///       seems to be impossible to introduce dummy argument inside the Case 
///       directly, so we use the type argument as a dummy argument for XTL_DECL_BOUND_VARS
#define CaseS_(C,...)   QueS(C)
#define CaseS(...)      XTL_APPLY_VARIADIC_MACRO(CaseS_,(__VA_ARGS__)) XTL_APPLY_VARIADIC_MACRO(XTL_DECL_BOUND_VARS,(__VA_ARGS__))
#define WhenS(...)      XTL_SUBCLAUSE_CONTINUE(__VA_ARGS__)
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
#define QueS(C,...)                                                            \
        XTL_SUBCLAUSE_CLOSE }}{                                                \
        typedef C target_type;                                                 \
        enum { is_inside_case_clause = 1 };                                    \
        if (auto matched = match<C>()(__selector_ptr))                         \
        {                                                                      \
            XTL_UNUSED(matched);                                               \
            XTL_SUBCLAUSE_OPEN(__VA_ARGS__)

/// NOTE: We need this extra indirection to properly handle 0 arguments as it
///       seems to be impossible to introduce dummy argument inside the Case 
///       directly, so we use the type argument as a dummy argument for XTL_DECL_BOUND_VARS
#define CaseS_(C,...)   QueS(C)
#define CaseS(...)      XTL_APPLY_VARIADIC_MACRO(CaseS_,(__VA_ARGS__)) XTL_APPLY_VARIADIC_MACRO(XTL_DECL_BOUND_VARS,(__VA_ARGS__))
#define WhenS(...)      XTL_SUBCLAUSE_CONTINUE(__VA_ARGS__)
#define OtherwiseS(...) XTL_CLAUSE_OTHERWISE(CaseS,__VA_ARGS__)
#define EndMatchS       XTL_SUBCLAUSE_LAST }}} while (false);

#endif

//------------------------------------------------------------------------------

template<typename T>              struct get_first_param;
template<typename R, typename P1> struct get_first_param<R(P1)> { typedef P1 type; };

/// This is a helper macro to be able to pass template instantiations as an
/// argument of a macro. The actual problem is that template instantiations
/// may contain commas in their argument list, which are treated as macro 
/// argument separator by the preprocessor. Enclosing such type argument in 
/// () directly will not work because (Type) is a conversion expression in C
/// and compiler reports an error when we try to user (Type) instead of a Type.
/// \example 
///       Case(TypeArg(MyMap<int,string>),x,y,z) ...
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
        typedef unified_switch<selector_type> switch_traits;                   \
        XTL_PRELOADABLE_LOCAL_STATIC(XTL_CPP0X_TYPENAME switch_traits::static_data_type,static_data,match_uid_type); \
        XTL_CPP0X_TYPENAME switch_traits::local_data_type  local_data;  \
        register bool processed = false;                                       \
        size_t jump_target = switch_traits::choose(__selector_ptr,static_data,local_data); \
        XTL_CONCAT(ReMatch,__LINE__):                                          \
        switch (jump_target)                                                   \
        {                                                                      \
            XTL_REDUNDANCY_LABEL(default:)                                     \
            { XTL_REDUNDANCY_TRY {{                                            \
            if (switch_traits::on_default(jump_target,local_data,static_data)) \
                goto XTL_CONCAT(ReMatch,__LINE__);                             \
            XTL_SUBCLAUSE_FIRST

/// Macro that defines the case statement for the above switch
#define QueQ(C,...)                                                            \
        XTL_SUBCLAUSE_CLOSE }}                                                 \
        XTL_REDUNDANCY_CATCH(C)                                                \
        {                                                                      \
            typedef XTL_CPP0X_TYPENAME switch_traits::                         \
                    XTL_CPP0X_TEMPLATE disambiguate<sizeof(C)<sizeof(XTL_CPP0X_TYPENAME switch_traits::selector_type)>:: \
                    XTL_CPP0X_TEMPLATE parameter<C> target_specific;           \
            typedef XTL_CPP0X_TYPENAME target_specific::target_type target_type;                          \
            enum { target_layout = target_specific::layout, target_label = XTL_COUNTER-__base_counter, is_inside_case_clause = 1 };  \
            if (XTL_UNLIKELY(target_specific::main_condition(__selector_ptr, local_data)))                \
            {                                                                                             \
                switch_traits::on_first_pass(__selector_ptr, local_data, target_label);                   \
            XTL_REDUNDANCY_LABEL(case target_specific::XTL_CPP0X_TEMPLATE CaseLabel<target_label>::value:)\
                auto matched = target_specific::get_matched(__selector_ptr,local_data);                   \
                XTL_UNUSED(matched);                                           \
                XTL_SUBCLAUSE_OPEN(__VA_ARGS__) processed = true;

/// NOTE: We need this extra indirection to properly handle 0 arguments as it
///       seems to be impossible to introduce dummy argument inside the Case 
///       directly, so we use the type argument as a dummy argument for XTL_DECL_BOUND_VARS
#define CaseQ_(C,...)   QueQ(C)
#define CaseQ(...)      XTL_APPLY_VARIADIC_MACRO(CaseQ_,(__VA_ARGS__)) XTL_APPLY_VARIADIC_MACRO(XTL_DECL_BOUND_VARS,(__VA_ARGS__))
#define WhenQ(...)      XTL_SUBCLAUSE_CONTINUE(__VA_ARGS__) processed = true;
#define OtherwiseQ(...) XTL_CLAUSE_OTHERWISE(CaseQ,__VA_ARGS__)
#define EndMatchQ       XTL_SUBCLAUSE_LAST }}}                                 \
        enum { target_label = XTL_COUNTER-__base_counter };                    \
        if (!processed) switch_traits::on_end(__selector_ptr, local_data, target_label); \
        case switch_traits::XTL_CPP0X_TEMPLATE CaseLabel<target_label>::exit: ; }}

//------------------------------------------------------------------------------

#if defined(Match) || defined(Case) || defined(Que) || defined(When) || defined(Otherwise) || defined(EndMatch)
    #error One of the macros used by the pattern-matching library has already been defined
#endif

//------------------------------------------------------------------------------

/// Now based on user's preference we can set up default syntax to one of the above
#if   XTL_DEFAULT_SYNTAX == 'P'
  /// The user chooses polymorphic match statement to be the default
  XTL_MESSAGE("Default pattern matching syntax is: P")
  #define  Match      MatchP
  #define  Case       CaseP
  #define  Que        QueP
  #define  When       WhenP
  #define  Otherwise  OtherwiseP
  #define  EndMatch   EndMatchP
#elif XTL_DEFAULT_SYNTAX == 'K'
  /// The user chooses closed kind match statement to be the default
  XTL_MESSAGE("Default pattern matching syntax is: K")
  #define  Match      MatchK
  #define  Case       CaseK
  #define  Que        QueK
  #define  When       WhenK
  #define  Otherwise  OtherwiseK
  #define  EndMatch   EndMatchK
#elif XTL_DEFAULT_SYNTAX == 'U'
  /// The user chooses discriminated union statement to be the default
  XTL_MESSAGE("Default pattern matching syntax is: U")
  #define  Match      MatchU
  #define  Case       CaseU
  #define  Que        QueU
  #define  When       WhenU
  #define  Otherwise  OtherwiseU
  #define  EndMatch   EndMatchU
#elif XTL_DEFAULT_SYNTAX == 'E'
  /// The user chooses exception match statement to be the default
  XTL_MESSAGE("Default pattern matching syntax is: E")
  #define  Match      MatchE
  #define  Case       CaseE
  #define  Que        QueE
  #define  When       WhenE
  #define  Otherwise  OtherwiseE
  #define  EndMatch   EndMatchE
#elif XTL_DEFAULT_SYNTAX == 'F'
  /// The user chooses closed kind match statement to be the default
  XTL_MESSAGE("Default pattern matching syntax is: F")
  #define  Match      MatchF
  #define  Case       CaseF
  #define  Que        QueF
  #define  When       WhenF
  #define  Otherwise  OtherwiseF
  #define  EndMatch   EndMatchF
#elif XTL_DEFAULT_SYNTAX == 'S'
  /// The user chooses sequential cascading-if match statement to be the default
  XTL_MESSAGE("Default pattern matching syntax is: S")
  #define  Match      MatchS
  #define  Case       CaseS
  #define  Que        QueS
  #define  When       WhenS
  #define  Otherwise  OtherwiseS
  #define  EndMatch   EndMatchS
#else
  /// The user chooses generic match statement to be the default
  XTL_MESSAGE("Default pattern matching syntax is: G")
  #define  Match      MatchQ
  #define  Case       CaseQ
  #define  Que        QueQ
  #define  When       WhenQ
  #define  Otherwise  OtherwiseQ
  #define  EndMatch   EndMatchQ
#endif

//------------------------------------------------------------------------------

/// A traits-like class used by pattern matching library to unify the syntax of
/// open and close cases. This is different from defining the XTL_DEFAULT_SYNTAX, 
/// which will make the choice global for every class hierarchy and Match 
/// statement used by the program. With the help of this class, the library will
/// be able to figure out on its own whether we are dealing with open, closed or
/// discriminated union case. The price of such generality is a slight performance
/// overhead that appears because of the necessity of merging the syntactic 
/// structures used by open and closed cases.
/// \note This is the general case that assumes no knowledge and generates simple 
///       cascading-if statement sequentially testing each clause.
template <typename SelectorType, class condition = void>
class unified_switch
{
public:

    /// Type of the argument on which extended switch is done
    typedef typename underlying<SelectorType>::type selector_type;

    /// Type of data that has to be statically allocated inside the block 
    /// containg extended switch
    struct static_data_type {};

    /// Type of data that has to be automatically allocated inside the block 
    /// containg extended switch
    struct local_data_type {};

    /// Meta function that defines some case labels required to support extended switch.
    /// The main difference of this function from the one used on case clauses is that 
    /// this one is used on the level of match statement to define the values of common
    /// entry and exit cases.
    template <size_t Counter>
    struct CaseLabel
    {
        enum
        {
            //entry = 0,      ///< Case label that will be used to enter beginning of the switch
            exit  = Counter ///< Case label that will be used to jump to the end of the switch
        };
    };

    /// Function used to get the value we'll be switching on
    static inline size_t choose(const selector_type*, static_data_type&, local_data_type&) noexcept
    {
        return 0;
    }

    /// Function that will be called upon first entry to the case through the fall-through behavior
    static inline void on_first_pass(const selector_type*, local_data_type&, size_t) noexcept {}
    
    /// Function that will be called when the fall-through behavior reached end of the switch
    static inline void on_end(const selector_type*, local_data_type&, size_t) noexcept {}

    /// Function that will be called on default clause. It should return true 
    /// when unconditional jump to ReMatch label should be performed.
    static inline bool on_default(size_t&, local_data_type&, static_data_type&) noexcept { return false; }

    /// Structure used to disambiguate whether first argument is a type or a value
    /// \note Not used for the general case, so we don't specialize it based on argument,
    ///       which will always be type.
    template <bool FirstParamIsValue>
    struct disambiguate
    {
        /// Essentially a catcher of the first argument of the case clause
        /// a type in this case.
        template <typename T>
        struct parameter
        {
            /// The type passed as a first argument of the case clause is the target type.
            typedef typename target_of<T>::type target_type;

            /// Layout that has to be used for the given target type.
            enum { layout = target_of<T>::layout };

            /// Depending on whether we handle open or closed case, different case labels
            /// are used for the generated match statement. This metafunction takes
            /// a unique (per match statement) counter and returns the actual label that
            /// will be used for the case clause.
            template <size_t Counter>
            struct CaseLabel
            {
                enum 
                {
                    value = Counter ///< Case label that will be used for case at line offset Counter
                };
            };

            /// Condition that guards applicability of the given case clause
            /// during the fall-through behavior.
            static inline bool main_condition(const selector_type* selector_ptr, local_data_type& local_data) noexcept
            {
                return true;
            }

            /// Performs the necessary conversion of the original selector into the proper
            /// object of target type.
            /// \note The selector is const-qualified, thus the target is also const-qualified
            static inline const target_type* get_matched(const selector_type* selector_ptr, local_data_type& local_data) noexcept
            {
                return static_cast<const target_type*>(selector_ptr);
            }

            /// Performs the necessary conversion of the original selector into the proper
            /// object of target type.
            /// \note The selector is non-const, thus the target is also non-const
            static inline       target_type* get_matched(      selector_type* selector_ptr, local_data_type& local_data) noexcept
            {
                return static_cast<      target_type*>(selector_ptr);
            }
        };
    };
};

/// A traits-like class used by pattern matching library to unify the syntax of
/// open and close cases. This is different from defining the XTL_DEFAULT_SYNTAX, 
/// which will make the choice global for every class hierarchy and Match 
/// statement used by the program. With the help of this class, the library will
/// be able to figure out on its own whether we are dealing with open, closed or
/// discriminated union case. The price of such generality is a slight performance
/// overhead that appears because of the necessity of merging the syntactic 
/// structures used by open and closed cases.
/// \note This is a specialization for polymorphic case
template <typename SelectorType>
class unified_switch<
    SelectorType, 
    typename std::enable_if<
                std::is_polymorphic<typename underlying<SelectorType>::type>::value &&
               !has_member_kind_selector<match_members<typename underlying<SelectorType>::type>>::value,
                void
             >::type
>
{
public:

    /// Type of the argument on which extended switch is done
    typedef typename underlying<SelectorType>::type selector_type;

    /// Type of data that has to be statically allocated inside the block 
    /// containg extended switch
    typedef vtblmap<type_switch_info&> static_data_type;

    /// Type of data that has to be automatically allocated inside the block 
    /// containg extended switch
    struct local_data_type
    {
        const void*       casted_ptr;
        type_switch_info* switch_info_ptr;
    };

    /// Meta function that defines some case labels required to support extended switch.
    /// The main difference of this function from the one used on case clauses is that 
    /// this one is used on the level of match statement to define the values of common
    /// entry and exit cases.
    template <size_t Counter>
    struct CaseLabel
    {
        enum
        {
            //entry = 0,      ///< Case label that will be used to enter beginning of the switch
            exit  = Counter ///< Case label that will be used to jump to the end of the switch
        };
    };

    /// Function used to get the value we'll be switching on
    static inline size_t choose(const selector_type* selector_ptr, static_data_type& static_data, local_data_type& local_data) noexcept
    {
        local_data.switch_info_ptr = &static_data.get(selector_ptr);
        return local_data.switch_info_ptr->line;
    }

    /// Function that will be called upon first entry to the case through the fall-through behavior
    static inline void on_first_pass(const selector_type* selector_ptr, local_data_type& local_data, size_t line) noexcept
    {
        if (XTL_LIKELY(local_data.switch_info_ptr->line == 0)) 
        {
            local_data.switch_info_ptr->line   = line; 
            local_data.switch_info_ptr->offset = intptr_t(local_data.casted_ptr)-intptr_t(selector_ptr);
        } 
    }
    
    /// Function that will be called when the fall-through behavior reached end of the switch
    static inline void on_end(const selector_type* selector_ptr, local_data_type& local_data, size_t line) noexcept
    {
        XTL_ASSERT(local_data.switch_info_ptr->line == 0);
        //if (XTL_LIKELY(local_data.switch_info_ptr->line == 0)) 
        { 
            local_data.switch_info_ptr->line = line;
        }
    }

    /// Function that will be called on default clause. It should return true 
    /// when unconditional jump to ReMatch label should be performed.
    static inline bool on_default(size_t&, local_data_type&, static_data_type&) noexcept { return false; }

    /// Structure used to disambiguate whether first argument is a type or a value
    /// \note Not used for the open case, so we don't specialize it based on argument,
    ///       which will always be type.
    template <bool FirstParamIsValue>
    struct disambiguate
    {
        static_assert(!FirstParamIsValue, "Open case does not allow values as first argument. Did you forget to use KS macro?");

        /// Essentially a catcher of the first argument of the case clause
        /// a type in this case.
        template <typename T>
        struct parameter
        {
            /// The type passed as a first argument of the case clause is the target type.
            typedef typename target_of<T>::type target_type;

            /// Layout that has to be used for the given target type.
            enum { layout = target_of<T>::layout };

            /// Depending on whether we handle open or closed case, different case labels
            /// are used for the generated match statement. This metafunction takes
            /// a unique (per match statement) counter and returns the actual label that
            /// will be used for the case clause.
            template <size_t Counter>
            struct CaseLabel
            {
                enum 
                {
                    value = Counter ///< Case label that will be used for case at line offset Counter
                };
            };

            /// Condition that guards applicability of the given case clause
            /// during the fall-through behavior.
            static inline bool main_condition(const selector_type* selector_ptr, local_data_type& local_data) noexcept
            {
                return (local_data.casted_ptr = dynamic_cast<const target_type*>(selector_ptr)) != 0;
            }

            /// Performs the necessary conversion of the original selector into the proper
            /// object of target type.
            /// \note The selector is const-qualified, thus the target is also const-qualified
            static inline const target_type* get_matched(const selector_type* selector_ptr, local_data_type& local_data) noexcept
            {
                return adjust_ptr<target_type>(selector_ptr,local_data.switch_info_ptr->offset);
            }

            /// Performs the necessary conversion of the original selector into the proper
            /// object of target type.
            /// \note The selector is non-const, thus the target is also non-const
            static inline       target_type* get_matched(      selector_type* selector_ptr, local_data_type& local_data) noexcept
            {
                return adjust_ptr<target_type>(selector_ptr,local_data.switch_info_ptr->offset);
            }
        };
    };
};

//------------------------------------------------------------------------------

/// A traits-like class used by pattern matching library to unify the syntax of
/// open and close cases. This is different from defining the XTL_DEFAULT_SYNTAX, 
/// which will make the choice global for every class hierarchy and Match 
/// statement used by the program. With the help of this class, the library will
/// be able to figure out on its own whether we are dealing with open, closed or
/// discriminated union case. The price of such generality is a slight performance
/// overhead that appears because of the necessity of merging the syntactic 
/// structures used by open and closed cases.
/// \note This is a specialization for the closed case. It becomes enabled when
///       user used KS macro inside @match_members to define which member will
///       be used as kind selector.
template <typename SelectorType>
class unified_switch<
    SelectorType, 
    typename std::enable_if<
                has_member_kind_selector<match_members<typename underlying<SelectorType>::type>>::value,
                void
             >::type
>
{
public:

    /// Type of the argument on which extended switch is done
    typedef typename underlying<SelectorType>::type selector_type;

    /// Type of data that has to be statically allocated inside the block 
    /// containg extended switch
    typedef std::vector<const lbl_type*> static_data_type;

    /// Type of data that has to be automatically allocated inside the block 
    /// containg extended switch
    struct local_data_type
    {
        local_data_type() : kinds(0)/*, attempt(0)*/ {} // NOTE: attempt is not intialized on purpose for performance reasons. It will be initialized before use in on_default.
        const lbl_type* kinds;
        size_t          attempt;
    };

    enum { match_exit_label = 0 }; // A label value reserved for exiting base class tag iteration loop

    /// Meta function that defines some case labels required to support extended switch.
    /// The main difference of this function from the one used on case clauses is that 
    /// this one is used on the level of match statement to define the values of common
    /// entry and exit cases.
    template <size_t Counter>
    struct CaseLabel
    {
        // We effectively prepend two new case labels to the user's range of 
        // labels to assure that all the labels are close to each other.
        // From our experiments we saw that putting just these two labels 
        // elsewhere was often triggering both gcc and msvc to generate a
        // binary search and then jump table based on subranges, which was
        // killing the performance.
        enum
        {
            exit = match_exit_label ///< Case label that will be used to jump to the end of the switch
        };
    };

    /// Function used to get the value we'll be switching on
    static inline size_t choose(const selector_type* selector_ptr, static_data_type&, local_data_type&)
    {
        return original2remapped<selector_type>(tag_type(kind_selector(selector_ptr)));
    }

    /// Function that will be called upon first entry to the case through the fall-through behavior
    static inline void on_first_pass(const selector_type*, local_data_type&, size_t) noexcept {}

    /// Function that will be called when the fall-through behavior reached end of the switch
    static inline void on_end(const selector_type*, local_data_type&, size_t) noexcept {}

    /// Function that will be called on default clause. It should return true 
    /// when unconditional jump to ReMatch label should be performed.
    static inline bool on_default(size_t& jump_target, local_data_type& local_data, static_data_type& static_data) noexcept
    {
        if (XTL_LIKELY(!local_data.kinds))
        {
            local_data.attempt = 0;
            if (XTL_UNLIKELY(jump_target >= static_data.size()))
                static_data.resize(jump_target+1);
            if (!(local_data.kinds = static_data[jump_target]))
                local_data.kinds = static_data[jump_target] = get_kinds<selector_type>(lbl_type(jump_target));
        }
        XTL_ASSERT(("Base classes for this kind were not specified",local_data.kinds));
        XTL_ASSERT(("Invalid list of kinds",local_data.kinds[local_data.attempt]==jump_target));
        jump_target = local_data.kinds ? local_data.kinds[++local_data.attempt] : lbl_type(match_exit_label);
        return true;
    }
    /// Structure used to disambiguate whether first argument is a type or a value.
    /// \note This generic one handles a value, which can be seen from the type of 
    ///       parameter struct.
    // C++ standard (14.7.3.2) would not allow us to explicitly specialize 
    // disambiguate later here, but will accept a partial specialization so we
    // add a bogus template parameter to turn explicit specialization into 
    // partial.
    template <bool FirstParamIsValue, typename bogus_parameter = void>
    struct disambiguate
    {
        /// Essentially a catcher of the first argument of the case clause
        /// a value in this case.
        template <size_t N>
        struct parameter
        {
            /// Since value as a first argument of the case clause is only
            /// allowed by us on discriminated unions, the target type is equal
            /// to the selector type.
            typedef typename target_of<selector_type>::type target_type;

            /// Layout that has to be used for the given target type.
            enum { layout = N };

            /// Depending on whether we handle open or closed case, different case labels
            /// are used for the generated match statement. This metafunction takes
            /// a unique (per match statement) counter and returns the actual label that
            /// will be used for the case clause.
            template <size_t Counter>
            struct CaseLabel
            {
                enum 
                {
                    value = remapped<target_type,N>::lbl ///< Case label that will be used for case at line offset Counter
                };
            };
            
            /// Condition that guards applicability of the given case clause
            /// during the fall-through behavior.
            static inline bool main_condition(const selector_type* selector_ptr, local_data_type&) 
            {
                lbl_type derived_kind = original2remapped<selector_type>(tag_type(kind_selector(selector_ptr)));
                return is_base_and_derived_kinds<selector_type>(remapped<target_type,layout>::lbl, derived_kind); 
            }

            /// Performs the necessary conversion of the original selector into the proper
            /// object of target type.
            /// \note The selector is const-qualified, thus the target is also const-qualified
            static inline const target_type* get_matched(const selector_type* selector_ptr, local_data_type& local_data) noexcept
            {
                return selector_ptr;
            }

            /// Performs the necessary conversion of the original selector into the proper
            /// object of target type.
            /// \note The selector is non-const, thus the target is also non-const
            static inline       target_type* get_matched(      selector_type* selector_ptr, local_data_type& local_data) noexcept
            {
                return selector_ptr;
            }
        };
    };

    /// Structure used to disambiguate whether first argument is a type or a value.
    /// \note This specialization handles a type, which can be seen from the type of 
    ///       parameter struct.
    template <typename bogus_parameter>
    struct disambiguate<false,bogus_parameter>
    {
        /// Essentially a catcher of the first argument of the case clause
        /// a type in this case.
        template <typename T>
        struct parameter
        {
            /// The type passed as a first argument of the case clause is the target type.
            typedef typename target_of<T>::type target_type;

            /// Layout that has to be used for the given target type.
            enum { layout = target_of<T>::layout };

            /// Depending on whether we handle open or closed case, different case labels
            /// are used for the generated match statement. This metafunction takes
            /// a unique (per match statement) counter and returns the actual label that
            /// will be used for the case clause.
            template <size_t Counter>
            struct CaseLabel
            {
                enum 
                {
                    value = remapped<target_type>::lbl ///< Case label that will be used for case at line offset Counter
                };
            };

            /// Condition that guards applicability of the given case clause
            /// during the fall-through behavior.
            static inline bool main_condition(const selector_type* selector_ptr, local_data_type& local_data) 
            { 
                lbl_type derived_kind = original2remapped<selector_type>(tag_type(kind_selector(selector_ptr)));
                return is_base_and_derived_kinds<selector_type>(remapped<target_type,layout>::lbl, derived_kind); 
            }

            /// Performs the necessary conversion of the original selector into the proper
            /// object of target type.
            /// \note The selector is const-qualified, thus the target is also const-qualified
            static inline const target_type* get_matched(const selector_type* selector_ptr, local_data_type& local_data) noexcept
            {
                return stat_cast<target_type>(selector_ptr);
            }

            /// Performs the necessary conversion of the original selector into the proper
            /// object of target type.
            /// \note The selector is non-const, thus the target is also non-const
            static inline       target_type* get_matched(      selector_type* selector_ptr, local_data_type& local_data) noexcept
            {
                return stat_cast<target_type>(selector_ptr);
            }
        };
    };
};

//------------------------------------------------------------------------------
