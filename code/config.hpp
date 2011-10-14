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
//#define TRACE_PERFORMANCE

//------------------------------------------------------------------------------

#define VTBL_DEFAULT_CACHE_BITS 7

//------------------------------------------------------------------------------

// FIX: Make this less empirical!
#ifdef _MSC_VER
    #if defined(_WIN64) && !defined(_DEBUG)
    #define VTBL_IRRELEVANT_BITS 4  ///< vtbl in MSVC in x64 Release builds seem to be alligned by 8 bytes
    #else
    #define VTBL_IRRELEVANT_BITS 3  ///< vtbl in MSVC seem to be alligned by 4 bytes
    #endif
#else
    #define VTBL_IRRELEVANT_BITS 4  ///< vtbl in G++ seem to be alligned by 8 bytes
#endif

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

#ifdef _DEBUG
#define DEBUG_ONLY(s) s
// Our own version of assert macro because of the fact that normal assert was 
// not always removed in the release builds.
#define XTL_ASSERT(x) if (!(x)) std::cerr << #x " in file " << __FILE__ << '[' << __LINE__ << ']'
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

#ifdef _MSC_VER

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
