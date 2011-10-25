///
/// \file match.hpp
///
/// This file defines the necessary infrastructure for ML-like pattern matching
/// against trees in C++.
///
/// \autor Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of the XTL framework (http://parasol.tamu.edu/xtl/).
/// Copyright (C) 2005-2011 Texas A&M University.
/// All rights reserved.
///

//  FIX: Big question: should null pointers match pointer variables???
//  FIX: Replace all MatchX_N versions of the macro with variadic macro 
//       taking either 1 or 2 arguments
// TODO: Make non-null version of matchers to call from match statement and avoid checks
// TODO: Make a test case that tests all features in template context
// TODO: Provide implementation when variadic templates are supported
//  FIX: Remove const currently added on auto-declared references to allow modification
// TODO: Preallocate cache for number of case clauses
// TODO: Add probabilities on classes to be taken into account for collisions
// TODO: Add support for multiple scrutinies
// TODO: Add static assert that verifies types in clauses to be related to selector type
//       in the same way dynamic_cast does.
// FIX:  Define logic with dedicated value used by closed matching and the way user
//       can override that value when it conflicts with one of its values.
// TODO: Add static_asserts that check that case constants are not equal to dedicate value
// TODO: Implement Otherwise for closed cases
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

#pragma once

#include "vtblmap.hpp"   // Class implementing fast mapping of v-table pointers to T

#include <cassert>
#include <ostream>
#include <type_traits>
#include <utility>
#include <vector>
#ifdef _DEBUG
#include <typeinfo>
#include <iostream>
#endif
#include "exprtmpl.hpp"
#include "ptrtools.hpp"  // Helper functions to work with pointers
#include "has_member.hpp"// Meta-functions to check use of certain @match_members facilities

//------------------------------------------------------------------------------

#if XTL_USE_MEMOIZED_CAST
  #include "memoized_cast.hpp"
  #define dynamic_cast memoized_cast
#else
  #define memoized_cast dynamic_cast
#endif

#ifdef _MSC_VER
/// Visual C++ 2010 does not include declval
namespace std { template <typename T> typename std::add_rvalue_reference<T>::type declval(); }
#endif

//------------------------------------------------------------------------------

/// Helper meta-function to remove reference ans const qualification from a type.
/// Used to get an underlying selector type of Match' argument.
template <typename T> 
struct underlying : std::remove_const<typename std::remove_reference<T>::type> {};

//------------------------------------------------------------------------------

/// A class representing a set of locations of type T, indexed by a compile-time
/// value N. The class is used to implicitly introduce global variables in block
/// scopes, whose initializer will only be known later in the lexical scope. 
/// Initialization of such variables will happen before main() and thus accesses
/// to these locations from within any function that was called after main can
/// rely on the value of initializer to be known.
template <typename T>
struct deferred_value
{
    /// Accessor to location N of type T
    template <size_t N>      static T& get() { static T location; return location; }
    /// A way to associate initializer with location.
    /// \note This may happen later in the lexical scope with the net effect 
    ///       that the result of the association will be available in an earlier
    ///       access to the location through get() without having to execute any
    ///       code at the program point where association has been made.
    template <size_t N, T V> struct set { static T* value; };
};

/// The trick that makes the above possible: instantiation of set will introduce
/// another static variable, whose dynamic initialization (before main) will 
/// force the initializer to be properly set.
template <class T> template <size_t N, T V> T* deferred_value<T>::set<N,V>::value = &(deferred_value<T>::get<N>() = V);

//------------------------------------------------------------------------------

/// Helper function to help disambiguate a unary version of a given function when 
/// overloads with different arity are available.
/// All of the members we work with so far through @match_members are unary:
/// they are either unary function, nullary member function (implicit argument 
/// this makes them unary effectively) or a data member (which can be treated
/// in the same way as nullary member function).
template<typename R, typename A1> R (    * unary(R (    *f)(A1)      ))(A1)     { return f; }
template<typename R, typename A1> R (A1::* unary(R  A1::*f           ))         { return f; }
template<typename R, typename A1> R (A1::* unary(R (A1::*f)(  )      ))()       { return f; }
template<typename R, typename A1> R (A1::* unary(R (A1::*f)(  ) const))() const { return f; }

//------------------------------------------------------------------------------

enum { default_layout = size_t(~0) };

/// Traits like structure that will define which members should be matched at 
/// which positions. It is intentionally left undefined as user will have to
/// provide specializations for his hierarchy.
template <typename type_being_matched, size_t layout = default_layout> 
struct match_members;

//------------------------------------------------------------------------------

/// A wrapper class that lets one pack a given layout and type into a view.
template <typename type_being_matched, size_t layout = default_layout> 
struct view
{
    typedef type_being_matched target_type;
    static const size_t        target_layout = layout;
};

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

/// We need this extra wrapper to be able to forward declare it from @vtblmap_of's header
template <typename T> inline size_t get_frequency(intptr_t vtbl) { return get_frequency_ex<T>(vtbl); }

//------------------------------------------------------------------------------

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
#define CM(Index,...) static inline decltype(unary(&__VA_ARGS__)) member##Index() { return unary(&__VA_ARGS__); }

/// Macro to define a kind selector - a member of the common base class that 
/// carries a distinct integral value that uniquely identifies the derived 
/// type.  Used in the decomposition of the base class.
/// \note Use this macro only inside specializations of @match_members
/// \note We use variadic macro parameter here in order to be able to handle 
///       templates, which might have commas, otherwise juse a second argument
///       would be sufficient.
/// FIX: KS doesn't accept now members qualified with base class, but CM does, check why.
#define KS(...)       static inline decltype(unary(&__VA_ARGS__)) kind_selector() { return unary(&__VA_ARGS__); } bool kind_selector_dummy() const;

/// Macro to define an integral constant that uniquely identifies the derived 
/// class. Used in the decomposition of a derived class.
/// \note Use this macro only inside specializations of @match_members
/// \note We use variadic macro parameter here in order to be able to handle 
///       templates, which might have commas, otherwise juse a second argument
///       would be sufficient.
#define KV(...) enum { kind_value = __VA_ARGS__ };//     static const size_t kind_value = __VA_ARGS__;

/// Macro to define an expected frequency of objects of a given class.
/// The exact values are not important as they are turned into probabilities by
/// dividing these numbers onto sum of frequencies of all classes, statically
/// allowable by a given match statement (derived from a selector type).
/// \note Use this macro only inside specializations of @match_members
/// \note We use variadic macro parameter here in order to be able to handle 
///       templates, which might have commas, otherwise juse a second argument
///       would be sufficient.
#define FQ(...) enum { fq = __VA_ARGS__ };

/// Macro to define a function that returns expected frequency of a given type.
/// The syntax of the function should be: size_t f(const selector_type&);
/// FIX: Make sure there is no re-entrancy problems when pattern matching is
///      used inside of the frequency function f to uncover selector type.
#define FQS(f) static size_t frequency(intptr_t vtbl) { return ::f(*reinterpret_cast<const XTL_CPP0X_TYPENAME get_param<match_members>::type*>(&vtbl)); } bool frequency_dummy() const;

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
#define RS(...)       static inline decltype(unary(&__VA_ARGS__)) raise_selector() { return unary(&__VA_ARGS__); } bool raise_selector_dummy() const;

//------------------------------------------------------------------------------

/// Predefined structure that will be used when user did not specify smallest
/// kind value for the class hierarchy with SKV macro.
struct smallest_kind_unknown {};

/// Predefined structure that will be used when user did specify smallest kind
template <int N> struct smallest_kind_is { enum {value = N }; };
//smallest_kind_unknown smallest_kind_value_helper(...);
smallest_kind_is<0> smallest_kind_value_helper(...);

/// Accessort to get the smallest kind for the hierarchy type T belongs to
template <typename T>
struct smallest_kind
{
    typedef decltype(smallest_kind_value_helper(std::declval<T>())) type;
    enum { value = type::value };
};

/// Macro to be used in global scope to set the smallest kind value N for the 
/// class hierarchy rooted at C. When it is not used, 0 is assumed.
#define SKV(C,N) smallest_kind_is<N> smallest_kind_value_helper(const C&);

/// Amount of extra values in the range of allowed kinds we need for our purposes
const int reserved_extra_kinds = 1;

/// Convenience meta-function to get the original kind associated with the class T
template <typename T, size_t L> struct original_kind                   { enum { value = L }; }; // FIX: This temporarily for test purposes assumes only union case. Split in two with enable_if
template <typename T>           struct original_kind<T,default_layout> { enum { value = match_members<T>::kind_value }; };
/// Convenience meta-function to get the remapped kind associated with the class T
template <typename T, size_t L = default_layout> struct remapped_kind  { enum { value = original_kind<T,L>::value - smallest_kind<T>::value + reserved_extra_kinds }; };
/// Convenience function to convert remapped kind to original at run-time
template <typename T> inline int remapped2original(int k) { return k + smallest_kind<T>::value - reserved_extra_kinds; }
/// Convenience function to convert original kind to remapped at run-time
template <typename T> inline int original2remapped(int k) { return k - smallest_kind<T>::value + reserved_extra_kinds; }

//------------------------------------------------------------------------------

typedef std::unordered_map<int, const int*> kind_to_kinds_map;

template <typename T>
inline kind_to_kinds_map& get_kind_to_kinds_map() 
{
    static kind_to_kinds_map result;
    return result;
}

/// Gets all the kinds of a class with static type T and dynamic type represented 
/// by kind. The first element of the returned list will always be equal to kind,
/// the last to a dedicated value and those in between to the kinds of base classes.
template <typename T>
inline const int* get_kinds(int kind)
{
    static kind_to_kinds_map& k2k = get_kind_to_kinds_map<T>();
    return k2k[kind];
}

template <typename T>
inline const int* set_kinds(int kind, const int* kinds)
{
    static kind_to_kinds_map& k2k = get_kind_to_kinds_map<T>();
    return k2k[kind] = kinds;
}

template <typename D, typename B>
struct associate_kinds
{
    static const int* kinds;
};

template <typename D, typename B>
const int* associate_kinds<D,B>::kinds = set_kinds<B>(remapped_kind<D>::value, match_members<D>::get_kinds());

//------------------------------------------------------------------------------

/// A helper macro to access kind value of a class
#define BCK(D,B) (associate_kinds<D,B>::kinds,remapped_kind<B>::value)

/// A set of macros handling various amount of base classes passed to BCS macro.
#define BCS0()                        static const int* get_kinds() { static const int kinds[] = { 0 }; return kinds; }
#define BCS1(x0)                      static const int* get_kinds() { static const int kinds[] = { BCK(D,x0), 0 }; return kinds; }
#define BCS2(x0,x1)                   static const int* get_kinds() { static const int kinds[] = { BCK(D,x0), BCK(D,x1), 0 }; return kinds; }
#define BCS3(x0,x1,x2)                static const int* get_kinds() { static const int kinds[] = { BCK(D,x0), BCK(D,x1), BCK(D,x2), 0 }; return kinds; }
#define BCS4(x0,x1,x2,x3)             static const int* get_kinds() { static const int kinds[] = { BCK(D,x0), BCK(D,x1), BCK(D,x2), BCK(D,x3), 0 }; return kinds; }
#define BCS5(x0,x1,x2,x3,x4)          static const int* get_kinds() { static const int kinds[] = { BCK(D,x0), BCK(D,x1), BCK(D,x2), BCK(D,x3), BCK(D,x4), 0 }; return kinds; }
#define BCS6(x0,x1,x2,x3,x4,x5)       static const int* get_kinds() { static const int kinds[] = { BCK(D,x0), BCK(D,x1), BCK(D,x2), BCK(D,x3), BCK(D,x4), BCK(D,x5), 0 }; return kinds; }
#define BCS7(x0,x1,x2,x3,x4,x5,x6)    static const int* get_kinds() { static const int kinds[] = { BCK(D,x0), BCK(D,x1), BCK(D,x2), BCK(D,x3), BCK(D,x4), BCK(D,x5), BCK(D,x6), 0 }; return kinds; }
#define BCS8(x0,x1,x2,x3,x4,x5,x6,x7) static const int* get_kinds() { static const int kinds[] = { BCK(D,x0), BCK(D,x1), BCK(D,x2), BCK(D,x3), BCK(D,x4), BCK(D,x5), BCK(D,x6), BCK(D,x7), 0 }; return kinds; }

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
#define BOUND_VAR_DECL(P,V) const auto& V = apply_member(matched, match_members<target_type,default_layout>::XTL_CONCAT(member,P)())

