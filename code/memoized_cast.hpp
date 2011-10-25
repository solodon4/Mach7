///
/// \file memoized_cast.hpp
///
/// This file defines function memoized_cast<T>(U) that behaves as dynamic_cast
/// but is much faster when multiple invokations are involved because of caching
///
/// \autor Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of the XTL framework (http://parasol.tamu.edu/xtl/).
/// Copyright (C) 2005-2011 Texas A&M University.
/// All rights reserved.
///

#include "vtblmap.hpp"

// TODO: 
// 1. vtbl with pointers directly to table instead of indecies - slows
// 2. store type index inside match_members
// 3. try smaller type int instead of size_t or ptrdiff_t
// 4. Problem: different amounts of virtual functions in the base class 
//             change the irrelevant bits. 
//    - This shouldn't be a problem in compiler implementation of pattern
//      matching as compiler will know this value
//    - For now we can make it a parameter of switch as well
//    - Try using Pearson hash to avoid dependence on irrelevant bits
// 5. Preallocate vtbl map - improves sequential but slows down random, 
//    probably because of locality.

template <typename U>
struct specific_to
{
    static inline size_t allocate_index() { return ++index; }

    template <typename T>
    static inline size_t type_index_of()
    {
        static const size_t ti = allocate_index();
        return ti;
    }

private:

    static size_t index;

};

//------------------------------------------------------------------------------

template <typename T> struct cast_target;
template <typename T> struct cast_target<      T*> { typedef T type; };
template <typename T> struct cast_target<const T*> { typedef T type; };

//------------------------------------------------------------------------------

static const std::ptrdiff_t no_cast_exists = 0x0FF1C1A1; // A dedicated constant marking impossible offset
static const std::ptrdiff_t unknown_offset = 0x0FF1C1A0; // A dedicated constant marking an offset that hasn't been computed yet

//------------------------------------------------------------------------------

struct dyn_cast_info
{
    dyn_cast_info() : offset(unknown_offset) {}
    std::ptrdiff_t offset;
};

//------------------------------------------------------------------------------

typedef vtblmap<dyn_cast_info&> vtbl2offset;

//------------------------------------------------------------------------------

template <typename T>
inline vtbl2offset& offset_map()
{
    static vtbl2offset ofsmap;
    return ofsmap;
}

//------------------------------------------------------------------------------

template <typename T>
inline std::ptrdiff_t& get_offset(const void* p)
{
    return offset_map<T>().get(p).offset;
}

//------------------------------------------------------------------------------

template <typename T, typename S>
inline T memoized_cast_non_null(const S* p)
{
    typedef S source_type;
    typedef typename cast_target<T>::type target_type;
    std::ptrdiff_t& offset = get_offset<target_type>(p);

    if (XTL_UNLIKELY(offset == unknown_offset))
    {
        T k = dynamic_cast<T>(p);
        offset = k 
                 ? reinterpret_cast<const char*>(k)-reinterpret_cast<const char*>(p) 
                 : no_cast_exists;
    }

    return offset == no_cast_exists ? 0 : adjust_ptr<target_type>(p, offset);
}

//------------------------------------------------------------------------------

template <typename T, typename U>
inline T memoized_cast_non_null(U* u)
{
    return 
        const_cast<T>(
            memoized_cast_non_null<typename cast_target<T >::type const*>(
                       static_cast<typename cast_target<U*>::type const*>(u)
            )
        );
}

//------------------------------------------------------------------------------

template <typename T, typename U>
inline T memoized_cast(U* u)
{
    return  u
            ? memoized_cast_non_null<T>(u) 
            : 0;
}

//------------------------------------------------------------------------------
