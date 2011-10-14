///
/// \file match.hpp
///
/// This file defines the necessary infrastructure for ML-like pattern matching
/// against trees in C++.
///
/// \author Yuriy Solodkyy
/// Copyright (C) 2011, Texas A&M University.  All rights reserved.
///

/// FIX: Big question: should null pointers match pointer variables???

#pragma once

#include <cassert>
#include <ostream>
#include "exprtmpl.hpp"

#ifdef _DEBUG
#include <typeinfo>
#include <iostream>
#endif

//------------------------------------------------------------------------------

#ifdef _DEBUG
#define DEBUG_ONLY(s) s
#else
#define DEBUG_ONLY(s)
#endif

//------------------------------------------------------------------------------

#define CM(Index,...) static decltype(&__VA_ARGS__) member##Index() { return &__VA_ARGS__; } // We need this to handle template as those might have commas

//------------------------------------------------------------------------------

/// Traits like structure that will define which members should be matched at 
/// which positions. It is intentionally left undefined as user will have to
/// provide specializations for his hierarchy.
template <typename type_being_matched> struct match_members;

template <typename type_being_matched, int kind_being_matched> struct match_members_ex;

//------------------------------------------------------------------------------
template <class T>
struct variable;

template <typename T, typename E>
struct guard
{
    guard(const variable<T>& v, const E& e) : m_v(v), m_e(e) {}
    template <typename U>
    bool operator()(const U& u) const { return m_v(u) && eval(m_e); }
    const variable<T>& m_v;
    const E&           m_e;
};

//------------------------------------------------------------------------------

/// A reference to a user provided variable
template <class T>
struct var_ref
{
    var_ref(T& var) : m_var(&var) {}
   ~var_ref() {}

    /// We report that matching succeeded and bind the value
    bool operator()(const T& t) const 
    {
        *m_var = t;
        return true;
    }

    /// Member that will hold matching value in case of successful matching
    T* m_var;
};

//------------------------------------------------------------------------------

/// Variable binding for a value type
template <class T>
struct variable
{
    variable() : m_value() {}
    /// We report that matching succeeded and bind the value
    bool operator()(const T& t) const 
    {
        m_value = t;
        return true;
    }
    template <typename E> guard<T,E> operator|(const E& e) const { return guard<T,E>(*this,e); }

    /// Helper conversion operator to let the variable be used in some places
    /// where T was allowed
    operator const T&() const { return m_value; }

    /// Member that will hold matching value in case of successful matching
    mutable T m_value;
};

//------------------------------------------------------------------------------

/// Variable binding for a pointer type
template <class T>
struct variable<const T*>
{
    variable() : m_value() {}
    /// We may be applied to a value of a base type, so first we have to figure
    /// out whether they dynamic type is actually T. We report match only if it is
    template <typename U>
    bool operator()(const U* u) const 
    {
        if (const T* t = dynamic_cast<const T*>(u))
        {
            m_value = t; 
            return true;
        }
        else
            return false;
    }
    template <typename E> guard<const T*,E> operator|(const E& e) const { return guard<const T*,E>(*this,e); }

    /// This distinguishes the case when type of the variable matches type of the member
    bool operator()(const T* t) const 
    {
        // NOTE: This will also assign the null pointer. Should it?
        m_value = t; 
        return true;
    }

    /// Helper conversion operator to let the variable be used in some places
    /// where T* is expected
    operator const T*() const { return m_value; }

    /// A helper member to let the wrapping variable be used as a pointer
    const T* operator->() const { return m_value; }

    /// A helper member to let the wrapping variable be used as a pointer
    const T& operator*()  const { return *m_value; }

    /// Member that will hold matching value in case of successful matching
    mutable const T* m_value;
};

//------------------------------------------------------------------------------

/// Variable binding for a pointer type
template <class T>
struct variable<const T&>
{
    variable() : m_value() {}

    /// We may be applied to a value of a base type, so first we have to figure
    /// out whether they dynamic type is actually T. We report match only if it is
    template <typename U>
    bool operator()(const U& u) const 
    {
        if (const T* t = dynamic_cast<const T*>(&u))
        {
            m_value = t; 
            return true;
        }
        else
            return false;
    }
    template <typename E> guard<const T&,E> operator|(const E& e) const { return guard<const T&,E>(*this,e); }

