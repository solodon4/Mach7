///
/// \file match.hpp
///
/// This file defines the necessary infrastructure for ML-like pattern matching
/// against trees in C++.
///
/// \autor Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of the XTL framework (http://parasol.tamu.edu/xtl/).
/// Copyright (C) 2005-2011 Texas A&M University.
/// All rights reserved.
///

/// FIX: Big question: should null pointers match pointer variables???

#pragma once

#include <cassert>
#include <ostream>
#include <boost/type_traits/remove_const.hpp>
#include "exprtmpl.hpp"
#include "vtblmap2.hpp"
//#include "memoized_cast.hpp"

//------------------------------------------------------------------------------

template <typename T> inline const T* adjust_ptr(const void* p, ptrdiff_t offset) { return  reinterpret_cast<const T*>(reinterpret_cast<const char*>(p)+offset); }
template <typename T> inline       T* adjust_ptr(      void* p, ptrdiff_t offset) { return  reinterpret_cast<      T*>(reinterpret_cast<      char*>(p)+offset); }

using boost::remove_const;

#ifdef _DEBUG
#include <typeinfo>
#include <iostream>
#endif

//------------------------------------------------------------------------------

enum { default_layout = size_t(~0) };

/// Traits like structure that will define which members should be matched at 
/// which positions. It is intentionally left undefined as user will have to
/// provide specializations for his hierarchy.
template <typename type_being_matched, size_t layout = default_layout> struct match_members;

//------------------------------------------------------------------------------

template <typename T> inline const T* addr(const T* t) { return  t; }
template <typename T> inline       T* addr(      T* t) { return  t; }
template <typename T> inline const T* addr(const T& t) { return &t; }
template <typename T> inline       T* addr(      T& t) { return &t; }

//------------------------------------------------------------------------------

template <int N> struct requires_bits    { enum { value = requires_bits<(N+1)/2>::value+1 }; };
template <>      struct requires_bits<1> { enum { value = 0 }; };

//------------------------------------------------------------------------------

//template <typename T> struct remove_const          { typedef T type; };
//template <typename T> struct remove_const<const T> { typedef T type; };

//------------------------------------------------------------------------------

template <typename T> struct remove_ref           { typedef T type; };
template <typename T> struct remove_ref<      T&> { typedef T type; };
template <typename T> struct remove_ref<const T > { typedef T type; };
template <typename T> struct remove_ref<const T&> { typedef T type; };

//------------------------------------------------------------------------------

template <typename T, typename U> inline const T* stat_cast(const U* p) { return static_cast<const T*>(p); }
template <typename T, typename U> inline       T* stat_cast(      U* p) { return static_cast<      T*>(p); }

//------------------------------------------------------------------------------

//#define dynamic_cast memoized_cast
#define memoized_cast dynamic_cast

/// Macro to define member's position within decomposition of a given data type
/// Example: CM(0,MyClass::member) or CM(1,external_func)
/// \note Use this macro only inside specializations of the above two templates
/// \note We use variadic macro parameter here in order to be able to handle 
///       templates, which might have commas, otherwise juse a second argument
///       would be sufficient.
#define CM(Index,...) static inline decltype(&__VA_ARGS__) member##Index() { return &__VA_ARGS__; }

/// Macro to define a kind selector - a member of the common base class that 
/// carries a distinct integral value that uniquely identifies the derived 
/// type.  Used in the decomposition of the base class.
#define KS(...)       static inline decltype(&__VA_ARGS__) kind_selector() { return &__VA_ARGS__; } bool kind_selector_dummy() const;

/// Macro to define an integral constant that uniquely identifies the derived 
/// class. Used in the decomposition of a derived class.
#define KV(kind)      static const size_t kind_value = kind;

/// 1 here (preallocated vtbl map) is better for sequential case, but for some
/// reason 0 (static member of a function vtbl map) is better for random case.
#if 0
/// Macro that starts the switch on pattern
#define SWITCH(s)\
        auto const   __selector_ptr = addr(s);\
        const void*  __casted_ptr;\
        type_switch_info& __switch_info = preallocated<vtblmap<type_switch_info&>,__LINE__>::value.get(__selector_ptr);\
        switch (__switch_info.line)
/// Extended version of the macro that starts the switch on pattern, that takes an expected number of cases in
#define SWITCH_N(s,N)\
        auto const   __selector_ptr = addr(s);\
        const void*  __casted_ptr;\
        type_switch_info& __switch_info = preallocated<vtblmap<type_switch_info&,requires_bits<N>::value>,__LINE__>::value.get(__selector_ptr);\
        switch (__switch_info.line)

#else /// These are with static function member

/// Macro that starts the switch on pattern
#define SWITCH(s)\
        static vtblmap<type_switch_info&> __vtbl2lines_map;\
        auto const   __selector_ptr = addr(s);\
        const void*  __casted_ptr;\
        type_switch_info& __switch_info = __vtbl2lines_map.get(__selector_ptr);\
        switch (__switch_info.line)
/// Extended version of the macro that starts the switch on pattern, that takes an expected number of cases in
#define SWITCH_N(s,N)\
        static vtblmap<type_switch_info&,requires_bits<N>::value> __vtbl2lines_map;\
        auto const   __selector_ptr = addr(s);\
        const void*  __casted_ptr;\
        type_switch_info& __switch_info = __vtbl2lines_map.get(__selector_ptr);\
        switch (__switch_info.line)

#endif

