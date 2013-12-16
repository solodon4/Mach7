///
/// \file config.hpp
///
/// This file defines various options for configuring the library for use with a 
/// specific project and its class hierarchies as well as macros that are compiler 
/// or platform specific.
///
/// Here are some of the library configuration options that can be set:
///
/// Options with performance impact:
/// - Use of multi-threading           \see @XTL_MULTI_THREADING
/// - Default syntax                   \see @XTL_DEFAULT_SYNTAX
/// - Use of vtbl frequencies          \see @XTL_USE_VTBL_FREQUENCY
/// - Use of memoized_cast             \see @XTL_USE_MEMOIZED_CAST
/// - Whether extractors might throw   \see @XTL_EXTRACTORS_MIGHT_THROW
/// - Use of static local variables    \see @XTL_PRELOAD_LOCAL_STATIC_VARIABLES
/// - Use number of case clauses init  \see @XTL_CLAUSES_NUM_ESTIMATES_TYPES_NUM
/// - Certain under-the-hood types     \see @vtbl_count_t
/// - Certain under-the-hood constants \see @XTL_MIN_LOG_SIZE, @XTL_MAX_LOG_SIZE, @XTL_MAX_LOG_INC, @XTL_LOCAL_CACHE_LOG_SIZE, @XTL_IRRELEVANT_VTBL_BITS
/// Most of the combinations of from this set are built with: make timing
///
/// Options with semantic or convenience impact
/// - Redundancy checking              \see @XTL_REDUNDANCY_CHECKING
/// - Fallthrough behavior             \see @XTL_FALL_THROUGH
/// - Use of { & } around case clauses \see @XTL_USE_BRACES
/// - Declarations in case clause      \see @XTL_CLAUSE_DECL
///
/// Options for logging and debugging
/// - Compile-time messages            \see @XTL_MESSAGE_ENABLED
/// - Trace of performance             \see @XTL_DUMP_PERFORMANCE
/// Most of the combinations of from this set are built with: make syntax
///
/// \autor Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of the XTL framework (http://parasol.tamu.edu/xtl/).
/// Copyright (C) 2005-2012 Texas A&M University.
/// All rights reserved.
///

#pragma once

#if defined(_DEBUG)
#include <iostream>            // We refer to std::cerr in debug mode
#endif

//------------------------------------------------------------------------------

#if !defined(XTL_DUMP_PERFORMANCE)
    /// Flag enabling showing results of performance tracing
    #define XTL_DUMP_PERFORMANCE 0
#endif
#define XTL_DUMP_PERFORMANCE_ONLY(...)   UCL_PP_IF(UCL_PP_NOT(XTL_DUMP_PERFORMANCE), UCL_PP_EMPTY(), UCL_PP_EXPAND(__VA_ARGS__))

#if !defined(XTL_MESSAGE_ENABLED)
    /// Flag compile-time messages from the library
    #define XTL_MESSAGE_ENABLED 0
#endif

#if !defined(XTL_USE_VTBL_FREQUENCY)
    /// When this macro is defined, vtblmaps will count frequency of requests using a
    /// given vtbl pointer and will take it into account during rearranging of the map.
    /// \note This introduces a slight performance overhead to the most frequent path,
    ///       but supposedly will pay when no zero conflict is possible.
    #define XTL_USE_VTBL_FREQUENCY 0
#endif
#define XTL_USE_VTBL_FREQUENCY_ONLY(...) UCL_PP_IF(UCL_PP_NOT(XTL_USE_VTBL_FREQUENCY), UCL_PP_EMPTY(), UCL_PP_EXPAND(__VA_ARGS__))

#if !defined(XTL_REDUNDANCY_CHECKING)
    /// When this macro is defined, our library will generate additional code that 
    /// will trigger compiler to check case clauses for redundancy.
    /// \warning Do not define this macro in actual builds as the generated code 
    ///          will effectively have a switch statement whose body is never evaluated!
    #define XTL_REDUNDANCY_CHECKING 0
#endif

#if !defined(XTL_CLAUSES_NUM_ESTIMATES_TYPES_NUM)
    /// When this macro is 1, we use the number of clauses in the Match statements
    /// as an estimate of the number of types that will pass through that statement.
    /// This helps avoid unnecessery vtblmap-cache reconfigurations and estimates 
    /// sizes of cache and the hash table.
    #define XTL_CLAUSES_NUM_ESTIMATES_TYPES_NUM 1
#endif