    /// This distinguishes the case when type of the variable matches type of the member
    bool operator()(const T& t) const 
    {
        // NOTE: This will also assign the null pointer. Should it?
        m_value = &t;
        return true;
    }

    /// Helper conversion operator to let the variable be used in some places
    /// where T was allowed
    operator const T&() const { assert(m_value); return *m_value; }

    /// Member that will hold matching value in case of successful matching
    mutable const T* m_value;
};

//------------------------------------------------------------------------------

/// Specialization that describes a meta variable that matches everything.
/// This variable will match any type returned by a member, but the main 
/// difference from just using a variable whose value is ignored is that 
/// use of this variable will make sure the actual member is never invoked!
template <>
struct variable<void>
{
    template <typename E> guard<void,E> operator|(const E& e) const { return guard<void,E>(*this,e); }
    // NOTE: We don't need the below application anymore since we have a 
    //       specialization that never applies the actual member before
    //       passing it to this meta variable that matches everything.
    //template <typename U>
    //bool operator()(const U& u) const { return true; }
};

//------------------------------------------------------------------------------

template <typename T>
std::ostream& operator<<(std::ostream& os, const variable<T>& v) { return os << v.m_value; }

template <typename T>
std::ostream& operator<<(std::ostream& os, const variable<T*>& v) { return os << *v.m_value; }

//------------------------------------------------------------------------------

template <class T>
struct value
{
    value(const T& t) : m_value(t) {}
    bool operator()(const T& t) const { return m_value == t; }
    T m_value;
};

template <class T> value<T> val(const T& t) { return value<T>(t); }

//------------------------------------------------------------------------------

//#define FOR_EACH_BINARY_OPERATOR(F,S) template <typename T, typename E> expr<F,variable<T>,E> XTL_CONCATENATE(operator,S)(const variable<T>& v, const E& e) { return expr<F,variable<T>,E>(v,e); }
//#include "loop_over_operators.hpp"
//#undef  FOR_EACH_BINARY_OPERATOR

template <typename T> T eval(const value<T>& e)    { return e.m_value; }
template <typename T> T eval(const variable<T>& e) { return e.m_value; }
//template <typename F, typename E1>              auto eval(const expr<F,E1>&    e) -> decltype(F()(eval(e.m_e1)))              { return F()(eval(e.m_e1)); }
//template <typename F, typename E1, typename E2> auto eval(const expr<F,E1,E2>& e) -> decltype(F()(eval(e.m_e1),eval(e.m_e2))) { return F()(eval(e.m_e1),eval(e.m_e2)); }
template <typename T> bool eval(T&) { return true; } // FIX: Just for test

//------------------------------------------------------------------------------

template <class C, class T, typename R>
R apply_member(const C* c, R (T::*method)() const)
{
    DEBUG_ONLY(std::clog << "\nApplying const member function to instance " << c << " of type " << typeid(*c).name() << std::endl);
    return (c->*method)();
}

//------------------------------------------------------------------------------

template <class C, class T, typename R>
R apply_member(      C* c, R (T::*method)()      )
{
    DEBUG_ONLY(std::clog << "\nApplying non-const member function to instance " << c << " of type " << typeid(*c).name() << std::endl);
    return (c->*method)();
}

//------------------------------------------------------------------------------

template <class C, class T, typename R>
R apply_member(const C* c, R T::*field)
{
    DEBUG_ONLY(std::clog << "\nApplying data member to instance " << c << " of type " << typeid(*c).name() << std::endl);
    return c->*field;
}

//------------------------------------------------------------------------------

template <class C, class T, typename R>
R apply_member(const C* c, R (*func)(const T*))
{
    DEBUG_ONLY(std::clog << "\nApplying external function to const instance " << c << " of type " << typeid(*c).name() << std::endl);
    return (*func)(c);
}

//------------------------------------------------------------------------------

template <class C, class T, typename R>
R apply_member(      C* c, R (*func)(      T*))
{
    DEBUG_ONLY(std::clog << "\nApplying external function to non-const instance " << c << " of type " << typeid(*c).name() << std::endl);
    return (*func)(c);
}

//------------------------------------------------------------------------------

/// We need this extra indirection to be able to intercept when we are trying to 
/// match a meta variable _ of type variable<void>, that matches everything of
/// any type. In this case we don't even want to invoke the underlain member!
template <typename E, typename C, typename M>
bool apply_expression(const E& e, const C* c, M m)
{
    #ifdef _MSC_VER
    #pragma warning( disable : 4800 )
    #endif

    return c && e(apply_member(c, m));
}

