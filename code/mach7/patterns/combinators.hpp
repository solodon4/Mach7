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
/// \file combinators.hpp
///
/// This file defines pattern combinators supported by our library.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// \see https://parasol.tamu.edu/mach7/
/// \see https://github.com/solodon4/Mach7
/// \see https://github.com/solodon4/SELL
///

#pragma once

#include "primitive.hpp" // FIX: Ideally this should be common.hpp, but GCC seem to disagree: http://gcc.gnu.org/bugzilla/show_bug.cgi?id=55460

namespace mch ///< Mach7 library namespace
{

//------------------------------------------------------------------------------

/// Conjunction pattern combinator.
/// We have a problem with pattern combinators as they also can participate in 
/// lazy expressions:
/// - x |= x > 3 && x < 7   - lazy expression combining two subexpression in a guard pattern
/// - C<Car>() && C<Ship>() - pattern that will match a class that is derived from both Car and Ship
/// FIX: Make both && and || patterns accepted_type_for<S> to be the common base class (join)
///      Or should we declare 2 operator() with each accepted_type?
template <typename P1, typename P2>
struct conjunction
{
    static_assert(is_pattern<P1>::value, "Argument P1 of a conjunction-pattern must be a pattern");
    static_assert(is_pattern<P2>::value, "Argument P2 of a conjunction-pattern must be a pattern");
    static_assert(!is_var<P1>::value,    "Attempting to host var<> directly. Use filter() to wrap it into ref2<>");
    static_assert(!is_var<P2>::value,    "Attempting to host var<> directly. Use filter() to wrap it into ref2<>");

    constexpr conjunction(const P1&  p1, const P2&  p2) noexcept_when(std::is_nothrow_copy_constructible<P1>::value && std::is_nothrow_copy_constructible<P2>::value) : m_p1(          p1 ), m_p2(          p2 ) {}
    constexpr conjunction(      P1&& p1, const P2&  p2) noexcept_when(std::is_nothrow_move_constructible<P1>::value && std::is_nothrow_copy_constructible<P2>::value) : m_p1(std::move(p1)), m_p2(          p2 ) {}
    constexpr conjunction(const P1&  p1,       P2&& p2) noexcept_when(std::is_nothrow_copy_constructible<P1>::value && std::is_nothrow_move_constructible<P2>::value) : m_p1(          p1 ), m_p2(std::move(p2)) {}
    constexpr conjunction(      P1&& p1,       P2&& p2) noexcept_when(std::is_nothrow_move_constructible<P1>::value && std::is_nothrow_move_constructible<P2>::value) : m_p1(std::move(p1)), m_p2(std::move(p2)) {}
    constexpr conjunction(const conjunction&  c)        noexcept_when(std::is_nothrow_copy_constructible<P1>::value && std::is_nothrow_copy_constructible<P2>::value) : m_p1(          c.m_p1 ), m_p2(          c.m_p2 ) {} ///< Copy constructor    
    constexpr conjunction(      conjunction&& c)        noexcept_when(std::is_nothrow_move_constructible<P1>::value && std::is_nothrow_move_constructible<P2>::value) : m_p1(std::move(c.m_p1)), m_p2(std::move(c.m_p2)) {} ///< Move constructor
    conjunction& operator=(const conjunction&) XTL_DELETED; ///< Assignment is not allowed for this class

    /// Type function returning a type that will be accepted by the pattern for
    /// a given subject type S. We use type function instead of an associated 
    /// type, because there is no a single accepted type for a #wildcard pattern
    /// for example. Requirement of #Pattern concept.
    template <typename S> 
    struct accepted_type_for 
    { 
        static_assert(std::is_same<
                          typename P1::template accepted_type_for<S>::type,
                          typename P2::template accepted_type_for<S>::type
                      >::value,"The type accepted by patterns in conjunction must be the same");
        typedef typename P1::template accepted_type_for<S>::type type; 
    };

    /// We parameterize over accepted type since the actually accepted type is 
    /// a function of the subject type.
    template <typename T>
    constexpr bool operator()(const T& subject) const { return m_p1(subject) && m_p2(subject); }

    P1 m_p1; ///< The 1st pattern in conjunction
    P2 m_p2; ///< The 2nd pattern in conjunction
};

/// #is_pattern_ is a helper meta-predicate capable of distinguishing all our patterns
template <typename P1, typename P2> struct is_pattern_<conjunction<P1,P2>> { static const bool value = true /*is_pattern<P1>::value && is_pattern<P2>::value*/; };

/// #is_expression_ is a helper meta-predicate that separates lazily evaluatable expressions we support
template <typename E1, typename E2> struct is_expression_<conjunction<E1,E2>> { static const bool value = is_expression<E1>::value && is_expression<E2>::value; };

//------------------------------------------------------------------------------

/// Disjunction pattern combinator
template <typename P1, typename P2>
struct disjunction
{
    static_assert(is_pattern<P1>::value, "Argument P1 of a disjunction-pattern must be a pattern");
    static_assert(is_pattern<P2>::value, "Argument P2 of a disjunction-pattern must be a pattern");
    static_assert(!is_var<P1>::value,    "Attempting to host var<> directly. Use filter() to wrap it into ref2<>");
    static_assert(!is_var<P2>::value,    "Attempting to host var<> directly. Use filter() to wrap it into ref2<>");