#if !defined(XTL_PRELOAD_LOCAL_STATIC_VARIABLES)
    /// In many cases we use local static variables not because we need to initialize
    /// some global state upon first entry, but because we would like to introduce it
    /// within a macro that is used in a local scope. Such global state will only be
    /// used from within the local scope, however we don't care that it is initialized
    /// on the first entry. Using static to declare such local state may introduce an
    /// if statement implementing initialization on first entry, in those cases when 
    /// compiler will not be able to figure out that initialization does not depend on
    /// the function's arguments. This is the case at the moment for all the user-defined
    /// types (e.g. vtblmap<T>), so in the allowed cases we use a trick to take the
    /// initialization outside to save some CPU cycles during actual execution.
    /// By default we enable preloading as it makes the code smaller and somewhat faster,
    /// but one should understand that preloading default initializes the object, so 
    /// passing additional arguments or deferred constatnt values is not possible.
    #define XTL_PRELOAD_LOCAL_STATIC_VARIABLES 1
#endif

#if XTL_PRELOAD_LOCAL_STATIC_VARIABLES
    /// In this case we simply introduce a local reference Name to the globally
    /// preallocated variable of type Type.
    #define XTL_PRELOADABLE_LOCAL_STATIC(Type,Name,UID,...) Type& Name = preallocated<Type,UID>::value
#else
    /// In this case we simply declare a function-local static variable Name of type Type
    #define XTL_PRELOADABLE_LOCAL_STATIC(Type,Name,UID,...) static Type Name UCL_PP_IF(UCL_PP_IS_EMPTY(__VA_ARGS__), UCL_PP_EMPTY(), (UCL_PP_EXPAND(__VA_ARGS__)))
#endif

#if !defined(XTL_FALL_THROUGH)
    /// When this macro is 1 the fall-through behavior of the underlying switch
    /// statement is enabled. It becomes up to the user to use break statements to 
    /// leave the case clause. Fall through behavior might be needed to implement 
    /// all-fit semantics of the @Match statement.
    /// When this macro is 0 the fall-through behavior is disabled and break 
    /// statements are implicitly inserted at the end of each case-clause, while
    /// sub-clauses are made exclusive with the use of else between the ifs.
    /// There are several reasons we enable fall-through by default:
    ///  - it follows the current semantics of C/C++ switch
    ///  - there are certain semantic awkwardnesses when fall through is disabled. 
    ///    They are solely due to library implementation and are easy to avoid in a
    ///    language solution, however, making them the default will be hard to 
    ///    explain to novices. 
    ///    An example of such awkwardness is the fact that When-sub-clauses will 
    ///    only work with refutable Qua-clauses, and thus cannot be used in Case-clauses.
    #define XTL_FALL_THROUGH 1
#endif
#define XTL_FALL_THROUGH_ONLY(...)     UCL_PP_IF(UCL_PP_NOT(XTL_FALL_THROUGH), UCL_PP_EMPTY(), UCL_PP_EXPAND(__VA_ARGS__))
#define XTL_NON_FALL_THROUGH_ONLY(...) UCL_PP_IF(           XTL_FALL_THROUGH,  UCL_PP_EMPTY(), UCL_PP_EXPAND(__VA_ARGS__))

#if !defined(XTL_USE_BRACES)
    /// For general syntax of Match statements, this definition only affects @MatchE
    /// and @MatchS statements. It specifies whether by default the user prefers to 
    /// use { and } to separate case clauses. These two macros can be made work 
    /// either way, but only one of them. The rest of the macros work with or 
    /// without them. There is one important reason to choose the default and stick
    /// to it for other @Match statements. Redundancy checking essentially builds 
    /// the same syntactic structure as @MatchE and thus depends on this choice. 
    /// If you'd like to be able to perform redundancy checking on your statements,
    /// please make the choice and specify it to the library with this macro.
    #define XTL_USE_BRACES 1
#endif
#define XTL_USE_BRACES_ONLY(...)     UCL_PP_IF(UCL_PP_NOT(XTL_USE_BRACES), UCL_PP_EMPTY(), UCL_PP_EXPAND(__VA_ARGS__))
#define XTL_NON_USE_BRACES_ONLY(...) UCL_PP_IF(           XTL_USE_BRACES,  UCL_PP_EMPTY(), UCL_PP_EXPAND(__VA_ARGS__))

