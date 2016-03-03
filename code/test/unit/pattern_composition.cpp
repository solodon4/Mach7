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
/// This file is a part of Mach7 library test suite.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// \see https://parasol.tamu.edu/mach7/
/// \see https://github.com/solodon4/Mach7
/// \see https://github.com/solodon4/SELL
///

#include <mach7/patterns/all.hpp>
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
    const ref0<T> cr(n);
          ref0<T> mr(n);
    // Against modifiable subjects
    XTL_VERIFY(test(           values,cr)        == N);       // Pattern Non-modifiable l-value
    XTL_VERIFY(test(           values,mr)        == N);       // Pattern Modifiable l-value    
    XTL_VERIFY(test(           values,ref0<T>(n)) == N);       // Pattern R-value               
    // Against non-modifiable subjects
    XTL_VERIFY(test((const_arr)values,cr)        == N);       // Pattern Non-modifiable l-value
    XTL_VERIFY(test((const_arr)values,mr)        == N);       // Pattern Modifiable l-value    
    XTL_VERIFY(test((const_arr)values,ref0<T>(n)) == N);       // Pattern R-value               

    // Testing reference pattern for const T&
    // FIX: Perhaps ref0<const T&> should behave as a value pattern, but right
    //      now it fails to compile due to assignment to its bound const reference
    /*
    const ref0<const T&> crcr(n);
          ref0<const T&> crmr(n);
    XTL_VERIFY(test(values,crcr) == N);            // Pattern Non-modifiable l-value
    XTL_VERIFY(test(values,crmr) == N);            // Pattern Modifiable l-value    
    XTL_VERIFY(test(values,ref0<const T&>(n)) == N);// Pattern R-value               

    XTL_VERIFY((const T&)crcr == c);
    XTL_VERIFY((const T&)crmr == c);
    */
    // Testing reference pattern for T&
    // FIX: Weird why this is allowed to compiler since we form a & to a & in ref0<>::m_var
    const ref0<T&> mrcr(n);
          ref0<T&> mrmr(n);
    // Against modifiable subjects
    XTL_VERIFY(test(           values,mrcr) == N);            // Pattern Non-modifiable l-value
    XTL_VERIFY(test(           values,mrmr) == N);            // Pattern Modifiable l-value    
    XTL_VERIFY(test(           values,ref0<T&>(n)) == N);      // Pattern R-value               
    // Against non-modifiable subjects
    // Correctly fails to compile as ref0<T&> will try to bind for modification

    XTL_VERIFY((T&)mrcr == c);
    XTL_VERIFY((T&)mrmr == c);

    (T&)mrmr = a;
    XTL_VERIFY(n == a);
    (T&)mrcr = c;                                  // Change last element of values to 7
    XTL_VERIFY(n == c);
}

//------------------------------------------------------------------------------

template <typename T, std::size_t N, typename E1>
void test_equivalence(T (&values)[N], E1&& e1, std::size_t c)
{
    static_assert(mch::is_expression<E1>::value, "E1 must model LazyExpression concept");

    using namespace mch;

    // Testing equivalence pattern 
    const auto& ey1 = +e1;
          auto  ey2 = +e1;

    XTL_VERIFY(test(values,ey1) == c);      // Pattern Non-modifiable l-value
    XTL_VERIFY(test(values,ey2) == c);      // Pattern Modifiable l-value    
    XTL_VERIFY(test(values,+e1) == c);      // Pattern R-value               
}

//------------------------------------------------------------------------------

