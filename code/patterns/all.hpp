///
/// \file all.hpp
///
/// Convenience header that includes all the patterns supported by the library.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of Mach7 library (http://parasol.tamu.edu/mach7/).
/// Copyright (C) 2011-2012 Texas A&M University.
/// All rights reserved.
///

#pragma once

#include "address.hpp"        // Address pattern
#if !defined(_MSC_VER)
#include "any.hpp"            // Any (one-of) pattern
#endif
#include "combinators.hpp"    // Pattern combinators
#include "constructor.hpp"    // Constructor pattern
#include "equivalence.hpp"    // Equivalence pattern
#include "guard.hpp"          // Guard pattern
#include "n+k.hpp"            // n+k pattern
#include "predicate.hpp"      // Predicate patterns
#include "primitive.hpp"      // Value, Variable and Wildcard patterns
#include "quantifiers.hpp"    // Quantifiers
#include "regex.hpp"          // Regular expression pattern
