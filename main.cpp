#include <algorithm>
#include <cctype>
#include <cstring>
#include <cstdint>
#include <iostream>
#include <cppstring.h>

int main() {
  char a = 'a';
  char b = 'b';
  String str = String();

  std::cout << a << " " << (char*)(nullptr) << " " << b;
}