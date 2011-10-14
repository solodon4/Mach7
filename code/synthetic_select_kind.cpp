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
    size_t m_kind;
    Shape(size_t kind) : m_kind(kind) {}
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
    shape_kind() : Shape(N) {}
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

template <>         struct match_members<Shape>         { KS(Shape::m_kind); };
template <size_t N> struct match_members<shape_kind<N>> { KV(N); CM(0,shape_kind<N>::m_member0); CM(1,shape_kind<N>::m_member1); };

#if 1
DO_NOT_INLINE_BEGIN
size_t do_match(const Shape& s)
{
    KIND_SWITCH(s)
    {
    #define FOR_EACH_MAX      NUMBER_OF_DERIVED-1
    #define FOR_EACH_PRELUDE  KIND_CASES_BEGIN
    #define FOR_EACH_N(N)     KIND_CASE(shape_kind<N>) return N;
    #define FOR_EACH_POSTLUDE KIND_CASES_END
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
    auto const __selector_ptr = addr(s); 
    switch (apply_member(__selector_ptr, match_members<remove_ref<decltype(*__selector_ptr)>::type>::kind_selector()))
    {
        {
        }
    case match_members<shape_kind<0>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<0>>(__selector_ptr);
            return 0;
        }
    case match_members<shape_kind<1>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<1>>(__selector_ptr);
            return 1;
        }
    case match_members<shape_kind<2>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<2>>(__selector_ptr);
            return 2;
        }
    case match_members<shape_kind<3>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<3>>(__selector_ptr);
            return 3;
        }
    case match_members<shape_kind<4>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<4>>(__selector_ptr);
            return 4;
        }
    case match_members<shape_kind<5>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<5>>(__selector_ptr);
            return 5;
        }
    case match_members<shape_kind<6>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<6>>(__selector_ptr);
            return 6;
        }
    case match_members<shape_kind<7>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<7>>(__selector_ptr);
            return 7;
        }
    case match_members<shape_kind<8>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<8>>(__selector_ptr);
            return 8;
        }
    case match_members<shape_kind<9>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<9>>(__selector_ptr);
            return 9;
        }
    case match_members<shape_kind<10>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<10>>(__selector_ptr);
            return 10;
        }
    case match_members<shape_kind<11>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<11>>(__selector_ptr);
            return 11;
        }
    case match_members<shape_kind<12>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<12>>(__selector_ptr);
            return 12;
        }
    case match_members<shape_kind<13>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<13>>(__selector_ptr);
            return 13;
        }
    case match_members<shape_kind<14>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<14>>(__selector_ptr);
            return 14;
        }
    case match_members<shape_kind<15>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<15>>(__selector_ptr);
            return 15;
        }
    case match_members<shape_kind<16>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<16>>(__selector_ptr);
            return 16;
        }
    case match_members<shape_kind<17>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<17>>(__selector_ptr);
            return 17;
        }
    case match_members<shape_kind<18>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<18>>(__selector_ptr);
            return 18;
        }
    case match_members<shape_kind<19>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<19>>(__selector_ptr);
            return 19;
        }
    case match_members<shape_kind<20>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<20>>(__selector_ptr);
            return 20;
        }
    case match_members<shape_kind<21>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<21>>(__selector_ptr);
            return 21;
        }
    case match_members<shape_kind<22>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<22>>(__selector_ptr);
            return 22;
        }
    case match_members<shape_kind<23>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<23>>(__selector_ptr);
            return 23;
        }
    case match_members<shape_kind<24>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<24>>(__selector_ptr);
            return 24;
        }
    case match_members<shape_kind<25>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<25>>(__selector_ptr);
            return 25;
        }
    case match_members<shape_kind<26>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<26>>(__selector_ptr);
            return 26;
        }
    case match_members<shape_kind<27>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<27>>(__selector_ptr);
            return 27;
        }
    case match_members<shape_kind<28>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<28>>(__selector_ptr);
            return 28;
        }
    case match_members<shape_kind<29>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<29>>(__selector_ptr);
            return 29;
        }
    case match_members<shape_kind<30>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<30>>(__selector_ptr);
            return 30;
        }
    case match_members<shape_kind<31>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<31>>(__selector_ptr);
            return 31;
        }
    case match_members<shape_kind<32>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<32>>(__selector_ptr);
            return 32;
        }
    case match_members<shape_kind<33>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<33>>(__selector_ptr);
            return 33;
        }
    case match_members<shape_kind<34>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<34>>(__selector_ptr);
            return 34;
        }
    case match_members<shape_kind<35>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<35>>(__selector_ptr);
            return 35;
        }
    case match_members<shape_kind<36>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<36>>(__selector_ptr);
            return 36;
        }
    case match_members<shape_kind<37>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<37>>(__selector_ptr);
            return 37;
        }
    case match_members<shape_kind<38>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<38>>(__selector_ptr);
            return 38;
        }
    case match_members<shape_kind<39>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<39>>(__selector_ptr);
            return 39;
        }
    case match_members<shape_kind<40>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<40>>(__selector_ptr);
            return 40;
        }
    case match_members<shape_kind<41>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<41>>(__selector_ptr);
            return 41;
        }
    case match_members<shape_kind<42>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<42>>(__selector_ptr);
            return 42;
        }
    case match_members<shape_kind<43>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<43>>(__selector_ptr);
            return 43;
        }
    case match_members<shape_kind<44>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<44>>(__selector_ptr);
            return 44;
        }
    case match_members<shape_kind<45>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<45>>(__selector_ptr);
            return 45;
        }
    case match_members<shape_kind<46>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<46>>(__selector_ptr);
            return 46;
        }
    case match_members<shape_kind<47>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<47>>(__selector_ptr);
            return 47;
        }
    case match_members<shape_kind<48>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<48>>(__selector_ptr);
            return 48;
        }
    case match_members<shape_kind<49>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<49>>(__selector_ptr);
            return 49;
        }
    case match_members<shape_kind<50>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<50>>(__selector_ptr);
            return 50;
        }
    case match_members<shape_kind<51>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<51>>(__selector_ptr);
            return 51;
        }
    case match_members<shape_kind<52>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<52>>(__selector_ptr);
            return 52;
        }
    case match_members<shape_kind<53>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<53>>(__selector_ptr);
            return 53;
        }
    case match_members<shape_kind<54>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<54>>(__selector_ptr);
            return 54;
        }
    case match_members<shape_kind<55>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<55>>(__selector_ptr);
            return 55;
        }
    case match_members<shape_kind<56>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<56>>(__selector_ptr);
            return 56;
        }
    case match_members<shape_kind<57>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<57>>(__selector_ptr);
            return 57;
        }
    case match_members<shape_kind<58>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<58>>(__selector_ptr);
            return 58;
        }
    case match_members<shape_kind<59>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<59>>(__selector_ptr);
            return 59;
        }
    case match_members<shape_kind<60>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<60>>(__selector_ptr);
            return 60;
        }
    case match_members<shape_kind<61>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<61>>(__selector_ptr);
            return 61;
        }
    case match_members<shape_kind<62>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<62>>(__selector_ptr);
            return 62;
        }
    case match_members<shape_kind<63>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<63>>(__selector_ptr);
            return 63;
        }
    case match_members<shape_kind<64>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<64>>(__selector_ptr);
            return 64;
        }
    case match_members<shape_kind<65>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<65>>(__selector_ptr);
            return 65;
        }
    case match_members<shape_kind<66>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<66>>(__selector_ptr);
            return 66;
        }
    case match_members<shape_kind<67>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<67>>(__selector_ptr);
            return 67;
        }
    case match_members<shape_kind<68>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<68>>(__selector_ptr);
            return 68;
        }
    case match_members<shape_kind<69>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<69>>(__selector_ptr);
            return 69;
        }
    case match_members<shape_kind<70>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<70>>(__selector_ptr);
            return 70;
        }
    case match_members<shape_kind<71>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<71>>(__selector_ptr);
            return 71;
        }
    case match_members<shape_kind<72>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<72>>(__selector_ptr);
            return 72;
        }
    case match_members<shape_kind<73>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<73>>(__selector_ptr);
            return 73;
        }
    case match_members<shape_kind<74>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<74>>(__selector_ptr);
            return 74;
        }
    case match_members<shape_kind<75>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<75>>(__selector_ptr);
            return 75;
        }
    case match_members<shape_kind<76>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<76>>(__selector_ptr);
            return 76;
        }
    case match_members<shape_kind<77>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<77>>(__selector_ptr);
            return 77;
        }
    case match_members<shape_kind<78>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<78>>(__selector_ptr);
            return 78;
        }
    case match_members<shape_kind<79>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<79>>(__selector_ptr);
            return 79;
        }
    case match_members<shape_kind<80>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<80>>(__selector_ptr);
            return 80;
        }
    case match_members<shape_kind<81>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<81>>(__selector_ptr);
            return 81;
        }
    case match_members<shape_kind<82>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<82>>(__selector_ptr);
            return 82;
        }
    case match_members<shape_kind<83>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<83>>(__selector_ptr);
            return 83;
        }
    case match_members<shape_kind<84>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<84>>(__selector_ptr);
            return 84;
        }
    case match_members<shape_kind<85>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<85>>(__selector_ptr);
            return 85;
        }
    case match_members<shape_kind<86>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<86>>(__selector_ptr);
            return 86;
        }
    case match_members<shape_kind<87>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<87>>(__selector_ptr);
            return 87;
        }
    case match_members<shape_kind<88>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<88>>(__selector_ptr);
            return 88;
        }
    case match_members<shape_kind<89>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<89>>(__selector_ptr);
            return 89;
        }
    case match_members<shape_kind<90>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<90>>(__selector_ptr);
            return 90;
        }
    case match_members<shape_kind<91>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<91>>(__selector_ptr);
            return 91;
        }
    case match_members<shape_kind<92>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<92>>(__selector_ptr);
            return 92;
        }
    case match_members<shape_kind<93>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<93>>(__selector_ptr);
            return 93;
        }
    case match_members<shape_kind<94>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<94>>(__selector_ptr);
            return 94;
        }
    case match_members<shape_kind<95>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<95>>(__selector_ptr);
            return 95;
        }
    case match_members<shape_kind<96>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<96>>(__selector_ptr);
            return 96;
        }
    case match_members<shape_kind<97>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<97>>(__selector_ptr);
            return 97;
        }
    case match_members<shape_kind<98>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<98>>(__selector_ptr);
            return 98;
        }
    case match_members<shape_kind<99>>::kind_value:
        {
            auto result_of_dynamic_cast = stat_cast<shape_kind<99>>(__selector_ptr);
            return 99;
        }
    default: ;
    }

    return -1;
}
DO_NOT_INLINE_END
#endif

DO_NOT_INLINE_BEGIN
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
DO_NOT_INLINE_END

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
    if (UNLIKELY_BRANCH(v <= 0 || m <= 0))
    {
        std::cout << "ERROR: Insufficient timer resolution. Increase number of iterations N" << std::endl;
        exit(42);
    }
    else
    if (UNLIKELY_BRANCH(v <= m))
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
