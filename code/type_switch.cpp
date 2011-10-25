#include "type_switch.hpp"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iterator>
#include <iostream>
#include <iomanip>
#include <numeric>
#include <string>
#include <vector>

#include "timing.hpp"

#if !defined(NUMBER_OF_VFUNCS)
#define NUMBER_OF_VFUNCS  1
#endif
#if !defined(NUMBER_OF_DERIVED)
#define NUMBER_OF_DERIVED 100
#endif

struct ShapeVisitor;

struct OtherBase
{
    OtherBase() : m_foo(0xAAAAAAAA) {}
    virtual int foo() { return m_foo; };

    int m_foo;
};

struct Shape
{
    virtual ~Shape() {}
    virtual void accept(ShapeVisitor&) const = 0;
    #define FOR_EACH_MAX NUMBER_OF_VFUNCS-2
    #define FOR_EACH_N(N) virtual void foo ## N() {}
    #include "loop_over_numbers.hpp"
    #undef  FOR_EACH_N
    #undef  FOR_EACH_MAX
};

template <size_t N>
struct shape_kind : OtherBase, Shape
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
    #define FOR_EACH_MAX NUMBER_OF_DERIVED-1
    #define FOR_EACH_N(N) virtual void visit(const shape_kind<N>&) {}
    #include "loop_over_numbers.hpp"
    #undef  FOR_EACH_N
    #undef  FOR_EACH_MAX
};

template <size_t N> void shape_kind<N>::accept(ShapeVisitor& v) const { v.visit(*this); }

#if 1
XTL_DO_NOT_INLINE_BEGIN
size_t do_match(const Shape& s)
{
    TypeMatchN(s,NUMBER_OF_DERIVED)
    {
        #define FOR_EACH_MAX  NUMBER_OF_DERIVED-1
        #define FOR_EACH_N(N) TypeCase(shape_kind<N>) return N;
        #include "loop_over_numbers.hpp"
        #undef  FOR_EACH_N
        #undef  FOR_EACH_MAX
    }
    EndTypeMatch
    return -1;
}
XTL_DO_NOT_INLINE_END
#else
#error No preprocessed code yet
#endif

XTL_DO_NOT_INLINE_BEGIN
size_t do_visit(const Shape& s)
{
    struct Visitor : ShapeVisitor
    {
        #define FOR_EACH_MAX  NUMBER_OF_DERIVED-1
        #define FOR_EACH_N(N) virtual void visit(const shape_kind<N>&) { result = N; }
        #include "loop_over_numbers.hpp"
        #undef  FOR_EACH_N
        #undef  FOR_EACH_MAX
        size_t result;
    };

    Visitor v;
    v.result = -1;
    s.accept(v);
    return v.result;
}
XTL_DO_NOT_INLINE_END

Shape* make_shape(size_t i)
{
    switch (i)
    {
        #define FOR_EACH_MAX  NUMBER_OF_DERIVED-1
        #define FOR_EACH_N(N) case N: return new shape_kind<N>;
        #include "loop_over_numbers.hpp"
        #undef  FOR_EACH_N
        #undef  FOR_EACH_MAX
    }
    return 0;
}

const size_t N = 10000; // The amount of times visitor and matching procedure is invoked in one time measuring
const size_t M = 101;   // The amount of times time measuring is done
const size_t K = NUMBER_OF_DERIVED; // The amount of cases we have in hierarchy

template <typename Container>
typename Container::value_type mean(const Container& c)
{
    return std::accumulate(c.begin(),c.end(),typename Container::value_type())/c.size();
}

template <typename Container>
typename Container::value_type deviation(const Container& c)
{
    typedef typename Container::value_type value_type;
    value_type m = mean(c);
    value_type d = 0;

    for (typename Container::const_iterator p = c.begin(); p != c.end(); ++p)
        d += (*p-m)*(*p-m);

    return value_type(std::sqrt(double(d)/c.size()));
}

template <typename T>
void statistics(std::vector<T>& measurements, T& min, T& max, T& avg, T& med, T& dev)
{
    std::sort(measurements.begin(), measurements.end());
    min = measurements.front();
    max = measurements.back();
    avg = mean(measurements);
    med = measurements[measurements.size()/2];
    dev = deviation(measurements);
}

int relative_performance(long long v, long long m)
{
    if (v <= 0 || m <= 0)
    {
        std::cout << "ERROR: Insufficient timer resolution. Increase number of iterations N" << std::endl;
        exit(42);
    }
    else
    if (v <= m)
    {
        int percent = m*100/v-100;
        std::cout << "\t\t" << percent << "% slower" << std::endl;
        return +percent; // Positive number indicates how many percents slower we are 
    }
    else
    {
        int percent = v*100/m-100;
        std::cout << "\t\t" << percent << "% faster" << std::endl;
        return -percent; // Negative number indicates how many percents faster we are 
    }
}

