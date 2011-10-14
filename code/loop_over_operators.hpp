///
/// \file loop_over_operators.hpp
///
/// This file helps generating repetetive textual patterns for all operators defined in exprtmpl.hpp
///
/// \author Yuriy Solodkyy
///
/// \note This file is a part of XTL libryary http://parasol.tamu.edu/xtl/
///
/// Copyright (C) 2011, Texas A&M University.  All rights reserved.
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

FOR_EACH_UNARY_OPERATOR(negation         ,- )
FOR_EACH_UNARY_OPERATOR(bit_complement   ,~ )
FOR_EACH_UNARY_OPERATOR(bool_complement  ,! )

FOR_EACH_BINARY_OPERATOR(addition        ,+ )
FOR_EACH_BINARY_OPERATOR(subtraction     ,- )
FOR_EACH_BINARY_OPERATOR(multiplication  ,* )
FOR_EACH_BINARY_OPERATOR(division        ,/ )
FOR_EACH_BINARY_OPERATOR(modulo          ,% )
FOR_EACH_BINARY_OPERATOR(bit_and         ,& )
FOR_EACH_BINARY_OPERATOR(bit_or          ,| )
FOR_EACH_BINARY_OPERATOR(bit_xor         ,^ )
FOR_EACH_BINARY_OPERATOR(bit_shift_left  ,<<)
FOR_EACH_BINARY_OPERATOR(bit_shift_right ,>>)
FOR_EACH_BINARY_OPERATOR(bool_and        ,&&)
FOR_EACH_BINARY_OPERATOR(bool_or         ,||)
FOR_EACH_BINARY_OPERATOR(equal           ,==)
FOR_EACH_BINARY_OPERATOR(not_equal       ,!=)
FOR_EACH_BINARY_OPERATOR(greater         ,> )
FOR_EACH_BINARY_OPERATOR(greater_equal   ,>=)
FOR_EACH_BINARY_OPERATOR(less            ,< )
FOR_EACH_BINARY_OPERATOR(less_equal      ,<=)

#if defined(FOR_EACH_UNARY_OPERATOR_DEFINED)
#undef FOR_EACH_UNARY_OPERATOR
#undef FOR_EACH_UNARY_OPERATOR_DEFINED
#endif

#if defined(FOR_EACH_BINARY_OPERATOR_DEFINED)
#undef FOR_EACH_BINARY_OPERATOR
#undef FOR_EACH_BINARY_OPERATOR_DEFINED
#endif