/// Macro that defines the case statement for the above switch
/// NOTE: If Visual C++ gives you error C2051: case expression not constant
///       on this CASE label, just change the Debug Format in project setting 
///       Project -> Properties -> C/C++ -> General -> Debug Information Format 
///       from "Program Database for Edit And Continue (/ZI)" 
///       to   "Program Database (/Zi)", which is the default in Release builds,
///       but not in Debug. This is a known bug of Visual C++ described here:
///       http://connect.microsoft.com/VisualStudio/feedback/details/375836/-line-not-seen-as-compile-time-constant
#define CASE(C,...) }} if (UNLIKELY_BRANCH(__casted_ptr = dynamic_cast<const C*>(__selector_ptr))) { if (LIKELY_BRANCH((__switch_info.line == 0))) { __switch_info.line = __LINE__; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); } case __LINE__: auto matched = adjust_ptr<C>(__selector_ptr,__switch_info.offset); if (LIKELY_BRANCH(match<C>(__VA_ARGS__)(matched))) {
#define CASES_BEGIN default: {{
#define CASES_END }} if (LIKELY_BRANCH((__switch_info.line == 0))) { __switch_info.line = __LINE__; } case __LINE__: ;

//------------------------------------------------------------------------------

/// Macro that starts the switch on dynamic type of a variable s that can be 
/// either pointer or reference to a polymorphic type.
#define TYPE_SWITCH(s)\
        static vtblmap<type_switch_info&> __vtbl2lines_map;\
        auto const   __selector_ptr = addr(s);\
        const void*  __casted_ptr;\
        type_switch_info& __switch_info = __vtbl2lines_map.get(__selector_ptr);\
        switch (__switch_info.line)
/// Extended version of the above macro that takes an expected number of cases in
/// to estimate the size of the cache needed instead of using the default size
#define TYPE_SWITCH_N(s,N)\
        static vtblmap<type_switch_info&,requires_bits<N>::value> __vtbl2lines_map;\
        auto const   __selector_ptr = addr(s);\
        const void*  __casted_ptr;\
        type_switch_info& __switch_info = __vtbl2lines_map.get(__selector_ptr);\
        switch (__switch_info.line)

/// Macro that defines the case statement for the above switch
/// NOTE: If Visual C++ gives you error C2051: case expression not constant
///       on this CASE label, just change the Debug Format in project setting 
///       Project -> Properties -> C/C++ -> General -> Debug Information Format 
///       from "Program Database for Edit And Continue (/ZI)" 
///       to   "Program Database (/Zi)", which is the default in Release builds,
///       but not in Debug. This is a known bug of Visual C++ described here:
///       http://connect.microsoft.com/VisualStudio/feedback/details/375836/-line-not-seen-as-compile-time-constant
#define TYPE_CASE(C)   } if (UNLIKELY_BRANCH(__casted_ptr = dynamic_cast<const C*>(__selector_ptr))) { if (__switch_info.line == 0) { __switch_info.line = __LINE__; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); } case __LINE__: auto matched = adjust_ptr<C>(__selector_ptr,__switch_info.offset);
#define TYPE_CASES_BEGIN default: {
#define TYPE_CASES_END } if (__switch_info.line == 0) { __switch_info.line = __LINE__; } case __LINE__: ;

//------------------------------------------------------------------------------

/// Macro that starts the switch on types that carry their own dynamic type as
/// a distinct integral value in one of their members.
#define KIND_SWITCH(s)\
        auto const __selector_ptr  = addr(s);\
        auto const __kind_selector = apply_member(__selector_ptr, match_members<remove_ref<decltype(*__selector_ptr)>::type>::kind_selector());\
        switch (__kind_selector)

/// Macro that defines the case statement for the above switch
#define KIND_CASE(C,...) }} if (UNLIKELY_BRANCH((__kind_selector == match_members<C>::kind_value))) { case match_members<C>::kind_value: auto matched = stat_cast<C>(__selector_ptr); if (LIKELY_BRANCH((match<C>(__VA_ARGS__)(matched)))) {
#define KIND_CASES_BEGIN {{
#define KIND_CASES_END   }} default: ;

//------------------------------------------------------------------------------

/// Macro that starts the switch on types that carry their own dynamic type as
/// a distinct integral value in one of their members.
#define UNION_SWITCH(s)\
        auto const __selector_ptr  = addr(s);\
        auto const __kind_selector = apply_member(__selector_ptr, match_members<remove_ref<decltype(*__selector_ptr)>::type>::kind_selector());\
        switch (__kind_selector)

/// Macro that defines the case statement for the above switch
#define UNION_CASE(C,L,...) }} if (UNLIKELY_BRANCH((__kind_selector == match_members<C,L>::kind_value))) { case match_members<C,L>::kind_value: auto matched = stat_cast<C>(__selector_ptr); if (LIKELY_BRANCH((match<C,L>(__VA_ARGS__)(matched)))) {
#define UNION_CASES_BEGIN {{
#define UNION_CASES_END   }} default: ;

#include "has_member.hpp"

//------------------------------------------------------------------------------

/// This is the general case that essentially assumes open case
template <typename SelectorType, class condition = void>
class generic_switch
{
public:
    typedef typename remove_ref<SelectorType>::type selector_type;
    typedef vtblmap<type_switch_info&>              static_data_type;

    // Open case only works for polymorphic types (types with virtual funcs)
    // If your base type is not polymorphic, but you'd still like to have 
    // type switching/pattern matching functionality on it, use KS macro in 
    // corresponding match_members<your_base_type> to identify which field
    // behaves as a selector and thus uniquely identifies derived type.
    static_assert(
        std::is_polymorphic<selector_type>::value,
        "Type of selector should either be polymorphic or you should provide kind selector for the type"
    );

    struct local_data_type
    {
        const void*       casted_ptr;
        type_switch_info* switch_info_ptr;
    };

    static void foo() { std::cout << "General" << std::endl; }

    static size_t choose(const selector_type* selector_ptr, static_data_type& static_data, local_data_type& local_data)
    {
        local_data.switch_info_ptr = &static_data.get(selector_ptr);
        return local_data.switch_info_ptr->line;
    }

    static void on_first_pass(const selector_type* selector_ptr, local_data_type& local_data, size_t line)
    {
        if (LIKELY_BRANCH(local_data.switch_info_ptr->line == 0)) 
        {
            local_data.switch_info_ptr->line   = line; 
            local_data.switch_info_ptr->offset = intptr_t(local_data.casted_ptr)-intptr_t(selector_ptr);
        } 
    }
    
