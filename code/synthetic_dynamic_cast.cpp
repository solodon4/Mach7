#include <algorithm>
#include <cmath>
#include <fstream>
#include <iterator>
#include <iostream>
#include <iomanip>
#include <numeric>
#include <string>
#include <vector>
#include "match_generic.hpp"
    #define NOMINMAX
    #include <windows.h>
//#define XTL_TIMING_METHOD_2
#include "timing.hpp"

//------------------------------------------------------------------------------

#if !defined(NUMBER_OF_VFUNCS)
#define NUMBER_OF_VFUNCS  1
#endif
#if !defined(NUMBER_OF_DERIVED)
#define NUMBER_OF_DERIVED 40
#endif

//------------------------------------------------------------------------------

struct ShapeVisitor;

//------------------------------------------------------------------------------

struct OtherBase
{
    OtherBase() : m_foo(0xAAAAAAAA) {}
    virtual int foo() { return m_foo; };

    int m_foo;
};

//------------------------------------------------------------------------------

static const size_t primes[41] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101,103,107,109,113,127,131,137,139,149,151,157,163,167,173,179};

struct Shape
{
    Shape(size_t n) :
        m_fdc_id(fdc_id(n)),
        m_member0(n+0),
        m_member1(n+1),
        m_member2(n+2),
        m_member3(n+3),
        m_member4(n+4),
        m_member5(n+5),
        m_member6(n+6),
        m_member7(n+7),
        m_member8(n+8),
        m_member9(n+9)
    {}

    virtual ~Shape() {}
    virtual void accept(ShapeVisitor&) const = 0;
    #define FOR_EACH_MAX NUMBER_OF_VFUNCS-2
    #define FOR_EACH_N(N) virtual void foo ## N() {}
    #include "loop_over_numbers.hpp"
    #undef  FOR_EACH_N
    #undef  FOR_EACH_MAX

    static size_t fdc_id(size_t n)
    {
        return 2*primes[n+1];
    }

