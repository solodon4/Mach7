///
/// \file regex.hpp
///
/// This file defines regular expression pattern.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of Mach7 library (http://parasol.tamu.edu/mach7/).
/// Copyright (C) 2011-2012 Texas A&M University.
/// All rights reserved.
///

#pragma once

#include "common.hpp"
#include <regex>
#include <string>

namespace mch ///< Mach7 library namespace
{

//------------------------------------------------------------------------------

/// RegEx pattern of 0 arguments
struct regex0 : std::regex
{
    /// Type function returning a type that will be accepted by the pattern for
    /// a given subject type S. We use type function instead of an associated 
    /// type, because there is no a single accepted type for a #wildcard pattern
    /// for example. Requirement of #Pattern concept.
    template <typename S> struct accepted_type_for { typedef std::string type; };

    regex0(const char* re) : std::regex(re) {}
    bool operator()(const std::string& s) const noexcept { return operator()(s.c_str()); }
    bool operator()(const char*        s) const noexcept { std::cmatch m; return regex_match(s,m,*this); }
};

//------------------------------------------------------------------------------

/// RegEx pattern of 1 arguments
template <typename P1>
struct regex1 : std::regex
{
    static_assert(is_pattern<P1>::value,    "Argument P1 of a regex-pattern must be a pattern");

    /// Type function returning a type that will be accepted by the pattern for
    /// a given subject type S. We use type function instead of an associated 
    /// type, because there is no a single accepted type for a #wildcard pattern
    /// for example. Requirement of #Pattern concept.
    template <typename S> struct accepted_type_for { typedef std::string type; };

    explicit regex1(const char* re, const P1&  p1) noexcept : std::regex(re), m_p1(p1) {}
    explicit regex1(const char* re,       P1&& p1) noexcept : std::regex(re), m_p1(std::move(p1)) {}
             regex1(regex1&& r)                    noexcept : std::regex(std::move(r)), m_p1(std::move(r.m_p1)) {}
    regex1& operator=(const regex1&); ///< Assignment is not allowed for this class


    bool operator()(const std::string& s) const noexcept { return operator()(s.c_str()); }
    bool operator()(const char*        s) const noexcept
    {
        std::cmatch m; 

        if (regex_match(s,m,*this))
        {
            XTL_ASSERT(m.size() >= 1); // There should be enough capture groups for each of the pattern arguments
            typename P1::template accepted_type_for<std::string>::type m1;
            std::stringstream ss(m[1]);
            return (ss >> m1) && m_p1(m1);
        }

        return false;
    }
    P1 m_p1;
};

//------------------------------------------------------------------------------

inline regex0 rex(const char* re) noexcept { return regex0(re); }

template <typename P1>
inline auto rex(const char* re, P1&& p1) noexcept -> XTL_RETURN
(
    regex1<
        typename underlying<decltype(filter(std::forward<P1>(p1)))>::type
    >(
        re, 
        filter(std::forward<P1>(p1))
     )
)

//------------------------------------------------------------------------------

template <>            struct is_pattern_<regex0>     { static const bool value = true; };
template <typename P1> struct is_pattern_<regex1<P1>> { static const bool value = true; };

//------------------------------------------------------------------------------

} // of namespace mch
