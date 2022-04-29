#include <iostream>
#include <iterator>
#include <vector>
#include <typeinfo>
#include <forward_list>
#include "contest_3/part_2/iter/enumerate.h"

template <typename T, typename It = typename T::iterator>
void foo(T&) {
  std::cout << __PRETTY_FUNCTION__ << std::endl;
}

int main() {
  auto p = Enumerate(std::vector<int>());
  auto t = Enumerate(p);

  foo(t);
}