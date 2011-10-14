#include <iostream>
#include <unordered_map>

typedef std::unordered_map<intptr_t, ptrdiff_t> vtbl2offset;
static const ptrdiff_t no_cast_exists = 0x0FF1C1A1; ///< A dedicated constant marking impossible offset

template <typename T> struct cast_target;
template <typename T> struct cast_target<T*> { typedef T type; };
template <typename T> struct cast_target<const T*> { typedef T type; };

/// A generic wrapper to obtain an offset of dynamic_cast from type U* to type T*.
/// We pass this function by address as it is the slowest part anyways, so one 
/// extra function call is not going make it slower, but will save on generated code.
template <typename T, typename U>
inline ptrdiff_t dyn_cast_offset(const void* p)
{
    T k = dynamic_cast<T>(static_cast<const U*>(p));
    return k ? reinterpret_cast<const char*>(k)-reinterpret_cast<const char*>(p) : no_cast_exists;
}

/// Helper function to get_offset that contains template independent part of it.
inline ptrdiff_t get_offset_ex(const void* p, ptrdiff_t (*dyn_off_getter)(const void*), vtbl2offset& ofsmap)
{
    const intptr_t vtbl = *reinterpret_cast<const intptr_t*>(p);
    const vtbl2offset::iterator q = ofsmap.find(vtbl);

    if (q != ofsmap.end())
        return q->second;
    else
    {
        const ptrdiff_t offset = dyn_off_getter(p);
        ofsmap.insert(vtbl2offset::value_type(vtbl,offset));
        return offset;
    }
}

/// This function is parameterized over T because we keep one hash map of vtbl 
/// to offsets per target type T.
template <typename T>
inline ptrdiff_t get_offset(const void* p, ptrdiff_t (*dyn_off_getter)(const void*))
{
    static vtbl2offset ofsmap;
    return get_offset_ex(p, dyn_off_getter, ofsmap);
}

// Number of bits based on which the caching will be made. The cache will have
// 2^cache_bits entries.
const int cache_bits = 3;
const int cache_mask = (1<<cache_bits)-1;

#ifdef _MSC_VER
const int irrelevant_bits = 3;  // vtbl in MSVC seem to be alligned by 4 bytes
#else
const int irrelevant_bits = 4;  // vtbl in G++ seem to be alligned by 8 bytes
#endif

/// Structure describing entry in the cache
struct cache_entry
{
    intptr_t  vtbl;   ///< vtbl for which offset has been computed
    ptrdiff_t offset; ///< offset that has to be added for ptr with given vtbl
};

inline intptr_t adjust_ptr(const void* p, ptrdiff_t (*dyn_off_getter)(const void*), vtbl2offset& ofsmap, cache_entry cache[1<<cache_bits])
{
    if (p)
    {
        const intptr_t vtbl = *reinterpret_cast<const intptr_t*>(p);
        cache_entry& ce = cache[(vtbl>>irrelevant_bits) & cache_mask];
        ptrdiff_t offset;

        if (ce.vtbl == vtbl)
            offset = ce.offset;
        else
        {
            ce.vtbl = vtbl;
            ce.offset = offset = get_offset_ex(p, dyn_off_getter, ofsmap);
        }

        return offset == no_cast_exists ? 0 : reinterpret_cast<intptr_t>(p)+offset;
    }

    return 0;
}

template <typename T>
inline T memoized_cast_ex(const void* p, ptrdiff_t (*dyn_off_getter)(const void*))
{
    static vtbl2offset ofsmap;                    // One per T
    static cache_entry cache[1<<cache_bits] = {}; // One per T
    return reinterpret_cast<T>(adjust_ptr(p, dyn_off_getter, ofsmap, cache));
}

template <typename T, typename U>
inline T memoized_cast(const U* u)
{
    return memoized_cast_ex<T>(u,&dyn_cast_offset<T,U>);
}

template <typename T, typename U>
inline T memoized_cast(U* u)
{
    return memoized_cast_ex<typename cast_target<T>::type const*>(u,&dyn_cast_offset<T,U>);
}
