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
/// This file defines constructor/type pattern support by our library.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// \see https://parasol.tamu.edu/mach7/
/// \see https://github.com/solodon4/Mach7
/// \see https://github.com/solodon4/SELL
///

#pragma once

#include "bindings.hpp"
#include "primitive.hpp" // FIX: Ideally this should be common.hpp, but GCC seem to disagree: http://gcc.gnu.org/bugzilla/show_bug.cgi?id=55460
#include <cstddef>

namespace mch ///< Mach7 library namespace
{

//------------------------------------------------------------------------------

/// Constructor/Type pattern of 0 arguments
template <typename T, size_t layout>
struct constr0
{
    /// Type function returning a type that will be accepted by the pattern for
    /// a given subject type S. We use type function instead of an associated 
    /// type, because there is no a single accepted type for a #wildcard pattern
    /// for example. Requirement of #Pattern concept.
    template <typename S> struct accepted_type_for { typedef T type; };

    ///@{
    /// Constructor patterns can be uniformly matched against pointers and 
    /// references to polymorphic objects. This uniformity is needed for 
    /// composability of constructor patterns, since application operators
    /// return a pointer to indicate whether pattern was accepted or refuted.
    /// The following set of application operators distinguishes:
    ///  - the arguments passed by reference from those passed by pointers
    ///  - whether the target type matches the subject type (to avoid dynamic_cast)
    ///  - const from non-const arguments to propagate constness further.
    template <typename U> const T* operator()(const U* u) const noexcept { return dynamic_cast<const T*>(u); }
    template <typename U>       T* operator()(      U* u) const noexcept { return dynamic_cast<      T*>(u); }
    template <typename U> const T* operator()(const U& u) const noexcept { return operator()(&u); }
    template <typename U>       T* operator()(      U& u) const noexcept { return operator()(&u); }
                          const T* operator()(const T* t) const noexcept { return t; }
                                T* operator()(      T* t) const noexcept { return t; }
                          const T* operator()(const T& t) const noexcept { return &t; }
                                T* operator()(      T& t) const noexcept { return &t; }
    ///@}
};

//------------------------------------------------------------------------------

/// Constructor/Type pattern of 1 arguments
template <typename T, size_t layout, typename P1, typename Condition = void>
struct constr1
{
    static_assert(is_pattern<P1>::value, "Argument P1 of constructor-pattern must be a pattern");

    /// Type function returning a type that will be accepted by the pattern for
    /// a given subject type S. We use type function instead of an associated 
    /// type, because there is no a single accepted type for a #wildcard pattern
    /// for example. Requirement of #Pattern concept.
    template <typename S> struct accepted_type_for { typedef T type; };

    constexpr explicit constr1(const P1&  p1)       noexcept_when(std::is_nothrow_copy_constructible<P1>::value) : m_p1(p1)            {}
    constexpr explicit constr1(      P1&& p1)       noexcept_when(std::is_nothrow_move_constructible<P1>::value) : m_p1(std::move(p1)) {}

    constexpr          constr1(const constr1&  src) noexcept_when(std::is_nothrow_copy_constructible<P1>::value) : m_p1(          src.m_p1 ) {} ///< Copy constructor
    constexpr          constr1(      constr1&& src) noexcept_when(std::is_nothrow_move_constructible<P1>::value) : m_p1(std::move(src.m_p1)) {} ///< Move constructor

    constr1& operator=(const constr1&) XTL_DELETED; ///< Assignment is not allowed for this class

    /// Helper function that does the actual structural matching once we have
    /// uncovered a value of the target type. Applies to a const argument!
    const T* match_structure(const T* t) const
    {
        XTL_ASSERT(t); // This helper function assumes t cannot be a nullptr
        return apply_expression(m_p1, t, bindings<T,layout>::member0()) // error C2027: use of undefined type 'bindings<type_being_matched,layout>'
                ? t                                                     // here means you did not provide bindings for type_being_matched and layout
                : 0;                                                    // described in the details of error message. See #bindings and #CM
                                                                        // error: incomplete type 'bindings<type_being_matched, layout>' used in nested name specifier (see above description for Visual C++)
    }
    /// Helper function that does the actual structural matching once we have
    /// uncovered a value of the target type. Applies to a non-const argument!
          T* match_structure(      T* t) const
    {
        XTL_ASSERT(t); // This helper function assumes t cannot be a nullptr
        return apply_expression(m_p1, t, bindings<T,layout>::member0()) // error C2027: use of undefined type 'bindings<type_being_matched,layout>'
                ? t                                                     // here means you did not provide bindings for type_being_matched and layout
                : 0;                                                    // described in the details of error message. See #bindings and #CM
                                                                        // error: incomplete type 'bindings<type_being_matched, layout>' used in nested name specifier (see above description for Visual C++)
    }

    ///@{
    /// Constructor patterns can be uniformly matched against pointers and 
    /// references to polymorphic objects. This uniformity is needed for 
    /// composability of constructor patterns, since application operators
    /// return a pointer to indicate whether pattern was accepted or refuted.
    /// The following set of application operators distinguishes:
    ///  - the arguments passed by reference from those passed by pointers
    ///  - whether the target type matches the subject type (to avoid dynamic_cast)
    ///  - const from non-const arguments to propagate constness further.
    template <typename U> const T* operator()(const U* u) const { return operator()(dynamic_cast<const T*>(u)); }
    template <typename U>       T* operator()(      U* u) const { return operator()(dynamic_cast<      T*>(u)); }
    template <typename U> const T* operator()(const U& u) const { return operator()(&u); }
    template <typename U>       T* operator()(      U& u) const { return operator()(&u); }
                          const T* operator()(const T* t) const { return t ? match_structure(t) : 0; }
                                T* operator()(      T* t) const { return t ? match_structure(t) : 0; }
                          const T* operator()(const T& t) const { return match_structure(&t); } // We assume references to be checked for not nullptr
                                T* operator()(      T& t) const { return match_structure(&t); } // and don't check it here to save on performance
    ///@}

