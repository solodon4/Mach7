#include <iostream>

#ifdef _MSC_VER
#define __func__ __FUNCTION__
#endif

template <const char* p>
struct Text
{
};

int* foo()
{
    static int result[10];
    return result;
}

char test[] = "Test";

int main()
{
    std::cout << __func__  << __DATE__ << __FILE__ << std::endl;
    //char func_name[sizeof(__func__)] = __func__;
    Text<test> t;


}