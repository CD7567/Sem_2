#include <iostream>
#include <vector>
#include <cstdint>
#include <numeric>

inline size_t Log(size_t num) {
  return 31 - __builtin_clz(num);
}

int main() {
  size_t characters_amount, comm_amount, tmp, left, right;
  std::cin >> characters_amount;

  const size_t table_height = Log(characters_amount);

  std::vector<int> characters;

  characters.reserve(characters_amount);
  for (size_t i = 0; i < characters_amount; ++i) {
    std::cin >> tmp;
    characters.push_back(tmp);
  }

  std::vector<std::vector<int>> sparse_table(table_height + 1, std::vector<int>(characters_amount, INT32_MAX));

  for (size_t i = 0; i < characters_amount; ++i) {
    sparse_table[0][i] = characters[i];
  }

  for (size_t i = 1; i <= table_height; ++i) {
    size_t width_bound = characters_amount - (1 << i);

    for (size_t j = 0; j <= width_bound; ++j) {
      sparse_table[i][j] = std::gcd(sparse_table[i - 1][j], sparse_table[i - 1][j + (1 << (i - 1))]);
    }
  }

  size_t height;
  std::cin >> comm_amount;
  for (size_t i = 0; i < comm_amount; ++i) {
    std::cin >> left >> right;
    --left;
    --right;

    height = Log(right - left + 1);
    std::cout << std::gcd(sparse_table[height][left], sparse_table[height][right - (1 << height) + 1]) << std::endl;
  }
}