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

// --------------------[ Collisions Management ]--------------------
// Definition: Collision is condition in which cache cell for a
//             given vtbl is occupied by a different vtbl.
// Rules:
// - We only try to rearrange cache when actual collision happens
// - There is no point in rearranging if table.size() hasn't changed
// - Counting conflicts without changes in table size may decrease 
//   the counter to overflow in repetitive scenario.
// - There is always M such that growing cache to size 2^M will have
//   zero collisions. Such M is often too large to justify growth.
// - We only try log sizes N and N+1, where N = log2(|table|)
// - We do not know apriory if the set of all vtbls that will come 
//   through this vtblmap can have 0 collisions for log size N, N+1
// - Cache arrangement with no conflicts can be a good indicator that
//   such arrangement will still be possible after adding one more vtbl.
// - Cache arrangement with conflicts will have them with more vtbls 
//   added unless the cache size got increased.
// - In most cases we'd want at least one rearrangement to happen as
//   default value for irrelevant bits is usually irrelevant for a
//   given vtblmap and has to be recomputed.
// - We also have to avoid rearranging after every vtbl added as
//   for some especially large tables conflicts with log sizes N and
//   N+1 are unavoidable.
// - Once we found offset/size combination that renders no conflicts
//   there is no point in computing entropy further.
// - Addition of extra vtbl can make probability of conflict smaller
//   since total number of vtbls increased and we divide by it.
// - Addition of extra vtbl can make probability of conflict larger
//   when that vtbl conflicts with some that was not in conflict before.
//------------------------------------------------------------------------------

#include <algorithm>
#include <cmath>
#include <cstring>
#include <mach7/config.hpp>                // Mach7 configuration
#include <mach7/ptrtools.hpp>              // Mach7 pointer tools

#if XTL_DUMP_PERFORMANCE
// For print out purposes only
#include <array>
#include <bitset>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#endif

namespace mch ///< Mach7 library namespace
{

//------------------------------------------------------------------------------

/// Type capable of representing bit offsets and bit counts in intptr_t
typedef unsigned char  bit_offset_t;
//typedef size_t  bit_offset_t;

/// The smallest integral type capable of representing the amount N of different 
/// vtbl pointers in the program. Roughly N should be equal to some constant c
/// multiplied by the amount of different classes polymorphic classes in the 
/// program. Constant c accounts for potential multiple inheritance.
typedef unsigned short vtbl_count_t;
//typedef size_t vtbl_count_t;

//------------------------------------------------------------------------------

const bit_offset_t min_log_size      = XTL_MIN_LOG_SIZE; ///< Log of the smallest cache size to start from
const bit_offset_t max_log_size      = XTL_MAX_LOG_SIZE; ///< Log of the largest cache size to try
const bit_offset_t max_log_inc       = XTL_MAX_LOG_INC;  ///< Log of the maximum allowed increased from the minimum requred log size (1 means twice from the min required size)
const vtbl_count_t min_expected_size = 1 << min_log_size;
const bit_offset_t irrelevant_bits   = XTL_IRRELEVANT_VTBL_BITS;
const int initial_collisions_before_update = 1;
const int renewed_collisions_before_update = 16;

/// Natural logarithm of 2 needed for conversion into log base 2.
const double ln2 = 0.69314718055994528622676398299518041312694549560546875;

const size_t N = 3; // The number of arguments this implementation handles

//------------------------------------------------------------------------------

/// Class for efficient mapping of vtbl-pointers to a value of type T.
/// This version of the class is for use in the single-threaded environment. 
/// The data structure is implemented in a lock-free manner.
///
/// The map can only grow in size - it does not provide any means to shrink or 
/// reallocate the contained data. The reason is that all the applications that
/// use the vtblmap so far rely on the reference to an element associated with 
/// given vtbl-pointer to not change throughout the lifetime of application. 
template <typename T>
class vtblmap
{
private:

    /// A helper data structure that is swapped during updates to 
    /// cache parameters k and l.
    struct cache_descriptor
    {
        /// Type of the stored values, which is a pair of vtbl-pointer and T value.
        struct stored_type
        {
            stored_type() : vtbl(), value() {}

            intptr_t vtbl[N];  ///< v-table pointers of the value
            T        value;    ///< value associated with the v-table pointers vtbl[]

