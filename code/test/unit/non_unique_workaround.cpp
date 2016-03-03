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
#include <mach7/type_switchN.hpp>          // Support for N-ary type switch statement

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

struct B : virtual A
{
    B(intptr_t b = byte_pattern<intptr_t>(0xBB)) : m_b(b) {}
    intptr_t m_b;
};

struct C : /*NON_MSVC(virtual A,)*/ B
{
    C(intptr_t c = byte_pattern<intptr_t>(0xCC)) : m_c(c) {}
    intptr_t m_c;
};

struct D : /*NON_MSVC(virtual A,)*/ B
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

struct G1 : G
{
    G1(intptr_t g = byte_pattern<intptr_t>(0x71)) : m_g(g) {}
    intptr_t m_g;
};

struct G2 : G
{
    G2(intptr_t g = byte_pattern<intptr_t>(0x72)) : m_g(g) {}
    intptr_t m_g;
};

struct G3 : G
{
    G3(intptr_t g = byte_pattern<intptr_t>(0x73)) : m_g(g) {}
    intptr_t m_g;
};

struct G1a : G1
{
    G1a(intptr_t g = byte_pattern<intptr_t>(0x7A)) : m_g(g) {}
    intptr_t m_g;
};

struct G1b : G1
{
    G1b(intptr_t g = byte_pattern<intptr_t>(0x7B)) : m_g(g) {}
    intptr_t m_g;
};

struct G1c : G1
{
    G1c(intptr_t g = byte_pattern<intptr_t>(0x7C)) : m_g(g) {}
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

#if defined(_MSC_VER)
/// Visual C++ does not follow the Common Vendor ABI, so the results of the
/// paper don't necessarily apply to their implementation. The library relies
/// on the following two properties, which are true about implementation 
/// following the Common Vendor API:
///  1. v-table pointer is located at offset 0 of the object
///  2. v-table pointers uniquely represent subobjects
/// Both of these properties are violated in case of G as implemented by the
/// Visual C++ (10 and 11 at least). We can see that B part of G is laid out
/// after non-polymorphic F, making its v-table pointer not be at offset 0 
/// relatively to G. Besides, the v-table pointer at offset 0 of B is not unique
/// as it is shared by G1, G2 and G3.
///
/// Mach7 calls a dedicated function vtbl_of(const void*), which can be 
/// overloaded by the user for a given sub-hierarchy to resolve both issues.
/// 
/// The overloaded function has to be declared in the library's namespace as
/// we did not want to polute the global namespace. Note that it simply forwards
/// the call to a base class, that does have v-table pointer at offset 0. Most
/// importantly note that we did not forward the call to the B base of G, that 
/// also has a v-table pointer at offset 0, but to its virtual base A. We did
/// this because B's v-table pointer does not uniquely identify subobject, while
/// A's does.
namespace mch
{
    inline std::intptr_t vtbl_of(const G* p)
    {
        return mch::vtbl_of(static_cast<const A*>(p));
    }
}
#endif

int foo(const G& g)
{
    Match(g)
    Case(G1) return 1;
    Case(G2) return 2;
    Case(G3) return 3;
    Otherwise() return 0;
    EndMatch
    return -1;
}

int bar(const G1& g1)
{
    Match(g1)
    Case(G1a) return 1;
    Case(G1b) return 2;
    Case(G1c) return 3;
    Otherwise() return 0;
    EndMatch
    return -1;
}

int main()
{
          A* am = new B;
    const A* an = am;
    const A* a = new A;
    const B* b = new B;
    const C* c = new C;
    const D* d = new D;
    const E* e = new E;
    const F* f = new F;
    const G* g = new G;
    const G1* g1 = new G1;
    const G2* g2 = new G2;
    const G3* g3 = new G3;
    const G1a* g1a = new G1a;
    const G1b* g1b = new G1b;
    const G1c* g1c = new G1c;

    //A* ca = (C::A*)c
    dump_hex(*a);
    dump_hex(*b);
    dump_hex(*c);
    dump_hex(*d);
    dump_hex(*e);
    dump_hex(*f);
    dump_hex(*g);
    dump_hex(*g1);
    dump_hex(*g2);
    dump_hex(*g3);
    dump_hex(*g1a);
    dump_hex(*g1b);
    dump_hex(*g1c);
    dump_hex(*an);

    // const A* a1 = (const B*)(const C*)e;

    std::cout << foo(*g)  << std::endl;
    std::cout << foo(*g1) << std::endl;
    std::cout << foo(*g2) << std::endl;
    std::cout << foo(*g3) << std::endl;

    std::cout << bar(*g1)  << std::endl;
    std::cout << bar(*g1a) << std::endl;
    std::cout << bar(*g1b) << std::endl;
    std::cout << bar(*g1c) << std::endl;

    b = g1;
    a = g1;

    std::cout << "Offset of B in G1: " << intptr_t(b) - intptr_t(dynamic_cast<const G1*>(b)) << std::endl;
    std::cout << "Offset of A in G1: " << intptr_t(a) - intptr_t(dynamic_cast<const G1*>(a)) << std::endl;
    std::cout << typeid(*b).name() << std::endl;
}