    static void on_end(const selector_type* selector_ptr, local_data_type& local_data, size_t line)
    {
        if (LIKELY_BRANCH(local_data.switch_info_ptr->line == 0)) 
        { 
            local_data.switch_info_ptr->line   = line;
        }
    }

    template <bool FirstParamIsValue>
    struct disambiguate
    {
        template <typename T>
        struct parameter
        {
            typedef T target_type;

            template <size_t LineNumber>
            struct CaseLabel
            {
                enum { value = LineNumber };
            };

            enum { layout = default_layout };

            static bool main_condition(const selector_type* selector_ptr, local_data_type& local_data)
            {
                return local_data.casted_ptr = dynamic_cast<const target_type*>(selector_ptr);
            }
            
            static const target_type* get_matched(const selector_type* selector_ptr, local_data_type& local_data)
            {
                std::cout << "Open case" << std::endl;
                return adjust_ptr<target_type>(selector_ptr,local_data.switch_info_ptr->offset);
            }
        };
    };
};

template <typename SelectorType>
class generic_switch<
    SelectorType, 
    typename std::enable_if<has_member_kind_selector<match_members<typename remove_ref<SelectorType>::type>>::value,void>::type
>
{
public:
    typedef typename remove_ref<SelectorType>::type selector_type;
    typedef bool static_data_type;
    typedef bool local_data_type;
    static void foo() { std::cout << "Special" << std::endl; }

    static size_t choose(const selector_type* selector_ptr, static_data_type& static_data, local_data_type& local_data)
    {
        return apply_member(selector_ptr, match_members<selector_type>::kind_selector());
    }
    static void on_first_pass(const selector_type* selector_ptr, local_data_type& local_data, size_t line) {}
    static void        on_end(const selector_type* selector_ptr, local_data_type& local_data, size_t line) {}

    // C++ standard (14.7.3.2) would not allow us to explicitly specialize 
    // disambiguate later here, but will accept a partial specialization so we
    // add a bogus template parameter to turn explicit specialization into 
    // partial.
    template <bool FirstParamIsValue, typename bogus_parameter = void>
    struct disambiguate
    {
        template <size_t N>
        struct parameter
        {
            typedef selector_type target_type;

            template <size_t LineNumber>
            struct CaseLabel
            {
                enum { value = N };
            };

            enum { layout = N };

            static bool main_condition(const selector_type* selector_ptr, local_data_type& local_data) { return true; }
            static const target_type* get_matched(const selector_type* selector_ptr, local_data_type& local_data)
            {
                std::cout << "Union case" << std::endl;
                return selector_ptr;
            }
        };
    };

    template <typename bogus_parameter>
    struct disambiguate<false,bogus_parameter>
    {
        template <typename T>
        struct parameter
        {
            typedef T target_type;

            template <size_t LineNumber>
            struct CaseLabel
            {
                enum { value = match_members<target_type>::kind_value };
            };

            enum { layout = default_layout };

            static bool main_condition(const selector_type* selector_ptr, local_data_type& local_data) { return true; }
            static const target_type* get_matched(const selector_type* selector_ptr, local_data_type& local_data)
            {
                std::cout << "Closed case" << std::endl;
                return stat_cast<target_type>(selector_ptr);
            }
        };
    };
};

//------------------------------------------------------------------------------

/// Macro that starts the switch on types that carry their own dynamic type as
/// a distinct integral value in one of their members.
#define GENERIC_SWITCH(s)\
        auto const __selector_ptr = addr(s);\
        enum { __base_line = __LINE__ };\
        typedef generic_switch<decltype(*__selector_ptr)> switch_traits;\
        static switch_traits::static_data_type static_data;\
               switch_traits::local_data_type  local_data;\
        switch (switch_traits::choose(__selector_ptr,static_data,local_data))

/// Macro that defines the case statement for the above switch
/// NOTE: If Visual C++ gives you error C2051: case expression not constant
///       on this CASE label, just change the Debug Format in project setting 
///       Project -> Properties -> C/C++ -> General -> Debug Information Format 
///       from "Program Database for Edit And Continue (/ZI)" 
///       to   "Program Database (/Zi)", which is the default in Release builds,
///       but not in Debug. This is a known bug of Visual C++ described here:
///       http://connect.microsoft.com/VisualStudio/feedback/details/375836/-line-not-seen-as-compile-time-constant
#define GENERIC_CASE(C,...) }}} { typedef switch_traits::disambiguate<sizeof(C)<sizeof(switch_traits::selector_type)>::parameter<C> target_specific; if (target_specific::main_condition(__selector_ptr, local_data)) { switch_traits::on_first_pass(__selector_ptr, local_data, __LINE__-__base_line); case target_specific::CaseLabel<__LINE__-__base_line>::value: auto matched = target_specific::get_matched(__selector_ptr,local_data); if (LIKELY_BRANCH(match<target_specific::target_type,target_specific::layout>(__VA_ARGS__)(matched))) {
#define GENERIC_CASES_BEGIN default: {{{
#define GENERIC_CASES_END }}} switch_traits::on_end(__selector_ptr, local_data, __LINE__-__base_line); case __LINE__-__base_line: ;

//------------------------------------------------------------------------------

template <class T>
struct variable;

//------------------------------------------------------------------------------

template <typename T, typename E>
struct guard
{
    guard(const variable<T>& v, const E& e) : m_v(v), m_e(e) {}
    guard(const variable<T>& v, E&& e) : m_v(v), m_e(e) {}
    guard(guard&& g) : m_v(g.m_v), m_e(std::move(g.m_e)) {}
    template <typename U>
    bool operator()(const U& u) const { return m_v(u) && eval(m_e); }
    const variable<T>& m_v;
    const E            m_e;
};

//------------------------------------------------------------------------------