            /// Helper function to in-place construct stored_type inside uninitialized memory
            void construct()    { new(this) stored_type(); }
            /// Helper function to manually destroy stored_type
            void destroy()      { this->~stored_type(); }
        };

        /// Cache mask to access entries. Always cache_size-1 since cache_size is a power of 2
        /// \note We currently rely in constructors on this member be first in 
        ///       declaration order so that it is initialized first!
        const size_t cache_mask;
        
        /// Optimal shift computed based on the vtbl pointers already in the map.
        /// Most of the time this value would be equal to #irrelevant_bits, but not
        /// necessarily always. In case of collisions, optimal_shift will have
        /// a value of a shift that maximizes entropy of caching vtbl pointers (which 
        /// effectively also minimizes probability of not finding something in cache)
        /*const*/ bit_offset_t optimal_shift[N];

        /// Total number of vtbl-pointers in the cache
        size_t used;

        /// Variable-sized array with actual pointers to stored_type
        stored_type* cache[XTL_VARIABLE_SIZE_ARRAY];

        #if defined(DBG_NEW)
            #undef new
        #endif

        void* operator new(size_t s, size_t log_size)
        {
            // FIX: Ensure proper alignment requirements
            return ::new char[s + ((1<<log_size)-XTL_VARIABLE_SIZE_ARRAY)*sizeof(stored_type*)];
        }

        #if defined(DBG_NEW)
            #define new DBG_NEW
        #endif

        /// We need to declare this placement delete operator since we overload new.
        void operator delete(void* p, size_t) { ::delete(static_cast<char*>(p)); } // We cast to char* to avoid warning on deleting void*, which is undefined

        /// We also provide non-placement delete operator since it doesn't really depend on extra arguments.
        void operator delete(void* p)         { ::delete(static_cast<char*>(p)); } // We cast to char* to avoid warning on deleting void*, which is undefined

        /// Creates new cache_descriptor based on parameters k and l of the hashing function
        cache_descriptor(
            const size_t       log_size,               ///< Parameter k of the cache - the log of the size of the cache
            const bit_offset_t shift = irrelevant_bits ///< Parameter l of the cache - number of irrelevant bits on the right to remove
        ) :
            cache_mask( (1<<log_size) - 1 ),
            //optimal_shift(shift),
            used(0)
        {
            // Initialize all optimal_shift values with the same value
            std::fill(&optimal_shift[0],&optimal_shift[N],shift);

            // Allocate all cache entries in one chunk for better cache performance.
            // Only allocate the difference from need and already present in old ones
            stored_type* cache_entries = new stored_type[1<<log_size];

            // Initialize pointers from cache to newly allocated cache entries
            for (size_t i = 0; i <= cache_mask; ++i)
                cache[i] = &cache_entries[i];
        }

        /// Creates new cache_descriptor based on parameters k and l of the 
        /// hashing function as well as a reference to the cache_descriptor it
        /// is going to replace.
        cache_descriptor(
            const size_t       log_size, ///< Parameter k of the cache - the log of the size of the cache                
            const bit_offset_t shifts[N],///< Parameter l of the cache - number of irrelevant bits on the right to remove
            cache_descriptor&& old       ///< cache_descriptor we will supposedly replace
        ) :
            cache_mask( (1<<log_size) - 1 ),
            //optimal_shift(shifts),
            used(old.used)
        {
            XTL_ASSERT(cache_mask >= old.cache_mask); // Since we are going to inherit all its existing elements

            std::copy(&shifts[0],&shifts[N],&optimal_shift[0]); // Initialize optimal shifts

            size_t i = 0;

            // Initialize first cache pointers to cache entries from old cache
            for (; i <= old.cache_mask; ++i)
            {
                cache[i] = 0;
                std::swap(cache[i],old.cache[i]);
            }

            if (cache_mask - old.cache_mask)
            {
                // Allocate all cache entries in one chunk for better cache performance.
                // Only allocate the difference from need and already present in old ones
                stored_type* cache_entries = new stored_type[cache_mask - old.cache_mask];

                // Initialize remaining pointers from cache to newly allocated cache entries
                for (size_t j = 0; i <= cache_mask; ++i, ++j)
                    cache[i] = &cache_entries[j];
            }
        }

