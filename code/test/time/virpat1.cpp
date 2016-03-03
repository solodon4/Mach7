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

#include <typeinfo>
#include <utility>
#include <ostream>
#include <mach7/config.hpp>                // Mach7 configuration
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
        //if (const object_of* o = dynamic_cast<const object_of*>(&obj))
        if (typeid(*this) == typeid(obj))
        {
            const object_of* o = static_cast<const object_of*>(&obj);
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
    std::cout << "r=" << r << " u=" << u << " timing=" << mch::cycles(liFinish1-liStart1)/T << " cycles/iteration" << std::endl;
}

//------------------------------------------------------------------------------
