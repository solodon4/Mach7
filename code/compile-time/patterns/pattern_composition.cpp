///
/// \file
///
/// This file is a part of pattern matching testing suite.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of Mach7 library (http://parasol.tamu.edu/mach7/).
/// Copyright (C) 2011-2012 Texas A&M University.
/// All rights reserved.
///

#include "patterns/all.hpp"
#include <iostream>

//------------------------------------------------------------------------------

template <typename T, std::size_t N> T& get(T  (&t)[N], std::size_t i) { return  t[i]; }
template <typename T, std::size_t N> T& get(T* (&t)[N], std::size_t i) { return *t[i]; }

template <typename T> struct value_type     { typedef T type; };
template <typename T> struct value_type<T*> { typedef T type; };

template <typename T>           struct const_array           {}; // Intentionally no definition
template <typename T, size_t N> struct const_array<T (&)[N]> { typedef const T (&type)[N]; };
template <typename T, size_t N> struct const_array<T*(&)[N]> { typedef const T*(&type)[N]; };

//------------------------------------------------------------------------------

/// This function tries to match against modifiable subjects
template <typename T, std::size_t N, typename P>
std::size_t test(T (&t)[N], P&& p)
{
    static_assert(mch::is_pattern<P>::value, "P must model Pattern concept");

    std::size_t matches = 0;

    for (std::size_t i = 0; i < N; ++i)
        if (p(t[i])) // Check matching agains modifiable reference
            ++matches;

    return matches;
}

//------------------------------------------------------------------------------

/// This function tries to match against modifiable subjects
template <typename T, std::size_t N, typename P>
std::size_t test(T* (&t)[N], P&& p)
{
    static_assert(mch::is_pattern<P>::value, "P must model Pattern concept");

    std::size_t matches = 0;

    for (std::size_t i = 0; i < N; ++i)
        if (p(*t[i]))
            ++matches;

    return matches;
}

//------------------------------------------------------------------------------