        /// Deallocates memory used for elements.
        ~cache_descriptor()
        {
            // The elements will be pointing into separate arrays, we want to
            // find all the beginnings of arrays to deallocate them
            std::sort(&cache[0], &cache[cache_mask+1]);

            for (size_t i = 0; i <= cache_mask;)
            {
                if (cache[i])
                {
                    size_t j = i+1;
                    for (; j <= cache_mask && intptr_t(cache[j])-intptr_t(cache[j-1]) == sizeof(stored_type); ++j);
                    delete[] cache[i];
                    i = j;
                }
                else
                    ++i;
            }
        }

        cache_descriptor& operator=(const cache_descriptor&); ///< No assignment

        bool is_full() const { return used > cache_mask; } ///< Checks whether cache is full
        size_t  size() const { return cache_mask+1; }      ///< Number of entries in cache

        const stored_type*& loc(intptr_t vtbl0, intptr_t vtbl1, intptr_t vtbl2) const { return cache[interleave(vtbl0 >> optimal_shift[0], vtbl1 >> optimal_shift[1], vtbl2 >> optimal_shift[2]) & cache_mask]; }
              stored_type*& loc(intptr_t vtbl0, intptr_t vtbl1, intptr_t vtbl2)       { return cache[interleave(vtbl0 >> optimal_shift[0], vtbl1 >> optimal_shift[1], vtbl2 >> optimal_shift[2]) & cache_mask]; }

        /// Main function that will be used to get a reference to the stored element. 
        inline stored_type*& get(const intptr_t vtbl0, const intptr_t vtbl1, const intptr_t vtbl2) noexcept
        {
            XTL_ASSERT(vtbl0); // Must be a valid vtbl pointer
            XTL_ASSERT(vtbl1); // Must be a valid vtbl pointer
            XTL_ASSERT(vtbl2); // Must be a valid vtbl pointer

            stored_type*& ce = loc(vtbl0,vtbl1,vtbl2); // Location where it should be

            XTL_ASSERT(ce);   // Since we pre-allocate all entries

            if (XTL_UNLIKELY(ce->vtbl[0] != vtbl0 || 
                             ce->vtbl[1] != vtbl1 || 
                             ce->vtbl[2] != vtbl2))
            {
                // NOTE: We don't check if the entry is occupied as even when 
                //       it is not, the vtbl may be elsewhere in the cache due 
                //       to changes to k and l after update.
                stored_type** cv;

                // See if (vtbl0,vtbl1) is elsewhere in the cache
                for (size_t i = 0; i <= cache_mask; ++i)
                    if (cache[i]->vtbl[0] == vtbl0 && 
                        cache[i]->vtbl[1] == vtbl1 && 
                        cache[i]->vtbl[2] == vtbl2) // if so ...
                    {
                        cv = &cache[i]; // swap it with the right position
                        goto Swap;
                    }

                // (vtbl0,vtbl1) is not in the cache
                if (used <= cache_mask) // there are empty slots in cache
                    for (size_t i = 0; i <= cache_mask; ++i)
                        if (cache[i]->vtbl[0] == 0) // find the first empty slot
                        {
                            XTL_ASSERT(cache[i]->vtbl[1] == 0);
                            cache[i]->vtbl[0] = vtbl0; // assign vtbl to it
                            cache[i]->vtbl[1] = vtbl1; // assign vtbl to it
                            cache[i]->vtbl[2] = vtbl2; // assign vtbl to it
                            ++used;
                            cv = &cache[i]; // swap it with the right position
                            goto Swap;
                        }

                // There are no empty slots, we return the slot in which it
                // is supposed to be, the caller should check vtbl of the 
                // returned entry to ensure it has vtbl he was looking for.
                return ce;
Swap:
                std::swap(ce,*cv);
            }

            return ce;
        }
    };

public:
    
#if XTL_DUMP_PERFORMANCE
    #if defined(DBG_NEW)
        #undef new
    #endif
    vtblmap(const char* fl, size_t ln, const char* fn, const vtbl_count_t expected_size = min_expected_size) : 
        descriptor(new(req_bits(expected_size-1)) cache_descriptor(req_bits(expected_size-1))),
        last_table_size(0),
        collisions_before_update(initial_collisions_before_update),
        file(fl), 
        line(ln),
        func(fn),
        updates(0), 
        clauses(expected_size),
        hits(0),
        misses(0),
        collisions(0)
    {}
    #if defined(DBG_NEW)
        #define new DBG_NEW
    #endif
#endif

