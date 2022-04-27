#include <iostream>
#include <iterator>
#include <vector>
#include <forward_list>
#include "contest_3/part_2/vector/vector.h"

template <typename Arg>
void f(Arg&&);

void foo(int, int&);

int main() {
  auto hash = std::hash<int>();

  hash(1);

  std::cout << std::distance(list.end(), list.begin());
}