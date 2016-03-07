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
/// \see https://parasol.tamu.edu/xtl/
/// \see https://parasol.tamu.edu/mach7/
/// \see https://github.com/solodon4/Mach7
/// \see https://github.com/solodon4/SELL
///

#pragma once

#include <xtl/xtl.hpp>                     // XTL subtyping definitions
#include <xtl/adapters/std/memory.hpp>     // XTL subtyping adapters for standard smart pointers
#include <assert.h>

//----------------------------------------------------------------------------------------------------------------------

int main()
{
          std::nullptr_t mn = nullptr;
    const std::nullptr_t cn = nullptr;

    static_assert( xtl::is_subtype<      std::nullptr_t,       int*>::value, "Must be a subtype");
    static_assert( xtl::is_subtype<      std::nullptr_t, const int*>::value, "Must be a subtype");
    static_assert( xtl::is_subtype<const std::nullptr_t, const int*>::value, "Must be a subtype");
    static_assert( xtl::is_subtype<const std::nullptr_t,       int*>::value, "Must be a subtype");

          int* pmi = nullptr;
    const int* pci = nullptr;

    pmi = xtl::subtype_cast<      int*>(mn); assert(pmi == nullptr);
    pci = xtl::subtype_cast<const int*>(mn); assert(pci == nullptr);
    pmi = xtl::subtype_cast<      int*>(cn); assert(pmi == nullptr);
    pci = xtl::subtype_cast<const int*>(cn); assert(pci == nullptr);

    static_assert( xtl::is_subtype<      std::nullptr_t,       void*>::value, "Must be a subtype");
    static_assert( xtl::is_subtype<      std::nullptr_t, const void*>::value, "Must be a subtype");
    static_assert( xtl::is_subtype<const std::nullptr_t, const void*>::value, "Must be a subtype");
    static_assert( xtl::is_subtype<const std::nullptr_t,       void*>::value, "Must be a subtype");

          void* pmv = nullptr;
    const void* pcv = nullptr;

    pmv = xtl::subtype_cast<      void*>(mn); assert(pmv == nullptr);
    pcv = xtl::subtype_cast<const void*>(mn); assert(pcv == nullptr);
    pmv = xtl::subtype_cast<      void*>(cn); assert(pmv == nullptr);
    pcv = xtl::subtype_cast<const void*>(cn); assert(pcv == nullptr);

    static_assert( xtl::is_subtype<      std::nullptr_t, std::unique_ptr<      int>>::value, "Must be a subtype");
    static_assert( xtl::is_subtype<      std::nullptr_t, std::unique_ptr<const int>>::value, "Must be a subtype");
    static_assert( xtl::is_subtype<const std::nullptr_t, std::unique_ptr<const int>>::value, "Must be a subtype");
    static_assert( xtl::is_subtype<const std::nullptr_t, std::unique_ptr<      int>>::value, "Must be a subtype");

    std::unique_ptr<      int> upmi = nullptr;
    std::unique_ptr<const int> upci = nullptr;

    upmi = xtl::subtype_cast<std::unique_ptr<      int>>(mn); assert(upmi == nullptr);
    upci = xtl::subtype_cast<std::unique_ptr<const int>>(mn); assert(upci == nullptr);
    upmi = xtl::subtype_cast<std::unique_ptr<      int>>(cn); assert(upmi == nullptr);
    upci = xtl::subtype_cast<std::unique_ptr<const int>>(cn); assert(upci == nullptr);

    static_assert( xtl::is_subtype<      std::nullptr_t, std::unique_ptr<      void>>::value, "Must be a subtype");
    static_assert( xtl::is_subtype<      std::nullptr_t, std::unique_ptr<const void>>::value, "Must be a subtype");
    static_assert( xtl::is_subtype<const std::nullptr_t, std::unique_ptr<const void>>::value, "Must be a subtype");
    static_assert( xtl::is_subtype<const std::nullptr_t, std::unique_ptr<      void>>::value, "Must be a subtype");

  //std::unique_ptr<      void> upmv = nullptr;
  //std::unique_ptr<const void> upcv = nullptr; // Check if this is valid

  //upmv = xtl::subtype_cast<std::unique_ptr<      void>>(mn); assert(upmv == nullptr);
  //upcv = xtl::subtype_cast<std::unique_ptr<const void>>(mn); assert(upcv == nullptr);
  //upmv = xtl::subtype_cast<std::unique_ptr<      void>>(cn); assert(upmv == nullptr);
  //upcv = xtl::subtype_cast<std::unique_ptr<const void>>(cn); assert(upcv == nullptr);

    static_assert( xtl::is_subtype<std::unique_ptr<      int>,       int*>::value, "Must be a subtype");
    static_assert( xtl::is_subtype<std::unique_ptr<      int>, const int*>::value, "Must be a subtype");
    static_assert( xtl::is_subtype<std::unique_ptr<const int>, const int*>::value, "Must be a subtype");
    static_assert(!xtl::is_subtype<std::unique_ptr<const int>,       int*>::value, "Must not be a subtype");

    upmi = std::unique_ptr<      int>(new int(4));
    upci = std::unique_ptr<const int>(new int(7));

    pmi = xtl::subtype_cast<      int*>(upmi); assert(pmi != nullptr && *pmi == 4);
  //pmi = xtl::subtype_cast<      int*>(upci); // This should fail to compile
    pci = xtl::subtype_cast<const int*>(upmi); assert(pci != nullptr && *pci == 4);
    pci = xtl::subtype_cast<const int*>(upci); assert(pci != nullptr && *pci == 4);

    static_assert( xtl::is_subtype<std::unique_ptr<      int>,       void*>::value, "Must be a subtype");
    static_assert( xtl::is_subtype<std::unique_ptr<      int>, const void*>::value, "Must be a subtype");
    static_assert( xtl::is_subtype<std::unique_ptr<const int>, const void*>::value, "Must be a subtype");
    static_assert(!xtl::is_subtype<std::unique_ptr<const int>,       void*>::value, "Must not be a subtype");

    pmv = xtl::subtype_cast<      void*>(upmi); assert(pmv != nullptr && *(int*)pmi == 4);
  //pmv = xtl::subtype_cast<      void*>(upci); // This should fail to compile
    pcv = xtl::subtype_cast<const void*>(upmi); assert(pcv != nullptr && *(const int*)pci == 4);
    pcv = xtl::subtype_cast<const void*>(upci); assert(pcv != nullptr && *(const int*)pci == 4);
}
