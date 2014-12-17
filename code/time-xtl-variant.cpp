#include <ctime>
#include <iostream>
#include <vector>
#include "boost/variant.hpp"

#include "type_switchN-patterns-xtl.hpp"
#include "adapt_boost_variant.hpp"
#include "patterns/all.hpp"

struct P { int i; };
struct Q { int i; };
struct R { int i; };

struct M { int i; };
struct N { int i; };

typedef boost::variant<P, Q, R> VP;
typedef boost::variant<M, N> VM;

int g_pm = 0;
int g_qm = 0;
int g_rm = 0;
int g_pn = 0;
int g_qn = 0;
int g_rn = 0;

struct result
{
    clock_t mach7;
    clock_t visit;
};

result measure(const std::vector<VP>& vecp,
               const std::vector<VM>& vecm,
               const bool mach7_first)
{
    result ans{};

    if (mach7_first)
    {
        clock_t t0 = clock();
        for (const VP& vp : vecp) for (const VM& vm : vecm)
        {
            using mch::C;

            Match(vp, vm)
            {
                Case(C<P>(), C<M>()) ++g_pm; break;
                Case(C<Q>(), C<M>()) ++g_qm; break;
                Case(C<R>(), C<M>()) ++g_rm; break;
                Case(C<P>(), C<N>()) ++g_pn; break;
                Case(C<Q>(), C<N>()) ++g_qn; break;
                Case(C<R>(), C<N>()) ++g_rn; break;
            }
            EndMatch
        }
        clock_t t1 = clock();
        ans.mach7 = (t1 - t0);
    }

    {
        clock_t t0 = clock();
        for (const VP& vp : vecp) for (const VM& vm : vecm)
        {
            struct increment : boost::static_visitor<>
            {
                void operator()(const P&, const M&) const { ++g_pm; }
                void operator()(const Q&, const M&) const { ++g_qm; }
                void operator()(const R&, const M&) const { ++g_rm; }
                void operator()(const P&, const N&) const { ++g_pn; }
                void operator()(const Q&, const N&) const { ++g_qn; }
                void operator()(const R&, const N&) const { ++g_rn; }
            };
            
            apply_visitor(increment{}, vp, vm);
        }
        clock_t t1 = clock();
        ans.visit = (t1 - t0);
    }

    if (!mach7_first)
    {
        clock_t t0 = clock();
        for (const VP& vp : vecp) for (const VM& vm : vecm)
        {
            using mch::C;

            Match(vp, vm)
            {
                Case(C<P>(), C<M>()) ++g_pm; break;
                Case(C<Q>(), C<M>()) ++g_qm; break;
                Case(C<R>(), C<M>()) ++g_rm; break;
                Case(C<P>(), C<N>()) ++g_pn; break;
                Case(C<Q>(), C<N>()) ++g_qn; break;
                Case(C<R>(), C<N>()) ++g_rn; break;
            }
            EndMatch
        }
        clock_t t1 = clock();
        ans.mach7 = (t1 - t0);
    }

    return ans;
}

void populate_data(std::vector<VP>& vec_vp,
                   std::vector<VM>& vec_vm,
                   const int SIZE)
{
    vec_vm.reserve(SIZE);
    vec_vp.reserve(SIZE);

    for(int i = 0; i != SIZE; ++i) {
        if (i % 3 == 0)
            vec_vp.push_back(P{0});
        else if (i % 3 == 1)
            vec_vp.push_back(Q{0});
        else
            vec_vp.push_back(R{0});

        if (i % 2 == 0)
            vec_vm.push_back(M{0});
        else
            vec_vm.push_back(N{0});
    }
}

const int DATA_SIZE = 7000;
const int TEST_NUM = 6;

int main()
{
    std::vector<VP> vec_vp;
    std::vector<VM> vec_vm;
    populate_data(vec_vp, vec_vm, DATA_SIZE);

    std::vector<result> results;
    for (int i = 0; i < TEST_NUM; i += 2) {
        results.push_back(measure(vec_vp, vec_vm, true));
        results.push_back(measure(vec_vp, vec_vm, false));
    }
    
    std::cout << "mach7" "\t" "visit" << std::endl;
    for (const result& r : results)
        std::cout << r.mach7 << "\t" << r.visit << std::endl;
}
