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
/// This file defines various macros that are used generate or select parts of code.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// \see https://parasol.tamu.edu/mach7/
/// \see https://github.com/solodon4/Mach7
/// \see https://github.com/solodon4/SELL
///

#pragma once

//------------------------------------------------------------------------------

/// The following set of macros was copied verbatim from a response by Moritz Beutel
/// here: https://forums.embarcadero.com/message.jspa?messageID=320338
/// It is used for suppressing , (comma) when argument is added to __VA_ARGS__, which
/// might be empty.
#define XTL_AND(a,b) XTL_CAT3(_XTL_AND_, a, b)
#define XTL_NOT(a) XTL_CAT2(_XTL_NOT_, a)
#define XTL_IF(a,b,...) XTL_CAT2(_XTL_IF_, a)(b, __VA_ARGS__)

#define XTL_EMPTY()
#define XTL_EXPAND(...) __VA_ARGS__
#define XTL_1ST(a,...) a
#define XTL_2ND(a,b,...) b
#define XTL_REMOVE_1ST(a,...) __VA_ARGS__
#define XTL_PAIR(a,...) a __VA_ARGS__
#define XTL_CAT2(a,...) XTL_CAT2_(a, __VA_ARGS__)
#define XTL_CAT3(a,b,...) XTL_CAT3_(a, b, __VA_ARGS__)

/// The two macros below are inspired by Laurent Deniau's posting on comp.lang.c from 2006/09/27
/// http://groups.google.com/group/comp.lang.c/browse_thread/thread/578912299f8f87ce#msg_937356effc43f569
#define XTL_IS_EMPTY(...) \
XTL_AND(XTL_IS_LIST(__VA_ARGS__ ()),XTL_NOT(XTL_IS_LIST(__VA_ARGS__ _)))
#define XTL_IS_LIST(...) \
XTL_PAIR(XTL_1ST, (XTL_CAT2(XTL_IS_LIST_RET_, XTL_IS_LIST_TST_ __VA_ARGS__)))


/// implementation details

#define XTL_IS_LIST_TST_(...) 1
#define XTL_IS_LIST_RET_XTL_IS_LIST_TST_ 0,
#define XTL_IS_LIST_RET_1 1,

#define XTL_CAT2_(a,...) XTL_EXPAND(a ## __VA_ARGS__)
#define XTL_CAT3_(a,b,...) XTL_EXPAND(a ## b ## __VA_ARGS__)

#define _XTL_AND_00 0
#define _XTL_AND_01 0
#define _XTL_AND_10 0
#define _XTL_AND_11 1

#define _XTL_IF_0(a,...) __VA_ARGS__
#define _XTL_IF_1(a,...) a

#define _XTL_NOT_0 1
#define _XTL_NOT_1 0

//------------------------------------------------------------------------------

/// MS Visual C++ 2005 (in which variadic macros became supported) till at
/// least MS Visual C++ 2010 has a bug in passing __VA_ARGS__ to subsequent
/// macros as a single token, which results in:
///     warning C4003: not enough actual parameters for macro 'XTL_ARG_N'
/// and incorrect behavior. The workaround used here is described at:
/// https://connect.microsoft.com/VisualStudio/feedback/details/380090/variadic-macro-replacement
#define XTL_APPLY_VARIADIC_MACRO(macro,tuple) macro tuple

//------------------------------------------------------------------------------

#define XTL_REPEAT_WITH_0(s,m,...)
#define XTL_REPEAT_WITH_1(s,m,...)                                      m(0,__VA_ARGS__)
#define XTL_REPEAT_WITH_2(s,m,...) XTL_REPEAT_WITH_1(s,m,__VA_ARGS__) s m(1,__VA_ARGS__)
#define XTL_REPEAT_WITH_3(s,m,...) XTL_REPEAT_WITH_2(s,m,__VA_ARGS__) s m(2,__VA_ARGS__)
#define XTL_REPEAT_WITH_4(s,m,...) XTL_REPEAT_WITH_3(s,m,__VA_ARGS__) s m(3,__VA_ARGS__)
#define XTL_REPEAT_WITH_5(s,m,...) XTL_REPEAT_WITH_4(s,m,__VA_ARGS__) s m(4,__VA_ARGS__)
#define XTL_REPEAT_WITH_6(s,m,...) XTL_REPEAT_WITH_5(s,m,__VA_ARGS__) s m(5,__VA_ARGS__)
#define XTL_REPEAT_WITH_7(s,m,...) XTL_REPEAT_WITH_6(s,m,__VA_ARGS__) s m(6,__VA_ARGS__)
#define XTL_REPEAT_WITH_8(s,m,...) XTL_REPEAT_WITH_7(s,m,__VA_ARGS__) s m(7,__VA_ARGS__)
#define XTL_REPEAT_WITH_9(s,m,...) XTL_REPEAT_WITH_8(s,m,__VA_ARGS__) s m(8,__VA_ARGS__)
#define XTL_REPEAT_WITH_10(s,m,...) XTL_REPEAT_WITH_9(s,m,__VA_ARGS__) s m(9,__VA_ARGS__)