#if !defined(XTL_CLAUSE_DECL)
    /// Enables/disables the syntax for declarations inside the case clause:
    ///   Case(const DerivedType& var) var.something();
    /// \note The reason we don't allow it by default is that allowing the usual
    ///       syntax for just type in case clause and this one together may have
    ///       performace implication since it will introduce a creation of 
    ///       anonymous object of type DerivedType into the scope.
    #define XTL_CLAUSE_DECL 0
#endif
#define XTL_CLAUSE_DECL_ONLY(...)     UCL_PP_IF(UCL_PP_NOT(XTL_CLAUSE_DECL), UCL_PP_EMPTY(), UCL_PP_EXPAND(__VA_ARGS__))
#define XTL_NON_CLAUSE_DECL_ONLY(...) UCL_PP_IF(           XTL_CLAUSE_DECL,  UCL_PP_EMPTY(), UCL_PP_EXPAND(__VA_ARGS__))

//------------------------------------------------------------------------------

#if !defined(XTL_MULTI_THREADING)
    /// Whether pattern-matching constructs can be used in multi-threading context
    #define XTL_MULTI_THREADING 0
#endif

//------------------------------------------------------------------------------

#if !defined(XTL_DEFAULT_SYNTAX)
    /// Several choices for configuring syntax:
    /// Default syntax Match,Case,Qua,Or,Otherwise,EndMatch are resolved to
    /// - 'G' - Generic switch (default)
    /// - 'P' - Polymorphic switch
    /// - 'K' - Kind switch: the-only-match
    /// - 'F' - Kind switch: nearly-best-match
    /// - 'U' - Union switch
    /// - 'E' - Exception switch
    /// - 'S' - Sequential cascading-if
    #define XTL_DEFAULT_SYNTAX 'G'
#endif

//------------------------------------------------------------------------------

#if !defined(XTL_EXTRACTORS_MIGHT_THROW)
    /// Choice of exception handling strategies:
    /// - Assume extractors won't throw and thus generate not exception wrappers
    /// - Assume extractors might throw, in which case:
    ///    * Propagate throw further
    ///    * Treat it as failed match
    /// Note that in any case exceptions inside statements associated with case 
    /// clauses are not intercepted in any way.
    /// By default we assume that for a given code using our library extractors 
    /// won't throw and thus we do not generate the necessary exception handling code
    #define XTL_EXTRACTORS_MIGHT_THROW 0
#endif

#if XTL_EXTRACTORS_MIGHT_THROW
    #if !defined(EXTRACTORS_PROPAGATE_THROW)
    #define EXTRACTORS_PROPAGATE_THROW 0
    #endif
#endif

//------------------------------------------------------------------------------

#if !defined(XTL_USE_MEMOIZED_CAST)
    /// Another choice is whether library code should try to benefit from memoized_cast 
    /// or just use dynamic_cast 
    #define XTL_USE_MEMOIZED_CAST 0
#endif

//------------------------------------------------------------------------------

#if !defined(XTL_MIN_LOG_SIZE)
    /// Log of the smallest cache size to start from
    #define XTL_MIN_LOG_SIZE 3
#endif

#if !defined(XTL_MAX_LOG_SIZE)
    /// Log of the largest cache size to try
    #define XTL_MAX_LOG_SIZE 14
#endif

#if !defined(XTL_MAX_LOG_INC)
    /// Log of the maximum allowed increased from the minimum requred log size (1 means twice from the min required size)
    #define XTL_MAX_LOG_INC 1
#endif

#if !defined(XTL_LOCAL_CACHE_LOG_SIZE)
    /// Log of the size of the local cache - the one we use to avoid allocating
    /// memory from heap in early stages
    #define XTL_LOCAL_CACHE_LOG_SIZE 7
#endif

//------------------------------------------------------------------------------

#if !defined(XTL_IRRELEVANT_VTBL_BITS)
/// \note The following section is compiler and platform specific for 
///       optimization purposes.
/// \note As subsequent experiments showed, this value may depend on the number
///       of virtual functions in the class as well. Chosing this value smaller 
///       or larger than necessary, increases the number of collisions and 
///       necessarily degrades performance.
/// FIX: Make this less empirical!
#if defined(_MSC_VER)
    #if defined(_WIN64)
        #if defined(_DEBUG)
            /// vtbl in MSVC in x64 Debug   builds seem to be alligned by 8 bytes
            #define XTL_IRRELEVANT_VTBL_BITS 3
        #else
            /// vtbl in MSVC in x64 Release builds seem to be alligned by 16 bytes
            #define XTL_IRRELEVANT_VTBL_BITS 4
        #endif
    #else
        #if defined(_DEBUG)
            /// vtbl in MSVC in x86 Debug   builds seem to be alligned by 4 bytes
            #define XTL_IRRELEVANT_VTBL_BITS 2
        #else
            /// vtbl in MSVC in x86 Release builds seem to be alligned by 8 bytes
            #define XTL_IRRELEVANT_VTBL_BITS 3
        #endif
    #endif
