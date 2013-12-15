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
    const char* strings[] = 
    {
        "979-739-3587",
        "571-739-3587",
        "XXX-739-3587",
        "971-739-3587",
        "1977",
        "var1"
    };

    for (size_t i = 0, n = sizeof(strings)/sizeof(strings[0]); i < n; ++i)
    {
        mch::var<int> area_code;

        std::cout << strings[i] << " is a ";

        Match(std::string(strings[i]))
        {
            With(mch::rex("([0-9]+)-([0-9]+)-([0-9]+)", 979))       std::cout << "Local Phone"; break;
            With(mch::rex("([0-9]+)-([0-9]+)-([0-9]+)", area_code |= area_code >= 970 && area_code <= 980)) std::cout << "Texas Phone "; break;
            With(mch::rex("([0-9]+)-([0-9]+)-([0-9]+)", area_code)) std::cout << "Phone with area code "     << area_code; break;
            With(mch::rex("([A-Z]+)-([0-9]+)-([0-9]+)", area_code)) std::cout << "Something with area code " << area_code; break;
            With(mch::rex("[0-9]+"))                                std::cout << "Number"; break;
            With(mch::rex("[A-Za-z_][A-Za-z_0-9]*"))                std::cout << "Identifier"; break;
            Otherwise()                                             std::cout << "UNRECOGNIZED"; break;
        }
        EndMatch

        std::cout << std::endl;
    }
}
