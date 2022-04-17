#include <iostream>
#include <queue>

int main() {
  const int64_t k_rate = 5;
  const long double k_percent_divisor = 100;
  size_t n;
  int64_t cost = 0;
  int64_t tmp;
  std::priority_queue<int64_t> integers;

  std::cin >> n;

  for (size_t i = 0; i < n; ++i) {
    std::cin >> tmp;
    integers.push(-tmp);
  }

  for (size_t i = 1; i < n; ++i) {
    tmp = integers.top();
    integers.pop();
    tmp += integers.top();
    integers.pop();

    cost -= tmp * k_rate;
    integers.push(tmp);
  }

  std::cout << static_cast<long double>(cost) / k_percent_divisor;
}