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
/// Traits for detecting presence of a given member. This file is a part of Mach7 library.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// \see https://parasol.tamu.edu/mach7/
/// \see https://github.com/solodon4/Mach7
/// \see https://github.com/solodon4/SELL
///

#pragma once

///
/// We use here code by Jason Shirk and Alexey Gurtovoy for detecting presence
/// of a given member. See the following discussion for details:
/// http://lists.boost.org/Archives/boost/2002/03/27251.php
///

typedef char (& no_tag)[1];
typedef char (&yes_tag)[2]; 

#ifdef _MSC_VER
// In the version for MSVC the member name has to be taken into ()
#define HAS_MEMBER_XXX(name,member_type,member_name) \
template < typename T, member_type > struct ptmf_##name##_helper {}; \
template < typename T > no_tag  has_member_##name##_helper(...); \
template < typename T > yes_tag has_member_##name##_helper(ptmf_##name##_helper<T, &(T::member_name)>* p); \
template < typename T > \
struct has_member_##name \
{ \
    static const bool value = sizeof(has_member_##name##_helper<T>(0)) == sizeof(yes_tag); \
}
#else
// In the version for GCC () around member name don't work for some reason, but note to keep space before >
#define HAS_MEMBER_XXX(name,member_type,member_name) \
template < typename T, member_type > struct ptmf_##name##_helper {}; \
template < typename T > no_tag  has_member_##name##_helper(...); \
template < typename T > yes_tag has_member_##name##_helper(ptmf_##name##_helper<T, &T::member_name >* p); \
template < typename T > \
struct has_member_##name \
{ \
    static const bool value = sizeof(has_member_##name##_helper<T>(0)) == sizeof(yes_tag); \
}
#endif

/// Helpers to detect use of some of our macros within a #bindings

HAS_MEMBER_XXX(kind_selector,  bool (T::*)() const,  kind_selector_dummy);
HAS_MEMBER_XXX(raise_selector, bool (T::*)() const, raise_selector_dummy);
HAS_MEMBER_XXX(frequency,      bool (T::*)() const,      frequency_dummy);
