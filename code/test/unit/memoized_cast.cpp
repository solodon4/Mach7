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
#include <iomanip>
#include <mach7/memoized_cast.hpp>         // Support for memoized dynamic_cast

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