#else
    /// vtbl in G++ seem to be alligned by 16 bytes
    #define XTL_IRRELEVANT_VTBL_BITS 4
    /// When i defined more virtual functions it became 3 for some reason
    //#define XTL_IRRELEVANT_VTBL_BITS 3
#endif
#endif

//------------------------------------------------------------------------------

#if defined(_DEBUG)
    #define XTL_DEBUG_ONLY(...) __VA_ARGS__
    /// Our own version of assert macro because of the fact that normal assert was 
    /// not always removed in the release builds.
    #define XTL_ASSERT(x) if (!(x)) { std::cerr << #x " in file " << __FILE__ << '[' << __LINE__ << ']'; std::abort(); }
    /// Our own version of assert macro because of the fact that normal assert was 
    /// not always removed in the release builds.
    #define XTL_VERIFY(x) if (!(x)) std::cerr << #x " in file " << __FILE__ << '[' << __LINE__ << ']'
#else
    #define XTL_DEBUG_ONLY(...)
    /// Our own version of assert macro because of the fact that normal assert was 
    /// not always removed in the release builds.
    #define XTL_ASSERT(x)
    /// Our own version of assert macro because of the fact that normal assert was 
    /// not always removed in the release builds.
    #define XTL_VERIFY(x) if (!(x)) std::cerr << #x " in file " << __FILE__ << '[' << __LINE__ << ']'
#endif

//------------------------------------------------------------------------------

/// A macro we use in functions with auto as a return type. It helps us avoid 
/// duplication of expression when both the expression in decltype() and in the
/// only return statement are the same.
/// \example auto foo(T1 t1, T2 t2) -> XTL_RETURN(t1 + t2)
/// \note We use ... (__VA_ARGS__ parameters) to allow expressions 
///       containing comma as argument. Essentially this is a one arg macro
#define XTL_RETURN(...) decltype(__VA_ARGS__) { return (__VA_ARGS__); }

/// The same as @XTL_RETURN but additionally takes a compile-time condition under
/// which the whole function is enabled.
#define XTL_RETURN_ENABLE_IF(C,...) typename std::enable_if<C,decltype(__VA_ARGS__)>::type { return (__VA_ARGS__); }

//------------------------------------------------------------------------------

/// The code that our macros generate may produce some variables that are not
/// necessarily used by the user. This macro is to suppress unused variable warning
#define XTL_UNUSED(x) (void)x;

//------------------------------------------------------------------------------

#if defined(_MSC_VER)
    /// Indicates deleted function \see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2346.htm
    #define XTL_DELETED
#else
    /// Indicates deleted function \see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2346.htm
    #define XTL_DELETED = delete
#endif

//------------------------------------------------------------------------------

#if !defined(__func__) && defined(_MSC_VER)
    #define __func__ __FUNCTION__
#endif

//------------------------------------------------------------------------------

#define XTL_FUNCTION __func__

//------------------------------------------------------------------------------

#if defined(_MSC_VER)
    #define alignof(T) __alignof(T)
#endif

//------------------------------------------------------------------------------

#if defined(_MSC_VER) || defined(__GNUC__)
    /// When the compiler supports Visual C++ like __COUNTER__ macro that is resolved 
    /// to consequitive number each time it is used, we use it, otherwise we approximate 
    /// it with line number. This means that multiple uses of this macro within the same
    /// line should be memoized in a constant to avoid differences in behavior as well as
    /// the user cannot rely on consequitiveness of numbers even when the counter is 
    /// supported because other macros may use the counter as well.
    #define XTL_COUNTER __COUNTER__
#else
    #define XTL_COUNTER __LINE__
#endif

//------------------------------------------------------------------------------

/// Helper macro for the following one to resolve macros inside its arguments
#define _XTL_STRING_LITERAL(x)  #x
/// Macro to stringize some expression.
#define XTL_STRING_LITERAL(x)   _XTL_STRING_LITERAL(x)

//------------------------------------------------------------------------------