    #if defined(DBG_NEW)
        #undef new
    #endif
    vtblmap(const vtbl_count_t expected_size = min_expected_size) :
        descriptor(new(req_bits(expected_size-1)) cache_descriptor(req_bits(expected_size-1))),
        collisions_before_update(initial_collisions_before_update)
        XTL_DUMP_PERFORMANCE_ONLY(,file("unspecified"), line(0), func("unspecified"), updates(0), clauses(expected_size), hits(0), misses(0), collisions(0))
    {}
    #if defined(DBG_NEW)
        #define new DBG_NEW
    #endif

   ~vtblmap()
    {
        XTL_DUMP_PERFORMANCE_ONLY(std::clog << *this << std::endl);
        delete descriptor;
    }

    /// This is the main function to get the value of type T associated with
    /// the vtbl of a given pointer.
    ///
    /// \note The function returns the value "by reference" to indicate that you 
    ///       may take address or change the value of the cell!
    inline T& get(const void* p0, const void* p1, const void* p2) noexcept
    {
        XTL_ASSERT(descriptor); // Allocated in constructor, deallocated in destructor

        const intptr_t vtbl0 = *reinterpret_cast<const intptr_t*>(p0);
        const intptr_t vtbl1 = *reinterpret_cast<const intptr_t*>(p1);
        const intptr_t vtbl2 = *reinterpret_cast<const intptr_t*>(p2);

        typename cache_descriptor::stored_type*& ce = descriptor->loc(vtbl0,vtbl1,vtbl2);

        XTL_ASSERT(vtbl0); // Since this represents VTBL pointer it cannot be null
        XTL_ASSERT(vtbl1); // Since this represents VTBL pointer it cannot be null
        XTL_ASSERT(vtbl2); // Since this represents VTBL pointer it cannot be null
        XTL_ASSERT(ce);    // Since we use stub entry with vtbl==0 to indicate an empty one

        if (XTL_UNLIKELY(ce->vtbl[0] != vtbl0 || 
                         ce->vtbl[1] != vtbl1 || 
                         ce->vtbl[2] != vtbl2))
        {
            XTL_DUMP_PERFORMANCE_ONLY(++misses);
            XTL_DUMP_PERFORMANCE_ONLY(if (ce->vtbl[0]) ++collisions);

            if (descriptor->is_full()                     // No entries left for possibly new vtbl in the cache
                || (ce->vtbl[0]                           // Collision - the entry for vtbl is already occupied
                && --collisions_before_update <= 0        // We had sufficiently many collisions to justify call
                && descriptor->used != last_table_size))  // There was at least one vtbl added since last update
                return update(vtbl0,vtbl1,vtbl2); // try to rearrange cache

            // Try to find entry with our vtbl and swap it with where it is expected to be
            descriptor->get(vtbl0,vtbl1,vtbl2); // This will bring correct pointer into ce
            XTL_ASSERT(ce->vtbl[0] == vtbl0);
            XTL_ASSERT(ce->vtbl[1] == vtbl1);
            XTL_ASSERT(ce->vtbl[2] == vtbl2);
        }
        XTL_DUMP_PERFORMANCE_ONLY(else ++hits);

        return ce->value;
    }

    /// A function that gets called when the cache is either too inefficient or full.
    T& update(intptr_t vtbl0, intptr_t vtbl1, intptr_t vtbl2);

#if XTL_DUMP_PERFORMANCE
    std::ostream& operator>>(std::ostream& os) const;
    friend std::ostream& operator<<(std::ostream& os, const vtblmap& m) { return m >> os; }
#endif

private:

    /// Cached mappings of vtbl to some indecies
    cache_descriptor* descriptor;

    /// Memoized table.size() during last cache rearranging
    size_t last_table_size;

