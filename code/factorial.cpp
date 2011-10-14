#include <iostream>

template <int N> struct factorial
{
	enum { result = N*factorial<N-1>::result };
};

template <>      struct factorial<0>
{
	enum { result = 1 };
};

int main()
{
	const int f6 = factorial<6>::result;
	std::cout << f6 << std::endl;
}
