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

#include <iostream>
#include <iomanip>
#include <mach7/ptrtools.hpp>              // Mach7 pointer tools


template <typename T, size_t N>
inline T my_interleave(const T (&vtbl)[N])
{
    T result = 0;
    T bit    = 1;

    for (size_t i = 0, j = 0, m = sizeof(T)*8/N; i < m; ++i, bit <<= 1)
        for (size_t n = 0; n < N; ++n)
            result |= ((vtbl[n] & bit) >> i) << j++;

    return result;
}

void test2()
{
    using namespace mch;

    for (intptr_t x = 0; x <= 0xFFFF; ++x)
    {
        if ((x & 0xFF) == 0) std::cout << '.';

        for (intptr_t y = 0; y <= 0xFFFF; ++y)
        {
            intptr_t v[2] = {x,y};
            auto n = interleave(x,y);
            auto m = my_interleave(v);
            if (n != m) std::cout << '(' << x << ',' << y << ") " << n << '|' << m << std::endl;
        }
    }
}

void test3()
{
    using namespace mch;

    for (intptr_t x = 0; x <= 0x03FF; ++x)
    {
        if ((x & 0x1F) == 0) std::cout << '.';

        for (intptr_t y = 0; y <= 0x03FF; ++y)
        {
            for (intptr_t z = 0; z <= 0x03FF; ++z)
            {
                intptr_t v[3] = {x,y,z};
                auto n = interleave(x,y,z);
                auto m = my_interleave(v);
                if (n != m) std::cout << '(' << x << ',' << y << ',' << z << ") " << n << '|' << m << std::endl;
            }
        }
    }
}

void test4()
{
    using namespace mch;

    for (intptr_t x = 0; x <= 0xFF; ++x)
    {
        if ((x & 0x1F) == 0) std::cout << '.';

        for (intptr_t y = 0; y <= 0xFF; ++y)
        {
            for (intptr_t z = 0; z <= 0xFF; ++z)
            {
                for (intptr_t w = 0; w <= 0xFF; ++w)
                {
                    intptr_t v[4] = {x,y,z,w};
                    auto n = interleave(x,y,z,w);
                    auto m = my_interleave(v);
                    if (n != m) std::cout << '(' << x << ',' << y << ',' << z << ',' << w << ") " << n << '|' << m << std::endl;
                }
            }
        }
    }
}

void MortonTable4()
{
    for (uint32_t i = 0; i < 256; ++i)
    {
        size_t x = i;
        x &= 0xFF;                        // x = 00000000 00000000 00000000 ABCDEFGH
        x = (x | (x << 12)) & 0x000F000F; // x = 00000000 0000ABCD 00000000 0000EFGH
        x = (x | (x <<  6)) & 0x03030303; // x = 000000AB 000000CD 000000EF 000000GH 
        x = (x | (x <<  3)) & 0x11111111; // x = 000A000B 000C000D 000E000F 000G000H
        std::cout << "0x" << std::setfill('0') << std::setw(8) << std::hex << x << " // " << std::dec << i << std::endl;
    }
}

int main()
{
    MortonTable4();
    //test2();
    //test3();
    //test4();
}