template <typename E, typename C, typename M>
bool apply_expression(const E& e,       C* c, M m)
{
    #ifdef _MSC_VER
    #pragma warning( disable : 4800 )
    #endif

    return c && e(apply_member(c, m));
}

/// This is the specialization that makes the member not to be invoked when we
/// are matching against the meta variable _ that matches everything.
template <typename C, typename M>
bool apply_expression(const variable<void>& e, const C* c, M m)
{
    return true;
}

//------------------------------------------------------------------------------

template <typename T>
struct matcher0
{
    const T* operator()(const T* t) const { return t; }
          T* operator()(      T* t) const { return t; }
    template <typename U> const T* operator()(const U* u) const { return operator()(dynamic_cast<const T*>(u)); }
    template <typename U>       T* operator()(      U* u) const { return operator()(dynamic_cast<      T*>(u)); }
    template <typename U> const T* operator()(const U& u) const { return operator()(&u); }
    template <typename U>       T* operator()(      U& u) const { return operator()(&u); }

    template <typename E> expr_or<matcher0,E> operator|(const E& e) const { return expr_or<matcher0,E>(*this,e); }
};

//------------------------------------------------------------------------------

template <typename T, typename E1>
struct matcher1
{
    matcher1(const E1& e1) : m_e1(e1) {}
    /*
    T* apply(T* t) const 
    {
        if ( !apply_expression(m_e1, t, match_members<T>::member0()) )
            t = 0;

        return t;
    }
    const T* operator()(const T* t) const { return apply(t); }
          T* operator()(      T* t) const { return apply(t); } 
    */
    const T* operator()(const T* t) const 
    {
        if ( !apply_expression(m_e1, t, match_members<T>::member0()) )
            t = 0;

        return t;
    }
          T* operator()(      T* t) const 
    {
        if ( !apply_expression(m_e1, t, match_members<T>::member0()) )
            t = 0;

        return t;
    }

    template <typename U> const T* operator()(const U* u) const { return operator()(dynamic_cast<const T*>(u)); }
    template <typename U>       T* operator()(      U* u) const { return operator()(dynamic_cast<      T*>(u)); }
    template <typename U> const T* operator()(const U& u) const { return operator()(&u); }
    template <typename U>       T* operator()(      U& u) const { return operator()(&u); }

    template <typename E> expr_or<matcher1,E> operator|(const E& e) const { return expr_or<matcher1,E>(*this,e); }
    const E1& m_e1;
};

//------------------------------------------------------------------------------

template <typename T, typename E1, typename E2>
struct matcher2
{
    matcher2(const E1& e1, const E2& e2) : m_e1(e1), m_e2(e2) {}
    /*
    T* apply(T* t) const 
    {
        if ( !apply_expression(m_e1, t, match_members<T>::member0()) ||
             !apply_expression(m_e2, t, match_members<T>::member1())
            )
            t = 0;

        return t;
    }
    const T* operator()(const T* t) const { return apply(t); }
          T* operator()(      T* t) const { return apply(t); } 
    */
    const T* operator()(const T* t) const 
    {
        if ( !apply_expression(m_e1, t, match_members<T>::member0()) ||
             !apply_expression(m_e2, t, match_members<T>::member1())
            )
            t = 0;

        return t;
    }
          T* operator()(      T* t) const 
    {
        if ( !apply_expression(m_e1, t, match_members<T>::member0()) ||
             !apply_expression(m_e2, t, match_members<T>::member1())
            )
            t = 0;

        return t;
    }

    template <typename U> const T* operator()(const U* u) const { return operator()(dynamic_cast<const T*>(u)); }
    template <typename U>       T* operator()(      U* u) const { return operator()(dynamic_cast<      T*>(u)); }
    template <typename U> const T* operator()(const U& u) const { return operator()(&u); }
    template <typename U>       T* operator()(      U& u) const { return operator()(&u); }

    template <typename E> expr_or<matcher2,E> operator|(const E& e) const { return expr_or<matcher2,E>(*this,e); }
    const E1& m_e1;
    const E2& m_e2;
};

//------------------------------------------------------------------------------

