#include <complex>
#include <iostream>
#include "match_generic.hpp"

enum { cart = default_layout, plar = 1 };

//template <typename T> struct match_members<std::complex<T>>       { CM(0,std::complex<T>::real); CM(1,std::complex<T>::imag); };
template <typename T> struct match_members<std::complex<T>, cart> { CM(0,std::real<T>); CM(1,std::imag<T>); };
template <typename T> struct match_members<std::complex<T>, plar> { CM(0,std::abs<T>);  CM(1,std::arg<T>);  };

int main()
{
    std::complex<double> c(3.14, 2.15);
    double r;
    double i;

    if (match<std::complex<double>>(r,i)(c))
        std::cout << '(' << r << ',' << i << ')' << std::endl;

    if (match<std::complex<double>, plar>(r,i)(c))
        std::cout << "The polar form of " << c << " is " << r << "*e^i*" << i << "rad " << std::polar(r,i) << std::endl;

    std::cout << '(' << std::real(c) << ',' << std::imag(c) << ')' << std::endl;
    unary(&std::real<double>);
}
