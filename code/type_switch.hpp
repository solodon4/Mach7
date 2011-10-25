///
/// \file type_switch.hpp
///
/// A stripped down version of match.hpp that can be used for type switching,
/// without support for binding or other pattern matching. 
///
/// \note Unlike match.hpp, use of this file does not require member matching
/// definitions or any other definitions.
///
/// \autor Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of the XTL framework (http://parasol.tamu.edu/xtl/).
/// Copyright (C) 2005-2011 Texas A&M University.
/// All rights reserved.
///

#pragma once

#include "vtblmap.hpp"

//------------------------------------------------------------------------------

/// Macro that starts the switch on dynamic type of a variable s that can be 
/// either pointer or reference to a polymorphic type.
#define TypeMatch(s) {\
        static vtblmap<type_switch_info&> __vtbl2lines_map XTL_DUMP_PERFORMANCE_ONLY((__FILE__,__LINE__));\
        auto const   __selector_ptr = addr(s);\
        XTL_ASSERT(("Trying to match against a nullptr",__selector_ptr));\
        const void*  __casted_ptr;\
        type_switch_info& __switch_info = __vtbl2lines_map.get(__selector_ptr);\
        switch (__switch_info.line) {\
        case 0: {
/// Extended version of the above macro that takes an expected number of cases in
/// to estimate the size of the cache needed instead of using the default size
#define TypeMatchN(s,N) {\
        static vtblmap<type_switch_info&/*,requires_bits<N>::value*/> __vtbl2lines_map(XTL_DUMP_PERFORMANCE_ONLY(__FILE__,__LINE__,)requires_bits<N>::value);\
        auto const   __selector_ptr = addr(s);\
        const void*  __casted_ptr;\
        type_switch_info& __switch_info = __vtbl2lines_map.get(__selector_ptr);\
        switch (__switch_info.line) {\
        case 0: {

/// Macro that defines the case statement for the above switch
/// NOTE: If Visual C++ gives you error C2051: case expression not constant
///       on this TypeCase label, just change the Debug Format in project setting 
///       Project -> Properties -> C/C++ -> General -> Debug Information Format 
///       from "Program Database for Edit And Continue (/ZI)" 
///       to   "Program Database (/Zi)", which is the default in Release builds,
///       but not in Debug. This is a known bug of Visual C++ described here:
///       http://connect.microsoft.com/VisualStudio/feedback/details/375836/-line-not-seen-as-compile-time-constant
#define TypeCase(C)   } if (XTL_UNLIKELY(__casted_ptr = dynamic_cast<const C*>(__selector_ptr))) { if (__switch_info.line == 0) { __switch_info.line = __LINE__; __switch_info.offset = intptr_t(__casted_ptr)-intptr_t(__selector_ptr); } case __LINE__: auto matched = adjust_ptr<C>(__selector_ptr,__switch_info.offset);
#define EndTypeMatch } if (__switch_info.line == 0) { __switch_info.line = __LINE__; } case __LINE__: ; }}

//------------------------------------------------------------------------------

