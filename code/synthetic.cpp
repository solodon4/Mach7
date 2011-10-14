#include <algorithm>
#include <cmath>
#include <fstream>
#include <iterator>
#include <iostream>
#include <iomanip>
#include <numeric>
#include <string>
#include <vector>
#include "match.hpp"
#include "timing.hpp"

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

DO_NOT_INLINE_BEGIN
int do_match(const Shape& s)
{
    #define FOR_EACH_N(N) if (match<shape_kind<N>>()(s)) return N;
    #include "loop_over_numbers.hpp"
    #undef  FOR_EACH_N
    //assert(!"Inexhaustive search");
    return -1;
}
DO_NOT_INLINE_END

DO_NOT_INLINE_BEGIN
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
DO_NOT_INLINE_END

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

long long display(const char* name, std::vector<long long>& timings)
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
              << std::setw(4) << microseconds(min) << " -- " 
              << std::setw(4) << microseconds(avg) << "/" 
              << std::setw(4) << microseconds(med) << " -- "
              << std::setw(4) << microseconds(max) << "] Dev = " 
              << std::setw(4) << dev << std::endl;
    return med;
}

void test_sequential()
{
    std::cout << "=================== Sequential Test ===================" << std::endl;

    for (int n = 0; n <= FOR_EACH_MAX; ++n)
    {
        Shape* s = make_shape(n);
        std::vector<long long> timingsV(M);
        std::vector<long long> timingsM(M);

        for (int m = 0; m < M; ++m)
        {
            int a1 = 0,a2 = 0;

            time_stamp liStart1 = get_time_stamp();

            for (int i = 0; i < N; ++i)
                a1 += do_visit(*s);

            time_stamp liFinish1 = get_time_stamp();

            time_stamp liStart2 = get_time_stamp();

            for (int i = 0; i < N; ++i)
                a2 += do_match(*s);

            time_stamp liFinish2 = get_time_stamp();

            assert(a1==a2);

            timingsV[m] = liFinish1-liStart1;
            timingsM[m] = liFinish2-liStart2;
        }

        long long avgV = display("AreaVisSeq", timingsV);
        long long avgM = display("AreaMatSeq", timingsM);
        //if (avgV)
            std::cout << avgM*100/avgV-100 << "% slower" << std::endl;
        //else
        //    std::cout << "Insufficient timer resolution" << std::endl;
        //std::cout << "//----------------------------------------------------------------------" << std::endl;

        delete s;
    }
}

void test_randomized()
{
    srand (get_time_stamp()/get_frequency()); // Randomize pseudo random number generator

    std::cout << "=================== Randomized Test ===================" << std::endl;

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
        int a1 = 0,a2 = 0;

        time_stamp liStart1 = get_time_stamp();

        for (int i = 0; i < N; ++i)
            a1 += do_visit(*shapes[i]);

        time_stamp liFinish1 = get_time_stamp();

        time_stamp liStart2 = get_time_stamp();

        for (int i = 0; i < N; ++i)
            a2 += do_match(*shapes[i]);

        time_stamp liFinish2 = get_time_stamp();

        assert(a1==a2);
        timingsV[m] = liFinish1-liStart1;
        timingsM[m] = liFinish2-liStart2;
    }

    long long avgV = display("AreaVisRnd", timingsV);
    long long avgM = display("AreaMatRnd", timingsM);
    //if (avgV)
        std::cout << avgM*100/avgV-100 << "% slower" << std::endl;
    //else
    //    std::cout << "Insufficient timer resolution" << std::endl;
    //std::cout << "//----------------------------------------------------------------------" << std::endl;
}

int main()
{
    test_sequential();
    test_randomized();
}

#undef  FOR_EACH_MAX
