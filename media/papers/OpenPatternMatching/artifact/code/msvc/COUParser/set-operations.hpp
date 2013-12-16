#include <algorithm>
#include <iterator>
#include <set>

#pragma once

//------------------------------------------------------------------------------

template<class K, class P, class A>
std::set<K,P,A>	operator&(const std::set<K,P,A>& a, const std::set<K,P,A>& b)
{
    std::set<K,P,A> r;
    std::set_intersection(a.begin(), a.end(), b.begin(), b.end(), std::inserter(r,r.begin()));
    return r;
}

//------------------------------------------------------------------------------

template<class K, class P, class A>
std::set<K,P,A>	operator|(const std::set<K,P,A>& a, const std::set<K,P,A>& b)
{
    std::set<K,P,A> r;
    std::set_union(a.begin(), a.end(), b.begin(), b.end(), std::inserter(r,r.begin()));
    return r;
}

//------------------------------------------------------------------------------

template<class K, class P, class A>
std::set<K,P,A>	operator^(const std::set<K,P,A>& a, const std::set<K,P,A>& b)
{
    std::set<K,P,A> r;
    std::set_difference(a.begin(), a.end(), b.begin(), b.end(), std::inserter(r,r.begin()));
    return r;
}

//------------------------------------------------------------------------------