/// A set of macros handling various amount of arguments passed to case statement.
#define DECL_BOUND_VAR_0(Dummy)                         XTL_UNUSED(matched);
#define DECL_BOUND_VAR_1(Dummy,x0)                      BOUND_VAR_DECL(0,x0);
#define DECL_BOUND_VAR_2(Dummy,x0,x1)                   BOUND_VAR_DECL(0,x0); BOUND_VAR_DECL(1,x1);
#define DECL_BOUND_VAR_3(Dummy,x0,x1,x2)                BOUND_VAR_DECL(0,x0); BOUND_VAR_DECL(1,x1); BOUND_VAR_DECL(2,x2);
#define DECL_BOUND_VAR_4(Dummy,x0,x1,x2,x3)             BOUND_VAR_DECL(0,x0); BOUND_VAR_DECL(1,x1); BOUND_VAR_DECL(2,x2); BOUND_VAR_DECL(3,x3);
#define DECL_BOUND_VAR_5(Dummy,x0,x1,x2,x3,x4)          BOUND_VAR_DECL(0,x0); BOUND_VAR_DECL(1,x1); BOUND_VAR_DECL(2,x2); BOUND_VAR_DECL(3,x3); BOUND_VAR_DECL(4,x4);
#define DECL_BOUND_VAR_6(Dummy,x0,x1,x2,x3,x4,x5)       BOUND_VAR_DECL(0,x0); BOUND_VAR_DECL(1,x1); BOUND_VAR_DECL(2,x2); BOUND_VAR_DECL(3,x3); BOUND_VAR_DECL(4,x4); BOUND_VAR_DECL(5,x5);
#define DECL_BOUND_VAR_7(Dummy,x0,x1,x2,x3,x4,x5,x6)    BOUND_VAR_DECL(0,x0); BOUND_VAR_DECL(1,x1); BOUND_VAR_DECL(2,x2); BOUND_VAR_DECL(3,x3); BOUND_VAR_DECL(4,x4); BOUND_VAR_DECL(5,x5); BOUND_VAR_DECL(6,x6);
#define DECL_BOUND_VAR_8(Dummy,x0,x1,x2,x3,x4,x5,x6,x7) BOUND_VAR_DECL(0,x0); BOUND_VAR_DECL(1,x1); BOUND_VAR_DECL(2,x2); BOUND_VAR_DECL(3,x3); BOUND_VAR_DECL(4,x4); BOUND_VAR_DECL(5,x5); BOUND_VAR_DECL(6,x6); BOUND_VAR_DECL(7,x7);

/// Helper macro for the one below
#define DECL_BOUND_VAR_(N, ...) XTL_CONCAT(DECL_BOUND_VAR_, N)(__VA_ARGS__)
/// A macro that will be passed arguments to case statement. This should include
/// the first type parameter that will be used as a dummy. This is required to
/// be able to handle 0 non-type parameters.
#define DECL_BOUND_VARS(...) DECL_BOUND_VAR_(XTL_NARG_EX(__VA_ARGS__),##__VA_ARGS__)

//------------------------------------------------------------------------------

/// When this macro is defined, our library will generate additional code that 
/// will trigger compiler to check case clauses for redundancy.
/// \warning Do not define this macro in actual builds as the generated code 
///          will effectively have a switch statement whose body is never evaluated!
#if defined(XTL_REDUNDANCY_CHECKING)
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
  /// should correspond to the try block. Our macros assume {} are present, but we
  /// can probably make it configurable via macro one day should there be demand for it.
  XTL_MESSAGE("WARNING: All Match statements except MatchE must have { and } around case clauses for redundancy checking mode!")
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

//------------------------------------------------------------------------------

