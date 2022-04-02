#include <iostream>
#include <vector>
#include <algorithm>

int main() {
  size_t number;
  int32_t elem;
  std::vector<int32_t> vect;

  std::cin >> number;

  vect.reserve(number);
  for (size_t i = 0; i < number; ++i) {
    std::cin >> elem;
    vect.push_back(elem);
  }

  std::cin >> elem;

  std::cout << std::count(vect.begin(), vect.end(), elem);
}