#if defined(_MSC_VER)
    /// MSVC10 doesn't seem to support the standard _Pragma operator
    #define XTL_PRAGMA(x) __pragma(x)
    #if XTL_MESSAGE_ENABLED
        /// Helper macro to output a message during compilation in format understood by Visual Studio
        #define XTL_MESSAGE(str) XTL_PRAGMA(message(__FILE__ "(" XTL_STRING_LITERAL(__LINE__) ") : " str))
    #else
        /// Helper macro to output a message during compilation in format understood by Visual Studio
        #define XTL_MESSAGE(str)
    #endif
    /// Macro to save the settings for diagnostics before the library will modify some
    #define XTL_WARNING_PUSH XTL_PRAGMA(warning(push))
    /// Macro to restore the settings for diagnostics after the library has modified some
    #define XTL_WARNING_POP  XTL_PRAGMA(warning(pop))
    /// Macro that disables all warnings
  //#define XTL_WARNING_IGNORE_ALL                  XTL_PRAGMA(warning( disable ))
    /// Macro that disables warning on constant conditional expression, which we have a lot from code generated by macros
    #define XTL_WARNING_IGNORE_CONSTANT_CONDITIONAL XTL_PRAGMA(warning( disable : 4127 )) // warning C4127: conditional expression is constant
    /// Macro that disables warning on manipulating pointers with reinterpret_cast
    #define XTL_WARNING_IGNORE_STRICT_ALIASING      
#elif defined(__GNUC__)
    /// Helper macro to use the sandard _Pragma operator
    #define XTL_PRAGMA(x) _Pragma(#x)
    #if XTL_MESSAGE_ENABLED
        /// Helper macro to output a message during compilation. GCC prepends file and line info to it anyways
        #define XTL_MESSAGE(str) XTL_PRAGMA(message str)
    #else
        /// Helper macro to output a message during compilation. GCC prepends file and line info to it anyways
        #define XTL_MESSAGE(str)
    #endif
    /// Macro to save the settings for diagnostics before the library will modify some
    #define XTL_WARNING_PUSH XTL_PRAGMA(GCC diagnostic push)
    /// Macro to restore the settings for diagnostics after the library has modified some
    #define XTL_WARNING_POP  XTL_PRAGMA(GCC diagnostic pop)
    /// Macro that disables all warnings
  //#define XTL_WARNING_IGNORE_ALL                  XTL_PRAGMA(GCC diagnostic ignored "-Wall")
    /// Macro that disables warning on constant conditional expression, which we have a lot from code generated by macros
    #define XTL_WARNING_IGNORE_CONSTANT_CONDITIONAL
    /// Macro that disables warning on manipulating pointers with reinterpret_cast
    #define XTL_WARNING_IGNORE_STRICT_ALIASING      XTL_PRAGMA(GCC diagnostic ignored "-Wstrict-aliasing") // warning: dereferencing type-punned pointer will break strict-aliasing rules [-Wstrict-aliasing]
#else
    /// Helper macro to use the sandard _Pragma operator
    #define XTL_PRAGMA(x)
    /// Helper macro to output a message during compilation. GCC prepends file and line info to it anyways
    #define XTL_MESSAGE(str)
    /// Macro to save the settings for diagnostics before the library will modify some
    #define XTL_WARNING_PUSH
    /// Macro to restore the settings for diagnostics after the library has modified some
    #define XTL_WARNING_POP
    /// Macro that disables all warnings
  //#define XTL_WARNING_IGNORE_ALL
    /// Macro that disables warning on constant conditional expression, which we have a lot from code generated by macros
    #define XTL_WARNING_IGNORE_CONSTANT_CONDITIONAL 
    /// Macro that disables warning on manipulating pointers with reinterpret_cast
    #define XTL_WARNING_IGNORE_STRICT_ALIASING
#endif

//------------------------------------------------------------------------------

/// if-statement with constant condition that does not generate a warning about it
#define XTL_STATIC_IF(c) XTL_WARNING_PUSH XTL_WARNING_IGNORE_CONSTANT_CONDITIONAL if ((c)) XTL_WARNING_POP

#if defined(__GNUC__)
    /// NOTE: GCC doesn't let put pragmas here
    /// while-statement with constant condition that does not generate a warning about it
    #define XTL_STATIC_WHILE(c) while ((c))
#else
    /// while-statement with constant condition that does not generate a warning about it
    #define XTL_STATIC_WHILE(c) XTL_WARNING_PUSH XTL_WARNING_IGNORE_CONSTANT_CONDITIONAL while ((c)) XTL_WARNING_POP
#endif

//------------------------------------------------------------------------------

#ifdef _MSC_VER
    #define XTL_CONCATENATE(A,B) A ## B
