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
