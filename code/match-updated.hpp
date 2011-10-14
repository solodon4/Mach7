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

//------------------------------------------------------------------------------

typedef char  yes_type;
typedef double no_type;
template <class U> yes_type is_class_tester(void(U::*)(void));
template <class U>  no_type is_class_tester(...);

template <typename T, bool is_class=sizeof(is_class_tester<T>(0)) == sizeof(yes_type)>
struct value_of : T 
{
    value_of() : T() {}
    value_of(const T& t) : T(t) {}
};

template <typename T>
struct value_of<T,false> 
{
    value_of() : m_value() {}
    value_of(const T& t) : m_value(t) {}
    T m_value; 
};

//------------------------------------------------------------------------------

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

#define CM(Index,Class,Member) static auto member##Index() -> decltype(&Class::Member) { return &Class::Member; }
#define EF(Index,Func)         static auto member##Index() -> decltype(&Func)          { return &Func; }

//------------------------------------------------------------------------------

/// Traits like structure that will define which members should be matched at 
/// which positions. It is intentionally left undefined as user will have to
/// provide specializations for his hierarchy.
template <typename AST> struct match_members;

//------------------------------------------------------------------------------

/// Variable binding for a value type
template <class T>
struct variable : value_of<T>
{
    variable() : value_of<T>() {}

    /// We report that matching succeeded and bind the value
    bool operator()(const T& t)
    {
        value_of<T>::operator=(t);
        return true;
    }

    /// Helper conversion operator to let the variable be used in some places
    /// where T was allowed
    //operator const T&() const { return m_value; }

    /// Member that will hold matching value in case of successful matching
    //mutable T m_value;
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
    // NOTE: We don't need the below application anymore since we have a 
    //       specialization that never applies the actual member before
    //       passing it to this meta variable that matches everything.
    //template <typename U>
    //bool operator()(const U& u) const { return true; }
};

//------------------------------------------------------------------------------

template <typename T>
std::ostream& operator<<(std::ostream& os, const variable<T>& v) { return os << static_cast<const T&>(v); }

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

//------------------------------------------------------------------------------

template <class C, class T, typename R>
R apply_member(const C* c, R (T::*method)() const)
{
    DEBUG_ONLY(std::clog << "\nApplying member function to instance " << c << " of type " << typeid(*c).name() << std::endl);
    return (c->*method)();
}

//------------------------------------------------------------------------------

template <class C, class T, typename R>
R apply_member(      C* c, R (T::*method)()      )
{
    DEBUG_ONLY(std::clog << "\nApplying member function to instance " << c << " of type " << typeid(*c).name() << std::endl);
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

/// We need this extra indirection to be able to intercept when we are trying to 
/// match a meta variable _ of type variable<void>, that matches everything of
/// any type. In this case we don't even want to invoke the underlain member!
template <typename E, typename C, typename M>
bool apply_expression(E&& e, C* c, M m)
{
    #ifdef _MSC_VER
    #pragma warning( disable : 4800 )
    #endif

    return std::forward<E>(e)(apply_member(c, m));
}

/// This is the specialization that makes the member not to be invoked when we
/// are matching against the meta variable _ that matches everything.
template <typename C, typename M>
bool apply_expression(const variable<void>& e, C* c, M m)
{
    return true;
}

//------------------------------------------------------------------------------

template <typename T>
struct matcher0
{
    const T* operator()(const T* t) const { return t; }
    template <typename U>
    const T* operator()(const U* u) const { return dynamic_cast<const T*>(u); }
    template <typename U>
    const T* operator()(const U& u) const { return operator()(&u); }
};

//------------------------------------------------------------------------------

template <typename T, typename E1>
struct matcher1
{
    matcher1(const E1& e1) : m_e1(e1) {}
    const T* operator()(const T* t) const 
    {
        if ( !apply_expression(m_e1, t, match_members<T>::member0()) )
            t = 0;

        return t;
    }
    template <typename U>
    const T* operator()(const U* u) const { return operator()(dynamic_cast<const T*>(u)); }
    template <typename U> 
    const T* operator()(const U& u) const { return operator()(&u); }
    const E1& m_e1;
};

//------------------------------------------------------------------------------

template <typename T, typename E1, typename E2>
struct matcher2
{
    matcher2(const E1& e1, const E2& e2) : m_e1(e1), m_e2(e2) {}
    const T* operator()(const T* t) const 
    {
        if ( !apply_expression(m_e1, t, match_members<T>::member0()) ||
             !apply_expression(m_e2, t, match_members<T>::member1())
            )
            t = 0;

        return t;
    }
    template <typename U>
    const T* operator()(const U* u) const { return operator()(dynamic_cast<const T*>(u)); }
    template <typename U>
    const T* operator()(const U& u) const { return operator()(&u); }
    const E1& m_e1;
    const E2& m_e2;
};

//------------------------------------------------------------------------------

template <typename T, typename E1, typename E2, typename E3>
struct matcher3
{
    matcher3(const E1& e1, const E2& e2, const E3& e3) : m_e1(e1), m_e2(e2), m_e3(e3) {}
    const T* operator()(const T* t) const 
    {
        if ( !apply_expression(m_e1, t, match_members<T>::member0()) ||
             !apply_expression(m_e2, t, match_members<T>::member1()) ||
             !apply_expression(m_e3, t, match_members<T>::member2())
            )
            t = 0;

        return t;
    }
    template <typename U>
    const T* operator()(const U* u) const { return operator()(dynamic_cast<const T*>(u)); }
    template <typename U>
    const T* operator()(const U& u) const { return operator()(&u); }
    const E1& m_e1;
    const E2& m_e2;
    const E3& m_e3;
};

//------------------------------------------------------------------------------

template <typename T, typename E1, typename E2, typename E3, typename E4>
struct matcher4
{
    matcher4(const E1& e1, const E2& e2, const E3& e3, const E4& e4) : m_e1(e1), m_e2(e2), m_e3(e3), m_e4(e4) {}
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
    template <typename U>
    const T* operator()(const U* u) const { return operator()(dynamic_cast<const T*>(u)); }
    template <typename U>
    const T* operator()(const U& u) const { return operator()(&u); }
    const E1& m_e1;
    const E2& m_e2;
    const E3& m_e3;
    const E4& m_e4;
};

//------------------------------------------------------------------------------

template <typename T>
matcher0<T> match()
{
    return matcher0<T>();
}

//------------------------------------------------------------------------------

template <typename T, typename E1>
matcher1<T,E1> match(const E1& e1)
{
    return matcher1<T,E1>(e1);
}

//------------------------------------------------------------------------------

template <typename T, typename E1, typename E2>
matcher2<T,E1,E2> match(const E1& e1, const E2& e2)
{
    return matcher2<T,E1,E2>(e1,e2);
}

//------------------------------------------------------------------------------

template <typename T, typename E1, typename E2, typename E3>
matcher3<T,E1,E2,E3> match(const E1& e1, const E2& e2, const E3& e3)
{
    return matcher3<T,E1,E2,E3>(e1,e2,e3);
}

//------------------------------------------------------------------------------

template <typename T, typename E1, typename E2, typename E3, typename E4>
matcher4<T,E1,E2,E3,E4> match(const E1& e1, const E2& e2, const E3& e3, const E4& e4)
{
    return matcher4<T,E1,E2,E3,E4>(e1,e2,e3,e4);
}

//------------------------------------------------------------------------------
