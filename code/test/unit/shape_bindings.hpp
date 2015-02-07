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
