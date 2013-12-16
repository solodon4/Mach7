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

#include "match.hpp"                // Support for Match statement
#include "patterns/combinators.hpp" // Support for pattern combinators
#include "patterns/guard.hpp"       // Support for guard patterns
#include "patterns/n+k.hpp"         // Support for n+k patterns
#include "patterns/regex.hpp"       // Support for regular expression patterns

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