    /// Number of colisions that we will still tolerate before next update
    int collisions_before_update;

#if XTL_DUMP_PERFORMANCE
    const char* file;      ///< File in which this vtblmap_of is instantiated
    size_t      line;      ///< Line in the file where it is instantiated
    const char* func;      ///< Function in which this vtblmap_of is instantiated
    size_t      updates;   ///< Amount of reconfigurations performed at run time
    size_t      clauses;   ///< Size of the table expected from the number of clauses
    size_t      hits;      ///< The amount of cache hits
    size_t      misses;    ///< The amount of cache misses
    size_t      collisions;///< Out of all the misses, how many were actual collisions
#endif

};

//------------------------------------------------------------------------------

template <typename T>
T& vtblmap<T>::update(intptr_t vtbl0, intptr_t vtbl1, intptr_t vtbl2)
{
    XTL_ASSERT(descriptor); // Allocated in constructor, deallocated in destructor
    XTL_ASSERT(last_table_size < descriptor->used || descriptor->is_full()); // We will only call this if size changed

    // FIX: vtbl might already exist in old descriptor and if it happens to be the first one, it won't be taken into consideration
    //const intptr_t vtbl[N] = {vtbl0,vtbl1,vtbl2};
          intptr_t prev[N] = {vtbl0,vtbl1,vtbl2};
          intptr_t diff[N] = {};

    // Compute bits in which existing vtbl, including the newly added one, differ
    for (size_t i = 0; i <= descriptor->cache_mask; ++i)
    {
        typename cache_descriptor::stored_type* const st = descriptor->cache[i];

        XTL_ASSERT(st);

        for (size_t s = 0; s < N; s++)
            if (intptr_t vtbl = st->vtbl[s])
            {
                diff[s] |= prev[s] ^ vtbl;
                prev[s] = vtbl;
            }
    }

//#if XTL_DUMP_PERFORMANCE
//        std::clog << "\nVtbl:New" << std::bitset<8*sizeof(intptr_t)>((unsigned long long)vtbl) << " -> " << ((vtbl >> optimal_shift) & cache_mask) << '\t' << vtbl_typeid(vtbl).name() << std::endl;
//        *this >> std::clog;       
//#endif

    XTL_DUMP_PERFORMANCE_ONLY(++updates); // Record update
    collisions_before_update = renewed_collisions_before_update;      // Reset collisions counter

    bit_offset_t k  = bit_offset_t(req_bits(descriptor->cache_mask)); // current log_size
    bit_offset_t n  = bit_offset_t(req_bits(descriptor->used));       // needed  log_size
    bit_offset_t m[N] = { bit_offset_t(req_bits(diff[0])), 
                          bit_offset_t(req_bits(diff[1])), 
                          bit_offset_t(req_bits(diff[2])) 
                        }; // highest bit in which vtbls differ
    bit_offset_t z[N] = { bit_offset_t(trailing_zeros(static_cast<unsigned int>(diff[0]))), 
                          bit_offset_t(trailing_zeros(static_cast<unsigned int>(diff[1]))),
                          bit_offset_t(trailing_zeros(static_cast<unsigned int>(diff[2])))
                        }; // lowest bits in which vtbls do not differ
    bit_offset_t l1 = std::min(max_log_size,std::max(k,n));                          // lower bound for log_size iteration
    bit_offset_t l2 = std::min(max_log_size,std::max(k,bit_offset_t(n+max_log_inc)));// upper bound for log_size iteration
    bit_offset_t no = l1;                                             // current estimate of the best log_size
    bit_offset_t zo[N] = {z[0],z[1],z[2]};                            // current estimate of the best offset

    // FIX: Ensure we do not run out of stack, since in new implementation we don't have hash tables anymore
    // We do this to not resort to vectors and heap and keep counting on stack
    const size_t cache_histogram_size = 1 + ((1<<l2) - 1)/XTL_BIT_SIZE(intptr_t);
    XTL_VLA(cache_histogram, intptr_t, cache_histogram_size, 1 + ((1<<max_log_size) - 1)/XTL_BIT_SIZE(intptr_t)); // intptr_t cache_histogram[cache_histogram_size];

    size_t max_cache_entries = 0;

    // Iterate over allowed log sizes
    for (bit_offset_t i = l1; i <= l2; ++i)
    {
        const size_t   cache_size = 1<<i;
        const intptr_t cache_mask = cache_size-1;

        // Iterate over possible offsets
        for (bit_offset_t j0 = z[0]; j0 <= m[0]-i; ++j0)
        for (bit_offset_t j1 = z[1]; j1 <= m[1]-i; ++j1)
        for (bit_offset_t j2 = z[2]; j2 <= m[2]-i; ++j2)
        {
            std::memset(cache_histogram,0,cache_histogram_size*sizeof(intptr_t)); // Reset bit histogram to zeros
            XTL_BIT_SET(cache_histogram, interleave(vtbl0 >> j0, vtbl1 >> j1, vtbl2 >> j2) & cache_mask); // Mark the entry for new vtbl

            // Iterate over vtbl in old cache and see where they are mapped with log size i and offset j
            for (size_t c = 0; c <= descriptor->cache_mask; ++c)
            {
                typename cache_descriptor::stored_type* const st = descriptor->cache[c];

                XTL_ASSERT(st);

                if (intptr_t vt0 = st->vtbl[0])
                {
                    intptr_t vt1 = st->vtbl[1];
                    intptr_t vt2 = st->vtbl[2];
                    XTL_ASSERT(vt1);
                    XTL_ASSERT(vt2);
                    XTL_BIT_SET(cache_histogram, interleave(vt0 >> j0, vt1 >> j1, vt2 >> j2) & cache_mask); // Mark the entry for each vtbl
                }
            }

            size_t entries = 0;

            // Count the number of used entries
            for (size_t h = 0; h < cache_histogram_size; ++h)
                entries += bits_set(cache_histogram[h]);

            // Update best estimates
            if (entries > max_cache_entries)
            {
                max_cache_entries = entries;
                no = i;
                zo[0] = j0;
                zo[1] = j1;
                zo[2] = j2;
            }

            if (entries == descriptor->used+1)
            {
                // We found size and offset without conflicts, exit both loops
                i = l2+1; // to exit both for loops
                //collisions_before_update = initial_collisions_before_update; // since it is likely that adding another vtbl may still render no conflicts
                break;
            }
        }
    }

    if (no != k ||
        descriptor->optimal_shift[0] != zo[0] || 
        descriptor->optimal_shift[1] != zo[1] || 
        descriptor->optimal_shift[2] != zo[2]
       )
    {
        if (no < k)
            no = k; // We never shrink, while we preallocate based on number of case clauses or the minimum

        cache_descriptor* old = descriptor;
        #if defined(DBG_NEW)
            #undef new
        #endif
        descriptor = new(no) cache_descriptor(no,zo,std::move(*old));
        #if defined(DBG_NEW)
            #define new DBG_NEW
        #endif
        delete old;
    }

//#if XTL_DUMP_PERFORMANCE
//        std::clog << "After" << std::endl;
//        *this >> std::clog;       
//#endif
    typename cache_descriptor::stored_type* res = descriptor->get(vtbl0,vtbl1,vtbl2);
    XTL_ASSERT(res && res->vtbl[0] == vtbl0 && res->vtbl[1] == vtbl1 && res->vtbl[2] == vtbl2); // We have ensured enough space, so no need to check this explicitly
    last_table_size = descriptor->used;   // Update memoized value
    return res->value;
}

//------------------------------------------------------------------------------

#if XTL_DUMP_PERFORMANCE

template <typename T>
size_t last_non_zero_count(const T arr[/*n*/], size_t n, size_t m) // arr[i] <= m
{
    size_t result = 0;

    for (size_t i = 0; i <= m; ++i)
        if (std::count(arr,arr+n,i) > 0) 
            result = i;

    return result; // [0..m]
}

//------------------------------------------------------------------------------

template <typename T>
std::ostream& vtblmap<T>::operator>>(std::ostream& os) const
{
    std::ios::fmtflags fmt = os.flags(); // store flags

    os << file << '[' << line << ']' << ' ' << func << std::endl;

    size_t vtbl_count = descriptor->used;
    size_t log_size   = req_bits(descriptor->cache_mask);
    size_t cache_size = (1<<log_size);

    // We do this to not resort to vectors and heap and keep counting on stack
    XTL_VLAZ(cache_histogram, vtbl_count_t, cache_size, 1<<max_log_size); // vtbl_count_t cache_histogram[cache_size];

    std::vector<std::array<intptr_t,N>> vtbls(vtbl_count);
    std::vector<std::array<intptr_t,N>>::iterator q = vtbls.begin();

    intptr_t prev[N] = {};
    intptr_t diff[N] = {};

    // Compute bits in which existing vtbl, including the newly added one, differ
    for (size_t i = 0; i <= descriptor->cache_mask; ++i)
    {
        typename cache_descriptor::stored_type* const st = descriptor->cache[i];

        XTL_ASSERT(st);

        if (st->vtbl[0])
        {
            std::array<intptr_t,N> a;

            for (size_t s = 0; s < N; s++)
            {
                intptr_t vtbl = a[s] = st->vtbl[s];

                XTL_ASSERT(vtbl); 

                if (prev[s]) diff[s] |= prev[s] ^ vtbl; // Update diff with information about which bits in all vtbls differ
                prev[s] = vtbl;
            }

            cache_histogram[interleave(a[0] >> descriptor->optimal_shift[0], a[1] >> descriptor->optimal_shift[1], a[2] >> descriptor->optimal_shift[2]) & descriptor->cache_mask]++;
            XTL_ASSERT(size_t(q-vtbls.begin()) < vtbl_count); // Since we preallocated only that much
            *q++ = a;
        }
    }

    // Sort vtables to output them in address order
    std::sort(vtbls.begin(),vtbls.end());
    std::fill(&prev[0],&prev[N],0);

    for (q = vtbls.begin(); q != vtbls.end(); ++q)
    {
        const std::array<intptr_t,N>& a = *q;
        os << "Vtbl:   ";

        for (size_t s = 0; s < N; s++)
        {
            intptr_t vtbl = a[s];
            os << (s ? " | " : "") << std::bitset<8*sizeof(intptr_t)>((unsigned long long)vtbl); // Show binary value of vtbl pointer
        }

        os << " -> " << std::setw(3) << (interleave(a[0] >> descriptor->optimal_shift[0], a[1] >> descriptor->optimal_shift[1], a[2] >> descriptor->optimal_shift[2]) & descriptor->cache_mask)    // Show cache index it is mapped to
           << ' ';
            
        std::copy(a.begin(),a.end(),&prev[0]);

        if (cache_histogram[interleave(a[0] >> descriptor->optimal_shift[0], a[1] >> descriptor->optimal_shift[1], a[2] >> descriptor->optimal_shift[2]) & descriptor->cache_mask] > 1)
            os << '[' << cache_histogram[interleave(a[0] >> descriptor->optimal_shift[0], a[1] >> descriptor->optimal_shift[1], a[2] >> descriptor->optimal_shift[2]) & descriptor->cache_mask] << ']'; // Show have many vtbl falls into the same cache index
        else
            os << "   ";

        os << '\t';

        for (size_t s = 0; s < N; s++)
        {
            intptr_t vtbl = a[s];
            os << (s ? "," : "") << vtbl_typeid(vtbl).name(); // Show name of the class of this vtbl
        }

        os << std::endl; // Show name of the class of this vtbl
    }

    os.flags(fmt);

    std::string outstr;

    for (size_t s = 0; s < N; s++)
    {
        // FIX: G++ crashes when we use std::stringstream here, so we have to workaround it manually
        std::string str(8*sizeof(prev[0]),'0');

        for (size_t j = 1, i = 8*sizeof(prev[0]); i; --i, j<<=1)
            if (diff[s] & j)
                str[i-1] = j & (((1 << (log_size+N-1-s)/N) - 1) << descriptor->optimal_shift[s]) ? 'x' : 'X'; // Indicate bit positions used for computing the hash after interleaving
            else
            if (prev[s] & j)
                str[i-1] = '1';

        outstr += (s ? " | " : "") + str;
    }

//    double entropy  = -1;
//    double conflict = -1;
//    size_t entries  = 999999999;

    os  << "VTBLS:  "     << outstr       << std::endl
        << " clauses="    << std::setw(4) << clauses      // Number of case clauses in the match statement
        << " total="      << std::setw(5) << vtbl_count   // Total amount of vtbl pointers seen
        << " log_size="   << std::setw(2) << log_size     // log2 size required
        //<< " shift="      << std::setw(2) << descriptor->optimal_shift// optimal shift used
        //<< " width="      << std::setw(2) << str.find_last_of("X")-str.find_first_of("X")+1 // total spread of different bits
        << " updates="    << std::setw(2) << updates      // how many updates have been performed on the cache
        << " hits="       << std::setw(8) << hits         // how many hits have we had
        << " misses="     << std::setw(8) << misses       // how many misses have we had
        << " collisions=" << std::setw(8) << collisions   // how many misses were actual collisions
//        << " entries: "   << std::setw(5) << entries      // how many entires in the cache are used
//        << " Entropy: "   << std::setw(9) << std::fixed << std::setprecision(7) << entropy  // Entropy
//        << " Conflict: "  << std::setw(9) << std::fixed << std::setprecision(7) << conflict // Probability of conflict
        << " Stmt: "      << file << '[' << line << ']' << ' ' << func
        << ";\n";
    os.flags(fmt);
#if 0
    bit_offset_t k  = req_bits(cache_mask);     // current log_size
    bit_offset_t n  = req_bits(vtbl_count-1);   // needed  log_size
    bit_offset_t m  = req_bits(diff);           // highest bit in which vtbls differ
    bit_offset_t z  = trailing_zeros(static_cast<unsigned int>(diff)); // amount of lowest bits in which vtbls do not differ
    bit_offset_t l1 = std::min(max_log_size,std::min(k,n));
    bit_offset_t l2 = std::min(max_log_size,std::max(k,bit_offset_t(n+max_log_inc)));

    for (bit_offset_t i = l1; i <= l2; ++i)
    {
        for (bit_offset_t j = z; j <= m-i; ++j)
        {
            double e, p;
            size_t t = get_stats_for(i,j,e,p);
            os  << "\tlog_size=" << std::setw(2) << int(i) 
                << " shift="     << std::setw(2) << int(j) 
                << " Entropy="   << std::setw(10)<< std::fixed << std::setprecision(7) << e 
                << " Conflict="  << std::setw(9) << std::fixed << std::setprecision(7) << p << (t == vtbl_count ? " \t*" : "") 
                << std::endl;
        }
    }
    os.flags(fmt);
#endif

    size_t d0 = 0, d1 = 0;
    double cache_conflict = 0.0;

    // Compute cache's probability of conflict
    for (size_t j = 0; j < cache_size; ++j)
    {
        size_t c = cache_histogram[j];
        if (c == 0) ++d0;
        if (c == 1) ++d1;
        if (c >  1) cache_conflict += double(c-1)/vtbl_count;
    }
    
    // Show cache's parameters
    os  << "\nCache:"
        << " buckets="     << std::setw(4) << cache_size
        << " load_factor=" << std::setw(4) << std::fixed << std::setprecision(2) << double(cache_size-d0)/cache_size
        << " perfect="     << std::setw(3) << (vtbl_count ? d1*100/vtbl_count : 0) << '%'
        //<< " sizeof(ent)=" << std::setw(2) << sizeof(cache_entry)
        << " conflict="    << std::setw(9) << std::fixed << std::setprecision(7) << cache_conflict // Probability of conflict in cache
        << " Stmt: "       << file << '[' << line << ']' << ' ' << func
        << "; ";

    size_t cache_last_non_zero_count = last_non_zero_count(cache_histogram,cache_size,vtbl_count);

    // Print cache histogram
    for (size_t i = 0; i <= cache_last_non_zero_count; ++i)
    {
        size_t d = std::count(cache_histogram,cache_histogram+cache_size,i);

        if (!i) os << std::setw(3) << d*100/cache_size << "% unused " << '[' << line << ']';
        os << std::setw(2) << i << "->" << d << "; ";
    }

    os.flags(fmt);

    // Print map of cache entries occupation
    for (size_t j = 0; j < cache_size; ++j)
    {
        const size_t entries_per_row = 64;

        if (j % entries_per_row == 0)
            os << std::endl << std::setw(4) << std::hex << j << ": ";

        size_t n = cache_histogram[j];

        char c;

        switch (n)
        {
        case 0:  c = '.'; break;
        case 1:  c = '1'; break;
        case 2:  c = '2'; break;
        case 3:  c = '3'; break;
        case 4:  c = '4'; break;
        case 5:  c = '5'; break;
        default: c = 'X'; break;
        }
        
        os << c;
    }

    os.flags(fmt);
    return os << std::endl;
}
#endif
//------------------------------------------------------------------------------

/// Data structure used by our Match statements to associate jump target and the 
/// required offset with the vtbl-pointer.
struct type_switch_info
{
    std::ptrdiff_t offset[N]; ///< Required this-pointer offset to the source sub-object
    std::size_t    target;    ///< Case label of the jump target of Match statement
};

//------------------------------------------------------------------------------

} // of namespace mch
