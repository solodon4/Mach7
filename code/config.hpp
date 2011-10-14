///
/// \file config.hpp
///
/// This file defines various macros that are compiler or platform specific.
///
/// \autor Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of the XTL framework (http://parasol.tamu.edu/xtl/).
/// Copyright (C) 2005-2011 Texas A&M University.
/// All rights reserved.
///

#pragma once

#include <cassert>

//------------------------------------------------------------------------------

// Uncomment this macro definition if you'd like to do some performance tracing
#if !defined(TRACE_PERFORMANCE)
//#define TRACE_PERFORMANCE
#endif

//------------------------------------------------------------------------------

// Uncomment this macro definition if you'd like to do some performance tracing
#if !defined(DUMP_PERFORMANCE)
#define DUMP_PERFORMANCE
#endif

//------------------------------------------------------------------------------

#if !defined(USE_PEARSON_HASH)
//#define USE_PEARSON_HASH
#endif

//------------------------------------------------------------------------------

#if !defined(VTBL_DEFAULT_CACHE_BITS)
#define VTBL_DEFAULT_CACHE_BITS 7
#endif

//------------------------------------------------------------------------------

#if !defined(VTBL_IRRELEVANT_BITS)
// \note The following section is compiler and platform specific for 
//       optimization purposes.
// \note As subsequent experiments showed, this value may depend on the number
//       of virtual functions in the class as well. Chosing this value smaller 
//       or larger than necessary, increases the number of collisions and 
//       necessarily degrades performance.
// FIX: Make this less empirical!
#if defined(_MSC_VER)
    #if defined(_WIN64)
        #if defined(_DEBUG)
            /// vtbl in MSVC in x64 Debug   builds seem to be alligned by 8 bytes
            #define VTBL_IRRELEVANT_BITS 3
        #else
            /// vtbl in MSVC in x64 Release builds seem to be alligned by 16 bytes
            #define VTBL_IRRELEVANT_BITS 4
        #endif
    #else
        #if defined(_DEBUG)
            /// vtbl in MSVC in x86 Debug   builds seem to be alligned by 4 bytes
            #define VTBL_IRRELEVANT_BITS 2
        #else
            /// vtbl in MSVC in x86 Release builds seem to be alligned by 8 bytes
            #define VTBL_IRRELEVANT_BITS 3
        #endif
    #endif
#else
    /// vtbl in G++ seem to be alligned by 16 bytes
    #define VTBL_IRRELEVANT_BITS 4
    // When i defined more virtual functions it became 3 for some reason
    //#define VTBL_IRRELEVANT_BITS 3
#endif
#endif

//------------------------------------------------------------------------------

#if defined(_DEBUG)
#define DEBUG_ONLY(s) s
// Our own version of assert macro because of the fact that normal assert was 
// not always removed in the release builds.
#define XTL_ASSERT(x) if (!(x)) { std::cerr << #x " in file " << __FILE__ << '[' << __LINE__ << ']'; std::exit(42); }
// Our own version of assert macro because of the fact that normal assert was 
// not always removed in the release builds.
#define XTL_VERIFY(x) if (!(x)) std::cerr << #x " in file " << __FILE__ << '[' << __LINE__ << ']'

#else
#define DEBUG_ONLY(s)
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

//------------------------------------------------------------------------------

/// The code that our macros generate may produce some variables that are not
/// necessarily used by the user. This macro is to suppress unused variable warning
#define XTL_UNUSED(x) (void)x;

//------------------------------------------------------------------------------

#if !defined(__func__) && defined(_MSC_VER)
    #define __func__ __FUNCTION__
#endif

//------------------------------------------------------------------------------

/// Helper macro for the following one to resolve macros inside its arguments
#define _STRING_LITERAL(x)  #x
/// Macro to stringize some expression.
#define STRING_LITERAL(x)   _STRING_LITERAL(x)

//------------------------------------------------------------------------------

#ifdef _MSC_VER
    #define XTL_CONCATENATE(A,B) A ## B
#else
    // Somehow operator ## == (or any other) results in error in GCC, so we use 
    // a trick to let users build operator name when they need.
    #define XTL_IDENTITY(A) A
    #define XTL_CONCATENATE(A,B) XTL_IDENTITY(A)XTL_IDENTITY(B)
#endif

//------------------------------------------------------------------------------

// FIX: Yet another version that works in different case, but hasn't been merged
//      with the above yet.
#define XTL_CONCAT(arg1, arg2)  XTL_CONCAT1(arg1, arg2)
#define XTL_CONCAT1(arg1, arg2) XTL_CONCAT2(arg1, arg2)
#define XTL_CONCAT2(arg1, arg2) arg1##arg2

//------------------------------------------------------------------------------

#define XTL_RSEQ_N() 63,62,61,60,59,58,57,56,55,54,53,52,51,50,49,48,47,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0
#define    XTL_ARG_N( _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, _61,_62,_63,      N,...) N 
#define XTL_ARG_N_EX( _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, _61,_62,_63,Dummy,N,...) N 

