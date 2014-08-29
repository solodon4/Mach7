//
//  Mach7: Pattern Matching Library for C++
//
//  Copyright 2011-2013, Texas A&M University.
//  Copyright 2014 Yuriy Solodkyy.
//  All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//
//      * Redistributions of source code must retain the above copyright
//        notice, this list of conditions and the following disclaimer.
//
//      * Redistributions in binary form must reproduce the above copyright
//        notice, this list of conditions and the following disclaimer in the
//        documentation and/or other materials provided with the distribution.
//
//      * Neither the names of Mach7 project nor the names of its contributors
//        may be used to endorse or promote products derived from this software
//        without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
//  IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY
//  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
//  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

///
/// \file
///
/// This file is a part of Mach7 library test suite.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// \see https://parasol.tamu.edu/xtl/
/// \see https://parasol.tamu.edu/mach7/
/// \see https://github.com/solodon4/Mach7
/// \see https://github.com/solodon4/SELL
///

#include <iostream>
#include "boost/variant.hpp"
#include "xtl.hpp"

namespace xtl
{
    template <class S, class T, class... Ts>
    struct is_subtype<S,boost::variant<T,Ts...>>
    {
        static const bool value = is_subtype<S,T>::value
                               || is_subtype<S,boost::variant<Ts...>>::value;
    };

    template <class S>
    struct is_subtype<S,boost::variant<>> : std::false_type {};
}

static_assert(xtl::is_subtype<int,int>::value,"No reflexivity");

class A { public: int a; virtual ~A() {}; virtual void foo() { std::cout << "A" << std::endl; } };
class B : public A { public: int b; virtual void foo() { std::cout << "B" << std::endl; } };

int main()
{
    A a;
    B b;
    a = xtl::subtype_cast<A>(b);
    //b = xtl::subtype_cast<B>(a); // error

    A* pa = xtl::subtype_cast<A*>(&b);
    //B* pb = xtl::subtype_cast<B*>(&a); // error
    B* pb = xtl::subtype_dynamic_cast<B*>(pa);
    pa->foo();
    if (pb) pb->foo();

    static_assert(xtl::is_subtype<int,boost::variant<double,float,int,unsigned int*>>::value, "Not a subtype");

    static_assert(xtl::is_subtype<B,A>::value,"Subclassing");
    return xtl::is_subtype<B,A>::value;
}
