#include <assert.h>

// NOTE: This doesn't compile only from MSVC IDE, if we compile it from command line, it is also accepted.
#ifdef _MSC_VER
#define NON_MSVC(...)
#else
#define NON_MSVC(...) __VA_ARGS__
#endif

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

struct C : NON_MSVC(A,) B
{
    C(int c = 0xCCCCCCCC) : m_c(c) {}
    int m_c;
};

struct D : NON_MSVC(A,) B
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

int main()
{
    const A* a = new A;
    const B* b = new B;
    const C* c = new C;
    const D* d = new D;
    const E* e = new E;
    const F* f = new F;
    const G* g = new G;

    const A* a1 = (const B*)(const D*)e;
    const void* v1 = a1;
    const E* e1 = dynamic_cast<const E*>(v1);
    const E* e2 = dynamic_cast<const E*>(v1);
    assert(e1 == e && e2 == e);

}
