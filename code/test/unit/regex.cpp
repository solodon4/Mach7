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
#include <mach7/patterns/combinators.hpp>  // Support for pattern combinators &&, || and !
#include <mach7/patterns/guard.hpp>        // Support for guard patterns
#include <mach7/patterns/n+k.hpp>          // Generalized n+k patterns
#include <mach7/patterns/primitive.hpp>    // Wildcard, variable and value patterns
#include <mach7/patterns/regex.hpp>        // Regular expression patterns

#include <iostream>


int main()
{
    using namespace mch;

    const char* strings[] = 
    {
        "1977-04-01",
        "1977-20-01",
        "979-739-3587",
        "571-739-3587",
        "XXX-739-3587",
        "971-739-3587",
        "1977",
        "var1"
    };

    for (size_t i = 0, n = sizeof(strings)/sizeof(strings[0]); i < n; ++i)
    {
        var<int> area_code;
        var<int> y,m,d;                    // Year, month, day
        auto year  = y |= y > 0;           // Year  pattern
        auto month = m |= m > 0 && m < 13; // Month pattern
        auto day   = d |= d > 0 && d < 31; // Day   pattern

        std::cout << strings[i] << " is a ";

        Match(std::string(strings[i]))
        {
            With(rex("([0-9]{4})[/-]([0-9]{2})[/-]([0-9]{2})", year, month, day)) std::cout << "Date YYYY-MM-DD: " << y << '-' << m << '-' << d; break;
            With(rex("([0-9]{4})[/-]([0-9]{2})[/-]([0-9]{2})", year, day, month)) std::cout << "Date YYYY-DD-MM: " << y << '-' << m << '-' << d; break;
            With(rex("([0-9]+)-([0-9]+)-([0-9]+)", 979))       std::cout << "Local Phone"; break;
            With(rex("([0-9]+)-([0-9]+)-([0-9]+)", area_code |= area_code >= 970 && area_code <= 980)) std::cout << "Texas Phone "; break;
            With(rex("([0-9]+)-([0-9]+)-([0-9]+)", area_code)) std::cout << "Phone with area code "     << area_code; break;
            With(rex("([A-Z]+)-([0-9]+)-([0-9]+)", area_code)) std::cout << "Something with area code " << area_code; break;
            With(rex("[0-9]{6}"))                              std::cout << "6 digits"; break;
            With(rex("[0-9]{5}"))                              std::cout << "5 digits"; break;
            With(rex("[0-9]{4}"))                              std::cout << "4 digits"; break;
            With(rex("[0-9]{3}"))                              std::cout << "3 digits"; break;
            With(rex("[0-9]+"))                                std::cout << "Number"; break;
            With(rex("[A-Za-z_][A-Za-z_0-9]*"))                std::cout << "Identifier"; break;
            Otherwise()                                             std::cout << "UNRECOGNIZED"; break;
        }
        EndMatch

        std::cout << std::endl;
    }
}
