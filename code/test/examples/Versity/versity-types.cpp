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
/// This file is a part of Mach7 library test suite.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// \see https://parasol.tamu.edu/mach7/
/// \see https://github.com/solodon4/Mach7
/// \see https://github.com/solodon4/SELL
///

#include "versity-bindings.hpp"
#include <mach7/type_switchN-patterns.hpp> // Support for N-ary Match statement on patterns
#include <mach7/patterns/address.hpp>      // Address and dereference combinators
#include <mach7/patterns/constructor.hpp>  // Support for constructor patterns
#include <mach7/patterns/equivalence.hpp>  // Equivalence combinator +
#include <mach7/patterns/guard.hpp>        // Support for guard patterns
#include <mach7/patterns/n+k.hpp>          // Generalized n+k patterns

//------------------------------------------------------------------------------

//using mch::C; // Enable use of constructor pattern without namespace qualification
using namespace mch;

//------------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& os, const TyPrim& t)
{
    int n;

    Match(t)
    {
    Case(C<TyChar   >( )) os << "TyChar"         ; break;
    Case(C<TyUChar  >( )) os << "TyUChar"        ; break;
    Case(C<TyShort  >( )) os << "TyShort"        ; break;
    Case(C<TyUShort >( )) os << "TyUShort"       ; break;
    Case(C<TyInt    >( )) os << "TyInt"          ; break;
    Case(C<TyUInt   >( )) os << "TyUInt"         ; break;
    Case(C<TyLong   >( )) os << "TyLong"         ; break;
    Case(C<TyULong  >( )) os << "TyULong"        ; break;
    Case(C<TyLLong  >( )) os << "TyLLong"        ; break;
    Case(C<TyULLong >( )) os << "TyULLong"       ; break;
    Case(C<TyFloat  >( )) os << "TyFloat"        ; break;
    Case(C<TyDouble >( )) os << "TyDouble"       ; break;
    Case(C<TyCharN  >(n)) os << "TyCharN"    << n; break;
    Case(C<TyUCharN >(n)) os << "TyUCharN"   << n; break;
    Case(C<TyShortN >(n)) os << "TyShortN"   << n; break;
    Case(C<TyUShortN>(n)) os << "TyUShortN"  << n; break;
    Case(C<TyIntN   >(n)) os << "TyIntN"     << n; break;
    Case(C<TyUIntN  >(n)) os << "TyUIntN"    << n; break;
    Case(C<TyLongN  >(n)) os << "TyLongN"    << n; break;
    Case(C<TyULongN >(n)) os << "TyULongN"   << n; break;
    Case(C<TyLLongN >(n)) os << "TyLLongN"   << n; break;
    Case(C<TyULLongN>(n)) os << "TyULLongN"  << n; break;
    Case(C<TyFloatN >(n)) os << "TyFloatN"   << n; break;
    Case(C<TyDoubleN>(n)) os << "TyDoubleN"  << n; break;
    }
    EndMatch

    return os;
}

//------------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& os, const Val& v)
{
    unsigned long long u;
             long long n;
                double d;

    Match(v)
    {
    Case(C<ValDouble >(d)) os << "(ValDouble  " << d << ')'; break;
    Case(C<ValFloat  >(d)) os << "(ValFloat   " << d << ')'; break;
    Case(C<ValChar   >(n)) os << "(ValChar    " << n << ')'; break;
    Case(C<ValUChar  >(u)) os << "(ValUChar   " << u << ')'; break;
    Case(C<ValShort  >(n)) os << "(ValShort   " << n << ')'; break;
    Case(C<ValUShort >(u)) os << "(ValUShort  " << u << ')'; break;
    Case(C<ValInt    >(n)) os << "(ValInt     " << n << ')'; break;
    Case(C<ValUInt   >(u)) os << "(ValUInt    " << u << ')'; break;
    Case(C<ValLong   >(n)) os << "(ValLong    " << n << ')'; break;
    Case(C<ValULong  >(u)) os << "(ValULong   " << u << ')'; break;
    Case(C<ValLLong  >(n)) os << "(ValLLong   " << n << ')'; break;
    Case(C<ValULLong >(u)) os << "(ValULLong  " << u << ')'; break;
    Case(C<ValDoubleN>( )) os << "(ValDoubleN "      << ')'; break;
    Case(C<ValFloatN >( )) os << "(ValFloatN  "      << ')'; break;
    Case(C<ValCharN  >( )) os << "(ValCharN   "      << ')'; break;
    Case(C<ValUCharN >( )) os << "(ValUCharN  "      << ')'; break;
    Case(C<ValShortN >( )) os << "(ValShortN  "      << ')'; break;
    Case(C<ValUShortN>( )) os << "(ValUShortN "      << ')'; break;
    Case(C<ValIntN   >( )) os << "(ValIntN    "      << ')'; break;
    Case(C<ValUIntN  >( )) os << "(ValUIntN   "      << ')'; break;
    Case(C<ValLongN  >( )) os << "(ValLongN   "      << ')'; break;
    Case(C<ValULongN >( )) os << "(ValULongN  "      << ')'; break;
    Case(C<ValLLongN >( )) os << "(ValLLongN  "      << ')'; break;
    Case(C<ValULLongN>( )) os << "(ValULLongN "      << ')'; break;
    }
    EndMatch

    return os;
}

//------------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& os, const Reg& r)
{
    std::string name;
    int         n;
    TyPrim*     type;
    Val*        value;

    Match(r)
    {
    Case(C<Ext>(type, name)) os << "(Ext " << *type << ' ' << name << ')'; break;
    Case(C<Tmp>(type, n))    os << "(Tmp " << *type << ' ' << n    << ')'; break;
    Case(C<Lit>(value))      os << "(Lit " << *value               << ')'; break;
    }
    EndMatch

    return os;
}