    P1 m_p1; ///< Pattern representing 1st operand
};

/// This is a specialization of the general one-argument construction pattern
/// for the case when the type of that argument matches the target type. This
/// is needed to be able to apply algebraic patterns to the entire object for
/// those cases when object already provides its structural decomposition.
/// For example std::complex<T> can be decomposed into two subcomponents and
/// accordingly to our logic when second argument is omitted, we should only 
/// match against the first one. Now if that first argument is of type 
/// complex<T> and not just T, we are trying to match against the whole object,
/// not against its first subcomponent! One can imagine a recursive data 
/// structure however where the type of the first subcomponent also matches
/// the actual type (e.g. list), so we must give preference in such case to
/// the structurally matched subcomponent instead of matching against the whole
/// object. 
/// FIX: Add extra condition that makes sure the type of first subcomponent is
///      not the same as the type itself!
template <typename T, size_t layout, typename P1>
struct constr1<T,layout,P1,typename std::enable_if<std::is_same<T,typename underlying<typename P1::template accepted_type_for<T>::type>::type>::value>::type>
{
    static_assert(is_pattern<P1>::value, "Argument P1 of constructor-pattern must be a pattern");

    /// Type function returning a type that will be accepted by the pattern for
    /// a given subject type S. We use type function instead of an associated 
    /// type, because there is no a single accepted type for a #wildcard pattern
    /// for example. Requirement of #Pattern concept.
    template <typename S> struct accepted_type_for { typedef T type; };

    constexpr explicit constr1(const P1&  p1)       noexcept_when(std::is_nothrow_copy_constructible<P1>::value) : m_p1(p1)            {}
    constexpr explicit constr1(      P1&& p1)       noexcept_when(std::is_nothrow_move_constructible<P1>::value) : m_p1(std::move(p1)) {}

    constexpr          constr1(const constr1&  src) noexcept_when(std::is_nothrow_copy_constructible<P1>::value) : m_p1(          src.m_p1 ) {} ///< Copy constructor
    constexpr          constr1(      constr1&& src) noexcept_when(std::is_nothrow_move_constructible<P1>::value) : m_p1(std::move(src.m_p1)) {} ///< Move constructor

    constr1& operator=(const constr1&) XTL_DELETED; ///< Assignment is not allowed for this class

    /// Helper function that does the actual structural matching once we have
    /// uncovered a value of the target type. Applies to a const argument!
    const T* match_structure(const T* t) const
    {
        XTL_ASSERT(t); // This helper function assumes t cannot be a nullptr
        return m_p1(*t) ? t : 0;
    }
    /// Helper function that does the actual structural matching once we have
    /// uncovered a value of the target type. Applies to a non-const argument!
          T* match_structure(      T* t) const
    {
        XTL_ASSERT(t); // This helper function assumes t cannot be a nullptr
        return m_p1(*t) ? t : 0;
    }

    ///@{
    /// Constructor patterns can be uniformly matched against pointers and 
    /// references to polymorphic objects. This uniformity is needed for 
    /// composability of constructor patterns, since application operators
    /// return a pointer to indicate whether pattern was accepted or refuted.
    /// The following set of application operators distinguishes:
    ///  - the arguments passed by reference from those passed by pointers
    ///  - whether the target type matches the subject type (to avoid dynamic_cast)
    ///  - const from non-const arguments to propagate constness further.
    template <typename U> const T* operator()(const U* u) const { return operator()(dynamic_cast<const T*>(u)); }
    template <typename U>       T* operator()(      U* u) const { return operator()(dynamic_cast<      T*>(u)); }
    template <typename U> const T* operator()(const U& u) const { return operator()(&u); }
    template <typename U>       T* operator()(      U& u) const { return operator()(&u); }
                          const T* operator()(const T* t) const { return t ? match_structure(t) : 0; }
                                T* operator()(      T* t) const { return t ? match_structure(t) : 0; }
                          const T* operator()(const T& t) const { return match_structure(&t); } // We assume references to be checked for not nullptr
                                T* operator()(      T& t) const { return match_structure(&t); } // and don't check it here to save on performance
    ///@}

    P1 m_p1; ///< Pattern representing 1st operand
};

//------------------------------------------------------------------------------

/// Constructor/Type pattern of 2 arguments
template <typename T, size_t layout, typename P1, typename P2>
struct constr2
{
    static_assert(is_pattern<P1>::value, "Argument P1 of constructor-pattern must be a pattern");
    static_assert(is_pattern<P2>::value, "Argument P2 of constructor-pattern must be a pattern");

    /// Type function returning a type that will be accepted by the pattern for
    /// a given subject type S. We use type function instead of an associated 
    /// type, because there is no a single accepted type for a #wildcard pattern
    /// for example. Requirement of #Pattern concept.
    template <typename S> struct accepted_type_for { typedef T type; };

    constexpr constr2(const P1&  p1, const P2&  p2) noexcept_when(std::is_nothrow_copy_constructible<P1>::value && std::is_nothrow_copy_constructible<P2>::value) : m_p1(          p1 ), m_p2(          p2 ) {}
    constexpr constr2(      P1&& p1, const P2&  p2) noexcept_when(std::is_nothrow_move_constructible<P1>::value && std::is_nothrow_copy_constructible<P2>::value) : m_p1(std::move(p1)), m_p2(          p2 ) {}
    constexpr constr2(const P1&  p1,       P2&& p2) noexcept_when(std::is_nothrow_copy_constructible<P1>::value && std::is_nothrow_move_constructible<P2>::value) : m_p1(          p1 ), m_p2(std::move(p2)) {}
    constexpr constr2(      P1&& p1,       P2&& p2) noexcept_when(std::is_nothrow_move_constructible<P1>::value && std::is_nothrow_move_constructible<P2>::value) : m_p1(std::move(p1)), m_p2(std::move(p2)) {}

