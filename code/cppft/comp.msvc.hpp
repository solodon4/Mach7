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
/// This file provides Microsoft Visual C++ specific support macros.
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

// MS Visual C++ workarounds
// For a good summary of compiler versions that support a particular C++ feature
// \see http://www.italiancpp.org/wp-content/uploads/2014/03/CppISO-Feb2014-r1.pdf

#if !defined(_MSC_VER)
#error This file must only be compiled by Microsoft Visual C++ compiler
#endif

//------------------------------------------------------------------------------

#if _MSC_VER >= 1700 /// Visual C++ 2012 supports alignof(T)
/// Support of alignof(x)
/// \see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2341.pdf
#define XTL_SUPPORT_alignof 1
#elif !defined(alignof) /// There was an intrinsic function before
#define alignof(T) __alignof(T)
#endif

//------------------------------------------------------------------------------

#if _MSC_VER >= 1400 /// Visual C++ 2005 supports alloca
#define XTL_SUPPORT_alloca 1
#endif

//------------------------------------------------------------------------------

#if _MSC_VER >= 1600 /// Visual C++ 2010 supports auto
/// Support of type deduction from initializer expression
/// \see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2006/n1984.pdf
#define XTL_SUPPORT_auto 1
#endif

//------------------------------------------------------------------------------

#if _MSC_VER >= 1900
/// Support of constexpr
/// \see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2235.pdf
#define XTL_SUPPORT_constexpr 1
#endif

//------------------------------------------------------------------------------

#if _MSC_VER >= 1800 /// Visual C++ 2013 supports deleted and defaulted functions
/// Indicates support of defaulted and deleted functions.
/// \see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2346.htm
#define XTL_SUPPORT_ddf 1
#endif

//------------------------------------------------------------------------------

#if _MSC_VER >= 1900 /// Visual C++ 2014 supports __func__
#define XTL_SUPPORT_func 1
#endif

//------------------------------------------------------------------------------

#if _MSC_VER >= 1900 /// Visual C++ 2014 supports inheriting constructors
/// Indicates support of C++11 inheriting constructors
/// \see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2540.htm
#define XTL_SUPPORT_inheriting_constructors 1
#endif

//------------------------------------------------------------------------------

#if _MSC_VER >= 1800 /// Visual C++ 2013 supports generalized initializers
#define XTL_SUPPORT_initializer_list 1
#endif

//------------------------------------------------------------------------------

#if _MSC_VER >= 1900 /// Visual C++ 2014 supports noexcept
#define XTL_SUPPORT_noexcept 1
#endif

//------------------------------------------------------------------------------

#if _MSC_VER >= 1600 /// Visual C++ 2010 supports nullptr
/// Support of the dedicated name for a null pointer: nullptr
/// \see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2431.pdf
#define XTL_SUPPORT_nullptr 1
#endif

//------------------------------------------------------------------------------

#if _MSC_VER >= 1700 /// Visual C++ 2012 supports range for
/// Support of C++11 range-based for loop
/// \see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2009/n2930.html
#define XTL_SUPPORT_range_for 1
#endif

//------------------------------------------------------------------------------

#if _MSC_VER >= 1600 /// Visual C++ 2010 supports rvalue references
#define XTL_SUPPORT_rvalref 1
#endif

//------------------------------------------------------------------------------

#if _MSC_VER >= 1600 /// Visual C++ 2010 supports static_assert
#define XTL_SUPPORT_static_assert 1
#else                /// Microsoft had a similar macro before
#define static_assert(cond,text) _STATIC_ASSERT(cond)
#endif

//------------------------------------------------------------------------------
/// Supports variadic templates
/// \see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2242.pdf
#if _MSC_VER >= 1800 /// Visual C++ 2013 supports variadic templates
#define XTL_SUPPORT_variadics 1
#endif

//------------------------------------------------------------------------------

#if !defined(XTL_SUPPORT_vla)
#define XTL_SUPPORT_vla 0
#endif

//------------------------------------------------------------------------------

#if _MSC_VER >= 1700 /// Visual C++ 2012 supports std::is_nothrow_copy_constructible<T>
#define XTL_SUPPORT_std_is_nothrow_copy_constructible 1
#endif

//------------------------------------------------------------------------------

