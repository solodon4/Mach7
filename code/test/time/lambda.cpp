//
//  Mach7: Pattern Matching Library for C++
//
//  Copyright 2011-2013, Texas A&M University.
//  Copyright 2014 Yuriy Solodkyy.
//  All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//
//      * Redistributions of source code must retain the above copyright
//        notice, this list of conditions and the following disclaimer.
//
//      * Redistributions in binary form must reproduce the above copyright
//        notice, this list of conditions and the following disclaimer in the
//        documentation and/or other materials provided with the distribution.
//
//      * Neither the names of Mach7 project nor the names of its contributors
//        may be used to endorse or promote products derived from this software
//        without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
//  IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY
//  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
//  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

///
/// \file
///
/// An example of implementing Lambda Calculus interpreter in Mach7.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// \see https://parasol.tamu.edu/mach7/
/// \see https://github.com/solodon4/Mach7
/// \see https://github.com/solodon4/SELL
///

//------------------------------------------------------------------------------

#include <mach7/type_switchN-patterns.hpp> // Support for N-ary Match statement on patterns
#include <mach7/patterns/address.hpp>      // Address and dereference combinators
#include <mach7/patterns/bindings.hpp>     // Mach7 support for bindings on arbitrary UDT
#include <mach7/patterns/constructor.hpp>  // Support for constructor patterns
#include <mach7/patterns/equivalence.hpp>  // Equivalence combinator +
#include <mach7/patterns/primitive.hpp>    // Wildcard, variable and value patterns
#include "testutils.hpp"

//------------------------------------------------------------------------------

struct Term { virtual ~Term() {} };
struct Var : Term { std::string name;       Var(const char* n) : name(n) {} };
struct Abs : Term { Var*  var;  Term* body; Abs(Var*  v, Term* t) : var(v), body(t) {} };
struct App : Term { Term* func; Term* arg;  App(Term* f, Term* a) : func(f), arg(a) {} };

//------------------------------------------------------------------------------

namespace mch ///< Mach7 library namespace
{
template <> struct bindings<Var> { Members(Var::name); };
template <> struct bindings<Abs> { Members(Abs::var , Abs::body); };
template <> struct bindings<App> { Members(App::func, App::arg);  };
} // of namespace mch

//------------------------------------------------------------------------------

using namespace mch; // Enable use of pattern-matching constructs without namespace qualification

//------------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& os, const Term& t)
{
    std::string      s;
    var<const Var&>  v;
    var<const Term&> t1,t2;

    Match(t)
    {
    Case(C<Var>(s))       return os << s;
    Case(C<Abs>(&v,&t1))  return os << '\\' << v << '.' << t1;
    Case(C<App>(&t1,&t2)) return os << '(' << t1 << ')' << '(' << t2 << ')';
    }
    EndMatch

    return os; // To prevent all control path warning
}

//------------------------------------------------------------------------------

/// Substitutes every occurence of variable #v in #s with #t.
Term* substitute(const Term& /*s*/, const Var& /*v*/, const Term& /*t*/) { return nullptr; }

//------------------------------------------------------------------------------

Term* evaluate(Term* t)
{
    var<const Var&>  v;
    var<const Term&> t1,t2;

    Match(t)
    {
    Case(C<Var>()) return &match0;
    Case(C<Abs>()) return &match0;
  //Case(C<Var>() || C<Abs>()) return &match0; // This would be possible instead of above two when common_type of two derived classes will be defined in the language
    Case(C<App>(C<Abs>(&v,&t1),&t2)) 
        return evaluate(substitute(t1,v,t2));
    Otherwise() std::cerr << "Error: Invalid term";
    }
    EndMatch

    return nullptr;
}

//------------------------------------------------------------------------------

bool operator==(const Term&, const Term&);
inline bool operator!=(const Term& left, const Term& right) { return !(left == right); }

//------------------------------------------------------------------------------

bool operator==(const Term& left, const Term& right)
{
    //std::clog << "(" << left << ',' << right << ')' << std::endl;
    var<std::string> s;
    //var<const Var&>  v;
    var<const Term&> v,t,f;

    Match(left,right)
    {
    Case(C<Var>(s),     C<Var>(+s)     ) return true;
    Case(C<Abs>(&v,&t), C<Abs>(&+v,&+t)) return true;
    Case(C<App>(&f,&t), C<App>(&+f,&+t)) return true;
    Otherwise()                          return false;
    }
    EndMatch

    return false; // To prevent all control path warning
}