    constexpr constr2(const constr2&  src)          noexcept_when(std::is_nothrow_copy_constructible<P1>::value && std::is_nothrow_copy_constructible<P2>::value) : m_p1(          src.m_p1 ), m_p2(          src.m_p2 ) {} ///< Copy constructor    
    constexpr constr2(      constr2&& src)          noexcept_when(std::is_nothrow_move_constructible<P1>::value && std::is_nothrow_move_constructible<P2>::value) : m_p1(std::move(src.m_p1)), m_p2(std::move(src.m_p2)) {} ///< Move constructor
    
    constr2& operator=(const constr2&) XTL_DELETED; ///< Assignment is not allowed for this class

    /// Helper function that does the actual structural matching once we have
    /// uncovered a value of the target type. Applies to a const argument!
    const T* match_structure(const T* t) const
    {
        XTL_ASSERT(t); // This helper function assumes t cannot be a nullptr
        return apply_expression(m_p1, t, bindings<T,layout>::member0()) // error C2027: use of undefined type 'bindings<type_being_matched,layout>'
            && apply_expression(m_p2, t, bindings<T,layout>::member1()) // here means you did not provide bindings for type_being_matched and layout
             ? t                                                        // described in the details of error message. See #bindings and #CM
             : 0;                                                       // error: incomplete type 'bindings<type_being_matched, layout>' used in nested name specifier (see above description for Visual C++)
    }
    /// Helper function that does the actual structural matching once we have
    /// uncovered a value of the target type. Applies to a non-const argument!
          T* match_structure(      T* t) const
    {
        XTL_ASSERT(t); // This helper function assumes t cannot be a nullptr
        return apply_expression(m_p1, t, bindings<T,layout>::member0()) // error C2027: use of undefined type 'bindings<type_being_matched,layout>'
            && apply_expression(m_p2, t, bindings<T,layout>::member1()) // here means you did not provide bindings for type_being_matched and layout
             ? t                                                        // described in the details of error message. See #bindings and #CM
             : 0;                                                       // error: incomplete type 'bindings<type_being_matched, layout>' used in nested name specifier (see above description for Visual C++)
    }

    ///@{
    /// Constructor patterns can be uniformly matched against pointers and 
    /// references to polymorphic objects. This uniformity is needed for 
    /// composability of constructor patterns, since application operators
    /// return a pointer to indicate whether pattern was accepted or refuted.
    /// The following set of application operators distinguishes:
    ///  - the arguments passed by reference from those passed by pointers
    ///  - whether the target type matches the subject type (to avoid dynamic_cast)
    ///  - const from non-const arguments to propagate constness further.
    template <typename U> const T* operator()(const U* u) const { return operator()(dynamic_cast<const T*>(u)); }
    template <typename U>       T* operator()(      U* u) const { return operator()(dynamic_cast<      T*>(u)); }
    template <typename U> const T* operator()(const U& u) const { return operator()(&u); }
    template <typename U>       T* operator()(      U& u) const { return operator()(&u); }
                          const T* operator()(const T* t) const { return t ? match_structure(t) : 0; }
                                T* operator()(      T* t) const { return t ? match_structure(t) : 0; }
                          const T* operator()(const T& t) const { return match_structure(&t); } // We assume references to be checked for not nullptr
                                T* operator()(      T& t) const { return match_structure(&t); } // and don't check it here to save on performance
    ///@}

    P1 m_p1; ///< Pattern representing 1st operand
    P2 m_p2; ///< Pattern representing 2nd operand
};

//------------------------------------------------------------------------------

/// Constructor/Type pattern of 3 arguments
template <typename T, size_t layout, typename P1, typename P2, typename P3>
struct constr3
{
    static_assert(is_pattern<P1>::value, "Argument P1 of constructor-pattern must be a pattern");
    static_assert(is_pattern<P2>::value, "Argument P2 of constructor-pattern must be a pattern");
    static_assert(is_pattern<P3>::value, "Argument P3 of constructor-pattern must be a pattern");

    /// Type function returning a type that will be accepted by the pattern for
    /// a given subject type S. We use type function instead of an associated 
    /// type, because there is no a single accepted type for a #wildcard pattern
    /// for example. Requirement of #Pattern concept.
    template <typename S> struct accepted_type_for { typedef T type; };

    constexpr constr3(const P1&  p1, const P2&  p2, const P3&  p3) noexcept_when(std::is_nothrow_copy_constructible<P1>::value && std::is_nothrow_copy_constructible<P2>::value && std::is_nothrow_copy_constructible<P3>::value) : m_p1(          p1 ), m_p2(          p2 ), m_p3(          p3 ) {}
    constexpr constr3(      P1&& p1, const P2&  p2, const P3&  p3) noexcept_when(std::is_nothrow_move_constructible<P1>::value && std::is_nothrow_copy_constructible<P2>::value && std::is_nothrow_copy_constructible<P3>::value) : m_p1(std::move(p1)), m_p2(          p2 ), m_p3(          p3 ) {}
    constexpr constr3(const P1&  p1,       P2&& p2, const P3&  p3) noexcept_when(std::is_nothrow_copy_constructible<P1>::value && std::is_nothrow_move_constructible<P2>::value && std::is_nothrow_copy_constructible<P3>::value) : m_p1(          p1 ), m_p2(std::move(p2)), m_p3(          p3 ) {}
    constexpr constr3(      P1&& p1,       P2&& p2, const P3&  p3) noexcept_when(std::is_nothrow_move_constructible<P1>::value && std::is_nothrow_move_constructible<P2>::value && std::is_nothrow_copy_constructible<P3>::value) : m_p1(std::move(p1)), m_p2(std::move(p2)), m_p3(          p3 ) {}
    constexpr constr3(const P1&  p1, const P2&  p2,       P3&& p3) noexcept_when(std::is_nothrow_copy_constructible<P1>::value && std::is_nothrow_copy_constructible<P2>::value && std::is_nothrow_move_constructible<P3>::value) : m_p1(          p1 ), m_p2(          p2 ), m_p3(std::move(p3)) {}
    constexpr constr3(      P1&& p1, const P2&  p2,       P3&& p3) noexcept_when(std::is_nothrow_move_constructible<P1>::value && std::is_nothrow_copy_constructible<P2>::value && std::is_nothrow_move_constructible<P3>::value) : m_p1(std::move(p1)), m_p2(          p2 ), m_p3(std::move(p3)) {}
    constexpr constr3(const P1&  p1,       P2&& p2,       P3&& p3) noexcept_when(std::is_nothrow_copy_constructible<P1>::value && std::is_nothrow_move_constructible<P2>::value && std::is_nothrow_move_constructible<P3>::value) : m_p1(          p1 ), m_p2(std::move(p2)), m_p3(std::move(p3)) {}
    constexpr constr3(      P1&& p1,       P2&& p2,       P3&& p3) noexcept_when(std::is_nothrow_move_constructible<P1>::value && std::is_nothrow_move_constructible<P2>::value && std::is_nothrow_move_constructible<P3>::value) : m_p1(std::move(p1)), m_p2(std::move(p2)), m_p3(std::move(p3)) {}

