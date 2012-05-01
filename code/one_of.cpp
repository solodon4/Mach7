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

#include "match.hpp"                // Support for Match statement
#include "patterns/any.hpp"         // Support for one-of patterns
#include "patterns/primitive.hpp"   // Support for primitive patterns
#include <iostream>

int main()
{
    const int values[] = {0,1,2,3,4,5,6,7,8,9};

    for (size_t i = 0, n = sizeof(values)/sizeof(values[0]); i < n; ++i)
    {
#if 1
        variable<int> area_code;
        std::cout << values[i] << " is ";
        Match(values[i])
        {
            Pattern(any({0,2,4,6,8})) std::cout << "Even"; break;
            Pattern(any({1,3,5,7,9})) std::cout << "Odd";  break;
            Pattern()                 std::cout << "UNRECOGNIZED"; break;
        }
        EndMatch
        std::cout << std::endl;
#else
#endif
    }
}
