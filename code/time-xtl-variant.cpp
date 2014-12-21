#include <ctime>
#include <iostream>
#include <vector>
#include "boost/variant.hpp"

#include "type_switchN-patterns-xtl.hpp"
#include "adapt_boost_variant.hpp"
#include "patterns/all.hpp"

#if defined(_MSC_VER)
#define __attribute__(x)
#define __builtin_expect(exp, val) exp
#endif

struct P { int i; P(int i) : i(i) {} };
struct Q { int i; Q(int i) : i(i) {} };
struct R { int i; R(int i) : i(i) {} };

struct M { int i; M(int i) : i(i) {} };
struct N { int i; N(int i) : i(i) {} };

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
    result ans = {};

    if (mach7_first)
    {
        clock_t t0 = clock();
        for (std::vector<VP>::const_iterator pvp = vecp.begin(); pvp != vecp.end(); ++pvp)
		{
			const VP& vp = *pvp;

		for (std::vector<VM>::const_iterator pvm = vecm.begin(); pvm != vecm.end(); ++pvm)
        {
			const VM& vm = *pvm;
            using mch::C;
#if 1
			{
				struct match_uid_type 
				{
				};
				enum 
				{
					is_inside_case_clause = 0, number_of_subjects = 2, polymorphic_index00 = -1, __base_counter = 0 
				};
				auto&& subject_ref0 = vp;
				auto const subject_ptr0 = mch::addr(subject_ref0);
				typedef XTL_CPP0X_TYPENAME mch::underlying<decltype(*subject_ptr0)>::type source_type0;
				typedef source_type0 target_type0 __attribute__((unused));
				enum 
				{
					is_polymorphic0 = xtl::is_poly_morphic<source_type0>::value, polymorphic_index0 = polymorphic_index00 + is_polymorphic0 
				};
				auto& match0 = *subject_ptr0;
				(void)match0;
				static_assert(xtl::is_poly_morphic<source_type0>::value, "Type of subject " "0" " should be polymorphic when you use Match");
				const void* __casted_ptr0 = 0;
				auto&& subject_ref1 = vm;
				auto const subject_ptr1 = mch::addr(subject_ref1);
				typedef XTL_CPP0X_TYPENAME mch::underlying<decltype(*subject_ptr1)>::type source_type1;
				typedef source_type1 target_type1 __attribute__((unused));
				enum 
				{
					is_polymorphic1 = xtl::is_poly_morphic<source_type1>::value, polymorphic_index1 = polymorphic_index0 + is_polymorphic1 
				};
				auto& match1 = *subject_ptr1;
				(void)match1;
				static_assert(xtl::is_poly_morphic<source_type1>::value, "Type of subject " "1" " should be polymorphic when you use Match");
				const void* __casted_ptr1 = 0;
				enum 
				{
					number_of_polymorphic_subjects = is_polymorphic0 + is_polymorphic1 
				};
				typedef mch::vtbl_map<number_of_polymorphic_subjects,mch::type_switch_info<number_of_polymorphic_subjects>> vtbl_map_type;
				vtbl_map_type& __vtbl2case_map = mch::preallocated<vtbl_map_type,match_uid_type>::value;
				mch::type_switch_info<number_of_polymorphic_subjects>& __switch_info = __vtbl2case_map.xtl_get(subject_ptr0 , subject_ptr1);
				switch (number_of_polymorphic_subjects ? __switch_info.target : 0) 
				{
				default: 
					{
						{
							{
								{
								}
							}
						}
						{
							typedef XTL_CPP0X_TYPENAME mch::underlying<decltype(mch::filter(C<P>()))>::type type_of_pattern0;
							static_assert(mch::is_pattern<type_of_pattern0>::value,"Case-clause expects patterns as its arguments");
							typedef XTL_CPP0X_TYPENAME mch::underlying<type_of_pattern0>::type:: accepted_type_for<source_type0>::type target_type0;
							typedef XTL_CPP0X_TYPENAME mch::underlying<decltype(mch::filter(C<M>()))>::type type_of_pattern1;
							static_assert(mch::is_pattern<type_of_pattern1>::value,"Case-clause expects patterns as its arguments");
							typedef XTL_CPP0X_TYPENAME mch::underlying<type_of_pattern1>::type:: accepted_type_for<source_type1>::type target_type1;
							if ((__casted_ptr0 = mch::dynamic_cast_when_polymorphic<const target_type0*>(subject_ptr0)) != 0 && (__casted_ptr1 = mch::dynamic_cast_when_polymorphic<const target_type1*>(subject_ptr1)) != 0) 
							{
								static_assert(number_of_subjects == 2, "Number of targets in the case clause must be the same as the number of subjects in the Match statement");
								enum 
								{
									target_label = 1 -__base_counter, is_inside_case_clause = 1 
								};
								if ((number_of_polymorphic_subjects))
									if ((__builtin_expect((long int)(__switch_info.target == 0), 1))) 
									{
										__switch_info.target = target_label;
										mch::type_switch_info_offset_helper<is_polymorphic0,decltype(__switch_info)>::set_offset(__switch_info, polymorphic_index0, intptr_t(__casted_ptr0)-intptr_t(subject_ptr0));
										mch::type_switch_info_offset_helper<is_polymorphic1,decltype(__switch_info)>::set_offset(__switch_info, polymorphic_index1, intptr_t(__casted_ptr1)-intptr_t(subject_ptr1));
									}
				case target_label: 
								auto& match0 = *mch::adjust_ptr_if_xtl_polymorphic<target_type0>(subject_ptr0,mch::type_switch_info_offset_helper<is_polymorphic0,decltype(__switch_info)>::get_offset(__switch_info, polymorphic_index0));
								auto& match1 = *mch::adjust_ptr_if_xtl_polymorphic<target_type1>(subject_ptr1,mch::type_switch_info_offset_helper<is_polymorphic1,decltype(__switch_info)>::get_offset(__switch_info, polymorphic_index1));
								if (mch::filter(C<P>())(match0) && mch::filter(C<M>())(match1)) 
								{
									++g_pm;
									break;
								}
							}
						}
						{
							typedef XTL_CPP0X_TYPENAME mch::underlying<decltype(mch::filter(C<Q>()))>::type type_of_pattern0;
							static_assert(mch::is_pattern<type_of_pattern0>::value,"Case-clause expects patterns as its arguments");
							typedef XTL_CPP0X_TYPENAME mch::underlying<type_of_pattern0>::type:: accepted_type_for<source_type0>::type target_type0;
							typedef XTL_CPP0X_TYPENAME mch::underlying<decltype(mch::filter(C<M>()))>::type type_of_pattern1;
							static_assert(mch::is_pattern<type_of_pattern1>::value,"Case-clause expects patterns as its arguments");
							typedef XTL_CPP0X_TYPENAME mch::underlying<type_of_pattern1>::type:: accepted_type_for<source_type1>::type target_type1;
							if ((__casted_ptr0 = mch::dynamic_cast_when_polymorphic<const target_type0*>(subject_ptr0)) != 0 && (__casted_ptr1 = mch::dynamic_cast_when_polymorphic<const target_type1*>(subject_ptr1)) != 0) 
							{
								static_assert(number_of_subjects == 2, "Number of targets in the case clause must be the same as the number of subjects in the Match statement");
								enum 
								{
									target_label = 2 -__base_counter, is_inside_case_clause = 1 
								};
								if ((number_of_polymorphic_subjects))
									if ((__builtin_expect((long int)(__switch_info.target == 0), 1))) 
									{
										__switch_info.target = target_label;
										mch::type_switch_info_offset_helper<is_polymorphic0,decltype(__switch_info)>::set_offset(__switch_info, polymorphic_index0, intptr_t(__casted_ptr0)-intptr_t(subject_ptr0));
										mch::type_switch_info_offset_helper<is_polymorphic1,decltype(__switch_info)>::set_offset(__switch_info, polymorphic_index1, intptr_t(__casted_ptr1)-intptr_t(subject_ptr1));
									}
				case target_label:
								auto& match0 = *mch::adjust_ptr_if_xtl_polymorphic<target_type0>(subject_ptr0,mch::type_switch_info_offset_helper<is_polymorphic0,decltype(__switch_info)>::get_offset(__switch_info, polymorphic_index0));
								auto& match1 = *mch::adjust_ptr_if_xtl_polymorphic<target_type1>(subject_ptr1,mch::type_switch_info_offset_helper<is_polymorphic1,decltype(__switch_info)>::get_offset(__switch_info, polymorphic_index1));
								if (mch::filter(C<Q>())(match0) && mch::filter(C<M>())(match1)) 
								{
									++g_qm;
									break;
								}
							}
						}
						{
							typedef XTL_CPP0X_TYPENAME mch::underlying<decltype(mch::filter(C<R>()))>::type type_of_pattern0;
							static_assert(mch::is_pattern<type_of_pattern0>::value,"Case-clause expects patterns as its arguments");
							typedef XTL_CPP0X_TYPENAME mch::underlying<type_of_pattern0>::type:: accepted_type_for<source_type0>::type target_type0;
							typedef XTL_CPP0X_TYPENAME mch::underlying<decltype(mch::filter(C<M>()))>::type type_of_pattern1;
							static_assert(mch::is_pattern<type_of_pattern1>::value,"Case-clause expects patterns as its arguments");
							typedef XTL_CPP0X_TYPENAME mch::underlying<type_of_pattern1>::type:: accepted_type_for<source_type1>::type target_type1;
							if ((__casted_ptr0 = mch::dynamic_cast_when_polymorphic<const target_type0*>(subject_ptr0)) != 0 && (__casted_ptr1 = mch::dynamic_cast_when_polymorphic<const target_type1*>(subject_ptr1)) != 0) 
							{
								static_assert(number_of_subjects == 2, "Number of targets in the case clause must be the same as the number of subjects in the Match statement");
								enum 
								{
									target_label = 3 -__base_counter, is_inside_case_clause = 1 
								};
								if ((number_of_polymorphic_subjects))
									if ((__builtin_expect((long int)(__switch_info.target == 0), 1))) 
									{
										__switch_info.target = target_label;
										mch::type_switch_info_offset_helper<is_polymorphic0,decltype(__switch_info)>::set_offset(__switch_info, polymorphic_index0, intptr_t(__casted_ptr0)-intptr_t(subject_ptr0));
										mch::type_switch_info_offset_helper<is_polymorphic1,decltype(__switch_info)>::set_offset(__switch_info, polymorphic_index1, intptr_t(__casted_ptr1)-intptr_t(subject_ptr1));
									}
				case target_label: 
								auto& match0 = *mch::adjust_ptr_if_xtl_polymorphic<target_type0>(subject_ptr0,mch::type_switch_info_offset_helper<is_polymorphic0,decltype(__switch_info)>::get_offset(__switch_info, polymorphic_index0));
								auto& match1 = *mch::adjust_ptr_if_xtl_polymorphic<target_type1>(subject_ptr1,mch::type_switch_info_offset_helper<is_polymorphic1,decltype(__switch_info)>::get_offset(__switch_info, polymorphic_index1));
								if (mch::filter(C<R>())(match0) && mch::filter(C<M>())(match1)) 
								{
									++g_rm;
									break;
								}
							}
						}
						{
							typedef XTL_CPP0X_TYPENAME mch::underlying<decltype(mch::filter(C<P>()))>::type type_of_pattern0;
							static_assert(mch::is_pattern<type_of_pattern0>::value,"Case-clause expects patterns as its arguments");
							typedef XTL_CPP0X_TYPENAME mch::underlying<type_of_pattern0>::type:: accepted_type_for<source_type0>::type target_type0;
							typedef XTL_CPP0X_TYPENAME mch::underlying<decltype(mch::filter(C<N>()))>::type type_of_pattern1;
							static_assert(mch::is_pattern<type_of_pattern1>::value,"Case-clause expects patterns as its arguments");
							typedef XTL_CPP0X_TYPENAME mch::underlying<type_of_pattern1>::type:: accepted_type_for<source_type1>::type target_type1;
							if ((__casted_ptr0 = mch::dynamic_cast_when_polymorphic<const target_type0*>(subject_ptr0)) != 0 && (__casted_ptr1 = mch::dynamic_cast_when_polymorphic<const target_type1*>(subject_ptr1)) != 0) 
							{
								static_assert(number_of_subjects == 2, "Number of targets in the case clause must be the same as the number of subjects in the Match statement");
								enum 
								{
									target_label = 4 -__base_counter, is_inside_case_clause = 1 
								};
								if ((number_of_polymorphic_subjects))
									if ((__builtin_expect((long int)(__switch_info.target == 0), 1))) 
									{
										__switch_info.target = target_label;
										mch::type_switch_info_offset_helper<is_polymorphic0,decltype(__switch_info)>::set_offset(__switch_info, polymorphic_index0, intptr_t(__casted_ptr0)-intptr_t(subject_ptr0));
										mch::type_switch_info_offset_helper<is_polymorphic1,decltype(__switch_info)>::set_offset(__switch_info, polymorphic_index1, intptr_t(__casted_ptr1)-intptr_t(subject_ptr1));
									}
				case target_label:
								auto& match0 = *mch::adjust_ptr_if_xtl_polymorphic<target_type0>(subject_ptr0,mch::type_switch_info_offset_helper<is_polymorphic0,decltype(__switch_info)>::get_offset(__switch_info, polymorphic_index0));
								auto& match1 = *mch::adjust_ptr_if_xtl_polymorphic<target_type1>(subject_ptr1,mch::type_switch_info_offset_helper<is_polymorphic1,decltype(__switch_info)>::get_offset(__switch_info, polymorphic_index1));
								if (mch::filter(C<P>())(match0) && mch::filter(C<N>())(match1)) 
								{
									++g_pn;
									break;
								}
							}
						}
						{
							typedef XTL_CPP0X_TYPENAME mch::underlying<decltype(mch::filter(C<Q>()))>::type type_of_pattern0;
							static_assert(mch::is_pattern<type_of_pattern0>::value,"Case-clause expects patterns as its arguments");
							typedef XTL_CPP0X_TYPENAME mch::underlying<type_of_pattern0>::type:: accepted_type_for<source_type0>::type target_type0;
							typedef XTL_CPP0X_TYPENAME mch::underlying<decltype(mch::filter(C<N>()))>::type type_of_pattern1;
							static_assert(mch::is_pattern<type_of_pattern1>::value,"Case-clause expects patterns as its arguments");
							typedef XTL_CPP0X_TYPENAME mch::underlying<type_of_pattern1>::type:: accepted_type_for<source_type1>::type target_type1;
							if ((__casted_ptr0 = mch::dynamic_cast_when_polymorphic<const target_type0*>(subject_ptr0)) != 0 && (__casted_ptr1 = mch::dynamic_cast_when_polymorphic<const target_type1*>(subject_ptr1)) != 0) 
							{
								static_assert(number_of_subjects == 2, "Number of targets in the case clause must be the same as the number of subjects in the Match statement");
								enum 
								{
									target_label = 5 -__base_counter, is_inside_case_clause = 1 
								};
								if ((number_of_polymorphic_subjects))
									if ((__builtin_expect((long int)(__switch_info.target == 0), 1))) 
									{
										__switch_info.target = target_label;
										mch::type_switch_info_offset_helper<is_polymorphic0,decltype(__switch_info)>::set_offset(__switch_info, polymorphic_index0, intptr_t(__casted_ptr0)-intptr_t(subject_ptr0));
										mch::type_switch_info_offset_helper<is_polymorphic1,decltype(__switch_info)>::set_offset(__switch_info, polymorphic_index1, intptr_t(__casted_ptr1)-intptr_t(subject_ptr1));
									}
				case target_label:
								auto& match0 = *mch::adjust_ptr_if_xtl_polymorphic<target_type0>(subject_ptr0,mch::type_switch_info_offset_helper<is_polymorphic0,decltype(__switch_info)>::get_offset(__switch_info, polymorphic_index0));
								auto& match1 = *mch::adjust_ptr_if_xtl_polymorphic<target_type1>(subject_ptr1,mch::type_switch_info_offset_helper<is_polymorphic1,decltype(__switch_info)>::get_offset(__switch_info, polymorphic_index1));
								if (mch::filter(C<Q>())(match0) && mch::filter(C<N>())(match1)) 
								{
									++g_qn;
									break;
								}
							}
						}
						{
							typedef XTL_CPP0X_TYPENAME mch::underlying<decltype(mch::filter(C<R>()))>::type type_of_pattern0;
							static_assert(mch::is_pattern<type_of_pattern0>::value,"Case-clause expects patterns as its arguments");
							typedef XTL_CPP0X_TYPENAME mch::underlying<type_of_pattern0>::type:: accepted_type_for<source_type0>::type target_type0;
							typedef XTL_CPP0X_TYPENAME mch::underlying<decltype(mch::filter(C<N>()))>::type type_of_pattern1;
							static_assert(mch::is_pattern<type_of_pattern1>::value,"Case-clause expects patterns as its arguments");
							typedef XTL_CPP0X_TYPENAME mch::underlying<type_of_pattern1>::type:: accepted_type_for<source_type1>::type target_type1;
							if ((__casted_ptr0 = mch::dynamic_cast_when_polymorphic<const target_type0*>(subject_ptr0)) != 0 && (__casted_ptr1 = mch::dynamic_cast_when_polymorphic<const target_type1*>(subject_ptr1)) != 0) 
							{
								static_assert(number_of_subjects == 2, "Number of targets in the case clause must be the same as the number of subjects in the Match statement");
								enum 
								{
									target_label = 6 -__base_counter, is_inside_case_clause = 1 
								};
								if ((number_of_polymorphic_subjects))
									if ((__builtin_expect((long int)(__switch_info.target == 0), 1))) 
									{
										__switch_info.target = target_label;
										mch::type_switch_info_offset_helper<is_polymorphic0,decltype(__switch_info)>::set_offset(__switch_info, polymorphic_index0, intptr_t(__casted_ptr0)-intptr_t(subject_ptr0));
										mch::type_switch_info_offset_helper<is_polymorphic1,decltype(__switch_info)>::set_offset(__switch_info, polymorphic_index1, intptr_t(__casted_ptr1)-intptr_t(subject_ptr1));
									}
				case target_label:
								auto& match0 = *mch::adjust_ptr_if_xtl_polymorphic<target_type0>(subject_ptr0,mch::type_switch_info_offset_helper<is_polymorphic0,decltype(__switch_info)>::get_offset(__switch_info, polymorphic_index0));
								auto& match1 = *mch::adjust_ptr_if_xtl_polymorphic<target_type1>(subject_ptr1,mch::type_switch_info_offset_helper<is_polymorphic1,decltype(__switch_info)>::get_offset(__switch_info, polymorphic_index1));
								if (mch::filter(C<R>())(match0) && mch::filter(C<N>())(match1)) 
								{
									++g_rn;
									break;
								}
							}
						}
					}
					if ((number_of_polymorphic_subjects))
						if ((__builtin_expect((long int)((__switch_info.target == 0)), 0))) 
						{
							enum 
							{
								target_label = 7 -__base_counter 
							};
							mch::deferred_constant<mch::vtbl_count_t>::set<match_uid_type,(target_label-1)>::value_ptr;
							__switch_info.target = target_label;
				case target_label: 
							;
						}
				}
			}
#else
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
#endif
        }
		}
        clock_t t1 = clock();
        ans.mach7 = (t1 - t0);
    }

    {
        clock_t t0 = clock();
        for (std::vector<VP>::const_iterator pvp = vecp.begin(); pvp != vecp.end(); ++pvp)
		{
			const VP& vp = *pvp;

		for (std::vector<VM>::const_iterator pvm = vecm.begin(); pvm != vecm.end(); ++pvm)
        {
			const VM& vm = *pvm;
            struct increment : boost::static_visitor<>
            {
                void operator()(const P&, const M&) const { ++g_pm; }
                void operator()(const Q&, const M&) const { ++g_qm; }
                void operator()(const R&, const M&) const { ++g_rm; }
                void operator()(const P&, const N&) const { ++g_pn; }
                void operator()(const Q&, const N&) const { ++g_qn; }
                void operator()(const R&, const N&) const { ++g_rn; }
            };
            
            apply_visitor(increment(), vp, vm);
        }
		}
        clock_t t1 = clock();
        ans.visit = (t1 - t0);
    }

    if (!mach7_first)
    {
        clock_t t0 = clock();
        for (std::vector<VP>::const_iterator pvp = vecp.begin(); pvp != vecp.end(); ++pvp)
		{
			const VP& vp = *pvp;

		for (std::vector<VM>::const_iterator pvm = vecm.begin(); pvm != vecm.end(); ++pvm)
        {
			const VM& vm = *pvm;
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
            vec_vp.push_back(P(40));
        else if (i % 3 == 1)
            vec_vp.push_back(Q(41));
        else
            vec_vp.push_back(R(42));

        if (i % 2 == 0)
            vec_vm.push_back(M(53));
        else
            vec_vm.push_back(N(54));
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
    for (std::vector<result>::const_iterator p = results.begin(); p != results.end(); ++p)
	{
		const result& r = *p;
        std::cout << r.mach7 << "\t" << r.visit << std::endl;
	}
}
