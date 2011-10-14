#include <algorithm>
#include <iostream>
#include <iomanip>
#include <numeric>
#include <vector>
#include <windows.h>
#include "match_select.hpp"

#define FOR_EACH_MAX 3

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

#if 1
int do_match(const Shape& s)
{
    SWITCH(s)
    {
    default:
    #define FOR_EACH_N(N) CASE(shape_kind<N>) return N;
    #include "loop_over_numbers.hpp"
    #undef  FOR_EACH_N
    }
    //assert(!"Inexhaustive search");
    return -1;
}
#else
int do_match(Shape& s)
{
    static vtbl2lines __vtbl2lines_map;
    decltype(s)& __selector_var = s;
    int n = on(__vtbl2lines_map,__selector_var);
    switch (n)
    {
    default:
    case  0: if (match<shape_kind< 0>>()(__selector_var,__vtbl2lines_map, 0)) return  0;
    case  1: if (match<shape_kind< 1>>()(__selector_var,__vtbl2lines_map, 1)) return  1;
    case  2: if (match<shape_kind< 2>>()(__selector_var,__vtbl2lines_map, 2)) return  2;
    case  3: if (match<shape_kind< 3>>()(__selector_var,__vtbl2lines_map, 3)) return  3;
    case  4: if (match<shape_kind< 4>>()(__selector_var,__vtbl2lines_map, 4)) return  4;
    case  5: if (match<shape_kind< 5>>()(__selector_var,__vtbl2lines_map, 5)) return  5;
    case  6: if (match<shape_kind< 6>>()(__selector_var,__vtbl2lines_map, 6)) return  6;
    case  7: if (match<shape_kind< 7>>()(__selector_var,__vtbl2lines_map, 7)) return  7;
    case  8: if (match<shape_kind< 8>>()(__selector_var,__vtbl2lines_map, 8)) return  8;
    case  9: if (match<shape_kind< 9>>()(__selector_var,__vtbl2lines_map, 9)) return  9;
    case 10: if (match<shape_kind<10>>()(__selector_var,__vtbl2lines_map,10)) return 10;
    case 11: if (match<shape_kind<11>>()(__selector_var,__vtbl2lines_map,11)) return 11;
    case 12: if (match<shape_kind<12>>()(__selector_var,__vtbl2lines_map,12)) return 12;
    case 13: if (match<shape_kind<13>>()(__selector_var,__vtbl2lines_map,13)) return 13;
    case 14: if (match<shape_kind<14>>()(__selector_var,__vtbl2lines_map,14)) return 14;
    case 15: if (match<shape_kind<15>>()(__selector_var,__vtbl2lines_map,15)) return 15;
    case 16: if (match<shape_kind<16>>()(__selector_var,__vtbl2lines_map,16)) return 16;
    case 17: if (match<shape_kind<17>>()(__selector_var,__vtbl2lines_map,17)) return 17;
    case 18: if (match<shape_kind<18>>()(__selector_var,__vtbl2lines_map,18)) return 18;
    case 19: if (match<shape_kind<19>>()(__selector_var,__vtbl2lines_map,19)) return 19;
    case 20: if (match<shape_kind<20>>()(__selector_var,__vtbl2lines_map,20)) return 20;
    case 21: if (match<shape_kind<21>>()(__selector_var,__vtbl2lines_map,21)) return 21;
    case 22: if (match<shape_kind<22>>()(__selector_var,__vtbl2lines_map,22)) return 22;
    case 23: if (match<shape_kind<23>>()(__selector_var,__vtbl2lines_map,23)) return 23;
    case 24: if (match<shape_kind<24>>()(__selector_var,__vtbl2lines_map,24)) return 24;
    case 25: if (match<shape_kind<25>>()(__selector_var,__vtbl2lines_map,25)) return 25;
    case 26: if (match<shape_kind<26>>()(__selector_var,__vtbl2lines_map,26)) return 26;
    case 27: if (match<shape_kind<27>>()(__selector_var,__vtbl2lines_map,27)) return 27;
    case 28: if (match<shape_kind<28>>()(__selector_var,__vtbl2lines_map,28)) return 28;
    case 29: if (match<shape_kind<29>>()(__selector_var,__vtbl2lines_map,29)) return 29;
    case 30: if (match<shape_kind<30>>()(__selector_var,__vtbl2lines_map,30)) return 30;
    case 31: if (match<shape_kind<31>>()(__selector_var,__vtbl2lines_map,31)) return 31;
    case 32: if (match<shape_kind<32>>()(__selector_var,__vtbl2lines_map,32)) return 32;
    case 33: if (match<shape_kind<33>>()(__selector_var,__vtbl2lines_map,33)) return 33;
    case 34: if (match<shape_kind<34>>()(__selector_var,__vtbl2lines_map,34)) return 34;
    case 35: if (match<shape_kind<35>>()(__selector_var,__vtbl2lines_map,35)) return 35;
    case 36: if (match<shape_kind<36>>()(__selector_var,__vtbl2lines_map,36)) return 36;
    case 37: if (match<shape_kind<37>>()(__selector_var,__vtbl2lines_map,37)) return 37;
    case 38: if (match<shape_kind<38>>()(__selector_var,__vtbl2lines_map,38)) return 38;
    case 39: if (match<shape_kind<39>>()(__selector_var,__vtbl2lines_map,39)) return 39;
    case 40: if (match<shape_kind<40>>()(__selector_var,__vtbl2lines_map,40)) return 40;
    case 41: if (match<shape_kind<41>>()(__selector_var,__vtbl2lines_map,41)) return 41;
    case 42: if (match<shape_kind<42>>()(__selector_var,__vtbl2lines_map,42)) return 42;
    case 43: if (match<shape_kind<43>>()(__selector_var,__vtbl2lines_map,43)) return 43;
    case 44: if (match<shape_kind<44>>()(__selector_var,__vtbl2lines_map,44)) return 44;
    case 45: if (match<shape_kind<45>>()(__selector_var,__vtbl2lines_map,45)) return 45;
    case 46: if (match<shape_kind<46>>()(__selector_var,__vtbl2lines_map,46)) return 46;
    case 47: if (match<shape_kind<47>>()(__selector_var,__vtbl2lines_map,47)) return 47;
    case 48: if (match<shape_kind<48>>()(__selector_var,__vtbl2lines_map,48)) return 48;
    case 49: if (match<shape_kind<49>>()(__selector_var,__vtbl2lines_map,49)) return 49;
    case 50: if (match<shape_kind<50>>()(__selector_var,__vtbl2lines_map,50)) return 50;
    case 51: if (match<shape_kind<51>>()(__selector_var,__vtbl2lines_map,51)) return 51;
    case 52: if (match<shape_kind<52>>()(__selector_var,__vtbl2lines_map,52)) return 52;
    case 53: if (match<shape_kind<53>>()(__selector_var,__vtbl2lines_map,53)) return 53;
    case 54: if (match<shape_kind<54>>()(__selector_var,__vtbl2lines_map,54)) return 54;
    case 55: if (match<shape_kind<55>>()(__selector_var,__vtbl2lines_map,55)) return 55;
    case 56: if (match<shape_kind<56>>()(__selector_var,__vtbl2lines_map,56)) return 56;
    case 57: if (match<shape_kind<57>>()(__selector_var,__vtbl2lines_map,57)) return 57;
    case 58: if (match<shape_kind<58>>()(__selector_var,__vtbl2lines_map,58)) return 58;
    case 59: if (match<shape_kind<59>>()(__selector_var,__vtbl2lines_map,59)) return 59;
    case 60: if (match<shape_kind<60>>()(__selector_var,__vtbl2lines_map,60)) return 60;
    case 61: if (match<shape_kind<61>>()(__selector_var,__vtbl2lines_map,61)) return 61;
    case 62: if (match<shape_kind<62>>()(__selector_var,__vtbl2lines_map,62)) return 62;
    case 63: if (match<shape_kind<63>>()(__selector_var,__vtbl2lines_map,63)) return 63;
    case 64: if (match<shape_kind<64>>()(__selector_var,__vtbl2lines_map,64)) return 64;
    case 65: if (match<shape_kind<65>>()(__selector_var,__vtbl2lines_map,65)) return 65;
    case 66: if (match<shape_kind<66>>()(__selector_var,__vtbl2lines_map,66)) return 66;
    case 67: if (match<shape_kind<67>>()(__selector_var,__vtbl2lines_map,67)) return 67;
    case 68: if (match<shape_kind<68>>()(__selector_var,__vtbl2lines_map,68)) return 68;
    case 69: if (match<shape_kind<69>>()(__selector_var,__vtbl2lines_map,69)) return 69;
    case 70: if (match<shape_kind<70>>()(__selector_var,__vtbl2lines_map,70)) return 70;
    case 71: if (match<shape_kind<71>>()(__selector_var,__vtbl2lines_map,71)) return 71;
    case 72: if (match<shape_kind<72>>()(__selector_var,__vtbl2lines_map,72)) return 72;
    case 73: if (match<shape_kind<73>>()(__selector_var,__vtbl2lines_map,73)) return 73;
    case 74: if (match<shape_kind<74>>()(__selector_var,__vtbl2lines_map,74)) return 74;
    case 75: if (match<shape_kind<75>>()(__selector_var,__vtbl2lines_map,75)) return 75;
    case 76: if (match<shape_kind<76>>()(__selector_var,__vtbl2lines_map,76)) return 76;
    case 77: if (match<shape_kind<77>>()(__selector_var,__vtbl2lines_map,77)) return 77;
    case 78: if (match<shape_kind<78>>()(__selector_var,__vtbl2lines_map,78)) return 78;
    case 79: if (match<shape_kind<79>>()(__selector_var,__vtbl2lines_map,79)) return 79;
    case 80: if (match<shape_kind<80>>()(__selector_var,__vtbl2lines_map,80)) return 80;
    case 81: if (match<shape_kind<81>>()(__selector_var,__vtbl2lines_map,81)) return 81;
    case 82: if (match<shape_kind<82>>()(__selector_var,__vtbl2lines_map,82)) return 82;
    case 83: if (match<shape_kind<83>>()(__selector_var,__vtbl2lines_map,83)) return 83;
    case 84: if (match<shape_kind<84>>()(__selector_var,__vtbl2lines_map,84)) return 84;
    case 85: if (match<shape_kind<85>>()(__selector_var,__vtbl2lines_map,85)) return 85;
    case 86: if (match<shape_kind<86>>()(__selector_var,__vtbl2lines_map,86)) return 86;
    case 87: if (match<shape_kind<87>>()(__selector_var,__vtbl2lines_map,87)) return 87;
    case 88: if (match<shape_kind<88>>()(__selector_var,__vtbl2lines_map,88)) return 88;
    case 89: if (match<shape_kind<89>>()(__selector_var,__vtbl2lines_map,89)) return 89;
    case 90: if (match<shape_kind<90>>()(__selector_var,__vtbl2lines_map,90)) return 90;
    case 91: if (match<shape_kind<91>>()(__selector_var,__vtbl2lines_map,91)) return 91;
    case 92: if (match<shape_kind<92>>()(__selector_var,__vtbl2lines_map,92)) return 92;
    case 93: if (match<shape_kind<93>>()(__selector_var,__vtbl2lines_map,93)) return 93;
    case 94: if (match<shape_kind<94>>()(__selector_var,__vtbl2lines_map,94)) return 94;
    case 95: if (match<shape_kind<95>>()(__selector_var,__vtbl2lines_map,95)) return 95;
    case 96: if (match<shape_kind<96>>()(__selector_var,__vtbl2lines_map,96)) return 96;
    case 97: if (match<shape_kind<97>>()(__selector_var,__vtbl2lines_map,97)) return 97;
    case 98: if (match<shape_kind<98>>()(__selector_var,__vtbl2lines_map,98)) return 98;
    case 99: if (match<shape_kind<99>>()(__selector_var,__vtbl2lines_map,99)) return 99;
    }
    //assert(!"Inexhaustive search");
    return -1;
}
#endif
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
const int M = 100;   // The amount of times time measuring is done

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
    long long min = timings.front();
    long long max = timings.back();
    long long avg = mean(timings);
    long long dev = deviation(timings);
    std::cout << name << " Time: ["
              << std::setw(4) << min*1000000/Freq.QuadPart << " -- " 
              << std::setw(4) << avg*1000000/Freq.QuadPart << " -- " 
              << std::setw(4) << max*1000000/Freq.QuadPart 
              << "] Dev = " << std::setw(4) << dev << std::endl;
    return avg;
}

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
}

int main()
{
    test_sequential();
    test_randomized();
}

#undef  FOR_EACH_MAX
