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

#if !defined(NUMBER_OF_VFUNCS)
  #define NUMBER_OF_VFUNCS  1
#endif
#if !defined(NUMBER_OF_BASES)
  #define NUMBER_OF_BASES  5
#endif
#if !defined(NUMBER_OF_DERIVED)
  #define NUMBER_OF_DERIVED 100
#endif
#if NUMBER_OF_BASES > NUMBER_OF_DERIVED
  #error Visitor Forwarding only makes sense when number of base classes is smaller than the number of all derived classes.
#endif
#if !defined(XTL_VISITOR_FORWARDING)
  #define XTL_VISITOR_FORWARDING 0
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
#if XTL_DEFAULT_SYNTAX == 'K' || XTL_DEFAULT_SYNTAX == 'k' || XTL_DEFAULT_SYNTAX == 'F' || XTL_DEFAULT_SYNTAX == 'f'
    size_t m_kind;
    Shape(size_t kind) : m_kind(kind),
#else
    Shape(size_t kind = 0) :
#endif
        m_member0(kind+0),
        m_member1(kind+1),
        m_member2(kind+2),
        m_member3(kind+3),
        m_member4(kind+4),
        m_member5(kind+5),
        m_member6(kind+6),
        m_member7(kind+7),
        m_member8(kind+8),
        m_member9(kind+9)
    {}
    virtual ~Shape() {}
    virtual void accept(ShapeVisitor&) const = 0;
    #define FOR_EACH_MAX NUMBER_OF_VFUNCS-2
    #define FOR_EACH_N(N) virtual void foo ## N() {}
    #include "loop_over_numbers.hpp"
    #undef  FOR_EACH_N
    #undef  FOR_EACH_MAX

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

#if XTL_VISITOR_FORWARDING

template <size_t N>
struct shape_kind;

#define FOR_EACH_MAX NUMBER_OF_BASES-1
#if XTL_DEFAULT_SYNTAX == 'K' || XTL_DEFAULT_SYNTAX == 'k' || XTL_DEFAULT_SYNTAX == 'F' || XTL_DEFAULT_SYNTAX == 'f'
#define FOR_EACH_N(N) template <> struct shape_kind<N> : OtherBase, Shape { typedef Shape base_type; void accept(ShapeVisitor&) const; shape_kind(size_t k = N) : base_type(k) {} };
#else
#define FOR_EACH_N(N) template <> struct shape_kind<N> : OtherBase, Shape { typedef Shape base_type; void accept(ShapeVisitor&) const; };
#endif
#include "loop_over_numbers.hpp"
#undef  FOR_EACH_N
#undef  FOR_EACH_MAX

//------------------------------------------------------------------------------

template <size_t N>
struct shape_kind : shape_kind<N % NUMBER_OF_BASES>
{
    typedef shape_kind<N % NUMBER_OF_BASES> base_type;
#if XTL_DEFAULT_SYNTAX == 'K' || XTL_DEFAULT_SYNTAX == 'k' || XTL_DEFAULT_SYNTAX == 'F' || XTL_DEFAULT_SYNTAX == 'f'
    shape_kind() : base_type(N) {}
#endif
    void accept(ShapeVisitor&) const;
};

#else // !XTL_VISITOR_FORWARDING

template <size_t N>
struct shape_kind : OtherBase, Shape
{
#if XTL_DEFAULT_SYNTAX == 'K' || XTL_DEFAULT_SYNTAX == 'k' || XTL_DEFAULT_SYNTAX == 'F' || XTL_DEFAULT_SYNTAX == 'f'
    shape_kind() : Shape(N) {}
#endif
    void accept(ShapeVisitor& v) const;
};

#endif // XTL_VISITOR_FORWARDING

//------------------------------------------------------------------------------

struct ShapeVisitor
{
    virtual void visit(const Shape&) {}
    #define FOR_EACH_MAX NUMBER_OF_DERIVED-1
    #if XTL_VISITOR_FORWARDING
    #define FOR_EACH_N(N) virtual void visit(const shape_kind<N>& s) { visit(static_cast<const shape_kind<N>::base_type&>(s)); }
    #else
    #define FOR_EACH_N(N) virtual void visit(const shape_kind<N>&) {}
    #endif
    #include "loop_over_numbers.hpp"
    #undef  FOR_EACH_N
    #undef  FOR_EACH_MAX
};

//------------------------------------------------------------------------------

#if XTL_VISITOR_FORWARDING
  #define FOR_EACH_MAX NUMBER_OF_BASES-1
  #define FOR_EACH_N(N) void shape_kind<N>::accept(ShapeVisitor& v) const { v.visit(*this); }
  #include "loop_over_numbers.hpp"
  #undef  FOR_EACH_N
  #undef  FOR_EACH_MAX
#endif // XTL_VISITOR_FORWARDING

//------------------------------------------------------------------------------

template <size_t N> void shape_kind<N>::accept(ShapeVisitor& v) const { v.visit(*this); }

//------------------------------------------------------------------------------

#if XTL_DEFAULT_SYNTAX == 'K' || XTL_DEFAULT_SYNTAX == 'k'
template <>         struct match_members<Shape>         { KS(Shape::m_kind); };
template <size_t N> struct match_members<shape_kind<N>> { KV(N); CM(0,shape_kind<N>::m_member0); CM(1,shape_kind<N>::m_member1); };

#elif XTL_DEFAULT_SYNTAX == 'F' || XTL_DEFAULT_SYNTAX == 'f'

SKV(Shape,0); // Declare the smallest kind value for Shape hierarchy
template <>         struct match_members<Shape>         { KS(Shape::m_kind); KV(NUMBER_OF_DERIVED+1); };

