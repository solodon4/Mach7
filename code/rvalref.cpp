#include <iostream>

template <typename T> void foo(T&)        { std::cout << "T&" << std::endl; }
//template <typename T> void foo(T&&)       { std::cout << "T&&" << std::endl; }
template <typename T> void foo(const T&)  { std::cout << "const T&" << std::endl; }
template <typename T> void foo(const T&&) { std::cout << "const T&&" << std::endl; }

int main()
{
	const int n = 42;
	int m = 2;
	foo(4);
	foo(n);
	foo(m);
}