template <typename TT, std::size_t N>
void test_primitive(TT (&values)[N])
{
    static_assert(N, "values must have at least 1 element");

    typedef typename const_array<TT (&)[N]>::type const_arr;
    typedef typename value_type<TT>::type T;
    const T& a = get(values,0);
    const T& b = get(values,N-1);
    const T  c = a + b;

    using namespace mch;

    // Testing wildcard pattern
    const wildcard w1;
          wildcard w2;
    // Against modifiable subjects
    XTL_VERIFY(test(           values,w1)         == N);      // Pattern is Non-modifiable l-value
    XTL_VERIFY(test(           values,w2)         == N);      // Pattern is Modifiable l-value    
    XTL_VERIFY(test(           values,wildcard()) == N);      // Pattern is R-value               
    // Against non-modifiable subjects
    XTL_VERIFY(test((const_arr)values,w1)         == N);      // Pattern Non-modifiable l-value
    XTL_VERIFY(test((const_arr)values,w2)         == N);      // Pattern Modifiable l-value    
    XTL_VERIFY(test((const_arr)values,wildcard()) == N);      // Pattern R-value               

    // Testing value pattern
    const value<T> ci(a);
          value<T> mi(b);
    // Against modifiable subjects
    XTL_VERIFY(test(           values,ci)          == 1);     // Pattern Non-modifiable l-value
    XTL_VERIFY(test(           values,mi)          == 1);     // Pattern Modifiable l-value
    XTL_VERIFY(test(           values,value<T>(a)) == 1);     // Pattern R-value
    // Against non-modifiable subjects
    XTL_VERIFY(test((const_arr)values,ci)          == 1);     // Pattern Non-modifiable l-value
    XTL_VERIFY(test((const_arr)values,mi)          == 1);     // Pattern Modifiable l-value
    XTL_VERIFY(test((const_arr)values,value<T>(a)) == 1);     // Pattern R-value

    // Testing variable pattern
    const var<T> cv;
          var<T> mv;
    // Against modifiable subjects
    XTL_VERIFY(test(           values,cv)       == N);        // Pattern Non-modifiable l-value
    XTL_VERIFY(test(           values,mv)       == N);        // Pattern Modifiable l-value    
    XTL_VERIFY(test(           values,var<T>()) == N);        // Pattern R-value               
    // Against non-modifiable subjects
    XTL_VERIFY(test((const_arr)values,cv)       == N);        // Pattern Non-modifiable l-value
    XTL_VERIFY(test((const_arr)values,mv)       == N);        // Pattern Modifiable l-value    
    XTL_VERIFY(test((const_arr)values,var<T>()) == N);        // Pattern R-value               

    // Testing variable pattern for const T&
    const var<const T&> crcv;
          var<const T&> crmv;
    // Against modifiable subjects
    XTL_VERIFY(test(           values,crcv)            == N); // Pattern Non-modifiable l-value
    XTL_VERIFY(test(           values,crmv)            == N); // Pattern Modifiable l-value    
    XTL_VERIFY(test(           values,var<const T&>()) == N); // Pattern R-value               
    // Against non-modifiable subjects
    XTL_VERIFY(test((const_arr)values,crcv)            == N); // Pattern Non-modifiable l-value
    XTL_VERIFY(test((const_arr)values,crmv)            == N); // Pattern Modifiable l-value    
    XTL_VERIFY(test((const_arr)values,var<const T&>()) == N); // Pattern R-value               

    // Testing variable pattern for T&
    const var<T&> mrcv;
          var<T&> mrmv;
    // Against modifiable subjects
    XTL_VERIFY(test(           values,mrcv)      == N);       // Pattern Non-modifiable l-value
    XTL_VERIFY(test(           values,mrmv)      == N);       // Pattern Modifiable l-value    
    XTL_VERIFY(test(           values,var<T&>()) == N);       // Pattern R-value               
    // Against non-modifiable subjects
    // Correctly fails to compile as var<T&> will try to bind for modification

    (T&)mrmv = a;
    XTL_VERIFY(get(values,N-1) == a);
    (T&)mrcv = c;                                  // Change last element of values to c
    XTL_VERIFY(get(values,N-1) == c);

    // Testing reference pattern. NOTE: This pattern in never explicitly created by the user!
              T   n;
    const ref<T> cr(n);
          ref<T> mr(n);
    // Against modifiable subjects
    XTL_VERIFY(test(           values,cr)        == N);       // Pattern Non-modifiable l-value
    XTL_VERIFY(test(           values,mr)        == N);       // Pattern Modifiable l-value    
    XTL_VERIFY(test(           values,ref<T>(n)) == N);       // Pattern R-value               
    // Against non-modifiable subjects
    XTL_VERIFY(test((const_arr)values,cr)        == N);       // Pattern Non-modifiable l-value
    XTL_VERIFY(test((const_arr)values,mr)        == N);       // Pattern Modifiable l-value    
    XTL_VERIFY(test((const_arr)values,ref<T>(n)) == N);       // Pattern R-value               

    // Testing reference pattern for const T&
    // FIX: Perhaps ref<const T&> should behave as a value pattern, but right
    //      now it fails to compile due to assignment to its bound const reference
    /*
    const ref<const T&> crcr(n);
          ref<const T&> crmr(n);
    XTL_VERIFY(test(values,crcr) == N);            // Pattern Non-modifiable l-value
    XTL_VERIFY(test(values,crmr) == N);            // Pattern Modifiable l-value    
    XTL_VERIFY(test(values,ref<const T&>(n)) == N);// Pattern R-value               

    XTL_VERIFY((const T&)crcr == c);
    XTL_VERIFY((const T&)crmr == c);
    */
    // Testing reference pattern for T&
    // FIX: Weird why this is allowed to compiler since we form a & to a & in ref<>::m_var
    const ref<T&> mrcr(n);
          ref<T&> mrmr(n);
    // Against modifiable subjects
    XTL_VERIFY(test(           values,mrcr) == N);            // Pattern Non-modifiable l-value
    XTL_VERIFY(test(           values,mrmr) == N);            // Pattern Modifiable l-value    
    XTL_VERIFY(test(           values,ref<T&>(n)) == N);      // Pattern R-value               
    // Against non-modifiable subjects
    // Correctly fails to compile as ref<T&> will try to bind for modification

    XTL_VERIFY((T&)mrcr == c);
    XTL_VERIFY((T&)mrmr == c);

    (T&)mrmr = a;
    XTL_VERIFY(n == a);
    (T&)mrcr = c;                                  // Change last element of values to 7
    XTL_VERIFY(n == c);
}

//------------------------------------------------------------------------------

template <typename T, std::size_t N, typename Ey, typename En>
void test_equivalence(T (&values)[N], Ey&& ey, En&& en)
{
    static_assert(mch::is_expression<Ey>::value, "Ey must model LazyExpression concept");
    static_assert(mch::is_expression<En>::value, "En must model LazyExpression concept");

    using namespace mch;

    // Testing equivalence pattern on present expression 
    const auto& ey1 = +ey;
          auto  ey2 = +ey;

    XTL_VERIFY(test(values,ey1) > 0);      // Pattern Non-modifiable l-value
    XTL_VERIFY(test(values,ey2) > 0);      // Pattern Modifiable l-value    
    XTL_VERIFY(test(values,+ey) > 0);      // Pattern R-value               

    // Testing equivalence pattern on absent expression 
    const auto& en1 = +en;
          auto  en2 = +en;

    XTL_VERIFY(test(values,en1) == 0);     // Pattern Non-modifiable l-value
    XTL_VERIFY(test(values,en2) == 0);     // Pattern Modifiable l-value    
    XTL_VERIFY(test(values,+en) == 0);     // Pattern R-value               
}

//------------------------------------------------------------------------------

