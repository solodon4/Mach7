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
/// This file defines compiler-neutral defaults for compiler-specific macros.
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

// Default/other compiler workarounds
// For a good summary of compiler versions that support a particular C++ feature
// \see http://www.italiancpp.org/wp-content/uploads/2014/03/CppISO-Feb2014-r1.pdf

//------------------------------------------------------------------------------

#if !defined(XTL_SUPPORT_alignof)
/// Support of alignof(x)
/// \see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2341.pdf
#define XTL_SUPPORT_alignof 0
#endif

//------------------------------------------------------------------------------

#if !defined(XTL_SUPPORT_alloca)
#define XTL_SUPPORT_alloca 0
#endif

//------------------------------------------------------------------------------

#if !defined(XTL_SUPPORT_auto)
/// Support of type deduction from initializer expression
/// \see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2006/n1984.pdf
#define XTL_SUPPORT_auto 0
#endif

//------------------------------------------------------------------------------

#if !defined(XTL_SUPPORT_constexpr)
/// Support of constexpr
/// \see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2235.pdf
#define XTL_SUPPORT_constexpr 0
#endif

//------------------------------------------------------------------------------

#if !defined(XTL_SUPPORT_ddf)
/// Indicates support of defaulted and deleted functions.
/// \see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2346.htm
#define XTL_SUPPORT_ddf 0
#endif

//------------------------------------------------------------------------------

#if !defined(XTL_SUPPORT_func)
#define XTL_SUPPORT_func 0
#endif

//------------------------------------------------------------------------------

#if !defined(XTL_SUPPORT_inheriting_constructors)
/// Indicates support of C++11 inheriting constructors
/// \see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2540.htm
#define XTL_SUPPORT_inheriting_constructors 0
#endif

//------------------------------------------------------------------------------

#if !defined(XTL_SUPPORT_initializer_list)
#define XTL_SUPPORT_initializer_list 0
#endif

//------------------------------------------------------------------------------

#if !defined(XTL_SUPPORT_noexcept)
#define XTL_SUPPORT_noexcept 0
#endif

//------------------------------------------------------------------------------

#if !defined(XTL_SUPPORT_nullptr)
/// Support of the dedicated name for a null pointer: nullptr
/// \see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2431.pdf
#define XTL_SUPPORT_nullptr 0
#endif

//------------------------------------------------------------------------------

#if !defined(XTL_SUPPORT_range_for)
/// Support of C++11 range-based for loop
/// \see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2009/n2930.html
#define XTL_SUPPORT_range_for 0
#endif

//------------------------------------------------------------------------------

#if !defined(XTL_SUPPORT_rvalref)
#define XTL_SUPPORT_rvalref 0
#endif

//------------------------------------------------------------------------------

#if !defined(XTL_SUPPORT_static_assert)
#define XTL_SUPPORT_static_assert 0
#endif

//------------------------------------------------------------------------------
/// Supports variadic templates
/// \see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2242.pdf
#if !defined(XTL_SUPPORT_variadics)
#define XTL_SUPPORT_variadics 0
#endif

//------------------------------------------------------------------------------

#if !defined(XTL_SUPPORT_vla)
#define XTL_SUPPORT_vla 0
#endif

//------------------------------------------------------------------------------

#if !defined(XTL_SUPPORT_std_is_nothrow_copy_constructible)
#define XTL_SUPPORT_std_is_nothrow_copy_constructible 0
#endif

//------------------------------------------------------------------------------

#if !defined(XTL_MSC_ONLY)
    #define XTL_MSC_ONLY(...)
    #define XTL_NON_MSC_ONLY(...) __VA_ARGS__
#endif

//------------------------------------------------------------------------------

#if !defined(XTL_GCC_ONLY)
    #define XTL_GCC_ONLY(...)
    #define XTL_NON_GCC_ONLY(...) __VA_ARGS__
#endif

//------------------------------------------------------------------------------

#if !defined(XTL_IRRELEVANT_VTBL_BITS)
/// Number of irrelevant lowest bits in a vtbl-pointer.
/// \note We use 2 as default because vtbl-pointer points into an array of function
///       pointers and thus will at least be aligned as pointers.
/// \note As subsequent experiments showed, this value may depend on the number
///       of virtual functions in the class as well. Chosing this value smaller 
///       or larger than necessary, increases the number of collisions and 
///       necessarily degrades performance.
#define XTL_IRRELEVANT_VTBL_BITS 2
#endif

//------------------------------------------------------------------------------

#if !defined(XTL_FUNCTION)
    #define  XTL_FUNCTION XTL_IF(XTL_SUPPORT(func), __func__, "<__func__ not supported>")