/// A reference to a user provided variable
template <class T>
struct var_ref
{
    var_ref(T& var) : m_var(&var) {}
    var_ref(var_ref&& v) : m_var(v.m_var) {}

    typedef T result_type;
    operator result_type() const throw() { return *m_var; }

    /// We report that matching succeeded and bind the value
    bool operator()(const T& t) const 
    {
        *m_var = t;
        return true;
    }

    /// Member that will hold matching value in case of successful matching
    T* m_var;
};

/// A reference to a user provided variable
template <class T>
struct var_ref<variable<T> >
{
    var_ref(variable<T>& var) : m_var(&var) {}
    var_ref(var_ref&& v) : m_var(v.m_var) {}

    typedef T result_type;
    operator result_type() const throw() { return *m_var; }

    /// We report that matching succeeded and bind the value
    bool operator()(const T& t) const 
    {
        *m_var = t;
        return true;
    }

    /// Member that will hold matching value in case of successful matching
    variable<T>* m_var;
};

//------------------------------------------------------------------------------

/// Variable binding for a value type
template <class T>
struct variable
{
    variable() : m_value() {}
    variable(const T& t) : m_value(t) {}
    variable(T&& t) : m_value(std::move(t)) {}
    variable(variable&& v) : m_value(std::move(v.m_value)) {}

    typedef T result_type;

    /// We report that matching succeeded and bind the value
    bool operator()(const T& t) const
    {
        m_value = t;
        return true;
    }

    variable& operator=(const T& t) { m_value = t; return *this; }

    template <typename E> 
    auto operator|=(E&& e) -> guard<T,decltype(filter(std::forward<E>(e)))>
    { 
        return guard<T,decltype(filter(std::forward<E>(e)))>(*this,filter(std::forward<E>(e))); 
    }

    /// Helper conversion operator to let the variable be used in some places
    /// where T was allowed
    operator const T&() const throw() { return m_value; }

    /// Member that will hold matching value in case of successful matching
    mutable T m_value;
};

//------------------------------------------------------------------------------

/// Variable binding for a pointer type
template <class T>
struct variable<const T*>
{
    variable() : m_value() {}
    variable(variable&& v) : m_value(std::move(v.m_value)) {}

    typedef const T* result_type;

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

    template <typename E> 
    auto operator|=(E&& e) -> guard<const T*,decltype(filter(std::forward<E>(e)))>
    { 
        return guard<const T*,decltype(filter(std::forward<E>(e)))>(*this,filter(std::forward<E>(e))); 
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
    operator const T*() const throw() { return m_value; }

    /// A helper member to let the wrapping variable be used as a pointer
    const T* operator->() const throw() { return m_value; }

    /// A helper member to let the wrapping variable be used as a pointer
    const T& operator*()  const throw() { return *m_value; }

    /// Member that will hold matching value in case of successful matching
    mutable const T* m_value;
};

//------------------------------------------------------------------------------

/// Variable binding for a pointer type
template <class T>
struct variable<const T&>
{
    variable() : m_value() {}
    variable(variable&& v) : m_value(std::move(v.m_value)) {}

    typedef const T& result_type;

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

    template <typename E> 
    auto operator|=(E&& e) -> guard<const T&,decltype(filter(std::forward<E>(e)))>
    { 
        return guard<const T&,decltype(filter(std::forward<E>(e)))>(*this,filter(std::forward<E>(e))); 
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
    operator const T&() const throw() { XTL_ASSERT(m_value); return *m_value; }

    /// Member that will hold matching value in case of successful matching
    mutable const T* m_value;
};

//------------------------------------------------------------------------------

/// Class that describes a meta variable that matches everything.
/// This variable will match any type returned by a member, but the main 
/// difference from just using a variable whose value is ignored is that 
/// use of this variable will make sure the actual member is never invoked!
struct wildcard
{
    //typedef void result_type;