#define XTL_MSC_ONLY(...)     __VA_ARGS__
#define XTL_NON_MSC_ONLY(...)

//------------------------------------------------------------------------------

#if !defined(XTL_IRRELEVANT_VTBL_BITS)
/// Number of irrelevant lowest bits in a vtbl-pointer.
/// \note The following section is compiler and platform specific for 
///       optimization purposes.
/// \note As subsequent experiments showed, this value may depend on the number
///       of virtual functions in the class as well. Chosing this value smaller 
///       or larger than necessary, increases the number of collisions and 
///       necessarily degrades performance.
/// FIX: Make this less empirical!
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
#endif

//------------------------------------------------------------------------------

#define XTL_FUNCTION __FUNCTION__

//------------------------------------------------------------------------------

#define XTL_COUNTER __COUNTER__

//------------------------------------------------------------------------------

#define XTL_ASSUME(expr) __assume(expr)
#define XTL_UNREACHABLE  { XTL_ASSERT(!"Unreachable code"); XTL_ASSUME(0); }

#if !XTL_TRACE_LIKELINESS
    /// Macros to use compiler's branch hinting. 
    /// \note These macros are only to be used in Case macro expansion, not in 
    ///       user's code since they explicitly expect a pointer argument
    /// \note We use ... (__VA_ARGS__ parameters) to allow expressions 
    ///       containing comma as argument. Essentially this is a one arg macro
    #define   XTL_LIKELY(...) (__VA_ARGS__)
    #define XTL_UNLIKELY(...) (__VA_ARGS__)
#endif

/// A macro that is supposed to be put before the function definition whose inlining should be disabled
#define XTL_DO_NOT_INLINE_BEGIN __pragma(auto_inline (off))
/// A macro that is supposed to be put after  the function definition whose inlining should be disabled
#define XTL_DO_NOT_INLINE_END   __pragma(auto_inline (on))
/// A macro that is supposed to be put before the function definition whose body must be inlined
#define XTL_FORCE_INLINE_BEGIN __forceinline
/// A macro that is supposed to be put after  the function definition whose body must be inlined
#define XTL_FORCE_INLINE_END

/// An attribute used in GCC code to silence warning about potentially unused typedef target_type, which we
/// generate to fall back on from Case clauses. The typedef is required in some cases, do not remove.
#define XTL_UNUSED_TYPEDEF

//------------------------------------------------------------------------------

/// MSVC10 doesn't seem to support the standard _Pragma operator
#define XTL_PRAGMA(x) __pragma(x)
#if XTL_MESSAGE_ENABLED
    /// Helper macro to output a message during compilation in format understood by Visual Studio
    #define XTL_MESSAGE(str) XTL_PRAGMA(message(__FILE__ "(" XTL_STRING_LITERAL(__LINE__) ") : " str))
#endif
/// Macro to save the settings for diagnostics before the library will modify some
#define XTL_WARNING_PUSH XTL_PRAGMA(warning(push))
/// Macro to restore the settings for diagnostics after the library has modified some
#define XTL_WARNING_POP  XTL_PRAGMA(warning(pop))
/// Macro that disables all warnings
#define XTL_WARNING_IGNORE_ALL                  XTL_PRAGMA(warning( disable ))
/// Macro that disables warning on constant conditional expression, which we have a lot from code generated by macros
#define XTL_WARNING_IGNORE_CONSTANT_CONDITIONAL XTL_PRAGMA(warning( disable : 4127 )) // warning C4127: conditional expression is constant
/// Macro that disables warning on manipulating pointers with reinterpret_cast
#define XTL_WARNING_IGNORE_STRICT_ALIASING      
/// Macro that disables name hiding warnings
#define XTL_WARNING_IGNORE_NAME_HIDING          XTL_PRAGMA(warning( disable : 4456 6246 )) // warning C4456: declaration of 'target_type0' hides previous local declaration

#pragma warning( disable : 4351 ) // warning C4351: new behavior: elements of array ... will be default initialized

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
#if _MSC_VER < 2000
#define XTL_CPP0X_TYPENAME
#endif

//------------------------------------------------------------------------------

/// Visual C++ has check that we do not redefine noexcept, constexpr and other 
/// keywords, which defeats the idea of emulating them silently for old compilers
#define _ALLOW_KEYWORD_MACROS 1

//------------------------------------------------------------------------------
