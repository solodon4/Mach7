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
/// \file bindings.hpp
///
/// Classes and macros used to define and work with bindings.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// \see https://parasol.tamu.edu/mach7/
/// \see https://github.com/solodon4/Mach7
/// \see https://github.com/solodon4/SELL
///

#pragma once

#include <cstddef>
#include "../metatools.hpp"
//#include <tuple>

namespace mch ///< Mach7 library namespace
{

//------------------------------------------------------------------------------

/// Predefined value representing a layout used by default when none is specified.
enum { default_layout = size_t(~0) };

/// Traits like structure that will define which members should be matched at
/// which positions. It is intentionally left undefined as user will have to
/// provide specializations for his hierarchy.
template <typename type_being_matched, size_t layout = default_layout>
struct bindings;

#if !defined(XTL_USE_GENERIC_BINDINGS)
/// A temporary switch while we experiment with generic bindings to be able to generate both kinds
#define XTL_USE_GENERIC_BINDINGS 0
#endif

struct MyClass { int a; float b; }; // User-defined class, which we are decomposing

template <typename T>
struct reflection
{
    template <size_t N>
    struct member
    {
    };
};

#if XTL_SUPPORT(constexpr) && 0 // Errors in VS 2015, but this was test code anyways
template <>
struct reflection<MyClass>
{
    template <size_t N, typename dummy = void> struct member {};
    template <typename dummy> struct member<0,dummy> { static constexpr auto value = &MyClass::a; };
    template <typename dummy> struct member<1,dummy> { static constexpr auto value = &MyClass::b; };
};

template <typename... T>
struct reflection<std::tuple<T...>>
{
    template <size_t N> struct member { static constexpr auto value = static_cast<typename std::tuple_element<N, std::tuple<T...>>::type& (*)(std::tuple<T...>&)>(&std::get<N>); };
};

auto t = std::make_tuple(42, 3.14);
auto v = reflection<decltype(t)>::member<0>::value(t); // static_cast<std::tuple_element<1, decltype(t)>::type& (*)(decltype(t)&)>(&std::get<1>);
#endif
//template <> template <> struct reflection<MyClass>::member<0> { static constexpr auto value = &MyClass::a; };
//template <> template <> struct reflection<MyClass>::member<1> { static constexpr auto value = &MyClass::b; };
//
//template <typename... T> template <size_t N> struct reflection<std::tuple<T...>>::member { static constexpr auto value = 9; };

//template <size_t N, typename T>
//void member()
//{
//    static_assert(N + N !=  2*N, "This type does not provide bindings"); // Always assert, only specializations of this class should be used
//};
//
//template <> inline decltype(unary(&MyClass::a)) member<0,MyClass>() { return unary(&MyClass::a); }
//template <> inline decltype(unary(&MyClass::b)) member<1,MyClass>() { return unary(&MyClass::b); }


//template <> struct bindings<MyClass>
//{
//    template <size_t N> static inline void member() { static_assert(N + N !=  2*N, "This type does not provide bindings"); }
//    template <> static inline decltype(unary(&MyClass::a)) member<0>() { return unary(&MyClass::a); }
//    template <> static inline decltype(unary(&MyClass::b)) member<1>() { return unary(&MyClass::b); }
//};

//    template <> template <> inline decltype(unary(&MyClass::a)) bindings<MyClass>::member<0>() { return unary(&MyClass::a); }
//    template <> template <> inline decltype(unary(&MyClass::b)) bindings<MyClass>::member<1>() { return unary(&MyClass::b); }

//------------------------------------------------------------------------------

/// A wrapper class that lets one pack a given layout and type into a view.
template <typename type_being_matched, size_t layout = default_layout>
struct view
{
    typedef type_being_matched target_type;
    static const size_t        target_layout = layout;
};

//------------------------------------------------------------------------------

/// Helper meta-function to extract target type and layout from a given type
/// that can be view.
template <typename T>
struct target_of
{
    typedef T type;
    enum { layout = default_layout };
};

/// The special case of view
template <typename T, size_t L>
struct target_of<view<T,L>>
{
    typedef T type;
    enum { layout = L };
};

//------------------------------------------------------------------------------

#if !defined(Members)
 #define CMa(I,...) CM(I,XTL_SELECT_ARG(I,__VA_ARGS__))
 #if defined(_MSC_VER)
  // Note: We do this extra level of indirection with MembersN to be able to handle expansion of XTL_NARG in Visual C++ properly.
  #define MembersN(N,...) XTL_APPLY_VARIADIC_MACRO(XTL_REPEAT,(N,CMa,__VA_ARGS__))
  #if !XTL_USE_GENERIC_BINDINGS
	#define Members(...)                                                              MembersN(XTL_NARG(__VA_ARGS__),__VA_ARGS__)
  #else
    #define Members(...) template <size_t N, typename dummy = void> struct member {}; MembersN(XTL_NARG(__VA_ARGS__),__VA_ARGS__)
  #endif
 #else
  // FIX: for whatever reason the weaker workaround for Visual C++ doesn't work in Clang, which is normally not the case.
  //      Hence an explicit general implementation for non-Visual C++ builds has to be preserved till fixed.
  #if !XTL_USE_GENERIC_BINDINGS
    #define Members(...)                                                              XTL_REPEAT(XTL_NARG(__VA_ARGS__),CMa,__VA_ARGS__)
  #else
    #define Members(...) template <size_t N, typename dummy = void> struct member {}; XTL_REPEAT(XTL_NARG(__VA_ARGS__),CMa,__VA_ARGS__)
  #endif
 #endif
#else
  #if XTL_MESSAGE_ENABLED
    #error Macro Members, used by Mach7 pattern-matching library, has already been defined
  #endif
#endif

//------------------------------------------------------------------------------

#if !defined(CM)
  #if !XTL_USE_GENERIC_BINDINGS
    /// \deprecated This macro is now deprecated, use Members(m1,..., mN) instead!
    /// Internal macro used by Members(...) to define member's position within decomposition of a given data type
    /// Example: CM(0,MyClass::member) or CM(1,external_func)
    /// \note Use this macro only inside specializations of #bindings
    /// \note The macro should be followed by a semicolon!
    /// \note We use variadic macro parameter here in order to be able to handle 
    ///       templates, which might have commas, otherwise just a second argument
    ///       would be sufficient.
    /// \note #unary is used here as an identity to disambiguate the necessary
    ///       [member-]function when multiple exist. For example: complex<T> has
    ///       two member functions imag() - one with no arugment and one with one
    ///       argument. We are only interested with the one without argument and
    ///       putting #unary here around taking the address of it saves the user
    ///       from having to disambiguate explicitly.
    #define CM(Index,...)                                           \
        static inline decltype(unary(&__VA_ARGS__)) member##Index() noexcept \
        {                                                           \
            return unary(&__VA_ARGS__);                             \
        }
  #else
    #define CM(Index,...)                                           \
    template <typename dummy> struct member<Index,dummy> { static constexpr auto value = unary(&__VA_ARGS__); }; \
    static inline decltype(member<Index>::value) member##Index() noexcept { return member<Index>::value; }
  #endif
#else
  #if XTL_MESSAGE_ENABLED
    #error Macro CM, used by Mach7 pattern-matching library, has already been defined
  #endif
#endif

#if !defined(KS)
    /// Macro to define a kind selector - a member of the common base class that 
    /// carries a distinct integral value that uniquely identifies the derived 
    /// type.  Used in the decomposition of the base class.
    /// \note Use this macro only inside specializations of #bindings
    /// \note The macro should be followed by a semicolon!
    /// \note We use variadic macro parameter here in order to be able to handle 
    ///       templates, which might have commas, otherwise just a single argument
    ///       would be sufficient.
    /// FIX: KS doesn't accept now members qualified with base class, but CM does, check why.
    #define KS(...)                                                 \
        static inline decltype(unary(&__VA_ARGS__)) kind_selector() noexcept \
        {                                                           \
            return unary(&__VA_ARGS__);                             \
        }                                                           \
        bool kind_selector_dummy() const noexcept
#else
  #if XTL_MESSAGE_ENABLED
    #error Macro KS, used by Mach7 pattern-matching library, has already been defined
  #endif
#endif

#if !defined(KV)
    /// Macro to define an integral constant that uniquely identifies the derived 
    /// class. Used in the decomposition of a derived class. 
    /// \param T -- the first argument must indicate the root of the hierarchy -- 
    ///        the least derived class for which tag values given in second 
    ///        argument are unique, which is also the class whose bindings define 
    ///        the KS macro.
    /// \note Use this macro only inside specializations of #bindings
    /// \note The macro should be followed by a semicolon!
    /// \note We use variadic macro parameter here in order to be able to handle 
    ///       templates, which might have commas, otherwise just a single argument
    ///       would be sufficient.
    #define KV(T, ...) typedef T root_type; enum { kind_value = __VA_ARGS__ }
#else
  #if XTL_MESSAGE_ENABLED
    #error Macro KV, used by Mach7 pattern-matching library, has already been defined
  #endif
#endif

#if !defined(FQ)
    /// Macro to define an expected frequency of objects of a given class.
    /// The exact values are not important as they are turned into probabilities by
    /// dividing these numbers onto sum of frequencies of all classes, statically
    /// allowable by a given match statement (derived from a source type).
    /// \note Use this macro only inside specializations of #bindings
    /// \note The macro should be followed by a semicolon!
    /// \note We use variadic macro parameter here in order to be able to handle 
    ///       templates, which might have commas, otherwise just a single argument
    ///       would be sufficient.
    #define FQ(...) enum { fq = __VA_ARGS__ }
#else
  #if XTL_MESSAGE_ENABLED
    #error Macro FQ, used by Mach7 pattern-matching library, has already been defined
  #endif
#endif

#if !defined(FQS)
    /// Macro to define a function that returns expected frequency of a given type.
    /// The syntax of the function should be: size_t f(const source_type&);
    /// \note The macro should be followed by a semicolon!
    /// FIX: Make sure there is no re-entrancy problems when pattern matching is
    ///      used inside of the frequency function f to uncover source type.
    #define FQS(f)                                                  \
        static size_t frequency(intptr_t vtbl) noexcept             \
        {                                                           \
            return ::f(*reinterpret_cast<const XTL_CPP0X_TYPENAME get_param<bindings>::type*>(&vtbl)); \
        }                                                           \
        bool frequency_dummy() const noexcept
#else
  #if XTL_MESSAGE_ENABLED
    #error Macro FQS, used by Mach7 pattern-matching library, has already been defined
  #endif
#endif

#if !defined(RS)
    /// Macro to define a raise selector - a virtual member function of the common
    /// base class that implements a polymorphic exception idiom (\see 
    /// http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Polymorphic_Exception). 
    /// Essentially it's a virtual member function (e.g. virtual void raise() const = 0;)
    /// that will be overriden in all subclasses in the following way:
    /// void SubClass::raise() const { throw *this; }
    /// \note Use this macro only inside specializations of #bindings
    /// \note The macro should be followed by a semicolon!
    /// \note We use variadic macro parameter here in order to be able to handle 
    ///       templates, which might have commas, otherwise just a single argument
    ///       would be sufficient.
    #define RS(...)                                                 \
        static inline decltype(unary(&__VA_ARGS__)) raise_selector() noexcept\
        {                                                           \
            return unary(&__VA_ARGS__);                             \
        }                                                           \
        bool raise_selector_dummy() const noexcept
#else
  #if XTL_MESSAGE_ENABLED
    #error Macro RS, used by Mach7 pattern-matching library, has already been defined
  #endif
#endif

//------------------------------------------------------------------------------

/// By default a value of any type is not decomposable i.e. the whole value is 
/// the only value it can be decomposed into.
/// NOTE: We should not need this anymore since we provide a specialization of 
///       constr1 structure for exactly this case.
/// NOTE: We still need this at the moment as specialization of constr1 does
///       not handle different types e.g. when subject type is int but target is size_t.
template <typename type_being_matched, size_t layout>
struct bindings { Members(mch::identity<type_being_matched>); };

//------------------------------------------------------------------------------

/// Helper function to access the value of the member specified with #KS macro 
/// on a given object.
template <typename T>
inline auto kind_selector(const T* p) -> XTL_RETURN
(
    apply_member(p, bindings<T>::kind_selector())
)

/// Helper function to call a function specified with #RS macro on a given object.
template <typename T>
inline auto raise_selector(const T* p) -> XTL_RETURN
(
    apply_member(p, bindings<T>::raise_selector())
)

//------------------------------------------------------------------------------

//template <typename T, typename C, T C::*m>
//T mem_field(const C& c)
//{
//    return c.*m;
//}
//
////------------------------------------------------------------------------------
//
//template <typename T, typename C, T (C::*f)() const>
//T mem_func(const C& c)
//{
//    return (c.*f)();
//}

//------------------------------------------------------------------------------

} // of namespace mch
