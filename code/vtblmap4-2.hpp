///
/// \file vtblmap4-2.hpp
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

template <typename T>
class vtblmap2
{
public:

    vtblmap2() : idxmap()
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

    T& get(const void* p1, const void* p2)
    {
        size_t j1 = idxmap[0].get(p1).target;
        size_t j2 = idxmap[1].get(p2).target;

        return 
    }

    vtblmap<type_switch_info> idxmap[2];
    std::vector<T>            data;
};