#endif

//------------------------------------------------------------------------------

#if !defined(XTL_COUNTER)
    /// When the compiler supports Visual C++ like __COUNTER__ macro that is resolved 
    /// to consequitive number each time it is used, we use it, otherwise we approximate 
    /// it with line number. This means that multiple uses of this macro within the same
    /// line should be memoized in a constant to avoid differences in behavior as well as
    /// the user cannot rely on consequitiveness of numbers even when the counter is 
    /// supported because other macros may use the counter as well.
    #define XTL_COUNTER __LINE__
#endif

//------------------------------------------------------------------------------

#if !defined(XTL_CONCATENATE)
    /// Somehow operator ## == (or any other) results in error in GCC, so we use
    /// a trick to let users build operator name when they need.
    #define XTL_IDENTITY(A) A
    #define XTL_CONCATENATE(A,B) XTL_IDENTITY(A)XTL_IDENTITY(B)
#endif

//------------------------------------------------------------------------------

#if !defined(XTL_ASSUME)
    #define  XTL_ASSUME(expr)
#endif

#if !defined(XTL_UNREACHABLE)
    #define  XTL_UNREACHABLE  { XTL_ASSERT(!"Unreachable code"); XTL_ASSUME(0); }
#endif

//------------------------------------------------------------------------------

#if !defined(XTL_LIKELY) && !XTL_TRACE_LIKELINESS
    /// Macros to use compiler's branch hinting. 
    /// \note These macros are only to be used in Case macro expansion, not in 
    ///       user's code since they explicitly expect a pointer argument
    /// \note We use ... (__VA_ARGS__ parameters) to allow expressions 
    ///       containing comma as argument. Essentially this is a one arg macro
    #define   XTL_LIKELY(...) (__VA_ARGS__)
    #define XTL_UNLIKELY(...) (__VA_ARGS__)
#endif

#if !defined(XTL_DO_NOT_INLINE_BEGIN)
    /// A macro that is supposed to be put before the function definition whose inlining should be disabled
    #define XTL_DO_NOT_INLINE_BEGIN
    /// A macro that is supposed to be put after  the function definition whose inlining should be disabled
    #define XTL_DO_NOT_INLINE_END
    /// A macro that is supposed to be put before the function definition whose body must be inlined
    #define XTL_FORCE_INLINE_BEGIN
    /// A macro that is supposed to be put after  the function definition whose body must be inlined
    #define XTL_FORCE_INLINE_END
#endif

#if !defined(XTL_UNUSED_TYPEDEF)
    /// An attribute used in GCC code to silence warning about potentially unused typedef target_type, which we
    /// generate to fall back on from Case clauses. The typedef is required in some cases, do not remove.
    #define  XTL_UNUSED_TYPEDEF
#endif

//------------------------------------------------------------------------------

#if !defined(XTL_PRAGMA)
    /// Helper macro to use the sandard _Pragma operator
    #define  XTL_PRAGMA(x)
#endif
#if !defined(XTL_MESSAGE)
    /// Helper macro to output a message during compilation. GCC prepends file and line info to it anyways
    #define  XTL_MESSAGE(str)
#endif
#if !defined(XTL_WARNING_PUSH)
    /// Macro to save the settings for diagnostics before the library will modify some
    #define  XTL_WARNING_PUSH
#endif
#if !defined(XTL_WARNING_POP)
    /// Macro to restore the settings for diagnostics after the library has modified some
    #define  XTL_WARNING_POP
#endif
#if !defined(XTL_WARNING_IGNORE_ALL)
    /// Macro that disables all warnings
  //#define  XTL_WARNING_IGNORE_ALL
#endif
#if !defined(XTL_WARNING_IGNORE_CONSTANT_CONDITIONAL)
    /// Macro that disables warning on constant conditional expression, which we have a lot from code generated by macros
    #define  XTL_WARNING_IGNORE_CONSTANT_CONDITIONAL
#endif
#if !defined(XTL_WARNING_IGNORE_STRICT_ALIASING)
    /// Macro that disables warning on manipulating pointers with reinterpret_cast
    #define  XTL_WARNING_IGNORE_STRICT_ALIASING
#endif
#if !defined(XTL_WARNING_IGNORE_NAME_HIDING)
    /// Macro that disables name hiding warnings
    #define XTL_WARNING_IGNORE_NAME_HIDING
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
#if !defined(XTL_CPP0X_TYPENAME)
    #define XTL_CPP0X_TYPENAME typename
#endif

#if !defined(XTL_CPP0X_TEMPLATE)
    #define XTL_CPP0X_TEMPLATE template
#endif

//------------------------------------------------------------------------------
