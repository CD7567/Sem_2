#include <iostream>

int main() {
  int32_t max_sum = INT32_MIN, sum = 0, elem, neg_pos = -1;
  size_t n, left_bound = 0, right_bound = 0;

  std::cin >> n;

  for (size_t i = 0; i < n; ++i) {
    std::cin >> elem;

    if (max_sum == INT32_MIN) {
      max_sum = elem;
    }

    sum += elem;

    if (sum > max_sum) {
      max_sum = sum;
      left_bound = neg_pos + 1;
      right_bound = i;
    }

    if (sum < 0) {
      sum = 0;
      neg_pos = static_cast<int32_t>(i);
    }
  }

  std::cout << left_bound + 1 << ' ' << right_bound + 1 << ' ' << max_sum;
}