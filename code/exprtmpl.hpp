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

//------------------------------------------------------------------------------

// There is intentionally no definition. If you get error here, it means there 
// is no solver for operation F.
template <typename F> struct solver;

//------------------------------------------------------------------------------

// Solver for negation operation
template <> struct solver<negation>
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
