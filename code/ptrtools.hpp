///
/// \file ptrtools.hpp
///
/// This file defines several utility functions for working with pointers,
/// especially pointers to (polymorphic) objects, that we use in XTL
///
/// \autor Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of the XTL framework (http://parasol.tamu.edu/xtl/).
/// Copyright (C) 2011 Texas A&M University.
/// All rights reserved.
///

#pragma once

#include <typeinfo>

//------------------------------------------------------------------------------

template <typename T> inline const T* adjust_ptr(const void* p, ptrdiff_t offset) { return  reinterpret_cast<const T*>(reinterpret_cast<const char*>(p)+offset); }
template <typename T> inline       T* adjust_ptr(      void* p, ptrdiff_t offset) { return  reinterpret_cast<      T*>(reinterpret_cast<      char*>(p)+offset); }

//------------------------------------------------------------------------------

template <typename T> inline const T* addr(const T* t) { return  t; }
template <typename T> inline       T* addr(      T* t) { return  t; }
template <typename T> inline const T* addr(const T& t) { return &t; }
template <typename T> inline       T* addr(      T& t) { return &t; }

//------------------------------------------------------------------------------

template <typename T, typename U> inline const T* stat_cast(const U* p) { return static_cast<const T*>(p); }
template <typename T, typename U> inline       T* stat_cast(      U* p) { return static_cast<      T*>(p); }

//------------------------------------------------------------------------------

template <typename T> inline const std::type_info& vtbl_typeid(intptr_t vtbl) { return typeid(*reinterpret_cast<const T*>(&vtbl)); }
template <typename T> inline const std::type_info& vtbl_typeid(const void* p) { return vtbl_typeid<T>(*reinterpret_cast<const intptr_t*>(p)); }

struct polymorphic_dummy { virtual ~polymorphic_dummy(){} };

inline const std::type_info& vtbl_typeid(intptr_t vtbl) { return vtbl_typeid<polymorphic_dummy>(vtbl); }
inline const std::type_info& vtbl_typeid(const void* p) { return vtbl_typeid<polymorphic_dummy>(p); }

//------------------------------------------------------------------------------

template <int N> struct requires_bits    { enum { value = requires_bits<(N+1)/2>::value+1 }; };
template <>      struct requires_bits<1> { enum { value = 0 }; };

//------------------------------------------------------------------------------