    constexpr constr3(const constr3&  src)                         noexcept_when(std::is_nothrow_copy_constructible<P1>::value && std::is_nothrow_copy_constructible<P2>::value && std::is_nothrow_copy_constructible<P3>::value) : m_p1(          src.m_p1 ), m_p2(          src.m_p2 ), m_p3(          src.m_p3 ) {} ///< Copy constructor    
    constexpr constr3(      constr3&& src)                         noexcept_when(std::is_nothrow_move_constructible<P1>::value && std::is_nothrow_move_constructible<P2>::value && std::is_nothrow_move_constructible<P3>::value) : m_p1(std::move(src.m_p1)), m_p2(std::move(src.m_p2)), m_p3(std::move(src.m_p3)) {} ///< Move constructor

    constr3& operator=(const constr3&) XTL_DELETED; ///< Assignment is not allowed for this class

    /// Helper function that does the actual structural matching once we have
    /// uncovered a value of the target type. Applies to a const argument!
    const T* match_structure(const T* t) const
    {
        XTL_ASSERT(t); // This helper function assumes t cannot be a nullptr
        return apply_expression(m_p1, t, bindings<T,layout>::member0()) // error C2027: use of undefined type 'bindings<type_being_matched,layout>'
            && apply_expression(m_p2, t, bindings<T,layout>::member1()) // here means you did not provide bindings for type_being_matched and layout
            && apply_expression(m_p3, t, bindings<T,layout>::member2()) // described in the details of error message. See #bindings and #CM
             ? t                                                        // error: incomplete type 'bindings<type_being_matched, layout>' used in nested name specifier (see above description for Visual C++)
             : 0;
    }
    /// Helper function that does the actual structural matching once we have
    /// uncovered a value of the target type. Applies to a non-const argument!
          T* match_structure(      T* t) const
    {
        XTL_ASSERT(t); // This helper function assumes t cannot be a nullptr
        return apply_expression(m_p1, t, bindings<T,layout>::member0()) // error C2027: use of undefined type 'bindings<type_being_matched,layout>'
            && apply_expression(m_p2, t, bindings<T,layout>::member1()) // here means you did not provide bindings for type_being_matched and layout
            && apply_expression(m_p3, t, bindings<T,layout>::member2()) // described in the details of error message. See #bindings and #CM
             ? t                                                        // error: incomplete type 'bindings<type_being_matched, layout>' used in nested name specifier (see above description for Visual C++)
             : 0;
    }

    ///@{
    /// Constructor patterns can be uniformly matched against pointers and 
    /// references to polymorphic objects. This uniformity is needed for 
    /// composability of constructor patterns, since application operators
    /// return a pointer to indicate whether pattern was accepted or refuted.
    /// The following set of application operators distinguishes:
    ///  - the arguments passed by reference from those passed by pointers
    ///  - whether the target type matches the subject type (to avoid dynamic_cast)
    ///  - const from non-const arguments to propagate constness further.
    template <typename U> const T* operator()(const U* u) const { return operator()(dynamic_cast<const T*>(u)); }
    template <typename U>       T* operator()(      U* u) const { return operator()(dynamic_cast<      T*>(u)); }
    template <typename U> const T* operator()(const U& u) const { return operator()(&u); }
    template <typename U>       T* operator()(      U& u) const { return operator()(&u); }
                          const T* operator()(const T* t) const { return t ? match_structure(t) : 0; }
                                T* operator()(      T* t) const { return t ? match_structure(t) : 0; }
                          const T* operator()(const T& t) const { return match_structure(&t); } // We assume references to be checked for not nullptr
                                T* operator()(      T& t) const { return match_structure(&t); } // and don't check it here to save on performance
    ///@}

    P1 m_p1; ///< Pattern representing 1st operand
    P2 m_p2; ///< Pattern representing 2nd operand
    P3 m_p3; ///< Pattern representing 3rd operand
};

//------------------------------------------------------------------------------

/// Constructor/Type pattern of 4 arguments
template <typename T, size_t layout, typename P1, typename P2, typename P3, typename P4>
struct constr4
{
    static_assert(is_pattern<P1>::value, "Argument P1 of constructor-pattern must be a pattern");
    static_assert(is_pattern<P2>::value, "Argument P2 of constructor-pattern must be a pattern");
    static_assert(is_pattern<P3>::value, "Argument P3 of constructor-pattern must be a pattern");
    static_assert(is_pattern<P4>::value, "Argument P4 of constructor-pattern must be a pattern");

    /// Type function returning a type that will be accepted by the pattern for
    /// a given subject type S. We use type function instead of an associated 
    /// type, because there is no a single accepted type for a #wildcard pattern
    /// for example. Requirement of #Pattern concept.
    template <typename S> struct accepted_type_for { typedef T type; };

