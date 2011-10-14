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
// Our own version of assert macro because of the fact that normal assert was 
// not always removed in the release builds.
#define XTL_ASSERT(x)
// Our own version of assert macro because of the fact that normal assert was 
// not always removed in the release builds.
#define XTL_VERIFY(x) if (!(x)) std::cerr << #x " in file " << __FILE__ << '[' << __LINE__ << ']'

#endif

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

#if !defined(ARR_SIZE)
#define ARR_SIZE(a) (sizeof(a)/sizeof((a)[0]))
#endif

//------------------------------------------------------------------------------

#if defined(_MSC_VER)

    /// Macros to use compiler's branch hinting. 
    /// \note These macros are only to be used in CASE macro expansion, not in 
    ///       user's code since they explicitly expect a pointer argument
    #define   LIKELY_BRANCH(ptr) (ptr)
    #define UNLIKELY_BRANCH(ptr) (ptr)

    /// A macro that is supposed to be put before the function definition whose inlining should be disabled
    #define DO_NOT_INLINE_BEGIN __pragma(auto_inline (off))
    /// A macro that is supposed to be put after  the function definition whose inlining should be disabled
    #define DO_NOT_INLINE_END   __pragma(auto_inline (on))
        
#else
    
    /// Macros to use compiler's branch hinting. 
    /// \note These macros are only to be used in CASE macro expansion, not in 
    ///       user's code since they explicitly expect a pointer argument
    #define   LIKELY_BRANCH(ptr) (__builtin_expect((ptr) != 0, 1))
    #define UNLIKELY_BRANCH(ptr) (__builtin_expect((ptr) != 0, 0))

    /// A macro that is supposed to be put before the function definition whose inlining should be disabled
    #define DO_NOT_INLINE_BEGIN __attribute__ ((noinline))
    /// A macro that is supposed to be put after  the function definition whose inlining should be disabled
    #define DO_NOT_INLINE_END

#endif

//------------------------------------------------------------------------------
