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
/// This file defines class vtbl_map<2,T> used for fast mapping of vtbl pointers
/// to type T.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// \see https://parasol.tamu.edu/mach7/
/// \see https://github.com/solodon4/Mach7
/// \see https://github.com/solodon4/SELL
///

#pragma once

namespace mch ///< Mach7 library namespace
{

//------------------------------------------------------------------------------

/// Class for efficient mapping of vtbl-pointers to a value of type T.
/// This version of the class is for use in the single-threaded environment. 
/// The data structure is implemented in a lock-free manner.
///
/// The map can only grow in size - it does not provide any means to shrink or 
/// reallocate the contained data. The reason is that all the applications that
/// use the vtbl_map so far rely on the reference to an element associated with 
/// given vtbl-pointer to not change throughout the lifetime of application. 
template <typename T>
class vtbl_map<2,T>
{
private:

    static const size_t N = 2; ///< The number of arguments this implementation handles

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
        #if defined(XTL_NO_RVALREF)
            cache_descriptor&  old       ///< cache_descriptor we will supposedly replace
        #else
            cache_descriptor&& old       ///< cache_descriptor we will supposedly replace
        #endif
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

            // Bring elements to their positions in cache
            // With this:    OVERALL: Random: 136% slower V= 66 M=157
            // Without this: OVERALL: Random: 290% slower V= 67 M=262
            put_entries_in_right_place(); // Re-establish invariant
        }

        /// Deallocates memory used for elements.
        ~cache_descriptor()
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

        cache_descriptor& operator=(const cache_descriptor&); ///< No assignment

        bool is_full() const { return used > cache_mask; } ///< Checks whether cache is full
        size_t  size() const { return cache_mask+1; }      ///< Number of entries in cache

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
        void put_entries_in_right_place()
        {
            for (size_t i = 0; i <= cache_mask; ++i)
            {
                size_t k = ~0; // Keeps cache index of the previous swap.

                while (cache[i]->vtbl[0]) // There is a valid tuple of vtbl pointers in the entry
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
            }
        }

