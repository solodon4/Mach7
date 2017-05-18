// This unit test is based on Mach7 issue #73 by @Rupsbant
// See https://github.com/solodon4/Mach7/issues/73 for details

#ifndef HAS_BOOST

#include <iostream>

int main() {
    std::cout << "This test requires Boost!\n";
    return 1;
}

#else

#include <stdio.h>
#include <mach7/adapters/boost/adapt_boost_variant.hpp>
#include <mach7/patterns/constructor.hpp> 
#include <mach7/patterns/primitive.hpp>   

template<int N> struct A {
  int val;
  A(int a) : val(a) {};
};
struct CC {
  boost::variant<A<0>, A<1>, A<2>, A<3>, A<4>, A<5>, A<6>, A<7>, A<8>> variant;
  template<typename T> CC(T t) : variant(t) {};
};
namespace mch {
  template <int N> struct bindings<A<N>> {
    Members(A<N>::val);
  };
  template <> struct bindings<CC> {
    Members(CC::variant);
  };
}
int do_some(CC c) {
  using namespace mch;
  var<int> x;
  Match(c.variant) {
    Case(C<A<0>>(x)) {
      return 0+x;
    }
    Case(C<A<8>>(x)) {
      return 8-x;
    }
    Otherwise() {
      return -1;
      break;
    }
  }
  EndMatch;
}
template<int N> int test() {
  return do_some(CC(A<N>(5)));
}
int main() {
  int result = 0;
  if (5 != test<0>()) { ++result; printf("Invalid result of test<0>()\n"); }
  if (3 != test<8>()) { ++result; printf("Invalid result of test<8>()\n"); }
  return result;
}

#endif // HAS_BOOST
