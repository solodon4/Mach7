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

#pragma once

//------------------------------------------------------------------------------

#if !defined(NDEBUG)
#include <cassert> // Support for assert() macro
#endif

//------------------------------------------------------------------------------

#if !defined(XTL_ASSERT)
  #if !defined(NDEBUG)
    /// By default just pass it to the standard library's assert, but user can override this
    #define XTL_ASSERT(...) assert(__VA_ARGS__)
  #else
    /// Since we bother to provide assume, we assume this default would be better than
    /// whatever assert does in non-debug builds
    #define XTL_ASSERT(...) XTL_ASSUME(__VA_ARGS__)
  #endif
#endif

//------------------------------------------------------------------------------

// This part re-defines various language facilities that can be emulated or
// ignored in older compilers in order to support more compilers for the type
// switching functionality, which does not require C++11.

#if defined(_MSC_VER)
#include "comp.msvc.hpp"    // Microsoft Visual C++ workarounds
#elif defined(__clang__)
#include "comp.clang.hpp"   // Clang workarounds
#elif defined(__GNUC__)
#include "comp.gcc.hpp"     // GNU C++ workarounds
#endif

#include "comp.other.hpp"   // Default workarounds for anything not handled by specialized configs

//------------------------------------------------------------------------------

// Somewhat more centralized way of checking for a support by compiler of some
// language feature.
#define XTL_SUPPORT(feature) XTL_SUPPORT_##feature

//------------------------------------------------------------------------------

#if !XTL_SUPPORT(constexpr) && !defined(constexpr)
/// We do not turn it into inline because of constexpr values
#define constexpr
#endif

#if XTL_SUPPORT(noexcept)
/// Since noexcept might be a macro in our library, we need a different syntax for noexcept specification with condition
#define noexcept_when(...) noexcept(__VA_ARGS__)
/// We also need to distinguish between noexcept operator and noexcept specifier
#define noexcept_of(...)   noexcept(__VA_ARGS__)
#else
/// We just ignore noexcept specification when noexcept is not supported
#define noexcept_when(...)
/// When noexcept is not supported, false for the result of noexcept operator seems to be a safer bet.
#define noexcept_of(...)   false
/// Turn noexcept into throw() for compilers not supporting it
#define noexcept throw()
#endif

#if !XTL_SUPPORT(nullptr) && !defined(nullptr)
/// Turn nullptr into 0 for compilers not supporting it
#define nullptr 0
#endif

#if !XTL_SUPPORT(static_assert) && !defined(static_assert)
/// Emulate static_assert with a typedef trick
#define static_assert(cond,text) typedef char __STATIC_ASSERT__[(cond) ? 1 : -1]
#endif

#if !XTL_SUPPORT(std_is_nothrow_copy_constructible)
/// Emulate std::is_nothrow_copy_constructible<T> by assuming stuff it is applied to always throws
namespace std
{
    // TODO: Optimize for easily-detectible cases like POD etc.
  //template <class T, class... Args> struct is_nothrow_constructible         { static const bool value = false; };
    template <class T>                struct is_nothrow_default_constructible { static const bool value = false; };
    template <class T>                struct is_nothrow_copy_constructible    { static const bool value = false; };
    template <class T>                struct is_nothrow_move_constructible    { static const bool value = false; };
    template <class T, class U>       struct is_nothrow_assignable            { static const bool value = false; };
    template <class T>                struct is_nothrow_copy_assignable       { static const bool value = false; };
    template <class T>                struct is_nothrow_move_assignable       { static const bool value = false; };
    template <class T>                struct is_nothrow_destructible          { static const bool value = false; };
}
#endif

//------------------------------------------------------------------------------
