#include <iostream>

int foo(int n)
{
    int j = n*2;
    int arr[10] = {0,1,2,3,4,5,6,7,8};

    int brr[10] = {9,8,7,6,5,4,3,2,1};

    arr[n] = 42;
    return j;
}

int main()
{
    std::cout << "Hello" << std::endl;
    foo(10);
    foo(11);
    foo(12);
    foo(13);
    foo(14);
}