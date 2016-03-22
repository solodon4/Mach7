//
//  XTL: eXtensible Typing Library
//
//  Copyright 2005 Texas A&M University.
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
/// A minimal version of the XTL library definitions required to support subtyping.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
/// \author Jaakko Jarvi <jarvi@cs.tamu.edu>
///
/// \see https://parasol.tamu.edu/xtl/
/// \see https://github.com/solodon4/SELL
///

#pragma once

#include <cppft/config.hpp> // C++ feature testing and feature emulation
#include <cstddef>      // std::nullptr_t
#include <type_traits>  // std::enable_if
#include <utility>      // std::forward

namespace xtl
{
    // In almost all type systems that define a subtyping relation, it is reflexive (meaning A<:A for any type A)
    // and transitive (meaning that if A<:B and B<:C then A<:C). This makes it a preorder on types.
    // While we can't provide transitivity on the level of a library, we guarantee reflexivity.
    template <class T, class U, class C = void> struct is_subtype : std::is_same<T,U> {};

    // std::nullptr_t is a subtype of all pointer types since nullptr is a value of any pointer type
    template <class T>          struct is_subtype<const std::nullptr_t, const T*> : std::true_type {};
    template <class T>          struct is_subtype<const std::nullptr_t,       T*> : std::true_type {};
    template <class T>          struct is_subtype<      std::nullptr_t, const T*> : std::true_type {};
    template <class T>          struct is_subtype<      std::nullptr_t,       T*> : std::true_type {};

    // void* is a supertype of all pointer types since all pointer types are convertible to it
    template <class S>          struct is_subtype<const S*, const void*> : std::true_type  {};
    template <class S>          struct is_subtype<      S*, const void*> : std::true_type  {};
    template <class S>          struct is_subtype<const S*,       void*> : std::false_type {};
    template <class S>          struct is_subtype<      S*,       void*> : std::true_type  {};

    // Two pointers are in subtyping relations when types pointed to are
    template <class S, class T> struct is_subtype<const S*, const    T*> : is_subtype<S,T> {};
    template <class S, class T> struct is_subtype<      S*, const    T*> : is_subtype<S,T> {};
    template <class S, class T> struct is_subtype<const S*,          T*> : std::false_type {};
    template <class S, class T> struct is_subtype<      S*,          T*> : is_subtype<S,T> {};

    // Derived class is a subtype of a base class
    template <class D, class B>
    struct is_subtype <D, B, typename std::enable_if<std::is_base_of<B,D>::value>::type> : std::true_type {};

    template <typename T>
    struct is_poly_morphic // Ignore weirdness in name for now, workaround of a Visual C++ 2010 compiler bug.
    {
        static const bool value = std::is_polymorphic<T>::value; // Most of the types do not exhibit polymorphic behavior, those that do will have to specialize
    };

    template <typename T>
    struct is_poly_morphic<const T> // Constness does not affect whether type is polymorphic
    {
        static const bool value = is_poly_morphic<T>::value;
    };

    template <typename T>
    struct is_poly_morphic<volatile T> // Volatility does not affect whether type is polymorphic
    {
        static const bool value = is_poly_morphic<T>::value;
    };

    //==============================================================================
    // target class definition
    //==============================================================================

    /// Helper qualifier to overcome problems with overload resolution when only
    /// several argument types are deduced and others are explicitly specified.
    /// This class will be used to wrap explicitly given arguments. Overloaded
    /// subtype_cast_impl have to use this qualifier to match target type T that was
    /// specified by the user in subtype_cast<T>(val).
    template <class T>
    struct target
    {
        /// Type that actually has to be returned by a subtype_cast. Most of the
        /// time it will be the same as target type T, however in some cases it may
        /// be different. An example of such is the result type of casting array
        /// array types, which is T* instead of T[N].By default, result type matches T.
        typedef T type;
    };

    //==============================================================================
    // Forward declarations
    //==============================================================================

    template <class T, class S>
    typename std::enable_if<is_subtype<typename std::remove_reference<S>::type, T>::value, typename target<T>::type>::type
    subtype_cast(S&& s);

    //==============================================================================
    // subtype_cast_impl definition
    //==============================================================================

    /// Helper function to overcome problems with overload resolution when only
    /// several arguments types are deduced and others are explicitly specified.
    /// This function has all of the types in deducible context, so that explicitly
    /// given arguments will be passed through dummy null-pointers.
    /// Default implementation just tries to cast one type to another.
    template <class T, class S>
    inline T subtype_cast_impl(target<T>, const S& s)
    {
        return (T)s;
    }

    //==============================================================================
    // subtype_cast definition
    //==============================================================================

    /// Routine to cast sub-type T to its super-type S.
    /// Extra level of indirection with impl was made to overcome the problem that
    /// when S is not among the arguments generic and more specific specialization
    /// are the same for compiler and it reports ambiguity.
//    template <class T, class S>
//    typename std::enable_if<is_subtype<S, T>::value, typename target<T>::type>::type
//    subtype_cast(const S& s)
//    {
//        // We create a dedicated variable with result because some classes may provide
//        // copy constructors that don't bind to temporaries (e.g. auto_ptr<T>).
//        typename target<T>::type result(subtype_cast_impl(target<T>(), s));
//        std::cout << "subtype_cast<" << typeid(T).name() << ">(" << typeid(S).name() << ") = " << std::endl;
//        return result;
//    }
    template <class T, class S>
    inline typename std::enable_if<is_subtype<typename std::remove_reference<S>::type, T>::value, typename target<T>::type>::type
    subtype_cast(S&& s)
    {
        return subtype_cast_impl(target<T>(), std::forward<S>(s));
    }

    //==============================================================================
    // subtype_dynamic_cast definition
    //==============================================================================

    template <class T>
    inline T* subtype_dynamic_cast_impl(target<T*>, T* p) noexcept
    {
        return p;
    }

    inline std::nullptr_t subtype_dynamic_cast_impl(...) noexcept
    {
        return nullptr;
    }

    template <class D, class B>
    inline typename std::enable_if<std::is_base_of<B, D>::value, D*>::type
    subtype_dynamic_cast_impl(target<D*>, B* p) noexcept
    {
        return dynamic_cast<D*>(p);
    }

    template <class S, class T>
    //inline typename std::enable_if<is_subtype<S, T>::value, S*>::type
    inline S subtype_dynamic_cast(T* t) noexcept
    {
        typename target<S>::type result = subtype_dynamic_cast_impl(target<S>(), t);
        //std::cout << "subtype_dynamic_cast<" << typeid(S).name() << ">(" << typeid(t).name() << ") = " << result << std::endl;
        return result;
    }

} // of namespace xtl
