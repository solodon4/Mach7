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
/// This file is a part of Mach7 library test suite.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// \see https://parasol.tamu.edu/mach7/
/// \see https://github.com/solodon4/Mach7
/// \see https://github.com/solodon4/SELL
///

#include <iostream>

template <class T>
struct value
{
    value(const T& t) : m_value(t) {}
    T m_value;
};

template <class T, T t>
struct constant
{
};

template <class T>
struct variable {
    variable() : m_value() {}
    T m_value;
};

template <typename E1, typename E2>
struct plus
{
    plus(const E1& e1, const E2& e2) : m_e1(e1), m_e2(e2) {}
    const E1 m_e1;
    const E2 m_e2;
private: 
    plus& operator=(const plus&);       // = delete;
};

template <typename E1, typename E2>
struct minus
{
    minus(const E1& e1, const E2& e2) : m_e1(e1), m_e2(e2) {}
    const E1 m_e1;
    const E2 m_e2;
private: 
    minus& operator=(const minus&);    // = delete;
};

template <typename E1, typename E2>
struct times
{
    times(const E1& e1, const E2& e2) : m_e1(e1), m_e2(e2) {}
    const E1 m_e1;
    const E2 m_e2;
private: 
    times& operator=(const times&);   // = delete;
};

template <typename E1, typename E2>
struct divide
{
    divide(const E1& e1, const E2& e2) : m_e1(e1), m_e2(e2) {}
    const E1 m_e1;
    const E2 m_e2;
private: 
    divide& operator=(const divide&); // = delete;
};

//template <typename T>
//constexpr auto cns(const T& t) -> constant<T,t> { return constant<T,t>(); }
#define cns(a) constant<decltype(a),a>()

template <typename T>
value<T> val(const T& t) { return value<T>(t); }

template <typename E1, typename E2>   plus<E1,E2> operator+(const E1& e1, const E2& e2) { return   plus<E1,E2>(e1,e2); }
template <typename E1, typename E2>  minus<E1,E2> operator-(const E1& e1, const E2& e2) { return  minus<E1,E2>(e1,e2); }
template <typename E1, typename E2>  times<E1,E2> operator*(const E1& e1, const E2& e2) { return  times<E1,E2>(e1,e2); }
template <typename E1, typename E2> divide<E1,E2> operator/(const E1& e1, const E2& e2) { return divide<E1,E2>(e1,e2); }

template <typename T, T t> T eval(const constant<T,t>&  ) { return t; }
template <typename T>      T eval(const    value<T>&   e) { return e.m_value; }
template <typename T>      T eval(const variable<T>&   e) { return e.m_value; }
template <typename E>   auto eval(const times<constant<int,0>,E>& e) -> decltype(eval(e.m_e2)) { return (decltype(eval(e.m_e2)))(42); }
template <typename E>   auto eval(const times<E,constant<int,0>>& e) -> decltype(eval(e.m_e1)) { return (decltype(eval(e.m_e1)))(42); }
template <typename E1, typename E2> 
auto eval(const plus<E1,E2>& e) -> decltype(eval(e.m_e1) + eval(e.m_e2))
{ 
    return eval(e.m_e1) + eval(e.m_e2); 
}

int main()
{
    plus<
        times<
            value<int>,
            value<int>
        >, 
        value<int>
    > e(
        times<
            value<int>,
            value<int>
        >(
            value<int>(5),
            value<int>(10)
         ), 
        value<int>(4)
      );

    variable<int> v;

    v.m_value = 7;
    auto x = v + val(3);
    std::cout << eval(x) << std::endl;

    //auto y = v + constant<int,5>();
    auto y = v + cns(5);
    std::cout << eval(y) << std::endl;

    auto z = v * cns(0);
    std::cout << eval(z) << std::endl;

}
