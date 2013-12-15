///
/// \file debug.hpp
///
/// Debugging support for Mach7 library
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of Mach7 library (http://parasol.tamu.edu/mach7/).
/// Copyright (C) 2011-2012 Texas A&M University.
/// All rights reserved.
///

#pragma once

#include <ostream>
#include <iomanip>

namespace mch ///< Mach7 library namespace
{

//------------------------------------------------------------------------------

/// We make this class parameterized only because we want subsequent 
/// uses of it be generated from this header file. For this we need 
/// linker to throw away duplicates of #instances initialization.
template <typename F, F& f>
struct call_on_last_instance
{
    call_on_last_instance() { ++instances; }
   ~call_on_last_instance() { if (--instances == 0) f(); }
    static size_t instances;
};

template <typename F, F& f>
size_t call_on_last_instance<F,f>::instances = 0;

//------------------------------------------------------------------------------

struct likliness_tracer
{
#ifdef _MSC_VER
  #pragma warning( push )
  #pragma warning( disable : 4351 ) // warning C4351: new behavior: elements of array ... will be default initialized
#endif

    likliness_tracer(const char* s, const char* f, int l) : counts(), text(s), file(f), line(l) {}

#ifdef _MSC_VER
  #pragma warning( pop )
#endif

    size_t& operator[](bool i) { return counts[i]; }

    std::ostream& operator>>(std::ostream& os) const
    {
        size_t Y = counts[true], N = counts[false];
        return os << file << '(' << line << ')' 
                  << (Y > N ? "   LIKELY " : " unlikely ") 
                  << " Yes: "        << std::setw(10) << Y
                  << " No:  "        << std::setw(10) << N
                  << " Total: "      << std::setw(10) << (Y+N)
                  << " Percentage: " << std::setw(6)  << std::fixed << std::setprecision(2) << 100*double(std::max(Y,N))/(Y+N) << '%' 
                  << " Condition: "  << text;
    }

    friend std::ostream& operator<<(std::ostream& os, const likliness_tracer& t) { return t >> os; }

    size_t counts[2];
    const char* text;
    const char* file;
    int         line;
};

//------------------------------------------------------------------------------

template <bool likely>
struct likliness_tracer_of : likliness_tracer
{
    likliness_tracer_of(const char* s, const char* f, int l) : likliness_tracer(s,f,l) {}
   ~likliness_tracer_of()
    {
        if (likely != (counts[true] >= counts[false]))
            std::clog << *this << std::endl;
    }
};

//------------------------------------------------------------------------------

template <bool likeliness, int line, typename T/*=void*/>
static bool trace_likeliness(bool c, const char* text, const char* file)
{
    static likliness_tracer_of<likeliness> tracer(text,file,line);
    ++tracer[c];
    return c;
}

//------------------------------------------------------------------------------

} // of namespace mch