    constexpr disjunction(const P1&  p1, const P2&  p2) noexcept_when(std::is_nothrow_copy_constructible<P1>::value && std::is_nothrow_copy_constructible<P2>::value) : m_p1(          p1 ), m_p2(          p2 ) {}
    constexpr disjunction(      P1&& p1, const P2&  p2) noexcept_when(std::is_nothrow_move_constructible<P1>::value && std::is_nothrow_copy_constructible<P2>::value) : m_p1(std::move(p1)), m_p2(          p2 ) {}
    constexpr disjunction(const P1&  p1,       P2&& p2) noexcept_when(std::is_nothrow_copy_constructible<P1>::value && std::is_nothrow_move_constructible<P2>::value) : m_p1(          p1 ), m_p2(std::move(p2)) {}
    constexpr disjunction(      P1&& p1,       P2&& p2) noexcept_when(std::is_nothrow_move_constructible<P1>::value && std::is_nothrow_move_constructible<P2>::value) : m_p1(std::move(p1)), m_p2(std::move(p2)) {}
    constexpr disjunction(const disjunction&  c)        noexcept_when(std::is_nothrow_copy_constructible<P1>::value && std::is_nothrow_copy_constructible<P2>::value) : m_p1(          c.m_p1 ), m_p2(          c.m_p2 ) {} ///< Copy constructor    
    constexpr disjunction(      disjunction&& c)        noexcept_when(std::is_nothrow_move_constructible<P1>::value && std::is_nothrow_move_constructible<P2>::value) : m_p1(std::move(c.m_p1)), m_p2(std::move(c.m_p2)) {} ///< Move constructor
    disjunction& operator=(const disjunction&) XTL_DELETED; ///< Assignment is not allowed for this class

    /// Type function returning a type that will be accepted by the pattern for
    /// a given subject type S. We use type function instead of an associated 
    /// type, because there is no a single accepted type for a #wildcard pattern
    /// for example. Requirement of #Pattern concept.
    template <typename S> 
    struct accepted_type_for 
    { 
        static_assert(std::is_same<
                          typename P1::template accepted_type_for<S>::type,
                          typename P2::template accepted_type_for<S>::type
                      >::value,"The type accepted by patterns in disjunction must be the same");
        //typedef typename std::common_type<
        //                    typename P1::template accepted_type_for<S>::type,
        //                    typename P2::template accepted_type_for<S>::type
        //                 >::type type; 
    };

    /// We parameterize over accepted type since the actually accepted type is 
    /// a function of the subject type.
    template <typename T>
    constexpr bool operator()(const T& subject) const { return m_p1(subject) || m_p2(subject); }

    P1 m_p1; ///< The 1st pattern of disjunction combinator
    P2 m_p2; ///< The 2nd pattern of disjunction combinator
};

/// #is_pattern_ is a helper meta-predicate capable of distinguishing all our patterns
template <typename P1, typename P2> struct is_pattern_<disjunction<P1,P2>> { static const bool value = true /*is_pattern<P1>::value && is_pattern<P2>::value*/; };

/// #is_expression_ is a helper meta-predicate that separates lazily evaluatable expressions we support
template <typename E1, typename E2> struct is_expression_<disjunction<E1,E2>> { static const bool value = is_expression<E1>::value && is_expression<E2>::value; };

//------------------------------------------------------------------------------

/// Negation pattern combinator
template <typename P1>
struct negation
{
    static_assert(is_pattern<P1>::value, "Argument P1 of a negation-pattern must be a pattern");
    static_assert(!is_var<P1>::value,    "Attempting to host var<> directly. Use filter() to wrap it into ref2<>");

    constexpr negation(const P1&  p1)      noexcept_when(std::is_nothrow_copy_constructible<P1>::value) : m_p1(p1) {}
    constexpr negation(      P1&& p1)      noexcept_when(std::is_nothrow_move_constructible<P1>::value) : m_p1(std::move(p1)) {}
    constexpr negation(const negation&  c) noexcept_when(std::is_nothrow_copy_constructible<P1>::value) : m_p1(          c.m_p1 ) {} ///< Copy constructor    
    constexpr negation(      negation&& c) noexcept_when(std::is_nothrow_move_constructible<P1>::value) : m_p1(std::move(c.m_p1)) {} ///< Move constructor
    negation& operator=(const negation&) XTL_DELETED; ///< Assignment is not allowed for this class