/// Repeats macro m(i) n times (separated by s) with i in [0..n-1]
#define XTL_REPEAT_WITH(s,n,m,...) XTL_REPEAT_WITH_ ## n(s,m,__VA_ARGS__)
/// Equivalent to #XTL_REPEAT_WITH with empty separator
#define XTL_REPEAT(n,m,...) XTL_REPEAT_WITH(,n,m,__VA_ARGS__)

#define XTL_ENUM_0(m,...)
#define XTL_ENUM_1(m,...)                             m(0,__VA_ARGS__)
#define XTL_ENUM_2(m,...) XTL_ENUM_1(m,__VA_ARGS__) , m(1,__VA_ARGS__)
#define XTL_ENUM_3(m,...) XTL_ENUM_2(m,__VA_ARGS__) , m(2,__VA_ARGS__)
#define XTL_ENUM_4(m,...) XTL_ENUM_3(m,__VA_ARGS__) , m(3,__VA_ARGS__)
#define XTL_ENUM_5(m,...) XTL_ENUM_4(m,__VA_ARGS__) , m(4,__VA_ARGS__)
#define XTL_ENUM_6(m,...) XTL_ENUM_5(m,__VA_ARGS__) , m(5,__VA_ARGS__)
#define XTL_ENUM_7(m,...) XTL_ENUM_6(m,__VA_ARGS__) , m(6,__VA_ARGS__)
#define XTL_ENUM_8(m,...) XTL_ENUM_7(m,__VA_ARGS__) , m(7,__VA_ARGS__)
#define XTL_ENUM_9(m,...) XTL_ENUM_8(m,__VA_ARGS__) , m(8,__VA_ARGS__)

/// Equivalent to #XTL_REPEAT_WITH with separator being a comma
#define XTL_ENUM(n,m,...) XTL_ENUM_ ## n(m,__VA_ARGS__)

//------------------------------------------------------------------------------

#define XTL_SELECT_ARG_0(a0, ...) a0
#define XTL_SELECT_ARG_1(a0,a1, ...) a1
#define XTL_SELECT_ARG_2(a0,a1,a2, ...) a2
#define XTL_SELECT_ARG_3(a0,a1,a2,a3, ...) a3
#define XTL_SELECT_ARG_4(a0,a1,a2,a3,a4, ...) a4
#define XTL_SELECT_ARG_5(a0,a1,a2,a3,a4,a5, ...) a5
#define XTL_SELECT_ARG_6(a0,a1,a2,a3,a4,a5,a6, ...) a6
#define XTL_SELECT_ARG_7(a0,a1,a2,a3,a4,a5,a6,a7, ...) a7
#define XTL_SELECT_ARG_8(a0,a1,a2,a3,a4,a5,a6,a7,a8, ...) a8
#define XTL_SELECT_ARG_9(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9, ...) a9

#define XTL_SELECT_ARG(i, ...) XTL_APPLY_VARIADIC_MACRO(XTL_SELECT_ARG_ ## i,(__VA_ARGS__,XTL_EMPTY()))

//------------------------------------------------------------------------------

#define XTL_SELECT_RST_0(a0, ...) __VA_ARGS__
#define XTL_SELECT_RST_1(a0,a1, ...) __VA_ARGS__
#define XTL_SELECT_RST_2(a0,a1,a2, ...) __VA_ARGS__
#define XTL_SELECT_RST_3(a0,a1,a2,a3, ...) __VA_ARGS__
#define XTL_SELECT_RST_4(a0,a1,a2,a3,a4, ...) __VA_ARGS__
#define XTL_SELECT_RST_5(a0,a1,a2,a3,a4,a5, ...) __VA_ARGS__
#define XTL_SELECT_RST_6(a0,a1,a2,a3,a4,a5,a6, ...) __VA_ARGS__
#define XTL_SELECT_RST_7(a0,a1,a2,a3,a4,a5,a6,a7, ...) __VA_ARGS__
#define XTL_SELECT_RST_8(a0,a1,a2,a3,a4,a5,a6,a7,a8, ...) __VA_ARGS__
#define XTL_SELECT_RST_9(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9, ...) __VA_ARGS__

#define XTL_SELECT_RST(i, ...) XTL_APPLY_VARIADIC_MACRO(XTL_SELECT_RST_ ## i,(__VA_ARGS__))

//------------------------------------------------------------------------------

#define XTL_PREV_0  00
#define XTL_PREV_1  0
#define XTL_PREV_2  1
#define XTL_PREV_3  2
#define XTL_PREV_4  3
#define XTL_PREV_5  4
#define XTL_PREV_6  5
#define XTL_PREV_7  6
#define XTL_PREV_8  7
#define XTL_PREV_9  8
#define XTL_PREV_10 9

#define XTL_PREV(i) XTL_PREV_##i

//------------------------------------------------------------------------------

#define XTL_SUCC_0 1
#define XTL_SUCC_1 2
#define XTL_SUCC_2 3
#define XTL_SUCC_3 4
#define XTL_SUCC_4 5
#define XTL_SUCC_5 6
#define XTL_SUCC_6 7
#define XTL_SUCC_7 8
#define XTL_SUCC_8 9
#define XTL_SUCC_9 10
#define XTL_SUCC_10 PLEASE_EXTEND_XTL_SUCC

#define XTL_SUCC(i) XTL_SUCC_##i

