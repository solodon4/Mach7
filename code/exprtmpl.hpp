#pragma once

//==============================================================================
// Functors implementing a particular operation for any pair of types
//==============================================================================

struct negation        { template <class A>          auto operator()(const A& a)             const -> decltype(-a)     { return -a; } };
struct bit_complement  { template <class A>          auto operator()(const A& a)             const -> decltype(~a)     { return ~a; } };
struct bool_complement { template <class A>          auto operator()(const A& a)             const -> decltype(!a)     { return !a; } };

struct addition        { template <class A, class B> auto operator()(const A& a, const B& b) const -> decltype(a +  b) { return a +  b; } };
struct subtraction     { template <class A, class B> auto operator()(const A& a, const B& b) const -> decltype(a -  b) { return a -  b; } };
struct multiplication  { template <class A, class B> auto operator()(const A& a, const B& b) const -> decltype(a *  b) { return a *  b; } };
struct division        { template <class A, class B> auto operator()(const A& a, const B& b) const -> decltype(a /  b) { return a /  b; } };
struct modulo          { template <class A, class B> auto operator()(const A& a, const B& b) const -> decltype(a %  b) { return a %  b; } };
struct bit_and         { template <class A, class B> auto operator()(const A& a, const B& b) const -> decltype(a &  b) { return a &  b; } };
struct bit_or          { template <class A, class B> auto operator()(const A& a, const B& b) const -> decltype(a |  b) { return a |  b; } };
struct bit_xor         { template <class A, class B> auto operator()(const A& a, const B& b) const -> decltype(a ^  b) { return a ^  b; } };
struct bit_shift_left  { template <class A, class B> auto operator()(const A& a, const B& b) const -> decltype(a << b) { return a << b; } };
struct bit_shift_right { template <class A, class B> auto operator()(const A& a, const B& b) const -> decltype(a >> b) { return a >> b; } };
struct bool_and        { template <class A, class B> auto operator()(const A& a, const B& b) const -> decltype(a && b) { return a && b; } };
struct bool_or         { template <class A, class B> auto operator()(const A& a, const B& b) const -> decltype(a || b) { return a || b; } };
struct equal           { template <class A, class B> auto operator()(const A& a, const B& b) const -> decltype(a == b) { return a == b; } };
struct not_equal       { template <class A, class B> auto operator()(const A& a, const B& b) const -> decltype(a != b) { return a != b; } };
struct greater         { template <class A, class B> auto operator()(const A& a, const B& b) const -> decltype(a >  b) { return a >  b; } };
struct greater_equal   { template <class A, class B> auto operator()(const A& a, const B& b) const -> decltype(a >= b) { return a >= b; } };
struct less            { template <class A, class B> auto operator()(const A& a, const B& b) const -> decltype(a <  b) { return a <  b; } };
struct less_equal      { template <class A, class B> auto operator()(const A& a, const B& b) const -> decltype(a <= b) { return a <= b; } };

//template <typename F, typename A, typename B>
//struct result_type
//{
//    typedef 
//};

//------------------------------------------------------------------------------

template <typename F, typename E1 = void, typename E2 = void>
struct expr;

//template <typename F>
//struct expr<F>
//{
//    template <typename U> operator U() const { return eval(*this); }
//};

template <typename F, typename E1>
struct expr<F,E1>
{
    expr(const E1& e1) : m_e1(e1) {}
    expr(E1&& e1) : m_e1(std::move(e1)) {}
    typedef decltype(F()(*static_cast<typename E1::result_type*>(0))) result_type;
    operator result_type() const { return eval(*this); }
    const E1 m_e1;
};

template <typename F, typename E1, typename E2>
struct expr
{
    expr(const E1& e1, const E2& e2) : m_e1(e1), m_e2(e2) {}
    expr(E1&& e1, E2&& e2) : m_e1(std::move(e1)), m_e2(std::move(e2)) {}
    typedef decltype(F()(*static_cast<typename E1::result_type*>(0),*static_cast<typename E2::result_type*>(0))) result_type;
    operator result_type() const { return eval(*this); }
    const E1 m_e1;
    const E2 m_e2;
};

//------------------------------------------------------------------------------

template <typename E1, typename E2>
struct expr_or
{
    expr_or(const E1& e1, const E2& e2) : m_e1(e1), m_e2(e2) {}
    expr_or(E1&& e1, E2&& e2) : m_e1(std::move(e1)), m_e2(std::move(e2)) {}
    template <typename U> bool operator()(const U* u) const { return m_e1(u) || m_e2(u); }
    template <typename U> bool operator()(      U* u) const { return m_e1(u) || m_e2(u); }
    template <typename U> bool operator()(const U& u) const { return operator()(&u); }
    template <typename U> bool operator()(      U& u) const { return operator()(&u); }
    const E1 m_e1;
    const E2 m_e2;
};

//------------------------------------------------------------------------------