    constexpr constr4(const P1&  p1, const P2&  p2, const P3&  p3, const P4&  p4) noexcept_when(std::is_nothrow_copy_constructible<P1>::value && std::is_nothrow_copy_constructible<P2>::value && std::is_nothrow_copy_constructible<P3>::value && std::is_nothrow_copy_constructible<P4>::value) : m_p1(          p1 ), m_p2(          p2 ), m_p3(          p3 ), m_p4(          p4 ) {}
    constexpr constr4(      P1&& p1, const P2&  p2, const P3&  p3, const P4&  p4) noexcept_when(std::is_nothrow_move_constructible<P1>::value && std::is_nothrow_copy_constructible<P2>::value && std::is_nothrow_copy_constructible<P3>::value && std::is_nothrow_copy_constructible<P4>::value) : m_p1(std::move(p1)), m_p2(          p2 ), m_p3(          p3 ), m_p4(          p4 ) {}
    constexpr constr4(const P1&  p1,       P2&& p2, const P3&  p3, const P4&  p4) noexcept_when(std::is_nothrow_copy_constructible<P1>::value && std::is_nothrow_move_constructible<P2>::value && std::is_nothrow_copy_constructible<P3>::value && std::is_nothrow_copy_constructible<P4>::value) : m_p1(          p1 ), m_p2(std::move(p2)), m_p3(          p3 ), m_p4(          p4 ) {}
    constexpr constr4(      P1&& p1,       P2&& p2, const P3&  p3, const P4&  p4) noexcept_when(std::is_nothrow_move_constructible<P1>::value && std::is_nothrow_move_constructible<P2>::value && std::is_nothrow_copy_constructible<P3>::value && std::is_nothrow_copy_constructible<P4>::value) : m_p1(std::move(p1)), m_p2(std::move(p2)), m_p3(          p3 ), m_p4(          p4 ) {}
    constexpr constr4(const P1&  p1, const P2&  p2,       P3&& p3, const P4&  p4) noexcept_when(std::is_nothrow_copy_constructible<P1>::value && std::is_nothrow_copy_constructible<P2>::value && std::is_nothrow_move_constructible<P3>::value && std::is_nothrow_copy_constructible<P4>::value) : m_p1(          p1 ), m_p2(          p2 ), m_p3(std::move(p3)), m_p4(          p4 ) {}
    constexpr constr4(      P1&& p1, const P2&  p2,       P3&& p3, const P4&  p4) noexcept_when(std::is_nothrow_move_constructible<P1>::value && std::is_nothrow_copy_constructible<P2>::value && std::is_nothrow_move_constructible<P3>::value && std::is_nothrow_copy_constructible<P4>::value) : m_p1(std::move(p1)), m_p2(          p2 ), m_p3(std::move(p3)), m_p4(          p4 ) {}
    constexpr constr4(const P1&  p1,       P2&& p2,       P3&& p3, const P4&  p4) noexcept_when(std::is_nothrow_copy_constructible<P1>::value && std::is_nothrow_move_constructible<P2>::value && std::is_nothrow_move_constructible<P3>::value && std::is_nothrow_copy_constructible<P4>::value) : m_p1(          p1 ), m_p2(std::move(p2)), m_p3(std::move(p3)), m_p4(          p4 ) {}
    constexpr constr4(      P1&& p1,       P2&& p2,       P3&& p3, const P4&  p4) noexcept_when(std::is_nothrow_move_constructible<P1>::value && std::is_nothrow_move_constructible<P2>::value && std::is_nothrow_move_constructible<P3>::value && std::is_nothrow_copy_constructible<P4>::value) : m_p1(std::move(p1)), m_p2(std::move(p2)), m_p3(std::move(p3)), m_p4(          p4 ) {}
    constexpr constr4(const P1&  p1, const P2&  p2, const P3&  p3,       P4&& p4) noexcept_when(std::is_nothrow_copy_constructible<P1>::value && std::is_nothrow_copy_constructible<P2>::value && std::is_nothrow_copy_constructible<P3>::value && std::is_nothrow_move_constructible<P4>::value) : m_p1(          p1 ), m_p2(          p2 ), m_p3(          p3 ), m_p4(std::move(p4)) {}
    constexpr constr4(      P1&& p1, const P2&  p2, const P3&  p3,       P4&& p4) noexcept_when(std::is_nothrow_move_constructible<P1>::value && std::is_nothrow_copy_constructible<P2>::value && std::is_nothrow_copy_constructible<P3>::value && std::is_nothrow_move_constructible<P4>::value) : m_p1(std::move(p1)), m_p2(          p2 ), m_p3(          p3 ), m_p4(std::move(p4)) {}
    constexpr constr4(const P1&  p1,       P2&& p2, const P3&  p3,       P4&& p4) noexcept_when(std::is_nothrow_copy_constructible<P1>::value && std::is_nothrow_move_constructible<P2>::value && std::is_nothrow_copy_constructible<P3>::value && std::is_nothrow_move_constructible<P4>::value) : m_p1(          p1 ), m_p2(std::move(p2)), m_p3(          p3 ), m_p4(std::move(p4)) {}
    constexpr constr4(      P1&& p1,       P2&& p2, const P3&  p3,       P4&& p4) noexcept_when(std::is_nothrow_move_constructible<P1>::value && std::is_nothrow_move_constructible<P2>::value && std::is_nothrow_copy_constructible<P3>::value && std::is_nothrow_move_constructible<P4>::value) : m_p1(std::move(p1)), m_p2(std::move(p2)), m_p3(          p3 ), m_p4(std::move(p4)) {}
    constexpr constr4(const P1&  p1, const P2&  p2,       P3&& p3,       P4&& p4) noexcept_when(std::is_nothrow_copy_constructible<P1>::value && std::is_nothrow_copy_constructible<P2>::value && std::is_nothrow_move_constructible<P3>::value && std::is_nothrow_move_constructible<P4>::value) : m_p1(          p1 ), m_p2(          p2 ), m_p3(std::move(p3)), m_p4(std::move(p4)) {}
    constexpr constr4(      P1&& p1, const P2&  p2,       P3&& p3,       P4&& p4) noexcept_when(std::is_nothrow_move_constructible<P1>::value && std::is_nothrow_copy_constructible<P2>::value && std::is_nothrow_move_constructible<P3>::value && std::is_nothrow_move_constructible<P4>::value) : m_p1(std::move(p1)), m_p2(          p2 ), m_p3(std::move(p3)), m_p4(std::move(p4)) {}
    constexpr constr4(const P1&  p1,       P2&& p2,       P3&& p3,       P4&& p4) noexcept_when(std::is_nothrow_copy_constructible<P1>::value && std::is_nothrow_move_constructible<P2>::value && std::is_nothrow_move_constructible<P3>::value && std::is_nothrow_move_constructible<P4>::value) : m_p1(          p1 ), m_p2(std::move(p2)), m_p3(std::move(p3)), m_p4(std::move(p4)) {}
    constexpr constr4(      P1&& p1,       P2&& p2,       P3&& p3,       P4&& p4) noexcept_when(std::is_nothrow_move_constructible<P1>::value && std::is_nothrow_move_constructible<P2>::value && std::is_nothrow_move_constructible<P3>::value && std::is_nothrow_move_constructible<P4>::value) : m_p1(std::move(p1)), m_p2(std::move(p2)), m_p3(std::move(p3)), m_p4(std::move(p4)) {}