    // NOTE: We don't need the below application anymore since we have a 
    //       specialization that never applies the actual member before
    //       passing it to this meta variable that matches everything.
    //template <typename U>
    //bool operator()(const U& u) const throw() { return true; }
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
    typedef T result_type;
    value(const T& t) : m_value(t) {}
    value(T&& t) : m_value(std::move(t)) {}
    value(value&& v) : m_value(std::move(v.m_value)) {}
    bool operator()(const T& t) const throw() { return m_value == t; }
    operator result_type() const throw() { return m_value; }
    T m_value;
};

template <class T> inline value<T> val(const T& t) { return value<T>(t); }

//------------------------------------------------------------------------------

template <typename F, typename E1 = void, typename E2 = void>
struct expr;

template <typename F, typename E1>
struct expr<F,E1>
{
    expr(const E1& e1) : m_e1(e1) {}
    expr(E1&& e1) : m_e1(std::move(e1)) {}
    expr(expr&& e) : m_e1(std::move(e.m_e1)) {}
    typedef typename remove_const<decltype(F()(*static_cast<typename E1::result_type*>(0)))>::type result_type; // We needed to add remove_const here as MSVC was returning const T
    operator result_type() const { return eval(*this); }
    bool operator()(const result_type& t) const 
    {
        typename E1::result_type v;
        return solver<F>()(v,t) && m_e1(v);
    }
    const E1 m_e1;
};

template <typename F, typename E1, typename E2>
struct expr
{
    expr(const E1& e1, const E2& e2) : m_e1(e1), m_e2(e2) {}
    expr(E1&& e1, E2&& e2) : m_e1(std::move(e1)), m_e2(std::move(e2)) {}
    expr(expr&& e) : m_e1(std::move(e.m_e1)), m_e2(std::move(e.m_e2)) {}
    typedef typename remove_const<decltype(F()(*static_cast<typename E1::result_type*>(0),*static_cast<typename E2::result_type*>(0)))>::type result_type; // We needed to add remove_const here as MSVC was returning const T
    operator result_type() const { return eval(*this); }
    bool operator()(const result_type& t) const 
    {
        typename E1::result_type v;
        return solver<F>()(v,eval(m_e2),t) && m_e1(v);
    }
    const E1 m_e1;
    const E2 m_e2;
};

//------------------------------------------------------------------------------

template <typename E1, typename E2>
struct expr_or
{
    expr_or(const E1& e1, const E2& e2) : m_e1(e1), m_e2(e2) {}
    expr_or(E1&& e1, E2&& e2) : m_e1(std::move(e1)), m_e2(std::move(e2)) {}
    expr_or(expr_or&& e) : m_e1(std::move(e.m_e1)), m_e2(std::move(e.m_e2)) {}
    template <typename U> bool operator()(const U* u) const { return m_e1(u) || m_e2(u); }
    template <typename U> bool operator()(      U* u) const { return m_e1(u) || m_e2(u); }
    template <typename U> bool operator()(const U& u) const { return operator()(&u); }
    template <typename U> bool operator()(      U& u) const { return operator()(&u); }
    const E1 m_e1;
    const E2 m_e2;
};

//------------------------------------------------------------------------------

template <typename T, size_t layout, typename E1, typename E2, typename E3, typename E4> struct matcher4;
template <typename T, size_t layout, typename E1, typename E2, typename E3>              struct matcher3;
template <typename T, size_t layout, typename E1, typename E2>                           struct matcher2;
template <typename T, size_t layout, typename E1>                                        struct matcher1;
template <typename T, size_t layout>                                                     struct matcher0;

//------------------------------------------------------------------------------

template <typename T> inline var_ref<T> filter(T& t)        { return var_ref<T>(t); }
template <typename T> inline value<T>   filter(const T& t)  { return value<T>(t); }

template <typename T, size_t layout, typename E1, typename E2, typename E3, typename E4>
matcher4<T,layout,E1,E2,E3,E4> inline filter(const matcher4<T,layout,E1,E2,E3,E4>& t) { return t; }
template <typename T, size_t layout, typename E1, typename E2, typename E3>
matcher3<T,layout,E1,E2,E3> inline filter(const matcher3<T,layout,E1,E2,E3>& t) { return t; }
template <typename T, size_t layout, typename E1, typename E2>
matcher2<T,layout,E1,E2> inline filter(const matcher2<T,layout,E1,E2>& t) { return t; }
template <typename T, size_t layout, typename E1>
matcher1<T,layout,E1> inline filter(const matcher1<T,layout,E1>& t) { return t; }
template <typename T, size_t layout>
matcher0<T,layout> inline filter(const matcher0<T,layout>& t) { return t; }

template <typename F, typename E1, typename E2>
expr<F,E1,E2> inline filter(const expr<F,E1,E2>& t) { return t; }
template <typename F, typename E1>
expr<F,E1> inline filter(const expr<F,E1>& t) { return t; }
template <typename F>
expr<F> inline filter(const expr<F>& t) { return t; }
template <typename T, typename E>
guard<T,E> inline filter(const guard<T,E>& t) { return t; }

//------------------------------------------------------------------------------

// Define all unary operators on variable
#define FOR_EACH_UNARY_OPERATOR(F,S) \
    template <typename T> \
    inline auto XTL_CONCATENATE(operator,S)(variable<T>& v)\
          -> expr<F,var_ref<variable<T> > >\
    { return expr<F,var_ref<variable<T> > >(var_ref<variable<T> >(v)); }
// Define all binary operators with first argument being variable and second whatever
#define FOR_EACH_BINARY_OPERATOR(F,S) \
    template <typename T, typename E> \
    inline auto XTL_CONCATENATE(operator,S)(variable<T>& v, E&& e)\
          -> expr<F,var_ref<variable<T> >,decltype(filter(std::forward<E>(e)))>\
    { return expr<F,var_ref<variable<T> >,decltype(filter(std::forward<E>(e)))>(var_ref<variable<T> >(v),filter(std::forward<E>(e))); }
#include "loop_over_operators.hpp"
#undef  FOR_EACH_BINARY_OPERATOR
#undef  FOR_EACH_UNARY_OPERATOR

// Define all unary operators on value
#define FOR_EACH_UNARY_OPERATOR(F,S) \
    template <typename T> \
    inline auto XTL_CONCATENATE(operator,S)(const value<T>& v)\
          -> expr<F,value<T> >\
    { return expr<F,value<T> >(v); }
// Define all binary operators with first argument being value and second whatever
#define FOR_EACH_BINARY_OPERATOR(F,S) \
    template <typename T, typename E> \
    inline auto XTL_CONCATENATE(operator,S)(const value<T>& v, E&& e)\
          -> expr<F,value<T>,decltype(filter(std::forward<E>(e)))>\
    { return expr<F,value<T>,decltype(filter(std::forward<E>(e)))>(v,filter(std::forward<E>(e))); }
#include "loop_over_operators.hpp"
#undef  FOR_EACH_BINARY_OPERATOR
#undef  FOR_EACH_UNARY_OPERATOR

// Define all unary operators on unary expressions
#define FOR_EACH_UNARY_OPERATOR(F,S) \
    template <typename F1, typename E1> \
    inline auto XTL_CONCATENATE(operator,S)(const expr<F1,E1>& v)\
          -> expr<F,expr<F1,E1> >\
    { return expr<F,expr<F1,E1> >(v); }
// Define all binary operators with first argument being unary expression and second whatever
#define FOR_EACH_BINARY_OPERATOR(F,S) \
    template <typename F1, typename E1, typename E> \
    inline auto XTL_CONCATENATE(operator,S)(const expr<F1,E1>& v, E&& e)\
          -> expr<F,expr<F1,E1>,decltype(filter(std::forward<E>(e)))>\
    { return expr<F,expr<F1,E1>,decltype(filter(std::forward<E>(e)))>(v,filter(std::forward<E>(e))); }
#include "loop_over_operators.hpp"
#undef  FOR_EACH_BINARY_OPERATOR
#undef  FOR_EACH_UNARY_OPERATOR

// Define all unary operators on binary expressions
#define FOR_EACH_UNARY_OPERATOR(F,S) \
    template <typename F1, typename E1, typename E2> \
    inline auto XTL_CONCATENATE(operator,S)(const expr<F1,E1,E2>& v)\
          -> expr<F,expr<F1,E1,E2> >\
    { return expr<F,expr<F1,E1,E2> >(v); }
// Define all binary operators with first argument being binary expression and second whatever
#define FOR_EACH_BINARY_OPERATOR(F,S) \
    template <typename F1, typename E1, typename E2, typename E> \
    inline auto XTL_CONCATENATE(operator,S)(const expr<F1,E1,E2>& v, E&& e)\
          -> expr<F,expr<F1,E1,E2>,decltype(filter(std::forward<E>(e)))>\
    { return expr<F,expr<F1,E1,E2>,decltype(filter(std::forward<E>(e)))>(v,filter(std::forward<E>(e))); }
#include "loop_over_operators.hpp"
#undef  FOR_EACH_BINARY_OPERATOR
#undef  FOR_EACH_UNARY_OPERATOR

//------------------------------------------------------------------------------

template <typename T> T inline eval(const value<T>& e)    { return e.m_value; }
template <typename T> T inline eval(const variable<T>& e) { return e.m_value; }
template <typename T> T inline eval(const var_ref<T>& e)  { return *e.m_var; }
template <typename T> T inline eval(const var_ref<variable<T> >& e) { return e.m_var->m_value; }
template <typename F, typename E1>              typename expr<F,E1>::result_type    inline eval(const expr<F,E1>&    e) { return F()(eval(e.m_e1)); }
template <typename F, typename E1, typename E2> typename expr<F,E1,E2>::result_type inline eval(const expr<F,E1,E2>& e) { return F()(eval(e.m_e1),eval(e.m_e2)); }

//------------------------------------------------------------------------------

template <typename E>                           struct is_const_expr                { static const bool value = false; };
template <typename T>                           struct is_const_expr<value<T>>      { static const bool value = true; };
template <typename F, typename E1>              struct is_const_expr<expr<F,E1>>    { static const bool value = is_const_expr<E1>::value; };
template <typename F, typename E1, typename E2> struct is_const_expr<expr<F,E1,E2>> { static const bool value = is_const_expr<E1>::value && is_const_expr<E2>::value; };

//------------------------------------------------------------------------------

template <class C, class T, typename R>
inline R apply_member(const C* c, R (T::*method)() const)
{
    DEBUG_ONLY(std::clog << "\nApplying const member function to instance " << c << " of type " << typeid(*c).name() << std::endl);
    return (c->*method)();
}

//------------------------------------------------------------------------------

template <class C, class T, typename R>
inline R apply_member(      C* c, R (T::*method)()      )
{
    DEBUG_ONLY(std::clog << "\nApplying non-const member function to instance " << c << " of type " << typeid(*c).name() << std::endl);
    return (c->*method)();
}

//------------------------------------------------------------------------------

template <class C, class T, typename R>
inline R apply_member(const C* c, R T::*field) throw()
{
    //DEBUG_ONLY(std::clog << "\nApplying data member to instance " << c << " of type " << typeid(*c).name() << std::endl);
    return c->*field;
}

//------------------------------------------------------------------------------

template <class C, class T, typename R>
inline R apply_member(const C* c, R (*func)(const T*))
{
    DEBUG_ONLY(std::clog << "\nApplying external function to const instance " << c << " of type " << typeid(*c).name() << std::endl);
    return (*func)(c);
}

//------------------------------------------------------------------------------

template <class C, class T, typename R>
inline R apply_member(      C* c, R (*func)(      T*))
{
    DEBUG_ONLY(std::clog << "\nApplying external function to non-const instance " << c << " of type " << typeid(*c).name() << std::endl);
    return (*func)(c);
}

//------------------------------------------------------------------------------

/// We need this extra indirection to be able to intercept when we are trying to 
/// match a meta variable _ of type wildcard, that matches everything of
/// any type. In this case we don't even want to invoke the underlain member!
template <typename E, typename C, typename M>
inline bool apply_expression(const E& e, const C* c, M m)
{
    #ifdef _MSC_VER
    #pragma warning( disable : 4800 )
    #endif

    return e(apply_member(c, m));
}

template <typename E, typename C, typename M>
inline bool apply_expression(const E& e,       C* c, M m)
{
    #ifdef _MSC_VER
    #pragma warning( disable : 4800 )
    #endif

    return e(apply_member(c, m));
}

/// This is the specialization that makes the member not to be invoked when we
/// are matching against the meta variable _ that matches everything.
template <typename C, typename M>
inline bool apply_expression(const var_ref<wildcard>&, const C*, M)
{
    return true;
}

template <typename C, typename M>
inline bool apply_expression(const var_ref<wildcard>&,       C*, M) throw()
{
    return true;
}

//------------------------------------------------------------------------------

template <typename T, size_t layout>
struct matcher0
{
    const T* operator()(const T* t) const throw() { return t; }
          T* operator()(      T* t) const throw() { return t; }
    template <typename U> const T* operator()(const U* u) const throw() { return dynamic_cast<const T*>(u); }
    template <typename U>       T* operator()(      U* u) const throw() { return dynamic_cast<      T*>(u); }
    template <typename U> const T* operator()(const U& u) const throw() { return operator()(&u); }
    template <typename U>       T* operator()(      U& u) const throw() { return operator()(&u); }