#else
    /// Somehow operator ## == (or any other) results in error in GCC, so we use 
    /// a trick to let users build operator name when they need.
    #define XTL_IDENTITY(A) A
    #define XTL_CONCATENATE(A,B) XTL_IDENTITY(A)XTL_IDENTITY(B)
#endif

//------------------------------------------------------------------------------

/// FIX: Yet another version that works in different case, but hasn't been merged
///      with the above yet.
#define XTL_CONCAT(arg1, arg2)  XTL_CONCAT1(arg1, arg2)
#define XTL_CONCAT1(arg1, arg2) XTL_CONCAT2(arg1, arg2)
#define XTL_CONCAT2(arg1, arg2) arg1##arg2

//------------------------------------------------------------------------------

/// MS Visual C++ 2005 (in which variadic macros became supported) till at 
/// least MS Visual C++ 2010 has a bug in passing __VA_ARGS__ to subsequent
/// macros as a single token, which results in:
///     warning C4003: not enough actual parameters for macro 'XTL_ARG_N'
/// and incorrect behavior. The workaround used here is described at:
/// https://connect.microsoft.com/VisualStudio/feedback/details/380090/variadic-macro-replacement
#define XTL_APPLY_VARIADIC_MACRO(macro,tuple) macro tuple

//------------------------------------------------------------------------------

#define XTL_RSEQ_N() 63,62,61,60,59,58,57,56,55,54,53,52,51,50,49,48,47,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0
#define    XTL_ARG_N( _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, _61,_62,_63,      N,...) N 
#define XTL_ARG_N_EX( _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, _61,_62,_63,Dummy,N,...) N 

#ifdef _MSC_VER

    /// A macro used to count a number of variadic arguments.
    /// \note Using this macro without arguments violates 6.10.3p4 of ISO C99 
    ///       and thus it cannot be used to detect zero arguments. 
    /// \note This macro was invented by Laurent Deniau and can be found here:
    /// \see  http://groups.google.com/group/comp.std.c/browse_thread/thread/77ee8c8f92e4a3fb/346fc464319b1ee5
    #define XTL_NARG(...)  XTL_APPLY_VARIADIC_MACRO(XTL_NARG_,(__VA_ARGS__,XTL_RSEQ_N()))
    #define XTL_NARG_(...) XTL_APPLY_VARIADIC_MACRO(XTL_ARG_N,(__VA_ARGS__))

    /// The same as above but assumes a dummy first argument that is not counted
    /// in order to deal with the fact that regular @XTL_NARG cannot cope with 
    /// zero arguments.
    /// \note We need here 0 to be a real 0 and not something that evaluates to 0
    ///       as is done in some solutions to this problem of NARG, because we 
    ///       append this number to a name to form another macro!
    #define XTL_NARG_EX(...)  XTL_APPLY_VARIADIC_MACRO(XTL_NARG_EX_,(__VA_ARGS__,XTL_RSEQ_N()))
    #define XTL_NARG_EX_(...) XTL_APPLY_VARIADIC_MACRO(XTL_ARG_N_EX,(__VA_ARGS__))

#else

    /// A macro used to count a number of variadic arguments.
    /// \note Using this macro without arguments violates 6.10.3p4 of ISO C99 
    ///       and thus it cannot be used to detect zero arguments. 
    /// \note This macro was invented by Laurent Deniau and can be found here:
    /// \see  http://groups.google.com/group/comp.std.c/browse_thread/thread/77ee8c8f92e4a3fb/346fc464319b1ee5
    #define XTL_NARG(...)  XTL_NARG_(__VA_ARGS__,XTL_RSEQ_N()) 
    #define XTL_NARG_(...) XTL_ARG_N(__VA_ARGS__) 

    /// The same as above but assumes a dummy first argument that is not counted
    /// in order to deal with the fact that regular @XTL_NARG cannot cope with 
    /// zero arguments.
    /// \note We need here 0 to be a real 0 and not something that evaluates to 0
    ///       as is done in some solutions to this problem of NARG, because we 
    ///       append this number to a name to form another macro!
    #define XTL_NARG_EX(...)  XTL_NARG_EX_(__VA_ARGS__,XTL_RSEQ_N()) 
    #define XTL_NARG_EX_(...) XTL_ARG_N_EX(__VA_ARGS__)

#endif

//------------------------------------------------------------------------------

#if !defined(XTL_ARR_SIZE)
    /// A macro used to infer the size of an array.
    #define XTL_ARR_SIZE(a) (sizeof(a)/sizeof((a)[0]))
