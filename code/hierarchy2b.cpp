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
    size_t m_kind;
    const int* m_all_kinds;
    Shape(size_t kind) : m_kind(kind), m_all_kinds(0) {}
    virtual ~Shape() {}
    virtual void accept(ShapeVisitor&) const = 0;
    #define FOR_EACH_MAX NUMBER_OF_VFUNCS-2
    #define FOR_EACH_N(N) virtual void foo ## N() {}
    #include "loop_over_numbers.hpp"
    #undef  FOR_EACH_N
    #undef  FOR_EACH_MAX
};

//------------------------------------------------------------------------------

template <size_t N>
struct shape_kind : OtherBase, Shape
{
    shape_kind() : Shape(N) { this->m_all_kinds = get_kinds<Shape>(original2remapped<Shape>(N)); }
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

//------------------------------------------------------------------------------

struct ShapeVisitor
{
    #define FOR_EACH_MAX NUMBER_OF_DERIVED-1
    #define FOR_EACH_N(N) virtual void visit(const shape_kind<N>&) {}
    #include "loop_over_numbers.hpp"
    #undef  FOR_EACH_N
    #undef  FOR_EACH_MAX
};

//------------------------------------------------------------------------------

template <size_t N> void shape_kind<N>::accept(ShapeVisitor& v) const { v.visit(*this); }

SKV(Shape,0); // Declare the smallest kind value for Shape hierarchy
template <>         struct match_members<Shape>         { KS(Shape::m_kind); };
template <size_t N> struct match_members<shape_kind<N>> { KV(N); CM(0,shape_kind<N>::m_member0); CM(1,shape_kind<N>::m_member1); };

#if 1
XTL_DO_NOT_INLINE_BEGIN
size_t do_match(const Shape& s)
{
    MatchF(s)
    {
    #define FOR_EACH_MAX      NUMBER_OF_DERIVED-1
    #define FOR_EACH_N(N)     CaseF(shape_kind<N>) return N;
    #include "loop_over_numbers.hpp"
    #undef  FOR_EACH_N
    #undef  FOR_EACH_MAX
    }
    EndMatchF
    return -1;
}
XTL_DO_NOT_INLINE_END
#else
XTL_DO_NOT_INLINE_BEGIN
size_t do_match(const Shape& s)
{
    {
        const int* __kinds = 0;
        size_t __attempt = 0;
        auto const __selector_ptr = addr(s);
        typedef underlying<decltype(*__selector_ptr)>::type __selector_type;
        ;
        int __kind_selector = original2remapped<__selector_type>((int)apply_member(__selector_ptr, match_members<__selector_type>::kind_selector()));
ReMatch:
        switch (__kind_selector) 
        {
        default:
            if ((!__kinds)) 
            {
                static std::vector<const int*> __kinds_cache;
                if ((__kind_selector >= __kinds_cache.size())) __kinds_cache.resize(__kind_selector+1);
                if (!(__kinds = __kinds_cache[__kind_selector])) __kinds = __kinds_cache[__kind_selector] = get_kinds<__selector_type>(__kind_selector);
            }
            ;
            ;
            __kind_selector = __kinds[++__attempt];
            goto ReMatch;
        case 0:
            break;
            {
                {
                    {
                    }
                }
                if (((__kind_selector == remapped_kind<shape_kind<0>>::value))) 
                {
        case remapped_kind<shape_kind<0>>::value:
            typedef shape_kind<0> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 0;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<1>>::value))) 
                {
        case remapped_kind<shape_kind<1>>::value:
            typedef shape_kind<1> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 1;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<2>>::value))) 
                {
        case remapped_kind<shape_kind<2>>::value:
            typedef shape_kind<2> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 2;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<3>>::value))) 
                {
        case remapped_kind<shape_kind<3>>::value:
            typedef shape_kind<3> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 3;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<4>>::value))) 
                {
        case remapped_kind<shape_kind<4>>::value:
            typedef shape_kind<4> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 4;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<5>>::value))) 
                {
        case remapped_kind<shape_kind<5>>::value:
            typedef shape_kind<5> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 5;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<6>>::value))) 
                {
        case remapped_kind<shape_kind<6>>::value:
            typedef shape_kind<6> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 6;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<7>>::value))) 
                {
        case remapped_kind<shape_kind<7>>::value:
            typedef shape_kind<7> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 7;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<8>>::value))) 
                {
        case remapped_kind<shape_kind<8>>::value:
            typedef shape_kind<8> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 8;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<9>>::value))) 
                {
        case remapped_kind<shape_kind<9>>::value:
            typedef shape_kind<9> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 9;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<10>>::value))) 
                {
        case remapped_kind<shape_kind<10>>::value:
            typedef shape_kind<10> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 10;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<11>>::value))) 
                {
        case remapped_kind<shape_kind<11>>::value:
            typedef shape_kind<11> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 11;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<12>>::value))) 
                {
        case remapped_kind<shape_kind<12>>::value:
            typedef shape_kind<12> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 12;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<13>>::value))) 
                {
        case remapped_kind<shape_kind<13>>::value:
            typedef shape_kind<13> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 13;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<14>>::value))) 
                {
        case remapped_kind<shape_kind<14>>::value:
            typedef shape_kind<14> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 14;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<15>>::value))) 
                {
        case remapped_kind<shape_kind<15>>::value:
            typedef shape_kind<15> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 15;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<16>>::value))) 
                {
        case remapped_kind<shape_kind<16>>::value:
            typedef shape_kind<16> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 16;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<17>>::value))) 
                {
        case remapped_kind<shape_kind<17>>::value:
            typedef shape_kind<17> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 17;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<18>>::value))) 
                {
        case remapped_kind<shape_kind<18>>::value:
            typedef shape_kind<18> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 18;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<19>>::value))) 
                {
        case remapped_kind<shape_kind<19>>::value:
            typedef shape_kind<19> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 19;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<20>>::value))) 
                {
        case remapped_kind<shape_kind<20>>::value:
            typedef shape_kind<20> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 20;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<21>>::value))) 
                {
        case remapped_kind<shape_kind<21>>::value:
            typedef shape_kind<21> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 21;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<22>>::value))) 
                {
        case remapped_kind<shape_kind<22>>::value:
            typedef shape_kind<22> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 22;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<23>>::value))) 
                {
        case remapped_kind<shape_kind<23>>::value:
            typedef shape_kind<23> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 23;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<24>>::value))) 
                {
        case remapped_kind<shape_kind<24>>::value:
            typedef shape_kind<24> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 24;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<25>>::value))) 
                {
        case remapped_kind<shape_kind<25>>::value:
            typedef shape_kind<25> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 25;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<26>>::value))) 
                {
        case remapped_kind<shape_kind<26>>::value:
            typedef shape_kind<26> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 26;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<27>>::value))) 
                {
        case remapped_kind<shape_kind<27>>::value:
            typedef shape_kind<27> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 27;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<28>>::value))) 
                {
        case remapped_kind<shape_kind<28>>::value:
            typedef shape_kind<28> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 28;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<29>>::value))) 
                {
        case remapped_kind<shape_kind<29>>::value:
            typedef shape_kind<29> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 29;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<30>>::value))) 
                {
        case remapped_kind<shape_kind<30>>::value:
            typedef shape_kind<30> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 30;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<31>>::value))) 
                {
        case remapped_kind<shape_kind<31>>::value:
            typedef shape_kind<31> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 31;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<32>>::value))) 
                {
        case remapped_kind<shape_kind<32>>::value:
            typedef shape_kind<32> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 32;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<33>>::value))) 
                {
        case remapped_kind<shape_kind<33>>::value:
            typedef shape_kind<33> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 33;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<34>>::value))) 
                {
        case remapped_kind<shape_kind<34>>::value:
            typedef shape_kind<34> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 34;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<35>>::value))) 
                {
        case remapped_kind<shape_kind<35>>::value:
            typedef shape_kind<35> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 35;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<36>>::value))) 
                {
        case remapped_kind<shape_kind<36>>::value:
            typedef shape_kind<36> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 36;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<37>>::value))) 
                {
        case remapped_kind<shape_kind<37>>::value:
            typedef shape_kind<37> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 37;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<38>>::value))) 
                {
        case remapped_kind<shape_kind<38>>::value:
            typedef shape_kind<38> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 38;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<39>>::value))) 
                {
        case remapped_kind<shape_kind<39>>::value:
            typedef shape_kind<39> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 39;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<40>>::value))) 
                {
        case remapped_kind<shape_kind<40>>::value:
            typedef shape_kind<40> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 40;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<41>>::value))) 
                {
        case remapped_kind<shape_kind<41>>::value:
            typedef shape_kind<41> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 41;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<42>>::value))) 
                {
        case remapped_kind<shape_kind<42>>::value:
            typedef shape_kind<42> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 42;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<43>>::value))) 
                {
        case remapped_kind<shape_kind<43>>::value:
            typedef shape_kind<43> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 43;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<44>>::value))) 
                {
        case remapped_kind<shape_kind<44>>::value:
            typedef shape_kind<44> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 44;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<45>>::value))) 
                {
        case remapped_kind<shape_kind<45>>::value:
            typedef shape_kind<45> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 45;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<46>>::value))) 
                {
        case remapped_kind<shape_kind<46>>::value:
            typedef shape_kind<46> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 46;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<47>>::value))) 
                {
        case remapped_kind<shape_kind<47>>::value:
            typedef shape_kind<47> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 47;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<48>>::value))) 
                {
        case remapped_kind<shape_kind<48>>::value:
            typedef shape_kind<48> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 48;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<49>>::value))) 
                {
        case remapped_kind<shape_kind<49>>::value:
            typedef shape_kind<49> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 49;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<50>>::value))) 
                {
        case remapped_kind<shape_kind<50>>::value:
            typedef shape_kind<50> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 50;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<51>>::value))) 
                {
        case remapped_kind<shape_kind<51>>::value:
            typedef shape_kind<51> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 51;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<52>>::value))) 
                {
        case remapped_kind<shape_kind<52>>::value:
            typedef shape_kind<52> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 52;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<53>>::value))) 
                {
        case remapped_kind<shape_kind<53>>::value:
            typedef shape_kind<53> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 53;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<54>>::value))) 
                {
        case remapped_kind<shape_kind<54>>::value:
            typedef shape_kind<54> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 54;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<55>>::value))) 
                {
        case remapped_kind<shape_kind<55>>::value:
            typedef shape_kind<55> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 55;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<56>>::value))) 
                {
        case remapped_kind<shape_kind<56>>::value:
            typedef shape_kind<56> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 56;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<57>>::value))) 
                {
        case remapped_kind<shape_kind<57>>::value:
            typedef shape_kind<57> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 57;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<58>>::value))) 
                {
        case remapped_kind<shape_kind<58>>::value:
            typedef shape_kind<58> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 58;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<59>>::value))) 
                {
        case remapped_kind<shape_kind<59>>::value:
            typedef shape_kind<59> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 59;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<60>>::value))) 
                {
        case remapped_kind<shape_kind<60>>::value:
            typedef shape_kind<60> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 60;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<61>>::value))) 
                {
        case remapped_kind<shape_kind<61>>::value:
            typedef shape_kind<61> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 61;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<62>>::value))) 
                {
        case remapped_kind<shape_kind<62>>::value:
            typedef shape_kind<62> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 62;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<63>>::value))) 
                {
        case remapped_kind<shape_kind<63>>::value:
            typedef shape_kind<63> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 63;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<64>>::value))) 
                {
        case remapped_kind<shape_kind<64>>::value:
            typedef shape_kind<64> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 64;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<65>>::value))) 
                {
        case remapped_kind<shape_kind<65>>::value:
            typedef shape_kind<65> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 65;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<66>>::value))) 
                {
        case remapped_kind<shape_kind<66>>::value:
            typedef shape_kind<66> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 66;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<67>>::value))) 
                {
        case remapped_kind<shape_kind<67>>::value:
            typedef shape_kind<67> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 67;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<68>>::value))) 
                {
        case remapped_kind<shape_kind<68>>::value:
            typedef shape_kind<68> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 68;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<69>>::value))) 
                {
        case remapped_kind<shape_kind<69>>::value:
            typedef shape_kind<69> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 69;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<70>>::value))) 
                {
        case remapped_kind<shape_kind<70>>::value:
            typedef shape_kind<70> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 70;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<71>>::value))) 
                {
        case remapped_kind<shape_kind<71>>::value:
            typedef shape_kind<71> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 71;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<72>>::value))) 
                {
        case remapped_kind<shape_kind<72>>::value:
            typedef shape_kind<72> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 72;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<73>>::value))) 
                {
        case remapped_kind<shape_kind<73>>::value:
            typedef shape_kind<73> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 73;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<74>>::value))) 
                {
        case remapped_kind<shape_kind<74>>::value:
            typedef shape_kind<74> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 74;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<75>>::value))) 
                {
        case remapped_kind<shape_kind<75>>::value:
            typedef shape_kind<75> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 75;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<76>>::value))) 
                {
        case remapped_kind<shape_kind<76>>::value:
            typedef shape_kind<76> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 76;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<77>>::value))) 
                {
        case remapped_kind<shape_kind<77>>::value:
            typedef shape_kind<77> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 77;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<78>>::value))) 
                {
        case remapped_kind<shape_kind<78>>::value:
            typedef shape_kind<78> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 78;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<79>>::value))) 
                {
        case remapped_kind<shape_kind<79>>::value:
            typedef shape_kind<79> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 79;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<80>>::value))) 
                {
        case remapped_kind<shape_kind<80>>::value:
            typedef shape_kind<80> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 80;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<81>>::value))) 
                {
        case remapped_kind<shape_kind<81>>::value:
            typedef shape_kind<81> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 81;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<82>>::value))) 
                {
        case remapped_kind<shape_kind<82>>::value:
            typedef shape_kind<82> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 82;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<83>>::value))) 
                {
        case remapped_kind<shape_kind<83>>::value:
            typedef shape_kind<83> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 83;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<84>>::value))) 
                {
        case remapped_kind<shape_kind<84>>::value:
            typedef shape_kind<84> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 84;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<85>>::value))) 
                {
        case remapped_kind<shape_kind<85>>::value:
            typedef shape_kind<85> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 85;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<86>>::value))) 
                {
        case remapped_kind<shape_kind<86>>::value:
            typedef shape_kind<86> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 86;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<87>>::value))) 
                {
        case remapped_kind<shape_kind<87>>::value:
            typedef shape_kind<87> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 87;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<88>>::value))) 
                {
        case remapped_kind<shape_kind<88>>::value:
            typedef shape_kind<88> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 88;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<89>>::value))) 
                {
        case remapped_kind<shape_kind<89>>::value:
            typedef shape_kind<89> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 89;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<90>>::value))) 
                {
        case remapped_kind<shape_kind<90>>::value:
            typedef shape_kind<90> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 90;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<91>>::value))) 
                {
        case remapped_kind<shape_kind<91>>::value:
            typedef shape_kind<91> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 91;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<92>>::value))) 
                {
        case remapped_kind<shape_kind<92>>::value:
            typedef shape_kind<92> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 92;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<93>>::value))) 
                {
        case remapped_kind<shape_kind<93>>::value:
            typedef shape_kind<93> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 93;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<94>>::value))) 
                {
        case remapped_kind<shape_kind<94>>::value:
            typedef shape_kind<94> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 94;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<95>>::value))) 
                {
        case remapped_kind<shape_kind<95>>::value:
            typedef shape_kind<95> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 95;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<96>>::value))) 
                {
        case remapped_kind<shape_kind<96>>::value:
            typedef shape_kind<96> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 96;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<97>>::value))) 
                {
        case remapped_kind<shape_kind<97>>::value:
            typedef shape_kind<97> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 97;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<98>>::value))) 
                {
        case remapped_kind<shape_kind<98>>::value:
            typedef shape_kind<98> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 98;
            }
                }
                if (((__kind_selector == remapped_kind<shape_kind<99>>::value))) 
                {
        case remapped_kind<shape_kind<99>>::value:
            typedef shape_kind<99> target_type;
            auto matched = stat_cast<target_type>(__selector_ptr);
            (void)matched;
            ;
            {
                return 99;
            }
                }
            }
        }
    }
    return -1;
}
XTL_DO_NOT_INLINE_END
#endif

//------------------------------------------------------------------------------

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
              << std::setw(4) << dev << std::endl;
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

//------------------------------------------------------------------------------

int main()
{
    int ps = test_sequential();
    int pr = test_randomized();
    std::cout << "OVERALL: Sequential: " 
              << abs(ps) << (ps >= 0 ? "% slower" : "% faster") << "; Random: " 
              << abs(pr) << (pr >= 0 ? "% slower" : "% faster") << std::endl; 
}
