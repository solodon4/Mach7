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

#include <boost/variant.hpp>
#include <mach7/type_switchN-patterns-xtl.hpp> // Support for N-ary Match statement on patterns with arbitrary subtyping relation

//----------------------------------------------------------------------------------------------------------------------

#define VARIANT_P(params) BOOST_PP_ENUM_PARAMS(10, params)

// NOTE: We declare vtbl_of in boost namespace because we want it to be found 
//       via two-phase name lookup due to its argument boost::variant
namespace boost
{
#if XTL_SUPPORT(variadics)
    // Default implementation of vtbl_of grabs sizeof(intptr_t) bytes from the beginning
    // of the object, which on 64-bit machines is larger that size of variant's which member.
    template <class... Ts>
    inline std::intptr_t vtbl_of(const boost::variant<Ts...>* p) noexcept { return p->which() + 1; }
#else
    template <VARIANT_P(class Ts)>
    inline std::intptr_t vtbl_of(const boost::variant<VARIANT_P(Ts)>* p) noexcept { return p->which() + 1; }
#endif
}

namespace xtl
{
#if XTL_SUPPORT(variadics)
    template <class... Ts>
    struct is_poly_morphic<boost::variant<Ts...>>
    {
        static const bool value = true;
    };

    template <class S, class T, class... Ts>
    struct is_subtype<S,boost::variant<T,Ts...>>
    {
        static const bool value = is_subtype<S,T>::value
                               || is_subtype<S,boost::variant<Ts...>>::value;
    };
#else
    template <VARIANT_P(class Ts)>
    struct is_poly_morphic<boost::variant<VARIANT_P(Ts)>>
    {
        static const bool value = true;
    };

    template <class S, class T, VARIANT_P(class Ts)>
    struct is_subtype<S,boost::variant<T,VARIANT_P(Ts)>>
    {
        static const bool value = is_subtype<S,T>::value
                               || is_subtype<S,boost::variant<VARIANT_P(Ts)>>::value;
    };
#endif

    template <class S, class T>
    struct is_subtype<S,boost::variant<T>> : is_subtype<S,T> {};

#if XTL_SUPPORT(variadics)
	template <class... Ts, class S>
    inline boost::variant<Ts...> subtype_cast_impl(target<boost::variant<Ts...>>, const S& s)
    {
        return boost::variant<Ts...>(s); // FIX: Actually this should be boost::variant<Ts...>(xtl::subtype_cast<Ti>(s)) where S <: Ti
    }
#else
    template <VARIANT_P(class Ts), class S>
    inline boost::variant<VARIANT_P(Ts)> subtype_cast_impl(target<boost::variant<VARIANT_P(Ts)>>, const S& s)
    {
        return boost::variant<VARIANT_P(Ts)>(s); // FIX: Actually this should be boost::variant<Ts...>(xtl::subtype_cast<Ti>(s)) where S <: Ti
    }
#endif

    template <typename T>
    struct is_subtype_visitor : public boost::static_visitor<T*>
    {
        template <typename S>
        inline T* operator()(S& s) const noexcept
        {
            return xtl::subtype_dynamic_cast<T*>(&s);
        }
    };

#if XTL_SUPPORT(variadics)
    template <class T, class... Ts>
    inline typename std::enable_if<xtl::is_subtype<T, boost::variant<Ts...>>::value, T*>::type
    subtype_dynamic_cast_impl(target<T*>, boost::variant<Ts...>* pv) noexcept
    {
        is_subtype_visitor<T> visitor;
        return boost::apply_visitor(visitor, *pv);
    }

    template <class T, class... Ts>
    inline typename std::enable_if<xtl::is_subtype<T, boost::variant<Ts...>>::value, const T*>::type
    subtype_dynamic_cast_impl(target<const T*>, const boost::variant<Ts...>* pv) noexcept
    {
        is_subtype_visitor<const T> visitor;
        return boost::apply_visitor(visitor, *pv);
    }
#else
    template <class T, VARIANT_P(class Ts)>
    inline typename std::enable_if<xtl::is_subtype<T, boost::variant<VARIANT_P(Ts)>>::value, T*>::type
    subtype_dynamic_cast_impl(target<T*>, boost::variant<VARIANT_P(Ts)>* pv) noexcept
    {
        is_subtype_visitor<T> visitor;
        return boost::apply_visitor(visitor, *pv);
    }

    template <class T, VARIANT_P(class Ts)>
    inline typename std::enable_if<xtl::is_subtype<T, boost::variant<VARIANT_P(Ts)>>::value, const T*>::type
    subtype_dynamic_cast_impl(target<const T*>, const boost::variant<VARIANT_P(Ts)>* pv) noexcept
    {
        is_subtype_visitor<const T> visitor;
        return boost::apply_visitor(visitor, *pv);
    }
#endif
}