//bool operator==(const Term& left, const Term& right)
//{
//    var<std::string> s;
//    var<const Term&> v,t,f;
//    {
//        struct match_uid_type 
//        {
//        }
//        ;
//        enum 
//        {
//            is_inside_case_clause = 0, number_of_subjects = 2, polymorphic_index00 = -1, __base_counter = 5 
//        }
//        ;
//        auto&& subject_ref0 = left;
//        auto const subject_ptr0 = mch::addr(subject_ref0);
//        typedef  mch::underlying<decltype(*subject_ptr0)>::type source_type0;
//        typedef source_type0 target_type0;
//        ;
//        enum 
//        {
//            is_polymorphic0 = std::is_polymorphic<source_type0>::value, polymorphic_index0 = polymorphic_index00 + is_polymorphic0 
//        }
//        ;
//        auto& match0 = *subject_ptr0;
//        (void)match0;
//        ;
//        register const void* __casted_ptr0 = 0;
//        auto&& subject_ref1 = right;
//        auto const subject_ptr1 = mch::addr(subject_ref1);
//        typedef  mch::underlying<decltype(*subject_ptr1)>::type source_type1;
//        typedef source_type1 target_type1;
//        ;
//        enum 
//        {
//            is_polymorphic1 = std::is_polymorphic<source_type1>::value, polymorphic_index1 = polymorphic_index0 + is_polymorphic1 
//        }
//        ;
//        auto& match1 = *subject_ptr1;
//        (void)match1;
//        ;
//        register const void* __casted_ptr1 = 0;
//        enum 
//        {
//            number_of_polymorphic_subjects = is_polymorphic0 + is_polymorphic1 
//        }
//        ;
//        typedef mch::vtbl_map<number_of_polymorphic_subjects,mch::type_switch_info<number_of_polymorphic_subjects>> vtbl_map_type;
//        vtbl_map_type& __vtbl2case_map = mch::preallocated<vtbl_map_type,match_uid_type>::value;
//        mch::type_switch_info<number_of_polymorphic_subjects>& __switch_info = __vtbl2case_map.get(subject_ptr0 , subject_ptr1);
//        switch (number_of_polymorphic_subjects ? __switch_info.target :
//            0) 
//        {
//        default:
//            {
//                {
//                    {
//                        {
//                        }
//                    }
//                }
//                {
//                    typedef  mch::underlying<decltype(mch::filter(C<Var>(s)))>::type type_of_pattern0;
//                    static_assert(mch::is_pattern<type_of_pattern0>::value,"Case-clause expects patterns as its arguments");
//                    typedef  mch::underlying<type_of_pattern0>::type::
//                        accepted_type_for<source_type0>::type target_type0;
//                    typedef  mch::underlying<decltype(mch::filter(C<Var>(+s)))>::type type_of_pattern1;
//                    static_assert(mch::is_pattern<type_of_pattern1>::value,"Case-clause expects patterns as its arguments");
//                    typedef  mch::underlying<type_of_pattern1>::type::
//                        accepted_type_for<source_type1>::type target_type1;
//                    if ((__casted_ptr0 = mch::dynamic_cast_when_polymorphic<const target_type0*>(subject_ptr0)) != 0 && (__casted_ptr1 = mch::dynamic_cast_when_polymorphic<const target_type1*>(subject_ptr1)) != 0) 
//                    {
//                        static_assert(number_of_subjects == 2, "Number of targets in the case clause must be the same as the number of subjects in the Match statement");
//                        enum 
//                        {
//                            target_label = 6-__base_counter, is_inside_case_clause = 1 
//                        }
//                        ;
//                        __pragma(warning(push)) __pragma(warning( disable :                        4127 ))
//                            if ((number_of_polymorphic_subjects)) __pragma(warning(pop))
//                                if ((__switch_info.target == 0)) 
//                                {
//                                    __switch_info.target = target_label;
//                                    __pragma(warning(push)) __pragma(warning( disable :                                    4127 ))
//                                        if ((is_polymorphic0)) __pragma(warning(pop)) __switch_info.offset[polymorphic_index0] = intptr_t(__casted_ptr0)-intptr_t(subject_ptr0);
//                                    __pragma(warning(push)) __pragma(warning( disable :                                    4127 ))
//                                        if ((is_polymorphic1)) __pragma(warning(pop)) __switch_info.offset[polymorphic_index1] = intptr_t(__casted_ptr1)-intptr_t(subject_ptr1);
//                                }
//        case target_label:
//                        auto& match0 = *mch::adjust_ptr_if_polymorphic<target_type0>(subject_ptr0,__switch_info.offset[polymorphic_index0]);
//                        auto& match1 = *mch::adjust_ptr_if_polymorphic<target_type1>(subject_ptr1,__switch_info.offset[polymorphic_index1]);
//                        if (mch::filter(C<Var>(s))(match0) && mch::filter(C<Var>(+s))(match1)) 
//                        {
//                            return true;
//                        }
//                    }
//                }
//                {
//                    typedef  mch::underlying<decltype(mch::filter(C<Abs>(&v,&t)))>::type type_of_pattern0;
//                    static_assert(mch::is_pattern<type_of_pattern0>::value,"Case-clause expects patterns as its arguments");
//                    typedef  mch::underlying<type_of_pattern0>::type::
//                        accepted_type_for<source_type0>::type target_type0;
//                    typedef  mch::underlying<decltype(mch::filter(C<Abs>(&+v,&+t)))>::type type_of_pattern1;
//                    static_assert(mch::is_pattern<type_of_pattern1>::value,"Case-clause expects patterns as its arguments");
//                    typedef  mch::underlying<type_of_pattern1>::type::
//                        accepted_type_for<source_type1>::type target_type1;
//                    if ((__casted_ptr0 = mch::dynamic_cast_when_polymorphic<const target_type0*>(subject_ptr0)) != 0 && (__casted_ptr1 = mch::dynamic_cast_when_polymorphic<const target_type1*>(subject_ptr1)) != 0) 
//                    {
//                        static_assert(number_of_subjects == 2, "Number of targets in the case clause must be the same as the number of subjects in the Match statement");
//                        enum 
//                        {
//                            target_label = 7-__base_counter, is_inside_case_clause = 1 
//                        }
//                        ;
//                        __pragma(warning(push)) __pragma(warning( disable :                        4127 ))
//                            if ((number_of_polymorphic_subjects)) __pragma(warning(pop))
//                                if ((__switch_info.target == 0)) 
//                                {
//                                    __switch_info.target = target_label;
//                                    __pragma(warning(push)) __pragma(warning( disable :                                    4127 ))
//                                        if ((is_polymorphic0)) __pragma(warning(pop)) __switch_info.offset[polymorphic_index0] = intptr_t(__casted_ptr0)-intptr_t(subject_ptr0);
//                                    __pragma(warning(push)) __pragma(warning( disable :                                    4127 ))
//                                        if ((is_polymorphic1)) __pragma(warning(pop)) __switch_info.offset[polymorphic_index1] = intptr_t(__casted_ptr1)-intptr_t(subject_ptr1);
//                                }
//        case target_label:
//                        auto& match0 = *mch::adjust_ptr_if_polymorphic<target_type0>(subject_ptr0,__switch_info.offset[polymorphic_index0]);
//                        auto& match1 = *mch::adjust_ptr_if_polymorphic<target_type1>(subject_ptr1,__switch_info.offset[polymorphic_index1]);
//                        if (mch::filter(C<Abs>(&v,&t))(match0) && mch::filter(C<Abs>(&+v,&+t))(match1)) 
//                        {
//                            return true;
//                        }
//                    }
//                }
//                {
//                    typedef  mch::underlying<decltype(mch::filter(C<App>(&f,&t)))>::type type_of_pattern0;
//                    static_assert(mch::is_pattern<type_of_pattern0>::value,"Case-clause expects patterns as its arguments");
//                    typedef  mch::underlying<type_of_pattern0>::type::
//                        accepted_type_for<source_type0>::type target_type0;
//                    typedef  mch::underlying<decltype(mch::filter(C<App>(&+f,&+t)))>::type type_of_pattern1;
//                    static_assert(mch::is_pattern<type_of_pattern1>::value,"Case-clause expects patterns as its arguments");
//                    typedef  mch::underlying<type_of_pattern1>::type::
//                        accepted_type_for<source_type1>::type target_type1;
//                    if ((__casted_ptr0 = mch::dynamic_cast_when_polymorphic<const target_type0*>(subject_ptr0)) != 0 && (__casted_ptr1 = mch::dynamic_cast_when_polymorphic<const target_type1*>(subject_ptr1)) != 0) 
//                    {
//                        static_assert(number_of_subjects == 2, "Number of targets in the case clause must be the same as the number of subjects in the Match statement");
//                        enum 
//                        {
//                            target_label = 8-__base_counter, is_inside_case_clause = 1 
//                        }
//                        ;
//                        __pragma(warning(push)) __pragma(warning( disable :                        4127 ))
//                            if ((number_of_polymorphic_subjects)) __pragma(warning(pop))
//                                if ((__switch_info.target == 0)) 
//                                {
//                                    __switch_info.target = target_label;
//                                    __pragma(warning(push)) __pragma(warning( disable :                                    4127 ))
//                                        if ((is_polymorphic0)) __pragma(warning(pop)) __switch_info.offset[polymorphic_index0] = intptr_t(__casted_ptr0)-intptr_t(subject_ptr0);
//                                    __pragma(warning(push)) __pragma(warning( disable :                                    4127 ))
//                                        if ((is_polymorphic1)) __pragma(warning(pop)) __switch_info.offset[polymorphic_index1] = intptr_t(__casted_ptr1)-intptr_t(subject_ptr1);
//                                }
//        case target_label:
//                        auto& match0 = *mch::adjust_ptr_if_polymorphic<target_type0>(subject_ptr0,__switch_info.offset[polymorphic_index0]);
//                        auto& match1 = *mch::adjust_ptr_if_polymorphic<target_type1>(subject_ptr1,__switch_info.offset[polymorphic_index1]);
//                        if (mch::filter(C<App>(&f,&t))(match0) && mch::filter(C<App>(&+f,&+t))(match1)) 
//                        {
//                            return true;
//                            static_assert(is_inside_case_clause, "Otherwise() must follow actual clauses! If you are trying to use it as a default sub-clause, use When() instead");
//                        }
//                    }
//                }
//                {
//                    {
//                        {
//                            enum 
//                            {
//                                target_label = 9-__base_counter, is_inside_case_clause = 1 
//                            }
//                            ;
//                            if ((__switch_info.target == 0)) __switch_info.target = target_label;
//        case target_label:
//                            return false;
//                        }
//                    }
//                }
//            }
//            __pragma(warning(push)) __pragma(warning( disable :            4127 ))
//                if ((number_of_polymorphic_subjects)) __pragma(warning(pop))
//                    if (((__switch_info.target == 0))) 
//                    {
//                        enum 
//                        {
//                            target_label = 10-__base_counter 
//                        }
//                        ;
//                        mch::deferred_constant<mch::vtbl_count_t>::set<match_uid_type,(target_label-1)>::value_ptr;
//                        __switch_info.target = target_label;
//        case target_label:
//            ;
//                    }
//        }
//    }
//    return false;
//}