template <typename TT, std::size_t N>
void test_equivalence(TT (&values)[N])
{
    static_assert(N, "values must have at least 1 element");

    typedef typename const_array<TT (&)[N]>::type const_arr;
    typedef typename value_type<TT>::type T;
    const T& a = get(values,0);
    const T& b = get(values,N-1);
    const T c = a + b;

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // Testing equivalence pattern built out of modifiable var<T>
    mch::var<T> myv(b);
    mch::var<T> mnv(c);

    // Against modifiable subjects
    test_equivalence(           values,myv,1); // Pattern should be matched once
    test_equivalence(           values,mnv,0); // Pattern should not be matched
    // Against non-modifiable subjects
    test_equivalence((const_arr)values,myv,1); // Pattern should be matched once
    test_equivalence((const_arr)values,mnv,0); // Pattern should not be matched

    // Testing equivalence pattern built out of non-modifiable var<T>
    const mch::var<T> cyv(b);
    const mch::var<T> cnv(c);

    // Against modifiable subjects
    test_equivalence(           values,cyv,1); // Pattern should be matched once
    test_equivalence(           values,cnv,0); // Pattern should not be matched
    // Against non-modifiable subjects
    test_equivalence((const_arr)values,cyv,1); // Pattern should be matched once
    test_equivalence((const_arr)values,cnv,0); // Pattern should not be matched

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    T mb = b;
    T mc = c;

    // Testing equivalence pattern built out of modifiable var<T&>
    mch::var<T&> mrmyv;
    mch::var<T&> mrmnv;
    // We need to bind reference variables before we can evaluate them through equivalence pattern +v
    mrmyv(mb);
    mrmnv(mc);

    // Against modifiable subjects
    test_equivalence(           values,mrmyv,1); // Pattern should be matched once
    test_equivalence(           values,mrmnv,0); // Pattern should not be matched
    // Against non-modifiable subjects
    test_equivalence((const_arr)values,mrmyv,1); // Pattern should be matched once
    test_equivalence((const_arr)values,mrmnv,0); // Pattern should not be matched

    // Testing equivalence pattern built out of non-modifiable var<T&>
    const mch::var<T&> crmyv;
    const mch::var<T&> crmnv;
    // We need to bind reference variables before we can evaluate them through equivalence pattern +v
    crmyv(mb);
    crmnv(mc);

    // Against modifiable subjects
    test_equivalence(           values,crmyv,1); // Pattern should be matched once
    test_equivalence(           values,crmnv,0); // Pattern should not be matched
    // Against non-modifiable subjects
    test_equivalence((const_arr)values,crmyv,1); // Pattern should be matched once
    test_equivalence((const_arr)values,crmnv,0); // Pattern should not be matched

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // Testing equivalence pattern built out of modifiable var<const T&>
    mch::var<const T&> mrcyv;
    mch::var<const T&> mrcnv;
    // We need to bind reference variables before we can evaluate them through equivalence pattern +v
    mrcyv(mb);
    mrcnv(mc);

    // Against modifiable subjects
    test_equivalence(           values,mrcyv,1); // Pattern should be matched once
    test_equivalence(           values,mrcnv,0); // Pattern should not be matched
    // Against non-modifiable subjects
    test_equivalence((const_arr)values,mrcyv,1); // Pattern should be matched once
    test_equivalence((const_arr)values,mrcnv,0); // Pattern should not be matched

    // Testing equivalence pattern built out of non-modifiable var<const T&>
    const mch::var<const T&> crcyv;
    const mch::var<const T&> crcnv;
    // We need to bind reference variables before we can evaluate them through equivalence pattern +v
    crcyv(mb);
    crcnv(mc);

    // Against modifiable subjects
    test_equivalence(           values,crcyv,1); // Pattern should be matched once
    test_equivalence(           values,crcnv,0); // Pattern should not be matched
    // Against non-modifiable subjects
    test_equivalence((const_arr)values,crcyv,1); // Pattern should be matched once
    test_equivalence((const_arr)values,crcnv,0); // Pattern should not be matched

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // Testing equivalence pattern built out of modifiable value<T>
    mch::value<T> myi(b);
    mch::value<T> mni(c);

    // Against modifiable subjects
    test_equivalence(           values,myi,1);  // Pattern should be matched once
    test_equivalence(           values,mni,0);  // Pattern should not be matched
    // Against non-modifiable subjects
    test_equivalence((const_arr)values,myi,1);  // Pattern should be matched once
    test_equivalence((const_arr)values,mni,0);  // Pattern should not be matched

    // Testing equivalence pattern built out of non-modifiable value<T>
    const mch::value<T> cyi(b);
    const mch::value<T> cni(c);

    // Against modifiable subjects
    test_equivalence(           values,cyi,1);  // Pattern should be matched once
    test_equivalence(           values,cni,0);  // Pattern should not be matched
    // Against non-modifiable subjects
    test_equivalence((const_arr)values,cyi,1);  // Pattern should be matched once
    test_equivalence((const_arr)values,cni,0);  // Pattern should not be matched

    // Testing equivalence pattern built out of tempoary value<T>

    // Against modifiable subjects
    test_equivalence(           values,mch::value<T>(b),1); // Pattern should be matched once
    test_equivalence(           values,mch::value<T>(c),0); // Pattern should not be matched
    // Against non-modifiable subjects
    test_equivalence((const_arr)values,mch::value<T>(b),1); // Pattern should be matched once
    test_equivalence((const_arr)values,mch::value<T>(c),0); // Pattern should not be matched
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
