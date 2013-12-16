///
/// \file
///
/// This file is a part of pattern matching testing suite.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of Mach7 library (http://parasol.tamu.edu/mach7/).
/// Copyright (C) 2011-2012 Texas A&M University.
/// All rights reserved.
///

#pragma once

#include "patterns/bindings.hpp"
#include "shape.hpp"

// Members binding for Shape hierarchy

namespace mch ///< Mach7 library namespace
{
template <> struct bindings<Shape>    {};

template <> struct bindings<Circle>   { CM(0,Circle::get_center); CM(1,Circle::radius); };
template <> struct bindings<Square>   { CM(0,Square::upper_left); CM(1,Square::side);   };
template <> struct bindings<Triangle> { CM(0,Triangle::first);    CM(1,Triangle::second); CM(2,Triangle::third); };

// Members binding for std::pair and cloc C struct we use for unions case

template <typename X, typename Y> 
            struct bindings<std::pair<X,Y>> { CM(0,std::pair<X,Y>::first);  CM(1,std::pair<X,Y>::second); };
template <> struct bindings<cloc>     { CM(0,cloc::first); CM(1,cloc::second); };

// Members binding for discriminated union (ADT) case of Shape hierarchy

template <> struct bindings<ADTShape>   { KS(ADTShape::kind); };

template <> struct bindings<ADTShape,ADTShape::circle>     { KV(ADTShape,ADTShape::circle);   CM(0,ADTShape::center);     CM(1,ADTShape::radius); };
template <> struct bindings<ADTShape,ADTShape::square>     { KV(ADTShape,ADTShape::square);   CM(0,ADTShape::upper_left); CM(1,ADTShape::size);   };
template <> struct bindings<ADTShape,ADTShape::triangle>   { KV(ADTShape,ADTShape::triangle); CM(0,ADTShape::first);      CM(1,ADTShape::second); CM(2,ADTShape::third); };

// Members binding for inherited class from the above ADT

template <> struct bindings<ADTShapeEx> { KS(ADTShape::kind); };

template <> struct bindings<ADTShapeEx,ADTShape::circle>   { KV(ADTShape,ADTShape::circle);   CM(0,ADTShape::center);     CM(1,ADTShape::radius); CM(2,ADTShapeEx::extra_field); };
template <> struct bindings<ADTShapeEx,ADTShape::square>   { KV(ADTShape,ADTShape::square);   CM(0,ADTShape::upper_left); CM(1,ADTShape::size);   CM(2,ADTShapeEx::extra_field); };
template <> struct bindings<ADTShapeEx,ADTShape::triangle> { KV(ADTShape,ADTShape::triangle); CM(0,ADTShape::first);      CM(1,ADTShape::second); CM(2,ADTShape::third); CM(3,ADTShapeEx::extra_field); };
} // of namespace mch
