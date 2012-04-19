///
/// \file
///
/// This file is a part of pattern matching testing suite.
///
/// \autor Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of the XTL framework (http://parasol.tamu.edu/xtl/).
/// Copyright (C) 2005-2012 Texas A&M University.
/// All rights reserved.
///

#include <iostream>
#include "patterns/regex.hpp"
#include "match.hpp"

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
#if 1
        variable<int> area_code;

        std::cout << strings[i] << " is a ";

        Match(std::string(strings[i]))
        {
            Pattern(rex("([0-9]+)-([0-9]+)-([0-9]+)", 979))       std::cout << "Local Phone"; break;
            Pattern(rex("([0-9]+)-([0-9]+)-([0-9]+)", area_code |= area_code >= 970 && area_code <= 980)) std::cout << "Texas Phone "; break;
            Pattern(rex("([0-9]+)-([0-9]+)-([0-9]+)", area_code)) std::cout << "Phone with area code "     << area_code; break;
            Pattern(rex("([A-Z]+)-([0-9]+)-([0-9]+)", area_code)) std::cout << "Something with area code " << area_code; break;
            Pattern(rex("[0-9]+"))                                std::cout << "Number"; break;
            Pattern(rex("[A-Za-z_][A-Za-z_0-9]*"))                std::cout << "Identifier"; break;
            Pattern()                                             std::cout << "UNRECOGNIZED"; break;
        }
        EndMatch
        
        std::cout << std::endl;
#else
#endif
    }
}
