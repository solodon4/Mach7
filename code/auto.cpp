#include <iostream>
#include <typeinfo>

int main()
{
    const int a = 5;
    const int* p = &a;
    auto& b = a;
    auto& c = p;
    std::cout << typeid(a).name() << std::endl;
    std::cout << typeid(p).name() << std::endl;
    std::cout << typeid(b).name() << std::endl;
    std::cout << typeid(c).name() << std::endl;
}