long long display(const char* name, std::vector<long long>& timings)
{
    long long min, max, avg, med, dev;

    statistics(timings, min, max, avg, med, dev); // Get statistics from timings

    std::fstream file;
   
    file.open(std::string(name)+".csv", std::fstream::out | std::fstream::app);

    if (file)
    {
        std::copy(timings.begin(), timings.end(), std::ostream_iterator<long long>(file, ", "));
        file << "End" << std::endl;
    }

    file.close();

    std::cout << name << " Time: ["
              << std::setw(4) << microseconds(min) << " -- " 
              << std::setw(4) << microseconds(avg) << "/" 
              << std::setw(4) << microseconds(med) << " -- "
              << std::setw(4) << microseconds(max) << "] Dev = " 
              << std::setw(4) << dev << std::endl;
    return med;
}

int test_sequential()
{
    std::cout << "=================== Sequential Test ===================" << std::endl;

    size_t a1 = 0,a2 = 0;
    std::vector<int> percentages(K); // Final verdict of medians for each of the K experiments

    for (size_t n = 0; n < K; ++n)
    {
        std::vector<Shape*> shapes(N);

        for (size_t i = 0; i < N; ++i)
        {
            size_t n = i%K;
            shapes[i] = make_shape(n);
        }

        std::vector<long long> timingsV(M);
        std::vector<long long> timingsM(M);

        for (size_t m = 0; m < M; ++m)
        {
            time_stamp liStart1 = get_time_stamp();

            for (size_t i = 0; i < N; ++i)
                a1 += do_visit(*shapes[i]);

            time_stamp liFinish1 = get_time_stamp();

            time_stamp liStart2 = get_time_stamp();

            for (size_t i = 0; i < N; ++i)
                a2 += do_match(*shapes[i]);

            time_stamp liFinish2 = get_time_stamp();

            XTL_ASSERT(a1==a2);

            timingsV[m] = liFinish1-liStart1;
            timingsM[m] = liFinish2-liStart2;
        }

        long long avgV = display("AreaVisSeq", timingsV);
        long long avgM = display("AreaMatSeq", timingsM);
        percentages[n] = relative_performance(avgV, avgM);

        for (size_t i = 0; i < N; ++i)
        {
            delete shapes[i];
            shapes[i] = 0;
        }
    }

    if (a1 != a2)
    {
        std::cout << "ERROR: Invariant " << a1 << "==" << a2 << " doesn't hold." << std::endl;
        exit(42);
    }

    std::sort(percentages.begin(), percentages.end());
    return percentages[percentages.size()/2];
}

int test_randomized()
{
#if !defined(NO_RANDOMIZATION)
    srand (get_time_stamp()/get_frequency()); // Randomize pseudo random number generator
#endif
    std::cout << "=================== Randomized Test ===================" << std::endl;

    size_t a1 = 0,a2 = 0;
    std::vector<int> percentages(K); // Final verdict of medians for each of the K experiments

    for (size_t n = 0; n < K; ++n)
    {
        std::vector<Shape*> shapes(N);
        XTL_TRACE_PERFORMANCE_ONLY(std::vector<size_t> distribution(K));

        for (size_t i = 0; i < N; ++i)
        {
            size_t n = rand()%K;
            XTL_TRACE_PERFORMANCE_ONLY(distribution[n]++);
            shapes[i] = make_shape(n);
        }
#if defined(XTL_TRACE_PERFORMANCE)
        size_t min, max, avg, med, dev;
        statistics(distribution, min, max, avg, med, dev);
        //std::copy(distribution.begin(), distribution.end(), std::ostream_iterator<size_t>(std::cout, ":"));
        std::cout << "Shape kind distribution: ["
                  << std::setw(4) << min << " -- " 
                  << std::setw(4) << avg << "/" 
                  << std::setw(4) << med << " -- "
                  << std::setw(4) << max << "] Dev = " 
                  << std::setw(4) << dev << std::endl;
#endif

        std::vector<long long> timingsV(M);
        std::vector<long long> timingsM(M);

        for (size_t m = 0; m < M; ++m)
        {
            time_stamp liStart1 = get_time_stamp();

            for (size_t i = 0; i < N; ++i)
                a1 += do_visit(*shapes[i]);

            time_stamp liFinish1 = get_time_stamp();

            time_stamp liStart2 = get_time_stamp();

            for (size_t i = 0; i < N; ++i)
                a2 += do_match(*shapes[i]);

            time_stamp liFinish2 = get_time_stamp();

            XTL_ASSERT(a1==a2);
            timingsV[m] = liFinish1-liStart1;
            timingsM[m] = liFinish2-liStart2;
        }

        long long avgV = display("AreaVisRnd", timingsV);
        long long avgM = display("AreaMatRnd", timingsM);
        percentages[n] = relative_performance(avgV, avgM);

        for (size_t i = 0; i < N; ++i)
        {
            delete shapes[i];
            shapes[i] = 0;
        }
    }

    if (a1 != a2)
    {
        std::cout << "ERROR: Invariant " << a1 << "==" << a2 << " doesn't hold." << std::endl;
        exit(42);
    }

    std::sort(percentages.begin(), percentages.end());
    return percentages[percentages.size()/2];
}

int main()
{
    int ps = test_sequential();
    int pr = test_randomized();
    std::cout << "OVERALL: Sequential: " 
              << abs(ps) << (ps >= 0 ? "% slower" : "% faster") << "; Random: " 
              << abs(pr) << (pr >= 0 ? "% slower" : "% faster") << std::endl; 
}
