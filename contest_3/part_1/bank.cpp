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
    std::cin >> command_type >> name;
    auto account = accounts.find(name);

    if (command_type == 1) {
      std::cin >> sum;

      if (account == accounts.end()) {
        accounts.emplace(name, sum);
      } else {
        account->second += sum;
      }
    } else {
      if (account == accounts.end()) {
        std::cout << "ERROR" << '\n';
      } else {
        std::cout << account->second << '\n';
      }
    }
  }
}