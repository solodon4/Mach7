#pragma once

#include "patterns/bindings.hpp"       // Bindings specification
#include "versity-types.hpp"

//------------------------------------------------------------------------------

namespace mch ///< Mach7 library namespace
{

template <> struct bindings<Op        > { CM(0,Op        ::name); };

template <> struct bindings<TyCharN   > { CM(0,TyCharN   ::size); };
template <> struct bindings<TyUCharN  > { CM(0,TyUCharN  ::size); };
template <> struct bindings<TyShortN  > { CM(0,TyShortN  ::size); };
template <> struct bindings<TyUShortN > { CM(0,TyUShortN ::size); };
template <> struct bindings<TyIntN    > { CM(0,TyIntN    ::size); };
template <> struct bindings<TyUIntN   > { CM(0,TyUIntN   ::size); };
template <> struct bindings<TyLongN   > { CM(0,TyLongN   ::size); };
template <> struct bindings<TyULongN  > { CM(0,TyULongN  ::size); };
template <> struct bindings<TyLLongN  > { CM(0,TyLLongN  ::size); };
template <> struct bindings<TyULLongN > { CM(0,TyULLongN ::size); };
template <> struct bindings<TyFloatN  > { CM(0,TyFloatN  ::size); };
template <> struct bindings<TyDoubleN > { CM(0,TyDoubleN ::size); };
                                                         
template <> struct bindings<ValDouble > { CM(0,ValDouble ::value); };
template <> struct bindings<ValFloat  > { CM(0,ValFloat  ::value); };
template <> struct bindings<ValChar   > { CM(0,ValChar   ::value); };
template <> struct bindings<ValUChar  > { CM(0,ValUChar  ::value); };
template <> struct bindings<ValShort  > { CM(0,ValShort  ::value); };
template <> struct bindings<ValUShort > { CM(0,ValUShort ::value); };
template <> struct bindings<ValInt    > { CM(0,ValInt    ::value); };
template <> struct bindings<ValUInt   > { CM(0,ValUInt   ::value); };
template <> struct bindings<ValLong   > { CM(0,ValLong   ::value); };
template <> struct bindings<ValULong  > { CM(0,ValULong  ::value); };
template <> struct bindings<ValLLong  > { CM(0,ValLLong  ::value); };
template <> struct bindings<ValULLong > { CM(0,ValULLong ::value); };
template <> struct bindings<ValDoubleN> { CM(0,ValDoubleN::value); };
template <> struct bindings<ValFloatN > { CM(0,ValFloatN ::value); };
template <> struct bindings<ValCharN  > { CM(0,ValCharN  ::value); };
template <> struct bindings<ValUCharN > { CM(0,ValUCharN ::value); };
template <> struct bindings<ValShortN > { CM(0,ValShortN ::value); };
template <> struct bindings<ValUShortN> { CM(0,ValUShortN::value); };
template <> struct bindings<ValIntN   > { CM(0,ValIntN   ::value); };
template <> struct bindings<ValUIntN  > { CM(0,ValUIntN  ::value); };
template <> struct bindings<ValLongN  > { CM(0,ValLongN  ::value); };
template <> struct bindings<ValULongN > { CM(0,ValULongN ::value); };
template <> struct bindings<ValLLongN > { CM(0,ValLLongN ::value); };
template <> struct bindings<ValULLongN> { CM(0,ValULLongN::value); };
                                                          
template <> struct bindings<Ext       > { CM(0,Ext::type); CM(1,Ext::name); };
template <> struct bindings<Tmp       > { CM(0,Tmp::type); CM(1,Tmp::name); };
template <> struct bindings<Lit       > { CM(0,Lit::value);           };
                                                          
template <> struct bindings<InstBin   > { CM(0,InstBin ::operation); CM(1,InstBin ::arg1); CM(2,InstBin::arg2); CM(3,InstBin::dest); };
template <> struct bindings<InstUn    > { CM(0,InstUn  ::operation); CM(1,InstUn  ::arg1); CM(2,InstUn ::dest); };
template <> struct bindings<InstMove  > { CM(0,InstMove::src);       CM(1,InstMove::dest); };

} // of namespace mch

//------------------------------------------------------------------------------
