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
/// This file provides GNU C++ specific support macros.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// \see https://parasol.tamu.edu/mach7/
/// \see https://github.com/solodon4/Mach7
/// \see https://github.com/solodon4/SELL
///

#pragma once

#if !defined(XTL_ASSERT)
#error This file should not be included directly. Please #include <cppft/config.hpp> instead
#endif

// GNU C++ workarounds
// For a good summary of compiler versions that support a particular C++ feature
// \see http://www.italiancpp.org/wp-content/uploads/2014/03/CppISO-Feb2014-r1.pdf

#if !defined(__GNUC__)
#error This file must only be compiled by GNU C++ compiler
#endif

//------------------------------------------------------------------------------

#define XTL_GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)


//------------------------------------------------------------------------------

#if !defined(XTL_SUPPORT_alignof)
/// Support of alignof(x)
/// \see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2341.pdf
#define XTL_SUPPORT_alignof 1
#endif

//------------------------------------------------------------------------------

#if !defined(XTL_SUPPORT_alloca)
#define XTL_SUPPORT_alloca 0
#endif

//------------------------------------------------------------------------------

#if XTL_GCC_VERSION >= 40400
/// Support of type deduction from initializer expression
/// \see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2006/n1984.pdf
#define XTL_SUPPORT_auto 1
#endif

//------------------------------------------------------------------------------

#if XTL_GCC_VERSION >= 40600
/// Support of constexpr
/// \see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2235.pdf
#define XTL_SUPPORT_constexpr 1
#endif

//------------------------------------------------------------------------------

#if XTL_GCC_VERSION >= 40400
/// Indicates support of defaulted and deleted functions.
/// \see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2346.htm
#define XTL_SUPPORT_ddf 1
#endif

//------------------------------------------------------------------------------

#if !defined(XTL_SUPPORT_func)
#define XTL_SUPPORT_func 1
#endif

//------------------------------------------------------------------------------

#if XTL_GCC_VERSION >= 40800
/// Indicates support of C++11 inheriting constructors
/// \see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2540.htm
#define XTL_SUPPORT_inheriting_constructors 1
#endif

//------------------------------------------------------------------------------

#if XTL_GCC_VERSION >= 40400
#define XTL_SUPPORT_initializer_list 1
#endif

//------------------------------------------------------------------------------

#if XTL_GCC_VERSION >= 40600
#define XTL_SUPPORT_noexcept 1
#endif

//------------------------------------------------------------------------------

#if XTL_GCC_VERSION >= 40600
/// Support of the dedicated name for a null pointer: nullptr
/// \see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2431.pdf
#define XTL_SUPPORT_nullptr 1
#endif

//------------------------------------------------------------------------------

#if XTL_GCC_VERSION >= 40600
/// Support of C++11 range-based for loop
/// \see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2009/n2930.html
#define XTL_SUPPORT_range_for 1
#endif

//------------------------------------------------------------------------------

#if XTL_GCC_VERSION >= 40300
#define XTL_SUPPORT_rvalref 1
#endif

//------------------------------------------------------------------------------

#if XTL_GCC_VERSION >= 40300
#define XTL_SUPPORT_static_assert 1
#endif

//------------------------------------------------------------------------------
/// Supports variadic templates
/// \see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2242.pdf
#if XTL_GCC_VERSION >= 40300
#define XTL_SUPPORT_variadics 1
#endif

//------------------------------------------------------------------------------

#if XTL_GCC_VERSION >= 900
#define XTL_SUPPORT_vla 1
#endif

//------------------------------------------------------------------------------

#if XTL_GCC_VERSION >= 40700
#define XTL_SUPPORT_std_is_nothrow_copy_constructible 1
#endif

//------------------------------------------------------------------------------

#define XTL_GCC_ONLY(...)     __VA_ARGS__
#define XTL_NON_GCC_ONLY(...)

//------------------------------------------------------------------------------

#if !defined(XTL_IRRELEVANT_VTBL_BITS)
    /// vtbl in G++ seem to be alligned by 16 bytes
    #define XTL_IRRELEVANT_VTBL_BITS 4
#endif

//------------------------------------------------------------------------------

#define XTL_FUNCTION __func__

//------------------------------------------------------------------------------

#define XTL_COUNTER __COUNTER__

//------------------------------------------------------------------------------

#if XTL_GCC_VERSION >= 40500
    #define XTL_ASSUME(expr) if (expr){} else __builtin_unreachable()
    #define XTL_UNREACHABLE { XTL_ASSERT(!"Unreachable code"); __builtin_unreachable(); }
#endif

#if !XTL_TRACE_LIKELINESS
    /// Macros to use compiler's branch hinting. 
    /// \note These macros are only to be used in Case macro expansion, not in 
    ///       user's code since they explicitly expect a pointer argument
    /// \note We use ... (__VA_ARGS__ parameters) to allow expressions 
    ///       containing comma as argument. Essentially this is a one arg macro
    #define   XTL_LIKELY(...) (__builtin_expect((long int)(__VA_ARGS__), 1))
    #define XTL_UNLIKELY(...) (__builtin_expect((long int)(__VA_ARGS__), 0))
#endif

/// A macro that is supposed to be put before the function definition whose inlining should be disabled
#define XTL_DO_NOT_INLINE_BEGIN __attribute__ ((noinline))
/// A macro that is supposed to be put after  the function definition whose inlining should be disabled
#define XTL_DO_NOT_INLINE_END
/// A macro that is supposed to be put before the function definition whose body must be inlined
#define XTL_FORCE_INLINE_BEGIN __attribute__ ((always_inline)) static inline 
/// A macro that is supposed to be put after  the function definition whose body must be inlined
#define XTL_FORCE_INLINE_END

/// An attribute used in GCC code to silence warning about potentially unused typedef target_type, which we
/// generate to fall back on from Case clauses. The typedef is required in some cases, do not remove.
#define XTL_UNUSED_TYPEDEF __attribute__((unused))

//------------------------------------------------------------------------------

/// Helper macro to use the sandard _Pragma operator
#define XTL_PRAGMA(x) _Pragma(#x)
#if XTL_MESSAGE_ENABLED
    /// Helper macro to output a message during compilation. GCC prepends file and line info to it anyways
    #define XTL_MESSAGE(str) XTL_PRAGMA(message str)
#endif

// GCC 4.5.2 at least gives error: #pragma GCC diagnostic not allowed inside functions
#if XTL_GCC_VERSION > 40600
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
#if XTL_GCC_VERSION <= 40500
    #define XTL_CPP0X_TYPENAME
#endif

#if XTL_GCC_VERSION <  40800
    #define XTL_CPP0X_TEMPLATE
#endif

//------------------------------------------------------------------------------
