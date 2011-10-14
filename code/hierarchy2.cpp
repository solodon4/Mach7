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

#if !defined(NUMBER_OF_BASES)
#define NUMBER_OF_BASES  5
#endif
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
struct shape_derived;

#define FOR_EACH_MAX NUMBER_OF_BASES-1
#define FOR_EACH_N(N) template <> struct shape_derived<N> : OtherBase, Shape { typedef Shape base_type; void accept(ShapeVisitor&) const; };
#include "loop_over_numbers.hpp"
#undef  FOR_EACH_N
#undef  FOR_EACH_MAX

template <size_t N>
struct shape_derived : shape_derived<N % NUMBER_OF_BASES>
{
    typedef shape_derived<N % NUMBER_OF_BASES> base_type;
    void accept(ShapeVisitor&) const;
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
    virtual void visit(const Shape&) {}
    #define FOR_EACH_MAX NUMBER_OF_DERIVED-1
    #define FOR_EACH_N(N) virtual void visit(const shape_derived<N>& s) { visit(static_cast<const shape_derived<N>::base_type&>(s)); }
    #include "loop_over_numbers.hpp"
    #undef  FOR_EACH_N
    #undef  FOR_EACH_MAX
};

#define FOR_EACH_MAX NUMBER_OF_BASES-1
#define FOR_EACH_N(N) void shape_derived<N>::accept(ShapeVisitor& v) const { v.visit(*this); }
#include "loop_over_numbers.hpp"
#undef  FOR_EACH_N
#undef  FOR_EACH_MAX

template <size_t N> void shape_derived<N>::accept(ShapeVisitor& v) const { v.visit(*this); }

#if 0
DO_NOT_INLINE_BEGIN
size_t do_match(const Shape& s)
{
    SWITCH_N(s,NUMBER_OF_DERIVED)
    {
        #define FOR_EACH_MAX      NUMBER_OF_BASES-1
        #define FOR_EACH_PRELUDE  CASES_BEGIN
        #define FOR_EACH_N(N)     CASE(shape_derived<N>) return N;
        #define FOR_EACH_POSTLUDE CASES_END
        #include "loop_over_numbers.hpp"
        #undef  FOR_EACH_POSTLUDE
        #undef  FOR_EACH_N
        #undef  FOR_EACH_PRELUDE
        #undef  FOR_EACH_MAX
    }
    return -1;
}
DO_NOT_INLINE_END
#else
DO_NOT_INLINE_BEGIN
size_t do_match(const Shape& s)
{
    static vtblmap<type_switch_info&,requires_bits<100>::value> __vtbl2lines_map; 
    auto const        __selector_ptr = addr(s); 
    const void*       __casted_ptr; 
    type_switch_info& __switch_info = __vtbl2lines_map.get(__selector_ptr); 
    switch (__switch_info.line)
    {
    default: {
             } 
             if ((__casted_ptr = dynamic_cast<const shape_derived<0>*>(__selector_ptr)))
             { 
                 if (__switch_info.line == 0) 
                 {
                     __switch_info.line = 1; 
                     __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); 
                 } 
    case 1: 
                 if ((match<shape_derived<0>>()(adjust_ptr<shape_derived<0>>(__selector_ptr,__switch_info.offset)))) 
                     return 0;
             }
             if ((__casted_ptr = dynamic_cast<const shape_derived<1>*>(__selector_ptr)))
             { 
                 if (__switch_info.line == 0) 
                 { 
                     __switch_info.line = 2; 
                     __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); 
                 } 
    case 2: 
                 if ((match<shape_derived<1>>()(adjust_ptr<shape_derived<1>>(__selector_ptr,__switch_info.offset)))) 
                     return 1;
             } 
             if ((__casted_ptr = dynamic_cast<const shape_derived<2>*>(__selector_ptr)))
             { 
                 if (__switch_info.line == 0) 
                 { 
                     __switch_info.line = 3; 
                     __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); 
                 } 
    case 3: 
                 if ((match<shape_derived<2>>()(adjust_ptr<shape_derived<2>>(__selector_ptr,__switch_info.offset)))) 
                     return 2;
             } 
             if ((__casted_ptr = dynamic_cast<const shape_derived<3>*>(__selector_ptr)))
             { 
                 if (__switch_info.line == 0) 
                 { 
                     __switch_info.line = 4; 
                     __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); 
                 } 
    case 4: 
                 if ((match<shape_derived<3>>()(adjust_ptr<shape_derived<3>>(__selector_ptr,__switch_info.offset)))) 
                     return 3;
             } 
             if ((__casted_ptr = dynamic_cast<const shape_derived<4>*>(__selector_ptr)))
             { 
                 if (__switch_info.line == 0)
                 { 
                     __switch_info.line = 5; 
                     __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); 
                 } 
    case 5: 
                 if ((match<shape_derived<4>>()(adjust_ptr<shape_derived<4>>(__selector_ptr,__switch_info.offset)))) 
                     return 4;
             } 
             if (__switch_info.line == 0)
             { 
                 __switch_info.line = 6; 
             } 
    case 6: ;
    }
    return -1;
}
DO_NOT_INLINE_END
#endif

