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
#include <type_traits>
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
template <typename T> object_of<T>  obj_from(const T& t) { return object_of<T>(t); }

//------------------------------------------------------------------------------

template <typename T>
inline typename std::enable_if<std::is_polymorphic<T>::value, const T*>::type
unbox(const object& obj)
{
    if (typeid(obj) == typeid(T))
        return static_cast<const T*>(&obj);
//    else // VC complains deeply in instantiation
//    if (typeid(obj) == typeid(object_of<T>))
//        return &static_cast<const object_of<T>&>(obj).m_value;
    else
        return dynamic_cast<const T*>(&obj);
}

//------------------------------------------------------------------------------

template <typename T>
inline typename std::enable_if<!std::is_polymorphic<T>::value, const T*>::type
unbox(const object& obj)
{
    if (typeid(obj) == typeid(object_of<T>))
        return &static_cast<const object_of<T>&>(obj).m_value;
    else
        return false;
}

//------------------------------------------------------------------------------

struct pattern
{
    virtual ~pattern() {}
    virtual bool matches(const object&) = 0;
};

//------------------------------------------------------------------------------

struct expression_pattern : pattern
{
    virtual bool current_value_is(const object&) const = 0;
};

//------------------------------------------------------------------------------

struct wildcard : pattern
{
    virtual bool matches(const object&) { return true; }
};

//------------------------------------------------------------------------------

struct value : expression_pattern
{
    value(const object& obj) : m_obj(obj) {}
    virtual bool matches(const object& obj) { return m_obj == obj; }
    virtual bool current_value_is(const object& obj) const { return m_obj == obj; }

private:
    value& operator=(const value&); ///< No assignment operator

    const object& m_obj;
};

//------------------------------------------------------------------------------

template <typename T> value val(const T& t) { return value(*make_obj(t)); }

//------------------------------------------------------------------------------

template <typename T>
struct value_of : expression_pattern
{
    value_of(const T& t) : m_value(t) {}
    virtual bool matches(const object& obj) { return current_value_is(obj); }
    virtual bool current_value_is(const object& obj) const
    {
        if (const T* p = unbox<T>(obj))
            return *p == m_value;
        else
            return false; 
    }
    const T m_value;

private:
    value_of& operator=(const value_of&); ///< No assignment operator
};

//------------------------------------------------------------------------------

struct variable : expression_pattern
{
    variable() : m_obj_ref(0) {}
    virtual bool matches(const object& obj) 
    {
        m_obj_ref = const_cast<object*>(&obj); 
        return true;
    }
    virtual bool current_value_is(const object& obj) const
    {
        return m_obj_ref && *m_obj_ref == obj; 
    }
    object* m_obj_ref;

private:
    variable& operator=(const variable&); // = delete;
};

//------------------------------------------------------------------------------

//variable var(object*& obj) { return variable(obj); }

//------------------------------------------------------------------------------

template <typename T>
struct ref_of : expression_pattern
{
    ref_of() : m_var(0) {}
    virtual bool matches(const object& obj)
    {
        if (const T* p = unbox<T>(obj))
        {
            m_var = p;
            return true;
        }
        else
            return false; 
    }
    virtual bool current_value_is(const object& obj) const
    {
        return m_var && *m_var == obj; 
    }
    const T* m_var;
};

//------------------------------------------------------------------------------

template <typename T>
struct var_of : expression_pattern
{
    var_of() : m_var() {}
    var_of(const T& v) : m_var(v) {}
    virtual bool matches(const object& obj)
    {
        if (const T* p = unbox<T>(obj))
        {
            m_var = *p;
            return true;
        }
        else
            return false; 
    }
    virtual bool current_value_is(const object& obj) const
    {
        const T* p = unbox<T>(obj);
        return p && *p == m_var; 
    }
    operator const T&() const { return m_var; }
    operator       T&()       { return m_var; }
    T m_var;
};

//------------------------------------------------------------------------------