        /// Main function that will be used to get a reference to the stored element. 
        inline stored_type* get(const intptr_t (&vtbl)[N]) noexcept
        {
            size_t j = cache_index(vtbl);
            return get(vtbl,j);
        }
        /// Optimization for when the cache index has already been computed
        inline stored_type* get(const intptr_t (&vtbl)[N], size_t j) noexcept
        {
            XTL_ASSERT(vtbl[0]); // Must be a valid vtbl pointer
            XTL_ASSERT(vtbl[1]); // Must be a valid vtbl pointer
            XTL_ASSERT(j == cache_index(vtbl)); // j must be index of location where vtbl should be
            stored_type*& ce = cache[j]; // Location where it should be

            XTL_ASSERT(ce);   // Since we pre-allocate all entries

            if (XTL_UNLIKELY(ce->vtbl[0] != vtbl[0] || 
                             ce->vtbl[1] != vtbl[1]))
            {
                // NOTE: We don't check if the entry is occupied as even when 
                //       it is not, the vtbl may be elsewhere in the cache due 
                //       to changes to k and l after update.
                stored_type** cv;

                // See if (vtbl0,vtbl1) is elsewhere in the cache
                for (size_t i = 0; i <= cache_mask; ++i)
                    if (cache[i]->vtbl[0] == vtbl[0] && 
                        cache[i]->vtbl[1] == vtbl[1]) // if so ...
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
                            cache[i]->vtbl[0] = vtbl[0]; // assign vtbl to it
                            cache[i]->vtbl[1] = vtbl[1]; // assign vtbl to it
                            ++used;
                            cv = &cache[i]; // swap it with the right position
                            goto Swap;
                        }

                // There are no empty slots, we return nullptr to indicate this.
                return 0;
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
    vtbl_map(const char* fl, size_t ln, const char* fn, const vtbl_count_t& num_clauses) : 
        descriptor(new(min_log_size) cache_descriptor(min_log_size)),
        case_clauses(num_clauses),
        last_table_size(0),
        collisions_before_update(initial_collisions_before_update),
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
        collisions_before_update(initial_collisions_before_update)
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
    inline T& get(const intptr_t vtbl0, const intptr_t vtbl1) noexcept
    {
        intptr_t vtbl[N] = {vtbl0,vtbl1};
        return get(vtbl);
    }

    inline T& get(const intptr_t (&vtbl)[N]) noexcept
    {
        XTL_ASSERT(descriptor); // Allocated in constructor, deallocated in destructor
        XTL_ASSERT(vtbl[0]); // Since this represents VTBL pointer it cannot be null
        XTL_ASSERT(vtbl[1]); // Since this represents VTBL pointer it cannot be null

        size_t j = descriptor->cache_index(vtbl);  // Index of location where it should be
        typename cache_descriptor::stored_type*& ce = descriptor->cache[j]; // Location where it should be

        XTL_ASSERT(ce);   // Since we pre-allocate all entries

        if (XTL_UNLIKELY(ce->vtbl[0] != vtbl[0] || 
                         ce->vtbl[1] != vtbl[1]))
        {
            XTL_DUMP_PERFORMANCE_ONLY(++misses);
            XTL_DUMP_PERFORMANCE_ONLY(if (ce->vtbl[0]) ++collisions);

            if (descriptor->is_full()                     // No entries left for possibly new vtbl in the cache
                || (ce->vtbl[0]                           // Collision - the entry for vtbl is already occupied
                && --collisions_before_update <= 0        // We had sufficiently many collisions to justify call
                && descriptor->used != last_table_size))  // There was at least one vtbl added since last update
                return update(vtbl);                      // try to rearrange cache

            // Try to find entry with our vtbl and swap it with where it is expected to be
            descriptor->get(vtbl,j); // This will bring correct pointer into ce
            XTL_ASSERT(ce->vtbl[0] == vtbl[0]);
            XTL_ASSERT(ce->vtbl[1] == vtbl[1]);
        }
        XTL_DUMP_PERFORMANCE_ONLY(else ++hits);

        return ce->value;
    }

    /// A function that gets called when the cache is either too inefficient or full.
    T& update(const intptr_t (&vtbl)[N]);

#if XTL_DUMP_PERFORMANCE
    std::ostream& operator>>(std::ostream& os) const;
    friend std::ostream& operator<<(std::ostream& os, const vtbl_map& m) { return m >> os; }
#endif

private:

    /// A reference to a global variable that will be initialized with the number of case clauses of a given match statement
    const vtbl_count_t& case_clauses;

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
    size_t      hits;      ///< The number of cache hits
    size_t      misses;    ///< The number of cache misses
    size_t      collisions;///< Out of all the misses, how many were actual collisions
#endif

};

//------------------------------------------------------------------------------

template <typename T>
T& vtbl_map<2,T>::update(const intptr_t (&vtbl)[N])
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
    bit_offset_t n  = bit_offset_t(req_bits(std::max<size_t>(descriptor->used,case_clauses))); // needed  log_size. NOTE: case_clauses will be initialized by now
    bit_offset_t m[N] = { bit_offset_t(req_bits(diff[0])), 
                          bit_offset_t(req_bits(diff[1])) 
                        }; // highest bit in which vtbls differ
    bit_offset_t z[N] = { bit_offset_t(trailing_zeros(static_cast<unsigned int>(diff[0]))), 
                          bit_offset_t(trailing_zeros(static_cast<unsigned int>(diff[1])))
                        }; // lowest bits in which vtbls do not differ
    bit_offset_t l1 = std::min(max_log_size,std::max(k,n));                          // lower bound for log_size iteration
    bit_offset_t l2 = std::min(max_log_size,std::max(k,bit_offset_t(n+max_log_inc)));// upper bound for log_size iteration
    bit_offset_t no = l1;                                             // current estimate of the best log_size
    bit_offset_t zo[N] = {z[0],z[1]};                                 // current estimate of the best offset

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
        for (bit_offset_t j0 = z[0]; j0 <= m[0]; ++j0)
        for (bit_offset_t j1 = z[1]; j1 <= m[1]; ++j1)
        {
            std::memset(cache_histogram,0,cache_histogram_size*sizeof(intptr_t)); // Reset bit histogram to zeros
            XTL_BIT_SET(cache_histogram, interleave(vtbl[0] >> j0, vtbl[1] >> j1) & cache_mask); // Mark the entry for new vtbl

            // Iterate over vtbl in old cache and see where they are mapped with log size i and offset j
            for (size_t c = 0; c <= descriptor->cache_mask; ++c)
            {
                typename cache_descriptor::stored_type* const st = descriptor->cache[c];

                XTL_ASSERT(st);

                if (intptr_t vtbl0 = st->vtbl[0])
                {
                    intptr_t vtbl1 = st->vtbl[1];
                    XTL_ASSERT(vtbl1);
                    XTL_BIT_SET(cache_histogram, interleave(vtbl0 >> j0, vtbl1 >> j1) & cache_mask); // Mark the entry for each vtbl
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

    if (no < k)
        no = k; // We never shrink, while we preallocate based on number of case clauses or the minimum

    if (no != k ||
        descriptor->optimal_shift[0] != zo[0] || 
        descriptor->optimal_shift[1] != zo[1]
       )
    {
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

//#if XTL_DUMP_PERFORMANCE
//        std::clog << "After" << std::endl;
//        *this >> std::clog;       
//#endif
    typename cache_descriptor::stored_type* res = descriptor->get(vtbl);
    XTL_ASSERT(res && res->vtbl[0] == vtbl[0] && res->vtbl[1] == vtbl[1]); // We have ensured enough space, so no need to check this explicitly
    last_table_size = descriptor->used;   // Update memoized value
    return res->value;
}

//------------------------------------------------------------------------------

#if XTL_DUMP_PERFORMANCE
template <typename T>
std::ostream& vtbl_map<2,T>::operator>>(std::ostream& os) const
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

            cache_histogram[interleave(a[0] >> descriptor->optimal_shift[0], a[1] >> descriptor->optimal_shift[1]) & descriptor->cache_mask]++;
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

        os << " -> " << std::setw(3) << (interleave(a[0] >> descriptor->optimal_shift[0], a[1] >> descriptor->optimal_shift[1]) & descriptor->cache_mask)    // Show cache index it is mapped to
           << ' ';
            
        std::copy(a.begin(),a.end(),&prev[0]);

        if (cache_histogram[interleave(a[0] >> descriptor->optimal_shift[0], a[1] >> descriptor->optimal_shift[1]) & descriptor->cache_mask] > 1)
            os << '[' << cache_histogram[interleave(a[0] >> descriptor->optimal_shift[0], a[1] >> descriptor->optimal_shift[1]) & descriptor->cache_mask] << ']'; // Show have many vtbl falls into the same cache index
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
        case 0:  c = descriptor->cache[j]->vtbl[0] ? '*' : '.'; break;
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

} // of namespace mch
