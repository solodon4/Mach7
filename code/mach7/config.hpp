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
/// This file defines various options for configuring the library for use with a 
/// specific project and its class hierarchies as well as macros that are compiler 
/// or platform specific.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// \see https://parasol.tamu.edu/mach7/
/// \see https://github.com/solodon4/Mach7
/// \see https://github.com/solodon4/SELL
///
/// Here are some of the library configuration options that can be set:
///
/// Options with performance impact:
/// - Use of multi-threading           \see #XTL_MULTI_THREADING
/// - Default syntax                   \see #XTL_DEFAULT_SYNTAX
/// - Use of vtbl frequencies          \see #XTL_USE_VTBL_FREQUENCY
/// - Use of memoized_cast             \see #XTL_USE_MEMOIZED_CAST
/// - Whether extractors might throw   \see #XTL_EXTRACTORS_MIGHT_THROW
/// - Use of static local variables    \see #XTL_PRELOAD_LOCAL_STATIC_VARIABLES
/// - Use number of case clauses init  \see #XTL_CLAUSES_NUM_ESTIMATES_TYPES_NUM
/// - Certain under-the-hood types     \see #vtbl_count_t
/// - Certain under-the-hood constants \see #XTL_MIN_LOG_SIZE, #XTL_MAX_LOG_INC, #XTL_MAX_STACK_LOG_SIZE, #XTL_IRRELEVANT_VTBL_BITS
/// Most of the combinations of from this set are built with: make timing
///
/// Options with semantic or convenience impact
/// - Redundancy checking              \see #XTL_REDUNDANCY_CHECKING
/// - Fallthrough behavior             \see #XTL_FALL_THROUGH
/// - Use of { & } around case clauses \see #XTL_USE_BRACES
/// - Declarations in case clause      \see #XTL_CLAUSE_DECL
/// Most of the combinations from this set are built with: make syntax
///
/// Options for logging and debugging
/// - Compile-time messages            \see #XTL_MESSAGE_ENABLED
/// - Trace of performance             \see #XTL_DUMP_PERFORMANCE
/// - Trace of memory leaks with lines \see #XTL_LEAKED_NEW_LOCATIONS
/// - Trace of conditions likeliness   \see #XTL_TRACE_LIKELINESS 
///

#pragma once

#include <cppft/config.hpp> // C++ feature testing and feature emulation
#include "macros.hpp"       // Tools for preprocessor meta-programming

//------------------------------------------------------------------------------

#if !defined(XTL_DUMP_PERFORMANCE)
    /// Flag enabling showing results of performance tracing.
    /// The flag is disabled by default because it incures performance overhead
    /// during normal program execution.
    #define XTL_DUMP_PERFORMANCE 0
#endif
#define XTL_DUMP_PERFORMANCE_ONLY(...)   XTL_IF(XTL_NOT(XTL_DUMP_PERFORMANCE), XTL_EMPTY(), XTL_EXPAND(__VA_ARGS__))

//------------------------------------------------------------------------------

#if !defined(XTL_TRACE_LIKELINESS)
    /// A macro that enables tracing of XTL_LIKELY and XTL_UNLIKELY macros to 
    /// ensure the actual calls match what we've put in code. Not for user code.
    /// By default we enable tracing only when we enabling performance tracing as
    /// it too has an overhead per each condition with XTL_[UN]LIKELY.
    #define XTL_TRACE_LIKELINESS XTL_DUMP_PERFORMANCE
#endif

#if XTL_TRACE_LIKELINESS
    #include "debug.hpp"
    #define XTL_LIKELY(c)   (mch::trace_likeliness< true,__LINE__,decltype(XTL_FUNCTION)>(c,#c,__FILE__))
    #define XTL_UNLIKELY(c) (mch::trace_likeliness<false,__LINE__,decltype(XTL_FUNCTION)>(c,#c,__FILE__))
#endif

//------------------------------------------------------------------------------

#if !defined(XTL_MESSAGE_ENABLED)
    /// Flag enabling compile-time messages from the library
    #define XTL_MESSAGE_ENABLED 1
#endif

#if !defined(XTL_USE_VTBL_FREQUENCY)
    /// When this macro is defined, vtblmaps will count frequency of requests using a
    /// given vtbl pointer and will take it into account during rearranging of the map.
    /// \note This introduces a slight performance overhead to the most frequent path,
    ///       but supposedly will pay when no zero conflict is possible.
    /// \deprecated This macro will be deprecated as tracing frequency was not improving
    ///       performance enough to justify the overhead it introduces.
    #define XTL_USE_VTBL_FREQUENCY 0
