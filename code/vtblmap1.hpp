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
/// This file defines class vtblmap<T> used for fast mapping of vtbl pointers
/// to type T.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// \see https://parasol.tamu.edu/mach7/
/// \see https://github.com/solodon4/Mach7
/// \see https://github.com/solodon4/SELL
///

#pragma once

#include <functional>
#include <unordered_map>
#include <limits>
#include "config.hpp"    // Various compiler/platform dependent macros

//------------------------------------------------------------------------------

#ifdef TRACE_PERFORMANCE
#include <bitset> // For print out purposes only
int cache_hits   = 0;
int cache_misses = 0;
intptr_t common  =~0;
intptr_t differ  = 0;
#endif

//------------------------------------------------------------------------------

template <typename T, size_t cache_bits = 7>
class vtblmap
{
private:

#ifdef _MSC_VER
   	/// Hash functor for vtbl
    struct vtbl_hasher
    {
	    typedef intptr_t argument_type;
	    typedef size_t   result_type;

        /// hash key to size_t value by pseudorandomizing transform
        size_t operator()(const intptr_t key) const { return key; }
    };

    /// MSVC uses by default a complicated hash function on all integral types,
    /// but for our application to vtables identity works best
    typedef std::unordered_map<intptr_t, T, vtbl_hasher> vtbl_to_t_map;
#else
    typedef std::unordered_map<intptr_t, T>              vtbl_to_t_map;
#endif

    typedef typename vtbl_to_t_map::iterator    iterator;
    typedef typename vtbl_to_t_map::value_type  value_type;

public:
    
    typedef typename vtbl_to_t_map::mapped_type mapped_type;

    /// A few useful constants
    enum
    {
        cache_mask = (1<<cache_bits)-1,
        /// Irrelevant lowest bits in vtbl pointers that are always the same for given 
        /// compiler/platform configuration.
        irrelevant_bits = VTBL_IRRELEVANT_BITS
    };

    /// Structure describing entry in the cache
    struct cache_entry
    {
        intptr_t  vtbl; ///< vtbl for which value has been computed
        T*        ptr;  ///< pointer to value associated with given vtbl
    };

    /// This is the main function to get hold of a value associated with the vtbl of a given pointer
    inline T& get(const void* p, const T& dflt = T()) throw()
    {
        const intptr_t vtbl = *reinterpret_cast<const intptr_t*>(p);
        //XTL_ASSERT(vtbl);                                // Since this represents VTBL pointer it cannot be null
        //XTL_ASSERT(!(vtbl & (1<<irrelevant_bits)-1));    // Assertion here means your irrelevant_bits is not correct as there are 1 bits in what we discard
        const intptr_t key  = vtbl>>irrelevant_bits; // We do this as we rely that hash function is identity
        cache_entry& ce = cache[key & cache_mask];

        if (ce.vtbl != vtbl)
        {
            const iterator q = table.find(key);
            ce.ptr  = q != table.end() ? &q->second : &table.insert(value_type(key,dflt)).first->second;
            ce.vtbl = vtbl;
#ifdef TRACE_PERFORMANCE
            //std::cout << "Differ: " << std::bitset<8*sizeof(intptr_t)>(differ) << std::endl;
            //std::cout << "Common: " << std::bitset<8*sizeof(intptr_t)>(common) << std::endl;
            //std::cout << "Vtbl  : " << std::bitset<8*sizeof(intptr_t)>((unsigned long long)vtbl) << std::endl;
            //std::cout << " Cm^Vt: " << std::bitset<8*sizeof(intptr_t)>(common ^ vtbl) << std::endl;
            //std::cout << "~Cm^Vt: " << std::bitset<8*sizeof(intptr_t)>(~(common ^ vtbl)) << std::endl;
            ++cache_misses;
            if (common == ~0)
            {
                common = vtbl;
                differ = 0;
            }
            else
            {
                intptr_t cmvt = common ^ vtbl;
                common &= ~cmvt;
                differ |=  cmvt;
            }
        }
        else
            ++cache_hits;
#else
        }
#endif

        return *ce.ptr;
    }

    vtbl_to_t_map table;
    cache_entry   cache[1<<cache_bits];
};

