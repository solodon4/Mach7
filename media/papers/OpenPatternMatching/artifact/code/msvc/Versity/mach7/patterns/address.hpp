///
/// \file address.hpp
///
/// This file defines address patterns supported by our library.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of Mach7 library (http://parasol.tamu.edu/mach7/).
/// Copyright (C) 2011-2012 Texas A&M University.
/// All rights reserved.
///

#pragma once

#include "primitive.hpp" // FIX: Ideally this should be common.hpp, but GCC seem to disagree: http://gcc.gnu.org/bugzilla/show_bug.cgi?id=55460

namespace mch ///< Mach7 library namespace
{

//------------------------------------------------------------------------------

template <typename P1>
struct address
{
    static_assert(is_pattern<P1>::value, "Argument P1 of an address pattern must be a pattern");
    static_assert(!is_var<P1>::value,    "Attempting to host var<> directly. Use filter() to wrap it into ref2<>");

    explicit address(const P1&  p) noexcept : m_p1(          p ) {}
    explicit address(      P1&& p) noexcept : m_p1(std::move(p)) {}
    address(const address&  e) noexcept : m_p1(          e.m_p1 ) {} ///< Copy constructor
    address(      address&& e) noexcept : m_p1(std::move(e.m_p1)) {} ///< Move constructor
    address& operator=(const address&); ///< Assignment is not allowed for this class

    /// Type function returning a type that will be accepted by the pattern for
    /// a given subject type S. We use type function instead of an associated 
    /// type, because there is no a single accepted type for a #wildcard pattern
    /// for example. Requirement of #Pattern concept.
    template <typename S> struct accepted_type_for; // Intentionally no definition
    template <typename S> struct accepted_type_for<S*> { typedef typename P1::template accepted_type_for<S>::type* type; };

    template <typename T> bool operator()(const T* t) const { return t && m_p1(*t); }
    template <typename T> bool operator()(      T* t) const { return t && m_p1(*t); }

    P1 m_p1;
};

//------------------------------------------------------------------------------

/// #is_pattern_ is a helper meta-predicate capable of distinguishing all our patterns
template <typename P1> struct is_pattern_<address<P1>> { static const bool value = true; };

//------------------------------------------------------------------------------

} // of namespace mch

//------------------------------------------------------------------------------

/// \note This operator has to be in the global namespace as E is unrestricted
///       and ADL won't work to find it!
template <typename P1>
inline auto operator&(P1&& p1) noexcept
        -> typename std::enable_if<
                        mch::is_pattern<P1>::value,
                        mch::address<
                            typename mch::underlying<decltype(mch::filter(std::forward<P1>(p1)))>::type
                        >
                    >::type
{
    return mch::address<
                        typename mch::underlying<decltype(mch::filter(std::forward<P1>(p1)))>::type
                    >(
                        mch::filter(std::forward<P1>(p1))
                    );
}

//------------------------------------------------------------------------------
