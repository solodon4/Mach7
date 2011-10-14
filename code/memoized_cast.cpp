#include <iomanip>
#include "memoized_cast.hpp"

struct A
{
    A(int a = 0xAAAAAAAA) : m_a(a) {}
    virtual ~A() {}
    int m_a;
};

struct B : A
{
    B(int b = 0xBBBBBBBB) : m_b(b) {}
    int m_b;
};

struct C : A, B
{
    C(int c = 0xCCCCCCCC) : m_c(c) {}
    int m_c;
};

struct D : A, B
{
    D(int d = 0xDDDDDDDD) : m_d(d) {}
    int m_d;
};

struct E : C, D
{
    E(int e = 0xEEEEEEEE) : m_e(e) {}
    int m_e;
};

struct F
{
    F(int f = 0xFFFFFFFF) : m_f(f) {}
    int m_f;
};

struct G : F, B
{
    G(int g = 0x77777777) : m_g(g) {}
    int m_g;
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
        std::cout << std::setw(8) << std::setfill('0') << std::hex << (int)*q;

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

    B* bm = memoized_cast<B*>(am);
//    memoized_cast<B*>(an);
    const B* bn1 = memoized_cast<const B*>(am);
    const B* bn2 = memoized_cast<const B*>(an);

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
}