/// Traits like structure that will define which members should be matched at
/// which positions. It is intentionally left undefined as user will have to
/// provide specializations for his hierarchy.
//template <typename type_being_matched> struct bindings;
#include "patterns/bindings.hpp"

//------------------------------------------------------------------------------

template <typename T>
struct cls_of0 : pattern
{
    virtual bool matches(const object& obj)
    {
        return unbox<T>(obj) != 0;
    }
};

//------------------------------------------------------------------------------

template <typename T>
struct cls_of1 : pattern
{
    cls_of1(pattern& p1) : m_p1(p1) {}
    virtual bool matches(const object& obj)
    {
        if (const T* p = unbox<T>(obj))
        {
            const object& a1 = mch::apply_member(p,mch::bindings<T>::member0());
            return m_p1.matches(a1);
        }
        else
            return false; 
    }

private:
    cls_of1& operator=(const cls_of1&); ///< No assignment operator

    pattern& m_p1;
};

//------------------------------------------------------------------------------

template <typename T>
struct cls_of2 : pattern
{
    cls_of2(pattern& p1, pattern& p2) : m_p1(p1), m_p2(p2) {}
    virtual bool matches(const object& obj)
    {
        if (const T* p = unbox<T>(obj))
            return m_p1.matches(mch::apply_member(p,mch::bindings<T>::member0()))
                && m_p2.matches(mch::apply_member(p,mch::bindings<T>::member1()));
        else
            return false; 
    }

private:
    cls_of2& operator=(const cls_of2&); ///< No assignment operator

    pattern& m_p1;
    pattern& m_p2;
};

//------------------------------------------------------------------------------

template <typename T>
struct p_plus_c : pattern
{
    p_plus_c(pattern& p, const T& t) : m_p(p), m_c(t) {}
    virtual bool matches(const object& obj)
    {
        if (const T* p = unbox<T>(obj))
        {
            if (std::is_unsigned<T>::value && m_c > *p) 
                return false;
            else
            {
                object_of<T> a = *p - m_c;
                return m_p.matches(a);
            }
        }
        else
            return false; 
    }

private:
    p_plus_c& operator=(const p_plus_c&); ///< No assignment operator

    pattern& m_p;
    T        m_c;
};

//------------------------------------------------------------------------------

template <typename T>
struct c_minus_p : pattern
{
    c_minus_p(const T& t, pattern& p) : m_p(p), m_c(t) {}
    virtual bool matches(const object& obj)
    {
        if (const T* p = unbox<T>(obj))
        {
            if (std::is_unsigned<T>::value && m_c < *p) 
                return false;
            else
            {
                object_of<T> a = m_c-*p;
                return m_p.matches(a);
            }
        }
        else
            return false; 
    }

private:
    c_minus_p& operator=(const c_minus_p&); ///< No assignment operator

    pattern& m_p;
    T        m_c;
};

//------------------------------------------------------------------------------

template <typename T>
struct p_times_c : pattern
{
    p_times_c(pattern& p, const T& t) : m_p(p), m_c(t) {}
    virtual bool matches(const object& obj)
    {
        if (const T* p = unbox<T>(obj))
        {
            if (std::is_integral<T>::value && *p % m_c != 0) 
                return false;
            else
                return m_p.matches(object_of<T>(*p/m_c));
        }
        else
            return false; 
    }

private:
    p_times_c& operator=(const p_times_c&); ///< No assignment operator

    pattern& m_p;
    T        m_c;
};

//------------------------------------------------------------------------------

struct equivalence : expression_pattern
{
    equivalence(expression_pattern& p) : m_p(p) {}
    virtual bool matches(const object& obj) { return current_value_is(obj); }
    virtual bool current_value_is(const object& obj) const { return m_p.current_value_is(obj); }

private:
    equivalence& operator=(const equivalence&); ///< No assignment operator

    expression_pattern& m_p;
};

//------------------------------------------------------------------------------
