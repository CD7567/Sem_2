#include <iostream>
#include <string>
#include <map>
#include <vector>

int main() {
  std::map<std::string, size_t> words;
  std::string word;

  while(std::cin >> word) {
    if (words.count(word) == 0) {
      words.insert(std::make_pair(word, 1));
    } else {
      ++words[word];
    }
  }

  std::vector<std::pair<std::string, size_t>> vect_words(words.begin(), words.end());
  std::sort(vect_words.begin(), vect_words.end(),[](const std::pair<std::string, size_t>& lhs, const std::pair<std::string, size_t>& rhs) {
    bool result;

    if (lhs.second == rhs.second) {
      result = lhs.first < rhs.first;
    } else {
      result = lhs.second > rhs.second;
    }

    return result;
  });

  for (auto& elem : vect_words) {
    std::cout << elem.first << std::endl;
  }
}