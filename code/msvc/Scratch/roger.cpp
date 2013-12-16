#include <iostream>

typedef int (*function_type)(double);

template <typename T, size_t N>
struct Dispatcher
{
    inline static int do_something(double d) { std::cout << "General case for " << N << "called with " << d << std::endl; return 2; }
};

template <typename T>
void dispatch_packed2(int n)
{
    switch (n)
    {
    case  0: Dispatcher<T, 0>::do_something(3.1415); break;
    case  1: Dispatcher<T, 1>::do_something(3.1415); break;
    case  2: Dispatcher<T, 2>::do_something(3.1415); break;
    case  3: Dispatcher<T, 3>::do_something(3.1415); break;
    case  4: Dispatcher<T, 4>::do_something(3.1415); break;
    case  5: Dispatcher<T, 5>::do_something(3.1415); break;
    case  6: Dispatcher<T, 6>::do_something(3.1415); break;
    case  7: Dispatcher<T, 7>::do_something(3.1415); break;
    case  8: Dispatcher<T, 8>::do_something(3.1415); break;
    case  9: Dispatcher<T, 9>::do_something(3.1415); break;
    case 10: Dispatcher<T,10>::do_something(3.1415); break;
    case 11: Dispatcher<T,11>::do_something(3.1415); break;
    case 12: Dispatcher<T,12>::do_something(3.1415); break;
    case 13: Dispatcher<T,13>::do_something(3.1415); break;
    case 14: Dispatcher<T,14>::do_something(3.1415); break;
    case 15: Dispatcher<T,15>::do_something(3.1415); break;
    }
}

template <typename T>
struct Dispatcher<T,3>
{
    inline static int do_something(double d) { std::cout << "Dispatcher 3 with " << d << std::endl;  return 5; }
    static const int extra = 42;
};

template <typename T>
struct Dispatcher<T,5>
{
    inline static int do_something(double d) { std::cout << "Dispatcher 5 with " << d << std::endl;  return 1; }
    static const int extra2 = 4;
};

int main()
{
    dispatch_packed2<float>(0);
    dispatch_packed2<float>(5);
    dispatch_packed2<float>(3);
}
