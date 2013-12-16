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
#include <iomanip>
#include "memoized_cast.hpp"

#define INHERIT_VIRTUALLY virtual

// NOTE: This doesn't compile only from MSVC IDE, if we compile it from command line, it is also accepted.
#ifdef _MSC_VER
#define NON_MSVC(...)
#else
#define NON_MSVC(...) __VA_ARGS__
#endif

template <typename T>
T byte_pattern(unsigned char c)
{
    T t;
    std::fill(reinterpret_cast<unsigned char*>(&t), reinterpret_cast<unsigned char*>(&t)+sizeof(t), c);
    return t;
}

struct A
{
    A(intptr_t a = byte_pattern<intptr_t>(0xAA)) : m_a(a) {}
    //virtual ~A() {}
    virtual int foo() { return 42; }
    intptr_t m_a;
};

struct B : INHERIT_VIRTUALLY A
{
    B(intptr_t b = byte_pattern<intptr_t>(0xBB)) : m_b(b) {}
    intptr_t m_b;
};

struct C : NON_MSVC(INHERIT_VIRTUALLY A,) B
{
    C(intptr_t c = byte_pattern<intptr_t>(0xCC)) : m_c(c) {}
    intptr_t m_c;
};

struct D : NON_MSVC(INHERIT_VIRTUALLY A,) B
{
    D(intptr_t d = byte_pattern<intptr_t>(0xDD)) : m_d(d) {}
    intptr_t m_d;
};

struct E : C, D
{
    E(intptr_t e = byte_pattern<intptr_t>(0xEE)) : m_e(e) {}
    intptr_t m_e;
};

struct F
{
    F(intptr_t f = byte_pattern<intptr_t>(0xFF)) : m_f(f) {}
    intptr_t m_f;
};

struct G : F, B
{
    G(intptr_t g = byte_pattern<intptr_t>(0x77)) : m_g(g) {}
    intptr_t m_g;
};

void dump_bytes(const void* p, size_t n)
{
    for (const unsigned char* q = static_cast<const unsigned char*>(p); n; --n,++q)
        std::cout << std::setw(2) << std::setfill('0') << std::hex << (int)*q;

    std::cout << std::endl;
}

void dump_ints(const void* p, size_t n)
{
    size_t m = n/sizeof(int);

    for (const int* q = static_cast<const int*>(p); m; --m,++q)
        std::cout << std::setw(8) << std::setfill('0') << std::hex << (int)*q << ' ';

    std::cout << std::endl;
}

template <typename T>
void dump_hex(const T& t)
{
    dump_ints(&t, sizeof(t));
}

int main()
{
          A* am = new B;
    const A* an = am;

    B* bm = memoized_cast<B*>(am); XTL_UNUSED(bm);
//    memoized_cast<B*>(an);
    const B* bn1 = memoized_cast<const B*>(am); XTL_UNUSED(bn1);
    const B* bn2 = memoized_cast<const B*>(an); XTL_UNUSED(bn2);

    const A* a = new A;
    const B* b = new B;
    const C* c = new C;
    const D* d = new D;
    const E* e = new E;
    const F* f = new F;
    const G* g = new G;

    //A* ca = (C::A*)c
    dump_hex(*a);
    dump_hex(*b);
    dump_hex(*c);
    dump_hex(*d);
    dump_hex(*e);
    dump_hex(*f);
    dump_hex(*g);

    const A* a1 = (const B*)(const C*)e;
    const E* e1 = memoized_cast<const E*>(a1); XTL_UNUSED(e1);
    const E* e2 = memoized_cast<const E*>(a1); XTL_UNUSED(e2);
    XTL_ASSERT(e1 == e && e2 == e);
}
