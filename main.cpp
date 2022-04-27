#include <iostream>
#include <iterator>
#include <vector>
#include <typeinfo>
#include <forward_list>
#include "contest_3/part_2/vector/vector.h"

template <typename Arg>
void f(Arg&&) {
  std::cout << __PRETTY_FUNCTION__ << '\n';
};

void g(const int&) {
  std::cout << __PRETTY_FUNCTION__ << '\n';
};

void foo(int, int&);

typedef const int val;
typedef const val cval;

void type(cval c) {
  std::cout << __PRETTY_FUNCTION__;
}

int main() {
  auto hash = std::hash<int>();

  const int a = 1;
  const int& b = a;
  const int&& c = 1;

  f(1);
  f(a);
  f(b);
  f(c);

  g(a);
  g(b);
  g(c);
  g(1);

  type(1);
  //std::cout << __PRETTY_FUNCTION__ ;
}