    constexpr constr4(const constr4&  src)                                        noexcept_when(std::is_nothrow_copy_constructible<P1>::value && std::is_nothrow_copy_constructible<P2>::value && std::is_nothrow_copy_constructible<P3>::value && std::is_nothrow_copy_constructible<P4>::value) : m_p1(          src.m_p1 ), m_p2(          src.m_p2 ), m_p3(          src.m_p3 ), m_p4(          src.m_p4 ) {} ///< Copy constructor    
    constexpr constr4(      constr4&& src)                                        noexcept_when(std::is_nothrow_move_constructible<P1>::value && std::is_nothrow_move_constructible<P2>::value && std::is_nothrow_move_constructible<P3>::value && std::is_nothrow_move_constructible<P4>::value) : m_p1(std::move(src.m_p1)), m_p2(std::move(src.m_p2)), m_p3(std::move(src.m_p3)), m_p4(std::move(src.m_p4)) {} ///< Move constructor

    constr4& operator=(const constr4&) XTL_DELETED; ///< Assignment is not allowed for this class

    /// Helper function that does the actual structural matching once we have
    /// uncovered a value of the target type. Applies to a const argument!
    const T* match_structure(const T* t) const
    {
        XTL_ASSERT(t); // This helper function assumes t cannot be a nullptr
        return apply_expression(m_p1, t, bindings<T,layout>::member0()) // error C2027: use of undefined type 'bindings<type_being_matched,layout>'
            && apply_expression(m_p2, t, bindings<T,layout>::member1()) // here means you did not provide bindings for type_being_matched and layout
            && apply_expression(m_p3, t, bindings<T,layout>::member2()) // described in the details of error message. See #bindings and #CM
            && apply_expression(m_p4, t, bindings<T,layout>::member3()) // error: incomplete type 'bindings<type_being_matched, layout>' used in nested name specifier (see above description for Visual C++)
             ? t
             : 0;
    }
    /// Helper function that does the actual structural matching once we have
    /// uncovered a value of the target type. Applies to a non-const argument!
          T* match_structure(      T* t) const
    {
        XTL_ASSERT(t); // This helper function assumes t cannot be a nullptr
        return apply_expression(m_p1, t, bindings<T,layout>::member0()) // error C2027: use of undefined type 'bindings<type_being_matched,layout>'
            && apply_expression(m_p2, t, bindings<T,layout>::member1()) // here means you did not provide bindings for type_being_matched and layout
            && apply_expression(m_p3, t, bindings<T,layout>::member2()) // described in the details of error message. See #bindings and #CM
            && apply_expression(m_p4, t, bindings<T,layout>::member3()) // error: incomplete type 'bindings<type_being_matched, layout>' used in nested name specifier (see above description for Visual C++)
             ? t
             : 0;
    }

    ///@{
    /// Constructor patterns can be uniformly matched against pointers and 
    /// references to polymorphic objects. This uniformity is needed for 
    /// composability of constructor patterns, since application operators
    /// return a pointer to indicate whether pattern was accepted or refuted.
    /// The following set of application operators distinguishes:
    ///  - the arguments passed by reference from those passed by pointers
    ///  - whether the target type matches the subject type (to avoid dynamic_cast)
    ///  - const from non-const arguments to propagate constness further.
    template <typename U> const T* operator()(const U* u) const { return operator()(dynamic_cast<const T*>(u)); }
    template <typename U>       T* operator()(      U* u) const { return operator()(dynamic_cast<      T*>(u)); }
    template <typename U> const T* operator()(const U& u) const { return operator()(&u); }
    template <typename U>       T* operator()(      U& u) const { return operator()(&u); }
                          const T* operator()(const T* t) const { return t ? match_structure(t) : 0; }
                                T* operator()(      T* t) const { return t ? match_structure(t) : 0; }
                          const T* operator()(const T& t) const { return match_structure(&t); } // We assume references to be checked for not nullptr
                                T* operator()(      T& t) const { return match_structure(&t); } // and don't check it here to save on performance
    ///@}

