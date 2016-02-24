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
/// This file defines functors similar to those of STL but capable of computing
/// given operator on any type, not just the one the functor was instantiated
/// with. The file also defines solvers for those operations that can be inverted.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// \see https://parasol.tamu.edu/mach7/
/// \see https://github.com/solodon4/Mach7
/// \see https://github.com/solodon4/SELL
///

#pragma once

#include <utility>

namespace mch ///< Mach7 library namespace
{

//==============================================================================
// Functors implementing a particular operation for any pair of types
//==============================================================================

struct unary_minus     { template <class A>          auto operator()(A&& a)        const -> decltype(-std::forward<A>(a))     { return -std::forward<A>(a); } };
struct bit_complement  { template <class A>          auto operator()(A&& a)        const -> decltype(~std::forward<A>(a))     { return ~std::forward<A>(a); } };
struct bool_complement { template <class A>          auto operator()(A&& a)        const -> decltype(!std::forward<A>(a))     { return !std::forward<A>(a); } };

struct addition        { template <class A, class B> auto operator()(A&& a, B&& b) const -> decltype(std::forward<A>(a) +  std::forward<B>(b)) { return std::forward<A>(a) +  std::forward<B>(b); } };
struct subtraction     { template <class A, class B> auto operator()(A&& a, B&& b) const -> decltype(std::forward<A>(a) -  std::forward<B>(b)) { return std::forward<A>(a) -  std::forward<B>(b); } };
struct multiplication  { template <class A, class B> auto operator()(A&& a, B&& b) const -> decltype(std::forward<A>(a) *  std::forward<B>(b)) { return std::forward<A>(a) *  std::forward<B>(b); } };
struct division        { template <class A, class B> auto operator()(A&& a, B&& b) const -> decltype(std::forward<A>(a) /  std::forward<B>(b)) { return std::forward<A>(a) /  std::forward<B>(b); } };
struct modulo          { template <class A, class B> auto operator()(A&& a, B&& b) const -> decltype(std::forward<A>(a) %  std::forward<B>(b)) { return std::forward<A>(a) %  std::forward<B>(b); } };
struct bit_and         { template <class A, class B> auto operator()(A&& a, B&& b) const -> decltype(std::forward<A>(a) &  std::forward<B>(b)) { return std::forward<A>(a) &  std::forward<B>(b); } };
struct bit_or          { template <class A, class B> auto operator()(A&& a, B&& b) const -> decltype(std::forward<A>(a) |  std::forward<B>(b)) { return std::forward<A>(a) |  std::forward<B>(b); } };
struct bit_xor         { template <class A, class B> auto operator()(A&& a, B&& b) const -> decltype(std::forward<A>(a) ^  std::forward<B>(b)) { return std::forward<A>(a) ^  std::forward<B>(b); } };
struct bit_shift_left  { template <class A, class B> auto operator()(A&& a, B&& b) const -> decltype(std::forward<A>(a) << std::forward<B>(b)) { return std::forward<A>(a) << std::forward<B>(b); } };
struct bit_shift_right { template <class A, class B> auto operator()(A&& a, B&& b) const -> decltype(std::forward<A>(a) >> std::forward<B>(b)) { return std::forward<A>(a) >> std::forward<B>(b); } };
struct bool_and        { template <class A, class B> auto operator()(A&& a, B&& b) const -> decltype(std::forward<A>(a) && std::forward<B>(b)) { return std::forward<A>(a) && std::forward<B>(b); } };
struct bool_or         { template <class A, class B> auto operator()(A&& a, B&& b) const -> decltype(std::forward<A>(a) || std::forward<B>(b)) { return std::forward<A>(a) || std::forward<B>(b); } };
struct equal           { template <class A, class B> auto operator()(A&& a, B&& b) const -> decltype(std::forward<A>(a) == std::forward<B>(b)) { return std::forward<A>(a) == std::forward<B>(b); } };
struct not_equal       { template <class A, class B> auto operator()(A&& a, B&& b) const -> decltype(std::forward<A>(a) != std::forward<B>(b)) { return std::forward<A>(a) != std::forward<B>(b); } };
struct greater         { template <class A, class B> auto operator()(A&& a, B&& b) const -> decltype(std::forward<A>(a) >  std::forward<B>(b)) { return std::forward<A>(a) >  std::forward<B>(b); } };
struct greater_equal   { template <class A, class B> auto operator()(A&& a, B&& b) const -> decltype(std::forward<A>(a) >= std::forward<B>(b)) { return std::forward<A>(a) >= std::forward<B>(b); } };
struct less            { template <class A, class B> auto operator()(A&& a, B&& b) const -> decltype(std::forward<A>(a) <  std::forward<B>(b)) { return std::forward<A>(a) <  std::forward<B>(b); } };
struct less_equal      { template <class A, class B> auto operator()(A&& a, B&& b) const -> decltype(std::forward<A>(a) <= std::forward<B>(b)) { return std::forward<A>(a) <= std::forward<B>(b); } };

//struct unary_minus     { template <class A>          auto operator()(const A& a)             const -> decltype(-a)     { return -a; } };
//struct bit_complement  { template <class A>          auto operator()(const A& a)             const -> decltype(~a)     { return ~a; } };
//struct bool_complement { template <class A>          auto operator()(const A& a)             const -> decltype(!a)     { return !a; } };
//
//struct addition        { template <class A, class B> auto operator()(const A& a, const B& b) const -> decltype(a +  b) { return a +  b; } };
//struct subtraction     { template <class A, class B> auto operator()(const A& a, const B& b) const -> decltype(a -  b) { return a -  b; } };
//struct multiplication  { template <class A, class B> auto operator()(const A& a, const B& b) const -> decltype(a *  b) { return a *  b; } };
//struct division        { template <class A, class B> auto operator()(const A& a, const B& b) const -> decltype(a /  b) { return a /  b; } };
//struct modulo          { template <class A, class B> auto operator()(const A& a, const B& b) const -> decltype(a %  b) { return a %  b; } };
//struct bit_and         { template <class A, class B> auto operator()(const A& a, const B& b) const -> decltype(a &  b) { return a &  b; } };
//struct bit_or          { template <class A, class B> auto operator()(const A& a, const B& b) const -> decltype(a |  b) { return a |  b; } };
//struct bit_xor         { template <class A, class B> auto operator()(const A& a, const B& b) const -> decltype(a ^  b) { return a ^  b; } };
//struct bit_shift_left  { template <class A, class B> auto operator()(const A& a, const B& b) const -> decltype(a << b) { return a << b; } };
//struct bit_shift_right { template <class A, class B> auto operator()(const A& a, const B& b) const -> decltype(a >> b) { return a >> b; } };
//struct bool_and        { template <class A, class B> auto operator()(const A& a, const B& b) const -> decltype(a && b) { return a && b; } };
//struct bool_or         { template <class A, class B> auto operator()(const A& a, const B& b) const -> decltype(a || b) { return a || b; } };
//struct equal           { template <class A, class B> auto operator()(const A& a, const B& b) const -> decltype(a == b) { return a == b; } };
//struct not_equal       { template <class A, class B> auto operator()(const A& a, const B& b) const -> decltype(a != b) { return a != b; } };
//struct greater         { template <class A, class B> auto operator()(const A& a, const B& b) const -> decltype(a >  b) { return a >  b; } };
//struct greater_equal   { template <class A, class B> auto operator()(const A& a, const B& b) const -> decltype(a >= b) { return a >= b; } };
//struct less            { template <class A, class B> auto operator()(const A& a, const B& b) const -> decltype(a <  b) { return a <  b; } };
//struct less_equal      { template <class A, class B> auto operator()(const A& a, const B& b) const -> decltype(a <= b) { return a <= b; } };
//------------------------------------------------------------------------------

// There is intentionally no definition. If you get error here, it means there 
// is no solver for operation F.
template <typename F> struct solver;

//------------------------------------------------------------------------------

// Solver for unary_minus operation
template <> struct solver<unary_minus>
{
    // Solver for the first argument: -a == r => a == -r
    template <class A> 
    bool operator()(A& a, const decltype(-a)& r)
    {
        a = -r;
        return -a == r;
    }
};

//------------------------------------------------------------------------------

// Solver for bit_complement operation
template <> struct solver<bit_complement>
{
    // Solver for the first argument: ~a == r => a == ~r
    template <class A> 
    bool operator()(A& a, const decltype(~a)& r)
    {
        a = ~r;
        return ~a == r;
    }
};

//------------------------------------------------------------------------------

// Solver for bool_complement operation
template <> struct solver<bool_complement>
{
    // Solver for the first argument: !a == r => a == !r
    template <class A> 
    bool operator()(A& a, const decltype(!a)& r)
    {
        a = !r;
        return !a == r;
    }
};

//------------------------------------------------------------------------------

// Solver for addition operation
template <> struct solver<addition>
{
    // Solver for the first argument: a+b == r => a == r-b
    template <class A, class B> 
    bool operator()(      A& a, const B& b, const decltype(a+b)& r)
    {
        a = r - b;
        return a + b == r; // Actually will always be true, even with overflows
    }

    // Solver for the second argument: a+b == r => b == r-a
    template <class A, class B> 
    bool operator()(const A& a,       B& b, const decltype(a+b)& r)
    {
        b = r - a;
        return a + b == r; // Actually will always be true, even with overflows
    }
};

//------------------------------------------------------------------------------

// Solver for subtraction operation
template <> struct solver<subtraction>
{
    // Solver for the first argument: a-b == r => a == r+b
    template <class A, class B> 
    bool operator()(      A& a, const B& b, const decltype(a-b)& r)
    {
        a = r + b;
        return a - b == r; // Actually will always be true, even with overflows
    }

    // Solver for the second argument: a-b == r => b == a-r
    template <class A, class B> 
    bool operator()(const A& a,       B& b, const decltype(a-b)& r)
    {
        b = a - r;
        return a - b == r; // Actually will always be true, even with overflows
    }
};

//------------------------------------------------------------------------------

// Solver for multiplication operation
template <> struct solver<multiplication>
{
    // Solver for the first argument: a*b == r => a == r/b
    template <class A, class B> 
    bool operator()(      A& a, const B& b, const decltype(a*b)& r)
    {
        a = r/b;
        return a*b == r; // We need this as for integer division several numbers divided by b will give same result
    }

    // Solver for the second argument: a*b == r => b == r/a
    template <class A, class B> 
    bool operator()(const A& a,       B& b, const decltype(a*b)& r)
    {
        b = r/a;
        return a*b == r; // We need this as for integer division several numbers divided by b will give same result
    }
};

//------------------------------------------------------------------------------

// Solver for division operation
template <> struct solver<division>
{
    // Solver for the first argument: a/b == r => a == r*b
    template <class A, class B> 
    bool operator()(      A& a, const B& b, const decltype(a/b)& r)
    {
        a = r*b;
        return a/b == r; // We need this as for integer division several numbers divided by b will give same result
    }

    // Solver for the second argument: a/b == r => b == a/r
    template <class A, class B> 
    bool operator()(const A& a,       B& b, const decltype(a/b)& r)
    {
        b = a/r;
        return a/b == r; // We need this as for integer division several numbers divided by b will give same result
    }
};

//------------------------------------------------------------------------------

template <typename F, typename A1>
inline bool do_solve(F f, A1& a1, const decltype(f(a1))& /*r*/)
{
    return false;
}

template <typename F, typename A1, typename A2>
inline bool do_solve(F f, A1& a1, const A2& a2, const decltype(f(a1,a2))& /*r*/)
{
    return false;
}

template <typename F, typename A1, typename A2>
inline bool do_solve(F f, const A1& a1, A2& a2, const decltype(f(a1,a2))& /*r*/)
{
    return false;
}

//------------------------------------------------------------------------------

// Solver for unary_minus operation
// Solver for the first argument: -a == r => a == -r
template <class A>
inline bool do_solve(unary_minus, A& a, const decltype(-a)& r)
{
    a = -r;
    return -a == r;
}

//------------------------------------------------------------------------------

// Solver for bit_complement operation
// Solver for the first argument: ~a == r => a == ~r
template <class A> 
inline bool do_solve(bit_complement, A& a, const decltype(~a)& r)
{
    a = ~r;
    return ~a == r;
}

//------------------------------------------------------------------------------

// Solver for bool_complement operation
// Solver for the first argument: !a == r => a == !r
template <class A> 
inline bool do_solve(bool_complement, A& a, const decltype(!a)& r)
{
    a = !r;
    return !a == r;
}

//------------------------------------------------------------------------------

// Solver for addition operation
// Solver for the first argument: a+b == r => a == r-b
template <class A, class B> 
inline bool do_solve(addition,      A& a, const B& b, const decltype(a+b)& r)
{
    a = r - b;
    return a + b == r; // Actually will always be true, even with overflows
}

// Solver for the second argument: a+b == r => b == r-a
template <class A, class B> 
inline bool do_solve(addition,const A& a,       B& b, const decltype(a+b)& r)
{
    b = r - a;
    return a + b == r; // Actually will always be true, even with overflows
}

//------------------------------------------------------------------------------

// Solver for subtraction operation
// Solver for the first argument: a-b == r => a == r+b
template <class A, class B> 
inline bool do_solve(subtraction,      A& a, const B& b, const decltype(a-b)& r)
{
    a = r + b;
    return a - b == r; // Actually will always be true, even with overflows
}

// Solver for the second argument: a-b == r => b == a-r
template <class A, class B> 
inline bool do_solve(subtraction,const A& a,       B& b, const decltype(a-b)& r)
{
    b = a - r;
    return a - b == r; // Actually will always be true, even with overflows
}

//------------------------------------------------------------------------------

// Solver for multiplication operation
// Solver for the first argument: a*b == r => a == r/b
template <class A, class B> 
inline bool do_solve(multiplication,      A& a, const B& b, const decltype(a*b)& r)
{
    a = r/b;
    return a*b == r; // We need this as for integer division several numbers divided by b will give same result
}

// Solver for the second argument: a*b == r => b == r/a
template <class A, class B> 
inline bool do_solve(multiplication,const A& a,       B& b, const decltype(a*b)& r)
{
    b = r/a;
    return a*b == r; // We need this as for integer division several numbers divided by b will give same result
}

//------------------------------------------------------------------------------

// Solver for division operation
// Solver for the first argument: a/b == r => a == r*b
template <class A, class B> 
inline bool do_solve(division,      A& a, const B& b, const decltype(a/b)& r)
{
    a = r*b;
    return a/b == r; // We need this as for integer division several numbers divided by b will give same result
}

// Solver for the second argument: a/b == r => b == a/r
template <class A, class B> 
inline bool do_solve(division,const A& a,       B& b, const decltype(a/b)& r)
{
    b = a/r;
    return a/b == r; // We need this as for integer division several numbers divided by b will give same result
}

//------------------------------------------------------------------------------

} // of namespace mch
