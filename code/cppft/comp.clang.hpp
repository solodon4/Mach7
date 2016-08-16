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
/// This file provides Clang specific support macros.
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

// Clang C++ workarounds
// For a good summary of compiler versions that support a particular C++ feature
// \see http://www.italiancpp.org/wp-content/uploads/2014/03/CppISO-Feb2014-r1.pdf

#if !defined(__clang__)
#error This file must only be compiled by Clang C++ compiler
#endif

//------------------------------------------------------------------------------

#define XTL_CLANG_VERSION (__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__)

#define XTL_GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)


//------------------------------------------------------------------------------

#if __has_feature(cxx_alignas)
/// Support of alignof(x)
/// \see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2341.pdf
#define XTL_SUPPORT_alignof 1
#endif

//------------------------------------------------------------------------------

#if __has_builtin(alloca)
#define XTL_SUPPORT_alloca 1
#endif

//------------------------------------------------------------------------------

#if __has_feature(cxx_auto_type)
/// Support of type deduction from initializer expression
/// \see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2006/n1984.pdf
#define XTL_SUPPORT_auto 1
#endif

//------------------------------------------------------------------------------

#if __has_feature(cxx_constexpr)
/// Support of constexpr
/// \see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2235.pdf
#define XTL_SUPPORT_constexpr 1
#endif

//------------------------------------------------------------------------------

#if __has_feature(cxx_defaulted_functions) && __has_feature(cxx_deleted_functions)
/// Indicates support of defaulted and deleted functions.
/// \see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2346.htm
#define XTL_SUPPORT_ddf 1
#endif

//------------------------------------------------------------------------------

#if !defined(XTL_SUPPORT_func)
#define XTL_SUPPORT_func 1
#endif

//------------------------------------------------------------------------------

#if __has_feature(cxx_inheriting_constructors)
/// Indicates support of C++11 inheriting constructors
/// \see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2540.htm
#define XTL_SUPPORT_inheriting_constructors 1
#endif

//------------------------------------------------------------------------------

#if __has_feature(cxx_generalized_initializers)
#define XTL_SUPPORT_initializer_list 1
#endif

//------------------------------------------------------------------------------

#if __has_feature(cxx_noexcept)
#define XTL_SUPPORT_noexcept 1
#endif

//------------------------------------------------------------------------------

#if __has_feature(cxx_nullptr)
/// Support of the dedicated name for a null pointer: nullptr
/// \see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2431.pdf
#define XTL_SUPPORT_nullptr 1
#endif

//------------------------------------------------------------------------------

#if __has_feature(cxx_rvalue_references) || __has_extension(cxx_rvalue_references)
/// Support of C++11 range-based for loop
/// \see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2009/n2930.html
#define XTL_SUPPORT_range_for 1
#endif

//------------------------------------------------------------------------------

#if __has_feature(cxx_rvalue_references)
#define XTL_SUPPORT_rvalref 1
#endif

//------------------------------------------------------------------------------

#if __has_feature(cxx_static_assert)
#define XTL_SUPPORT_static_assert 1
#endif

//------------------------------------------------------------------------------
/// Supports variadic templates
/// \see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2242.pdf
#if __has_feature(cxx_variadic_templates)
#define XTL_SUPPORT_variadics 1
#endif

//------------------------------------------------------------------------------

#if !defined(XTL_SUPPORT_vla)
#define XTL_SUPPORT_vla 1
#endif

//------------------------------------------------------------------------------

#if XTL_CLANG_VERSION >= 30100
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

#if __has_builtin(__builtin_assume)
#define XTL_ASSUME(expr) //__builtin_assume(expr)
#endif
#if __has_builtin(__builtin_unreachable)
#define XTL_UNREACHABLE  { XTL_ASSERT(!"Unreachable code"); __builtin_unreachable(); }
#endif

#if !XTL_TRACE_LIKELINESS
    /// Macros to use compiler's branch hinting.
    /// \note These macros are only to be used in Case macro expansion, not in
    ///       user's code since they explicitly expect a pointer argument
    /// \note We use ... (__VA_ARGS__ parameters) to allow expressions
    ///       containing comma as argument. Essentially this is a one arg macro
#if __has_builtin(__builtin_expect)
    #define   XTL_LIKELY(...) (__builtin_expect((long int)(__VA_ARGS__), 1))
    #define XTL_UNLIKELY(...) (__builtin_expect((long int)(__VA_ARGS__), 0))
#endif
#endif

#if __has_attribute(noinline)
/// A macro that is supposed to be put before the function definition whose inlining should be disabled
#define XTL_DO_NOT_INLINE_BEGIN __attribute__ ((noinline))
/// A macro that is supposed to be put after  the function definition whose inlining should be disabled
#define XTL_DO_NOT_INLINE_END
#endif
#if __has_attribute(always_inline)
/// A macro that is supposed to be put before the function definition whose body must be inlined
#define XTL_FORCE_INLINE_BEGIN __attribute__ ((always_inline)) static inline
/// A macro that is supposed to be put after  the function definition whose body must be inlined
#define XTL_FORCE_INLINE_END
#endif
#if __has_attribute(unused)
/// An attribute used in GCC code to silence warning about potentially unused typedef target_type, which we
/// generate to fall back on from Case clauses. The typedef is required in some cases, do not remove.
#define XTL_UNUSED_TYPEDEF __attribute__((unused))
#endif

//------------------------------------------------------------------------------

/// Helper macro to use the sandard _Pragma operator
#define XTL_PRAGMA(x) _Pragma(#x)
#if XTL_MESSAGE_ENABLED
    /// Helper macro to output a message during compilation. GCC prepends file and line info to it anyways
    #define XTL_MESSAGE(str) XTL_PRAGMA(message str)
#endif

    /// Macro to save the settings for diagnostics before the library will modify some
    #define XTL_WARNING_PUSH XTL_PRAGMA(GCC diagnostic push)
    /// Macro to restore the settings for diagnostics after the library has modified some
    #define XTL_WARNING_POP  XTL_PRAGMA(GCC diagnostic pop)
#if __has_warning("-Wall")
    /// Macro that disables all warnings
  //#define XTL_WARNING_IGNORE_ALL                  XTL_PRAGMA(GCC diagnostic ignored "-Wall")
#endif
    /// Macro that disables warning on constant conditional expression, which we have a lot from code generated by macros
    #define XTL_WARNING_IGNORE_CONSTANT_CONDITIONAL
#if __has_warning("-Wstrict-aliasing")
    /// Macro that disables warning on manipulating pointers with reinterpret_cast
    #define XTL_WARNING_IGNORE_STRICT_ALIASING      XTL_PRAGMA(GCC diagnostic ignored "-Wstrict-aliasing") // warning: dereferencing type-punned pointer will break strict-aliasing rules [-Wstrict-aliasing]
#endif

//------------------------------------------------------------------------------

// to avoid deprecation macro
#define register

//------------------------------------------------------------------------------