//------------------------------------------------------------------------------

template <int N = 5>
class vtbl2lines : public vtblmap<int,N>
{
private:
    typedef vtblmap<int,N> base_type;
public:
    inline void update(int ln, const void* t) throw()
    {
        typename base_type::mapped_type& line = this->get(t);

        if (line == 0)
            line = ln;
    }
};

//------------------------------------------------------------------------------

class pearson_hash
{
protected:

    static const unsigned char H[256];

};

//------------------------------------------------------------------------------

const unsigned char pearson_hash::H[256] = {
      1,  87,  49,  12, 176, 178, 102, 166, 121, 193,   6,  84, 249, 230,  44, 163,
     14, 197, 213, 181, 161,  85, 218,  80,  64, 239,  24, 226, 236, 142,  38, 200,
    110, 177, 104, 103, 141, 253, 255,  50,  77, 101,  81,  18,  45,  96,  31, 222,
     25, 107, 190,  70,  86, 237, 240,  34,  72, 242,  20, 214, 244, 227, 149, 235,
     97, 234,  57,  22,  60, 250,  82, 175, 208,   5, 127, 199, 111,  62, 135, 248,
    174, 169, 211,  58,  66, 154, 106, 195, 245, 171,  17, 187, 182, 179,   0, 243,
    132,  56, 148,  75, 128, 133, 158, 100, 130, 126,  91,  13, 153, 246, 216, 219,
    119,  68, 223,  78,  83,  88, 201,  99, 122,  11,  92,  32, 136, 114,  52, 10 ,
    138,  30,  48, 183, 156,  35,  61,  26, 143,  74, 251,  94, 129, 162,  63, 152,
    170,   7, 115, 167, 241, 206,   3, 150,  55,  59, 151, 220,  90,  53,  23, 131,
    125, 173,  15, 238,  79,  95,  89,  16, 105, 137, 225, 224, 217, 160,  37, 123,
    118,  73,   2, 157,  46, 116,   9, 145, 134, 228, 207, 212, 202, 215,  69, 229,
     27, 188,  67, 124, 168, 252,  42,   4,  29, 108,  21, 247,  19, 205,  39, 203,
    233,  40, 186, 147, 198, 192, 155,  33, 164, 191,  98, 204, 165, 180, 117, 76 ,
    140,  36, 210, 172,  41,  54, 159,   8, 185, 232, 113, 196, 231,  47, 146, 120,
     51,  65,  28, 144, 254, 221,  93, 189, 194, 139, 112,  43,  71, 109, 184, 209
};

//------------------------------------------------------------------------------

class vtbl2lines2 : pearson_hash
{
public:

    vtbl2lines2()
    {
        std::fill(cache,cache+(1<<cache_bits),std::numeric_limits<int>::max()^1);
    }

    /// A few useful constants
    enum
    {
        cache_bits = 1,
        cache_mask = (1<<cache_bits)-1,

        // FIX: Make this less empirical
    #ifdef _MSC_VER
        irrelevant_bits = 3  // vtbl in MSVC seem to be alligned by 4 bytes
    #else
        irrelevant_bits = 4  // vtbl in G++ seem to be alligned by 8 bytes
    #endif
    };

    static inline unsigned char hash(intptr_t key)
    {
        unsigned char h = 0;
        h = H[h ^ (unsigned char)(key>> 0)];
        h = H[h ^ (unsigned char)(key>> 8)];
        h = H[h ^ (unsigned char)(key>>16)];
        h = H[h ^ (unsigned char)(key>>24)];
        return h;
    }

    /// This is the main function to get hold of a value associated with the vtbl of a given pointer
    inline int& get(const void* p) throw()
    {
        const intptr_t vtbl = *reinterpret_cast<const intptr_t*>(p);
        const unsigned char key = hash(vtbl>>irrelevant_bits);
        return cache[key & cache_mask];
    }

    inline void update(int ln, const void* t, const void* success) throw()
    {
        int& line = get(t);

        if (success)
            if (line%2 == 1 || 2*ln < line)
                line = 2*ln;
            else
                ;
        else
            if (line%2 == 1)
                line = 2*ln+1;
            else
                ;
    }

    int cache[1<<cache_bits];
};
