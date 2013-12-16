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
#include "patterns/constructor.hpp" // Support for constructor patterns

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
    std::complex<double> c(3.14, 2.15);
    double r;
    double i;

    if (mch::C<std::complex<double>>(r,i)(c))
        std::cout << '(' << r << ',' << i << ')' << std::endl;

    if (mch::C<std::complex<double>, plar>(r,i)(c))
        std::cout << "The polar form of " << c << " is " << r << "*e^i*" << i << "rad " << std::polar(r,i) << std::endl;

    if (mch::C<cartesian>(r,i)(c))
        std::cout << '(' << r << ',' << i << ')' << std::endl;

    if (mch::C<polar>(r,i)(c))
        std::cout << "The polar form of " << c << " is " << r << "*e^i*" << i << "rad " << std::polar(r,i) << std::endl;

    std::cout << '(' << std::real(c) << ',' << std::imag(c) << ')' << std::endl;
}