#if 0
DO_NOT_INLINE_BEGIN
size_t do_visit(const Shape& s)
{
    struct Visitor : ShapeVisitor
    {
        #define FOR_EACH_MAX  NUMBER_OF_BASES-1
        #define FOR_EACH_N(N) virtual void visit(const shape_derived<N>&) { result = N; }
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
DO_NOT_INLINE_END
#else
DO_NOT_INLINE_BEGIN
size_t do_visit(const Shape& s)
{
    struct Visitor : ShapeVisitor
    {
        virtual void visit(const shape_derived<0>&) { result = 0; }
        virtual void visit(const shape_derived<1>&) { result = 1; }
        virtual void visit(const shape_derived<2>&) { result = 2; }
        virtual void visit(const shape_derived<3>&) { result = 3; }
        virtual void visit(const shape_derived<4>&) { result = 4; }
        size_t result;
    };
    Visitor v;
    v.result = -1;
    s.accept(v);
    return v.result;
}
DO_NOT_INLINE_END
#endif

Shape* make_shape(size_t i)
{
    switch (i)
    {
    #define FOR_EACH_MAX  NUMBER_OF_DERIVED-1
    #define FOR_EACH_N(N) case N: return new shape_derived<N>;
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
        //if (avgV)
            int percent = percentages[n] = avgM*100/avgV-100;
            std::cout << "\t\t" << percent << "% slower" << std::endl;
        //else
        //    std::cout << "Insufficient timer resolution" << std::endl;
        //std::cout << "\t\tThe following 2 numbers should be the same " << a1 << "==" << a2 << " ? " << std::boolalpha << (a1==a2) << std::endl;

        for (size_t i = 0; i < N; ++i)
        {
            delete shapes[i];
            shapes[i] = 0;
        }
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
        TRACE_PERFORMANCE_ONLY(std::vector<size_t> distribution(K));

        for (size_t i = 0; i < N; ++i)
        {
            size_t n = rand()%K;
            TRACE_PERFORMANCE_ONLY(distribution[n]++);
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
        //if (avgV)
            int percent = percentages[n] = avgM*100/avgV-100;
            std::cout << "\t\t" << percent << "% slower" << std::endl;
        //else
        //    std::cout << "Insufficient timer resolution" << std::endl;
        //std::cout << "\t\tThe following 2 numbers should be the same " << a1 << "==" << a2 << " ? " << std::boolalpha << (a1==a2) << std::endl;

        for (size_t i = 0; i < N; ++i)
        {
            delete shapes[i];
            shapes[i] = 0;
        }
    }

    std::sort(percentages.begin(), percentages.end());
    return percentages[percentages.size()/2];
}

int main()
{
    int ps = test_sequential();
    int pr = test_randomized();
    std::cout << "OVERALL: Sequential: " << ps << "% slower; Random: " << pr << "% slower;" << std::endl; 
}