template <typename T, typename E1, typename E2, typename E3>
struct matcher3
{
    matcher3(const E1& e1, const E2& e2, const E3& e3) : m_e1(e1), m_e2(e2), m_e3(e3) {}
    /*
    T* apply(T* t) const
    {
        if ( !apply_expression(m_e1, t, match_members<T>::member0()) ||
             !apply_expression(m_e2, t, match_members<T>::member1()) ||
             !apply_expression(m_e3, t, match_members<T>::member2())
            )
            t = 0;

        return t;
    }
    const T* operator()(const T* t) const { return apply(t); }
          T* operator()(      T* t) const { return apply(t); } 
    */

    const T* operator()(const T* t) const 
    {
        if ( !apply_expression(m_e1, t, match_members<T>::member0()) ||
             !apply_expression(m_e2, t, match_members<T>::member1()) ||
             !apply_expression(m_e3, t, match_members<T>::member2())
            )
            t = 0;

        return t;
    }
          T* operator()(      T* t) const 
    {
        if ( !apply_expression(m_e1, t, match_members<T>::member0()) ||
             !apply_expression(m_e2, t, match_members<T>::member1()) ||
             !apply_expression(m_e3, t, match_members<T>::member2())
            )
            t = 0;

        return t;
    }

    template <typename U> const T* operator()(const U* u) const { return operator()(dynamic_cast<const T*>(u)); }
    template <typename U>       T* operator()(      U* u) const { return operator()(dynamic_cast<      T*>(u)); }
    template <typename U> const T* operator()(const U& u) const { return operator()(&u); }
    template <typename U>       T* operator()(      U& u) const { return operator()(&u); }

    template <typename E> expr_or<matcher3,E> operator|(const E& e) const { return expr_or<matcher3,E>(*this,e); }
    const E1& m_e1;
    const E2& m_e2;
    const E3& m_e3;
};

//------------------------------------------------------------------------------

template <typename T, typename E1, typename E2, typename E3, typename E4>
struct matcher4
{
    matcher4(const E1& e1, const E2& e2, const E3& e3, const E4& e4) : m_e1(e1), m_e2(e2), m_e3(e3), m_e4(e4) {}
    /*
    T* apply(T* t) const 
    {
        if ( !apply_expression(m_e1, t, match_members<T>::member0()) ||
             !apply_expression(m_e2, t, match_members<T>::member1()) ||
             !apply_expression(m_e3, t, match_members<T>::member2()) ||
             !apply_expression(m_e4, t, match_members<T>::member3())
            )
            t = 0;

        return t;
    }

    const T* operator()(const T* t) const { return apply(t); }
          T* operator()(      T* t) const { return apply(t); } 
    */

    const T* operator()(const T* t) const 
    {
        if ( !apply_expression(m_e1, t, match_members<T>::member0()) ||
             !apply_expression(m_e2, t, match_members<T>::member1()) ||
             !apply_expression(m_e3, t, match_members<T>::member2()) ||
             !apply_expression(m_e4, t, match_members<T>::member3())
            )
            t = 0;

        return t;
    }
          T* operator()(      T* t) const 
    {
        if ( !apply_expression(m_e1, t, match_members<T>::member0()) ||
             !apply_expression(m_e2, t, match_members<T>::member1()) ||
             !apply_expression(m_e3, t, match_members<T>::member2()) ||
             !apply_expression(m_e4, t, match_members<T>::member3())
            )
            t = 0;

        return t;
    }

    template <typename U> const T* operator()(const U* u) const { return operator()(dynamic_cast<const T*>(u)); }
    template <typename U>       T* operator()(      U* u) const { return operator()(dynamic_cast<      T*>(u)); }
    template <typename U> const T* operator()(const U& u) const { return operator()(&u); }
    template <typename U>       T* operator()(      U& u) const { return operator()(&u); }

    template <typename E> expr_or<matcher4,E> operator|(const E& e) const { return expr_or<matcher4,E>(*this,e); }
    const E1& m_e1;
    const E2& m_e2;
    const E3& m_e3;
    const E4& m_e4;
};

//------------------------------------------------------------------------------

template <typename T> struct flt           { typedef T type; };
template <typename T> struct flt<const T>  { typedef T type; };
template <typename T> struct flt<variable<T>&> { typedef variable<T> type; };
template <typename T> struct flt<      T&> { typedef var_ref<T> type; };
template <typename T> struct flt<const T&> { typedef T type; };