#endif
#define XTL_USE_VTBL_FREQUENCY_ONLY(...) XTL_IF(XTL_NOT(XTL_USE_VTBL_FREQUENCY), XTL_EMPTY(), XTL_EXPAND(__VA_ARGS__))

#if !defined(XTL_REDUNDANCY_CHECKING)
    /// When this macro is defined, our library will generate additional code that 
    /// will trigger compiler to check case clauses for redundancy.
    /// \warning Do not define this macro in actual builds as the generated code 
    ///          will effectively have a switch statement whose body is never evaluated!
    /// \note    Only works with older match.hpp as in the library setting this 
    ///          is only possible for single argument, not for multiple arguments
    #define XTL_REDUNDANCY_CHECKING 0
#endif
#define XTL_REDUNDANCY_ONLY(...)     XTL_IF(XTL_NOT(XTL_REDUNDANCY_CHECKING), XTL_EMPTY(), XTL_EXPAND(__VA_ARGS__))
#define XTL_NON_REDUNDANCY_ONLY(...) XTL_IF(        XTL_REDUNDANCY_CHECKING,  XTL_EMPTY(), XTL_EXPAND(__VA_ARGS__))

#if !defined(XTL_CLAUSES_NUM_ESTIMATES_TYPES_NUM)
    /// When this macro is 1, we use the number of clauses in the Match statements
    /// as an estimate of the number of types that will pass through that statement.
    /// This helps avoid unnecessery vtblmap-cache reconfigurations and estimates 
    /// sizes of cache and the hash table.
    /// Enabled by default to decrease the number of cache reconfigurations.
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
    #define XTL_PRELOADABLE_LOCAL_STATIC(Type,Name,UID,...) Type& Name = mch::preallocated<Type,UID>::value
#else
    /// In this case we simply declare a function-local static variable Name of type Type
    #define XTL_PRELOADABLE_LOCAL_STATIC(Type,Name,UID,...) static Type Name XTL_IF(XTL_IS_EMPTY(__VA_ARGS__), XTL_EMPTY(), (XTL_EXPAND(__VA_ARGS__)))
#endif

#if !defined(XTL_FALL_THROUGH)
    /// When this macro is 1 the fall-through behavior of the underlying switch
    /// statement is enabled. It becomes up to the user to use break statements to 
    /// leave the case clause. Fall through behavior might be needed to implement 
    /// all-fit semantics of the #Match statement.
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
    /// \deprecated This macro will likely be deprecated as the fall through behavior
    ///             it offers differs from the fall through behavior of switch statement.
    /// \note Several novices made a mistake of not putting break at the end of the case
    ///       clause effectively making the Match statement super slow because the code
    ///       was needlessly checking all the predicates after the matching clause. We
    ///       should probably make 0 the default just to help novices & avoid questions
    ///       about poor performance. (Sep 14, 2013)
    /// \bug  Old Qua clauses seem to be putting break statements in the wrong place.
    ///       Reverting back to 1 for now. (Dec 7, 2014) \see extractor.cpp for repro.
    #define XTL_FALL_THROUGH 1
#endif
#define XTL_FALL_THROUGH_ONLY(...)     XTL_IF(XTL_NOT(XTL_FALL_THROUGH), XTL_EMPTY(), XTL_EXPAND(__VA_ARGS__))
#define XTL_NON_FALL_THROUGH_ONLY(...) XTL_IF(        XTL_FALL_THROUGH,  XTL_EMPTY(), XTL_EXPAND(__VA_ARGS__))

#if !defined(XTL_USE_BRACES)
    /// For general syntax of Match statements, this definition only affects #MatchE
    /// and #MatchS statements. It specifies whether by default the user prefers to 
    /// use { and } to separate case clauses. These two macros can be made work 
    /// either way, but only one of them. The rest of the macros work with or 
    /// without them. There is one important reason to choose the default and stick
    /// to it for other #Match statements. Redundancy checking essentially builds 
    /// the same syntactic structure as #MatchE and thus depends on this choice. 
    /// If you'd like to be able to perform redundancy checking on your statements,
    /// please make the choice and specify it to the library with this macro.
    #define XTL_USE_BRACES 1