    /// Type function returning a type that will be accepted by the pattern for
    /// a given subject type S. We use type function instead of an associated 
    /// type, because there is no a single accepted type for a #wildcard pattern
    /// for example. Requirement of #Pattern concept.
    template <typename S> struct accepted_type_for : P1::template accepted_type_for<S> { typedef S type; };

    /// We parameterize over accepted type since the actually accepted type is 
    /// a function of the subject type.
    template <typename T>
    constexpr bool operator()(const T& subject) const { return !m_p1(subject); }

    P1 m_p1; ///< The argument pattern of negation combinator
};

/// #is_pattern_ is a helper meta-predicate capable of distinguishing all our patterns
template <typename P1> struct is_pattern_<negation<P1>> { static const bool value = true /*is_pattern<P1>::value*/; };

/// #is_expression_ is a helper meta-predicate that separates lazily evaluatable expressions we support
template <typename E1> struct is_expression_<negation<E1>> { static const bool value = is_expression<E1>::value; };

//------------------------------------------------------------------------------

///@{
/// Set of overloads capable of decomposing an expression template that models
/// a LazyExpression concept and evaluating it.
/// \note See header files of other patterns for more overloads!
template <typename E1, typename E2>              
inline auto eval(const conjunction<E1,E2>& e)
        -> typename std::enable_if<
                        is_expression<E1>::value && is_expression<E2>::value,
                        decltype(eval(e.m_p1) && eval(e.m_p2))
                    >::type
{
    return eval(e.m_p1) && eval(e.m_p2); 
}

template <typename E1, typename E2>              
inline auto eval(const disjunction<E1,E2>& e)
        -> typename std::enable_if<
                        is_expression<E1>::value && is_expression<E2>::value,
                        decltype(eval(e.m_p1) || eval(e.m_p2))
                    >::type
{
    return eval(e.m_p1) || eval(e.m_p2); 
}

template <typename E1>              
inline auto eval(const negation<E1>& e)
        -> typename std::enable_if<
                        is_expression<E1>::value,
                        decltype(!eval(e.m_p1))
                    >::type
{
    return !eval(e.m_p1); 
}
///@}

//------------------------------------------------------------------------------

} // of namespace mch

//------------------------------------------------------------------------------

/// \note This operator has to be in the global namespace as Pi is unrestricted
///       and ADL won't work to find it!
template <typename P1, typename P2>
inline auto operator&&(P1&& p1, P2&& p2) noexcept
        -> typename std::enable_if<
                        mch::either_is_pattern<P1,P2>::value,
                        mch::conjunction<
                            typename mch::underlying<decltype(mch::filter(std::forward<P1>(p1)))>::type,
                            typename mch::underlying<decltype(mch::filter(std::forward<P2>(p2)))>::type
                        >
                    >::type
{
    return mch::conjunction<
                            typename mch::underlying<decltype(mch::filter(std::forward<P1>(p1)))>::type,
                            typename mch::underlying<decltype(mch::filter(std::forward<P2>(p2)))>::type
                        >(
                            mch::filter(std::forward<P1>(p1)),
                            mch::filter(std::forward<P2>(p2))
                         );
}

//------------------------------------------------------------------------------

/// \note This operator has to be in the global namespace as Pi is unrestricted
///       and ADL won't work to find it!
template <typename P1, typename P2>
inline auto operator||(P1&& p1, P2&& p2) noexcept
        -> typename std::enable_if<
                        mch::either_is_pattern<P1,P2>::value,
                        mch::disjunction<
                            typename mch::underlying<decltype(mch::filter(std::forward<P1>(p1)))>::type,
                            typename mch::underlying<decltype(mch::filter(std::forward<P2>(p2)))>::type
                        >
                    >::type
{
    return mch::disjunction<
                            typename mch::underlying<decltype(mch::filter(std::forward<P1>(p1)))>::type,
                            typename mch::underlying<decltype(mch::filter(std::forward<P2>(p2)))>::type
                        >(
                            mch::filter(std::forward<P1>(p1)),
                            mch::filter(std::forward<P2>(p2))
                         );
}

//------------------------------------------------------------------------------

/// \note This operator has to be in the global namespace as Pi is unrestricted
///       and ADL won't work to find it!
template <typename P1>
inline auto operator!(P1&& p1) noexcept 
        -> typename std::enable_if<
                        mch::is_pattern<P1>::value, 
                        mch::negation<
                            typename mch::underlying<decltype(mch::filter(std::forward<P1>(p1)))>::type
                        >
                    >::type 
{
    return mch::negation<
                            typename mch::underlying<decltype(mch::filter(std::forward<P1>(p1)))>::type
                        >(
                            mch::filter(std::forward<P1>(p1))
                        );
}

//------------------------------------------------------------------------------