    template <typename E> expr_or<matcher0,E> operator|(const E& e) const throw() { return expr_or<matcher0,E>(*this,e); }
};

//------------------------------------------------------------------------------

template <typename T, size_t layout, typename E1>
struct matcher1
{
    matcher1(const E1& e1) : m_e1(e1) {}
    matcher1(E1&& e1) : m_e1(std::move(e1)) {}
    matcher1(matcher1&& src) : m_e1(std::move(src.m_e1)) {}
    const T* operator()(const T* t) const 
    {
        return 
            (t                                                             &&
             apply_expression(m_e1, t, match_members<T,layout>::member0())
            )
            ? t
            : 0;
    }
          T* operator()(      T* t) const 
    {
        return 
            (t                                                             &&
             apply_expression(m_e1, t, match_members<T,layout>::member0())
            )
            ? t
            : 0;
    }

    template <typename U> const T* operator()(const U* u) const { return operator()(dynamic_cast<const T*>(u)); }
    template <typename U>       T* operator()(      U* u) const { return operator()(dynamic_cast<      T*>(u)); }
    template <typename U> const T* operator()(const U& u) const { return operator()(&u); }
    template <typename U>       T* operator()(      U& u) const { return operator()(&u); }

    template <typename E> expr_or<matcher1,E> operator|(const E& e) const throw() { return expr_or<matcher1,E>(*this,e); }
    const E1 m_e1;
};

//------------------------------------------------------------------------------

template <typename T, size_t layout, typename E1, typename E2>
struct matcher2
{
    matcher2(const E1& e1, const E2& e2) : m_e1(e1), m_e2(e2) {}
    matcher2(E1&& e1, E2&& e2) : m_e1(std::move(e1)), m_e2(std::move(e2)) {}
    matcher2(matcher2&& src) : m_e1(std::move(src.m_e1)), m_e2(std::move(src.m_e2)) {}

