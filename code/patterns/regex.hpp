///
/// \file regex.hpp
///
/// This file defines regular expression pattern.
///
/// \autor Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of the XTL framework (http://parasol.tamu.edu/xtl/).
/// Copyright (C) 2005-2012 Texas A&M University.
/// All rights reserved.
///

#pragma once

#include "primitive.hpp"
#include <regex>
#include <string>

//------------------------------------------------------------------------------

/// RegEx pattern of 0 arguments
struct regex0 : std::regex
{
    typedef std::string result_type;
    regex0(const char* re) : std::regex(re) {}
    bool operator()(const std::string& s) const noexcept { return operator()(s.c_str()); }
    bool operator()(const char*        s) const noexcept { std::cmatch m; return regex_match(s,m,*this); }
};

//------------------------------------------------------------------------------

/// RegEx pattern of 1 arguments
template <typename P1>
struct regex1 : std::regex
{
    typedef std::string result_type;
    static_assert(is_pattern<P1>::value,    "Argument P1 of a regex-pattern must be a pattern");

    explicit regex1(const char* re, const P1&  p1) noexcept : std::regex(re), m_p1(p1) {}
    explicit regex1(const char* re,       P1&& p1) noexcept : std::regex(re), m_p1(std::move(p1)) {}
             regex1(regex1&& r)                    noexcept : std::regex(std::move(r)), m_p1(std::move(r.m_p1)) {}


    bool operator()(const std::string& s) const noexcept { return operator()(s.c_str()); }
    bool operator()(const char*        s) const noexcept
    {
        std::cmatch m; 

        if (regex_match(s,m,*this))
        {
            XTL_ASSERT(m.size() >= 1); // There should be enough capture groups for each of the pattern arguments
            typename P1::result_type m1;
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
    regex1<decltype(filter(std::forward<P1>(p1)))>(re, filter(std::forward<P1>(p1)))
)

//------------------------------------------------------------------------------

template <>            struct is_pattern_<regex0>     { enum { value = true }; };
template <typename P1> struct is_pattern_<regex1<P1>> { enum { value = true }; };