#endif

//------------------------------------------------------------------------------

/// A macro used to infer number of bits in a given type of variable.
#define XTL_BIT_SIZE(T) (8*sizeof(T))

/// Sets i^th bit in bit_array
#define XTL_BIT_SET(bit_array, i) ( (bit_array)[(i)/XTL_BIT_SIZE((bit_array)[0])] |= (1 << ((i) % XTL_BIT_SIZE((bit_array)[0]))) )

/// Gets i^th bit in bit_array
#define XTL_BIT_GET(bit_array, i) ( (bit_array)[(i)/XTL_BIT_SIZE((bit_array)[0])]  & (1 << ((i) % XTL_BIT_SIZE((bit_array)[0]))) )

//------------------------------------------------------------------------------

#if !defined(_DEBUG)
    #if defined(__GNUC__)
        #define XTL_SUPPORTS_VLA
    #elif defined(_MSC_VER)
        #define XTL_SUPPORTS_ALLOCA
    #endif
#endif

//------------------------------------------------------------------------------

#if defined(XTL_SUPPORTS_VLA)
    #define XTL_VLA(v,T,n,N)  T v[n]
    #define XTL_VLAZ(v,T,n,N) T v[n]; std::memset(v,0,n*sizeof(T))
#elif defined(XTL_SUPPORTS_ALLOCA)
    #define XTL_VLA(v,T,n,N)  T* v = static_cast<T*>(alloca(n*sizeof(T)))
    #define XTL_VLAZ(v,T,n,N) T* v = static_cast<T*>(alloca(n*sizeof(T))); std::memset(v,0,n*sizeof(T))
#else
    #define XTL_VLA(v,T,n,N)  T v[N]
    #define XTL_VLAZ(v,T,n,N) T v[N] = {}
#endif

//------------------------------------------------------------------------------

#if defined(_MSC_VER)
    #define XTL_MSC_ONLY(...)     __VA_ARGS__
    #define XTL_NON_MSC_ONLY(...)
#else
    #define XTL_MSC_ONLY(...)
    #define XTL_NON_MSC_ONLY(...) __VA_ARGS__
#endif

//------------------------------------------------------------------------------

#if defined(__GNUC__)
    #define XTL_GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
    #define XTL_GCC_ONLY(...)     __VA_ARGS__
    #define XTL_NON_GCC_ONLY(...)
#else
    #define XTL_GCC_ONLY(...)
    #define XTL_NON_GCC_ONLY(...) __VA_ARGS__
#endif

//------------------------------------------------------------------------------

#if defined(_MSC_VER) || defined(__GNUC__) && (XTL_GCC_VERSION < 40600)
    #define noexcept throw()
#endif

//------------------------------------------------------------------------------

#if defined(_MSC_VER)

    /// Macros to use compiler's branch hinting. 
    /// \note These macros are only to be used in Case macro expansion, not in 
    ///       user's code since they explicitly expect a pointer argument
    /// \note We use ... (__VA_ARGS__ parameters) to allow expressions 
    ///       containing comma as argument. Essentially this is a one arg macro
    #define   XTL_LIKELY(...) (__VA_ARGS__)
    #define XTL_UNLIKELY(...) (__VA_ARGS__)

    /// A macro that is supposed to be put before the function definition whose inlining should be disabled
    #define XTL_DO_NOT_INLINE_BEGIN __pragma(auto_inline (off))
    /// A macro that is supposed to be put after  the function definition whose inlining should be disabled
    #define XTL_DO_NOT_INLINE_END   __pragma(auto_inline (on))

    /// A macro that is supposed to be put before the function definition whose body must be inlined
    #define XTL_FORCE_INLINE_BEGIN __forceinline
    /// A macro that is supposed to be put after  the function definition whose body must be inlined
    #define XTL_FORCE_INLINE_END
#else

    /// Macros to use compiler's branch hinting. 
    /// \note These macros are only to be used in Case macro expansion, not in 
    ///       user's code since they explicitly expect a pointer argument
    /// \note We use ... (__VA_ARGS__ parameters) to allow expressions 
    ///       containing comma as argument. Essentially this is a one arg macro
    #define   XTL_LIKELY(...) (__builtin_expect((long int)(__VA_ARGS__), 1))
    #define XTL_UNLIKELY(...) (__builtin_expect((long int)(__VA_ARGS__), 0))

    /// A macro that is supposed to be put before the function definition whose inlining should be disabled
    #define XTL_DO_NOT_INLINE_BEGIN __attribute__ ((noinline))
    /// A macro that is supposed to be put after  the function definition whose inlining should be disabled
    #define XTL_DO_NOT_INLINE_END


    /// A macro that is supposed to be put before the function definition whose body must be inlined
    #define XTL_FORCE_INLINE_BEGIN __attribute__ ((always_inline)) static inline 
    /// A macro that is supposed to be put after  the function definition whose body must be inlined
    #define XTL_FORCE_INLINE_END