template <typename T>       var_ref<T>   filter(T& t)        { return var_ref<T>(t); }
template <typename T>       variable<T>& filter(variable<T>& t)        { return t; }
//template <typename T> T&& filter(T&& t)       { return std::forward<T>(t); }
template <typename T> const typename flt<T>::type& filter(const T& t)  { return t; }
//template <typename T> const typename flt<T>::type& filter(const T&& t) { return t; }

//------------------------------------------------------------------------------

template <typename T>
matcher0<T> match()
{
    return matcher0<T>();
}
//#define match_ex match
//------------------------------------------------------------------------------

template <typename T, typename E1>
matcher1<T
        ,typename flt<E1>::type
> match_ex(E1&& e1)
//matcher1<T,E1> match_ex(E1&& e1)
{
    return matcher1<T,typename flt<E1>::type>(std::forward<E1>(e1));
}

template <typename T, typename E1>
matcher1<T,typename flt<E1>::type> match(E1&& e1)
{
    return match_ex<T>(filter(std::forward<E1>(e1)));
}

//template <typename T, typename E1>
//auto match(E1&& e1) -> decltype(match_ex<T>(filter(std::forward<E1>(e1))))
//{
//    return match_ex<T>(filter(std::forward<E1>(e1)));
//}

//------------------------------------------------------------------------------

template <typename T, typename E1, typename E2>
matcher2<T
        ,typename flt<E1>::type
        ,typename flt<E2>::type
> match_ex(E1&& e1, E2&& e2)
//matcher2<T,E1,E2> match_ex(const E1& e1, const E2& e2)
{
    return matcher2<T,typename flt<E1>::type,typename flt<E2>::type>(std::forward<E1>(e1), std::forward<E2>(e2));
}

template <typename T, typename E1, typename E2>
matcher2<T,typename flt<E1>::type,typename flt<E2>::type> match(E1&& e1, E2&& e2)
{
    return match_ex<T>(filter(std::forward<E1>(e1)), filter(std::forward<E2>(e2)));
}

//template <typename T, typename E1, typename E2>
//auto match(E1&& e1, E2&& e2) -> decltype(match_ex<T>(filter(std::forward<E1>(e1)), filter(std::forward<E2>(e2))))
//{
//    return match_ex<T>(filter(std::forward<E1>(e1)), filter(std::forward<E2>(e2)));
//}

//------------------------------------------------------------------------------

template <typename T, typename E1, typename E2, typename E3>
matcher3<T,typename flt<E1>::type,typename flt<E2>::type,typename flt<E3>::type> match_ex(E1&& e1, E2&& e2, E3&& e3)
//matcher3<T,E1,E2,E3> match_ex(E1&& e1, E2&& e2, E3&& e3)
{
    return matcher3<T,typename flt<E1>::type,typename flt<E2>::type,typename flt<E3>::type>(std::forward<E1>(e1), std::forward<E2>(e2), std::forward<E3>(e3));
}

template <typename T, typename E1, typename E2, typename E3>
matcher3<T,typename flt<E1>::type,typename flt<E2>::type,typename flt<E3>::type> match(E1&& e1, E2&& e2, E3&& e3)
{
    return match_ex<T>(filter(std::forward<E1>(e1)), filter(std::forward<E2>(e2)), filter(std::forward<E3>(e3)));
}

//template <typename T, typename E1, typename E2, typename E3>
//auto match(E1&& e1, E2&& e2, E3&& e3) -> decltype(match_ex<T>(filter(std::forward<E1>(e1)), filter(std::forward<E2>(e2)), filter(std::forward<E3>(e3))))
//{
//    return match_ex<T>(filter(std::forward<E1>(e1)), filter(std::forward<E2>(e2)), filter(std::forward<E3>(e3)));
//}

//------------------------------------------------------------------------------

template <typename T, typename E1, typename E2, typename E3, typename E4>
matcher4<T,typename flt<E1>::type,typename flt<E2>::type,typename flt<E3>::type,typename flt<E4>::type> match_ex(E1&& e1, E2&& e2, E3&& e3, E4&& e4)
//matcher4<T,E1,E2,E3,E4> match_ex(E1&& e1, E2&& e2, E3&& e3, E4&& e4)
{
    return matcher4<T,typename flt<E1>::type,typename flt<E2>::type,typename flt<E3>::type,typename flt<E4>::type>(std::forward<E1>(e1), std::forward<E2>(e2), std::forward<E3>(e3), std::forward<E4>(e4));
}

