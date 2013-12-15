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

#include <typeinfo>
//#include <utility>
#include <ostream>

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
//private:
    T m_value;
};

//------------------------------------------------------------------------------

template <typename T> object_of<T>* make_obj(const T& t) { return new object_of<T>(t); }

//------------------------------------------------------------------------------

struct pattern
{
    virtual ~pattern() {}
    virtual bool matches(const object&) = 0;
};

//------------------------------------------------------------------------------

struct wildcard : pattern
{
    virtual bool matches(const object&) { return true; }
};

//------------------------------------------------------------------------------

struct value : pattern
{
    value(const object& obj) : m_obj(obj) {}
    virtual bool matches(const object& obj) { return m_obj == obj; }
    const object& m_obj;
};

//------------------------------------------------------------------------------

template <typename T> value val(const T& t) { return value(*make_obj(t)); }

//------------------------------------------------------------------------------

struct variable : pattern
{
    variable() : m_obj_ref(0) {}
    virtual bool matches(const object& obj) 
    {
        m_obj_ref = const_cast<object*>(&obj); 
        return true;
    }
    object* m_obj_ref;
private:
    variable& operator=(const variable&); // = delete;
};

//------------------------------------------------------------------------------

//variable var(object*& obj) { return variable(obj); }

//------------------------------------------------------------------------------
