///
/// \file solvers.hpp
///
/// This file defines various patterns supported by our library.
///
/// \autor Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of the XTL framework (http://parasol.tamu.edu/xtl/).
/// Copyright (C) 2005-2011 Texas A&M University.
/// All rights reserved.
///

#pragma once

#include "patterns.hpp"

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
inline bool solve(const variable<T>& e, const S& r)
{
    return e(r);
}

//------------------------------------------------------------------------------

// Solver for variable (reference)
template <typename T, typename S>
inline bool solve(const var_ref<T>& e, const S& r)
{
    return e(r);
}

//------------------------------------------------------------------------------

// Solver for the only argument of negation: -a == r => a == -r
template <typename E1, typename S>
inline bool solve(const expr<negation,E1>& e, const S& r)
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
    typedef S source_type;               // The type a subject
    typedef typename E1::result_type target_type; // The type of a target expression

    // NOTE: The following conditions are known at compile time and we rely here
    //       on compiler eliminating dead branches. The reason we do this as 
    //       opposed making enable_if-ed overloads is to avoid the exponensial
    //       explosion of cases we'll have to add to make overloading unambiguous!
    if (std::is_unsigned<target_type>::value) 
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
    if (std::is_unsigned<typename E1::result_type>::value) 
        return e.m_e1.m_value < r && solve(e.m_e2,r-e.m_e1.m_value);
    else
        return solve(e.m_e2,r-e.m_e1.m_value);
}

//------------------------------------------------------------------------------

// Solver for the first argument of subtraction: a-b == r => a == r+b
template <typename E1, typename T, typename S>
inline bool solve(const expr<subtraction,E1,value<T>>& e, const S& r)
{
    return solve(e.m_e1,r+e.m_e2.m_value) && eval(e) == r;
}

// Solver for the second argument of subtraction: a-b == r => b == a-r
template <typename E1, typename T, typename S>
inline bool solve(const expr<subtraction,value<T>,E1>& e, const S& r)
{
    return solve(e.m_e2,e.m_e1.m_value-r) && eval(e) == r;
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
    if (std::is_integral<typename E1::result_type>::value) 
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
    if (std::is_integral<typename E1::result_type>::value) 
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
#include <complex>
template <typename T1, typename T2>
struct is_complex_and_scalar
{
    enum { value = false };
};

template <typename T>
struct is_complex_and_scalar<std::complex<T>,T>
{
    enum { value = true };
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
          >& e, 
    const S& r)
{
    typedef typename E1::result_type Scalar;
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
