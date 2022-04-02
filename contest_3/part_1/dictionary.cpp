#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

int main() {
  size_t n, q;
  std::unordered_map<std::string, std::string> dictionary;
  std::string word_1, word_2;

  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> n;

  for (size_t i = 0; i < n; ++i) {
    std::cin >> word_1 >> word_2;

    dictionary.insert(std::make_pair(word_1, word_2));
    dictionary.insert(std::make_pair(word_2, word_1));
  }

  std::cin >> q;

  for (size_t i = 0; i < q; ++i) {
    std::cin >> word_1;
    std::cout << dictionary[word_1] << "\n";
  }
}