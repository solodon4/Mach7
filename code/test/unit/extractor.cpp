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
/// \see https://parasol.tamu.edu/mach7/
/// \see https://github.com/solodon4/Mach7
/// \see https://github.com/solodon4/SELL
///

#include <mach7/match.hpp>                 // Support for Match statement
#include <mach7/patterns/bindings.hpp>     // Mach7 support for bindings on arbitrary UDT
#include <mach7/patterns/combinators.hpp>  // Support for pattern combinators &&, || and !
#include <mach7/patterns/constructor.hpp>  // Support for constructor patterns
#include <mach7/patterns/guard.hpp>        // Support for guard patterns
#include <mach7/patterns/n+k.hpp>          // Generalized n+k patterns
#include <mach7/patterns/primitive.hpp>    // Wildcard, variable and value patterns
#include <mach7/adapters/std/complex.hpp>  // Mach7 bindings for std::complex<T>


#include <iostream>

int main()
{
    using namespace mch;

    std::complex<double> c(3.14, 2.15);
    double r;
    double i;

    if (C<std::complex<double>>(r,i)(c))
        std::cout << '(' << r << ',' << i << ')' << std::endl;

    if (C<std::complex<double>, plar>(r,i)(c))
        std::cout << "The polar form of " << c << " is " << r << "*e^i*" << i << "rad " << std::polar(r,i) << std::endl;

    if (C<cartesian>(r,i)(c))
        std::cout << '(' << r << ',' << i << ')' << std::endl;

    if (C<polar>(r,i)(c))
        std::cout << "The polar form of " << c << " is " << r << "*e^i*" << i << "rad " << std::polar(r,i) << std::endl;

    std::cout << '(' << std::real(c) << ',' << std::imag(c) << ')' << std::endl;

    typedef std::complex<double> Cmplx;

    const Cmplx values[] = {Cmplx(0,0), Cmplx(2,0), Cmplx(0,2), Cmplx(3.14, 2.15)};

    for (size_t j = 0; j < XTL_ARR_SIZE(values); ++j)
    {
        Match(values[j])
        {
            Qua(cartesian, 0, 0) std::cout << "Null"             << std::endl; break;
            Qua(cartesian, r, 0) std::cout << "Real number"      << std::endl; break;
            Qua(cartesian, 0, i) std::cout << "Imaginary number" << std::endl; break;
            Qua(cartesian, r, i) std::cout << "Complex number"   << std::endl; break;
        }
        EndMatch
    }

    var<double> R, F;
    const double PI = 3.1415926;

    for (size_t j = 0; j < XTL_ARR_SIZE(values); ++j)
    {
        Match(values[j])
        {
            Qua(polar, 0, _)                            std::cout << "Null"             << std::endl; break;
            Qua(polar, R, F |= F == -PI/2 || F == PI/2) std::cout << "Real number"      << std::endl; break;
            Qua(polar, R, F |= F == 0     || F == PI  ) std::cout << "Imaginary number" << std::endl; break;
            Qua(polar, R, F)                            std::cout << "Complex number"   << std::endl; break;
        }
        EndMatch
    }
}
