///
/// \file
///
/// This file is a part of pattern matching testing suite.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of Mach7 library (http://parasol.tamu.edu/mach7/).
/// Copyright (C) 2011-2012 Texas A&M University.
/// All rights reserved.
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
