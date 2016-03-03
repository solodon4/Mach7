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
#include <mach7/patterns/any.hpp>          // Support for one-of pattern
#include <mach7/patterns/primitive.hpp>    // Wildcard, variable and value patterns

#include <iostream>

int main()
{
    const int values[] = {0,1,2,3,4,5,6,7,8,9};

    for (size_t i = 0, n = sizeof(values)/sizeof(values[0]); i < n; ++i)
    {
        mch::var<int> area_code;
        std::cout << values[i] << " is ";

#if XTL_SUPPORT(initializer_list)        
        Match(values[i])
        {
            With(mch::any({0,2,4,6,8})) std::cout << "Even"; break;
            With(mch::any({1,3,5,7,9})) std::cout << "Odd";  break;
            //With()                 std::cout << "UNRECOGNIZED"; break;
        }
        EndMatch
#else
        static const int evens[] = {0,2,4,6,8};
        static const int  odds[] = {1,3,5,7,9};
        Match(values[i])
        {
            With(mch::any(std::begin(evens),std::end(evens))) std::cout << "even"; break;
            With(mch::any(std::begin( odds),std::end( odds))) std::cout << "odd";  break;
            //With()                 std::cout << "UNRECOGNIZED"; break;
        }
        EndMatch
#endif
        std::cout << std::endl;
    }
}