// NOTE: We need to explicitly instantiate all match_members as otherwise our kinds 
//       associations don't get instantiated and recorded. This happens because
//       now that match_members with specific N > base cases is never instantiated
// FIX:  Wonder though why it didn't instantiate them for base cases at least since
//       we instantiate them explicitly in Case statements
#define FOR_EACH_MAX NUMBER_OF_BASES-1
#define FOR_EACH_N(N) template <> struct match_members<shape_kind<N>> { KV(N);  BCS(shape_kind<N>,Shape); CM(0,shape_kind<N>::m_member0); CM(1,shape_kind<N>::m_member1); };
#include "loop_over_numbers.hpp"
#undef  FOR_EACH_N
#undef  FOR_EACH_MAX

// NOTE: We need to explicitly instantiate all match_members as otherwise our kinds 
//       associations don't get instantiated and recorded. This happens because
//       now that match_members with specific N > base cases is never instantiated
#define FOR_EACH_MAX NUMBER_OF_DERIVED-NUMBER_OF_BASES-1
#define FOR_EACH_N(N) template <> struct match_members<shape_kind<NUMBER_OF_BASES+N>> { KV(NUMBER_OF_BASES+N);  BCS(shape_kind<NUMBER_OF_BASES+N>,shape_kind<NUMBER_OF_BASES+N>::base_type,Shape); CM(0,shape_kind<NUMBER_OF_BASES+N>::m_member0); CM(1,shape_kind<NUMBER_OF_BASES+N>::m_member1); };
#include "loop_over_numbers.hpp"
#undef  FOR_EACH_N
#undef  FOR_EACH_MAX

#endif

//------------------------------------------------------------------------------

XTL_DO_NOT_INLINE_BEGIN
size_t do_match(const Shape& s, size_t n)
{
    #if XTL_VISITOR_FORWARDING
        #define FOR_EACH_MAX      NUMBER_OF_BASES-1
        #define FOR_EACH_N(N)     Case(shape_kind<NUMBER_OF_BASES-1-N>)   return NUMBER_OF_BASES-1-N;
    #else
        #define FOR_EACH_MAX      NUMBER_OF_DERIVED-1
        #define FOR_EACH_N(N)     Case(shape_kind<NUMBER_OF_DERIVED-1-N>) return NUMBER_OF_DERIVED-1-N;
    #endif
    #define FOR_EACH_PRELUDE  Match(s)
    #define FOR_EACH_POSTLUDE EndMatch
    #include "loop_over_numbers.hpp"
    #undef  FOR_EACH_POSTLUDE
    #undef  FOR_EACH_N
    #undef  FOR_EACH_PRELUDE
    #undef  FOR_EACH_MAX
    return -1;
}
XTL_DO_NOT_INLINE_END

//------------------------------------------------------------------------------

XTL_DO_NOT_INLINE_BEGIN
size_t do_visit(const Shape& s, size_t n)
{
    struct Visitor : ShapeVisitor
    {
        #if XTL_VISITOR_FORWARDING
        #define FOR_EACH_MAX  NUMBER_OF_BASES-1
        #else
        #define FOR_EACH_MAX  NUMBER_OF_DERIVED-1
        #endif
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

#if XTL_VISITOR_FORWARDING
    #define forwarding "Yes"
#else
    #define forwarding "No "
#endif

//------------------------------------------------------------------------------

#if XTL_DEFAULT_SYNTAX == 'P' || XTL_DEFAULT_SYNTAX == 'K' || XTL_DEFAULT_SYNTAX == 'F' || XTL_DEFAULT_SYNTAX == 'U' || XTL_DEFAULT_SYNTAX == 'E'
    #define syntax "Special"
#else
    #define syntax "Generic"
#endif

//------------------------------------------------------------------------------

#if   XTL_DEFAULT_SYNTAX == 'P' || XTL_DEFAULT_SYNTAX == 'p'
    #define encoding "Polymorphic"
#elif XTL_DEFAULT_SYNTAX == 'K' || XTL_DEFAULT_SYNTAX == 'k'
    #define encoding "Kind       "
#elif XTL_DEFAULT_SYNTAX == 'F' || XTL_DEFAULT_SYNTAX == 'f'
    #define encoding "KindAdvanced"
#elif XTL_DEFAULT_SYNTAX == 'U' || XTL_DEFAULT_SYNTAX == 'u'
    #define encoding "Union      "
#elif XTL_DEFAULT_SYNTAX == 'E' || XTL_DEFAULT_SYNTAX == 'e'
    #define encoding "Exception  "
#else
    #define encoding "Unknown    "
#endif

//------------------------------------------------------------------------------

#define msg_prefix "SYNTAX: " syntax " ENCODING: " encoding " FORWARDING: " forwarding " TEST: "

//------------------------------------------------------------------------------

int main()
{
#if   defined(XTL_REP_TEST)
    int pr = test_repetitive();
    std::cout << msg_prefix "Repetitive: " << abs(pr) << (pr >= 0 ? "% slower" : "% faster") << std::endl; 
#elif defined(XTL_SEQ_TEST)
    int ps = test_sequential();
    std::cout << msg_prefix "Sequential: " << abs(ps) << (ps >= 0 ? "% slower" : "% faster") << std::endl; 
#elif defined(XTL_RND_TEST)
    int pn = test_randomized();
    std::cout << msg_prefix "Randomized: " << abs(pn) << (pn >= 0 ? "% slower" : "% faster") << std::endl; 
#else
    #error Test scenario REP, SEQ or RND has not been chosen.
#endif
}