//------------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& os, const Inst& i)
{
    std::string op; 
    Reg* arg1; Reg* arg2; Reg* src; Reg* dest;

    Match(i)
    {
    Case(C<InstBin >(C<Op>(op), arg1, arg2, dest)) 
        os << "InstBin (Op " << op << ") " << *arg1 << ' ' << *arg2 << ' ' << *dest; break;
    Case(C<InstUn  >(C<Op>(op), arg1, dest))
        os << "InstUn  (Op " << op << ") " << *arg1 << ' ' << *dest; break;
    Case(C<InstMove>(src, dest))
        os << "InstMove " << *src  << ' ' << *dest; break;
    }
    EndMatch

    return os;
}

//------------------------------------------------------------------------------

bool operator==(const TyPrim& a, const TyPrim& b)
{
    var<int> n;

    Match(a,b)
    {
    Case(C<TyChar   >( ), C<TyChar   >(  )) return true;
    Case(C<TyUChar  >( ), C<TyUChar  >(  )) return true;
    Case(C<TyShort  >( ), C<TyShort  >(  )) return true;
    Case(C<TyUShort >( ), C<TyUShort >(  )) return true;
    Case(C<TyInt    >( ), C<TyInt    >(  )) return true;
    Case(C<TyUInt   >( ), C<TyUInt   >(  )) return true;
    Case(C<TyLong   >( ), C<TyLong   >(  )) return true;
    Case(C<TyULong  >( ), C<TyULong  >(  )) return true;
    Case(C<TyLLong  >( ), C<TyLLong  >(  )) return true;
    Case(C<TyULLong >( ), C<TyULLong >(  )) return true;
    Case(C<TyFloat  >( ), C<TyFloat  >(  )) return true;
    Case(C<TyDouble >( ), C<TyDouble >(  )) return true;
    Case(C<TyCharN  >(n), C<TyCharN  >(+n)) return true;
    Case(C<TyUCharN >(n), C<TyUCharN >(+n)) return true;
    Case(C<TyShortN >(n), C<TyShortN >(+n)) return true;
    Case(C<TyUShortN>(n), C<TyUShortN>(+n)) return true;
    Case(C<TyIntN   >(n), C<TyIntN   >(+n)) return true;
    Case(C<TyUIntN  >(n), C<TyUIntN  >(+n)) return true;
    Case(C<TyLongN  >(n), C<TyLongN  >(+n)) return true;
    Case(C<TyULongN >(n), C<TyULongN >(+n)) return true;
    Case(C<TyLLongN >(n), C<TyLLongN >(+n)) return true;
    Case(C<TyULLongN>(n), C<TyULLongN>(+n)) return true;
    Case(C<TyFloatN >(n), C<TyFloatN >(+n)) return true;
    Case(C<TyDoubleN>(n), C<TyDoubleN>(+n)) return true;
    }
    EndMatch

    return false;
}

bool operator==(const Val& a, const Val& b)
{
    var<unsigned long long> u;
    var<         long long> n;
    var<            double> d;

    Match(a,b)
    {
    Case(C<ValDouble >(d), C<ValDouble >(+d)) return true;
    Case(C<ValFloat  >(d), C<ValFloat  >(+d)) return true;
    Case(C<ValChar   >(n), C<ValChar   >(+n)) return true;
    Case(C<ValUChar  >(u), C<ValUChar  >(+u)) return true;
    Case(C<ValShort  >(n), C<ValShort  >(+n)) return true;
    Case(C<ValUShort >(u), C<ValUShort >(+u)) return true;
    Case(C<ValInt    >(n), C<ValInt    >(+n)) return true;
    Case(C<ValUInt   >(u), C<ValUInt   >(+u)) return true;
    Case(C<ValLong   >(n), C<ValLong   >(+n)) return true;
    Case(C<ValULong  >(u), C<ValULong  >(+u)) return true;
    Case(C<ValLLong  >(n), C<ValLLong  >(+n)) return true;
    Case(C<ValULLong >(u), C<ValULLong >(+u)) return true;
    Case(C<ValDoubleN>( ), C<ValDoubleN>(  )) return true;
    Case(C<ValFloatN >( ), C<ValFloatN >(  )) return true;
    Case(C<ValCharN  >( ), C<ValCharN  >(  )) return true;
    Case(C<ValUCharN >( ), C<ValUCharN >(  )) return true;
    Case(C<ValShortN >( ), C<ValShortN >(  )) return true;
    Case(C<ValUShortN>( ), C<ValUShortN>(  )) return true;
    Case(C<ValIntN   >( ), C<ValIntN   >(  )) return true;
    Case(C<ValUIntN  >( ), C<ValUIntN  >(  )) return true;
    Case(C<ValLongN  >( ), C<ValLongN  >(  )) return true;
    Case(C<ValULongN >( ), C<ValULongN >(  )) return true;
    Case(C<ValLLongN >( ), C<ValLLongN >(  )) return true;
    Case(C<ValULLongN>( ), C<ValULLongN>(  )) return true;
    }
    EndMatch

    return false;
}

bool operator==(const Reg& a,    const Reg& b)
{
    var<std::string>   name;
    var<int>           n;
    var<const TyPrim&> type;  // FIX: See error when const is missing.
    var<const Val&>    value;

    Match(a,b)
    {
    Case(C<Ext>(&type, name), C<Ext>(&+type, +name)) return true;
    Case(C<Tmp>(&type, n)  ,  C<Tmp>(&+type, +n)   ) return true;
    Case(C<Lit>(&value)    ,  C<Lit>(&+value)      ) return true;
    }
    EndMatch

    return false;        
}

