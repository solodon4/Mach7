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

#include <utility>
#include <ostream>
#include "config.hpp"
#include "timing.hpp"

//------------------------------------------------------------------------------

#if defined(XTL_TIMING_METHOD_1)
    XTL_MESSAGE("Timing method 1: based on QueryPerformanceCounter()")
#elif defined(XTL_TIMING_METHOD_2)
    XTL_MESSAGE("Timing method 2: based on rdtsc register")
#elif defined(XTL_TIMING_METHOD_3)
    XTL_MESSAGE("Timing method 3: based on clock()")
#endif

//------------------------------------------------------------------------------

/// CONS: We need to foresee a root hierarchy before matching
struct object
{
    virtual ~object() {}
    virtual bool operator==(const object&) const = 0; ///< CONST: And make assumption about some operations
    virtual std::ostream& operator>>(std::ostream& os) const = 0;
    friend  std::ostream& operator<<(std::ostream& os, const object& obj) 
    {
        if (&obj)
            return obj >> os;
        else
            return os << "nullptr";
    }
};

//------------------------------------------------------------------------------

template <typename T>
class object_of : public object
{
public:
    object_of(const T& t) : m_value(t) {}
    virtual bool operator==(const object& obj) const
    {
        if (const object_of* o = dynamic_cast<const object_of*>(&obj))
        {
            return m_value == o->m_value;
        }

        return false;
    }
    virtual std::ostream& operator>>(std::ostream& os) const
    {
        return os << m_value;
    }
private:
    T m_value;
};

//------------------------------------------------------------------------------

template <typename T>
object_of<T>* make_obj(const T& t)
{
    return new object_of<T>(t);
}

//------------------------------------------------------------------------------

struct pattern
{
    virtual ~pattern() {}
    virtual bool matches(const object*) = 0;
};

//------------------------------------------------------------------------------

struct wildcard_pattern : pattern
{
    virtual bool matches(const object*) { return true; }
};

//------------------------------------------------------------------------------

struct value_pattern : pattern
{
    virtual ~value_pattern() { delete m_obj; }
    value_pattern(object* obj) : m_obj(obj) {}
    virtual bool matches(const object* obj) 
    {
        return *m_obj == *obj; 
    }
    object* m_obj;
};

//------------------------------------------------------------------------------

template <typename T>
value_pattern* val(const T& t)
{
    return new value_pattern(make_obj(t));
}

//------------------------------------------------------------------------------

struct variable_pattern : pattern
{
    variable_pattern(object*& obj_ref) : m_obj_ref(obj_ref) {}
    virtual bool matches(const object* obj) 
    {
        m_obj_ref = const_cast<object*>(obj); 
        return true;
    }
    object*& m_obj_ref;
private:
    variable_pattern& operator=(const variable_pattern&); // = delete;
};

//------------------------------------------------------------------------------

variable_pattern* var(object*& obj)
{
    return new variable_pattern(obj);
}

//------------------------------------------------------------------------------

#include <iostream>

//------------------------------------------------------------------------------

const size_t T = 10000000;

//------------------------------------------------------------------------------

int main()
{
    object* val1 = make_obj(7);
    object* val2 = make_obj(42);
    object* val3 = 0;
    pattern* pat1 = val(42);
    pattern* pat2 = var(val3);
    std::cout << std::boolalpha << (*val1==*val2) << std::endl;
    std::cout << std::boolalpha << pat1->matches(val1) << std::endl;
    std::cout << std::boolalpha << pat2->matches(val2) << " binds " << *val3 << std::endl;
    std::cout << " binds " << *val3 << std::endl;

    int tests[]   = {2,3,5,7,11,13,17,19,23,29,31};
    const size_t N = XTL_ARR_SIZE(tests);
    object* objects[N] = {};

    for (size_t i = 0; i < N; ++i)
        objects[i] = make_obj(tests[i]);

    int r = 0;
    size_t u = 0;

    object*  v = 0;
    pattern* val_pat = val(17);
    pattern* var_pat = var(v);
    pattern*  wc_pat = new wildcard_pattern;

    mch::time_stamp liStart1 = mch::get_time_stamp();

    for (size_t j = 0; j < T; ++j)
        for (size_t i = 0; i < N; ++i)
        {
            if (val_pat->matches(objects[i])) r+=1;
            if (var_pat->matches(objects[i])) r+=2;
            if ( wc_pat->matches(objects[i])) r+=4;
            u += r;
            //std::cout << "r=" << r << " u=" << u << std::endl;
        }

    mch::time_stamp liFinish1 = mch::get_time_stamp();
    std::cout << liStart1 << '-' << liFinish1 << ':' << (liFinish1-liStart1) << std::endl;
    std::cout << "outcome=" << r << " xxx=" << u << " timing=" << mch::cycles(liFinish1-liStart1)/T << " cycles/iteration" << std::endl;
}

//------------------------------------------------------------------------------
