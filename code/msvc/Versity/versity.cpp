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