template <typename T, typename E1, typename E2, typename E3, typename E4>
matcher4<T,typename flt<E1>::type,typename flt<E2>::type,typename flt<E3>::type,typename flt<E4>::type> match(E1&& e1, E2&& e2, E3&& e3, E4&& e4)
{
    return match_ex<T>(filter(std::forward<E1>(e1)), filter(std::forward<E2>(e2)), filter(std::forward<E3>(e3)), filter(std::forward<E4>(e4)));
}

//template <typename T, typename E1, typename E2, typename E3, typename E4>
//auto match(E1&& e1, E2&& e2, E3&& e3, E4&& e4) -> decltype(match_ex<T>(filter(std::forward<E1>(e1)), filter(std::forward<E2>(e2)), filter(std::forward<E3>(e3)), filter(std::forward<E4>(e4))))
//{
//    return match_ex<T>(filter(std::forward<E1>(e1)), filter(std::forward<E2>(e2)), filter(std::forward<E3>(e3)), filter(std::forward<E4>(e4)));
//}

//------------------------------------------------------------------------------




































//------------------------------------------------------------------------------

template <typename T, int N>
struct matcher0ex
{
    const T* operator()(const T* t) const { return match<T>(value<int>(N))(t); }
          T* operator()(      T* t) const { return match<T>(value<int>(N))(t); }
    template <typename U> const T* operator()(const U* u) const { return operator()(dynamic_cast<const T*>(u)); }
    template <typename U>       T* operator()(      U* u) const { return operator()(dynamic_cast<      T*>(u)); }
    template <typename U> const T* operator()(const U& u) const { return operator()(&u); }
    template <typename U>       T* operator()(      U& u) const { return operator()(&u); }
};

//------------------------------------------------------------------------------

template <typename T, int N, typename E1>
struct matcher1ex
{
    matcher1ex(const E1& e1) : m_e1(e1) {}

    const T* operator()(const T* t) const 
    {
        if ( !match<T>(value<int>(N))(t) ||
             !apply_expression(m_e1, t, match_members_ex<T,N>::member0()) )
            t = 0;

        return t;
    }
          T* operator()(      T* t) const 
    {
        if ( !match<T>(value<int>(N))(t) ||
             !apply_expression(m_e1, t, match_members_ex<T,N>::member0()) )
            t = 0;

        return t;
    }

    template <typename U> const T* operator()(const U* u) const { return operator()(dynamic_cast<const T*>(u)); }
    template <typename U>       T* operator()(      U* u) const { return operator()(dynamic_cast<      T*>(u)); }
    template <typename U> const T* operator()(const U& u) const { return operator()(&u); }
    template <typename U>       T* operator()(      U& u) const { return operator()(&u); }
    const E1& m_e1;
};

//------------------------------------------------------------------------------

template <typename T, int N, typename E1, typename E2>
struct matcher2ex
{
    matcher2ex(const E1& e1, const E2& e2) : m_e1(e1), m_e2(e2) {}

    const T* operator()(const T* t) const 
    {
        if ( !match<T>(value<int>(N))(t) ||
             !apply_expression(m_e1, t, match_members_ex<T,N>::member0()) ||
             !apply_expression(m_e2, t, match_members_ex<T,N>::member1())
            )
            t = 0;

        return t;
    }
          T* operator()(      T* t) const 
    {
        if ( !match<T>(value<int>(N))(t) ||
             !apply_expression(m_e1, t, match_members_ex<T,N>::member0()) ||
             !apply_expression(m_e2, t, match_members_ex<T,N>::member1())
            )
            t = 0;

        return t;
    }

    template <typename U> const T* operator()(const U* u) const { return operator()(dynamic_cast<const T*>(u)); }
    template <typename U>       T* operator()(      U* u) const { return operator()(dynamic_cast<      T*>(u)); }
    template <typename U> const T* operator()(const U& u) const { return operator()(&u); }
    template <typename U>       T* operator()(      U& u) const { return operator()(&u); }
    const E1& m_e1;
    const E2& m_e2;
};

//------------------------------------------------------------------------------

template <typename T, int N, typename E1, typename E2, typename E3>
struct matcher3ex
{
    matcher3ex(const E1& e1, const E2& e2, const E3& e3) : m_e1(e1), m_e2(e2), m_e3(e3) {}

