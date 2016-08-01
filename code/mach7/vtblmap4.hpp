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
/// This file defines class vtblmap<N,T> used for fast mapping of N vtbl
/// pointers to type T.
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
//   through this vtbl_map can have 0 collisions for log size N, N+1
// - Cache arrangement with no conflicts can be a good indicator that
//   such arrangement will still be possible after adding one more vtbl.
// - Cache arrangement with conflicts will have them with more vtbls 
//   added unless the cache size got increased.
// - In most cases we'd want at least one rearrangement to happen as
//   default value for irrelevant bits is usually irrelevant for a
//   given vtbl_map and has to be recomputed.
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
#include <cstdarg>
#include "ptrtools.hpp"  // Helper functions to work with pointers
#include <xtl/xtl.hpp>   // XTL subtyping definitions

#if XTL_DUMP_PERFORMANCE
// For print out purposes only
#include <array>
#include <bitset>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#endif

#if !defined(XTL_HASH_VTBL_ARRAY)
#define XTL_HASH_VTBL_ARRAY 1
#endif

#define XTL_VTBL_HASHING(...) XTL_IF(XTL_NOT(XTL_HASH_VTBL_ARRAY), XTL_EMPTY(), XTL_EXPAND(__VA_ARGS__))

#if !defined(XTL_USE_LCG_WALK)
#define XTL_USE_LCG_WALK 1
#endif

namespace mch ///< Mach7 library namespace
{

//------------------------------------------------------------------------------

/// Type capable of representing bit offsets and bit counts in intptr_t
//typedef unsigned char  bit_offset_t;
typedef size_t  bit_offset_t;

/// The smallest integral type capable of representing the number N of different 
/// vtbl pointers in the program. Roughly N should be equal to some constant c
/// multiplied by the number of different classes polymorphic classes in the 
/// program. Constant c accounts for potential multiple inheritance.
typedef unsigned short vtbl_count_t;
//typedef size_t vtbl_count_t;

//------------------------------------------------------------------------------

const bit_offset_t min_log_size      = XTL_MIN_LOG_SIZE; ///< Log of the smallest cache size to start from
const bit_offset_t max_stack_log_size= XTL_MAX_STACK_LOG_SIZE; ///< Log of the maximum stack size we can reserve to do some histogram computations.
const bit_offset_t max_log_inc       = XTL_MAX_LOG_INC;  ///< Log of the maximum allowed increased from the minimum requred log size (1 means twice from the min required size)
//const vtbl_count_t min_expected_size = 1 << min_log_size;
const bit_offset_t irrelevant_bits   = 0; // XTL_IRRELEVANT_VTBL_BITS; // FIX: temporarily set to 0 for experiments with XTL subtyping where we don't work with vtbl-pointers
const int initial_collisions_before_update = 16;

#if XTL_USE_LCG_WALK
// In case of collisions in cache, we are going try finding next available slot
// with LCG. This should avoid accumulating collisions in few places and instead
// distribute them over the entire cache.
const size_t lcg_a = 5;   ///< \see http://en.wikipedia.org/wiki/Linear_congruential_generator and Hull-Dobell Theorem
const size_t lcg_c = 131; ///< \see http://en.wikipedia.org/wiki/Linear_congruential_generator and Hull-Dobell Theorem
#endif

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

/// Show binary value of vtbl pointer(s)
template <size_t N>
inline void vtbl_bin_print(const intptr_t (&vtbl)[N], std::ostream& os)
{
    for (size_t s = 0; s < N; s++)
        os << (s ? " | " : "") 
           << std::bitset<8*sizeof(intptr_t)>((unsigned long long)vtbl[s]);
}
        
//------------------------------------------------------------------------------

/// Show name of the class of this vtbl pointer(s)
template <size_t N>
inline void vtbl_class_print(const intptr_t (&vtbl)[N], std::ostream& os)
{
    for (size_t s = 0; s < N; s++)
        os << (s ? " \t| " : "") 
         /*<< vtbl_typeid(vtbl[s]).name()*/;
}

//------------------------------------------------------------------------------

template <typename T, size_t N>
inline const T (&to_array(const std::array<T,N>& arr))[N]
{
    return *reinterpret_cast<const T (*)[N]>(&arr[0]);
}
#endif // XTL_DUMP_PERFORMANCE

//------------------------------------------------------------------------------

template <typename T, size_t N>
inline T get_hash(const T (&a)[N])
{
    T result = a[0];
    for (size_t i = 1; i < N; ++i) result ^= a[i];
    return result;
}

//------------------------------------------------------------------------------

template <typename T, size_t N>
inline bool array_equal(const T (&a)[N], const T (&b)[N])
{
    // OVERALL: Random: 312% slower V= 67 M=276
    for (size_t i = 0; i < N; ++i)
        // OVERALL: Random: 297% slower V= 66 M=264 - XTL_LIKELY
        // OVERALL: Random: 306% slower V= 67 M=273 - XTL_UNLIKELY
        if (XTL_LIKELY(a[i] != b[i])) 
            return false;
    return true;

    // OVERALL: Random: 484% slower V= 67 M=393
    //T result = 0;
    //for (size_t i = 0; i < N; ++i)
    //    result |= a[i] ^ b[i];
    //return result == 0;

    // OVERALL: Random:1830% slower V= 71 M=1380
    //return memcmp(a,b,sizeof(a)) == 0;

    // OVERALL: Random:1818% slower V= 71 M=1364
    // return std::equal(&a[0],&a[N],&b[0]);
}

//------------------------------------------------------------------------------

template <typename T>
inline bool array_equal(const T (&a)[1], const T (&b)[1])
{
    return a[0] == b[0];
}

//------------------------------------------------------------------------------

template <typename T, size_t N>
inline void array_copy(const T (&src)[N], T (&tgt)[N])
{
    for (size_t i = 0; i < N; ++i) tgt[i] = src[i];
}

//------------------------------------------------------------------------------

template <typename T, size_t N>
inline void array_fill(const T (&a)[N], const T& v)
{
    for (size_t i = 0; i < N; ++i) a[i] = v;
}

//------------------------------------------------------------------------------

/// Type of the stored values, which is a pair of vtbl-pointer and T value.
/// Taken outside the class to be able to specialize it for N=1 to not do any 
/// hashing since hash will be the value of the only vtbl pointer.
template <size_t N, typename T>
struct stored_type_for
{
    stored_type_for() : XTL_VTBL_HASHING(hash(0),) vtbl(), value() {}

    XTL_VTBL_HASHING(intptr_t hash;)     ///< hash of vtbl[i] for comparing vtbl for large N (> 2)
    intptr_t vtbl[N];  ///< v-table pointers of the value
    T        value;    ///< value associated with the v-table pointers vtbl[]

    /// Helper function to in-place construct stored_type inside uninitialized memory
    void construct()    { new(this) stored_type_for(); }
    /// Helper function to manually destroy stored_type
    void destroy()      { this->~stored_type_for(); }
    /// Checks whether given entry is already occupied
    bool occupied() const  { return vtbl[0] != 0; }
    /// Checks whether given entry is vacant
    bool vacant()   const  { return !occupied(); }
    /// Checks whether passed set of vtbl pointers matches ours set
    bool is_for(const intptr_t (&v)[N]) const { return array_equal(vtbl,v); }
    /// Optimized is_for when hash of v is known
    XTL_VTBL_HASHING(bool is_for(const intptr_t (&v)[N], intptr_t h) const { return h == hash && array_equal(vtbl,v); })
    /// Copies passed set of vtbl pointers into ours
    stored_type_for& operator=(const intptr_t (&v)[N]) { array_copy(v,vtbl); XTL_VTBL_HASHING(hash = get_hash(vtbl);) return *this; }
};

//------------------------------------------------------------------------------

/// Type of the stored values, which is a pair of vtbl-pointer and T value.
/// The specialization for 1 vtbl pointer that doesn't do the hashing.
template <typename T>
struct stored_type_for<1,T>
{
    stored_type_for() : vtbl(), value() {}

    intptr_t vtbl[1];  ///< v-table pointers of the value
    T        value;    ///< value associated with the v-table pointers vtbl[]

    /// Helper function to in-place construct stored_type inside uninitialized memory
    void construct()    { new(this) stored_type_for(); }
    /// Helper function to manually destroy stored_type
    void destroy()      { this->~stored_type_for(); }
    /// Checks whether given entry is already occupied
    bool occupied() const  { return vtbl[0] != 0; }
    /// Checks whether given entry is vacant
    bool vacant()   const  { return !occupied(); }
    /// Checks whether passed set of vtbl pointers matches ours set
    bool is_for(const intptr_t (&v)[1]) const { return vtbl[0] == v[0]; }
    /// Optimized is_for when hash of v is known
    XTL_VTBL_HASHING(bool is_for(const intptr_t (&v)[1], intptr_t h) const { XTL_UNUSED(h); XTL_ASSERT(h == v[0]); return vtbl[0] == v[0]; })
    /// Copies passed set of vtbl pointers into ours
    stored_type_for& operator=(const intptr_t (&v)[1]) { vtbl[0] = v[0]; return *this; }
};

//------------------------------------------------------------------------------

template <size_t N, typename T>
class vtbl_map
{
private:

