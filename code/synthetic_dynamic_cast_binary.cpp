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
#include "timing.hpp"

//------------------------------------------------------------------------------

#if !XTL_USE_MEMOIZED_CAST
    #define dynamic_cast fast_dynamic_cast
#endif

//------------------------------------------------------------------------------

#if !defined(NUMBER_OF_VFUNCS)
#define NUMBER_OF_VFUNCS  1
#endif
#if !defined(NUMBER_OF_DERIVED)
#define NUMBER_OF_DERIVED 100
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

    enum { fdc_size = 10 };
    static const size_t fast_dynamic_cast_primes[fdc_size][2];
    static size_t fdc_id(size_t n)
    {
        XTL_ASSERT(req_bits(n) < fdc_size);
        size_t id = 1;

        if (n)
            for (size_t m = req_bits(n), i = m; i; --i)
                id *= fast_dynamic_cast_primes[m-i][(n & (1<<i-1)) != 0];
        //std::cout << n << "->" << id << std::endl;
        return id;
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
struct shape_kind : shape_kind<N/2>
{
    typedef shape_kind<N/2> base_class;
    shape_kind(size_t n = N) : base_class(n) {}
    void accept(ShapeVisitor&) const;
};

template <>
struct shape_kind<0> : OtherBase, Shape
{
    typedef Shape base_class;
    shape_kind(size_t n = 0) : base_class(n) {}
    void accept(ShapeVisitor&) const;
};

//------------------------------------------------------------------------------

struct ShapeVisitor
{
    virtual void visit(const shape_kind<0>&) {}
    #define FOR_EACH_MAX NUMBER_OF_DERIVED-2
    #define FOR_EACH_N(N) virtual void visit(const shape_kind<N+1>& s) { visit(static_cast<const shape_kind<N+1>::base_class&>(s)); }
    #include "loop_over_numbers.hpp"
    #undef  FOR_EACH_N
    #undef  FOR_EACH_MAX
};

template <size_t N> void shape_kind<N>::accept(ShapeVisitor& v) const { v.visit(*this); }
                    void shape_kind<0>::accept(ShapeVisitor& v) const { v.visit(*this); }

//------------------------------------------------------------------------------

/// Primes numbers for each level of the binary hierarchy
const size_t Shape::fast_dynamic_cast_primes[fdc_size][2] = 
{
    { 2, 2}, // Because the root is 2
    { 3, 5},
    { 7,11},
    {13,17},
    {19,23},
    {29,31},
    {37,41},
    {43,47},
    {53,59},
    {61,67}
};

inline size_t id(size_t n) { return Shape::fdc_id(n); }
const size_t shape_ids[100] =
{
     id( 0), id( 1), id( 2), id( 3), id( 4), id( 5), id( 6), id( 7), id( 8), id( 9),
     id(10), id(11), id(12), id(13), id(14), id(15), id(16), id(17), id(18), id(19),
     id(20), id(21), id(22), id(23), id(24), id(25), id(26), id(27), id(28), id(29),
     id(30), id(31), id(32), id(33), id(34), id(35), id(36), id(37), id(38), id(39),
     id(40), id(41), id(42), id(43), id(44), id(45), id(46), id(47), id(48), id(49),
     id(50), id(51), id(52), id(53), id(54), id(55), id(56), id(57), id(58), id(59),
     id(60), id(61), id(62), id(63), id(64), id(65), id(66), id(67), id(68), id(69),
     id(70), id(71), id(72), id(73), id(74), id(75), id(76), id(77), id(78), id(79),
     id(80), id(81), id(82), id(83), id(84), id(85), id(86), id(87), id(88), id(89),
     id(90), id(91), id(92), id(93), id(94), id(95), id(96), id(97), id(98), id(99),
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

//------------------------------------------------------------------------------

XTL_DO_NOT_INLINE_BEGIN
size_t do_match(const Shape& s, size_t n)
{
    if (const shape_kind< 0>* p0 = dynamic_cast<const shape_kind< 0>*>(&s))
        if (const shape_kind< 1>* p1 = dynamic_cast<const shape_kind< 1>*>(p0)) 
            if (const shape_kind< 2>* p2 = dynamic_cast<const shape_kind< 2>*>(p1)) 
                if (const shape_kind< 4>* p4 = dynamic_cast<const shape_kind< 4>*>(p2)) 
                    if (const shape_kind< 8>* p8  = dynamic_cast<const shape_kind< 8>*>(p4)) 
                        if (const shape_kind<16>* p16 = dynamic_cast<const shape_kind<16>*>(p8)) 
                            if (const shape_kind<32>* p32 = dynamic_cast<const shape_kind<32>*>(p16))
                                if (const shape_kind<64>* p64 = dynamic_cast<const shape_kind<64>*>(p32))
                                    return 64 ;
                                else
                                if (const shape_kind<65>* p65 = dynamic_cast<const shape_kind<65>*>(p32))
                                    return 65 ;
                                else
                                    return 32 ;
                            else
                            if (const shape_kind<33>* p33 = dynamic_cast<const shape_kind<33>*>(p16))
                                if (const shape_kind<66>* p66 = dynamic_cast<const shape_kind<66>*>(p33))
                                    return 66 ;
                                else
                                if (const shape_kind<67>* p67 = dynamic_cast<const shape_kind<67>*>(p33))
                                    return 67 ;
                                else
                                    return 33 ;
                            else
                                return 16 ;
                        else
                        if (const shape_kind<17>* p17 = dynamic_cast<const shape_kind<17>*>(p8)) 
                            if (const shape_kind<34>* p34 = dynamic_cast<const shape_kind<34>*>(p17))
                                if (const shape_kind<68>* p68 = dynamic_cast<const shape_kind<68>*>(p34))
                                    return 68 ;
                                else
                                if (const shape_kind<69>* p69 = dynamic_cast<const shape_kind<69>*>(p34))
                                    return 69 ;
                                else
                                    return 34 ;
                            else
                            if (const shape_kind<35>* p35 = dynamic_cast<const shape_kind<35>*>(p17))
                                if (const shape_kind<70>* p70 = dynamic_cast<const shape_kind<70>*>(p35))
                                    return 70 ;
                                else
                                if (const shape_kind<71>* p71 = dynamic_cast<const shape_kind<71>*>(p35))
                                    return 71 ;
                                else
                                    return 35 ;
                            else
                                return 17 ;
                        else
                            return  8 ;
                    else
                    if (const shape_kind< 9>* p9  = dynamic_cast<const shape_kind< 9>*>(p4)) 
                        if (const shape_kind<18>* p18 = dynamic_cast<const shape_kind<18>*>(p9))
                            if (const shape_kind<36>* p36 = dynamic_cast<const shape_kind<36>*>(p18))
                                if (const shape_kind<72>* p72 = dynamic_cast<const shape_kind<72>*>(p36))
                                    return 72 ;
                                else
                                if (const shape_kind<73>* p73 = dynamic_cast<const shape_kind<73>*>(p36))
                                    return 73 ;
                                else
                                    return 36 ;
                            else
                            if (const shape_kind<37>* p37 = dynamic_cast<const shape_kind<37>*>(p18))
                                if (const shape_kind<74>* p74 = dynamic_cast<const shape_kind<74>*>(p37))
                                    return 74 ;
                                else
                                if (const shape_kind<75>* p75 = dynamic_cast<const shape_kind<75>*>(p37))
                                    return 75 ;
                                else
                                    return 37 ;
                            else
                                return 18 ;
                        else
                        if (const shape_kind<19>* p19 = dynamic_cast<const shape_kind<19>*>(p9)) 
                            if (const shape_kind<38>* p38 = dynamic_cast<const shape_kind<38>*>(p19))
                                if (const shape_kind<76>* p76 = dynamic_cast<const shape_kind<76>*>(p38))
                                    return 76 ;
                                else
                                if (const shape_kind<77>* p77 = dynamic_cast<const shape_kind<77>*>(p38))
                                    return 77 ;
                                else
                                    return 38 ;
                            else
                            if (const shape_kind<39>* p39 = dynamic_cast<const shape_kind<39>*>(p19))
                                if (const shape_kind<78>* p78 = dynamic_cast<const shape_kind<78>*>(p39))
                                    return 78 ;
                                else
                                if (const shape_kind<79>* p79 = dynamic_cast<const shape_kind<79>*>(p39))
                                    return 79 ;
                                else
                                    return 39 ;
                            else
                                return 19 ;
                        else
                            return  9 ;
                    else
                        return  4 ;
                else
                if (const shape_kind< 5>* p5 = dynamic_cast<const shape_kind< 5>*>(p2)) 
                    if (const shape_kind<10>* p10 = dynamic_cast<const shape_kind<10>*>(p5)) 
                        if (const shape_kind<20>* p20 = dynamic_cast<const shape_kind<20>*>(p10))
                            if (const shape_kind<40>* p40 = dynamic_cast<const shape_kind<40>*>(p20))
                                if (const shape_kind<80>* p80 = dynamic_cast<const shape_kind<80>*>(p40))
                                    return 80 ;
                                else
                                if (const shape_kind<81>* p81 = dynamic_cast<const shape_kind<81>*>(p40))
                                    return 81 ;
                                else
                                    return 40 ;
                            else
                            if (const shape_kind<41>* p41 = dynamic_cast<const shape_kind<41>*>(p20))
                                if (const shape_kind<82>* p82 = dynamic_cast<const shape_kind<82>*>(p41))
                                    return 82 ;
                                else
                                if (const shape_kind<83>* p83 = dynamic_cast<const shape_kind<83>*>(p41))
                                    return 83 ;
                                else
                                    return 41 ;
                            else
                                return 20 ;
                        else
                        if (const shape_kind<21>* p21 = dynamic_cast<const shape_kind<21>*>(p10))
                            if (const shape_kind<42>* p42 = dynamic_cast<const shape_kind<42>*>(p21))
                                if (const shape_kind<84>* p84 = dynamic_cast<const shape_kind<84>*>(p42))
                                    return 84 ;
                                else
                                if (const shape_kind<85>* p85 = dynamic_cast<const shape_kind<85>*>(p42))
                                    return 85 ;
                                else
                                    return 42 ;
                            else
                            if (const shape_kind<43>* p43 = dynamic_cast<const shape_kind<43>*>(p21))
                                if (const shape_kind<86>* p86 = dynamic_cast<const shape_kind<86>*>(p43))
                                    return 86 ;
                                else
                                if (const shape_kind<87>* p87 = dynamic_cast<const shape_kind<87>*>(p43))
                                    return 87 ;
                                else
                                    return 43 ;
                            else
                                return 21 ;
                        else
                            return 10 ;
                    else
                    if (const shape_kind<11>* p11 = dynamic_cast<const shape_kind<11>*>(p5)) 
                        if (const shape_kind<22>* p22 = dynamic_cast<const shape_kind<22>*>(p11))
                            if (const shape_kind<44>* p44 = dynamic_cast<const shape_kind<44>*>(p22))
                                if (const shape_kind<88>* p88 = dynamic_cast<const shape_kind<88>*>(p44))
                                    return 88 ;
                                else
                                if (const shape_kind<89>* p89 = dynamic_cast<const shape_kind<89>*>(p44))
                                    return 89 ;
                                else
                                    return 44 ;
                            else
                            if (const shape_kind<45>* p45 = dynamic_cast<const shape_kind<45>*>(p22))
                                if (const shape_kind<90>* p90 = dynamic_cast<const shape_kind<90>*>(p45))
                                    return 90 ;
                                else
                                if (const shape_kind<91>* p91 = dynamic_cast<const shape_kind<91>*>(p45))
                                    return 91 ;
                                else
                                    return 45 ;
                            else
                                return 22 ;
                        else
                        if (const shape_kind<23>* p23 = dynamic_cast<const shape_kind<23>*>(p11))
                            if (const shape_kind<46>* p46 = dynamic_cast<const shape_kind<46>*>(p23))
                                if (const shape_kind<92>* p92 = dynamic_cast<const shape_kind<92>*>(p46))
                                    return 92 ;
                                else
                                if (const shape_kind<93>* p93 = dynamic_cast<const shape_kind<93>*>(p46))
                                    return 93 ;
                                else
                                    return 46 ;
                            else
                            if (const shape_kind<47>* p47 = dynamic_cast<const shape_kind<47>*>(p23))
                                if (const shape_kind<94>* p94 = dynamic_cast<const shape_kind<94>*>(p47))
                                    return 94 ;
                                else
                                if (const shape_kind<95>* p95 = dynamic_cast<const shape_kind<95>*>(p47))
                                    return 95 ;
                                else
                                    return 47 ;
                            else
                                return 23 ;
                        else
                            return 11 ;
                    else
                        return  5 ;
                else
                    return  2 ;
            else
            if (const shape_kind< 3>* p3 = dynamic_cast<const shape_kind< 3>*>(p1)) 
                if (const shape_kind< 6>* p6 = dynamic_cast<const shape_kind< 6>*>(p3)) 
                    if (const shape_kind<12>* p12 = dynamic_cast<const shape_kind<12>*>(p6)) 
                        if (const shape_kind<24>* p24 = dynamic_cast<const shape_kind<24>*>(p12))
                            if (const shape_kind<48>* p48 = dynamic_cast<const shape_kind<48>*>(p24))
                                if (const shape_kind<96>* p96 = dynamic_cast<const shape_kind<96>*>(p48))
                                    return 96 ;
                                else
                                if (const shape_kind<97>* p97 = dynamic_cast<const shape_kind<97>*>(p48))
                                    return 97 ;
                                else
                                    return 48 ;
                            else
                            if (const shape_kind<49>* p49 = dynamic_cast<const shape_kind<49>*>(p24))
                                if (const shape_kind<98>* p98 = dynamic_cast<const shape_kind<98>*>(p49))
                                    return 98 ;
                                else
                                if (const shape_kind<99>* p99 = dynamic_cast<const shape_kind<99>*>(p49))
                                    return 99 ;
                                else
                                    return 49 ;
                            else
                                return 24 ;
                        else
                        if (const shape_kind<25>* p25 = dynamic_cast<const shape_kind<25>*>(p12))
                            if (const shape_kind<50>* p50 = dynamic_cast<const shape_kind<50>*>(p25))
                                return 50 ;
                            else
                            if (const shape_kind<51>* p51 = dynamic_cast<const shape_kind<51>*>(p25))
                                return 51 ;
                            else
                                return 25 ;
                        else
                            return 12 ;
                    else
                    if (const shape_kind<13>* p13 = dynamic_cast<const shape_kind<13>*>(p6)) 
                        if (const shape_kind<26>* p26 = dynamic_cast<const shape_kind<26>*>(p13))
                            if (const shape_kind<52>* p52 = dynamic_cast<const shape_kind<52>*>(p26))
                                return 52 ;
                            else
                            if (const shape_kind<53>* p53 = dynamic_cast<const shape_kind<53>*>(p26))
                                return 53 ;
                            else
                                return 26 ;
                        else
                        if (const shape_kind<27>* p27 = dynamic_cast<const shape_kind<27>*>(p13))
                            if (const shape_kind<54>* p54 = dynamic_cast<const shape_kind<54>*>(p27))
                                return 54 ;
                            else
                            if (const shape_kind<55>* p55 = dynamic_cast<const shape_kind<55>*>(p27))
                                return 55 ;
                            else
                                return 27 ;
                        else
                            return 13 ;
                    else
                        return  6 ;
                else
                if (const shape_kind< 7>* p7 = dynamic_cast<const shape_kind< 7>*>(p3)) 
                    if (const shape_kind<14>* p14 = dynamic_cast<const shape_kind<14>*>(p7)) 
                        if (const shape_kind<28>* p28 = dynamic_cast<const shape_kind<28>*>(p14))
                            if (const shape_kind<56>* p56 = dynamic_cast<const shape_kind<56>*>(p28))
                                return 56 ;
                            else
                            if (const shape_kind<57>* p57 = dynamic_cast<const shape_kind<57>*>(p28))
                                return 57 ;
                            else
                                return 28 ;
                        else
                        if (const shape_kind<29>* p29 = dynamic_cast<const shape_kind<29>*>(p14))
                            if (const shape_kind<58>* p58 = dynamic_cast<const shape_kind<58>*>(p29))
                                return 58 ;
                            else
                            if (const shape_kind<59>* p59 = dynamic_cast<const shape_kind<59>*>(p29))
                                return 59 ;
                            else
                                return 29 ;
                        else
                            return 14 ;
                    else
                    if (const shape_kind<15>* p15 = dynamic_cast<const shape_kind<15>*>(p7)) 
                        if (const shape_kind<30>* p30 = dynamic_cast<const shape_kind<30>*>(p15))
                            if (const shape_kind<60>* p60 = dynamic_cast<const shape_kind<60>*>(p30))
                                return 60 ;
                            else
                            if (const shape_kind<61>* p61 = dynamic_cast<const shape_kind<61>*>(p30))
                                return 61 ;
                            else
                                return 30 ;
                        else
                        if (const shape_kind<31>* p31 = dynamic_cast<const shape_kind<31>*>(p15))
                            if (const shape_kind<62>* p62 = dynamic_cast<const shape_kind<62>*>(p31))
                                return 62 ;
                            else
                            if (const shape_kind<63>* p63 = dynamic_cast<const shape_kind<63>*>(p31))
                                return 63 ;
                            else
                                return 31 ;
                        else
                            return 15 ;
                    else
                        return  7 ;
                else
                    return  3 ;
            else
                return  1 ;
        else
            return  0 ;
    return -1;
}
XTL_DO_NOT_INLINE_END

//------------------------------------------------------------------------------

XTL_DO_NOT_INLINE_BEGIN
size_t do_visit(const Shape& s, size_t n)
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
        std::vector<Shape*> shapes(N);

        for (size_t i = 0; i < N; ++i)
        {
            shapes[i] = make_shape((n+i)*2-n-2*i);
        }

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

        long long avgV = display("AreaVisRep", timingsV);
        long long avgM = display("AreaMatRep", timingsM);
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
