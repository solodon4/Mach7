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
inline T memoized_cast(const U* p)
{
    if (p)
    {
        intptr_t vtbl = *reinterpret_cast<const intptr_t*>(p);
        vtbl2offset& ofsmap = offset_map<T>();
        vtbl2offset::iterator q = ofsmap.find(vtbl);

        if (q == ofsmap.end())
        {
            T k = dynamic_cast<T>(p);
            ptrdiff_t offset = k ? reinterpret_cast<const char*>(k)-reinterpret_cast<const char*>(p) : no_cast_exists;
            ofsmap.insert(vtbl2offset::value_type(vtbl,offset));
            return k;
        }
        else
        {
            T k = q->second == no_cast_exists ? 0 : reinterpret_cast<T>(reinterpret_cast<const char*>(p)+q->second);
            //assert(k == dynamic_cast<T>(p));
            return k;
        }
    }

    return 0;
}

template <typename T, typename U>
inline T memoized_cast(U* u)
{
    return const_cast<T>(memoized_cast<typename cast_target<T>::type const*>(static_cast<typename cast_target<U*>::type const*>(u)));
}