    /// A helper data structure that is swapped during updates to 
    /// cache parameters k and l.
    struct cache_descriptor
    {
        /// Type of the stored values, which is a pair of vtbl-pointer and T value.
        typedef stored_type_for<N,T> stored_type;

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
        void operator delete(void* p, size_t) { ::delete[](static_cast<char*>(p)); } // We cast to char* to avoid warning on deleting void*, which is undefined

        /// We also provide non-placement delete operator since it doesn't really depend on extra arguments.
        void operator delete(void* p)         { ::delete[](static_cast<char*>(p)); } // We cast to char* to avoid warning on deleting void*, which is undefined

        /// Creates new cache_descriptor based on parameters k and l of the hashing function
        cache_descriptor(
            const size_t       log_size,               ///< Parameter k of the cache - the log of the size of the cache
            const bit_offset_t shift = irrelevant_bits ///< Parameter l of the cache - number of irrelevant bits on the right to remove
        );

        /// Creates new cache_descriptor based on parameters k and l of the 
        /// hashing function as well as a reference to the cache_descriptor it
        /// is going to replace.
        cache_descriptor(
            const size_t       log_size,    ///< Parameter k of the cache - the log of the size of the cache                
            const bit_offset_t (&shifts)[N],///< Parameter l of the cache - number of irrelevant bits on the right to remove
        #if defined(XTL_NO_RVALREF)
            cache_descriptor&  old          ///< cache_descriptor we will supposedly replace
        #else
            cache_descriptor&& old          ///< cache_descriptor we will supposedly replace
        #endif
        );

        /// Deallocates memory used for elements.
       ~cache_descriptor();

        bool    is_full() const { return used > cache_mask; } ///< Checks whether cache is full
        size_t     size() const { return cache_mask+1; }      ///< Number of entries in cache
        size_t lcg_next(size_t j) const { return (lcg_a*j + lcg_c) & cache_mask; }

        size_t memory_used() const 
        {
            return sizeof(cache_descriptor)                                   // Descriptor itself
                + (cache_mask+1-XTL_VARIABLE_SIZE_ARRAY)*sizeof(stored_type*) // Pointers in cache
                + (cache_mask+1)*sizeof(stored_type);                         // Actual cached values pointers in cache point to
        }

        /// Global function computing cache index for a given vtbl pointers, offsets and cache mask
        static inline size_t cache_index(const intptr_t vtbl[N], const bit_offset_t shifts[N], size_t cache_mask)
        {
            intptr_t vtbl_shifted[N];

            for (size_t i = 0; i < N; ++i)
                vtbl_shifted[i] = vtbl[i] >> shifts[i];

            return interleave(vtbl_shifted) & cache_mask;
        }

        /// Computes cache index for current optimal offsets and cache mask.
        size_t cache_index(const intptr_t vtbl[N]) const { return cache_index(vtbl,optimal_shift,cache_mask); }

        /// Re-establishes invariant that vtbls can only be in the cache 
        /// entry that correspond to their cache index, unless that entry
        /// is already taken.
        void put_entries_in_right_place();

        /// Main function that will be used to get a reference to the stored element. 
        stored_type* get(const intptr_t (&vtbl)[N]) noexcept { return get(vtbl,cache_index(vtbl)); }

        /// Optimization for when the cache index has already been computed
        stored_type* get(const intptr_t (&vtbl)[N], size_t j) noexcept;

        /// Computes the number of entries an existing set of vtbl-pointer tuples 
        /// extended with the new one will occupy in cache of a given #log_size 
        /// with given #offsets
        size_t entries_for(const intptr_t (&vtbl)[N], size_t log_size, const bit_offset_t (&offsets)[N]) const;

    private:

        cache_descriptor(const cache_descriptor&);            ///< No copy constructor
        cache_descriptor& operator=(const cache_descriptor&); ///< No assignment operator

    }; // of class cache_descriptor

private:

    vtbl_map(const vtbl_map&);            ///< No copy constructor
    vtbl_map& operator=(const vtbl_map&); ///< No assignment operator

public:
    
#if XTL_DUMP_PERFORMANCE
    #if defined(DBG_NEW)
        #undef new
    #endif
    vtbl_map(const char* fl, size_t ln, const char* fn, const vtbl_count_t& num_clauses) : 
        descriptor(new(min_log_size) cache_descriptor(min_log_size)),
        case_clauses(num_clauses),
        last_table_size(0),
        collisions_before_update(initial_collisions_before_update),
        prev_collisions_before_update(initial_collisions_before_update),
        file(fl), 
        line(ln),
        func(fn),
        updates(0), 
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
    vtbl_map(const vtbl_count_t& num_clauses) : 
        descriptor(new(min_log_size) cache_descriptor(min_log_size)),
        case_clauses(num_clauses),
        last_table_size(0),
        collisions_before_update(initial_collisions_before_update),
        prev_collisions_before_update(initial_collisions_before_update)
        XTL_DUMP_PERFORMANCE_ONLY(,file("unspecified"), line(0), func("unspecified"), updates(0), hits(0), misses(0), collisions(0))
    {}
    #if defined(DBG_NEW)
        #define new DBG_NEW
    #endif

   ~vtbl_map()
    {
        XTL_DUMP_PERFORMANCE_ONLY(std::clog << *this << std::endl);
        delete descriptor;
    }

    size_t memory_used() const 
    {
        XTL_ASSERT(descriptor);
        return sizeof(vtbl_map) + descriptor->memory_used();
    }

    /// This is the main function to get the value of type T associated with
    /// the (vtbl0,...,vtblN) of given pointers.
    ///
    /// \note The function returns the value "by reference" to indicate that you 
    ///       may take address or change the value of the cell!
    /*
    inline T& get(intptr_t vtbl0, ...) noexcept
    {
        XTL_ASSERT(descriptor); // Allocated in constructor, deallocated in destructor

        intptr_t vtbl[N] = {vtbl0};

        va_list vl;
        va_start(vl,vtbl0);

        for (size_t i = 1; i < N; i++)
        {
            intptr_t vtbl_i = va_arg(vl,intptr_t);
            XTL_ASSERT(vtbl_i); // Must be a valid vtbl pointer
            vtbl[i] = vtbl_i;
        }

        va_end(vl);
        return get(vtbl);
    }
    */
    inline T& get(const intptr_t (&vtbl)[N]) noexcept
    {
        size_t j = descriptor->cache_index(vtbl);  // Index of location where it should be
        typename cache_descriptor::stored_type*& ce = descriptor->cache[j]; // Location where it should be

        XTL_ASSERT(ce);   // Since we pre-allocate all entries

        if (XTL_LIKELY(ce->is_for(vtbl)))
        {
            XTL_DUMP_PERFORMANCE_ONLY(++hits);
            return ce->value;
        }
        else
        {
            XTL_DUMP_PERFORMANCE_ONLY(++misses);
            XTL_DUMP_PERFORMANCE_ONLY(if (ce->occupied()) ++collisions);

            if (XTL_UNLIKELY(
                descriptor->is_full()                     // No entries left for possibly new vtbl in the cache
                || (ce->occupied()                        // Collision - the entry for vtbl is already occupied
                && --collisions_before_update <= 0        // We had sufficiently many collisions to justify call
                && descriptor->used != last_table_size))) // There was at least one vtbl added since last update
                return update(vtbl);                      // try to rearrange cache

            // Try to find entry with our vtbl and swap it with where it is expected to be
            descriptor->get(vtbl,j); // This will bring correct pointer into ce
            XTL_ASSERT(ce->is_for(vtbl));
            return ce->value;
        }
    }

//------------------------------------------------------------------------------

    // FIX: temporary copypaste of get overloads below into xtl_get. Make get more generic to work with both cases

    //template <typename S1> inline auto xtl_get(const S1* s1) -> typename std::enable_if<!xtl::is_poly_morphic<S1>::value,T&>::type { static T dummy; return dummy; }
    template <typename S1> inline auto xtl_get(const S1* s1) -> typename std::enable_if< xtl::is_poly_morphic<S1>::value,T&>::type { intptr_t vtbl[1] = {vtbl_of(s1)}; return get(vtbl); }

