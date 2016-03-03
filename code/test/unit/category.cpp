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

#include <mach7/match.hpp>                 // Support for Match statement
#include <mach7/patterns/constructor.hpp>  // Support for constructor patterns

#include <iostream>
#include <typeinfo>
#include <utility>

#if defined(__GNUC__)
    // We need to do this workaround for this file until GCC will fix the bug 
    // in their handling of C++0x. The details of the bug can be found here:
    // http://gcc.gnu.org/bugzilla/show_bug.cgi?id=50080
    #undef  XTL_CPP0X_TEMPLATE
    #define XTL_CPP0X_TEMPLATE template
#endif

template<class X, class Y, class S, class T>
std::pair<S, T> lift(const std::pair<X, Y>& p, S f(X), T g(Y))
{
	return std::pair<S, T>(f(p.first), g(p.second));
}

template<class X, class Y> struct Either;
template<class X, class Y> struct Left;
template<class X, class Y> struct Right;

template<class X, class Y>
struct EitherVisitor
{
	virtual void visit(const Left<X, Y>&) = 0;
	virtual void visit(const Right<X, Y>&) = 0;
};

template<class X, class Y>
struct Either
{
	virtual ~Either() { }
	virtual void accept(EitherVisitor<X, Y>& v) const = 0;
	virtual std::ostream& operator>>(std::ostream& os) const = 0;
	friend  std::ostream& operator<<(std::ostream& os, const Either& e) { return e >> os; }
};

template<class X, class Y>
struct Left : Either<X, Y>
{
	const X x;
	Left(const X& x) : x(x) { }
    Left& operator=(const Left&) XTL_DELETED;
	void accept(EitherVisitor<X, Y>& v) const { v.visit(*this); }
	virtual std::ostream& operator>>(std::ostream& os) const { return os << "Left<" << typeid(X).name() << ',' << typeid(Y).name() << ">(" << x << ')'; }
};

template<class X, class Y>
struct Right : Either<X, Y> 
{
	const Y y;
	Right(const Y& y) : y(y) { }
    Right& operator=(const Right&) XTL_DELETED;
	void accept(EitherVisitor<X, Y>& v) const { v.visit(*this); }
	virtual std::ostream& operator>>(std::ostream& os) const { return os << "Right<" << typeid(X).name() << ',' << typeid(Y).name() << ">(" << y << ')'; }
};

template<class S, class T>
Left<S,T>*  left(const S& s)  { return new Left<S,T>(s); }

template<class S, class T>
Right<S,T>* right(const T& t) { return new Right<S,T>(t); }

template<class X, class Y, class S, class T>
const Either<S, T>* lift(const Either<X, Y>& e, S f(X), T g(Y))
{
	typedef S (*F)(X);
	typedef T (*G)(Y);

	struct Impl : EitherVisitor<X, Y>
	{
		F f;
		G g;
		const Either<S, T>* value;
		Impl(F f, G g) : f(f), g(g), value() { }
		void visit(const Left<X, Y>& e)
		{
			value = left<S, T>(f(e.x));
		}
		void visit(const Right<X, Y>& e)
		{
			value = right<S, T>(g(e.y));
		}
	};

	Impl vis(f, g);
	e.accept(vis);
	return vis.value;
}

namespace mch 
{
    template <class X, class Y> struct bindings<Left<X,Y> >   { typedef Left<X,Y>  type; Members(type::x); }; // We use a typedef here because of comma in Left<X,Y>
    template <class X, class Y> struct bindings<Right<X,Y> >  { typedef Right<X,Y> type; Members(type::y); }; // We use a typedef here because of comma in Right<X,Y>
} // of namespace mch

template<class X, class Y, class S, class T>
const Either<S, T>* lift_ex(const Either<X, Y>& e, S f(X), T g(Y))
{
	X x;
	Y y;

	if (mch::C<Left<X, Y> >(x)(e))
		return left<S, T>(f(x));
	else
	if (mch::C<Right<X, Y> >(y)(e))
		return right<S, T>(g(y));
	else
		return 0;
}


template<class X, class Y, class S, class T>
const Either<S, T>* lift_ex2(const Either<X, Y>& e, S f(X), T g(Y))
{
    Match(e)
    {
        Case(TypeArg( Left<X,Y>), x) return  left<S, T>(f(x));
        Case(TypeArg(Right<X,Y>), y) return right<S, T>(g(y));
        Otherwise() return nullptr;
    }
    EndMatch

    return nullptr; // TODO: Check if EndMatch can generate something to indicate unreachable when Otherwise is present
}

int  my_f(double d) { return (int)(d+0.5); }
bool my_g(char c)   { return c >= 'A' && c <= 'Z'; }

int main()
{
	Either<double,char>* pa = left<double,char>(3.14);
	Either<double,char>* pb = right<double,char>('S');
	const Either<int,bool>* pa1 = lift(*pa,my_f,my_g);
	const Either<int,bool>* pb1 = lift(*pb,my_f,my_g);
	const Either<int,bool>* pa2 = lift_ex(*pa,my_f,my_g);
	const Either<int,bool>* pb2 = lift_ex(*pb,my_f,my_g);
	const Either<int,bool>* pa3 = lift_ex2(*pa,my_f,my_g);
	const Either<int,bool>* pb3 = lift_ex2(*pb,my_f,my_g);

	std::cout << *pa << std::endl;
	std::cout << *pb << std::endl;
	std::cout << *pa1 << std::endl;
	std::cout << *pb1 << std::endl;
	std::cout << *pa2 << std::endl;
	std::cout << *pb2 << std::endl;
	std::cout << *pa3 << std::endl;
	std::cout << *pb3 << std::endl;

}
/*
template <typename C>
void test(const C& c)
{
	typedef type_variable<0> K;
	typedef type_variable<1> T;
	typedef type_variable<2> A;

	if (C<std::vector<T,A> >(c))
	else
	if (C<std::list<T,A> >(c))
}
*/