#ifdef _MSC_VER

    // MS Visual C++ 2005 (in which variadic macros became supported) till at 
    // least MS Visual C++ 2010 has a bug in passing __VA_ARGS__ to subsequent
    // macros as a single token, which results in:
    //     warning C4003: not enough actual parameters for macro 'XTL_ARG_N'
    // and incorrect behavior. The workaround used here is described at:
    // https://connect.microsoft.com/VisualStudio/feedback/details/380090/variadic-macro-replacement
    #define XTL_APPLY_VARIADIC_MACRO(macro,tuple) macro tuple

    // A macro used to count a number of variadic arguments.
    // \note Using this macro without arguments violates 6.10.3p4 of ISO C99 
    //       and thus it cannot be used to detect zero arguments. 
    // \note This macro was invented by Laurent Deniau and can be found here:
    // \see  http://groups.google.com/group/comp.std.c/browse_thread/thread/77ee8c8f92e4a3fb/346fc464319b1ee5
    #define XTL_NARG(...)  XTL_APPLY_VARIADIC_MACRO(XTL_NARG_,(__VA_ARGS__,XTL_RSEQ_N()))
    #define XTL_NARG_(...) XTL_APPLY_VARIADIC_MACRO(XTL_ARG_N,(__VA_ARGS__))

    // The same as above but assumes a dummy first argument that is not counted
    // in order to deal with the fact that regular @XTL_NARG cannot cope with 
    // zero arguments.
    // \note We need here 0 to be a real 0 and not something that evaluates to 0
    //       as is done in some solutions to this problem of NARG, because we 
    //       append this number to a name to form another macro!
    #define XTL_NARG_EX(...)  XTL_APPLY_VARIADIC_MACRO(XTL_NARG_EX_,(__VA_ARGS__,XTL_RSEQ_N()))
    #define XTL_NARG_EX_(...) XTL_APPLY_VARIADIC_MACRO(XTL_ARG_N_EX,(__VA_ARGS__))

#else

    // A macro used to count a number of variadic arguments.
    // \note Using this macro without arguments violates 6.10.3p4 of ISO C99 
    //       and thus it cannot be used to detect zero arguments. 
    // \note This macro was invented by Laurent Deniau and can be found here:
    // \see  http://groups.google.com/group/comp.std.c/browse_thread/thread/77ee8c8f92e4a3fb/346fc464319b1ee5
    #define XTL_NARG(...)  XTL_NARG_(__VA_ARGS__,XTL_RSEQ_N()) 
    #define XTL_NARG_(...) XTL_ARG_N(__VA_ARGS__) 

    // The same as above but assumes a dummy first argument that is not counted
    // in order to deal with the fact that regular @XTL_NARG cannot cope with 
    // zero arguments.
    // \note We need here 0 to be a real 0 and not something that evaluates to 0
    //       as is done in some solutions to this problem of NARG, because we 
    //       append this number to a name to form another macro!
    #define XTL_NARG_EX(...)  XTL_NARG_EX_(__VA_ARGS__,XTL_RSEQ_N()) 
    #define XTL_NARG_EX_(...) XTL_ARG_N_EX(__VA_ARGS__)

#endif

//------------------------------------------------------------------------------

#if !defined(ARR_SIZE)
#define ARR_SIZE(a) (sizeof(a)/sizeof((a)[0]))
#endif

//------------------------------------------------------------------------------

#if defined(_MSC_VER)

    /// Macros to use compiler's branch hinting. 
    /// \note These macros are only to be used in CASE macro expansion, not in 
    ///       user's code since they explicitly expect a pointer argument
    /// \note We use ... (__VA_ARGS__ parameters) to allow expressions 
    ///       containing comma as argument. Essentially this is a one arg macro
    #define   LIKELY_BRANCH(...) (__VA_ARGS__)
    #define UNLIKELY_BRANCH(...) (__VA_ARGS__)

    /// A macro that is supposed to be put before the function definition whose inlining should be disabled
    #define DO_NOT_INLINE_BEGIN __pragma(auto_inline (off))
    /// A macro that is supposed to be put after  the function definition whose inlining should be disabled
    #define DO_NOT_INLINE_END   __pragma(auto_inline (on))
        
#else
    
    /// Macros to use compiler's branch hinting. 
    /// \note These macros are only to be used in CASE macro expansion, not in 
    ///       user's code since they explicitly expect a pointer argument
    /// \note We use ... (__VA_ARGS__ parameters) to allow expressions 
    ///       containing comma as argument. Essentially this is a one arg macro
    #define   LIKELY_BRANCH(...) (__builtin_expect((long int)(__VA_ARGS__), 1))
    #define UNLIKELY_BRANCH(...) (__builtin_expect((long int)(__VA_ARGS__), 0))

    /// A macro that is supposed to be put before the function definition whose inlining should be disabled
    #define DO_NOT_INLINE_BEGIN __attribute__ ((noinline))
    /// A macro that is supposed to be put after  the function definition whose inlining should be disabled
    #define DO_NOT_INLINE_END

#endif

//------------------------------------------------------------------------------