#endif

//------------------------------------------------------------------------------

/// Apparently in C++0x typename can be used to annotate types even in 
/// non-template context. If indeed so, this is what we need to avoid duplication
/// of macros depending on whether they are used in templated and non-templated 
/// context.
///
/// From: C++0x 14.2[5]
/// A name prefixed by the keyword template shall be a template-id or the name shall refer to a class template.
/// [ Note: The keyword template may not be applied to non-template members of class templates. -end
/// note ] [ Note: As is the case with the typename prefix, the template prefix is allowed in cases where it is
/// not strictly necessary; i.e., when the nested-name-specifier or the expression on the left of the -> or . is not
/// dependent on a template-parameter, or the use does not appear in the scope of a template. -end note ]
#if defined(__GNUC__) && (XTL_GCC_VERSION > 40500)
    #define XTL_CPP0X_TYPENAME typename
#else
    #define XTL_CPP0X_TYPENAME 
#endif
#define XTL_CPP0X_TEMPLATE XTL_NON_GCC_ONLY(template)

//------------------------------------------------------------------------------

/// The following set of macros was copied verbatim from a response by Moritz Beutel
/// here: https://forums.embarcadero.com/message.jspa?messageID=320338
/// It is used for suppressing , (comma) when argument is added to __VA_ARGS__, which
/// might be empty.
#define UCL_PP_AND(a,b) UCL_PP_CAT3(_UCL_PP_AND_, a, b)
#define UCL_PP_NOT(a) UCL_PP_CAT2(_UCL_PP_NOT_, a)
#define UCL_PP_IF(a,b,...) UCL_PP_CAT2(_UCL_PP_IF_, a)(b, __VA_ARGS__)
 
#define UCL_PP_EMPTY()
#define UCL_PP_EXPAND(...) __VA_ARGS__
#define UCL_PP_1ST(a,...) a
#define UCL_PP_2ND(a,b,...) b
#define UCL_PP_REMOVE_1ST(a,...) __VA_ARGS__
#define UCL_PP_PAIR(a,...) a __VA_ARGS__
#define UCL_PP_CAT2(a,...) UCL_PP_CAT2_(a, __VA_ARGS__)
#define UCL_PP_CAT3(a,b,...) UCL_PP_CAT3_(a, b, __VA_ARGS__)
 
/// The two macros below are inspired by Laurent Deniau's posting on comp.lang.c from 2006/09/27
/// http://groups.google.com/group/comp.lang.c/browse_thread/thread/578912299f8f87ce#msg_937356effc43f569
#define UCL_PP_IS_EMPTY(...) \
    UCL_PP_AND(UCL_PP_IS_LIST(__VA_ARGS__ ()),UCL_PP_NOT(UCL_PP_IS_LIST(__VA_ARGS__ _)))
#define UCL_PP_IS_LIST(...) \
    UCL_PP_PAIR(UCL_PP_1ST, (UCL_PP_CAT2(UCL_PP_IS_LIST_RET_, UCL_PP_IS_LIST_TST_ __VA_ARGS__)))
 
 
/// implementation details
 
#define UCL_PP_IS_LIST_TST_(...) 1
#define UCL_PP_IS_LIST_RET_UCL_PP_IS_LIST_TST_ 0,
#define UCL_PP_IS_LIST_RET_1 1,
 
#define UCL_PP_CAT2_(a,...) UCL_PP_EXPAND(a ## __VA_ARGS__)
#define UCL_PP_CAT3_(a,b,...) UCL_PP_EXPAND(a ## b ## __VA_ARGS__)
 
#define _UCL_PP_AND_00 0
#define _UCL_PP_AND_01 0
#define _UCL_PP_AND_10 0
#define _UCL_PP_AND_11 1
 
#define _UCL_PP_IF_0(a,...) __VA_ARGS__
#define _UCL_PP_IF_1(a,...) a
 
#define _UCL_PP_NOT_0 1
#define _UCL_PP_NOT_1 0

//------------------------------------------------------------------------------