template <typename TT, std::size_t N>
void test_equivalence(TT (&values)[N])
{
    static_assert(N, "values must have at least 1 element");

    typedef typename value_type<TT>::type T;
    const T& a = get(values,0);
    const T& b = get(values,N-1);
    const T c = a + b;

          mch::var<T> myv(b);
          mch::var<T> mnv(c);
    const mch::var<T> cyv(b);
    const mch::var<T> cnv(c);

    test_equivalence(values,myv,mnv);
    test_equivalence(values,cyv,cnv);

    T mb = b;
    T mc = c;

          mch::var<T&> mryv;
          mch::var<T&> mrnv;
    const mch::var<T&> cryv;
    const mch::var<T&> crnv;

    // We need to bind reference variables before we can evaluate them through equivalence pattern +v
    mryv(mb);
    mrnv(mc);
    cryv(mb);
    crnv(mc);

    test_equivalence(values,mryv,mrnv);
    test_equivalence(values,cryv,crnv);

          mch::value<T> myi(b);
          mch::value<T> mni(c);
    const mch::value<T> cyi(b);
    const mch::value<T> cni(c);

    test_equivalence(values,myi,mni);
    test_equivalence(values,cyi,cni);
    test_equivalence(values,mch::value<T>(b),mch::value<T>(c));
}

//------------------------------------------------------------------------------

template <typename TT, std::size_t N, typename P1, typename P2>
void test_combinators(TT (&values)[N], P1&& p1, P2&& p2, std::size_t nc, std::size_t nd, std::size_t nn1, std::size_t nn2)
{
    static_assert(mch::is_expression<P1>::value, "P1 must model Pattern concept");
    static_assert(mch::is_expression<P2>::value, "P2 must model Pattern concept");
    static_assert(N, "values must have at least 1 element");

    // Testing conjunction pattern
    const auto& c1 = p1 && p2;
          auto  c2 = p1 && p2;

    XTL_VERIFY(test(values,c1) == nc);      // Pattern Non-modifiable l-value
    XTL_VERIFY(test(values,c2) == nc);      // Pattern Modifiable l-value    
    XTL_VERIFY(test(values,p1 && p2) == nc);// Pattern R-value               

    // Testing disjunction pattern
    const auto& d1 = p1 || p2;
          auto  d2 = p1 || p2;

    XTL_VERIFY(test(values,d1) == nd);      // Pattern Non-modifiable l-value
    XTL_VERIFY(test(values,d2) == nd);      // Pattern Modifiable l-value    
    XTL_VERIFY(test(values,p1 || p2) == nd);// Pattern R-value               

    // Testing negation pattern
    const auto& n11 = !p1;
          auto  n12 = !p1;

    XTL_VERIFY(test(values,n11) == nn1);    // Pattern Non-modifiable l-value
    XTL_VERIFY(test(values,n12) == nn1);    // Pattern Modifiable l-value    
    XTL_VERIFY(test(values,!p1) == nn1);    // Pattern R-value               

    const auto& n21 = !p2;
          auto  n22 = !p2;

    XTL_VERIFY(test(values,n21) == nn2);    // Pattern Non-modifiable l-value
    XTL_VERIFY(test(values,n22) == nn2);    // Pattern Modifiable l-value    
    XTL_VERIFY(test(values,!p2) == nn2);    // Pattern R-value               
}

//------------------------------------------------------------------------------

template <typename TT, std::size_t N>
void test_combinators(TT (&values)[N])
{
    typedef typename value_type<TT>::type T;
    //const T& a = get(values,0);
    const T& b = get(values,N-1);
    //const T c = a + b;

    using namespace mch;

          mch::var<T>   mv;
    const mch::var<T>   cv;
          mch::value<T> mi(b);
    const mch::value<T> ci(b);

    test_combinators(values,mv,mv,N,N,0,0);
    test_combinators(values,mv,cv,N,N,0,0);
    test_combinators(values,cv,mv,N,N,0,0);
    test_combinators(values,cv,cv,N,N,0,0);

    test_combinators(values,mv,mi,1,N,0,N-1);
    test_combinators(values,mi,cv,1,N,N-1,0);
    test_combinators(values,cv,ci,1,N,0,N-1);
    test_combinators(values,ci,mv,1,N,N-1,0);
}

//------------------------------------------------------------------------------

#include <string>

//------------------------------------------------------------------------------

int main()
{
    // NOTE: The sume of first and last element should not be in the array!
    int         ints[]    = {-7,-1,0,1,2};
    double      doubles[] = {-2.57,-1.0,0,1.77,2,3.1415926};
    std::string strings[] = {"Hello","World"};

    test_primitive(ints);
    test_primitive(doubles);
    test_primitive(strings);

    test_equivalence(ints);
    test_equivalence(doubles);
    test_equivalence(strings);

    test_combinators(ints);
    test_combinators(doubles);
    test_combinators(strings);

    int*         int_ptrs[]    = {new int(-7),new int(-1),new int(0),new int(1),new int(2)};
    double*      double_ptrs[] = {new double(-2.57),new double(-1.0),new double(0),new double(1.77),new double(2),new double(3.1415926)};
    std::string* string_ptrs[] = {new std::string("Hello"),new std::string("World")};

    test_primitive(int_ptrs);
    test_primitive(double_ptrs);
    test_primitive(string_ptrs);

    test_equivalence(int_ptrs);
    test_equivalence(double_ptrs);
    test_equivalence(string_ptrs);

    test_combinators(int_ptrs);
    test_combinators(double_ptrs);
    test_combinators(string_ptrs);
}

//------------------------------------------------------------------------------