#endif
#define XTL_USE_BRACES_ONLY(...)     XTL_IF(XTL_NOT(XTL_USE_BRACES), XTL_EMPTY(), XTL_EXPAND(__VA_ARGS__))
#define XTL_NON_USE_BRACES_ONLY(...) XTL_IF(        XTL_USE_BRACES,  XTL_EMPTY(), XTL_EXPAND(__VA_ARGS__))

#if !defined(XTL_CLAUSE_DECL)
    /// Enables/disables the syntax for declarations inside the case clause:
    ///   Case(const DerivedType& var) var.something();
    /// \note The reason we don't allow it by default is that allowing the usual
    ///       syntax for just type in case clause and this one together may have
    ///       performace implication since it will introduce a creation of 
    ///       anonymous object of type DerivedType into the scope.
    #define XTL_CLAUSE_DECL 0
#endif
#define XTL_CLAUSE_DECL_ONLY(...)     XTL_IF(XTL_NOT(XTL_CLAUSE_DECL), XTL_EMPTY(), XTL_EXPAND(__VA_ARGS__))
#define XTL_NON_CLAUSE_DECL_ONLY(...) XTL_IF(        XTL_CLAUSE_DECL,  XTL_EMPTY(), XTL_EXPAND(__VA_ARGS__))

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
    /// \deprecated This macro is deprecated now and is only used with old vtblmap implementations
    #define XTL_MAX_LOG_SIZE 14
#endif

#if !defined(XTL_MAX_LOG_INC)
    /// Log of the maximum allowed increased from the minimum requred log size (1 means twice from the min required size)
    #define XTL_MAX_LOG_INC 1
#endif

#if !defined(XTL_LOCAL_CACHE_LOG_SIZE)
    /// Log of the size of the local cache - the one we use to avoid allocating
    /// memory from heap in early stages.
    /// \deprecated This macro is deprecated now and is only used with old vtblmap implementations
    #define XTL_LOCAL_CACHE_LOG_SIZE 7
#endif

#if !defined(XTL_MAX_STACK_LOG_SIZE)
    /// Log of the maximum stack size the library can use to do some histogram 
    /// computations. Making this value smaller will still work, however the 
    /// library may not be able to distinguish optimal and sub-optimal cache 
    /// parameters.
    /// A value 20 means the library can allocate an array of up to 2^20 bytes
    /// on the stack to build a histogram of a given hash function. The actually
    /// requested value will typically be smaller through the use of VLA in GCC 
    /// and alloca in MSVC. The full array will be allocated for those compilers
    /// not providing any similar means.
    #define XTL_MAX_STACK_LOG_SIZE 18
#endif

//------------------------------------------------------------------------------

#if !defined(NDEBUG)
    /// Helper function to be used within XTL_ASSERT only that allows one to pass an explanation
    inline bool xtl_failure(const char*, bool condition) { return condition; }

    #define XTL_DEBUG_ONLY(...) __VA_ARGS__
#else
    /// In release builds, xtl_failure is a macro that ignores its explanation argument
    #define xtl_failure(text, condition) condition

    #define XTL_DEBUG_ONLY(...)
#endif

/// Our own version of assert macro because of the fact that normal assert was 
/// not always removed in the release builds.
#define XTL_VERIFY(x) if (!(x)) std::cerr << #x " in file " << __FILE__ << '[' << __LINE__ << ']' << std::endl

//------------------------------------------------------------------------------

/// A macro we use in functions with auto as a return type. It helps us avoid 
/// duplication of expression when both the expression in decltype() and in the
/// only return statement are the same.
/// \code
/// auto foo(T1 t1, T2 t2) -> XTL_RETURN(t1 + t2)
/// \endcode
/// \note We use ... (__VA_ARGS__ parameters) to allow expressions 
///       containing comma as argument. Essentially this is a one arg macro
#define XTL_RETURN(...) decltype(__VA_ARGS__) { return (__VA_ARGS__); }

/// The same as #XTL_RETURN but additionally takes a compile-time condition under
/// which the whole function is enabled.
#define XTL_RETURN_ENABLE_IF(C,...) typename std::enable_if<C,decltype(__VA_ARGS__)>::type { return (__VA_ARGS__); }

//------------------------------------------------------------------------------

/// The code that our macros generate may produce some variables that are not
/// necessarily used by the user. This macro is to suppress unused variable warning
#define XTL_UNUSED(x) (void)x;

//------------------------------------------------------------------------------

