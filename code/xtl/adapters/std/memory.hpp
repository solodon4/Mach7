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
/// This file is a part of Mach7 library test suite.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// \see https://parasol.tamu.edu/xtl/
/// \see https://parasol.tamu.edu/mach7/
/// \see https://github.com/solodon4/Mach7
/// \see https://github.com/solodon4/SELL
///

#pragma once

#include <xtl/xtl.hpp>   // XTL subtyping definitions
#include <memory>

//------------------------------------------------------------------------------

namespace xtl
{
    // Establish shubtyping relationship between unique_ptr<T> <: T*
    template <class S, class T>
    struct is_subtype<std::unique_ptr<S>,T*>
    {
        static const bool value = is_subtype<S*,T*>::value;
    };

    // Implement the conversion from subtype unique_ptr<T> to the super type T*
    template <class T, class S>
    inline T* subtype_cast_impl(target<T*>, const std::unique_ptr<S>& s)
    {
        return xtl::subtype_cast<T*>(s.get());
    }

    // Since nullptr is a valid value for all pointer-types, we establish the
    // subtyping relation between nullptr_t <: unique_ptr<T> for any T
    template <class T>
    struct is_subtype<std::nullptr_t, std::unique_ptr<T>>
    {
        static const bool value = true;
    };

    // Might not be needed in a real-world use, but for completeness also const nullptr_t
    template <class T>
    struct is_subtype<const std::nullptr_t, std::unique_ptr<T>>
    {
        static const bool value = true;
    };

    // Implements the conversion from subtype nullptr_t to super type unique_ptr<T>
    template <class T>
    inline std::unique_ptr<T> subtype_cast_impl(target<std::unique_ptr<T>>, const std::nullptr_t&)
    {
        return std::unique_ptr<T>(nullptr);
    }
}

//------------------------------------------------------------------------------

namespace xtl
{
    // Establish shubtyping relationship between shared_ptr<T> <: T*
    template <class S, class T>
    struct is_subtype<std::shared_ptr<S>,T*>
    {
        static const bool value = is_subtype<S*,T*>::value;
    };

    // Implement the conversion from subtype shared_ptr<T> to the super type T*
	template <class T, class S>
    inline T* subtype_cast_impl(target<T*>, const std::shared_ptr<S>& s)
    {
        return xtl::subtype_cast<T*>(s.get());
    }

    // Since nullptr is a valid value for all pointer-types, we establish the
    // subtyping relation between nullptr_t <: shared_ptr<T> for any T
    template <class T>
    struct is_subtype<std::nullptr_t, std::shared_ptr<T>>
    {
        static const bool value = true;
    };

    // Might not be needed in a real-world use, but for completeness also const nullptr_t
    template <class T>
    struct is_subtype<const std::nullptr_t, std::shared_ptr<T>>
    {
        static const bool value = true;
    };

    // Implements the conversion from subtype nullptr_t to super type shared_ptr<T>
	template <class T>
    inline std::shared_ptr<T> subtype_cast_impl(target<std::shared_ptr<T>>, const std::nullptr_t& s)
    {
        return std::shared_ptr<T>(nullptr);
    }
}

//------------------------------------------------------------------------------