    const T* operator()(const T* t) const 
    {
        return 
            (t                                                             &&
             apply_expression(m_e1, t, match_members<T,layout>::member0()) &&
             apply_expression(m_e2, t, match_members<T,layout>::member1())
            )
            ? t
            : 0;
    }
          T* operator()(      T* t) const 
    {
        return 
            (t                                                             &&
             apply_expression(m_e1, t, match_members<T,layout>::member0()) &&
             apply_expression(m_e2, t, match_members<T,layout>::member1())
            )
            ? t
            : 0;
    }

    template <typename U> const T* operator()(const U* u) const { return operator()(dynamic_cast<const T*>(u)); }
    template <typename U>       T* operator()(      U* u) const { return operator()(dynamic_cast<      T*>(u)); }
    template <typename U> const T* operator()(const U& u) const { return operator()(&u); }
    template <typename U>       T* operator()(      U& u) const { return operator()(&u); }

    template <typename E> expr_or<matcher2,E> operator|(const E& e) const throw() { return expr_or<matcher2,E>(*this,e); }
    const E1 m_e1;
    const E2 m_e2;
};

//------------------------------------------------------------------------------

template <typename T, size_t layout, typename E1, typename E2, typename E3>
struct matcher3
{
    matcher3(const E1& e1, const E2& e2, const E3& e3) : m_e1(e1), m_e2(e2), m_e3(e3) {}
    matcher3(E1&& e1, E2&& e2, E3&& e3) : m_e1(std::move(e1)), m_e2(std::move(e2)), m_e3(std::move(e3)) {}
    matcher3(matcher3&& src) : m_e1(std::move(src.m_e1)), m_e2(std::move(src.m_e2)), m_e3(std::move(src.m_e3)) {}

    const T* operator()(const T* t) const 
    {
        return 
            (t                                                             &&
             apply_expression(m_e1, t, match_members<T,layout>::member0()) &&
             apply_expression(m_e2, t, match_members<T,layout>::member1()) &&
             apply_expression(m_e3, t, match_members<T,layout>::member2())
            )
            ? t
            : 0;
    }
          T* operator()(      T* t) const 
    {
        return 
            (t                                                             &&
             apply_expression(m_e1, t, match_members<T,layout>::member0()) &&
             apply_expression(m_e2, t, match_members<T,layout>::member1()) &&
             apply_expression(m_e3, t, match_members<T,layout>::member2())
            )
            ? t
            : 0;
    }

    template <typename U> const T* operator()(const U* u) const { return operator()(dynamic_cast<const T*>(u)); }
    template <typename U>       T* operator()(      U* u) const { return operator()(dynamic_cast<      T*>(u)); }
    template <typename U> const T* operator()(const U& u) const { return operator()(&u); }
    template <typename U>       T* operator()(      U& u) const { return operator()(&u); }

    template <typename E> expr_or<matcher3,E> operator|(const E& e) const throw() { return expr_or<matcher3,E>(*this,e); }
    const E1 m_e1;
    const E2 m_e2;
    const E3 m_e3;
};

//------------------------------------------------------------------------------

template <typename T, size_t layout, typename E1, typename E2, typename E3, typename E4>
struct matcher4
{
    matcher4(const E1& e1, const E2& e2, const E3& e3, const E4& e4) : m_e1(e1), m_e2(e2), m_e3(e3), m_e4(e4) {}
    matcher4(E1&& e1, E2&& e2, E3&& e3, E4&& e4) : m_e1(std::move(e1)), m_e2(std::move(e2)), m_e3(std::move(e3)), m_e4(std::move(e4)) {}
    matcher4(matcher4&& src) : m_e1(std::move(src.m_e1)), m_e2(std::move(src.m_e2)), m_e3(std::move(src.m_e3)), m_e4(std::move(src.m_e4)) {}

    const T* operator()(const T* t) const 
    {
        return 
            (t                                                             &&
             apply_expression(m_e1, t, match_members<T,layout>::member0()) &&
             apply_expression(m_e2, t, match_members<T,layout>::member1()) &&
             apply_expression(m_e3, t, match_members<T,layout>::member2()) &&
             apply_expression(m_e4, t, match_members<T,layout>::member3())
            )
            ? t
            : 0;
    }
          T* operator()(      T* t) const 
    {
        return 
            (t                                                             &&
             apply_expression(m_e1, t, match_members<T,layout>::member0()) &&
             apply_expression(m_e2, t, match_members<T,layout>::member1()) &&
             apply_expression(m_e3, t, match_members<T,layout>::member2()) &&
             apply_expression(m_e4, t, match_members<T,layout>::member3())
            )
            ? t
            : 0;
    }

