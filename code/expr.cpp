#include <iostream>
#include <typeinfo>
#include <string>
#include "match_shape.hpp"

template <typename T> struct type_name              { static std::string value() { return typeid(T).name(); } };
template <>           struct type_name<bool>        { static std::string value() { return "bool";    } };
template <>           struct type_name<char>        { static std::string value() { return "char";    } };
template <>           struct type_name<short>       { static std::string value() { return "short";   } };
template <>           struct type_name<int>         { static std::string value() { return "int";     } };
template <>           struct type_name<long>        { static std::string value() { return "long";    } };
template <>           struct type_name<float>       { static std::string value() { return "float";   } };
template <>           struct type_name<double>      { static std::string value() { return "double";  } };
template <>           struct type_name<std::string> { static std::string value() { return "string";  } };
template <typename T> struct type_name<T const>     { static std::string value() { return type_name<T>::value() + " const"; } };
template <typename T> struct type_name<T volatile>  { static std::string value() { return type_name<T>::value() + " volatile"; } };
template <typename T> struct type_name<T&>          { static std::string value() { return type_name<T>::value() + '&'; } };
template <typename T> struct type_name<T*>          { static std::string value() { return type_name<T>::value() + '*'; } };
template <typename T, int N> struct type_name<T[N]> { static std::string value() { return type_name<T>::value() + "[N]"; } };
template <typename T> struct type_name<variable<T>> { static std::string value() { return "variable<" + type_name<T>::value() + '>'; } };
template <typename T> struct type_name<var_ref<T>>  { static std::string value() { return "var_ref<" + type_name<T>::value() + '>'; } };
template <typename T> struct type_name<value<T>>    { static std::string value() { return "value<" + type_name<T>::value() + '>'; } };
template <typename T, typename E>  struct type_name<guard<T,E>> { static std::string value() { return "guard<" + type_name<T>::value() + ',' + type_name<E>::value() + '>'; } };
template <typename F, typename E1> struct type_name<expr<F,E1>> { static std::string value() { return "expr<"  + type_name<F>::value() + ',' + type_name<E1>::value() + '>'; } };
template <typename F, typename E1, typename E2> struct type_name<expr<F,E1,E2>> { static std::string value() { return "expr<" + type_name<F>::value() + ',' + type_name<E1>::value() + ',' + type_name<E2>::value() + '>'; } };


template <typename T>
void dump(T&&)
{
	std::cout << type_name<T>::value() << std::endl;
}

template <typename T>
void dump_r(const T&)
{
	std::cout << type_name<typename T::result_type>::value() << std::endl;
}

template <typename T>
void dump_r(T&)
{
	std::cout << type_name<typename T::result_type>::value() << std::endl;
}

template <typename T, typename E> 
//expr<addition,variable<T>,typename flt<E>::type> 
void foo(const variable<T>& v, E&& e) 
{
	std::cout << "----------------------------" << std::endl;
	dump(v);
	dump(std::forward<E>(e));
	std::cout << type_name<E>::value() << std::endl;
	dump(filter(std::forward<E>(e)));
	//return expr<addition,variable<T>,typename flt<E>::type>(v,filter(std::forward<E>(e))); 
}

int main()
{
	variable<int> a = 5;
	variable<int> b = 5;
	std::cout << a << std::endl;
	std::cout << b << std::endl;
	variable<int> c(a+b+b);
	variable<int> c1 = c;
	int d = b*2;
	b*2;
	int e = 2*b;
	std::cout << c1 << ' ' << d << ' ' << e << std::endl;
	dump(a);
	dump(a+b);
	dump(a+b+b);
	foo(a,42);
	dump(b*2);
	dump(2*b);
	dump(a | a == 1);
	dump(b*2+1);
	typedef decltype(b*2+1) Etype; 
	std::cout << type_name<Etype::result_type>::value() << std::endl;
	dump_r(b*2+1);
	dump_r(b);
	dump_r(val(2));
	dump_r(b+1);
	dump(-(a*2+1));
	dump(2*b);
}