    //template <typename S1, typename S2> inline auto xtl_get(const S1* s1, const S2* s2) -> typename std::enable_if<!xtl::is_poly_morphic<S1>::value && !xtl::is_poly_morphic<S2>::value,T&>::type { static T dummy; return dummy; }
    template <typename S1, typename S2> inline auto xtl_get(const S1*   , const S2* s2) -> typename std::enable_if<!xtl::is_poly_morphic<S1>::value &&  xtl::is_poly_morphic<S2>::value,T&>::type { intptr_t vtbl[1] = {            vtbl_of(s2)}; return get(vtbl); }
    template <typename S1, typename S2> inline auto xtl_get(const S1* s1, const S2*   ) -> typename std::enable_if< xtl::is_poly_morphic<S1>::value && !xtl::is_poly_morphic<S2>::value,T&>::type { intptr_t vtbl[1] = {vtbl_of(s1)            }; return get(vtbl); }
    template <typename S1, typename S2> inline auto xtl_get(const S1* s1, const S2* s2) -> typename std::enable_if< xtl::is_poly_morphic<S1>::value &&  xtl::is_poly_morphic<S2>::value,T&>::type { intptr_t vtbl[2] = {vtbl_of(s1),vtbl_of(s2)}; return get(vtbl); }

    //template <typename S1, typename S2, typename S3> inline auto xtl_get(const S1*   , const S2*   , const S3*   ) -> typename std::enable_if<!xtl::is_poly_morphic<S1>::value && !xtl::is_poly_morphic<S2>::value && !xtl::is_poly_morphic<S3>::value,T&>::type { static T dummy; return dummy; }
    template <typename S1, typename S2, typename S3> inline auto xtl_get(const S1*   , const S2*   , const S3* s3) -> typename std::enable_if<!xtl::is_poly_morphic<S1>::value && !xtl::is_poly_morphic<S2>::value &&  xtl::is_poly_morphic<S3>::value,T&>::type { intptr_t vtbl[1] = {                        vtbl_of(s3)}; return get(vtbl); }
    template <typename S1, typename S2, typename S3> inline auto xtl_get(const S1*   , const S2* s2, const S3*   ) -> typename std::enable_if<!xtl::is_poly_morphic<S1>::value &&  xtl::is_poly_morphic<S2>::value && !xtl::is_poly_morphic<S3>::value,T&>::type { intptr_t vtbl[1] = {            vtbl_of(s2)            }; return get(vtbl); }
    template <typename S1, typename S2, typename S3> inline auto xtl_get(const S1*   , const S2* s2, const S3* s3) -> typename std::enable_if<!xtl::is_poly_morphic<S1>::value &&  xtl::is_poly_morphic<S2>::value &&  xtl::is_poly_morphic<S3>::value,T&>::type { intptr_t vtbl[2] = {            vtbl_of(s2),vtbl_of(s3)}; return get(vtbl); }
    template <typename S1, typename S2, typename S3> inline auto xtl_get(const S1* s1, const S2*   , const S3*   ) -> typename std::enable_if< xtl::is_poly_morphic<S1>::value && !xtl::is_poly_morphic<S2>::value && !xtl::is_poly_morphic<S3>::value,T&>::type { intptr_t vtbl[1] = {vtbl_of(s1)                        }; return get(vtbl); }
    template <typename S1, typename S2, typename S3> inline auto xtl_get(const S1* s1, const S2*   , const S3* s3) -> typename std::enable_if< xtl::is_poly_morphic<S1>::value && !xtl::is_poly_morphic<S2>::value &&  xtl::is_poly_morphic<S3>::value,T&>::type { intptr_t vtbl[2] = {vtbl_of(s1),            vtbl_of(s3)}; return get(vtbl); }
    template <typename S1, typename S2, typename S3> inline auto xtl_get(const S1* s1, const S2* s2, const S3*   ) -> typename std::enable_if< xtl::is_poly_morphic<S1>::value &&  xtl::is_poly_morphic<S2>::value && !xtl::is_poly_morphic<S3>::value,T&>::type { intptr_t vtbl[2] = {vtbl_of(s1),vtbl_of(s2)            }; return get(vtbl); }
    template <typename S1, typename S2, typename S3> inline auto xtl_get(const S1* s1, const S2* s2, const S3* s3) -> typename std::enable_if< xtl::is_poly_morphic<S1>::value &&  xtl::is_poly_morphic<S2>::value &&  xtl::is_poly_morphic<S3>::value,T&>::type { intptr_t vtbl[3] = {vtbl_of(s1),vtbl_of(s2),vtbl_of(s3)}; return get(vtbl); }

    //template <typename S1, typename S2, typename S3, typename S4> inline auto xtl_get(const S1* s1, const S2* s2, const S3* s3, const S4* s4) -> typename std::enable_if<!xtl::is_poly_morphic<S1>::value && !xtl::is_poly_morphic<S2>::value && !xtl::is_poly_morphic<S3>::value && !xtl::is_poly_morphic<S4>::value,T&>::type { static T dummy; return dummy; }
    template <typename S1, typename S2, typename S3, typename S4> inline auto xtl_get(const S1*   , const S2*   , const S3*   , const S4* s4) -> typename std::enable_if<!xtl::is_poly_morphic<S1>::value && !xtl::is_poly_morphic<S2>::value && !xtl::is_poly_morphic<S3>::value &&  xtl::is_poly_morphic<S4>::value,T&>::type { intptr_t vtbl[1] = {                                    vtbl_of(s4)}; return get(vtbl); }
    template <typename S1, typename S2, typename S3, typename S4> inline auto xtl_get(const S1*   , const S2*   , const S3* s3, const S4*   ) -> typename std::enable_if<!xtl::is_poly_morphic<S1>::value && !xtl::is_poly_morphic<S2>::value &&  xtl::is_poly_morphic<S3>::value && !xtl::is_poly_morphic<S4>::value,T&>::type { intptr_t vtbl[1] = {                        vtbl_of(s3)            }; return get(vtbl); }
    template <typename S1, typename S2, typename S3, typename S4> inline auto xtl_get(const S1*   , const S2*   , const S3* s3, const S4* s4) -> typename std::enable_if<!xtl::is_poly_morphic<S1>::value && !xtl::is_poly_morphic<S2>::value &&  xtl::is_poly_morphic<S3>::value &&  xtl::is_poly_morphic<S4>::value,T&>::type { intptr_t vtbl[2] = {                        vtbl_of(s3),vtbl_of(s4)}; return get(vtbl); }
    template <typename S1, typename S2, typename S3, typename S4> inline auto xtl_get(const S1*   , const S2* s2, const S3*   , const S4*   ) -> typename std::enable_if<!xtl::is_poly_morphic<S1>::value &&  xtl::is_poly_morphic<S2>::value && !xtl::is_poly_morphic<S3>::value && !xtl::is_poly_morphic<S4>::value,T&>::type { intptr_t vtbl[1] = {            vtbl_of(s2)                        }; return get(vtbl); }
    template <typename S1, typename S2, typename S3, typename S4> inline auto xtl_get(const S1*   , const S2* s2, const S3*   , const S4* s4) -> typename std::enable_if<!xtl::is_poly_morphic<S1>::value &&  xtl::is_poly_morphic<S2>::value && !xtl::is_poly_morphic<S3>::value &&  xtl::is_poly_morphic<S4>::value,T&>::type { intptr_t vtbl[2] = {            vtbl_of(s2),            vtbl_of(s4)}; return get(vtbl); }
    template <typename S1, typename S2, typename S3, typename S4> inline auto xtl_get(const S1*   , const S2* s2, const S3* s3, const S4*   ) -> typename std::enable_if<!xtl::is_poly_morphic<S1>::value &&  xtl::is_poly_morphic<S2>::value &&  xtl::is_poly_morphic<S3>::value && !xtl::is_poly_morphic<S4>::value,T&>::type { intptr_t vtbl[2] = {            vtbl_of(s2),vtbl_of(s3)            }; return get(vtbl); }
    template <typename S1, typename S2, typename S3, typename S4> inline auto xtl_get(const S1*   , const S2* s2, const S3* s3, const S4* s4) -> typename std::enable_if<!xtl::is_poly_morphic<S1>::value &&  xtl::is_poly_morphic<S2>::value &&  xtl::is_poly_morphic<S3>::value &&  xtl::is_poly_morphic<S4>::value,T&>::type { intptr_t vtbl[3] = {            vtbl_of(s2),vtbl_of(s3),vtbl_of(s4)}; return get(vtbl); }
    template <typename S1, typename S2, typename S3, typename S4> inline auto xtl_get(const S1* s1, const S2*   , const S3*   , const S4*   ) -> typename std::enable_if< xtl::is_poly_morphic<S1>::value && !xtl::is_poly_morphic<S2>::value && !xtl::is_poly_morphic<S3>::value && !xtl::is_poly_morphic<S4>::value,T&>::type { intptr_t vtbl[1] = {vtbl_of(s1)                                    }; return get(vtbl); }
    template <typename S1, typename S2, typename S3, typename S4> inline auto xtl_get(const S1* s1, const S2*   , const S3*   , const S4* s4) -> typename std::enable_if< xtl::is_poly_morphic<S1>::value && !xtl::is_poly_morphic<S2>::value && !xtl::is_poly_morphic<S3>::value &&  xtl::is_poly_morphic<S4>::value,T&>::type { intptr_t vtbl[2] = {vtbl_of(s1),                        vtbl_of(s4)}; return get(vtbl); }
    template <typename S1, typename S2, typename S3, typename S4> inline auto xtl_get(const S1* s1, const S2*   , const S3* s3, const S4*   ) -> typename std::enable_if< xtl::is_poly_morphic<S1>::value && !xtl::is_poly_morphic<S2>::value &&  xtl::is_poly_morphic<S3>::value && !xtl::is_poly_morphic<S4>::value,T&>::type { intptr_t vtbl[2] = {vtbl_of(s1),            vtbl_of(s3)            }; return get(vtbl); }
    template <typename S1, typename S2, typename S3, typename S4> inline auto xtl_get(const S1* s1, const S2*   , const S3* s3, const S4* s4) -> typename std::enable_if< xtl::is_poly_morphic<S1>::value && !xtl::is_poly_morphic<S2>::value &&  xtl::is_poly_morphic<S3>::value &&  xtl::is_poly_morphic<S4>::value,T&>::type { intptr_t vtbl[3] = {vtbl_of(s1),            vtbl_of(s3),vtbl_of(s4)}; return get(vtbl); }
    template <typename S1, typename S2, typename S3, typename S4> inline auto xtl_get(const S1* s1, const S2* s2, const S3*   , const S4*   ) -> typename std::enable_if< xtl::is_poly_morphic<S1>::value &&  xtl::is_poly_morphic<S2>::value && !xtl::is_poly_morphic<S3>::value && !xtl::is_poly_morphic<S4>::value,T&>::type { intptr_t vtbl[2] = {vtbl_of(s1),vtbl_of(s2)                        }; return get(vtbl); }
    template <typename S1, typename S2, typename S3, typename S4> inline auto xtl_get(const S1* s1, const S2* s2, const S3*   , const S4* s4) -> typename std::enable_if< xtl::is_poly_morphic<S1>::value &&  xtl::is_poly_morphic<S2>::value && !xtl::is_poly_morphic<S3>::value &&  xtl::is_poly_morphic<S4>::value,T&>::type { intptr_t vtbl[3] = {vtbl_of(s1),vtbl_of(s2),            vtbl_of(s4)}; return get(vtbl); }
    template <typename S1, typename S2, typename S3, typename S4> inline auto xtl_get(const S1* s1, const S2* s2, const S3* s3, const S4*   ) -> typename std::enable_if< xtl::is_poly_morphic<S1>::value &&  xtl::is_poly_morphic<S2>::value &&  xtl::is_poly_morphic<S3>::value && !xtl::is_poly_morphic<S4>::value,T&>::type { intptr_t vtbl[3] = {vtbl_of(s1),vtbl_of(s2),vtbl_of(s3)            }; return get(vtbl); }
    template <typename S1, typename S2, typename S3, typename S4> inline auto xtl_get(const S1* s1, const S2* s2, const S3* s3, const S4* s4) -> typename std::enable_if< xtl::is_poly_morphic<S1>::value &&  xtl::is_poly_morphic<S2>::value &&  xtl::is_poly_morphic<S3>::value &&  xtl::is_poly_morphic<S4>::value,T&>::type { intptr_t vtbl[4] = {vtbl_of(s1),vtbl_of(s2),vtbl_of(s3),vtbl_of(s4)}; return get(vtbl); }

//------------------------------------------------------------------------------

