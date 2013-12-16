///
/// \file
///
/// This file is a part of C++ pattern matching library.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of Mach7 library (http://parasol.tamu.edu/mach7/).
/// Copyright (C) 2011-2012 Texas A&M University.
/// All rights reserved.
///

#include "config.hpp"

#if XTL_MULTI_THREADING
#include "vtblmap3mt.hpp"  // Multi-threaded version of vtblmap based on atomics and lock-free programming
#else
#include "vtblmap3st.hpp"  // Single-threaded version that does not use unordered_map (hash table)
//#include "vtblmap3.hpp"    // Single-threaded version that uses unordered_map (hash table) - original performance testing
#endif
