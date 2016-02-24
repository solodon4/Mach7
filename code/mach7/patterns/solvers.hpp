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
/// This file defines various patterns supported by our library.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// \see https://parasol.tamu.edu/mach7/
/// \see https://github.com/solodon4/Mach7
/// \see https://github.com/solodon4/SELL
///

#pragma once

#include <complex>
#include "equivalence.hpp"

namespace mch ///< Mach7 library namespace
{

//------------------------------------------------------------------------------

// Solver for value
template <typename T, typename S>
inline bool solve(const value<T>& e, const S& r)
{
    return e(r);
}

//------------------------------------------------------------------------------

// Solver for variable
template <typename T, typename S>
inline bool solve(const var<T>& e, const S& r)
{
    return e(r);
}

//------------------------------------------------------------------------------

// Solver for variable (reference)
template <typename T, typename S>
inline bool solve(const ref2<T>& e, const S& r)
{
    return e(r);
}

//------------------------------------------------------------------------------

// Solver for the only argument of unary_minus: -a == r => a == -r
template <typename E1, typename S>
inline bool solve(const expr<unary_minus,E1>& e, const S& r)
{
    return solve(e.m_e1,-r);
}

//------------------------------------------------------------------------------

// Solver for the only argument of bit_complement: ~a == r => a == ~r
template <typename E1, typename S>
inline bool solve(const expr<bit_complement,E1>& e, const S& r)
{
    return solve(e.m_e1,~r);
}

//------------------------------------------------------------------------------

// Solver for the only argument of bool_complement: !a == r => a == !r
template <typename E1, typename S>
inline bool solve(const expr<bool_complement,E1>& e, const S& r)
{
    return solve(e.m_e1,!r);
}

//------------------------------------------------------------------------------

// Solver for the first argument of addition: a+b == r => a == r-b
template <typename E1, typename T, typename S>
inline bool solve(const expr<addition,E1,value<T>>& e, const S& r)
{
    //typedef S source_type;               // The type a subject
    typedef typename E1::result_type target_type; // The type of a target expression

    // NOTE: The following conditions are known at compile time and we rely here
    //       on compiler eliminating dead branches. The reason we do this as 
    //       opposed making enable_if-ed overloads is to avoid the exponensial
    //       explosion of cases we'll have to add to make overloading unambiguous!
    // FIX:  Move first condition of && into if and make both solve calls in else 
    // FIX:  Why is condition strict v < r? Shouldn't it be v <= r?
    XTL_STATIC_IF(std::is_unsigned<target_type>::value) 
        return e.m_e2.m_value < r && solve(e.m_e1,r-e.m_e2.m_value);
    else
        return solve(e.m_e1,r-e.m_e2.m_value);
}

// Solver for the second argument of addition: a+b == r => b == r-a
template <typename E1, typename T, typename S>
inline bool solve(const expr<addition,value<T>,E1>& e, const S& r)
{
    // NOTE: The following conditions are known at compile time and we rely here
    //       on compiler eliminating dead branches. The reason we do this as 
    //       opposed making enable_if-ed overloads is to avoid the exponensial
    //       explosion of cases we'll have to add to make overloading unambiguous!
    // FIX:  Move first condition of && into if and make both solve calls in else 
    // FIX:  Why is condition strict v < r? Shouldn't it be v <= r?
    XTL_STATIC_IF(std::is_unsigned<typename E1::result_type>::value) 
        return e.m_e1.m_value < r && solve(e.m_e2,r-e.m_e1.m_value);
    else
        return solve(e.m_e2,r-e.m_e1.m_value);
}

//------------------------------------------------------------------------------

// Solver for the first argument of addition: a+b == r => a == r-b
template <typename E1, typename E2, typename S>
inline bool solve(const expr<addition,E1,equivalence<E2>>& e, const S& r)
{
    //typedef S source_type;               // The type a subject
    typedef typename E1::result_type target_type; // The type of a target expression

    // NOTE: The following conditions are known at compile time and we rely here
    //       on compiler eliminating dead branches. The reason we do this as 
    //       opposed making enable_if-ed overloads is to avoid the exponensial
    //       explosion of cases we'll have to add to make overloading unambiguous!
    // FIX:  Move first condition of && into if and make both solve calls in else 
    // FIX:  Why is condition strict v < r? Shouldn't it be v <= r?
    auto v = eval(e.m_e2);
    XTL_STATIC_IF(std::is_unsigned<target_type>::value) 
        return v < r && solve(e.m_e1,r-v);
    else
        return solve(e.m_e1,r-v);
}

// Solver for the second argument of addition: a+b == r => b == r-a
template <typename E1, typename E2, typename S>
inline bool solve(const expr<addition,equivalence<E1>,E2>& e, const S& r)
{
    // NOTE: The following conditions are known at compile time and we rely here
    //       on compiler eliminating dead branches. The reason we do this as 
    //       opposed making enable_if-ed overloads is to avoid the exponensial
    //       explosion of cases we'll have to add to make overloading unambiguous!
    // FIX:  Move first condition of && into if and make both solve calls in else 
    // FIX:  Why is condition strict v < r? Shouldn't it be v <= r?
    auto v = eval(e.m_e1);
    XTL_STATIC_IF(std::is_unsigned<typename E2::result_type>::value) 
        return v < r && solve(e.m_e2,r-v);
    else
        return solve(e.m_e2,r-v);
}

//------------------------------------------------------------------------------

// Solver for the first argument of subtraction: a-b == r => a == r+b
template <typename E1, typename T, typename S>
inline bool solve(const expr<subtraction,E1,value<T>>& e, const S& r)
{
    // FIX:  Shouldn't there be a similar is_unsigned check?
    return solve(e.m_e1,r+e.m_e2.m_value) && eval(e) == r;
}

// Solver for the second argument of subtraction: a-b == r => b == a-r
template <typename E1, typename T, typename S>
inline bool solve(const expr<subtraction,value<T>,E1>& e, const S& r)
{
    // FIX:  Shouldn't there be a similar is_unsigned check?
    return solve(e.m_e2,e.m_e1.m_value-r) && eval(e) == r;
}

//------------------------------------------------------------------------------

// Solver for the first argument of subtraction: a-b == r => a == r+b
template <typename E1, typename E2, typename S>
inline bool solve(const expr<subtraction,E1,equivalence<E2>>& e, const S& r)
{
    // FIX:  Shouldn't there be a similar is_unsigned check?
    return solve(e.m_e1,r+eval(e.m_e2)) && eval(e) == r;
}

// Solver for the second argument of subtraction: a-b == r => b == a-r
template <typename E1, typename E2, typename S>
inline bool solve(const expr<subtraction,equivalence<E1>,E2>& e, const S& r)
{
    // FIX:  Shouldn't there be a similar is_unsigned check?
    return solve(e.m_e2,eval(e.m_e1)-r) && eval(e) == r;
}

//------------------------------------------------------------------------------

// Solver for the first argument of multiplication: a*b == r => a == r/b
template <typename E1, typename T, typename S>
inline bool solve(const expr<multiplication,E1,value<T>>& e, const S& r)
{
    // NOTE: The following conditions are known at compile time and we rely here
    //       on compiler eliminating dead branches. The reason we do this as 
    //       opposed making enable_if-ed overloads is to avoid the exponensial
    //       explosion of cases we'll have to add to make overloading unambiguous!
    XTL_STATIC_IF(std::is_integral<typename E1::result_type>::value) 
        //return r % e.m_e2.m_value == 0 ? solve(e.m_e1,r/e.m_e2.m_value) : false;
        return solve(e.m_e1,r/e.m_e2.m_value) && eval(e) == r; // NOTE: Somehow this one is faster than the above commented out one
    else
        return solve(e.m_e1,r/e.m_e2.m_value);
}

// Solver for the second argument of multiplication: a*b == r => b == r/a
template <typename E1, typename T, typename S>
inline bool solve(const expr<multiplication,value<T>,E1>& e, const S& r)
{
    // NOTE: The following conditions are known at compile time and we rely here
    //       on compiler eliminating dead branches. The reason we do this as 
    //       opposed making enable_if-ed overloads is to avoid the exponensial
    //       explosion of cases we'll have to add to make overloading unambiguous!
    XTL_STATIC_IF(std::is_integral<typename E1::result_type>::value) 
        //return r % e.m_e1.m_value == 0 ? solve(e.m_e2,r/e.m_e1.m_value) : false;
        return solve(e.m_e2,r/e.m_e1.m_value) && eval(e) == r; // NOTE: Somehow this one is faster than the above commented out one
    else
        return solve(e.m_e2,r/e.m_e1.m_value);
}

//------------------------------------------------------------------------------

// Solver for the first argument of division: a/b == r => a == r*b
template <typename E1, typename T, typename S>
inline bool solve(const expr<division,E1,value<T>>& e, const S& r)
{
    return solve(e.m_e1,r*e.m_e2.m_value) && eval(e) == r;
}

// Solver for the second argument of division: a/b == r => b == a/r
template <typename E1, typename T, typename S>
inline bool solve(const expr<division,value<T>,E1>& e, const S& r)
{
    return solve(e.m_e2,e.m_e1.m_value/r) && eval(e) == r;
}

//------------------------------------------------------------------------------

template <typename T1, typename T2>
struct is_complex_and_scalar
{
    static const bool value = false;
};

template <typename T>
struct is_complex_and_scalar<std::complex<T>,T>
{
    static const bool value = true;
};
/*
// Solver for algebraic decomposition of complex
template <typename E1, typename E2, typename S>
inline typename std::enable_if< is_complex_and_scalar<typename E1::result_type,typename E2::result_type>::value, bool>::type
solve(const expr<addition,E1,E2>& e, const S& r)
{
    typedef typename E2::result_type Scalar;
    Scalar sss;
    typename E1::result_type ttt;
    return true; //solve(e.m_e1,std::complex<Scalar>(0,1));
}

template <typename E1, typename T, typename S>
inline typename std::enable_if< is_complex_and_scalar<T,typename E1::result_type>::value, bool>::type
solve(const expr<multiplication,E1,value<T>>& e, const S& r)
{
    typedef typename E1::result_type Scalar;
    return true; //solve(e.m_e1,std::complex<Scalar>(0,1));
}
*/
template <typename E1, typename E2, typename S>
inline bool solve(
    const expr<addition,
               expr<multiplication,
                    E1,
                    value<std::complex<typename E1::result_type>>
               >,
               E2
          >& /*e*/, 
    const S& /*r*/)
{
    //typedef typename E1::result_type Scalar;
    // x + y*C = m + n*i
    // y = n/C.imag();
    // x = m - C.real()*y
    //const std::complex<Scalar>& c = e.m_e1.m_e2.m_value;
    //Scalar y = r.imag()/c.imag();
    //Scalar x = r.real() - c.real()*y;
    //return solve(e.m_e1,x) && solve(e.m_e2,y);
    return false;
}


// Solver for algebraic decomposition of complex
//template <typename E1, typename E2, typename S>
//inline typename std::enable_if< is_complex_and_scalar<typename E2::result_type,typename E1::result_type>::value, bool>::type
//solve(const expr<multiplication,E1,E2>& e, const S& r)
//{
//    typedef typename E1::result_type Scalar;
//    Scalar sss;
//    typename E2::result_type ttt;
//    return true; //solve(e.m_e1,std::complex<Scalar>(0,1));
//}

//------------------------------------------------------------------------------

} // of namespace mch
