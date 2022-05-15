#include <iostream>
#include <vector>

int main() {
  size_t n, k, lbound, rbound;

  std::cin >> n;

  std::vector<size_t> votes(n + 1);
  std::cin >> votes[1];

  for (size_t i = 2; i <= n; ++i) {
    std::cin >> votes[i];
    votes[i] += votes[i - 1];
  }

  std::cin >> k;

  for (size_t i = 0; i < k; ++i) {
    std::cin >> lbound >> rbound;

    if (lbound > 1) {
      std::cout << votes[rbound] - votes[lbound - 1] << " ";
    } else {
      std::cout << votes[rbound] << " ";
    }
  }
}