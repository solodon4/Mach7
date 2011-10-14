#include <iostream>
#include <unordered_map>

typedef std::unordered_map<intptr_t, ptrdiff_t> vtbl2offset;

template <typename T>
vtbl2offset& offset_map()
{
    static vtbl2offset ofsmap;
    return ofsmap;
}

template <typename T, typename U>
T memoized_cast(const U* p)
{
    if (p)
    {
        intptr_t vtbl = *static_cast<const int*>(static_cast<const void*>(p));
        vtbl2offset& ofsmap = offset_map<T>();
        vtbl2offset::iterator q = ofsmap.find(vtbl);

        if (q == ofsmap.end())
        {
            std::pair<vtbl2offset::iterator, bool> r = ofsmap.insert(vtbl2offset::value_type(vtbl,0));
        }
    }    
    return dynamic_cast<T>(p);
}

template <typename T, typename U>
T memoized_cast(U* u)
{
    return dynamic_cast<T>(u);
}
