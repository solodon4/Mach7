#include <iostream>
#include <vector>
#include <windows.h>
#include "match_select.hpp"

#define FOR_EACH_MAX 99

struct ShapeVisitor;

struct Shape
{
    virtual void accept(ShapeVisitor&) const = 0;
};

template <int N>
struct shape_kind : Shape
{
    void accept(ShapeVisitor& v) const;
    int m_member0;
    int m_member1;
    int m_member2;
    int m_member3;
    int m_member4;
    int m_member5;
    int m_member6;
    int m_member7;
    int m_member8;
    int m_member9;
};

struct ShapeVisitor
{
    #define FOR_EACH_N(N) virtual void visit(const shape_kind<N>&) {}
    #include "loop_over_numbers.hpp"
    #undef  FOR_EACH_N
};

template <int N> void shape_kind<N>::accept(ShapeVisitor& v) const { v.visit(*this); }

int do_match(const Shape& s)
{
    switch (on(__LINE__,s))
    {
    default:
    #define FOR_EACH_N(N) case __LINE__: if (match<shape_kind<N>>(__LINE__)(s)) return N;
    #include "loop_over_numbers.hpp"
    #undef  FOR_EACH_N
    }
    assert(!"Inexhaustive search");
    return -1;
}

int do_visit(const Shape& s)
{
    struct Visitor : ShapeVisitor
    {
        #define FOR_EACH_N(N) virtual void visit(const shape_kind<N>&) { result = N; }
        #include "loop_over_numbers.hpp"
        #undef  FOR_EACH_N
        int result;
    };

    Visitor v;
    v.result = -1;
    s.accept(v);
    return v.result;
}

Shape* make_shape(int i)
{
    switch (i)
    {
    #define FOR_EACH_N(N) case N: return new shape_kind<N>;
    #include "loop_over_numbers.hpp"
    #undef  FOR_EACH_N
    }
    return 0;
}

void test_sequential()
{
    std::cout << "=================== Sequential Test ===================" << std::endl;
    LARGE_INTEGER Freq;

    QueryPerformanceFrequency(&Freq);

    const int N = 10000;

    for (int n = 0; n <= FOR_EACH_MAX; ++n)
    {
        Shape* s = make_shape(n);

        LARGE_INTEGER liStart1, liFinish1, liStart2, liFinish2;
        int a1 = 0,a2 = 0;

        QueryPerformanceCounter(&liStart1);

        for (int i = 0; i < N; ++i)
            a1 += do_visit(*s);

        QueryPerformanceCounter(&liFinish1);

        QueryPerformanceCounter(&liStart2);

        for (int i = 0; i < N; ++i)
            a2 += do_match(*s);

        QueryPerformanceCounter(&liFinish2);

        assert(a1==a2);

        std::cout << "AreaV Time:" << (liFinish1.QuadPart-liStart1.QuadPart)*1000000/Freq.QuadPart << std::endl;
        std::cout << "AreaM Time:" << (liFinish2.QuadPart-liStart2.QuadPart)*1000000/Freq.QuadPart << std::endl;
        std::cout << (liFinish2.QuadPart-liStart2.QuadPart)*100/(liFinish1.QuadPart-liStart1.QuadPart)-100 << "% slower" << std::endl;

        delete s;
    }
}

void test_randomized()
{
    std::cout << "=================== Randomized Test ===================" << std::endl;
    LARGE_INTEGER Freq;

    QueryPerformanceFrequency(&Freq);

    const int N = 10000;
    std::vector<Shape*> shapes(N);

    for (int i = 0; i < N; ++i)
    {
        int n = rand()%FOR_EACH_MAX;
        shapes[i] = make_shape(n);
    }

    LARGE_INTEGER liStart1, liFinish1, liStart2, liFinish2;
    int a1 = 0,a2 = 0;

    QueryPerformanceCounter(&liStart1);

    for (int i = 0; i < N; ++i)
        a1 += do_visit(*shapes[i]);

    QueryPerformanceCounter(&liFinish1);

    QueryPerformanceCounter(&liStart2);

    for (int i = 0; i < N; ++i)
        a2 += do_match(*shapes[i]);

    QueryPerformanceCounter(&liFinish2);

    assert(a1==a2);

    std::cout << "AreaV Time:" << (liFinish1.QuadPart-liStart1.QuadPart)*1000000/Freq.QuadPart << std::endl;
    std::cout << "AreaM Time:" << (liFinish2.QuadPart-liStart2.QuadPart)*1000000/Freq.QuadPart << std::endl;
    std::cout << (liFinish2.QuadPart-liStart2.QuadPart)*100/(liFinish1.QuadPart-liStart1.QuadPart)-100 << "% slower" << std::endl;
}

int main()
{
    test_sequential();
    test_randomized();
}

#undef  FOR_EACH_MAX
