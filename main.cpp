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

static std::hash<std::basic_string<char>> hash1;

void foo(const std::string& a) {
  std::cout << hash1(a);
}

int main() {
  auto hash = std::hash<int>();
  auto list = std::forward_list<int>();

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

  const auto str = std::to_string(1);

  foo(str);
}