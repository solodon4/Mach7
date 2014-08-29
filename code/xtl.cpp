#include "xtl.hpp"

static_assert(xtl::is_subtype<int,int>::value,"No reflexivity");

class A { public: int a; virtual ~A(); };
class B : public A { public: int b; };

int main()
{
    static_assert(xtl::is_subtype<B,A>::value,"Subclassing");
    return xtl::is_subtype<B,A>::value;

    A a;
    B b;
    a = xtl::subtype_cast<A>(b);
    //b = xtl::subtype_cast<B>(a); // error

    A* pa = xtl::subtype_cast<A*>(&b);
    //B* pb = xtl::subtype_cast<B*>(&a); // error
    B* pb = xtl::subtype_dynamic_cast<B*>(pa);
}

