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

#include "patterns/all.hpp"
#include <iostream>
#include <typeinfo>

int main()
{
    using namespace mch;

    int n;
    auto a1 = filter(n);    std::cout << typeid(decltype(a1)).name() << std::endl;
    auto a2 = filter(42);   std::cout << typeid(decltype(a2)).name() << std::endl;
    var<double> v;
    auto a3 = filter(v);    std::cout << typeid(decltype(a3)).name() << std::endl;
    auto a4 = filter(_);    std::cout << typeid(decltype(a4)).name() << std::endl;
    auto a5 = filter(v && n);    std::cout << typeid(decltype(a5)).name() << std::endl;
    auto a6 = v || n;
    auto a7 = filter(a6);   std::cout << typeid(decltype(a7)).name() << std::endl;
}