  //template <typename S1> inline auto get(const S1* s1) -> typename std::enable_if<!std::is_polymorphic<S1>::value,T&>::type { static T dummy; return dummy; }
    template <typename S1> inline auto get(const S1* s1) -> typename std::enable_if< std::is_polymorphic<S1>::value,T&>::type { intptr_t vtbl[1] = {vtbl_of(s1)}; return get(vtbl); }

  //template <typename S1, typename S2> inline auto get(const S1* s1, const S2* s2) -> typename std::enable_if<!std::is_polymorphic<S1>::value && !std::is_polymorphic<S2>::value,T&>::type { static T dummy; return dummy; }
    template <typename S1, typename S2> inline auto get(const S1*   , const S2* s2) -> typename std::enable_if<!std::is_polymorphic<S1>::value &&  std::is_polymorphic<S2>::value,T&>::type { intptr_t vtbl[1] = {            vtbl_of(s2)}; return get(vtbl); }
    template <typename S1, typename S2> inline auto get(const S1* s1, const S2*   ) -> typename std::enable_if< std::is_polymorphic<S1>::value && !std::is_polymorphic<S2>::value,T&>::type { intptr_t vtbl[1] = {vtbl_of(s1)            }; return get(vtbl); }
    template <typename S1, typename S2> inline auto get(const S1* s1, const S2* s2) -> typename std::enable_if< std::is_polymorphic<S1>::value &&  std::is_polymorphic<S2>::value,T&>::type { intptr_t vtbl[2] = {vtbl_of(s1),vtbl_of(s2)}; return get(vtbl); }

  //template <typename S1, typename S2, typename S3> inline auto get(const S1*   , const S2*   , const S3*   ) -> typename std::enable_if<!std::is_polymorphic<S1>::value && !std::is_polymorphic<S2>::value && !std::is_polymorphic<S3>::value,T&>::type { static T dummy; return dummy; }
    template <typename S1, typename S2, typename S3> inline auto get(const S1*   , const S2*   , const S3* s3) -> typename std::enable_if<!std::is_polymorphic<S1>::value && !std::is_polymorphic<S2>::value &&  std::is_polymorphic<S3>::value,T&>::type { intptr_t vtbl[1] = {                        vtbl_of(s3)}; return get(vtbl); }
    template <typename S1, typename S2, typename S3> inline auto get(const S1*   , const S2* s2, const S3*   ) -> typename std::enable_if<!std::is_polymorphic<S1>::value &&  std::is_polymorphic<S2>::value && !std::is_polymorphic<S3>::value,T&>::type { intptr_t vtbl[1] = {            vtbl_of(s2)            }; return get(vtbl); }
    template <typename S1, typename S2, typename S3> inline auto get(const S1*   , const S2* s2, const S3* s3) -> typename std::enable_if<!std::is_polymorphic<S1>::value &&  std::is_polymorphic<S2>::value &&  std::is_polymorphic<S3>::value,T&>::type { intptr_t vtbl[2] = {            vtbl_of(s2),vtbl_of(s3)}; return get(vtbl); }
    template <typename S1, typename S2, typename S3> inline auto get(const S1* s1, const S2*   , const S3*   ) -> typename std::enable_if< std::is_polymorphic<S1>::value && !std::is_polymorphic<S2>::value && !std::is_polymorphic<S3>::value,T&>::type { intptr_t vtbl[1] = {vtbl_of(s1)                        }; return get(vtbl); }
    template <typename S1, typename S2, typename S3> inline auto get(const S1* s1, const S2*   , const S3* s3) -> typename std::enable_if< std::is_polymorphic<S1>::value && !std::is_polymorphic<S2>::value &&  std::is_polymorphic<S3>::value,T&>::type { intptr_t vtbl[2] = {vtbl_of(s1),            vtbl_of(s3)}; return get(vtbl); }
    template <typename S1, typename S2, typename S3> inline auto get(const S1* s1, const S2* s2, const S3*   ) -> typename std::enable_if< std::is_polymorphic<S1>::value &&  std::is_polymorphic<S2>::value && !std::is_polymorphic<S3>::value,T&>::type { intptr_t vtbl[2] = {vtbl_of(s1),vtbl_of(s2)            }; return get(vtbl); }
    template <typename S1, typename S2, typename S3> inline auto get(const S1* s1, const S2* s2, const S3* s3) -> typename std::enable_if< std::is_polymorphic<S1>::value &&  std::is_polymorphic<S2>::value &&  std::is_polymorphic<S3>::value,T&>::type { intptr_t vtbl[3] = {vtbl_of(s1),vtbl_of(s2),vtbl_of(s3)}; return get(vtbl); }

