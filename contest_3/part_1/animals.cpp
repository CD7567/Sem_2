#include <iostream>
#include <vector>
#include <algorithm>

int main() {
  size_t number, idx;
  int32_t color;
  std::vector<int32_t> animals;

  std::cin >> number;

  animals.reserve(number);
  for (size_t i = 0; i < number; ++i) {
    std::cin >> color;
    animals.push_back(color);
  }

  std::cin >> number;

  for (size_t i = 0; i < number; ++i) {
    std::cin >> color;
    idx = std::upper_bound(animals.begin(), animals.end(), color) -
          std::lower_bound(animals.begin(), animals.end(), color);

    std::cout << idx << std::endl;
  }
}