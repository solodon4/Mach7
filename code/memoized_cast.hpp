#include <iostream>
#include <unordered_map>

typedef std::unordered_map<intptr_t, ptrdiff_t> vtbl2offset;
static const ptrdiff_t no_cast_exists = 0x0FF1C1A1; // A dedicated constant marking impossible offset

template <typename T> struct cast_target;
template <typename T> struct cast_target<T*> { typedef T type; };
template <typename T> struct cast_target<const T*> { typedef T type; };

template <typename T>
inline vtbl2offset& offset_map()
{
    static vtbl2offset ofsmap;
    return ofsmap;
}

template <typename T, typename U>
ptrdiff_t dyn_cast_offset(const void* p)
{
    T k = dynamic_cast<T>(static_cast<const U*>(p));
    return k ? reinterpret_cast<const char*>(k)-reinterpret_cast<const char*>(p) : no_cast_exists;
}

template <typename T, typename U>
inline ptrdiff_t get_offset(const U* p)
{
    const intptr_t vtbl = *reinterpret_cast<const intptr_t*>(p);
    vtbl2offset& ofsmap = offset_map<T>();
    const vtbl2offset::iterator q = ofsmap.find(vtbl);

    if (q != ofsmap.end())
        return q->second;
    else
    {
        T k = dynamic_cast<T>(p);
        const ptrdiff_t offset = k ? reinterpret_cast<const char*>(k)-reinterpret_cast<const char*>(p) : no_cast_exists;
        ofsmap.insert(vtbl2offset::value_type(vtbl,offset));
        return offset;
    }
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

template <typename T, typename U>
inline T memoized_cast(const U* p)
{
    static cache_entry cache[1<<cache_bits] = {};

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
            ce.offset = offset = get_offset<T>(p);
        }

        return 
            offset == no_cast_exists 
                ? 0 
                : reinterpret_cast<T>(reinterpret_cast<const char*>(p)+offset);
    }

    return 0;
}

template <typename T, typename U>
inline T memoized_cast(U* u)
{
    return const_cast<T>(memoized_cast<typename cast_target<T>::type const*>(static_cast<typename cast_target<U*>::type const*>(u)));
}