    template <typename U> const T* operator()(const U* u) const { return operator()(dynamic_cast<const T*>(u)); }
    template <typename U>       T* operator()(      U* u) const { return operator()(dynamic_cast<      T*>(u)); }
    template <typename U> const T* operator()(const U& u) const { return operator()(&u); }
    template <typename U>       T* operator()(      U& u) const { return operator()(&u); }

    template <typename E> expr_or<matcher4,E> operator|(const E& e) const throw() { return expr_or<matcher4,E>(*this,e); }
    const E1 m_e1;
    const E2 m_e2;
    const E3 m_e3;
    const E4 m_e4;
};

//------------------------------------------------------------------------------

template <typename T>
inline matcher0<T,default_layout> match() throw()
{
    return matcher0<T,default_layout>();
}

template <typename T, size_t layout>
inline matcher0<T,layout> match() throw()
{
    return matcher0<T,layout>();
}

//------------------------------------------------------------------------------

template <typename T, size_t layout, typename E1>
inline matcher1<T,layout,E1> match_ex(E1&& e1) throw()
{
    return matcher1<T,layout,E1>(std::forward<E1>(e1));
}

template <typename T, typename E1>
inline auto match(E1&& e1) throw() -> decltype(match_ex<T,default_layout>(filter(std::forward<E1>(e1))))
{
    return match_ex<T,default_layout>(filter(std::forward<E1>(e1)));
}

template <typename T, size_t layout, typename E1>
inline auto match(E1&& e1) throw() -> decltype(match_ex<T,layout>(filter(std::forward<E1>(e1))))
{
    return match_ex<T,layout>(filter(std::forward<E1>(e1)));
}

//------------------------------------------------------------------------------

template <typename T, size_t layout, typename E1, typename E2>
inline matcher2<T,layout,E1,E2> match_ex(E1&& e1, E2&& e2) throw()
{
    return matcher2<T,layout,E1,E2>(std::forward<E1>(e1),std::forward<E2>(e2));
}

template <typename T, typename E1, typename E2>
inline auto match(E1&& e1, E2&& e2) throw() -> decltype(match_ex<T,default_layout>(filter(std::forward<E1>(e1)), filter(std::forward<E2>(e2))))
{
    return match_ex<T,default_layout>(filter(std::forward<E1>(e1)), filter(std::forward<E2>(e2)));
}

template <typename T, size_t layout, typename E1, typename E2>
inline auto match(E1&& e1, E2&& e2) throw() -> decltype(match_ex<T,layout>(filter(std::forward<E1>(e1)), filter(std::forward<E2>(e2))))
{
    return match_ex<T,layout>(filter(std::forward<E1>(e1)), filter(std::forward<E2>(e2)));
}

//------------------------------------------------------------------------------

template <typename T, size_t layout, typename E1, typename E2, typename E3>
inline matcher3<T,layout,E1,E2,E3> match_ex(E1&& e1, E2&& e2, E3&& e3) throw()
{
    return matcher3<T,layout,E1,E2,E3>(std::forward<E1>(e1),std::forward<E2>(e2),std::forward<E3>(e3));
}

template <typename T, typename E1, typename E2, typename E3>
inline auto match(E1&& e1, E2&& e2, E3&& e3) throw() -> decltype(match_ex<T,default_layout>(filter(std::forward<E1>(e1)), filter(std::forward<E2>(e2)), filter(std::forward<E3>(e3))))
{
    return match_ex<T,default_layout>(filter(std::forward<E1>(e1)), filter(std::forward<E2>(e2)), filter(std::forward<E3>(e3)));
}

template <typename T, size_t layout, typename E1, typename E2, typename E3>
inline auto match(E1&& e1, E2&& e2, E3&& e3) throw() -> decltype(match_ex<T,layout>(filter(std::forward<E1>(e1)), filter(std::forward<E2>(e2)), filter(std::forward<E3>(e3))))
{
    return match_ex<T,layout>(filter(std::forward<E1>(e1)), filter(std::forward<E2>(e2)), filter(std::forward<E3>(e3)));
}

//------------------------------------------------------------------------------

template <typename T, size_t layout, typename E1, typename E2, typename E3, typename E4>
inline matcher4<T,layout,E1,E2,E3,E4> match_ex(E1&& e1, E2&& e2, E3&& e3, E4&& e4) throw()
{
    return matcher4<T,layout,E1,E2,E3,E4>(std::forward<E1>(e1),std::forward<E2>(e2),std::forward<E3>(e3),std::forward<E4>(e4));
}

template <typename T, typename E1, typename E2, typename E3, typename E4>
inline auto match(E1&& e1, E2&& e2, E3&& e3, E4&& e4) throw() -> decltype(match_ex<T,default_layout>(filter(std::forward<E1>(e1)), filter(std::forward<E2>(e2)), filter(std::forward<E3>(e3)), filter(std::forward<E4>(e4))))
{
    return match_ex<T,default_layout>(filter(std::forward<E1>(e1)), filter(std::forward<E2>(e2)), filter(std::forward<E3>(e3)), filter(std::forward<E4>(e4)));
}

template <typename T, size_t layout, typename E1, typename E2, typename E3, typename E4>
inline auto match(E1&& e1, E2&& e2, E3&& e3, E4&& e4) throw() -> decltype(match_ex<T,layout>(filter(std::forward<E1>(e1)), filter(std::forward<E2>(e2)), filter(std::forward<E3>(e3)), filter(std::forward<E4>(e4))))
{
    return match_ex<T,layout>(filter(std::forward<E1>(e1)), filter(std::forward<E2>(e2)), filter(std::forward<E3>(e3)), filter(std::forward<E4>(e4)));
}

//------------------------------------------------------------------------------

