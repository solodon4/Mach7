#include <algorithm>
#include <fstream>
#include <iterator>
#include <iostream>
#include <iomanip>
#include <numeric>
#include <string>
#include <vector>
#define NOMINMAX
#include <windows.h>
#include "match.hpp"

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
    #define FOR_EACH_N(N) if (match<shape_kind<N>>()(s)) return N;
    #include "loop_over_numbers.hpp"
    #undef  FOR_EACH_N
    //assert(!"Inexhaustive search");
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

const int N = 10000; // The amount of times visitor and matching procedure is invoked in one time measuring
const int M = 101;   // The amount of times time measuring is done

template <typename Container>
typename Container::value_type mean(const Container& c)
{
    return std::accumulate(c.begin(),c.end(),typename Container::value_type())/c.size();
}

template <typename Container>
typename Container::value_type deviation(const Container& c)
{
    typename Container::value_type m = mean(c);
    typename Container::value_type d = 0;

    for (typename Container::const_iterator p = c.begin(); p != c.end(); ++p)
        d += (*p-m)*(*p-m);

    return std::sqrt(double(d)/c.size());
}

long long display(const char* name, std::vector<long long>& timings, const LARGE_INTEGER& Freq)
{
    std::sort(timings.begin(), timings.end());
    std::fstream file;
   
    file.open(std::string(name)+".csv", std::fstream::out | std::fstream::app);

    if (file)
    {
        std::copy(timings.begin(), timings.end(), std::ostream_iterator<long long>(file, ", "));
        file << "End" << std::endl;
    }

    file.close();

    long long min = timings.front();
    long long max = timings.back();
    long long avg = mean(timings);
    long long med = timings[timings.size()/2];
    long long dev = deviation(timings);
    std::cout << name << " Time: ["
              << std::setw(4) << min*1000000/Freq.QuadPart << " -- " 
              << std::setw(4) << avg*1000000/Freq.QuadPart << "/" 
              << std::setw(4) << med*1000000/Freq.QuadPart << " -- "
              << std::setw(4) << max*1000000/Freq.QuadPart << "] Dev = " 
              << std::setw(4) << dev << std::endl;
    return med;
}

#include <bitset> // For print out purposes only

void test_sequential()
{
    std::cout << "=================== Sequential Test ===================" << std::endl;
    LARGE_INTEGER Freq;

    QueryPerformanceFrequency(&Freq);

    for (int n = 0; n <= FOR_EACH_MAX; ++n)
    {
        Shape* s = make_shape(n);
        std::vector<long long> timingsV(M);
        std::vector<long long> timingsM(M);

        for (int m = 0; m < M; ++m)
        {
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

            timingsV[m] = liFinish1.QuadPart-liStart1.QuadPart;
            timingsM[m] = liFinish2.QuadPart-liStart2.QuadPart;
        }

        long long avgV = display("AreaVis", timingsV, Freq);
        long long avgM = display("AreaMat", timingsM, Freq);
        std::cout << avgM*100/avgV-100 << "% slower" << std::endl;
        //std::cout << "//----------------------------------------------------------------------" << std::endl;

        delete s;
    }
}

void test_randomized()
{
    std::cout << "=================== Randomized Test ===================" << std::endl;
    LARGE_INTEGER Freq;

    QueryPerformanceFrequency(&Freq);

    std::vector<Shape*> shapes(N);

    for (int i = 0; i < N; ++i)
    {
        int n = rand()%FOR_EACH_MAX;
        shapes[i] = make_shape(n);
    }

    std::vector<long long> timingsV(M);
    std::vector<long long> timingsM(M);

    for (int m = 0; m < M; ++m)
    {
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
        timingsV[m] = liFinish1.QuadPart-liStart1.QuadPart;
        timingsM[m] = liFinish2.QuadPart-liStart2.QuadPart;
    }

    long long avgV = display("AreaVis", timingsV, Freq);
    long long avgM = display("AreaMat", timingsM, Freq);
    std::cout << avgM*100/avgV-100 << "% slower" << std::endl;
    //std::cout << "//----------------------------------------------------------------------" << std::endl;
}

int main()
{
    test_sequential();
    test_randomized();
}

#undef  FOR_EACH_MAX