bool equal_terms(const Term& left, const Term& right)
{
    if (typeid(left) != typeid(right))
        return false;

    if (typeid(left) == typeid(Var))
    {
        return static_cast<const Var&>(left).name == static_cast<const Var&>(right).name;
    }
    else
    if (typeid(left) == typeid(Abs))
    {
        const Abs& l = static_cast<const Abs&>(left);
        const Abs& r = static_cast<const Abs&>(right);

        return equal_terms(*l.var, *r.var) 
            && equal_terms(*l.body,*r.body);
    }
    else
    if (typeid(left) == typeid(App))
    {
        const App& l = static_cast<const App&>(left);
        const App& r = static_cast<const App&>(right);

        return equal_terms(*l.func,*r.func) 
            && equal_terms(*l.arg, *r.arg);
    }

    XTL_UNREACHABLE; // To avoid warning that control may reach end of a non-void function
}

//------------------------------------------------------------------------------

Term* random_term(int n)
{
    static Var* variables[] = {new Var("a"), new Var("b"), new Var("c"), new Var("d"), new Var("e"), new Var("f")};
    const int NN = XTL_ARR_SIZE(variables);
    Var* v = variables[rand()%NN];

    if (n < 3)
    {
        switch (n)
        {
        case 0: return v;
        case 1: return new Abs(v,v);
        case 2: return new App(new Abs(v,v),variables[rand()%NN]);
        }
    }
    else
    {
        switch (n % 3)
        {
        case 0: return v;
        case 1: return new Abs(v,random_term(n/3));
        case 2: return new App(random_term(n/3),random_term(n/3));
        }
    }

    XTL_UNREACHABLE; // To avoid warning that control may reach end of a non-void function
}

//------------------------------------------------------------------------------

inline size_t compare_terms1(Term* left, Term* right) { return equal_terms(*left, *right); }
inline size_t compare_terms2(Term* left, Term* right) { return *left == *right; }

//------------------------------------------------------------------------------
 
int main()
{
    std::vector<Term*> arguments(N);

    for (size_t i = 0; i < N; ++i)
    {
        arguments[i] = random_term(rand()%1000);
        //std::cout << *arguments[i] << std::endl;
    }
    /*
    for (size_t i = 0; i < N; ++i)
    {
        for (size_t j = 0; j < N; ++j)
            std::cout << compare_terms1(arguments[i],arguments[j]);

        std::cout << "\t" << *arguments[i] << std::endl;
    }
    return 0;
    */
    verdict v = get_timings2<size_t,Term*,compare_terms1,compare_terms2>(arguments);
    std::cout << "Verdict: \t" << v << std::endl;
}

//------------------------------------------------------------------------------