    const T* operator()(const T* t) const 
    {
        if ( !match<T>(value<int>(N))(t) ||
             !apply_expression(m_e1, t, match_members_ex<T,N>::member0()) ||
             !apply_expression(m_e2, t, match_members_ex<T,N>::member1()) ||
             !apply_expression(m_e3, t, match_members_ex<T,N>::member2())
            )
            t = 0;

        return t;
    }
          T* operator()(      T* t) const 
    {
        if ( !match<T>(value<int>(N))(t) ||
             !apply_expression(m_e1, t, match_members_ex<T,N>::member0()) ||
             !apply_expression(m_e2, t, match_members_ex<T,N>::member1()) ||
             !apply_expression(m_e3, t, match_members_ex<T,N>::member2())
            )
            t = 0;

        return t;
    }

    template <typename U> const T* operator()(const U* u) const { return operator()(dynamic_cast<const T*>(u)); }
    template <typename U>       T* operator()(      U* u) const { return operator()(dynamic_cast<      T*>(u)); }
    template <typename U> const T* operator()(const U& u) const { return operator()(&u); }
    template <typename U>       T* operator()(      U& u) const { return operator()(&u); }
    const E1& m_e1;
    const E2& m_e2;
    const E3& m_e3;
};

//------------------------------------------------------------------------------

template <typename T, int N, typename E1, typename E2, typename E3, typename E4>
struct matcher4ex
{
    matcher4ex(const E1& e1, const E2& e2, const E3& e3, const E4& e4) : m_e1(e1), m_e2(e2), m_e3(e3), m_e4(e4) {}

    const T* operator()(const T* t) const 
    {
        if ( !match<T>(value<int>(N))(t) ||
             !apply_expression(m_e1, t, match_members_ex<T,N>::member0()) ||
             !apply_expression(m_e2, t, match_members_ex<T,N>::member1()) ||
             !apply_expression(m_e3, t, match_members_ex<T,N>::member2()) ||
             !apply_expression(m_e4, t, match_members_ex<T,N>::member3())
            )
            t = 0;

        return t;
    }
          T* operator()(      T* t) const 
    {
        if ( !match<T>(value<int>(N))(t) ||
             !apply_expression(m_e1, t, match_members_ex<T,N>::member0()) ||
             !apply_expression(m_e2, t, match_members_ex<T,N>::member1()) ||
             !apply_expression(m_e3, t, match_members_ex<T,N>::member2()) ||
             !apply_expression(m_e4, t, match_members_ex<T,N>::member3())
            )
            t = 0;

        return t;
    }

    template <typename U> const T* operator()(const U* u) const { return operator()(dynamic_cast<const T*>(u)); }
    template <typename U>       T* operator()(      U* u) const { return operator()(dynamic_cast<      T*>(u)); }
    template <typename U> const T* operator()(const U& u) const { return operator()(&u); }
    template <typename U>       T* operator()(      U& u) const { return operator()(&u); }
    const E1& m_e1;
    const E2& m_e2;
    const E3& m_e3;
    const E4& m_e4;
};

//------------------------------------------------------------------------------

template <typename T, int N>
matcher0ex<T,N> matchex()
{
    return matcher0ex<T,N>();
}

//------------------------------------------------------------------------------

template <typename T, int N, typename E1>
matcher1ex<T,N,E1> matchex(const E1& e1)
{
    return matcher1ex<T,N,E1>(e1);
}

//------------------------------------------------------------------------------

template <typename T, int N, typename E1, typename E2>
matcher2ex<T,N,E1,E2> matchex(const E1& e1, const E2& e2)
{
    return matcher2ex<T,N,E1,E2>(e1,e2);
}

//------------------------------------------------------------------------------

template <typename T, int N, typename E1, typename E2, typename E3>
matcher3ex<T,N,E1,E2,E3> matchex(const E1& e1, const E2& e2, const E3& e3)
{
    return matcher3ex<T,N,E1,E2,E3>(e1,e2,e3);
}

//------------------------------------------------------------------------------

template <typename T, int N, typename E1, typename E2, typename E3, typename E4>
matcher4ex<T,N,E1,E2,E3,E4> matchex(const E1& e1, const E2& e2, const E3& e3, const E4& e4)
{
    return matcher4ex<T,N,E1,E2,E3,E4>(e1,e2,e3,e4);
}
