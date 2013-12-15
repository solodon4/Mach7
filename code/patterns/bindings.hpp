///
/// \file bindings.hpp
///
/// Classes and macros used to define and work with bindings.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of Mach7 library (http://parasol.tamu.edu/mach7/).
/// Copyright (C) 2011-2012 Texas A&M University.
/// All rights reserved.
///

#pragma once

#include <cstddef>
#include "../metatools.hpp"

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

#if !defined(CM)
    /// Macro to define member's position within decomposition of a given data type
    /// Example: CM(0,MyClass::member) or CM(1,external_func)
    /// \note Use this macro only inside specializations of #bindings
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
    #error Macro CM used by the pattern-matching library has already been defined
#endif

#if !defined(KS)
    /// Macro to define a kind selector - a member of the common base class that 
    /// carries a distinct integral value that uniquely identifies the derived 
    /// type.  Used in the decomposition of the base class.
    /// \note Use this macro only inside specializations of #bindings
    /// \note We use variadic macro parameter here in order to be able to handle 
    ///       templates, which might have commas, otherwise just a single argument
    ///       would be sufficient.
    /// FIX: KS doesn't accept now members qualified with base class, but CM does, check why.
    #define KS(...)                                                 \
        static inline decltype(unary(&__VA_ARGS__)) kind_selector() noexcept \
        {                                                           \
            return unary(&__VA_ARGS__);                             \
        }                                                           \
        bool kind_selector_dummy() const noexcept;
#else
    #error Macro KS used by the pattern-matching library has already been defined
#endif

#if !defined(KV)
    /// Macro to define an integral constant that uniquely identifies the derived 
    /// class. Used in the decomposition of a derived class. 
    /// \param T -- the first argument must indicate the root of the hierarchy -- 
    ///        the least derived class for which tag values given in second 
    ///        argument are unique, which is also the class whose bindings define 
    ///        the KS macro.
    /// \note Use this macro only inside specializations of #bindings
    /// \note We use variadic macro parameter here in order to be able to handle 
    ///       templates, which might have commas, otherwise just a single argument
    ///       would be sufficient.
    #define KV(T, ...) typedef T root_type; enum { kind_value = __VA_ARGS__ };
#else
    #error Macro KV used by the pattern-matching library has already been defined
#endif

#if !defined(FQ)
    /// Macro to define an expected frequency of objects of a given class.
    /// The exact values are not important as they are turned into probabilities by
    /// dividing these numbers onto sum of frequencies of all classes, statically
    /// allowable by a given match statement (derived from a source type).
    /// \note Use this macro only inside specializations of #bindings
    /// \note We use variadic macro parameter here in order to be able to handle 
    ///       templates, which might have commas, otherwise just a single argument
    ///       would be sufficient.
    #define FQ(...) enum { fq = __VA_ARGS__ };
#else
    #error Macro FQ used by the pattern-matching library has already been defined
#endif

#if !defined(FQS)
    /// Macro to define a function that returns expected frequency of a given type.
    /// The syntax of the function should be: size_t f(const source_type&);
    /// FIX: Make sure there is no re-entrancy problems when pattern matching is
    ///      used inside of the frequency function f to uncover source type.
    #define FQS(f)                                                  \
        static size_t frequency(intptr_t vtbl) noexcept             \
        {                                                           \
            return ::f(*reinterpret_cast<const XTL_CPP0X_TYPENAME get_param<bindings>::type*>(&vtbl)); \
        }                                                           \
        bool frequency_dummy() const noexcept;
#else
    #error Macro FQS used by the pattern-matching library has already been defined
#endif

#if !defined(RS)
    /// Macro to define a raise selector - a virtual member function of the common
    /// base class that implements a polymorphic exception idiom (\see 
    /// http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Polymorphic_Exception). 
    /// Essentially it's a virtual member function (e.g. virtual void raise() const = 0;)
    /// that will be overriden in all subclasses in the following way:
    /// void SubClass::raise() const { throw *this; }
    /// \note Use this macro only inside specializations of #bindings
    /// \note We use variadic macro parameter here in order to be able to handle 
    ///       templates, which might have commas, otherwise just a single argument
    ///       would be sufficient.
    #define RS(...)                                                 \
        static inline decltype(unary(&__VA_ARGS__)) raise_selector() noexcept\
        {                                                           \
            return unary(&__VA_ARGS__);                             \
        }                                                           \
        bool raise_selector_dummy() const noexcept;
#else
    #error Macro RS used by the pattern-matching library has already been defined
#endif

//------------------------------------------------------------------------------

/// By default a value of any type is not decomposable i.e. the whole value is 
/// the only value it can be decomposed into.
/// NOTE: We should not need this anymore since we provide a specialization of 
///       constr1 structure for exactly this case.
/// NOTE: We still need this at the moment as specialization of constr1 does
///       not handle different types e.g. when subject type is int but target is size_t.
template <typename type_being_matched, size_t layout>
struct bindings { CM(0,identity<type_being_matched>); };

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