/// Macro that starts the switch on dynamic type of a variable s that can be 
/// either pointer or reference to a polymorphic type.
/// \note case 0: instead of default: will work in the same way because we 
///       initialize cache with 0, however through experiments we can see
///       that having default here is quite a bit faster than having case 0.
#define TypeMatch(s) {\
        auto const   __selector_ptr = addr(s);\
        typedef XTL_CPP0X_TYPENAME underlying<decltype(*__selector_ptr)>::type selector_type;\
        enum { __base_counter = XTL_COUNTER };\
        XTL_ASSERT(("Trying to match against a nullptr",__selector_ptr));\
        static_assert(std::is_polymorphic<selector_type>::value, "Type of selector should be polymorphic when you use TypeMatch");\
        static vtblmap_of<selector_type, type_switch_info&> __vtbl2lines_map XTL_DUMP_PERFORMANCE_ONLY((__FILE__,__LINE__));\
        const void*  __casted_ptr;\
        type_switch_info& __switch_info = __vtbl2lines_map.get(__selector_ptr);\
        switch (__switch_info.line) {\
        default: {
/// Extended version of the above macro that takes an expected number of cases in
/// to estimate the size of the cache needed instead of using the default size
/// \note case 0: instead of default: will work in the same way because we 
///       initialize cache with 0, however through experiments we can see
///       that having default here is quite a bit faster than having case 0.
#define TypeMatchN(s,N) {\
        auto const   __selector_ptr = addr(s);\
        typedef XTL_CPP0X_TYPENAME underlying<decltype(*__selector_ptr)>::type selector_type;\
        enum { __base_counter = XTL_COUNTER };\
        XTL_ASSERT(("Trying to match against a nullptr",__selector_ptr));\
        static_assert(std::is_polymorphic<selector_type>::value, "Type of selector should be polymorphic when you use TypeMatchN");\
        static vtblmap_of<selector_type, type_switch_info&/*,requires_bits<N>::value*/> __vtbl2lines_map(XTL_DUMP_PERFORMANCE_ONLY(__FILE__,__LINE__,)requires_bits<N>::value);\
        const void*  __casted_ptr;\
        type_switch_info& __switch_info = __vtbl2lines_map.get(__selector_ptr);\
        switch (__switch_info.line) {\
        default: {

/// Macro that defines the case statement for the above switch
/// NOTE: If Visual C++ gives you error C2051: case expression not constant
///       on this CaseP label, just change the Debug Format in project setting 
///       Project -> Properties -> C/C++ -> General -> Debug Information Format 
///       from "Program Database for Edit And Continue (/ZI)" 
///       to   "Program Database (/Zi)", which is the default in Release builds,
///       but not in Debug. This is a known bug of Visual C++ described here:
///       http://connect.microsoft.com/VisualStudio/feedback/details/375836/-line-not-seen-as-compile-time-constant
#define TypeCase(C)   } if (XTL_UNLIKELY(__casted_ptr = dynamic_cast<const C*>(__selector_ptr))) { if (__switch_info.line == 0) { __switch_info.line = __LINE__; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); } case __LINE__: auto matched = adjust_ptr<C>(__selector_ptr,__switch_info.offset); XTL_UNUSED(matched);
#define EndTypeMatch } if (__switch_info.line == 0) { __switch_info.line = __LINE__; } case __LINE__: ; }}

//------------------------------------------------------------------------------

/// Macro that starts the switch on pattern
/// \note case 0: instead of default: will work in the same way because we 
///       initialize cache with 0, however through experiments we can see
///       that having default here is quite a bit faster than having case 0.
#define MatchP(s) {                                                                            \
        auto const   __selector_ptr = addr(s);                                                 \
        typedef XTL_CPP0X_TYPENAME underlying<decltype(*__selector_ptr)>::type selector_type;  \
        enum { __base_counter = XTL_COUNTER };                                                 \
        XTL_ASSERT(("Trying to match against a nullptr",__selector_ptr));                      \
        static_assert(std::is_polymorphic<selector_type>::value, "Type of selector should be polymorphic when you use MatchP");\
        static vtblmap_of<selector_type, type_switch_info&> __vtbl2lines_map(XTL_DUMP_PERFORMANCE_ONLY(__FILE__,__LINE__,)deferred_value<vtbl_count_t>::get<__base_counter>());\
        const void*  __casted_ptr;                                                             \
        type_switch_info& __switch_info = __vtbl2lines_map.get(__selector_ptr);                \
        switch (__switch_info.line)                                                            \
        {                                                                                      \
        XTL_REDUNDANCY_LABEL(default:) { XTL_REDUNDANCY_TRY {{

/// Extended version of the macro that starts the switch on pattern, that takes an expected number of cases in
/// \note case 0: instead of default: will work in the same way because we 
///       initialize cache with 0, however through experiments we can see
///       that having default here is quite a bit faster than having case 0.
#define MatchP_N(s,N) {                                                                        \
        auto const   __selector_ptr = addr(s);                                                 \
        typedef XTL_CPP0X_TYPENAME underlying<decltype(*__selector_ptr)>::type selector_type;  \
        enum { __base_counter = XTL_COUNTER };                                                 \
        XTL_ASSERT(("Trying to match against a nullptr",__selector_ptr));                      \
        static_assert(std::is_polymorphic<selector_type>::value, "Type of selector should be polymorphic when you use MatchP_N");\
        static vtblmap_of<selector_type, type_switch_info&/*,requires_bits<N>::value*/> __vtbl2lines_map(XTL_DUMP_PERFORMANCE_ONLY(__FILE__,__LINE__,)deferred_value<vtbl_count_t>::get<__base_counter>());\
        const void*  __casted_ptr;                                                             \
        type_switch_info& __switch_info = __vtbl2lines_map.get(__selector_ptr);                \
        switch (__switch_info.line)                                                            \
        {                                                                                      \
        XTL_REDUNDANCY_LABEL(default:) { XTL_REDUNDANCY_TRY {{

/// NOTE: We need this extra indirection to properly handle 0 arguments as it
///       seems to be impossible to introduce dummy argument inside the Case 
///       directly, so we use the type argument as a dummy argument for DECL_BOUND_VARS
#define CaseP_(C,...) }}}                                                                      \
        XTL_REDUNDANCY_CATCH(C)                                                                \
        {                                                                                      \
            typedef C target_type;                                                             \
            enum { target_label = XTL_COUNTER-__base_counter };                                \
            if (XTL_UNLIKELY(__casted_ptr = dynamic_cast<const target_type*>(__selector_ptr))) \
            {                                                                                  \
                if (XTL_LIKELY((__switch_info.line == 0)))                                     \
                {                                                                              \
                    __switch_info.line = target_label;                                         \
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);    \
                }                                                                              \
            XTL_REDUNDANCY_LABEL(case target_label:)                                           \
                auto matched = adjust_ptr<target_type>(__selector_ptr,__switch_info.offset);   \
                XTL_UNUSED(matched);

/// Macro that defines the case statement for the above switch
/// NOTE: If Visual C++ gives you error C2051: case expression not constant
///       on this CaseP label, just change the Debug Format in project setting 
///       Project -> Properties -> C/C++ -> General -> Debug Information Format 
///       from "Program Database for Edit And Continue (/ZI)" 
///       to   "Program Database (/Zi)", which is the default in Release builds,
///       but not in Debug. This is a known bug of Visual C++ described here:
///       http://connect.microsoft.com/VisualStudio/feedback/details/375836/-line-not-seen-as-compile-time-constant
#ifdef _MSC_VER
    #define CaseP(...) XTL_APPLY_VARIADIC_MACRO(CaseP_,(__VA_ARGS__)) XTL_APPLY_VARIADIC_MACRO(DECL_BOUND_VARS,(__VA_ARGS__)) {
#else
    #define CaseP(...) CaseP_(__VA_ARGS__) DECL_BOUND_VARS(__VA_ARGS__) {
#endif
#define QueP(C,...)  }}}                                                                       \
        XTL_REDUNDANCY_CATCH(C)                                                                \
        {                                                                                      \
            typedef C target_type;                                                             \
            enum { target_label = XTL_COUNTER-__base_counter };                                \
            if (XTL_UNLIKELY(__casted_ptr = dynamic_cast<const target_type*>(__selector_ptr))) \
            {                                                                                  \
                if (XTL_LIKELY((__switch_info.line == 0)))                                     \
                {                                                                              \
                    __switch_info.line = target_label;                                         \
                    __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr);    \
                }                                                                              \
            XTL_REDUNDANCY_LABEL(case target_label:)                                           \
                auto matched = adjust_ptr<target_type>(__selector_ptr,__switch_info.offset);   \
                if (XTL_LIKELY(match<target_type>(__VA_ARGS__)(matched))) {

#define OrP(...)     } else if (match<target_type>(__VA_ARGS__)(matched)) {
#define OtherwiseP(...) CaseP(selector_type UCL_PP_IF(UCL_PP_IS_EMPTY(__VA_ARGS__), UCL_PP_EMPTY(), ,) __VA_ARGS__)
#define EndMatchP    }}} enum { target_label = XTL_COUNTER-__base_counter }; deferred_value<vtbl_count_t>::set<__base_counter,target_label-1>::value; if (XTL_LIKELY((__switch_info.line == 0))) { __switch_info.line = target_label; } case target_label: ; }}

//------------------------------------------------------------------------------

/// Macro that starts the switch on types that carry their own dynamic type as
/// a distinct integral value in one of their members.
#define MatchK(s) {                                                                            \
        auto const __selector_ptr  = addr(s);                                                  \
        typedef XTL_CPP0X_TYPENAME underlying<decltype(*__selector_ptr)>::type selector_type;  \
        static_assert(has_member_kind_selector<match_members<selector_type>>::value, "Before using MatchK, you have to specify kind selector on the selector type using KS macro");\
        XTL_ASSERT(("Trying to match against a nullptr",__selector_ptr));                      \
        auto const __kind_selector = apply_member(__selector_ptr, match_members<selector_type>::kind_selector());\
        switch (__kind_selector) { {{

/// NOTE: We need this extra indirection to properly handle 0 arguments as it
///       seems to be impossible to introduce dummy argument inside the Case 
///       directly, so we use the type argument as a dummy argument for DECL_BOUND_VARS
#define CaseK_(C,...) }}                                                                       \
        if (XTL_UNLIKELY((__kind_selector == match_members<C>::kind_value)))                   \
        {                                                                                      \
        case match_members<C>::kind_value:                                                     \
            typedef C target_type;                                                             \
            auto matched = stat_cast<target_type>(__selector_ptr);                             \
            XTL_UNUSED(matched);

/// Macro that defines the case statement for the above switch
#ifdef _MSC_VER
    #define CaseK(...) XTL_APPLY_VARIADIC_MACRO(CaseK_,(__VA_ARGS__)) XTL_APPLY_VARIADIC_MACRO(DECL_BOUND_VARS,(__VA_ARGS__)) {
#else
    #define CaseK(...) CaseK_(__VA_ARGS__) DECL_BOUND_VARS(__VA_ARGS__) {
#endif

#define EndMatchK   }} }}

//------------------------------------------------------------------------------

/// Macro that starts the switch on types that carry their own dynamic type as
/// a distinct integral value in one of their members.
#define MatchU(s) {                                                                            \
        auto const __selector_ptr  = addr(s);                                                  \
        typedef XTL_CPP0X_TYPENAME underlying<decltype(*__selector_ptr)>::type selector_type;  \
        static_assert(has_member_kind_selector<match_members<selector_type>>::value, "Before using MatchU, you have to specify kind selector on the selector type using KS macro");\
        XTL_ASSERT(("Trying to match against a nullptr",__selector_ptr));                      \
        auto const __kind_selector = apply_member(__selector_ptr, match_members<selector_type>::kind_selector());\
        switch (__kind_selector) { {{

/// Macro that defines the case statement for the above switch
#define CaseU_(L,...) }}                                                                       \
        if (XTL_UNLIKELY((__kind_selector == match_members<selector_type,L>::kind_value)))     \
        {                                                                                      \
        case match_members<selector_type,L>::kind_value:                                       \
            typedef selector_type target_type;                                                 \
            enum { default_layout = L };                                                       \
            auto matched = __selector_ptr;                                                     \
            XTL_UNUSED(matched);

/// Macro that defines the case statement for the above switch
#ifdef _MSC_VER
    #define CaseU(...) XTL_APPLY_VARIADIC_MACRO(CaseU_,(__VA_ARGS__)) XTL_APPLY_VARIADIC_MACRO(DECL_BOUND_VARS,(__VA_ARGS__)) {
#else
    #define CaseU(...) CaseU_(__VA_ARGS__) DECL_BOUND_VARS(__VA_ARGS__) {
#endif

#define EndMatchU   }} }}

//------------------------------------------------------------------------------

/// Macro that starts the switch on types that carry their own dynamic type as
/// a distinct integral value in one of their members.
/// \note Unlike the rest of our Match statements, MatchE does not allow {} 
///       around the case clauses.
#define MatchE(s) {                                                                            \
        auto const __selector_ptr  = addr(s);                                                  \
        typedef XTL_CPP0X_TYPENAME underlying<decltype(*__selector_ptr)>::type selector_type;  \
        static_assert(has_member_raise_selector<match_members<selector_type>>::value, "Before using MatchE, you have to specify raise selector on the selector type using RS macro");\
        XTL_ASSERT(("Trying to match against a nullptr",__selector_ptr));                      \
        try { apply_member(__selector_ptr, match_members<selector_type>::raise_selector());

/// Macro that defines the case statement for the above switch
#define CaseE_(L,...) }                                                                        \
        catch (L& matched_ref)                                                                 \
        {                                                                                      \
            typedef L target_type;                                                             \
            auto matched = &matched_ref;                                                       \
            XTL_UNUSED(matched);

/// Macro that defines the case statement for the above switch
#ifdef _MSC_VER
    #define CaseE(...) XTL_APPLY_VARIADIC_MACRO(CaseE_,(__VA_ARGS__)) XTL_APPLY_VARIADIC_MACRO(DECL_BOUND_VARS,(__VA_ARGS__)) /*{*/
#else
    #define CaseE(...) CaseE_(__VA_ARGS__) DECL_BOUND_VARS(__VA_ARGS__) /*{*/
#endif

#define EndMatchE } catch (...) {} }

//------------------------------------------------------------------------------

/// Macro that starts the switch on types that carry their own dynamic type as
/// a distinct integral value in one of their members.
/// Non-forwarding: Sequential:  33% faster; Random: 34% faster
///     Forwarding: Sequential: 251% faster; Random: 33% faster
#define MatchF(s) {\
        auto const __selector_ptr  = addr(s);\
        typedef XTL_CPP0X_TYPENAME underlying<decltype(*__selector_ptr)>::type selector_type;\
        static_assert(has_member_kind_selector<match_members<selector_type>>::value, "Before using MatchF, you have to specify kind selector on the selector type using KS macro");\
        XTL_ASSERT(("Trying to match against a nullptr",__selector_ptr));\
        auto __kind_selector = original2remapped<selector_type>((int)apply_member(__selector_ptr, match_members<selector_type>::kind_selector()));\
        const int* __kinds = 0;\
        size_t __attempt;\
        XTL_CONCAT(ReMatch,__LINE__):\
        switch (__kind_selector) {\
        default:\
            if (XTL_LIKELY(!__kinds)) \
            { \
                __attempt = 0;\
                static std::vector<const int*> __kinds_cache;\
                if (XTL_UNLIKELY(__kind_selector >= __kinds_cache.size()))\
                    __kinds_cache.resize(__kind_selector+1);\
                if (!(__kinds = __kinds_cache[__kind_selector]))\
                    __kinds = __kinds_cache[__kind_selector] = get_kinds<selector_type>(__kind_selector);\
            } \
            XTL_ASSERT(("Base classes for this kind were not specified",__kinds));\
            XTL_ASSERT(("Invalid list of kinds",__kinds[__attempt]==__kind_selector));\
            __kind_selector = __kinds[++__attempt];\
            goto XTL_CONCAT(ReMatch,__LINE__);\
        case 0: break; {{

/// Version of the above macro when kinds are stored in vtbl-like of structure.
/// Non-forwarding: Sequential:  68% faster; Random: 37% faster
///     Forwarding: Sequential: 425% faster; Random: 54% faster
//#define MatchF(s) {\
//        auto const __selector_ptr  = addr(s);\
//        typedef XTL_CPP0X_TYPENAME underlying<decltype(*__selector_ptr)>::type selector_type;\
//        XTL_ASSERT(("Trying to match against a nullptr",__selector_ptr));\
////        auto __kind_selector = original2remapped<selector_type>((int)apply_member(__selector_ptr, match_members<selector_type>::kind_selector()));\
//        size_t __attempt = 0;\
//        XTL_CONCAT(ReMatch,__LINE__):\
//        switch (__kind_selector) {\
//        default:\
//            XTL_ASSERT(("Base classes for this kind were not specified",__selector_ptr->m_all_kinds));\
//            XTL_ASSERT(("Invalid list of kinds",__selector_ptr->m_all_kinds[__attempt]==__kind_selector));\
//            __kind_selector = __selector_ptr->m_all_kinds[++__attempt];\
//            goto XTL_CONCAT(ReMatch,__LINE__);\
//        case 0: break; {{

/// NOTE: We need this extra indirection to properly handle 0 arguments as it
///       seems to be impossible to introduce dummy argument inside the Case 
///       directly, so we use the type argument as a dummy argument for DECL_BOUND_VARS
#define CaseF_(C,...) }} \
        if (XTL_UNLIKELY((__kind_selector == remapped_kind<C>::value))) \
        { \
        case remapped_kind<C>::value: \
            typedef C target_type; \
            auto matched = stat_cast<target_type>(__selector_ptr); \
            XTL_UNUSED(matched);

/// Macro that defines the case statement for the above switch
#ifdef _MSC_VER
    #define CaseF(...) XTL_APPLY_VARIADIC_MACRO(CaseF_,(__VA_ARGS__)) XTL_APPLY_VARIADIC_MACRO(DECL_BOUND_VARS,(__VA_ARGS__)) {
#else
    #define CaseF(...) CaseF_(__VA_ARGS__) DECL_BOUND_VARS(__VA_ARGS__) {
#endif

#define OtherwiseF(...) CaseF(selector_type UCL_PP_IF(UCL_PP_IS_EMPTY(__VA_ARGS__), UCL_PP_EMPTY(), ,) __VA_ARGS__)
#define EndMatchF   }} }}

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
#define MatchG(s) {\
        auto const __selector_ptr = addr(s);\
        typedef XTL_CPP0X_TYPENAME underlying<decltype(*__selector_ptr)>::type selector_type;\
        XTL_ASSERT(("Trying to match against a nullptr",__selector_ptr));\
        enum { __base_counter = XTL_COUNTER };\
        typedef generic_switch<selector_type> switch_traits;\
        static XTL_CPP0X_TYPENAME switch_traits::static_data_type static_data;\
               XTL_CPP0X_TYPENAME switch_traits::local_data_type  local_data;\
        switch (switch_traits::choose(__selector_ptr,static_data,local_data))\
        {\
            XTL_REDUNDANCY_LABEL(case switch_traits::XTL_CPP0X_TEMPLATE CaseLabel<0>::entry:) { XTL_REDUNDANCY_TRY {{

/// NOTE: We need this extra indirection to properly handle 0 arguments as it
///       seems to be impossible to introduce dummy argument inside the Case 
///       directly, so we use the type argument as a dummy argument for DECL_BOUND_VARS
#define CaseG_(C,...) }}} \
        XTL_REDUNDANCY_CATCH(C) \
        { \
            typedef XTL_CPP0X_TYPENAME switch_traits::XTL_CPP0X_TEMPLATE disambiguate<sizeof(C)<sizeof(XTL_CPP0X_TYPENAME switch_traits::selector_type)>::XTL_CPP0X_TEMPLATE parameter<C> target_specific; \
            typedef XTL_CPP0X_TYPENAME target_specific::target_type target_type; \
            enum { default_layout = target_specific::layout, target_label = XTL_COUNTER-__base_counter }; \
            if (XTL_UNLIKELY(target_specific::main_condition(__selector_ptr, local_data))) \
            { \
                switch_traits::on_first_pass(__selector_ptr, local_data, target_label); \
            XTL_REDUNDANCY_LABEL(case target_specific::XTL_CPP0X_TEMPLATE CaseLabel<target_label>::value:) \
                auto matched = target_specific::get_matched(__selector_ptr,local_data); \
                XTL_UNUSED(matched);

/// Macro that defines the case statement for the above switch
/// NOTE: If Visual C++ gives you error C2051: case expression not constant
///       on this CaseP label, just change the Debug Format in project setting 
///       Project -> Properties -> C/C++ -> General -> Debug Information Format 
///       from "Program Database for Edit And Continue (/ZI)" 
///       to   "Program Database (/Zi)", which is the default in Release builds,
///       but not in Debug. This is a known bug of Visual C++ described here:
///       http://connect.microsoft.com/VisualStudio/feedback/details/375836/-line-not-seen-as-compile-time-constant
#ifdef _MSC_VER
    #define CaseG(...) XTL_APPLY_VARIADIC_MACRO(CaseG_,(__VA_ARGS__)) XTL_APPLY_VARIADIC_MACRO(DECL_BOUND_VARS,(__VA_ARGS__)) {
#else
    #define CaseG(...) CaseG_(__VA_ARGS__) DECL_BOUND_VARS(__VA_ARGS__) {
#endif
#define QueG(C,...)  }}} {\
        enum { target_label = XTL_COUNTER-__base_counter }; \
        typedef XTL_CPP0X_TYPENAME switch_traits::XTL_CPP0X_TEMPLATE disambiguate<sizeof(C)<sizeof(XTL_CPP0X_TYPENAME switch_traits::selector_type)>::XTL_CPP0X_TEMPLATE parameter<C> target_specific; \
        if (XTL_UNLIKELY(target_specific::main_condition(__selector_ptr, local_data))) \
        { \
            switch_traits::on_first_pass(__selector_ptr, local_data, target_label); \
        case target_specific::XTL_CPP0X_TEMPLATE CaseLabel<target_label>::value: \
            auto matched = target_specific::get_matched(__selector_ptr,local_data); \
            if (XTL_LIKELY(match<XTL_CPP0X_TYPENAME target_specific::target_type,target_specific::layout>(__VA_ARGS__)(matched))) {
#define OrG(...)     } else if (match<XTL_CPP0X_TYPENAME target_specific::target_type,target_specific::layout>(__VA_ARGS__)(matched)) {
#define OtherwiseG() }}} {{ default: auto matched = __selector_ptr; { XTL_UNUSED(matched);
#define EndMatchG    }}} \
        enum { target_label = XTL_COUNTER-__base_counter }; \
        switch_traits::on_end(__selector_ptr, local_data, target_label); \
        case switch_traits::XTL_CPP0X_TEMPLATE CaseLabel<target_label>::exit: ; }}

//------------------------------------------------------------------------------

/// Macro that starts generic switch on types capable of figuring out by itself
/// which of the 3 cases presented above we are dealing with: open, closed or union.
/// \note This macro cannot be used in a template context! If you need to have 
///       it inside a template, please use @TMatch and corresponding @TCase and 
///       @TEndMatch. Unfortunately at the moment we are unaware how to unify 
///       these 2 macros as types and templates should only be annotated inside
///       a template context and not outside.
#define MatchQ(s) {\
        auto const __selector_ptr = addr(s);\
        typedef XTL_CPP0X_TYPENAME underlying<decltype(*__selector_ptr)>::type selector_type;\
        XTL_ASSERT(("Trying to match against a nullptr",__selector_ptr));\
        enum { __base_counter = XTL_COUNTER };\
        typedef unified_switch<selector_type> switch_traits;\
        static XTL_CPP0X_TYPENAME switch_traits::static_data_type static_data;\
               XTL_CPP0X_TYPENAME switch_traits::local_data_type  local_data;\
        size_t jump_target = switch_traits::choose(__selector_ptr,static_data,local_data);\
        XTL_CONCAT(ReMatch,__LINE__):\
        switch (jump_target)\
        {\
            XTL_REDUNDANCY_LABEL(default:) { XTL_REDUNDANCY_TRY {{\
            if (switch_traits::on_default(jump_target,local_data,static_data))\
                goto XTL_CONCAT(ReMatch,__LINE__);\

/// NOTE: We need this extra indirection to properly handle 0 arguments as it
///       seems to be impossible to introduce dummy argument inside the Case 
///       directly, so we use the type argument as a dummy argument for DECL_BOUND_VARS
#define CaseQ_(C,...) }}} \
        XTL_REDUNDANCY_CATCH(C) \
        { \
            typedef XTL_CPP0X_TYPENAME switch_traits::XTL_CPP0X_TEMPLATE disambiguate<sizeof(C)<sizeof(XTL_CPP0X_TYPENAME switch_traits::selector_type)>::XTL_CPP0X_TEMPLATE parameter<C> target_specific; \
            typedef XTL_CPP0X_TYPENAME target_specific::target_type target_type; \
            enum { default_layout = target_specific::layout, target_label = XTL_COUNTER-__base_counter }; \
            if (XTL_UNLIKELY(target_specific::main_condition(__selector_ptr, local_data))) \
            { \
                switch_traits::on_first_pass(__selector_ptr, local_data, target_label); \
            XTL_REDUNDANCY_LABEL(case target_specific::XTL_CPP0X_TEMPLATE CaseLabel<target_label>::value:) \
                auto matched = target_specific::get_matched(__selector_ptr,local_data); \
                XTL_UNUSED(matched);

/// Macro that defines the case statement for the above switch
/// NOTE: If Visual C++ gives you error C2051: case expression not constant
///       on this CaseP label, just change the Debug Format in project setting 
///       Project -> Properties -> C/C++ -> General -> Debug Information Format 
///       from "Program Database for Edit And Continue (/ZI)" 
///       to   "Program Database (/Zi)", which is the default in Release builds,
///       but not in Debug. This is a known bug of Visual C++ described here:
///       http://connect.microsoft.com/VisualStudio/feedback/details/375836/-line-not-seen-as-compile-time-constant
#ifdef _MSC_VER
    #define CaseQ(...) XTL_APPLY_VARIADIC_MACRO(CaseQ_,(__VA_ARGS__)) XTL_APPLY_VARIADIC_MACRO(DECL_BOUND_VARS,(__VA_ARGS__)) {
#else
    #define CaseQ(...) CaseQ_(__VA_ARGS__) DECL_BOUND_VARS(__VA_ARGS__) {
#endif
#define QueQ(C,...)  }}}                                                                                                 \
        XTL_REDUNDANCY_CATCH(C)                                                                                          \
        {                                                                                                                \
            enum { target_label = XTL_COUNTER-__base_counter };                                                          \
            typedef XTL_CPP0X_TYPENAME switch_traits::                                                                   \
                    XTL_CPP0X_TEMPLATE disambiguate<sizeof(C)<sizeof(XTL_CPP0X_TYPENAME switch_traits::selector_type)>:: \
                    XTL_CPP0X_TEMPLATE parameter<C> target_specific;                                                     \
            if (XTL_UNLIKELY(target_specific::main_condition(__selector_ptr, local_data)))                               \
            {                                                                                                            \
                switch_traits::on_first_pass(__selector_ptr, local_data, target_label);                                  \
            XTL_REDUNDANCY_LABEL(case target_specific::XTL_CPP0X_TEMPLATE CaseLabel<target_label>::value:)               \
                auto matched = target_specific::get_matched(__selector_ptr,local_data);                                  \
                if (XTL_LIKELY(match<XTL_CPP0X_TYPENAME target_specific::target_type,target_specific::layout>(__VA_ARGS__)(matched))) {
#define OrQ(...)     } else if (match<XTL_CPP0X_TYPENAME target_specific::target_type,target_specific::layout>(__VA_ARGS__)(matched)) {
#define OtherwiseQ(...) CaseQ(selector_type UCL_PP_IF(UCL_PP_IS_EMPTY(__VA_ARGS__), UCL_PP_EMPTY(), ,) __VA_ARGS__)
#define EndMatchQ    }}} \
        enum { target_label = XTL_COUNTER-__base_counter }; \
        switch_traits::on_end(__selector_ptr, local_data, target_label); \
        case switch_traits::XTL_CPP0X_TEMPLATE CaseLabel<target_label>::exit: ; }}

//------------------------------------------------------------------------------

/// Now based on user's preference we can set up default syntax to one of the above
#if   XTL_DEFAULT_SYNTAX == 'P'
  /// The user chooses polymorphic match statement to be the default
  XTL_MESSAGE("Default pattern matching syntax is: P")
  #define  Match      MatchP
  #define  Case       CaseP
  #define  Que        QueP
  #define  Alt        OrP // Conflicts with ipr::Or
  #define  Otherwise  OtherwiseP
  #define  EndMatch   EndMatchP
#elif XTL_DEFAULT_SYNTAX == 'K'
  /// The user chooses closed kind match statement to be the default
  XTL_MESSAGE("Default pattern matching syntax is: K")
  #define  Match      MatchK
  #define  Case       CaseK
  #define  Que        QueK
  #define  Alt        OrK // Conflicts with ipr::Or
  #define  Otherwise  OtherwiseK
  #define  EndMatch   EndMatchK
#elif XTL_DEFAULT_SYNTAX == 'U'
  /// The user chooses discriminated union statement to be the default
  XTL_MESSAGE("Default pattern matching syntax is: U")
  #define  Match      MatchU
  #define  Case       CaseU
  #define  Que        QueU
  #define  Alt        OrU // Conflicts with ipr::Or
  #define  Otherwise  OtherwiseU
  #define  EndMatch   EndMatchU
#elif XTL_DEFAULT_SYNTAX == 'E'
  /// The user chooses exception match statement to be the default
  XTL_MESSAGE("Default pattern matching syntax is: E")
  #define  Match      MatchE
  #define  Case       CaseE
  #define  Que        QueE
  #define  Alt        OrE // Conflicts with ipr::Or
  #define  Otherwise  OtherwiseE
  #define  EndMatch   EndMatchE
#elif XTL_DEFAULT_SYNTAX == 'F'
  /// The user chooses closed kind match statement to be the default
  XTL_MESSAGE("Default pattern matching syntax is: F")
  #define  Match      MatchF
  #define  Case       CaseF
  #define  Que        QueF
  #define  Alt        OrF // Conflicts with ipr::Or
  #define  Otherwise  OtherwiseF
  #define  EndMatch   EndMatchF
#else
  /// The user chooses generic match statement to be the default
  XTL_MESSAGE("Default pattern matching syntax is: G")
  //#define  Match      MatchG
  //#define  Case       CaseG
  //#define  Que        QueG
  //#define  Alt        OrG // Conflicts with ipr::Or
  //#define  Otherwise  OtherwiseG
  //#define  EndMatch   EndMatchG
  #define  Match      MatchQ
  #define  Case       CaseQ
  #define  Que        QueQ
  #define  Alt        OrQ // Conflicts with ipr::Or
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
/// \note This is the general case that essentially assumes open case
template <typename SelectorType, class condition = void>
class generic_switch
{
public:

    /// Type of the argument on which extended switch is done
    typedef typename underlying<SelectorType>::type selector_type;

    // Open case only works for polymorphic types (types with virtual funcs)
    // If your base type is not polymorphic, but you'd still like to have 
    // type switching/pattern matching functionality on it, use KS macro in 
    // corresponding @match_members<your_base_type> to identify which field
    // behaves as a selector and thus uniquely identifies derived type.
    static_assert(
        std::is_polymorphic<selector_type>::value,
        "Type of selector should either be polymorphic or you should provide kind selector for the type"
    );

    /// Type of data that has to be statically allocated inside the block 
    /// containg extended switch
    typedef vtblmap_of<selector_type, type_switch_info&> static_data_type;

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
            entry = 0,      ///< Case label that will be used to enter beginning of the switch
            exit  = Counter ///< Case label that will be used to jump to the end of the switch
        };
    };

    /// Function used to get the value we'll be switching on
    static inline size_t choose(const selector_type* selector_ptr, static_data_type& static_data, local_data_type& local_data)
    {
        local_data.switch_info_ptr = &static_data.get(selector_ptr);
        return local_data.switch_info_ptr->line;
    }

    /// Function that will be called upon first entry to the case through the fall-through behavior
    static inline void on_first_pass(const selector_type* selector_ptr, local_data_type& local_data, size_t line)
    {
        if (XTL_LIKELY(local_data.switch_info_ptr->line == 0)) 
        {
            local_data.switch_info_ptr->line   = line; 
            local_data.switch_info_ptr->offset = intptr_t(local_data.casted_ptr)-intptr_t(selector_ptr);
        } 
    }
    
    /// Function that will be called when the fall-through behavior reached end of the switch
    static inline void on_end(const selector_type* selector_ptr, local_data_type& local_data, size_t line)
    {
        if (XTL_LIKELY(local_data.switch_info_ptr->line == 0)) 
        { 
            local_data.switch_info_ptr->line   = line;
        }
    }

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
            typedef T target_type;

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

            /// Layout that has to be used for the given target type.
            enum { layout = default_layout };

            /// Condition that guards applicability of the given case clause
            /// during the fall-through behavior.
            static inline bool main_condition(const selector_type* selector_ptr, local_data_type& local_data)
            {
                return (local_data.casted_ptr = dynamic_cast<const target_type*>(selector_ptr)) != 0;
            }

            /// Performs the necessary conversion of the original selector into the proper
            /// object of target type.
            /// \note The selector is const-qualified, thus the target is also const-qualified
            static inline const target_type* get_matched(const selector_type* selector_ptr, local_data_type& local_data)
            {
                //std::cout << "Open case (const)" << std::endl;
                return adjust_ptr<target_type>(selector_ptr,local_data.switch_info_ptr->offset);
            }

            /// Performs the necessary conversion of the original selector into the proper
            /// object of target type.
            /// \note The selector is non-const, thus the target is also non-const
            static inline       target_type* get_matched(      selector_type* selector_ptr, local_data_type& local_data)
            {
                //std::cout << "Open case (non-const)" << std::endl;
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
class generic_switch<
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
    typedef bool static_data_type;

    /// Type of data that has to be automatically allocated inside the block 
    /// containg extended switch
    typedef bool local_data_type;

    enum 
    {
        /// The value that should be equal to the smalles kind used by selector_type
        /// FIX: Let user override this inside @match_members in case his minimum is not 0
        user_kind_minimum_value = 0,
        /// Just a mnemonic name to the amount of cases we add on top of user kinds.
        /// We effectively shift user kinds by this number in order to maintain all
        /// case labels sequentials to assure that jump table is generated for the 
        /// switch.
        kind_selector_shift     = 2
    };

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
            entry = user_kind_minimum_value, ///< Case label that will be used to enter beginning of the switch
            exit                             ///< Case label that will be used to jump to the end of the switch
        };
    };

    /// Function used to get the value we'll be switching on
    static inline auto choose(const selector_type* selector_ptr, static_data_type& static_data, local_data_type& local_data) 
             -> typename underlying<decltype(apply_member(selector_ptr, match_members<selector_type>::kind_selector()))>::type
    {
        typedef typename underlying<decltype(apply_member(selector_ptr, match_members<selector_type>::kind_selector()))>::type result_type; // Can be enum
        return result_type(apply_member(selector_ptr, match_members<selector_type>::kind_selector()) + kind_selector_shift);
    }

    /// Function that will be called upon first entry to the case through the fall-through behavior
    static inline void on_first_pass(const selector_type*, local_data_type&, size_t) {}

    /// Function that will be called when the fall-through behavior reached end of the switch
    static inline void        on_end(const selector_type*, local_data_type&, size_t) {}

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
            typedef selector_type target_type;

            /// Depending on whether we handle open or closed case, different case labels
            /// are used for the generated match statement. This metafunction takes
            /// a unique (per match statement) counter and returns the actual label that
            /// will be used for the case clause.
            template <size_t Counter>
            struct CaseLabel
            {
                enum 
                {
                    value = N + kind_selector_shift          ///< Case label that will be used for case at line offset Counter
                };
            };
            
            /// Layout that has to be used for the given target type.
            enum { layout = N };

            /// Condition that guards applicability of the given case clause
            /// during the fall-through behavior.
            static inline bool main_condition(const selector_type* selector_ptr, local_data_type& local_data) { return true; }

            /// Performs the necessary conversion of the original selector into the proper
            /// object of target type.
            /// \note The selector is const-qualified, thus the target is also const-qualified
            static inline const target_type* get_matched(const selector_type* selector_ptr, local_data_type& local_data)
            {
                //std::cout << "Union case (const)" << std::endl;
                return selector_ptr;
            }

            /// Performs the necessary conversion of the original selector into the proper
            /// object of target type.
            /// \note The selector is non-const, thus the target is also non-const
            static inline       target_type* get_matched(      selector_type* selector_ptr, local_data_type& local_data)
            {
                //std::cout << "Union case (non-const)" << std::endl;
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
            typedef T target_type;

            /// Depending on whether we handle open or closed case, different case labels
            /// are used for the generated match statement. This metafunction takes
            /// a unique (per match statement) counter and returns the actual label that
            /// will be used for the case clause.
            template <size_t Counter>
            struct CaseLabel
            {
                enum 
                {
                    value = match_members<target_type>::kind_value + kind_selector_shift ///< Case label that will be used for case at line offset Counter
                };
            };

            /// Layout that has to be used for the given target type.
            enum { layout = default_layout };

            /// Condition that guards applicability of the given case clause
            /// during the fall-through behavior.
            static inline bool main_condition(const selector_type* selector_ptr, local_data_type& local_data) { return true; }

            /// Performs the necessary conversion of the original selector into the proper
            /// object of target type.
            /// \note The selector is const-qualified, thus the target is also const-qualified
            static inline const target_type* get_matched(const selector_type* selector_ptr, local_data_type& local_data)
            {
                //std::cout << "Closed case (const)" << std::endl;
                return stat_cast<target_type>(selector_ptr);
            }

            /// Performs the necessary conversion of the original selector into the proper
            /// object of target type.
            /// \note The selector is non-const, thus the target is also non-const
            static inline       target_type* get_matched(      selector_type* selector_ptr, local_data_type& local_data)
            {
                //std::cout << "Closed case (non-const)" << std::endl;
                return stat_cast<target_type>(selector_ptr);
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
/// \note This is the general case that essentially assumes open case
/// \note The only difference of @unified_switch from @generic_switch is that it
///       is capable of handling base classes in case clauses for the closed case.
///       The open case is exactly the same.
template <typename SelectorType, class condition = void>
class unified_switch
{
public:

    /// Type of the argument on which extended switch is done
    typedef typename underlying<SelectorType>::type selector_type;

    // Open case only works for polymorphic types (types with virtual funcs)
    // If your base type is not polymorphic, but you'd still like to have 
    // type switching/pattern matching functionality on it, use KS macro in 
    // corresponding @match_members<your_base_type> to identify which field
    // behaves as a selector and thus uniquely identifies derived type.
    static_assert(
        std::is_polymorphic<selector_type>::value,
        "Type of selector should either be polymorphic or you should provide kind selector for the type"
    );

    /// Type of data that has to be statically allocated inside the block 
    /// containg extended switch
    typedef vtblmap_of<selector_type, type_switch_info&> static_data_type;

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
    static inline size_t choose(const selector_type* selector_ptr, static_data_type& static_data, local_data_type& local_data)
    {
        local_data.switch_info_ptr = &static_data.get(selector_ptr);
        return local_data.switch_info_ptr->line;
    }

    /// Function that will be called upon first entry to the case through the fall-through behavior
    static inline void on_first_pass(const selector_type* selector_ptr, local_data_type& local_data, size_t line)
    {
        if (XTL_LIKELY(local_data.switch_info_ptr->line == 0)) 
        {
            local_data.switch_info_ptr->line   = line; 
            local_data.switch_info_ptr->offset = intptr_t(local_data.casted_ptr)-intptr_t(selector_ptr);
        } 
    }
    
    /// Function that will be called when the fall-through behavior reached end of the switch
    static inline void on_end(const selector_type* selector_ptr, local_data_type& local_data, size_t line)
    {
        if (XTL_LIKELY(local_data.switch_info_ptr->line == 0)) 
        { 
            local_data.switch_info_ptr->line   = line;
        }
    }

    /// Function that will be called on default clause. It should return true 
    /// when unconditional jump to ReMatch label should be performed.
    static inline bool on_default(size_t&, local_data_type&, static_data_type&) { return false; }

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
            typedef T target_type;

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

            /// Layout that has to be used for the given target type.
            enum { layout = default_layout };

            /// Condition that guards applicability of the given case clause
            /// during the fall-through behavior.
            static inline bool main_condition(const selector_type* selector_ptr, local_data_type& local_data)
            {
                return (local_data.casted_ptr = dynamic_cast<const target_type*>(selector_ptr)) != 0;
            }

            /// Performs the necessary conversion of the original selector into the proper
            /// object of target type.
            /// \note The selector is const-qualified, thus the target is also const-qualified
            static inline const target_type* get_matched(const selector_type* selector_ptr, local_data_type& local_data)
            {
                //std::cout << "Open case (const)" << std::endl;
                return adjust_ptr<target_type>(selector_ptr,local_data.switch_info_ptr->offset);
            }

            /// Performs the necessary conversion of the original selector into the proper
            /// object of target type.
            /// \note The selector is non-const, thus the target is also non-const
            static inline       target_type* get_matched(      selector_type* selector_ptr, local_data_type& local_data)
            {
                //std::cout << "Open case (non-const)" << std::endl;
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
    typedef std::vector<const int*> static_data_type;

    /// Type of data that has to be automatically allocated inside the block 
    /// containg extended switch
    struct local_data_type
    {
        local_data_type() : kinds(0)/*, attempt(0)*/ {} // NOTE: attempt is not intialized on purpose for performance reasons. It will be initialized before use in on_default.
        const int* kinds;
        size_t     attempt;
    };

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
            exit = 0 ///< Case label that will be used to jump to the end of the switch
        };
    };

    /// Function used to get the value we'll be switching on
    static inline size_t choose(const selector_type* selector_ptr, static_data_type& static_data, local_data_type& local_data)
    {
        return size_t(original2remapped<selector_type>((int)apply_member(selector_ptr, match_members<selector_type>::kind_selector())));
    }

    /// Function that will be called upon first entry to the case through the fall-through behavior
    static inline void on_first_pass(const selector_type*, local_data_type&, size_t) {}

    /// Function that will be called when the fall-through behavior reached end of the switch
    static inline void on_end(const selector_type*, local_data_type&, size_t) {}

    /// Function that will be called on default clause. It should return true 
    /// when unconditional jump to ReMatch label should be performed.
    static inline bool on_default(size_t& jump_target, local_data_type& local_data, static_data_type& static_data)
    {
        if (XTL_LIKELY(!local_data.kinds))
        {
            local_data.attempt = 0;
            if (XTL_UNLIKELY(jump_target >= static_data.size()))
                static_data.resize(jump_target+1);
            if (!(local_data.kinds = static_data[jump_target]))
                local_data.kinds = static_data[jump_target] = get_kinds<selector_type>(jump_target);
        }
        XTL_ASSERT(("Base classes for this kind were not specified",local_data.kinds));
        XTL_ASSERT(("Invalid list of kinds",local_data.kinds[local_data.attempt]==jump_target));
        jump_target = local_data.kinds[++local_data.attempt];
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
            typedef selector_type target_type;

            /// Depending on whether we handle open or closed case, different case labels
            /// are used for the generated match statement. This metafunction takes
            /// a unique (per match statement) counter and returns the actual label that
            /// will be used for the case clause.
            template <size_t Counter>
            struct CaseLabel
            {
                enum 
                {
                    value = remapped_kind<target_type,N>::value ///< Case label that will be used for case at line offset Counter
                };
            };
            
            /// Layout that has to be used for the given target type.
            enum { layout = N };

            /// Condition that guards applicability of the given case clause
            /// during the fall-through behavior.
            static inline bool main_condition(const selector_type* selector_ptr, local_data_type& local_data) { return true; }

            /// Performs the necessary conversion of the original selector into the proper
            /// object of target type.
            /// \note The selector is const-qualified, thus the target is also const-qualified
            static inline const target_type* get_matched(const selector_type* selector_ptr, local_data_type& local_data)
            {
                //std::cout << "Union case (const)" << std::endl;
                return selector_ptr;
            }

            /// Performs the necessary conversion of the original selector into the proper
            /// object of target type.
            /// \note The selector is non-const, thus the target is also non-const
            static inline       target_type* get_matched(      selector_type* selector_ptr, local_data_type& local_data)
            {
                //std::cout << "Union case (non-const)" << std::endl;
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
            typedef T target_type;

            /// Depending on whether we handle open or closed case, different case labels
            /// are used for the generated match statement. This metafunction takes
            /// a unique (per match statement) counter and returns the actual label that
            /// will be used for the case clause.
            template <size_t Counter>
            struct CaseLabel
            {
                enum 
                {
                    value = remapped_kind<target_type>::value ///< Case label that will be used for case at line offset Counter
                };
            };

            /// Layout that has to be used for the given target type.
            enum { layout = default_layout };

            /// Condition that guards applicability of the given case clause
            /// during the fall-through behavior.
            static inline bool main_condition(const selector_type* selector_ptr, local_data_type& local_data) { return true; }

            /// Performs the necessary conversion of the original selector into the proper
            /// object of target type.
            /// \note The selector is const-qualified, thus the target is also const-qualified
            static inline const target_type* get_matched(const selector_type* selector_ptr, local_data_type& local_data)
            {
                //std::cout << "Closed case (const)" << std::endl;
                return stat_cast<target_type>(selector_ptr);
            }

            /// Performs the necessary conversion of the original selector into the proper
            /// object of target type.
            /// \note The selector is non-const, thus the target is also non-const
            static inline       target_type* get_matched(      selector_type* selector_ptr, local_data_type& local_data)
            {
                //std::cout << "Closed case (non-const)" << std::endl;
                return stat_cast<target_type>(selector_ptr);
            }
        };
    };
};

//------------------------------------------------------------------------------

template <class T>
struct variable;

//------------------------------------------------------------------------------

template <typename T, typename E>
struct guard
{
    guard(const variable<T>& v, const E& e) : m_v(v), m_e(e) {}
    guard(const variable<T>& v, E&& e) : m_v(v), m_e(e) {}
    guard(guard&& g) : m_v(g.m_v), m_e(std::move(g.m_e)) {}
    template <typename U>
    bool operator()(const U& u) const { return m_v(u) && eval(m_e); }
    const variable<T>& m_v;
    const E            m_e;
};

//------------------------------------------------------------------------------

/// A reference to a user provided variable
template <class T>
struct var_ref
{
    var_ref(T& var) : m_var(&var) {}
    var_ref(var_ref&& v) : m_var(v.m_var) {}

    typedef T result_type;
    operator result_type() const throw() { return *m_var; }

    /// We report that matching succeeded and bind the value
    bool operator()(const T& t) const 
    {
        *m_var = t;
        return true;
    }

    /// Member that will hold matching value in case of successful matching
    T* m_var;
};

/// A reference to a user provided variable
template <class T>
struct var_ref<variable<T> >
{
    var_ref(variable<T>& var) : m_var(&var) {}
    var_ref(var_ref&& v) : m_var(v.m_var) {}

    typedef T result_type;
    operator result_type() const throw() { return *m_var; }

    /// We report that matching succeeded and bind the value
    bool operator()(const T& t) const 
    {
        *m_var = t;
        return true;
    }

    /// Member that will hold matching value in case of successful matching
    variable<T>* m_var;
};

//------------------------------------------------------------------------------

/// Variable binding for a value type
template <class T>
struct variable
{
    variable() : m_value() {}
    variable(const T& t) : m_value(t) {}
    variable(T&& t) : m_value(std::move(t)) {}
    variable(variable&& v) : m_value(std::move(v.m_value)) {}

    typedef T result_type;

    /// We report that matching succeeded and bind the value
    bool operator()(const T& t) const
    {
        m_value = t;
        return true;
    }

    variable& operator=(const T& t) { m_value = t; return *this; }

    template <typename E> 
    auto operator|=(E&& e) -> guard<T,decltype(filter(std::forward<E>(e)))>
    { 
        return guard<T,decltype(filter(std::forward<E>(e)))>(*this,filter(std::forward<E>(e))); 
    }

    /// Helper conversion operator to let the variable be used in some places
    /// where T was allowed
    operator const T&() const throw() { return m_value; }

    /// Member that will hold matching value in case of successful matching
    mutable T m_value;
};

//------------------------------------------------------------------------------

/// Variable binding for a pointer type
template <class T>
struct variable<const T*>
{
    variable() : m_value() {}
    variable(variable&& v) : m_value(std::move(v.m_value)) {}

    typedef const T* result_type;

    /// We may be applied to a value of a base type, so first we have to figure
    /// out whether they dynamic type is actually T. We report match only if it is
    template <typename U>
    bool operator()(const U* u) const 
    {
        if (const T* t = dynamic_cast<const T*>(u))
        {
            m_value = t; 
            return true;
        }
        else
            return false;
    }

    template <typename E> 
    auto operator|=(E&& e) -> guard<const T*,decltype(filter(std::forward<E>(e)))>
    { 
        return guard<const T*,decltype(filter(std::forward<E>(e)))>(*this,filter(std::forward<E>(e))); 
    }


    /// This distinguishes the case when type of the variable matches type of the member
    bool operator()(const T* t) const 
    {
        // NOTE: This will also assign the null pointer. Should it?
        m_value = t; 
        return true;
    }

    /// Helper conversion operator to let the variable be used in some places
    /// where T* is expected
    operator const T*() const throw() { return m_value; }

    /// A helper member to let the wrapping variable be used as a pointer
    const T* operator->() const throw() { return m_value; }

    /// A helper member to let the wrapping variable be used as a pointer
    const T& operator*()  const throw() { return *m_value; }

    /// Member that will hold matching value in case of successful matching
    mutable const T* m_value;
};

//------------------------------------------------------------------------------

/// Variable binding for a pointer type
template <class T>
struct variable<const T&>
{
    variable() : m_value() {}
    variable(variable&& v) : m_value(std::move(v.m_value)) {}

    typedef const T& result_type;

    /// We may be applied to a value of a base type, so first we have to figure
    /// out whether they dynamic type is actually T. We report match only if it is
    template <typename U>
    bool operator()(const U& u) const 
    {
        if (const T* t = dynamic_cast<const T*>(&u))
        {
            m_value = t; 
            return true;
        }
        else
            return false;
    }

    template <typename E> 
    auto operator|=(E&& e) -> guard<const T&,decltype(filter(std::forward<E>(e)))>
    { 
        return guard<const T&,decltype(filter(std::forward<E>(e)))>(*this,filter(std::forward<E>(e))); 
    }

    /// This distinguishes the case when type of the variable matches type of the member
    bool operator()(const T& t) const 
    {
        // NOTE: This will also assign the null pointer. Should it?
        m_value = &t;
        return true;
    }

    /// Helper conversion operator to let the variable be used in some places
    /// where T was allowed
    operator const T&() const throw() { XTL_ASSERT(m_value); return *m_value; }

    /// Member that will hold matching value in case of successful matching
    mutable const T* m_value;
};

//------------------------------------------------------------------------------

/// Class that describes a meta variable that matches everything.
/// This variable will match any type returned by a member, but the main 
/// difference from just using a variable whose value is ignored is that 
/// use of this variable will make sure the actual member is never invoked!
struct wildcard
{
    //typedef void result_type;

    // NOTE: We don't need the below application anymore since we have a 
    //       specialization that never applies the actual member before
    //       passing it to this meta variable that matches everything.
    //template <typename U>
    //bool operator()(const U& u) const throw() { return true; }
};

//------------------------------------------------------------------------------

template <typename T>
std::ostream& operator<<(std::ostream& os, const variable<T>& v) { return os << v.m_value; }

template <typename T>
std::ostream& operator<<(std::ostream& os, const variable<T*>& v) { return os << *v.m_value; }

//------------------------------------------------------------------------------

template <class T>
struct value
{
    typedef T result_type;
    value(const T& t) : m_value(t) {}
    value(T&& t) : m_value(std::move(t)) {}
    value(value&& v) : m_value(std::move(v.m_value)) {}
    bool operator()(const T& t) const throw() { return m_value == t; }
    operator result_type() const throw() { return m_value; }
    T m_value;
};

template <class T> inline value<T> val(const T& t) { return value<T>(t); }

//------------------------------------------------------------------------------

template <typename F, typename E1 = void, typename E2 = void>
struct expr;

template <typename F, typename E1>
struct expr<F,E1>
{
    expr(const E1& e1) : m_e1(e1) {}
    expr(E1&& e1) : m_e1(std::move(e1)) {}
    expr(expr&& e) : m_e1(std::move(e.m_e1)) {}
    typedef typename std::remove_const<decltype(F()(std::declval<typename E1::result_type>()))>::type result_type; // We needed to add remove_const here as MSVC was returning const T
    operator result_type() const { return eval(*this); }
    bool operator()(const result_type& t) const 
    {
        typename E1::result_type v;
        return solver<F>()(v,t) && m_e1(v);
    }
    const E1 m_e1;
};

template <typename F, typename E1, typename E2>
struct expr
{
    expr(const E1& e1, const E2& e2) : m_e1(e1), m_e2(e2) {}
    expr(E1&& e1, E2&& e2) : m_e1(std::move(e1)), m_e2(std::move(e2)) {}
    expr(expr&& e) : m_e1(std::move(e.m_e1)), m_e2(std::move(e.m_e2)) {}
    typedef typename std::remove_const<decltype(F()(std::declval<typename E1::result_type>(),std::declval<typename E2::result_type>()))>::type result_type; // We needed to add remove_const here as MSVC was returning const T
    operator result_type() const { return eval(*this); }
    bool operator()(const result_type& t) const 
    {
        typename E1::result_type v;
        return solver<F>()(v,eval(m_e2),t) && m_e1(v);
    }
    const E1 m_e1;
    const E2 m_e2;
};

//------------------------------------------------------------------------------

template <typename E1, typename E2>
struct expr_or
{
    expr_or(const E1& e1, const E2& e2) : m_e1(e1), m_e2(e2) {}
    expr_or(E1&& e1, E2&& e2) : m_e1(std::move(e1)), m_e2(std::move(e2)) {}
    expr_or(expr_or&& e) : m_e1(std::move(e.m_e1)), m_e2(std::move(e.m_e2)) {}
    template <typename U> bool operator()(const U* u) const { return m_e1(u) || m_e2(u); }
    template <typename U> bool operator()(      U* u) const { return m_e1(u) || m_e2(u); }
    template <typename U> bool operator()(const U& u) const { return operator()(&u); }
    template <typename U> bool operator()(      U& u) const { return operator()(&u); }
    const E1 m_e1;
    const E2 m_e2;
};

//------------------------------------------------------------------------------

template <typename T, size_t layout, typename E1, typename E2, typename E3, typename E4> struct matcher4;
template <typename T, size_t layout, typename E1, typename E2, typename E3>              struct matcher3;
template <typename T, size_t layout, typename E1, typename E2>                           struct matcher2;
template <typename T, size_t layout, typename E1>                                        struct matcher1;
template <typename T, size_t layout>                                                     struct matcher0;

//------------------------------------------------------------------------------

template <typename T> inline var_ref<T> filter(T& t)        { return var_ref<T>(t); }
template <typename T> inline value<T>   filter(const T& t)  { return value<T>(t); }

template <typename T, size_t layout, typename E1, typename E2, typename E3, typename E4>
matcher4<T,layout,E1,E2,E3,E4> inline filter(const matcher4<T,layout,E1,E2,E3,E4>& t) { return t; }
template <typename T, size_t layout, typename E1, typename E2, typename E3>
matcher3<T,layout,E1,E2,E3> inline filter(const matcher3<T,layout,E1,E2,E3>& t) { return t; }
template <typename T, size_t layout, typename E1, typename E2>
matcher2<T,layout,E1,E2> inline filter(const matcher2<T,layout,E1,E2>& t) { return t; }
template <typename T, size_t layout, typename E1>
matcher1<T,layout,E1> inline filter(const matcher1<T,layout,E1>& t) { return t; }
template <typename T, size_t layout>
matcher0<T,layout> inline filter(const matcher0<T,layout>& t) { return t; }

template <typename F, typename E1, typename E2>
expr<F,E1,E2> inline filter(const expr<F,E1,E2>& t) { return t; }
template <typename F, typename E1>
expr<F,E1> inline filter(const expr<F,E1>& t) { return t; }
template <typename F>
expr<F> inline filter(const expr<F>& t) { return t; }
template <typename T, typename E>
guard<T,E> inline filter(const guard<T,E>& t) { return t; }

//------------------------------------------------------------------------------

// Define all unary operators on variable
#define FOR_EACH_UNARY_OPERATOR(F,S) \
    template <typename T> \
    inline auto XTL_CONCATENATE(operator,S)(variable<T>& v)\
          -> expr<F,var_ref<variable<T> > >\
    { return expr<F,var_ref<variable<T> > >(var_ref<variable<T> >(v)); }
// Define all binary operators with first argument being variable and second whatever
#define FOR_EACH_BINARY_OPERATOR(F,S) \
    template <typename T, typename E> \
    inline auto XTL_CONCATENATE(operator,S)(variable<T>& v, E&& e)\
          -> expr<F,var_ref<variable<T> >,decltype(filter(std::forward<E>(e)))>\
    { return expr<F,var_ref<variable<T> >,decltype(filter(std::forward<E>(e)))>(var_ref<variable<T> >(v),filter(std::forward<E>(e))); }
#include "loop_over_operators.hpp"
#undef  FOR_EACH_BINARY_OPERATOR
#undef  FOR_EACH_UNARY_OPERATOR

// Define all unary operators on value
#define FOR_EACH_UNARY_OPERATOR(F,S) \
    template <typename T> \
    inline auto XTL_CONCATENATE(operator,S)(const value<T>& v)\
          -> expr<F,value<T> >\
    { return expr<F,value<T> >(v); }
// Define all binary operators with first argument being value and second whatever
#define FOR_EACH_BINARY_OPERATOR(F,S) \
    template <typename T, typename E> \
    inline auto XTL_CONCATENATE(operator,S)(const value<T>& v, E&& e)\
          -> expr<F,value<T>,decltype(filter(std::forward<E>(e)))>\
    { return expr<F,value<T>,decltype(filter(std::forward<E>(e)))>(v,filter(std::forward<E>(e))); }
#include "loop_over_operators.hpp"
#undef  FOR_EACH_BINARY_OPERATOR
#undef  FOR_EACH_UNARY_OPERATOR

// Define all unary operators on unary expressions
#define FOR_EACH_UNARY_OPERATOR(F,S) \
    template <typename F1, typename E1> \
    inline auto XTL_CONCATENATE(operator,S)(const expr<F1,E1>& v)\
          -> expr<F,expr<F1,E1> >\
    { return expr<F,expr<F1,E1> >(v); }
// Define all binary operators with first argument being unary expression and second whatever
#define FOR_EACH_BINARY_OPERATOR(F,S) \
    template <typename F1, typename E1, typename E> \
    inline auto XTL_CONCATENATE(operator,S)(const expr<F1,E1>& v, E&& e)\
          -> expr<F,expr<F1,E1>,decltype(filter(std::forward<E>(e)))>\
    { return expr<F,expr<F1,E1>,decltype(filter(std::forward<E>(e)))>(v,filter(std::forward<E>(e))); }
#include "loop_over_operators.hpp"
#undef  FOR_EACH_BINARY_OPERATOR
#undef  FOR_EACH_UNARY_OPERATOR

// Define all unary operators on binary expressions
#define FOR_EACH_UNARY_OPERATOR(F,S) \
    template <typename F1, typename E1, typename E2> \
    inline auto XTL_CONCATENATE(operator,S)(const expr<F1,E1,E2>& v)\
          -> expr<F,expr<F1,E1,E2> >\
    { return expr<F,expr<F1,E1,E2> >(v); }
// Define all binary operators with first argument being binary expression and second whatever
#define FOR_EACH_BINARY_OPERATOR(F,S) \
    template <typename F1, typename E1, typename E2, typename E> \
    inline auto XTL_CONCATENATE(operator,S)(const expr<F1,E1,E2>& v, E&& e)\
          -> expr<F,expr<F1,E1,E2>,decltype(filter(std::forward<E>(e)))>\
    { return expr<F,expr<F1,E1,E2>,decltype(filter(std::forward<E>(e)))>(v,filter(std::forward<E>(e))); }
#include "loop_over_operators.hpp"
#undef  FOR_EACH_BINARY_OPERATOR
#undef  FOR_EACH_UNARY_OPERATOR

//------------------------------------------------------------------------------

template <typename T> T inline eval(const value<T>& e)    { return e.m_value; }
template <typename T> T inline eval(const variable<T>& e) { return e.m_value; }
template <typename T> T inline eval(const var_ref<T>& e)  { return *e.m_var; }
template <typename T> T inline eval(const var_ref<variable<T> >& e) { return e.m_var->m_value; }
template <typename F, typename E1>              typename expr<F,E1>::result_type    inline eval(const expr<F,E1>&    e) { return F()(eval(e.m_e1)); }
template <typename F, typename E1, typename E2> typename expr<F,E1,E2>::result_type inline eval(const expr<F,E1,E2>& e) { return F()(eval(e.m_e1),eval(e.m_e2)); }

//------------------------------------------------------------------------------

template <typename E>                           struct is_const_expr                { static const bool value = false; };
template <typename T>                           struct is_const_expr<value<T>>      { static const bool value = true; };
template <typename F, typename E1>              struct is_const_expr<expr<F,E1>>    { static const bool value = is_const_expr<E1>::value; };
template <typename F, typename E1, typename E2> struct is_const_expr<expr<F,E1,E2>> { static const bool value = is_const_expr<E1>::value && is_const_expr<E2>::value; };

//------------------------------------------------------------------------------

#define XTL_DEBUG_APPLY_MEMBER(what, c, f) //XTL_DEBUG_ONLY(std::clog << "\nApplying " what << c << " of type " << typeid(*c).name() << std::endl)

//------------------------------------------------------------------------------

template <class C, class T, typename R>
inline R apply_member(const C* c, R (T::*method)() const)
{
    XTL_DEBUG_APPLY_MEMBER("const member function to const instance ", c, method);
    return (c->*method)();
}

//------------------------------------------------------------------------------

template <class C, class T, typename R>
inline R apply_member(      C* c, R (T::*method)() const)
{
    XTL_DEBUG_APPLY_MEMBER("const member function to non-const instance ", c, method);
    return (c->*method)();
}

//------------------------------------------------------------------------------

template <class C, class T, typename R>
inline R apply_member(      C* c, R (T::*method)()      )
{
    XTL_DEBUG_APPLY_MEMBER("non-const member function to non-const instance ", c, method);
    return (c->*method)();
}

//------------------------------------------------------------------------------

template <class C, class T, typename R>
inline const R& apply_member(const C* c, R T::*field) throw()
{
    XTL_DEBUG_APPLY_MEMBER("data member to const instance ", c, field);
    return c->*field;
}

//------------------------------------------------------------------------------

template <class C, class T, typename R>
inline       R& apply_member(      C* c, R T::*field) throw()
{
    XTL_DEBUG_APPLY_MEMBER("data member to non-const instance ", c, field);
    return c->*field;
}

//------------------------------------------------------------------------------

template <class C, class T, typename R>
inline R apply_member(const C* c, R (*func)(const T*))
{
    XTL_DEBUG_APPLY_MEMBER("external function taking const pointer to const instance ", c, func);
    return (*func)(c);
}

//------------------------------------------------------------------------------

template <class C, class T, typename R>
inline R apply_member(      C* c, R (*func)(const T*))
{
    XTL_DEBUG_APPLY_MEMBER("external function taking const pointer to non-const instance ", c, func);
    return (*func)(c);
}

//------------------------------------------------------------------------------

template <class C, class T, typename R>
inline R apply_member(      C* c, R (*func)(      T*))
{
    XTL_DEBUG_APPLY_MEMBER("external function taking non-const pointer to non-const instance ", c, func);
    return (*func)(c);
}

//------------------------------------------------------------------------------

template <class C, class T, typename R>
inline R apply_member(const C* c, R (*func)(const T&))
{
    XTL_DEBUG_APPLY_MEMBER("external function taking const reference to const instance ", c, func);
    return (*func)(*c);
}

//------------------------------------------------------------------------------

template <class C, class T, typename R>
inline R apply_member(      C* c, R (*func)(const T&))
{
    XTL_DEBUG_APPLY_MEMBER("external function taking const reference to non-const instance ", c, func);
    return (*func)(*c);
}

//------------------------------------------------------------------------------

template <class C, class T, typename R>
inline R apply_member(      C* c, R (*func)(      T&))
{
    XTL_DEBUG_APPLY_MEMBER("external function taking non-const reference to non-const instance ", c, func);
    return (*func)(*c);
}

//------------------------------------------------------------------------------

/// We need this extra indirection to be able to intercept when we are trying to 
/// match a meta variable _ of type wildcard, that matches everything of
/// any type. In this case we don't even want to invoke the underlain member!
template <typename E, typename C, typename M>
inline bool apply_expression(const E& e, const C* c, M m)
{
    #ifdef _MSC_VER
    #pragma warning( disable : 4800 )
    #endif

    return e(apply_member(c, m));
}

template <typename E, typename C, typename M>
inline bool apply_expression(const E& e,       C* c, M m)
{
    #ifdef _MSC_VER
    #pragma warning( disable : 4800 )
    #endif

    return e(apply_member(c, m));
}

/// This is the specialization that makes the member not to be invoked when we
/// are matching against the meta variable _ that matches everything.
template <typename C, typename M>
inline bool apply_expression(const var_ref<wildcard>&, const C*, M)
{
    return true;
}

template <typename C, typename M>
inline bool apply_expression(const var_ref<wildcard>&,       C*, M) throw()
{
    return true;
}

//------------------------------------------------------------------------------

template <typename T, size_t layout>
struct matcher0
{
    const T* operator()(const T* t) const throw() { return t; }
          T* operator()(      T* t) const throw() { return t; }
    template <typename U> const T* operator()(const U* u) const throw() { return dynamic_cast<const T*>(u); }
    template <typename U>       T* operator()(      U* u) const throw() { return dynamic_cast<      T*>(u); }
    template <typename U> const T* operator()(const U& u) const throw() { return operator()(&u); }
    template <typename U>       T* operator()(      U& u) const throw() { return operator()(&u); }

    template <typename E> expr_or<matcher0,E> operator|(const E& e) const throw() { return expr_or<matcher0,E>(*this,e); }
};

//------------------------------------------------------------------------------

template <typename T, size_t layout, typename E1>
struct matcher1
{
    matcher1(const E1& e1) : m_e1(e1) {}
    matcher1(E1&& e1) : m_e1(std::move(e1)) {}
    matcher1(matcher1&& src) : m_e1(std::move(src.m_e1)) {}
    const T* operator()(const T* t) const 
    {
        return 
            (t                                                             &&
             apply_expression(m_e1, t, match_members<T,layout>::member0())
            )
            ? t
            : 0;
    }
          T* operator()(      T* t) const 
    {
        return 
            (t                                                             &&
             apply_expression(m_e1, t, match_members<T,layout>::member0())
            )
            ? t
            : 0;
    }

    template <typename U> const T* operator()(const U* u) const { return operator()(dynamic_cast<const T*>(u)); }
    template <typename U>       T* operator()(      U* u) const { return operator()(dynamic_cast<      T*>(u)); }
    template <typename U> const T* operator()(const U& u) const { return operator()(&u); }
    template <typename U>       T* operator()(      U& u) const { return operator()(&u); }

    template <typename E> expr_or<matcher1,E> operator|(const E& e) const throw() { return expr_or<matcher1,E>(*this,e); }
    const E1 m_e1;
};

//------------------------------------------------------------------------------

template <typename T, size_t layout, typename E1, typename E2>
struct matcher2
{
    matcher2(const E1& e1, const E2& e2) : m_e1(e1), m_e2(e2) {}
    matcher2(E1&& e1, E2&& e2) : m_e1(std::move(e1)), m_e2(std::move(e2)) {}
    matcher2(matcher2&& src) : m_e1(std::move(src.m_e1)), m_e2(std::move(src.m_e2)) {}

    const T* operator()(const T* t) const 
    {
        return 
            (t                                                             &&
             apply_expression(m_e1, t, match_members<T,layout>::member0()) &&
             apply_expression(m_e2, t, match_members<T,layout>::member1())
            )
            ? t
            : 0;
    }
          T* operator()(      T* t) const 
    {
        return 
            (t                                                             &&
             apply_expression(m_e1, t, match_members<T,layout>::member0()) &&
             apply_expression(m_e2, t, match_members<T,layout>::member1())
            )
            ? t
            : 0;
    }

    template <typename U> const T* operator()(const U* u) const { return operator()(dynamic_cast<const T*>(u)); }
    template <typename U>       T* operator()(      U* u) const { return operator()(dynamic_cast<      T*>(u)); }
    template <typename U> const T* operator()(const U& u) const { return operator()(&u); }
    template <typename U>       T* operator()(      U& u) const { return operator()(&u); }

    template <typename E> expr_or<matcher2,E> operator|(const E& e) const throw() { return expr_or<matcher2,E>(*this,e); }
    const E1 m_e1;
    const E2 m_e2;
};

//------------------------------------------------------------------------------

template <typename T, size_t layout, typename E1, typename E2, typename E3>
struct matcher3
{
    matcher3(const E1& e1, const E2& e2, const E3& e3) : m_e1(e1), m_e2(e2), m_e3(e3) {}
    matcher3(E1&& e1, E2&& e2, E3&& e3) : m_e1(std::move(e1)), m_e2(std::move(e2)), m_e3(std::move(e3)) {}
    matcher3(matcher3&& src) : m_e1(std::move(src.m_e1)), m_e2(std::move(src.m_e2)), m_e3(std::move(src.m_e3)) {}

    const T* operator()(const T* t) const 
    {
        return 
            (t                                                             &&
             apply_expression(m_e1, t, match_members<T,layout>::member0()) &&
             apply_expression(m_e2, t, match_members<T,layout>::member1()) &&
             apply_expression(m_e3, t, match_members<T,layout>::member2())
            )
            ? t
            : 0;
    }
          T* operator()(      T* t) const 
    {
        return 
            (t                                                             &&
             apply_expression(m_e1, t, match_members<T,layout>::member0()) &&
             apply_expression(m_e2, t, match_members<T,layout>::member1()) &&
             apply_expression(m_e3, t, match_members<T,layout>::member2())
            )
            ? t
            : 0;
    }

    template <typename U> const T* operator()(const U* u) const { return operator()(dynamic_cast<const T*>(u)); }
    template <typename U>       T* operator()(      U* u) const { return operator()(dynamic_cast<      T*>(u)); }
    template <typename U> const T* operator()(const U& u) const { return operator()(&u); }
    template <typename U>       T* operator()(      U& u) const { return operator()(&u); }

    template <typename E> expr_or<matcher3,E> operator|(const E& e) const throw() { return expr_or<matcher3,E>(*this,e); }
    const E1 m_e1;
    const E2 m_e2;
    const E3 m_e3;
};

//------------------------------------------------------------------------------

template <typename T, size_t layout, typename E1, typename E2, typename E3, typename E4>
struct matcher4
{
    matcher4(const E1& e1, const E2& e2, const E3& e3, const E4& e4) : m_e1(e1), m_e2(e2), m_e3(e3), m_e4(e4) {}
    matcher4(E1&& e1, E2&& e2, E3&& e3, E4&& e4) : m_e1(std::move(e1)), m_e2(std::move(e2)), m_e3(std::move(e3)), m_e4(std::move(e4)) {}
    matcher4(matcher4&& src) : m_e1(std::move(src.m_e1)), m_e2(std::move(src.m_e2)), m_e3(std::move(src.m_e3)), m_e4(std::move(src.m_e4)) {}

    const T* operator()(const T* t) const 
    {
        return 
            (t                                                             &&
             apply_expression(m_e1, t, match_members<T,layout>::member0()) &&
             apply_expression(m_e2, t, match_members<T,layout>::member1()) &&
             apply_expression(m_e3, t, match_members<T,layout>::member2()) &&
             apply_expression(m_e4, t, match_members<T,layout>::member3())
            )
            ? t
            : 0;
    }
          T* operator()(      T* t) const 
    {
        return 
            (t                                                             &&
             apply_expression(m_e1, t, match_members<T,layout>::member0()) &&
             apply_expression(m_e2, t, match_members<T,layout>::member1()) &&
             apply_expression(m_e3, t, match_members<T,layout>::member2()) &&
             apply_expression(m_e4, t, match_members<T,layout>::member3())
            )
            ? t
            : 0;
    }

    template <typename U> const T* operator()(const U* u) const { return operator()(dynamic_cast<const T*>(u)); }
    template <typename U>       T* operator()(      U* u) const { return operator()(dynamic_cast<      T*>(u)); }
    template <typename U> const T* operator()(const U& u) const { return operator()(&u); }
    template <typename U>       T* operator()(      U& u) const { return operator()(&u); }

    template <typename E> expr_or<matcher4,E> operator|(const E& e) const throw() { return expr_or<matcher4,E>(*this,e); }
    const E1 m_e1;
    const E2 m_e2;
    const E3 m_e3;
    const E4 m_e4;
};

//------------------------------------------------------------------------------

/// 0-argument version of a helper function to @match that accepts arguments that
/// have been already preprocessed with @filter to convert regular variables into
/// @var_ref and constants into @value.
/// \note This version will be called from @match with a non-@view target type
template <typename T, size_t layout>
inline matcher0<T,layout> match_ex(const view<T,layout>&) throw()
{
    return matcher0<T,layout>();
}

/// 0-argument version of a helper function to @match that accepts arguments that
/// have been already preprocessed with @filter to convert regular variables into
/// @var_ref and constants into @value.
/// \note This version will be called from @match that had its target type a @view
template <typename T, size_t layout>
inline matcher0<T,layout> match_ex(const view<view<T,layout>>&) throw()
{
    return matcher0<T,layout>();
}

/// A 0-argument version of a tree-pattern constructor. Target type is allowed 
/// to be a @view here.
template <typename T>
inline auto match() throw() -> XTL_RETURN(match_ex(view<T>()))

/// A 0-argument version of a tree-pattern constructor that takes layout in 
/// addition to the target type.
/// \note @view is not supposed to be passed as a target type to this version
///       of the function because we will then have two potentially conflicting
///       layouts. Any layout different from @default_layout passed here will 
///       result in a compile time error.
template <typename T, size_t layout>
inline auto match() throw() -> XTL_RETURN(match_ex(view<T,layout>()))

//------------------------------------------------------------------------------

/// 1-argument version of a helper function to @match that accepts arguments that
/// have been already preprocessed with @filter to convert regular variables into
/// @var_ref and constants into @value.
/// \note This version will be called from @match with a non-@view target type
template <typename T, size_t layout, typename E1>
inline matcher1<T,layout,E1> match_ex(const view<T,layout>&, E1&& e1) throw()
{
    return matcher1<T,layout,E1>(std::forward<E1>(e1));
}

/// 1-argument version of a helper function to @match that accepts arguments that
/// have been already preprocessed with @filter to convert regular variables into
/// @var_ref and constants into @value.
/// \note This version will be called from @match that had its target type a @view
template <typename T, size_t layout, typename E1>
inline matcher1<T,layout,E1> match_ex(const view<view<T,layout>>&, E1&& e1) throw()
{
    return matcher1<T,layout,E1>(std::forward<E1>(e1));
}

/// A 1-argument version of a tree-pattern constructor. Target type is allowed 
/// to be a @view here.
template <typename T, typename E1>
inline auto match(E1&& e1) throw() -> XTL_RETURN
(
    match_ex(
        view<T>(),
        filter(std::forward<E1>(e1))
    )
)

/// A 1-argument version of a tree-pattern constructor that takes layout in 
/// addition to the target type.
/// \note @view is not supposed to be passed as a target type to this version
///       of the function because we will then have two potentially conflicting
///       layouts. Any layout different from @default_layout passed here will 
///       result in a compile time error.
template <typename T, size_t layout, typename E1>
inline auto match(E1&& e1) throw() -> XTL_RETURN
(
    match_ex(
        view<T,layout>(),
        filter(std::forward<E1>(e1))
    )
)

//------------------------------------------------------------------------------

/// 2-argument version of a helper function to @match that accepts arguments that
/// have been already preprocessed with @filter to convert regular variables into
/// @var_ref and constants into @value.
/// \note This version will be called from @match with a non-@view target type
template <typename T, size_t layout, typename E1, typename E2>
inline matcher2<T,layout,E1,E2> match_ex(const view<T,layout>&, E1&& e1, E2&& e2) throw()
{
    return matcher2<T,layout,E1,E2>(
            std::forward<E1>(e1),
            std::forward<E2>(e2)
           );
}

/// 2-argument version of a helper function to @match that accepts arguments that
/// have been already preprocessed with @filter to convert regular variables into
/// @var_ref and constants into @value.
/// \note This version will be called from @match that had its target type a @view
template <typename T, size_t layout, typename E1, typename E2>
inline matcher2<T,layout,E1,E2> match_ex(const view<view<T,layout>>&, E1&& e1, E2&& e2) throw()
{
    return matcher2<T,layout,E1,E2>(
            std::forward<E1>(e1),
            std::forward<E2>(e2)
           );
}

/// A 2-argument version of a tree-pattern constructor. Target type is allowed 
/// to be a @view here.
template <typename T, typename E1, typename E2>
inline auto match(E1&& e1, E2&& e2) throw() -> XTL_RETURN
(
    match_ex(
        view<T>(),
        filter(std::forward<E1>(e1)), 
        filter(std::forward<E2>(e2))
    )
)

/// A 2-argument version of a tree-pattern constructor that takes layout in 
/// addition to the target type.
/// \note @view is not supposed to be passed as a target type to this version
///       of the function because we will then have two potentially conflicting
///       layouts. Any layout different from @default_layout passed here will 
///       result in a compile time error.
template <typename T, size_t layout, typename E1, typename E2>
inline auto match(E1&& e1, E2&& e2) throw() -> XTL_RETURN
(
    match_ex(
        view<T,layout>(),
        filter(std::forward<E1>(e1)), 
        filter(std::forward<E2>(e2))
    )
)

//------------------------------------------------------------------------------

/// 3-argument version of a helper function to @match that accepts arguments that
/// have been already preprocessed with @filter to convert regular variables into
/// @var_ref and constants into @value.
/// \note This version will be called from @match with a non-@view target type
template <typename T, size_t layout, typename E1, typename E2, typename E3>
inline matcher3<T,layout,E1,E2,E3> match_ex(const view<T,layout>&, E1&& e1, E2&& e2, E3&& e3) throw()
{
    return matcher3<T,layout,E1,E2,E3>(
            std::forward<E1>(e1),
            std::forward<E2>(e2),
            std::forward<E3>(e3)
           );
}

/// 3-argument version of a helper function to @match that accepts arguments that
/// have been already preprocessed with @filter to convert regular variables into
/// @var_ref and constants into @value.
/// \note This version will be called from @match that had its target type a @view
template <typename T, size_t layout, typename E1, typename E2, typename E3>
inline matcher3<T,layout,E1,E2,E3> match_ex(const view<view<T,layout>>&, E1&& e1, E2&& e2, E3&& e3) throw()
{
    return matcher3<T,layout,E1,E2,E3>(
            std::forward<E1>(e1),
            std::forward<E2>(e2),
            std::forward<E3>(e3)
           );
}

/// A 3-argument version of a tree-pattern constructor. Target type is allowed 
/// to be a @view here.
template <typename T, typename E1, typename E2, typename E3>
inline auto match(E1&& e1, E2&& e2, E3&& e3) throw() -> XTL_RETURN
(
    match_ex(
        view<T>(),
        filter(std::forward<E1>(e1)), 
        filter(std::forward<E2>(e2)), 
        filter(std::forward<E3>(e3))
    )
)

/// A 3-argument version of a tree-pattern constructor that takes layout in 
/// addition to the target type.
/// \note @view is not supposed to be passed as a target type to this version
///       of the function because we will then have two potentially conflicting
///       layouts. Any layout different from @default_layout passed here will 
///       result in a compile time error.
template <typename T, size_t layout, typename E1, typename E2, typename E3>
inline auto match(E1&& e1, E2&& e2, E3&& e3) throw() -> XTL_RETURN
(
    match_ex(
        view<T,layout>(),
        filter(std::forward<E1>(e1)), 
        filter(std::forward<E2>(e2)), 
        filter(std::forward<E3>(e3))
    )
)

//------------------------------------------------------------------------------

/// 4-argument version of a helper function to @match that accepts arguments that
/// have been already preprocessed with @filter to convert regular variables into
/// @var_ref and constants into @value.
/// \note This version will be called from @match with a non-@view target type
template <typename T, size_t layout, typename E1, typename E2, typename E3, typename E4>
inline matcher4<T,layout,E1,E2,E3,E4> match_ex(const view<T,layout>&, E1&& e1, E2&& e2, E3&& e3, E4&& e4) throw()
{
    return matcher4<T,layout,E1,E2,E3,E4>(
            std::forward<E1>(e1),
            std::forward<E2>(e2),
            std::forward<E3>(e3),
            std::forward<E4>(e4)
           );
}

/// 4-argument version of a helper function to @match that accepts arguments that
/// have been already preprocessed with @filter to convert regular variables into
/// @var_ref and constants into @value.
/// \note This version will be called from @match that had its target type a @view
template <typename T, size_t layout, typename E1, typename E2, typename E3, typename E4>
inline matcher4<T,layout,E1,E2,E3,E4> match_ex(const view<view<T,layout>>&, E1&& e1, E2&& e2, E3&& e3, E4&& e4) throw()
{
    return matcher4<T,layout,E1,E2,E3,E4>(
            std::forward<E1>(e1),
            std::forward<E2>(e2),
            std::forward<E3>(e3),
            std::forward<E4>(e4)
           );
}

/// A 4-argument version of a tree-pattern constructor. Target type is allowed 
/// to be a @view here.
template <typename T, typename E1, typename E2, typename E3, typename E4>
inline auto match(E1&& e1, E2&& e2, E3&& e3, E4&& e4) throw() -> XTL_RETURN
(
    match_ex(
        view<T>(),
        filter(std::forward<E1>(e1)), 
        filter(std::forward<E2>(e2)), 
        filter(std::forward<E3>(e3)), 
        filter(std::forward<E4>(e4))
    )
)

/// A 4-argument version of a tree-pattern constructor that takes layout in 
/// addition to the target type.
/// \note @view is not supposed to be passed as a target type to this version
///       of the function because we will then have two potentially conflicting
///       layouts. Any layout different from @default_layout passed here will 
///       result in a compile time error.
template <typename T, size_t layout, typename E1, typename E2, typename E3, typename E4>
inline auto match(E1&& e1, E2&& e2, E3&& e3, E4&& e4) throw() -> XTL_RETURN
(
    match_ex(
        view<T,layout>(),
        filter(std::forward<E1>(e1)), 
        filter(std::forward<E2>(e2)), 
        filter(std::forward<E3>(e3)), 
        filter(std::forward<E4>(e4))
    )
)

//------------------------------------------------------------------------------