    size_t m_fdc_id; // Fast dynamic cast ID

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

//------------------------------------------------------------------------------

template <size_t N>
struct shape_kind : OtherBase, Shape
{
    typedef Shape base_class;
    shape_kind(size_t n = N) : base_class(n) {}
    void accept(ShapeVisitor&) const;
};

//------------------------------------------------------------------------------
inline size_t id(size_t n) { return Shape::fdc_id(n); }
const size_t shape_ids[100] =
{
     id( 0), id( 1), id( 2), id( 3), id( 4), id( 5), id( 6), id( 7), id( 8), id( 9),
     id(10), id(11), id(12), id(13), id(14), id(15), id(16), id(17), id(18), id(19),
     id(20), id(21), id(22), id(23), id(24), id(25), id(26), id(27), id(28), id(29),
     id(30), id(31), id(32), id(33), id(34), id(35), id(36), id(37), id(38), id(39),
     //id(40), id(41), id(42), id(43), id(44), id(45), id(46), id(47), id(48), id(49),
     //id(50), id(51), id(52), id(53), id(54), id(55), id(56), id(57), id(58), id(59),
     //id(60), id(61), id(62), id(63), id(64), id(65), id(66), id(67), id(68), id(69),
     //id(70), id(71), id(72), id(73), id(74), id(75), id(76), id(77), id(78), id(79),
     //id(80), id(81), id(82), id(83), id(84), id(85), id(86), id(87), id(88), id(89),
     //id(90), id(91), id(92), id(93), id(94), id(95), id(96), id(97), id(98), id(99),
};

template <size_t N>
inline const shape_kind<N>* fast_dynamic_cast_ex(const shape_kind<N>*, const Shape* u)
{
    return u->m_fdc_id % shape_ids[N] == 0
                ? static_cast<const shape_kind<N>*>(u) 
                : 0;
}

template <typename T>
inline T fast_dynamic_cast(const Shape* u)
{
    return fast_dynamic_cast_ex(static_cast<T>(0), u);
}

#define dynamic_cast fast_dynamic_cast

//------------------------------------------------------------------------------

struct ShapeVisitor
{
    #define FOR_EACH_MAX NUMBER_OF_DERIVED-1
    #define FOR_EACH_N(N) virtual void visit(const shape_kind<N>&) {}
    #include "loop_over_numbers.hpp"
    #undef  FOR_EACH_N
    #undef  FOR_EACH_MAX
};

template <size_t N> void shape_kind<N>::accept(ShapeVisitor& v) const { v.visit(*this); }

//------------------------------------------------------------------------------

#if 0
XTL_DO_NOT_INLINE_BEGIN
size_t do_match(const Shape& s, size_t n)
{
    #define FOR_EACH_MAX  NUMBER_OF_DERIVED-1
    #define FOR_EACH_N(N) if (const shape_kind<N>* p = dynamic_cast<const shape_kind<N>*>(&s)) return N/*+p->m_member1*/;
    #include "loop_over_numbers.hpp"
    #undef  FOR_EACH_N
    #undef  FOR_EACH_MAX
    return -1;
}
XTL_DO_NOT_INLINE_END
#else
XTL_DO_NOT_INLINE_BEGIN
size_t do_match(const Shape& s, size_t n)
{
    if (const shape_kind< 0>* p = dynamic_cast<const shape_kind< 0>*>(&s)) return  0 ;
    if (const shape_kind< 1>* p = dynamic_cast<const shape_kind< 1>*>(&s)) return  1 ;
    if (const shape_kind< 2>* p = dynamic_cast<const shape_kind< 2>*>(&s)) return  2 ;
    if (const shape_kind< 3>* p = dynamic_cast<const shape_kind< 3>*>(&s)) return  3 ;
    if (const shape_kind< 4>* p = dynamic_cast<const shape_kind< 4>*>(&s)) return  4 ;
    if (const shape_kind< 5>* p = dynamic_cast<const shape_kind< 5>*>(&s)) return  5 ;
    if (const shape_kind< 6>* p = dynamic_cast<const shape_kind< 6>*>(&s)) return  6 ;
    if (const shape_kind< 7>* p = dynamic_cast<const shape_kind< 7>*>(&s)) return  7 ;
    if (const shape_kind< 8>* p = dynamic_cast<const shape_kind< 8>*>(&s)) return  8 ;
    if (const shape_kind< 9>* p = dynamic_cast<const shape_kind< 9>*>(&s)) return  9 ;
    if (const shape_kind<10>* p = dynamic_cast<const shape_kind<10>*>(&s)) return 10 ;
    if (const shape_kind<11>* p = dynamic_cast<const shape_kind<11>*>(&s)) return 11 ;
    if (const shape_kind<12>* p = dynamic_cast<const shape_kind<12>*>(&s)) return 12 ;
    if (const shape_kind<13>* p = dynamic_cast<const shape_kind<13>*>(&s)) return 13 ;
    if (const shape_kind<14>* p = dynamic_cast<const shape_kind<14>*>(&s)) return 14 ;
    if (const shape_kind<15>* p = dynamic_cast<const shape_kind<15>*>(&s)) return 15 ;
    if (const shape_kind<16>* p = dynamic_cast<const shape_kind<16>*>(&s)) return 16 ;
    if (const shape_kind<17>* p = dynamic_cast<const shape_kind<17>*>(&s)) return 17 ;
    if (const shape_kind<18>* p = dynamic_cast<const shape_kind<18>*>(&s)) return 18 ;
    if (const shape_kind<19>* p = dynamic_cast<const shape_kind<19>*>(&s)) return 19 ;
    if (const shape_kind<20>* p = dynamic_cast<const shape_kind<20>*>(&s)) return 20 ;
    if (const shape_kind<21>* p = dynamic_cast<const shape_kind<21>*>(&s)) return 21 ;
    if (const shape_kind<22>* p = dynamic_cast<const shape_kind<22>*>(&s)) return 22 ;
    if (const shape_kind<23>* p = dynamic_cast<const shape_kind<23>*>(&s)) return 23 ;
    if (const shape_kind<24>* p = dynamic_cast<const shape_kind<24>*>(&s)) return 24 ;
    if (const shape_kind<25>* p = dynamic_cast<const shape_kind<25>*>(&s)) return 25 ;
    if (const shape_kind<26>* p = dynamic_cast<const shape_kind<26>*>(&s)) return 26 ;
    if (const shape_kind<27>* p = dynamic_cast<const shape_kind<27>*>(&s)) return 27 ;
    if (const shape_kind<28>* p = dynamic_cast<const shape_kind<28>*>(&s)) return 28 ;
    if (const shape_kind<29>* p = dynamic_cast<const shape_kind<29>*>(&s)) return 29 ;
    if (const shape_kind<30>* p = dynamic_cast<const shape_kind<30>*>(&s)) return 30 ;
    if (const shape_kind<31>* p = dynamic_cast<const shape_kind<31>*>(&s)) return 31 ;
    if (const shape_kind<32>* p = dynamic_cast<const shape_kind<32>*>(&s)) return 32 ;
    if (const shape_kind<33>* p = dynamic_cast<const shape_kind<33>*>(&s)) return 33 ;
    if (const shape_kind<34>* p = dynamic_cast<const shape_kind<34>*>(&s)) return 34 ;
    if (const shape_kind<35>* p = dynamic_cast<const shape_kind<35>*>(&s)) return 35 ;
    if (const shape_kind<36>* p = dynamic_cast<const shape_kind<36>*>(&s)) return 36 ;
    if (const shape_kind<37>* p = dynamic_cast<const shape_kind<37>*>(&s)) return 37 ;
    if (const shape_kind<38>* p = dynamic_cast<const shape_kind<38>*>(&s)) return 38 ;
    if (const shape_kind<39>* p = dynamic_cast<const shape_kind<39>*>(&s)) return 39 ;
    //if (const shape_kind<40>* p = dynamic_cast<const shape_kind<40>*>(&s)) return 40 ;
    //if (const shape_kind<41>* p = dynamic_cast<const shape_kind<41>*>(&s)) return 41 ;
    //if (const shape_kind<42>* p = dynamic_cast<const shape_kind<42>*>(&s)) return 42 ;
    //if (const shape_kind<43>* p = dynamic_cast<const shape_kind<43>*>(&s)) return 43 ;
    //if (const shape_kind<44>* p = dynamic_cast<const shape_kind<44>*>(&s)) return 44 ;
    //if (const shape_kind<45>* p = dynamic_cast<const shape_kind<45>*>(&s)) return 45 ;
    //if (const shape_kind<46>* p = dynamic_cast<const shape_kind<46>*>(&s)) return 46 ;
    //if (const shape_kind<47>* p = dynamic_cast<const shape_kind<47>*>(&s)) return 47 ;
    //if (const shape_kind<48>* p = dynamic_cast<const shape_kind<48>*>(&s)) return 48 ;
    //if (const shape_kind<49>* p = dynamic_cast<const shape_kind<49>*>(&s)) return 49 ;
    //if (const shape_kind<50>* p = dynamic_cast<const shape_kind<50>*>(&s)) return 50 ;
    //if (const shape_kind<51>* p = dynamic_cast<const shape_kind<51>*>(&s)) return 51 ;
    //if (const shape_kind<52>* p = dynamic_cast<const shape_kind<52>*>(&s)) return 52 ;
    //if (const shape_kind<53>* p = dynamic_cast<const shape_kind<53>*>(&s)) return 53 ;
    //if (const shape_kind<54>* p = dynamic_cast<const shape_kind<54>*>(&s)) return 54 ;
    //if (const shape_kind<55>* p = dynamic_cast<const shape_kind<55>*>(&s)) return 55 ;
    //if (const shape_kind<56>* p = dynamic_cast<const shape_kind<56>*>(&s)) return 56 ;
    //if (const shape_kind<57>* p = dynamic_cast<const shape_kind<57>*>(&s)) return 57 ;
    //if (const shape_kind<58>* p = dynamic_cast<const shape_kind<58>*>(&s)) return 58 ;
    //if (const shape_kind<59>* p = dynamic_cast<const shape_kind<59>*>(&s)) return 59 ;
    //if (const shape_kind<60>* p = dynamic_cast<const shape_kind<60>*>(&s)) return 60 ;
    //if (const shape_kind<61>* p = dynamic_cast<const shape_kind<61>*>(&s)) return 61 ;
    //if (const shape_kind<62>* p = dynamic_cast<const shape_kind<62>*>(&s)) return 62 ;
    //if (const shape_kind<63>* p = dynamic_cast<const shape_kind<63>*>(&s)) return 63 ;
    //if (const shape_kind<64>* p = dynamic_cast<const shape_kind<64>*>(&s)) return 64 ;
    //if (const shape_kind<65>* p = dynamic_cast<const shape_kind<65>*>(&s)) return 65 ;
    //if (const shape_kind<66>* p = dynamic_cast<const shape_kind<66>*>(&s)) return 66 ;
    //if (const shape_kind<67>* p = dynamic_cast<const shape_kind<67>*>(&s)) return 67 ;
    //if (const shape_kind<68>* p = dynamic_cast<const shape_kind<68>*>(&s)) return 68 ;
    //if (const shape_kind<69>* p = dynamic_cast<const shape_kind<69>*>(&s)) return 69 ;
    //if (const shape_kind<70>* p = dynamic_cast<const shape_kind<70>*>(&s)) return 70 ;
    //if (const shape_kind<71>* p = dynamic_cast<const shape_kind<71>*>(&s)) return 71 ;
    //if (const shape_kind<72>* p = dynamic_cast<const shape_kind<72>*>(&s)) return 72 ;
    //if (const shape_kind<73>* p = dynamic_cast<const shape_kind<73>*>(&s)) return 73 ;
    //if (const shape_kind<74>* p = dynamic_cast<const shape_kind<74>*>(&s)) return 74 ;
    //if (const shape_kind<75>* p = dynamic_cast<const shape_kind<75>*>(&s)) return 75 ;
    //if (const shape_kind<76>* p = dynamic_cast<const shape_kind<76>*>(&s)) return 76 ;
    //if (const shape_kind<77>* p = dynamic_cast<const shape_kind<77>*>(&s)) return 77 ;
    //if (const shape_kind<78>* p = dynamic_cast<const shape_kind<78>*>(&s)) return 78 ;
    //if (const shape_kind<79>* p = dynamic_cast<const shape_kind<79>*>(&s)) return 79 ;
    //if (const shape_kind<80>* p = dynamic_cast<const shape_kind<80>*>(&s)) return 80 ;
    //if (const shape_kind<81>* p = dynamic_cast<const shape_kind<81>*>(&s)) return 81 ;
    //if (const shape_kind<82>* p = dynamic_cast<const shape_kind<82>*>(&s)) return 82 ;
    //if (const shape_kind<83>* p = dynamic_cast<const shape_kind<83>*>(&s)) return 83 ;
    //if (const shape_kind<84>* p = dynamic_cast<const shape_kind<84>*>(&s)) return 84 ;
    //if (const shape_kind<85>* p = dynamic_cast<const shape_kind<85>*>(&s)) return 85 ;
    //if (const shape_kind<86>* p = dynamic_cast<const shape_kind<86>*>(&s)) return 86 ;
    //if (const shape_kind<87>* p = dynamic_cast<const shape_kind<87>*>(&s)) return 87 ;
    //if (const shape_kind<88>* p = dynamic_cast<const shape_kind<88>*>(&s)) return 88 ;
    //if (const shape_kind<89>* p = dynamic_cast<const shape_kind<89>*>(&s)) return 89 ;
    //if (const shape_kind<90>* p = dynamic_cast<const shape_kind<90>*>(&s)) return 90 ;
    //if (const shape_kind<91>* p = dynamic_cast<const shape_kind<91>*>(&s)) return 91 ;
    //if (const shape_kind<92>* p = dynamic_cast<const shape_kind<92>*>(&s)) return 92 ;
    //if (const shape_kind<93>* p = dynamic_cast<const shape_kind<93>*>(&s)) return 93 ;
    //if (const shape_kind<94>* p = dynamic_cast<const shape_kind<94>*>(&s)) return 94 ;
    //if (const shape_kind<95>* p = dynamic_cast<const shape_kind<95>*>(&s)) return 95 ;
    //if (const shape_kind<96>* p = dynamic_cast<const shape_kind<96>*>(&s)) return 96 ;
    //if (const shape_kind<97>* p = dynamic_cast<const shape_kind<97>*>(&s)) return 97 ;
    //if (const shape_kind<98>* p = dynamic_cast<const shape_kind<98>*>(&s)) return 98 ;
    //if (const shape_kind<99>* p = dynamic_cast<const shape_kind<99>*>(&s)) return 99 ;
    return -1;
}
XTL_DO_NOT_INLINE_END
#endif

//------------------------------------------------------------------------------

XTL_DO_NOT_INLINE_BEGIN
size_t do_visit(const Shape& s, size_t n)
{
    struct Visitor : ShapeVisitor
    {
        #define FOR_EACH_MAX  NUMBER_OF_DERIVED-1
        #define FOR_EACH_N(N) virtual void visit(const shape_kind<N>& s) { result = N/*+s.m_member1*/; }
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

//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------

const size_t N = 10000; // The amount of times visitor and matching procedure is invoked in one time measuring
const size_t M = 101;   // The amount of times time measuring is done
const size_t K = NUMBER_OF_DERIVED; // The amount of cases we have in hierarchy

//------------------------------------------------------------------------------

template <typename Container>
typename Container::value_type mean(const Container& c)
{
    return std::accumulate(c.begin(),c.end(),typename Container::value_type())/c.size();
}

//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------

int relative_performance(long long v, long long m)
{
    if (XTL_UNLIKELY(v <= 0 || m <= 0))
    {
        std::cout << "ERROR: Insufficient timer resolution. Increase number of iterations N" << std::endl;
        exit(42);
    }
    else
    if (XTL_UNLIKELY(v <= m))
    {
        int percent = int(m*100/v-100);
        std::cout << "\t\t" << percent << "% slower" << std::endl;
        return +percent; // Positive number indicates how many percents slower we are 
    }
    else
    {
        int percent = int(v*100/m-100);
        std::cout << "\t\t" << percent << "% faster" << std::endl;
        return -percent; // Negative number indicates how many percents faster we are 
    }
}

//------------------------------------------------------------------------------

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
              << std::setw(4) << microseconds(dev)
#if   defined(XTL_TIMING_METHOD_1)
              // FIX: 1000 is heuristic here for my laptop. QueryPerformanceCounter doesn't specify how it is related to cycles!
              << " Cycles/iteration: ["
              << std::setw(4) << min*1000/N << " -- " 
              << std::setw(4) << avg*1000/N << "/" 
              << std::setw(4) << med*1000/N << " -- "
              << std::setw(4) << max*1000/N << "]"
#elif defined(XTL_TIMING_METHOD_2)
              << " Cycles/iteration: ["
              << std::setw(4) << min/N << " -- " 
              << std::setw(4) << avg/N << "/" 
              << std::setw(4) << med/N << " -- "
              << std::setw(4) << max/N << "]"
#endif
              << std::endl;

    return med;
}

//------------------------------------------------------------------------------

int test_repetitive()
{
    std::cout << "=================== Repetitive Test ===================" << std::endl;

    size_t a1 = 0,a2 = 0;
    std::vector<int> percentages(K); // Final verdict of medians for each of the K experiments

    for (size_t n = 0; n < K; ++n)
    {
        Shape* s = make_shape(n);
        std::vector<long long> timingsV(M);
        std::vector<long long> timingsM(M);

        for (size_t m = 0; m < M; ++m)
        {
            time_stamp liStart1 = get_time_stamp();

            for (size_t i = 0; i < N; ++i)
                a1 += do_visit(*s,i);

            time_stamp liFinish1 = get_time_stamp();

            time_stamp liStart2 = get_time_stamp();

            for (size_t i = 0; i < N; ++i)
                a2 += do_match(*s,i);

            time_stamp liFinish2 = get_time_stamp();

            XTL_ASSERT(a1==a2);

            timingsV[m] = liFinish1-liStart1;
            timingsM[m] = liFinish2-liStart2;
        }

        long long avgV = display("AreaVisSeq", timingsV);
        long long avgM = display("AreaMatSeq", timingsM);
        percentages[n] = relative_performance(avgV, avgM);
        delete s;
    }

    if (a1 != a2)
    {
        std::cout << "ERROR: Invariant " << a1 << "==" << a2 << " doesn't hold." << std::endl;
        exit(42);
    }

    std::sort(percentages.begin(), percentages.end());
    return percentages[percentages.size()/2];
}

//------------------------------------------------------------------------------

int test_randomized()
{
#if !defined(NO_RANDOMIZATION)
    srand (unsigned(get_time_stamp()/get_frequency())); // Randomize pseudo random number generator
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
#if defined(TRACE_PERFORMANCE)
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
                a1 += do_visit(*shapes[i],i);

            time_stamp liFinish1 = get_time_stamp();

            time_stamp liStart2 = get_time_stamp();

            for (size_t i = 0; i < N; ++i)
                a2 += do_match(*shapes[i],i);

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

//------------------------------------------------------------------------------

int main()
{
    int ps = test_repetitive();
    //int pr = test_randomized();
    std::cout << "OVERALL: Repetitive: " 
              << abs(ps) << (ps >= 0 ? "% slower" : "% faster") << "; Random: " 
              //<< abs(pr) << (pr >= 0 ? "% slower" : "% faster") 
              << std::endl; 
}