  //template <typename S1, typename S2, typename S3, typename S4> inline auto get(const S1* s1, const S2* s2, const S3* s3, const S4* s4) -> typename std::enable_if<!std::is_polymorphic<S1>::value && !std::is_polymorphic<S2>::value && !std::is_polymorphic<S3>::value && !std::is_polymorphic<S4>::value,T&>::type { static T dummy; return dummy; }
    template <typename S1, typename S2, typename S3, typename S4> inline auto get(const S1*   , const S2*   , const S3*   , const S4* s4) -> typename std::enable_if<!std::is_polymorphic<S1>::value && !std::is_polymorphic<S2>::value && !std::is_polymorphic<S3>::value &&  std::is_polymorphic<S4>::value,T&>::type { intptr_t vtbl[1] = {                                    vtbl_of(s4)}; return get(vtbl); }
    template <typename S1, typename S2, typename S3, typename S4> inline auto get(const S1*   , const S2*   , const S3* s3, const S4*   ) -> typename std::enable_if<!std::is_polymorphic<S1>::value && !std::is_polymorphic<S2>::value &&  std::is_polymorphic<S3>::value && !std::is_polymorphic<S4>::value,T&>::type { intptr_t vtbl[1] = {                        vtbl_of(s3)            }; return get(vtbl); }
    template <typename S1, typename S2, typename S3, typename S4> inline auto get(const S1*   , const S2*   , const S3* s3, const S4* s4) -> typename std::enable_if<!std::is_polymorphic<S1>::value && !std::is_polymorphic<S2>::value &&  std::is_polymorphic<S3>::value &&  std::is_polymorphic<S4>::value,T&>::type { intptr_t vtbl[2] = {                        vtbl_of(s3),vtbl_of(s4)}; return get(vtbl); }
    template <typename S1, typename S2, typename S3, typename S4> inline auto get(const S1*   , const S2* s2, const S3*   , const S4*   ) -> typename std::enable_if<!std::is_polymorphic<S1>::value &&  std::is_polymorphic<S2>::value && !std::is_polymorphic<S3>::value && !std::is_polymorphic<S4>::value,T&>::type { intptr_t vtbl[1] = {            vtbl_of(s2)                        }; return get(vtbl); }
    template <typename S1, typename S2, typename S3, typename S4> inline auto get(const S1*   , const S2* s2, const S3*   , const S4* s4) -> typename std::enable_if<!std::is_polymorphic<S1>::value &&  std::is_polymorphic<S2>::value && !std::is_polymorphic<S3>::value &&  std::is_polymorphic<S4>::value,T&>::type { intptr_t vtbl[2] = {            vtbl_of(s2),            vtbl_of(s4)}; return get(vtbl); }
    template <typename S1, typename S2, typename S3, typename S4> inline auto get(const S1*   , const S2* s2, const S3* s3, const S4*   ) -> typename std::enable_if<!std::is_polymorphic<S1>::value &&  std::is_polymorphic<S2>::value &&  std::is_polymorphic<S3>::value && !std::is_polymorphic<S4>::value,T&>::type { intptr_t vtbl[2] = {            vtbl_of(s2),vtbl_of(s3)            }; return get(vtbl); }
    template <typename S1, typename S2, typename S3, typename S4> inline auto get(const S1*   , const S2* s2, const S3* s3, const S4* s4) -> typename std::enable_if<!std::is_polymorphic<S1>::value &&  std::is_polymorphic<S2>::value &&  std::is_polymorphic<S3>::value &&  std::is_polymorphic<S4>::value,T&>::type { intptr_t vtbl[3] = {            vtbl_of(s2),vtbl_of(s3),vtbl_of(s4)}; return get(vtbl); }
    template <typename S1, typename S2, typename S3, typename S4> inline auto get(const S1* s1, const S2*   , const S3*   , const S4*   ) -> typename std::enable_if< std::is_polymorphic<S1>::value && !std::is_polymorphic<S2>::value && !std::is_polymorphic<S3>::value && !std::is_polymorphic<S4>::value,T&>::type { intptr_t vtbl[1] = {vtbl_of(s1)                                    }; return get(vtbl); }
    template <typename S1, typename S2, typename S3, typename S4> inline auto get(const S1* s1, const S2*   , const S3*   , const S4* s4) -> typename std::enable_if< std::is_polymorphic<S1>::value && !std::is_polymorphic<S2>::value && !std::is_polymorphic<S3>::value &&  std::is_polymorphic<S4>::value,T&>::type { intptr_t vtbl[2] = {vtbl_of(s1),                        vtbl_of(s4)}; return get(vtbl); }
    template <typename S1, typename S2, typename S3, typename S4> inline auto get(const S1* s1, const S2*   , const S3* s3, const S4*   ) -> typename std::enable_if< std::is_polymorphic<S1>::value && !std::is_polymorphic<S2>::value &&  std::is_polymorphic<S3>::value && !std::is_polymorphic<S4>::value,T&>::type { intptr_t vtbl[2] = {vtbl_of(s1),            vtbl_of(s3)            }; return get(vtbl); }
    template <typename S1, typename S2, typename S3, typename S4> inline auto get(const S1* s1, const S2*   , const S3* s3, const S4* s4) -> typename std::enable_if< std::is_polymorphic<S1>::value && !std::is_polymorphic<S2>::value &&  std::is_polymorphic<S3>::value &&  std::is_polymorphic<S4>::value,T&>::type { intptr_t vtbl[3] = {vtbl_of(s1),            vtbl_of(s3),vtbl_of(s4)}; return get(vtbl); }
    template <typename S1, typename S2, typename S3, typename S4> inline auto get(const S1* s1, const S2* s2, const S3*   , const S4*   ) -> typename std::enable_if< std::is_polymorphic<S1>::value &&  std::is_polymorphic<S2>::value && !std::is_polymorphic<S3>::value && !std::is_polymorphic<S4>::value,T&>::type { intptr_t vtbl[2] = {vtbl_of(s1),vtbl_of(s2)                        }; return get(vtbl); }
    template <typename S1, typename S2, typename S3, typename S4> inline auto get(const S1* s1, const S2* s2, const S3*   , const S4* s4) -> typename std::enable_if< std::is_polymorphic<S1>::value &&  std::is_polymorphic<S2>::value && !std::is_polymorphic<S3>::value &&  std::is_polymorphic<S4>::value,T&>::type { intptr_t vtbl[3] = {vtbl_of(s1),vtbl_of(s2),            vtbl_of(s4)}; return get(vtbl); }
    template <typename S1, typename S2, typename S3, typename S4> inline auto get(const S1* s1, const S2* s2, const S3* s3, const S4*   ) -> typename std::enable_if< std::is_polymorphic<S1>::value &&  std::is_polymorphic<S2>::value &&  std::is_polymorphic<S3>::value && !std::is_polymorphic<S4>::value,T&>::type { intptr_t vtbl[3] = {vtbl_of(s1),vtbl_of(s2),vtbl_of(s3)            }; return get(vtbl); }
    template <typename S1, typename S2, typename S3, typename S4> inline auto get(const S1* s1, const S2* s2, const S3* s3, const S4* s4) -> typename std::enable_if< std::is_polymorphic<S1>::value &&  std::is_polymorphic<S2>::value &&  std::is_polymorphic<S3>::value &&  std::is_polymorphic<S4>::value,T&>::type { intptr_t vtbl[4] = {vtbl_of(s1),vtbl_of(s2),vtbl_of(s3),vtbl_of(s4)}; return get(vtbl); }

    /// A function that gets called when the cache is either too inefficient or full.
    T& update(const intptr_t (&vtbl)[N]);

#if XTL_DUMP_PERFORMANCE
    std::ostream& operator>>(std::ostream& os) const;
    friend std::ostream& operator<<(std::ostream& os, const vtbl_map& m) { return m >> os; }
#endif

private:

    /// Cached mappings of vtbl to some indecies
    cache_descriptor* descriptor;

    /// A reference to a global variable that will be initialized with the 
    /// number of case clauses of a given match statement
    const vtbl_count_t& case_clauses;

    /// Memoized table.size() during last cache rearranging
    size_t last_table_size;

    /// Number of colisions that we will still tolerate before next update
    int collisions_before_update;

