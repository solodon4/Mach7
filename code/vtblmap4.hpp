///
/// \file vtblmap4.hpp
///
/// This file defines class vtblmap<N,T> used for fast mapping of N vtbl
/// pointers to type T.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of Mach7 library (http://parasol.tamu.edu/mach7/).
/// Copyright (C) 2011-2012 Texas A&M University.
/// All rights reserved.
///

#pragma once

#include "vtblmap.hpp"

template <size_t N, typename T>
class vtblmapX
{
public:

    vtblmapX() : idxmap(), stride(), bounds() 
    {
        size_t s = 1;

        for (size_t i = 0; i < N; ++i)
        {
            bounds[i] = 1;
            stride[N-i] = s;
            s *= 4;
        }

        stride[0] = s;
        data.resize(s);
    }

    T& get(const void* p, ...)
    {
        size_t e = 0;
        vtblmap<type_switch_info>& vtbl2idx = idxmap[0];
        size_t j = vtbl2idx.get(p).target; // We use target to store index of vtbl in dimension i
        e += j*stride[0+1];

        va_list args;
        va_start(args, p);

        for (size_t i = 1; i < N; i++)
        {
            void* p = va_arg(args, void*);
            vtblmap<type_switch_info>& vtbl2idx = idxmap[i];
            size_t j = vtbl2idx.get(p).target; // We use target to store index of vtbl in dimension i
            e += j*stride[i+1];
        }

        va_end(args);

        return 
    }

    type_switch_info& get_ref(size_t i, const void* p)
    {
        XTL_ASSERT(i < N);

        vtblmap<type_switch_info>& vtbl2idx = idxmap[i];
        size_t s1 = vtbl2idx.table.size();
        type_switch_info& t = vtbl2idx.get(p);
        size_t s2 = vtbl2idx.table.size();

        if (s1 != s2)
        {
            size_t m = max_bound(i); 

            if (s2 >= m)
                reserve(i,m*2);
        }

        return t;
    }

    size_t idx(size_t i, const void* p)
    {
        type_switch_info& t = get_ref(i, p);
        return t.target;
    }

    ptrdiff_t ofs(size_t i, const void* p)
    {
        type_switch_info& t = get_ref(i, p);
        return t.offset;
    }

    size_t max_bound(size_t i) const 
    { 
        XTL_ASSERT(i < N);
        return stride[i]/stride[i+1];
    }

    void reserve(size_t i, size_t n)
    {
        XTL_ASSERT(i < N);
        XTL_ASSERT(max_bound(i) < n);

        size_t new_stride[N+1];
        size_t s = 1;

        for (size_t j = 0; j < i; ++j)
            new_stride[N-j] = stride[N-j];

        new_stride[N-i] = n*stride[N-i+1];

        for (size_t j = i+1; j < N; ++j)
            new_stride[N-j] = bound(N-j)*new_stride[N-j+1];

        size_t old_bounds[N];
        size_t new_bounds[N];

        for (size_t j = 0; j < N; ++j)
        {
            old_bounds[j] = max_bound(j);
            new_bounds[j] = new_stride[j]/new_stride[j+1];;
        }


        std::vector<T> d(new_stride[0]);

        size_t idx[N] = {0};
        size_t k = N-1;

        for (size_t j = 0, m = d.size(); j < m; ++j)
        {
            while (idx[k] == new_bounds[k]-1)
                idx[k--] = 0;

            ++idx[k];
            k = N-1;
        }
    }

    vtblmap<type_switch_info> idxmap[N];
    size_t                    stride[N+1];
    //size_t                    bounds[N];
    std::vector<T>            data;
};

