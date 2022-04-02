#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

int main() {
  size_t n;
  std::unordered_map<std::string, int64_t> accounts;
  std::string name;
  int64_t sum, command_type;

  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> n;

  for (size_t i = 0; i < n; ++i) {
    std::cin >> command_type;

    if (command_type == 1) {
      std::cin >> name >> sum;

      if (!accounts.contains(name)) {
        accounts.insert(std::make_pair(name, 0));
      }

      accounts[name] += sum;
    } else {
      std::cin >> name;

      if (accounts.contains(name)) {
        std::cout << accounts[name] << '\n';
      } else {
        std::cout << "ERROR" << '\n';
      }
    }
  }
}