    /// Previous number of colisions that we will still tolerate before next update
    int prev_collisions_before_update;

#if XTL_DUMP_PERFORMANCE
    const char* file;      ///< File in which this vtblmap_of is instantiated
    size_t      line;      ///< Line in the file where it is instantiated
    const char* func;      ///< Function in which this vtblmap_of is instantiated
    size_t      updates;   ///< Amount of reconfigurations performed at run time
    size_t      hits;      ///< The number of cache hits
    size_t      misses;    ///< The number of cache misses
    size_t      collisions;///< Out of all the misses, how many were actual collisions
#endif

};

//------------------------------------------------------------------------------

/// This specialization is used when none of the arguments of Match-statement is polymorphic.
template <typename T>
class vtbl_map<0,T>
{
public:
    vtbl_map(XTL_DUMP_PERFORMANCE_ONLY(const char*, size_t, const char*,) const vtbl_count_t&) {}
    inline T& get(...) noexcept { return dummy; }
    static T dummy; 
};

template <typename T> T vtbl_map<0,T>::dummy;

//------------------------------------------------------------------------------

template <size_t N, typename T>
vtbl_map<N,T>::cache_descriptor::cache_descriptor(
    const size_t       log_size, ///< Parameter k of the cache - the log of the size of the cache
    const bit_offset_t shift     ///< Parameter l of the cache - number of irrelevant bits on the right to remove
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
    // NOTE: We allocate them in the order of LCG traversal to improve
    //       cache locality in the use of cache_entries later.
    for (size_t i = 0, j = 0; i <= cache_mask; ++i)
    {
        cache[j] = &cache_entries[i];
        j++; //j = lcg_next(j);
    }
}

//------------------------------------------------------------------------------

template <size_t N, typename T>
vtbl_map<N,T>::cache_descriptor::cache_descriptor(
    const size_t       log_size,    ///< Parameter k of the cache - the log of the size of the cache                
    const bit_offset_t (&shifts)[N],///< Parameter l of the cache - number of irrelevant bits on the right to remove
#if defined(XTL_NO_RVALREF)
    cache_descriptor&  old          ///< cache_descriptor we will supposedly replace
#else
    cache_descriptor&& old          ///< cache_descriptor we will supposedly replace
#endif
) :
    cache_mask( (1<<log_size) - 1 ),
    used(old.used)
{
    XTL_ASSERT(cache_mask >= old.cache_mask); // Since we are going to inherit all its existing elements

    array_copy(shifts,optimal_shift);         // Initialize optimal shifts

    // We'll be initializing cache pointers out of order for performance reasons
    // so zero them out first to see which ones we have alredy initialized
    for (size_t i = 0; i <= cache_mask; ++i) cache[i] = 0;

    // Initialize first cache pointers to occupied cache entries in the old cache
    for (size_t i = 0; i <= old.cache_mask; ++i)
    {
        XTL_ASSERT(old.cache[i]); // Since we pre-allocate them

        if (old.cache[i]->occupied())
        {
            size_t j = cache_index(old.cache[i]->vtbl);
            while (cache[j]) j = lcg_next(j);
            std::swap(old.cache[i],cache[j]);
        }
    }

    size_t j = 0;

    // Now copy the rest of allocated but vacant entries
    for (size_t i = 0; i <= old.cache_mask; ++i)
    {
        if (old.cache[i])
        {
            XTL_ASSERT(old.cache[i]->vacant());

            // Skip initialized entries if needed
            while (cache[j])
            {
                j++; //j = lcg_next(j);
                XTL_ASSERT(j <= cache_mask);
            }

            std::swap(old.cache[i],cache[j]);
        }
    }

    if (cache_mask - old.cache_mask)
    {
        // Allocate all cache entries in one chunk for better cache performance.
        // Only allocate the difference from need and already present in old ones
        stored_type* cache_entries = new stored_type[cache_mask - old.cache_mask];

        // Initialize remaining pointers from cache to newly allocated cache entries
        // NOTE: We allocate them in the order of LCG traversal to improve
        //       cache locality in the use of cache_entries later.
        for (size_t i = 0; i < cache_mask-old.cache_mask; ++i)
        {
            // Skip initialized entries if needed
            while (cache[j])
            {
                j++; //j = lcg_next(j);
                XTL_ASSERT(j <= cache_mask);
            }

            cache[j] = &cache_entries[i];
        }
    }
}

//------------------------------------------------------------------------------

template <size_t N, typename T>
vtbl_map<N,T>::cache_descriptor::~cache_descriptor()
{
    // The elements will be pointing into separate arrays, we want to
    // find all the beginnings of arrays to deallocate them
    std::sort(&cache[0], &cache[cache_mask+1]);

    for (size_t i = 0; i <= cache_mask;)
    {
        if (cache[i]) // We check because subsequent descriptor could have moved out entries from this one
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

//------------------------------------------------------------------------------

template <size_t N, typename T>
void vtbl_map<N,T>::cache_descriptor::put_entries_in_right_place()
{
    for (size_t i = 0; i <= cache_mask; ++i)
    {
#if XTL_USE_LCG_WALK
        if (cache[i]->occupied()) // There is a valid tuple of vtbl pointers in the entry
        {
            XTL_ASSERT(cache[i]->vtbl[N-1]);        // Either all 0 or all non 0

            size_t q = cache_index(cache[i]->vtbl); // Index of location where it should be (equivalence class)

            if (i == q) break;                      // The entity is in the right place

            size_t j = q;                           // Current position to try placing in

            for (size_t j = q; cache[i]->occupied(); j = lcg_next(j))
                std::swap(cache[i],cache[j]);

            while (cache[j]->occupied())
            {
                size_t k = cache_index(cache[j]->vtbl);

                if (k == j || // the entry is occupied by the right entity
                    k == q)   // or by another entity in the same equivalence class
                    j = lcg_next(j); // get next cache entry to try
                else
                    break;

                if (j == i)   // this is an entry in the equivalence class
                    goto next;// break out of both loops
            }

            // entry in j is neither in its rightful place, not an entry from equivalence class q
            std::swap(cache[i],cache[j]); // swap current to where it should be and continue looking at current
        }
next:   ;
#else
        size_t k = size_t(~0); // Keeps cache index of the previous swap.

        while (cache[i]->occupied()) // There is a valid tuple of vtbl pointers in the entry
        {
            XTL_ASSERT(cache[i]->vtbl[N-1]);        // Either all 0 or all non 0
            size_t j = cache_index(cache[i]->vtbl); // Index of location where it should be

            if (j != k && j != i) // where it should be is not where previous one was swapped to and not here
            {
                std::swap(cache[i],cache[j]); // swap current to where it should be
                k = j;                        // remember where we swapped it to
            }
            else
                break;
        }
#endif
    }
}

//------------------------------------------------------------------------------

template <size_t N, typename T>
auto vtbl_map<N,T>::cache_descriptor::get(const intptr_t (&vtbl)[N], size_t j) noexcept -> stored_type*
{
    XTL_ASSERT(j == cache_index(vtbl)); // j must be index of location where vtbl should be
    stored_type*& ce = cache[j]; // Location where it should be

    XTL_ASSERT(ce);   // Since we pre-allocate all entries

    // When the entry is occupied, our combination can be elsewhere in 
    // the cache due to collisions, however when the entry is not 
    // occupied, we do not have to search whether it is in the cache as
    // constructor establishes invariant that entries can only be in
    // their place.
#if !XTL_USE_LCG_WALK
    if (XTL_LIKELY(ce->occupied()))
#endif
    {
        // Precompute hash of the vtbl array for faster comparisons.
        XTL_VTBL_HASHING(const intptr_t vtbl_hash = get_hash(vtbl);)

#if XTL_USE_LCG_WALK
        // See if (vtbl0,...,vtblN) is elsewhere in the cache
        // Start from j (to account if it is already there) and walk all entries
        // in the LCG order. We are guaranteed to hit each entry only once.
        for (size_t i = 0; i <= cache_mask; ++i)
        {
            if (XTL_UNLIKELY(cache[j]->vacant())) // we found an empty slot
            {
                XTL_ASSERT(cache[j]->vtbl[N-1] == 0); // Either all 0 or all non 0
                *cache[j] = vtbl; //array_copy(vtbl,cache[i]->vtbl);
                ++used;
                std::swap(ce,cache[j]); // swap it with the right position
                return ce;
            }
            else
            if (XTL_UNLIKELY(cache[j]->is_for(vtbl XTL_VTBL_HASHING(,vtbl_hash)))) // if so ...
            {
                std::swap(ce,cache[j]); // swap it with the right position
                return ce;
            }

            j = lcg_next(j);
        }
#else
        // See if (vtbl0,...,vtblN) is elsewhere in the cache
        // Start from j (to account if it is already there) till end
        for (size_t i = j; i <= cache_mask; ++i)
            if (XTL_UNLIKELY(cache[i]->is_for(vtbl XTL_VTBL_HASHING(,vtbl_hash)))) // if so ...
            {
                std::swap(ce,cache[i]); // swap it with the right position
                return ce;
            }
        // If not found, continue from the beginning until j
        for (size_t i = 0; i < j; ++i)
            if (XTL_UNLIKELY(cache[i]->is_for(vtbl XTL_VTBL_HASHING(,vtbl_hash)))) // if so ...
            {
                std::swap(ce,cache[i]); // swap it with the right position
                return ce;
            }
#endif
    }

    // (vtbl0,...,vtblN) is not in the cache
    if (XTL_LIKELY(used <= cache_mask)) // there are empty slots in cache
    {
#if XTL_USE_LCG_WALK
        XTL_ASSERT(!"Not possible to get here with LCG walk");
#else
        // Start from j (since it can be empty) till end
        for (size_t i = j; i <= cache_mask; ++i)
            if (XTL_UNLIKELY(cache[i]->vacant())) // find the first empty slot
            {
                XTL_ASSERT(cache[i]->vtbl[N-1] == 0); // Either all 0 or all non 0
                *cache[i] = vtbl; //array_copy(vtbl,cache[i]->vtbl);
                ++used;
                std::swap(ce,cache[i]); // swap it with the right position
                return ce;
            }
        // If not found, continue from the beginning until j
        for (size_t i = 0; i < j; ++i)
            if (XTL_UNLIKELY(cache[i]->vacant())) // find the first empty slot
            {
                XTL_ASSERT(cache[i]->vtbl[N-1] == 0); // Either all 0 or all non 0
                *cache[i] = vtbl; //array_copy(vtbl,cache[i]->vtbl);
                ++used;
                std::swap(ce,cache[i]); // swap it with the right position
                return ce;
            }
#endif
    }

    // There are no empty slots, we return nullptr to indicate this
    return 0;
}

//------------------------------------------------------------------------------

template <size_t N, typename T>
size_t vtbl_map<N,T>::cache_descriptor::entries_for(const intptr_t (&vtbl)[N], size_t log_size, const bit_offset_t (&offsets)[N]) const
{
    // NOTE: This function should not be inlined because of the use of VLA or 
    //       alloca. Some compilers have been reported to have errors inlining 
    //       such functions in a loop. 
    // NOTE: The use of alloca is discouraged in general! We do this for 
    //       performance to not resort to vectors and heap and keep counting on stack
    // NOTE: For very large #log_size values (e.g. #log_size > #max_stack_log_size+3)
    //       the function does not return the actual number of occupied entires but
    //       only approximation of thereof (at least that value or larger). This is
    //       because we wrap over on the largest bit size of the stack allowed.
    const intptr_t new_cache_mask       = (1<<log_size)-1; // Actual cache mask for the hash function
    const intptr_t max_stack_mask       = (1<<(max_stack_log_size+3))-1; // Mask for the largest number of bits we are allowed to allocate on stack: +3 is *8 for the number of bits in the allowed stack size
    const size_t   cache_histogram_size = 1 + std::min(new_cache_mask,max_stack_mask)/XTL_BIT_SIZE(intptr_t); // Number of elements in intptr_t array allocated on the stack
    XTL_VLAZ(cache_histogram, intptr_t, cache_histogram_size, 1 + max_stack_mask/XTL_BIT_SIZE(intptr_t)); // Declares intptr_t cache_histogram[cache_histogram_size] = {0};
    XTL_BIT_SET(cache_histogram, cache_index(vtbl,offsets,new_cache_mask) & max_stack_mask); // Mark the entry for new vtbl

    // Iterate over vtbl in old cache and see where they are mapped with log size i and offset j
    for (size_t c = 0; c <= this->cache_mask; ++c)
    {
        stored_type* const st = cache[c];

        XTL_ASSERT(st);

        if (st->occupied())
            XTL_BIT_SET(cache_histogram, cache_index(st->vtbl,offsets,new_cache_mask) & max_stack_mask); // Mark the entry for each vtbl
    }

    size_t entries = 0;

    // Count the number of used entries
    for (size_t h = 0; h < cache_histogram_size; ++h)
        entries += bits_set(cache_histogram[h]);

    return entries;
}

//------------------------------------------------------------------------------

template <size_t N, typename T>
T& vtbl_map<N,T>::update(const intptr_t (&vtbl)[N])
{
    XTL_ASSERT(descriptor); // Allocated in constructor, deallocated in destructor
    XTL_ASSERT(last_table_size < descriptor->used || descriptor->is_full()); // We will only call this if size changed

    // FIX: vtbl might already exist in old descriptor and if it happens to be the first one, it won't be taken into consideration
    intptr_t prev[N];
    intptr_t diff[N] = {};

    array_copy(vtbl,prev);

    // Compute bits in which existing vtbl, including the newly added one, differ
    for (size_t i = 0; i <= descriptor->cache_mask; ++i)
    {
        typename cache_descriptor::stored_type* const st = descriptor->cache[i];

        XTL_ASSERT(st);

        for (size_t s = 0; s < N; s++)
            if (intptr_t vt = st->vtbl[s])
            {
                diff[s] |= prev[s] ^ vt;
                prev[s] = vt;
            }
    }

//#if XTL_DUMP_PERFORMANCE
//        std::clog << "\nVtbl:New";
//        vtbl_bin_print(vtbl,std::clog); // Show binary value of vtbl pointer
//        std::clog << " -> " << descriptor->cache_index(vtbl) << '\t';
//        vtbl_class_print(vtbl,std::clog); // Show name of the class of this vtbl
//        std::clog << std::endl;
//        *this >> std::clog;       
//#endif

    XTL_DUMP_PERFORMANCE_ONLY(++updates); // Record update

    bit_offset_t k  = bit_offset_t(req_bits(descriptor->cache_mask));     // current log_size
    bit_offset_t n  = bit_offset_t(req_bits(descriptor->used));           // needed  log_size
    bit_offset_t c  = bit_offset_t(req_bits(case_clauses));               // log_size estimate. NOTE: case_clauses will be initialized by now
    bit_offset_t l1 = std::max(std::max(k,c),n);                          // lower bound for log_size iteration
    bit_offset_t l2 = std::max(std::max(k,c),bit_offset_t(n+max_log_inc));// upper bound for log_size iteration
    bit_offset_t no = l1; // current estimate of the best log_size
    bit_offset_t zo[N];   // current estimate of the best offset
    bit_offset_t m[N];    // highest bit in which vtbls differ
    bit_offset_t z[N];    // lowest bits in which vtbls do not differ

    for (size_t i = 0; i < N; ++i)
    {
        if (diff[i])  // We have to check for non-zero as trailing_zeros will return -127 for 0
        {
            m[i] = bit_offset_t(req_bits(diff[i])); // highest bit in which vtbls differ
            z[i] = bit_offset_t(trailing_zeros(static_cast<unsigned int>(diff[i]))); // lowest bits in which vtbls do not differ. 
        }
        else
            m[i] = z[i] = descriptor->optimal_shift[i];

        zo[i] = z[i]; // current estimate of the best offset
    }

    //std::clog << "Iterate between: ";
    //std::copy(&z[0],&z[N],std::ostream_iterator<bit_offset_t>(std::clog,","));
    //std::clog << " and ";
    //std::copy(&m[0],&m[N],std::ostream_iterator<bit_offset_t>(std::clog,","));
    //std::clog << std::endl;

#if 0
    size_t max_cache_entries = 0;

    // Iterate over allowed log sizes
    for (bit_offset_t i = l1; i <= l2; ++i)
    {
        bit_offset_t mm[N]; // Largest offset to iterate to taking into account the number of useful bits in argument position s.

        for (size_t s = 0; s < N; ++s) 
        {
            bit_offset_t bits_in_arg_mask = (i+N-1-s)/N;
            mm[s] = m[s] > bits_in_arg_mask && m[s] - bits_in_arg_mask >= z[s] ? m[s] - bits_in_arg_mask : m[s];
        }

        size_t q = 0;
        union { bit_offset_t x[N+1]; bit_offset_t j[N]; };

        array_copy(z,j); // Copy lowest offsets to try
        x[N] = 0;        // The extra element of the array must be 0 and will become 1 when we iterated over all offsets

        // Iterate over possible offsets
        do
        {
            size_t entries = descriptor->entries_for(vtbl, i, j); // Count the number of used entries

            //std::clog << "Trying size: " << i << " offset ";
            //std::copy(&x[0],&x[N],std::ostream_iterator<bit_offset_t>(std::clog,","));
            //std::clog << " entries: " << entries << std::endl;

            // Update best estimates
            if (entries > max_cache_entries)
            {
                max_cache_entries = entries;
                no = i;
                array_copy(j,zo);

                if (entries == descriptor->used+1)
                {
                    // We found size and offset without conflicts, exit both loops
                    i = l2+1; // to exit both for loops
                    break;
                }
            }

            // Update current offsets to next ones
            while (x[q] == mm[q])
            {
                x[q] = z[q];
                ++q;
            }

            ++x[q];
            q = 0;
        } while (x[N]==0);
    }

#else

    size_t max_cache_entries = descriptor->entries_for(vtbl, l1, descriptor->optimal_shift);
    array_copy(descriptor->optimal_shift,zo); // Copy current solution as current optimal
    no = l1;

    // Iterate over allowed log sizes
    for (bit_offset_t i = l1; i <= l2; ++i)
    {
//        size_t saved_max_cache_entries = 0;

        // We iterate until we can make improvements to the number of used cache entries
//        while (max_cache_entries > saved_max_cache_entries)
        {
//            saved_max_cache_entries = max_cache_entries;

            // Try to improve independently each argument position
            for (size_t s = 0; s < N; ++s) 
            {
                bit_offset_t bits_in_arg_mask = (i+N-1-s)/N;
                bit_offset_t mm = m[s] > bits_in_arg_mask && m[s] - bits_in_arg_mask >= z[s] ? m[s] - bits_in_arg_mask : m[s];
                bit_offset_t cur = zo[s];

                //XTL_ASSERT(z[s] <= cur && cur <= mm);

                for (bit_offset_t t = z[s]; t <= mm; ++t)
                {
                    if (t != cur)
                    {
                        zo[s] = t;

                        size_t entries = descriptor->entries_for(vtbl, i, zo); // Count the number of used entries

                        // Update best estimates
                        if (entries > max_cache_entries)
                        {
                            max_cache_entries = entries;
                            no  = i;
                            cur = t;

                            if (entries == descriptor->used+1)
                            {
                                // We found size and offset without conflicts, exit both loops
                                i = l2+1; // to exit both for loops
                                zo[s] = cur;
                                goto break_of_both_loops;
                            }
                        }
                    }
                }

                zo[s] = cur;
            } // of loop over argument positions

break_of_both_loops: ;

        } // of while there are improvements
    } // of loop over possible log sizes
#endif
    if (no < k)
        no = k; // We never shrink, while we preallocate based on number of case clauses or the minimum

    if (no != k || !array_equal(descriptor->optimal_shift,zo))
    {
        // OK, either log size or optimal shifts changed. Reset collisions counter to default one
        // Having fixed initial collision count may be counterproductive for small type switches.
        // We thus make this number proportional to the number of case clauses to somewhat estimate
        // after how many collisions an update may be useful.
        prev_collisions_before_update = collisions_before_update = case_clauses ? case_clauses : N*initial_collisions_before_update;

        cache_descriptor* old = descriptor;
        #if defined(DBG_NEW)
            #undef new
        #endif
        #if defined(XTL_NO_RVALREF)
            descriptor = new(no) cache_descriptor(no,zo,*old);
        #else
            descriptor = new(no) cache_descriptor(no,zo,std::move(*old));
        #endif
        #if defined(DBG_NEW)
            #define new DBG_NEW
        #endif
        delete old;
    }
    else
    {
        // Update hasn't changed anything, increase the number of colisions before next update
        prev_collisions_before_update = collisions_before_update = prev_collisions_before_update*2;
    }

//#if XTL_DUMP_PERFORMANCE
//        std::clog << "After" << std::endl;
//        *this >> std::clog;       
//#endif

    // NOTE: Important to call descriptor's get instead of our own in order to 
    //       not get into endless recursion update <-> get!
    typename cache_descriptor::stored_type* res = descriptor->get(vtbl);
    XTL_ASSERT(res && res->is_for(vtbl)); // We have ensured enough space, so no need to check this explicitly
    last_table_size = descriptor->used;   // Update memoized value
    return res->value;
}

//------------------------------------------------------------------------------

#if XTL_DUMP_PERFORMANCE
template <size_t N, typename T>
std::ostream& vtbl_map<N,T>::operator>>(std::ostream& os) const
{
    std::ios::fmtflags fmt = os.flags(); // store flags

    os << file << '[' << line << ']' << ' ' << func << std::endl;

    size_t vtbl_count = descriptor->used;
    size_t log_size   = req_bits(descriptor->cache_mask);
    size_t cache_size = (1<<log_size);

    std::vector<vtbl_count_t> cache_histogram(cache_size);
    std::vector<std::array<intptr_t,N>> vtbls(vtbl_count);
    typename std::vector<std::array<intptr_t,N>>::iterator q = vtbls.begin();

    intptr_t prev[N] = {};
    intptr_t diff[N] = {};

    // Compute bits in which existing vtbl, including the newly added one, differ
    for (size_t i = 0; i <= descriptor->cache_mask; ++i)
    {
        typename cache_descriptor::stored_type* const st = descriptor->cache[i];

        XTL_ASSERT(st);

        if (st->occupied())
        {
            std::array<intptr_t,N> a;

            for (size_t s = 0; s < N; s++)
            {
                intptr_t vtbl = a[s] = st->vtbl[s];

                XTL_ASSERT(vtbl); 

                if (prev[s]) diff[s] |= prev[s] ^ vtbl; // Update diff with information about which bits in all vtbls differ
                prev[s] = vtbl;
            }

            cache_histogram[cache_descriptor::cache_index(to_array(a),descriptor->optimal_shift,descriptor->cache_mask)]++;
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
        vtbl_bin_print(to_array(a),os); // Show binary value of vtbl pointer
        os << " -> " << std::setw(3) << descriptor->cache_index(to_array(a)) << ' '; // Show cache index it is mapped to

        if (cache_histogram[descriptor->cache_index(to_array(a))] > 1)
            os << '[' << cache_histogram[descriptor->cache_index(to_array(a))] << ']'; // Show have many vtbl falls into the same cache index
        else
            os << "   ";

        if (array_equal(to_array(a),prev))
            os << "\t ERROR: Duplicate entry!";

        os << '\t';
        vtbl_class_print(to_array(a),os); // Show name of the class of this vtbl
        os << std::endl;

        XTL_ASSERT(!array_equal(to_array(a),prev)); // There should never be duplicates in the vtbl_map

        array_copy(to_array(a),prev);
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

    os  << "VTBLS:  "     << outstr       << std::endl
        << " clauses="    << std::setw(4) << case_clauses // Number of case clauses in the match statement
        << " total="      << std::setw(5) << vtbl_count   // Total number of vtbl pointers seen
        << " log_size="   << std::setw(2) << log_size     // log2 size required
        << " updates="    << std::setw(2) << updates      // how many updates have been performed on the cache
        << " hits="       << std::setw(8) << hits         // how many hits have we had
        << " misses="     << std::setw(8) << misses       // how many misses have we had
        << " collisions=" << std::setw(8) << collisions   // how many misses were actual collisions
        << " memory="     << std::setw(8) << memory_used()// number of bytes used
        << " Stmt: "      << file << '[' << line << ']' << ' ' << func
        << ";\n";
    os.flags(fmt);
#if 0
    bit_offset_t k  = req_bits(cache_mask);     // current log_size
    bit_offset_t n  = req_bits(vtbl_count-1);   // needed  log_size
    bit_offset_t m  = req_bits(diff);           // highest bit in which vtbls differ
    bit_offset_t z  = trailing_zeros(static_cast<unsigned int>(diff)); // number of lowest bits in which vtbls do not differ
    bit_offset_t l1 = std::min(k,n);
    bit_offset_t l2 = std::max(k,bit_offset_t(n+max_log_inc));

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
        << " conflict="    << std::setw(9) << std::fixed << std::setprecision(7) << cache_conflict // Probability of conflict in cache
        << " Stmt: "       << file << '[' << line << ']' << ' ' << func
        << "; ";

    size_t cache_last_non_zero_count = last_non_zero_count(&cache_histogram[0],cache_size,vtbl_count);

    // Print cache histogram
    for (size_t i = 0; i <= cache_last_non_zero_count; ++i)
    {
        size_t d = std::count(cache_histogram.begin(),cache_histogram.end(),i);

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
        case 0:  c = descriptor->cache[j]->occupied() ? '*' : '.'; break;
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
template <size_t N>
struct type_switch_info
{
    std::ptrdiff_t offset[N]; ///< Required this-pointer offset to the source sub-object
    std::size_t    target;    ///< Case label of the jump target of Match statement
};

template <>
struct type_switch_info<0>
{
    std::size_t    target;    ///< Case label of the jump target of Match statement
    std::ptrdiff_t offset[XTL_VARIABLE_SIZE_ARRAY]; ///< Dummy array, not used. Ideally should be 0 size
};

//------------------------------------------------------------------------------

} // of namespace mch

// Generic M and V without vtbl array hashing are:
// N=1:  100 combinations V=50 M=53  -   5% slower
// N=2:  100 combinations V=55 M=38  -  43% faster
// N=3: 1000 combinations V=68 M=69  -   2% slower

// Generic M and V with vtbl array hashing are:
// N=1:  100 combinations V=56 M=53  -   4% faster
// N=2:  100 combinations V=52 M=36  -  42% faster
// N=3: 1000 combinations V=67 M=63  -   7% faster
// Hashing thus improves performance for N >= 3

// Specialized M and V without vtbl array hashing are:
// N=1:  100 combinations V=51 M=54  -   6% slower
// N=2:  100 combinations V=58 M=35  -  62% faster
// N=3: 1000 combinations V=63 M=70  -  10% slower

//#include "vtblmap4st-1.hpp" // Implementation of vtbl_map<1,T> partial specialization
//#include "vtblmap4st-2.hpp" // Implementation of vtbl_map<2,T> partial specialization
//#include "vtblmap4st-3.hpp" // Implementation of vtbl_map<3,T> partial specialization

//------------------------------------------------------------------------------
#undef if
