//
//  Mach7: Pattern Matching Library for C++
//
//  Copyright 2011-2013, Texas A&M University.
//  Copyright 2014-2015, Yuriy Solodkyy.
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
/// \see https://parasol.tamu.edu/mach7/
/// \see https://github.com/solodon4/Mach7
/// \see https://github.com/solodon4/SELL
///

#include <iostream>
#include <mach7/type_switchN-patterns.hpp> // Support for N-ary Match statement on patterns
#include <mach7/patterns/address.hpp>      // Address and dereference combinators
#include <mach7/patterns/bindings.hpp>     // Mach7 support for bindings on arbitrary UDT
#include <mach7/patterns/constructor.hpp>  // Support for constructor patterns
#include <mach7/patterns/primitive.hpp>    // Wildcard, variable and value patterns
#include <xtl/adapters/std/memory.hpp>     // XTL subtyping adapters for standard smart pointers

using namespace mch;

//------------------------------------------------------------------------------

struct Nat
{ 
    virtual ~Nat() {}
};

struct O : Nat
{
};

struct S : Nat
{
    S(const Nat* n) : e(n) {}
   ~S() { if (e) delete e; }
    const Nat* e;
};

//------------------------------------------------------------------------------

namespace mch ///< Mach7 library namespace
{
template <> struct bindings<O> { };
template <> struct bindings<S> { Members(S::e); };
} // of namespace mch

//------------------------------------------------------------------------------

int evl(const Nat& n)
{
    var<const Nat&> e;

    Match(n)
    {
        Case(C<O>()  ) return 0;
        Case(C<S>(&e)) return evl(e) + 1;
    }
    EndMatch

    XTL_UNREACHABLE; // To avoid warning that control may reach end of a non-void function
}

//------------------------------------------------------------------------------

// Add two inductively defined Nat
const Nat* plus(const Nat* n, const Nat* m)
{
	var<const Nat*> a;
	var<const Nat*> b;

    Match(n, m)
    {

      Case(_, C<O>())        return n; // m + 0 = m
      Case(C<O>(), _)        return m; // 0 + m = m
      Case(C<S>(a), C<S>(b)) return new S(new S(plus(a, b))); // S n + S m = S (S (n + m))
      Otherwise()            return nullptr;
    }
    EndMatch

    XTL_UNREACHABLE; // To avoid warning that control may reach end of a non-void function
}

//------------------------------------------------------------------------------

int main()
{
    Nat* a = new S(new S(new O));
    Nat* b = new S(new O);

    std::cout << evl(*a) << std::endl;
    std::cout << evl(*b) << std::endl;

    const Nat* c = plus(a,b);

    std::cout << evl(*c) << std::endl;
}

//------------------------------------------------------------------------------
