///
/// \file
///
/// This file is a part of pattern matching testing suite.
///
/// \autor Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of the XTL framework (http://parasol.tamu.edu/xtl/).
/// Copyright (C) 2005-2012 Texas A&M University.
/// All rights reserved.
///

#include "match.hpp"                // Support for Match statement
#include "patterns/constructor.hpp" // Support for constructor patterns
#include "patterns/guard.hpp"       // Support for guard patterns
#include "patterns/n+k.hpp"         // Support for n+k patterns
#include <complex>
#include <iostream>

enum { cart = mch::default_layout, plar = 1 };

namespace mch ///< Mach7 library namespace
{
//template <typename T> struct bindings<std::complex<T>>       { CM(0,std::complex<T>::real); CM(1,std::complex<T>::imag); };
template <typename T> struct bindings<std::complex<T>, cart> { CM(0,std::real<T>); CM(1,std::imag<T>); };
template <typename T> struct bindings<std::complex<T>, plar> { CM(0,std::abs<T>);  CM(1,std::arg<T>);  };
} // of namespace mch

typedef mch::view<std::complex<double>,cart> cartesian;
typedef mch::view<std::complex<double>,plar> polar;

int main()
{
    const std::complex<double> i(0.0,1.0);
    mch::var<double> a,b,r,f;
    mch::wildcard _;

    Match(i)
    {
    Qua(cartesian,a,b)        std::cout << a << "+"    << b << "*i" << std::endl;
     When(a,b |= a < b)       std::cout << a << "<"    << b << std::endl;
     When(a,b |= a ==b)       std::cout << a << "=="   << b << std::endl;
     When(a,b |= a > b)       std::cout << a << ">"    << b << std::endl;
     When(0,1)                std::cout << "(0,1)"     << std::endl;
     When(1+a,1+b)            std::cout << "1+" << a << ",1+" << b << std::endl;
     When(a+1,b+1)            std::cout << a << "+1,"  << b << "+1"<< std::endl;
    Qua(polar,    r,f)        std::cout << r << "*e^i" << f << std::endl;
     When(r,f |= r < f)       std::cout << r << "<"    << f << std::endl;
     When(r,f |= r ==f)       std::cout << r << "=="   << f << std::endl;
     When(r,f |= r > f)       std::cout << r << ">"    << f << std::endl;
     When(2*r,2*f)            std::cout << "2*" << r << ",2*"  << f << std::endl;
     When(r*2,f*2)            std::cout << r << "*2,"  << f << "*2" << std::endl;
     When(r*2 |= r > 0.6,f*2) std::cout << r << "*2>1.2,"  << f << "*2" << std::endl;
     When(r*2,f*2 |= f > 0.6) std::cout << r << "*2,"  << f << "*2>1.2" << std::endl;
     When(r*2,  _)            std::cout << r << "*2, ???" << std::endl;
   //When(r*2,  _ |= r < 0.6) std::cout << r << "*2<1.2, ???" << std::endl;
     When(r*2,  f |= r < 0.6) std::cout << r << "*2<1.2, ???" << std::endl; // FIX: Replaced _ with f in the above as currently we don't have guard specialization
    }
    EndMatch
}
