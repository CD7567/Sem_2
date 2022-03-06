#include <algorithm>
#include <cctype>
#include <cstring>
#include <cstdint>
#include <iostream>
#include <cppstring.h>

int main() {
  const String a;
  const String b("caba");

  std::cout << (a + b == "caba");
}