#define XTL_DELETED   XTL_IF(XTL_SUPPORT(ddf),= delete,)
#define XTL_DEFAULTED XTL_IF(XTL_SUPPORT(ddf),= default,{})

//------------------------------------------------------------------------------

/// Helper macro for the following one to resolve macros inside its arguments
#define _XTL_STRING_LITERAL(x)  #x
/// Macro to stringize some expression.
#define XTL_STRING_LITERAL(x)   _XTL_STRING_LITERAL(x)

//------------------------------------------------------------------------------

//#ifdef _MSC_VER
//    #define XTL_CONCATENATE(A,B) A ## B
//#else
//    /// Somehow operator ## == (or any other) results in error in GCC, so we use 
//    /// a trick to let users build operator name when they need.
//    #define XTL_IDENTITY(A) A
//    #define XTL_CONCATENATE(A,B) XTL_IDENTITY(A)XTL_IDENTITY(B)
//#endif

//------------------------------------------------------------------------------

/// FIX: Yet another version that works in different case, but hasn't been merged
///      with the above yet.
#define XTL_CONCAT(arg1, arg2)  XTL_CONCAT1(arg1, arg2)
#define XTL_CONCAT1(arg1, arg2) XTL_CONCAT2(arg1, arg2)
#define XTL_CONCAT2(arg1, arg2) arg1##arg2

//------------------------------------------------------------------------------

#define XTL_RSEQ_N() 10,9,8,7,6,5,4,3,2,1,0
#define    XTL_ARG_N( _1, _2, _3, _4, _5, _6, _7, _8, _9, _10,        N, ...) N
#define XTL_ARG_N_EX( _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, Dummy, N, ...) N

#ifdef _MSC_VER

    /// A macro used to count a number of variadic arguments.
    /// \note Using this macro without arguments violates 6.10.3p4 of ISO C99 
    ///       and thus it cannot be used to detect zero arguments. 
    /// \note This macro was invented by Laurent Deniau and can be found here:
    /// \see  http://groups.google.com/group/comp.std.c/browse_thread/thread/77ee8c8f92e4a3fb/346fc464319b1ee5
    #define XTL_NARG(...)  XTL_APPLY_VARIADIC_MACRO(XTL_NARG_,(__VA_ARGS__,XTL_RSEQ_N()))
    #define XTL_NARG_(...) XTL_APPLY_VARIADIC_MACRO(XTL_ARG_N,(__VA_ARGS__))

    /// The same as above but assumes a dummy first argument that is not counted
    /// in order to deal with the fact that regular #XTL_NARG cannot cope with 
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
    /// in order to deal with the fact that regular #XTL_NARG cannot cope with 
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

#if !defined(XTL_VARIABLE_SIZE_ARRAY)
    /// A macro used as an array subscript for trailing array idiom.
    /// We add it because some compilers accept 0 there, while others 1 and to
    /// explicitly indicate the programmer's intent to allocate memory right after.
    #define XTL_VARIABLE_SIZE_ARRAY 1
#endif

//------------------------------------------------------------------------------

/// A macro used to infer number of bits in a given type of variable.
#define XTL_BIT_SIZE(T) (8*sizeof(T))

/// Sets i^th bit in bit_array
#define XTL_BIT_SET(bit_array, i) ( (bit_array)[(i)/XTL_BIT_SIZE((bit_array)[0])] |= (1 << ((i) % XTL_BIT_SIZE((bit_array)[0]))) )

/// Gets i^th bit in bit_array
#define XTL_BIT_GET(bit_array, i) ( (bit_array)[(i)/XTL_BIT_SIZE((bit_array)[0])]  & (1 << ((i) % XTL_BIT_SIZE((bit_array)[0]))) )

//------------------------------------------------------------------------------

#if XTL_SUPPORT(vla)
    #define XTL_VLA(v,T,n,N)  T v[n]
    #define XTL_VLAZ(v,T,n,N) T v[n]; std::memset(v,0,n*sizeof(T))
#elif XTL_SUPPORT(alloca)
    #define XTL_VLA(v,T,n,N)  T* v = static_cast<T*>(alloca(n*sizeof(T)))
    #define XTL_VLAZ(v,T,n,N) T* v = static_cast<T*>(alloca(n*sizeof(T))); std::memset(v,0,n*sizeof(T))
#else
    #define XTL_VLA(v,T,n,N)  T v[N]
    #define XTL_VLAZ(v,T,n,N) T v[N] = {}
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
