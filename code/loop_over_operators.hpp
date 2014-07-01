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
/// This file helps generating repetetive textual patterns for all operators defined in exprtmpl.hpp
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// \see https://parasol.tamu.edu/mach7/
/// \see https://github.com/solodon4/Mach7
/// \see https://github.com/solodon4/SELL
///
///
/// \file loop_over_operators.hpp
///
/// This file helps generating repetetive textual patterns for all operators defined in exprtmpl.hpp
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of Mach7 library (http://parasol.tamu.edu/mach7/).
/// Copyright (C) 2011-2012 Texas A&M University.
/// All rights reserved.
///

///
/// \note Inclusion guards are not provided on purpose cause this file may be
///       included several times.
///

#if !defined (FOR_EACH_UNARY_OPERATOR) && !defined (FOR_EACH_BINARY_OPERATOR)
#error You have to define macro FOR_EACH_UNARY_OPERATOR(F,S) or FOR_EACH_BINARY_OPERATOR(F,S) before including this header
#endif

#if !defined(FOR_EACH_UNARY_OPERATOR)
#define FOR_EACH_UNARY_OPERATOR(F,S)
#define FOR_EACH_UNARY_OPERATOR_DEFINED
#endif

#if !defined(FOR_EACH_BINARY_OPERATOR)
#define FOR_EACH_BINARY_OPERATOR(F,S)
#define FOR_EACH_BINARY_OPERATOR_DEFINED
#endif

FOR_EACH_UNARY_OPERATOR(mch::unary_minus      ,- )
FOR_EACH_UNARY_OPERATOR(mch::bit_complement   ,~ )
FOR_EACH_UNARY_OPERATOR(mch::bool_complement  ,! )

FOR_EACH_BINARY_OPERATOR(mch::addition        ,+ )
FOR_EACH_BINARY_OPERATOR(mch::subtraction     ,- )
FOR_EACH_BINARY_OPERATOR(mch::multiplication  ,* )
FOR_EACH_BINARY_OPERATOR(mch::division        ,/ )
FOR_EACH_BINARY_OPERATOR(mch::modulo          ,% )
FOR_EACH_BINARY_OPERATOR(mch::bit_and         ,& )
FOR_EACH_BINARY_OPERATOR(mch::bit_or          ,| )
FOR_EACH_BINARY_OPERATOR(mch::bit_xor         ,^ )
FOR_EACH_BINARY_OPERATOR(mch::bit_shift_left  ,<<)
FOR_EACH_BINARY_OPERATOR(mch::bit_shift_right ,>>)
FOR_EACH_BINARY_OPERATOR(mch::bool_and        ,&&)
FOR_EACH_BINARY_OPERATOR(mch::bool_or         ,||)
FOR_EACH_BINARY_OPERATOR(mch::equal           ,==)
FOR_EACH_BINARY_OPERATOR(mch::not_equal       ,!=)
FOR_EACH_BINARY_OPERATOR(mch::greater         ,> )
FOR_EACH_BINARY_OPERATOR(mch::greater_equal   ,>=)
FOR_EACH_BINARY_OPERATOR(mch::less            ,< )
FOR_EACH_BINARY_OPERATOR(mch::less_equal      ,<=)

#if defined(FOR_EACH_UNARY_OPERATOR_DEFINED)
#undef FOR_EACH_UNARY_OPERATOR
#undef FOR_EACH_UNARY_OPERATOR_DEFINED
#endif

#if defined(FOR_EACH_BINARY_OPERATOR_DEFINED)
#undef FOR_EACH_BINARY_OPERATOR
#undef FOR_EACH_BINARY_OPERATOR_DEFINED
#endif