    P1 m_p1; ///< Pattern representing 1st operand
    P2 m_p2; ///< Pattern representing 2nd operand
    P3 m_p3; ///< Pattern representing 3rd operand
    P4 m_p4; ///< Pattern representing 4th operand
};

//------------------------------------------------------------------------------

/// 0-argument version of a helper function to #cons that accepts arguments that
/// have been already preprocessed with #filter to convert regular variables into
/// #ref and constants into #value.
/// \note This version will be called from #cons with a non-#view target type
template <typename T, size_t layout>
inline constr0<T,layout> cons_ex(const view<T,layout>&) noexcept
{
    return constr0<T,layout>();
}

/// 0-argument version of a helper function to #cons that accepts arguments that
/// have been already preprocessed with #filter to convert regular variables into
/// #ref and constants into #value.
/// \note This version will be called from #cons that had its target type a #view
template <typename T, size_t layout>
inline constr0<T,layout> cons_ex(const view<view<T,layout>>&) noexcept
{
    return constr0<T,layout>();
}

/// A 0-argument version of a tree-pattern constructor. Target type is allowed
/// to be a #view here.
template <typename T>
inline auto C() noexcept -> XTL_RETURN(cons_ex(view<T>()))

/// A 0-argument version of a tree-pattern constructor that takes layout in
/// addition to the target type.
/// \note #view is not supposed to be passed as a target type to this version
///       of the function because we will then have two potentially conflicting
///       layouts. Any layout different from #default_layout passed here will
///       result in a compile time error.
template <typename T, size_t layout>
inline auto C() noexcept -> XTL_RETURN(cons_ex(view<T,layout>()))

//------------------------------------------------------------------------------

/// 1-argument version of a helper function to #cons that accepts arguments that
/// have been already preprocessed with #filter to convert regular variables into
/// #ref and constants into #value.
/// \note This version will be called from #cons with a non-#view target type
template <typename T, size_t layout, typename P1>
inline constr1<T,layout,
            typename underlying<P1>::type
       > 
cons_ex(const view<T,layout>&, P1&& p1) noexcept
{
    return constr1<T,layout,
            typename underlying<P1>::type
           >(
            std::forward<P1>(p1)
           );
}

/// 1-argument version of a helper function to #cons that accepts arguments that
/// have been already preprocessed with #filter to convert regular variables into
/// #ref and constants into #value.
/// \note This version will be called from #cons that had its target type a #view
template <typename T, size_t layout, typename P1>
inline constr1<T,layout,
            typename underlying<P1>::type
       > 
cons_ex(const view<view<T,layout>>&, P1&& p1) noexcept
{
    return constr1<T,layout,
            typename underlying<P1>::type
           >(
            std::forward<P1>(p1)
           );
}

/// A 1-argument version of a tree-pattern constructor. Target type is allowed
/// to be a #view here.
template <typename T, typename P1>
inline auto C(P1&& p1) noexcept -> XTL_RETURN
(
    cons_ex(
        view<T>(),
        filter(std::forward<P1>(p1))
    )
)

/// A 1-argument version of a tree-pattern constructor that takes layout in
/// addition to the target type.
/// \note #view is not supposed to be passed as a target type to this version
///       of the function because we will then have two potentially conflicting
///       layouts. Any layout different from #default_layout passed here will
///       result in a compile time error.
template <typename T, size_t layout, typename P1>
inline auto C(P1&& p1) noexcept -> XTL_RETURN
(
    cons_ex(
        view<T,layout>(),
        filter(std::forward<P1>(p1))
    )
)

//------------------------------------------------------------------------------

/// 2-argument version of a helper function to #cons that accepts arguments that
/// have been already preprocessed with #filter to convert regular variables into
/// #ref and constants into #value.
/// \note This version will be called from #cons with a non-#view target type
template <typename T, size_t layout, typename P1, typename P2>
inline constr2<T,layout,
            typename underlying<P1>::type,
            typename underlying<P2>::type
       > 
cons_ex(const view<T,layout>&, P1&& p1, P2&& p2) noexcept
{
    return constr2<T,layout,
            typename underlying<P1>::type,
            typename underlying<P2>::type
           >(
            std::forward<P1>(p1),
            std::forward<P2>(p2)
           );
}

/// 2-argument version of a helper function to #cons that accepts arguments that
/// have been already preprocessed with #filter to convert regular variables into
/// #ref and constants into #value.
/// \note This version will be called from #cons that had its target type a #view
template <typename T, size_t layout, typename P1, typename P2>
inline constr2<T,layout,
            typename underlying<P1>::type,
            typename underlying<P2>::type
       > 
cons_ex(const view<view<T,layout>>&, P1&& p1, P2&& p2) noexcept
{
    return constr2<T,layout,
            typename underlying<P1>::type,
            typename underlying<P2>::type
           >(
            std::forward<P1>(p1),
            std::forward<P2>(p2)
           );
}

/// A 2-argument version of a tree-pattern constructor. Target type is allowed
/// to be a #view here.
template <typename T, typename P1, typename P2>
inline auto C(P1&& p1, P2&& p2) noexcept -> XTL_RETURN
(
    cons_ex(
        view<T>(),
        filter(std::forward<P1>(p1)),
        filter(std::forward<P2>(p2))
    )
)

/// A 2-argument version of a tree-pattern constructor that takes layout in
/// addition to the target type.
/// \note #view is not supposed to be passed as a target type to this version
///       of the function because we will then have two potentially conflicting
///       layouts. Any layout different from #default_layout passed here will
///       result in a compile time error.
template <typename T, size_t layout, typename P1, typename P2>
inline auto C(P1&& p1, P2&& p2) noexcept -> XTL_RETURN
(
    cons_ex(
        view<T,layout>(),
        filter(std::forward<P1>(p1)),
        filter(std::forward<P2>(p2))
    )
)

//------------------------------------------------------------------------------

/// 3-argument version of a helper function to #cons that accepts arguments that
/// have been already preprocessed with #filter to convert regular variables into
/// #ref and constants into #value.
/// \note This version will be called from #cons with a non-#view target type
template <typename T, size_t layout, typename P1, typename P2, typename P3>
inline constr3<T,layout,
            typename underlying<P1>::type,
            typename underlying<P2>::type,
            typename underlying<P3>::type
       > 
cons_ex(const view<T,layout>&, P1&& p1, P2&& p2, P3&& p3) noexcept
{
    return constr3<T,layout,
            typename underlying<P1>::type,
            typename underlying<P2>::type,
            typename underlying<P3>::type
           >(
            std::forward<P1>(p1),
            std::forward<P2>(p2),
            std::forward<P3>(p3)
           );
}

/// 3-argument version of a helper function to #cons that accepts arguments that
/// have been already preprocessed with #filter to convert regular variables into
/// #ref and constants into #value.
/// \note This version will be called from #cons that had its target type a #view
template <typename T, size_t layout, typename P1, typename P2, typename P3>
inline constr3<T,layout,
            typename underlying<P1>::type,
            typename underlying<P2>::type,
            typename underlying<P3>::type
       > 
cons_ex(const view<view<T,layout>>&, P1&& p1, P2&& p2, P3&& p3) noexcept
{
    return constr3<T,layout,
            typename underlying<P1>::type,
            typename underlying<P2>::type,
            typename underlying<P3>::type
           >(
            std::forward<P1>(p1),
            std::forward<P2>(p2),
            std::forward<P3>(p3)
           );
}

/// A 3-argument version of a tree-pattern constructor. Target type is allowed
/// to be a #view here.
template <typename T, typename P1, typename P2, typename P3>
inline auto C(P1&& p1, P2&& p2, P3&& p3) noexcept -> XTL_RETURN
(
    cons_ex(
        view<T>(),
        filter(std::forward<P1>(p1)),
        filter(std::forward<P2>(p2)),
        filter(std::forward<P3>(p3))
    )
)

/// A 3-argument version of a tree-pattern constructor that takes layout in
/// addition to the target type.
/// \note #view is not supposed to be passed as a target type to this version
///       of the function because we will then have two potentially conflicting
///       layouts. Any layout different from #default_layout passed here will
///       result in a compile time error.
template <typename T, size_t layout, typename P1, typename P2, typename P3>
inline auto C(P1&& p1, P2&& p2, P3&& p3) noexcept -> XTL_RETURN
(
    cons_ex(
        view<T,layout>(),
        filter(std::forward<P1>(p1)),
        filter(std::forward<P2>(p2)),
        filter(std::forward<P3>(p3))
    )
)

//------------------------------------------------------------------------------

/// 4-argument version of a helper function to #cons that accepts arguments that
/// have been already preprocessed with #filter to convert regular variables into
/// #ref and constants into #value.
/// \note This version will be called from #cons with a non-#view target type
template <typename T, size_t layout, typename P1, typename P2, typename P3, typename P4>
inline constr4<T,layout,
            typename underlying<P1>::type,
            typename underlying<P2>::type,
            typename underlying<P3>::type,
            typename underlying<P4>::type
       > 
cons_ex(const view<T,layout>&, P1&& p1, P2&& p2, P3&& p3, P4&& p4) noexcept
{
    return constr4<T,layout,
            typename underlying<P1>::type,
            typename underlying<P2>::type,
            typename underlying<P3>::type,
            typename underlying<P4>::type
           >(
            std::forward<P1>(p1),
            std::forward<P2>(p2),
            std::forward<P3>(p3),
            std::forward<P4>(p4)
           );
}

/// 4-argument version of a helper function to #cons that accepts arguments that
/// have been already preprocessed with #filter to convert regular variables into
/// #ref and constants into #value.
/// \note This version will be called from #cons that had its target type a #view
template <typename T, size_t layout, typename P1, typename P2, typename P3, typename P4>
inline constr4<T,layout,
            typename underlying<P1>::type,
            typename underlying<P2>::type,
            typename underlying<P3>::type,
            typename underlying<P4>::type
       > 
cons_ex(const view<view<T,layout>>&, P1&& p1, P2&& p2, P3&& p3, P4&& p4) noexcept
{
    return constr4<T,layout,
            typename underlying<P1>::type,
            typename underlying<P2>::type,
            typename underlying<P3>::type,
            typename underlying<P4>::type
           >(
            std::forward<P1>(p1),
            std::forward<P2>(p2),
            std::forward<P3>(p3),
            std::forward<P4>(p4)
           );
}

/// A 4-argument version of a tree-pattern constructor. Target type is allowed
/// to be a #view here.
template <typename T, typename P1, typename P2, typename P3, typename P4>
inline auto C(P1&& p1, P2&& p2, P3&& p3, P4&& p4) noexcept -> XTL_RETURN
(
    cons_ex(
        view<T>(),
        filter(std::forward<P1>(p1)),
        filter(std::forward<P2>(p2)),
        filter(std::forward<P3>(p3)),
        filter(std::forward<P4>(p4))
    )
)

/// A 4-argument version of a tree-pattern constructor that takes layout in
/// addition to the target type.
/// \note #view is not supposed to be passed as a target type to this version
///       of the function because we will then have two potentially conflicting
///       layouts. Any layout different from #default_layout passed here will
///       result in a compile time error.
template <typename T, size_t layout, typename P1, typename P2, typename P3, typename P4>
inline auto C(P1&& p1, P2&& p2, P3&& p3, P4&& p4) noexcept -> XTL_RETURN
(
    cons_ex(
        view<T,layout>(),
        filter(std::forward<P1>(p1)),
        filter(std::forward<P2>(p2)),
        filter(std::forward<P3>(p3)),
        filter(std::forward<P4>(p4))
    )
)

//------------------------------------------------------------------------------

/// #is_pattern_ is a helper meta-predicate capable of distinguishing all our patterns
template <typename T, size_t L>                                                     struct is_pattern_<constr0<T,L>>              { static const bool value = true; };
template <typename T, size_t L, typename P1>                                        struct is_pattern_<constr1<T,L,P1>>           { static const bool value = true; };
template <typename T, size_t L, typename P1, typename P2>                           struct is_pattern_<constr2<T,L,P1,P2>>        { static const bool value = true; };
template <typename T, size_t L, typename P1, typename P2, typename P3>              struct is_pattern_<constr3<T,L,P1,P2,P3>>     { static const bool value = true; };
template <typename T, size_t L, typename P1, typename P2, typename P3, typename P4> struct is_pattern_<constr4<T,L,P1,P2,P3,P4>>  { static const bool value = true; };

//------------------------------------------------------------------------------

} // of namespace mch
