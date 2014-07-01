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

#include "versity-types.hpp"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>

//------------------------------------------------------------------------------

extern int yyparse(void);
extern int yy_flex_debug;
int yydebug;

//------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    yydebug = 0;       // disable Yacc debugging mode
    yy_flex_debug = 0; // disable Flex debugging mode
    std::string base_name = "undefined";

    try
    {
        if (argc < 2)
        {
            std::cerr << "Usage: " << argv[0] << " filename.i0" << std::endl;
            return 1;
        }

        if (!freopen(argv[1],"r",stdin)) //redirect standard input
        {
            std::cerr << "Error: Can't open file " << argv[1] << std::endl;
            return 2;
        }

        base_name = argv[1];
        base_name.erase(base_name.find_last_of('.'));

        int result = yyparse();
        std::clog.flush();

        if (result == 0)
        {
            extern instruction_stream_type* instruction_stream;

            if (instruction_stream)
            {
                std::cout << '[';

                for (instruction_stream_type::const_iterator p = instruction_stream->begin(); p != instruction_stream->end(); ++p)
                    std::cout << **p << ',' << std::endl;

                std::cout << ']';
            }
        }

        return result;
    }
    catch (...)
    {
        std::cerr << "ERROR: Unhandled exception caught while parsing" << std::endl;
    	return 4;
    }
}

//------------------------------------------------------------------------------
