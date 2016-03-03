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

#include <mach7/patterns/bindings.hpp>     // Mach7 support for bindings on arbitrary UDT
#include "shape.hpp"

// Members binding for Shape hierarchy

namespace mch ///< Mach7 library namespace
{
template <> struct bindings<Shape>    {};

template <> struct bindings<Circle>   { Members(Circle::get_center,Circle::radius); };
template <> struct bindings<Square>   { Members(Square::upper_left,Square::side);   };
template <> struct bindings<Triangle> { Members(Triangle::first,Triangle::second,Triangle::third); };

// Members binding for std::pair and cloc C struct we use for unions case

template <typename X, typename Y> 
            struct bindings<std::pair<X,Y>> { typedef std::pair<X,Y> arg_type; Members(arg_type::first,arg_type::second); };
template <> struct bindings<cloc>     { Members(cloc::first,cloc::second); };

// Members binding for discriminated union (ADT) case of Shape hierarchy

template <> struct bindings<ADTShape>   { KS(ADTShape::kind); };

template <> struct bindings<ADTShape,ADTShape::circle>     { KV(ADTShape,ADTShape::circle);   Members(ADTShape::center,ADTShape::radius); };
template <> struct bindings<ADTShape,ADTShape::square>     { KV(ADTShape,ADTShape::square);   Members(ADTShape::upper_left,ADTShape::size);   };
template <> struct bindings<ADTShape,ADTShape::triangle>   { KV(ADTShape,ADTShape::triangle); Members(ADTShape::first,ADTShape::second,ADTShape::third); };

// Members binding for inherited class from the above ADT

template <> struct bindings<ADTShapeEx> { KS(ADTShape::kind); };

template <> struct bindings<ADTShapeEx,ADTShape::circle>   { KV(ADTShape,ADTShape::circle);   Members(ADTShape::center,ADTShape::radius,ADTShapeEx::extra_field); };
template <> struct bindings<ADTShapeEx,ADTShape::square>   { KV(ADTShape,ADTShape::square);   Members(ADTShape::upper_left,ADTShape::size,ADTShapeEx::extra_field); };
template <> struct bindings<ADTShapeEx,ADTShape::triangle> { KV(ADTShape,ADTShape::triangle); Members(ADTShape::first,ADTShape::second,ADTShape::third,ADTShapeEx::extra_field); };
} // of namespace mch
