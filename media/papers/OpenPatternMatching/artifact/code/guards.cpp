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

#include "match.hpp"                // Support for Match statement
#include "patterns/combinators.hpp" // Support for pattern combinators
#include "patterns/constructor.hpp" // Support for constructor patterns
#include "patterns/guard.hpp"       // Support for guard patterns
#include "patterns/n+k.hpp"         // Support for n+k patterns
#include <complex>
#include <iostream>

enum { cart = mch::default_layout, plar = 1 };

namespace mch ///< Mach7 library namespace
{
#if defined(_MSC_VER) && _MSC_VER >= 1700 || defined(__GNUC__) && XTL_GCC_VERSION > 40700
/// C++ 11 introduced a weird overload for numeric types T for 
/// std::real<T>, std::imag<T>, std::abs<T> and std::arg<T>, which messed up our
/// nice syntax, and which is why we had to take address of member-functions
/// or do the explicit cast first:
template <typename T> struct bindings<std::complex<T>, cart> { CM(0,std::complex<T>::real); CM(1,std::complex<T>::imag); };
template <typename T> struct bindings<std::complex<T>, plar> { CM(0,(T (&)(const std::complex<T>&))std::abs<T>);  CM(1,(T (&)(const std::complex<T>&))std::arg<T>);  };
#else
/// Otherwise you should be able to write nicely like this:
template <typename T> struct bindings<std::complex<T>, cart> { CM(0,std::real<T>); CM(1,std::imag<T>); };
template <typename T> struct bindings<std::complex<T>, plar> { CM(0,std::abs<T>);  CM(1,std::arg<T>);  };
#endif
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
