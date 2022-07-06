#include <iostream>
#include <vector>
#include <string>

class BinaryIndexedTree {
 public:
  explicit BinaryIndexedTree(std::vector<int64_t>&& src) : src_array_(std::move(src)) {
    size_t size = src_array_.size();
    std::vector<int64_t> prefix_sums(size);

    tree_array_.resize(src_array_.size());

    tree_array_[0] = src_array_[0];
    prefix_sums[0] = src_array_[0];

    for (size_t i = 1; i < src_array_.size(); i++) {
      prefix_sums[i] = prefix_sums[i - 1] + src_array_[i];

      if (F(i) == 0) {
        tree_array_[i] = prefix_sums[i];
      } else {
        tree_array_[i] = prefix_sums[i] - prefix_sums[F(i) - 1];
      }
    }
  }

  void Update(size_t idx, int64_t value) {
    for (size_t i = idx; i < tree_array_.size(); i = i | (i + 1)) {
      tree_array_[i] += value - src_array_[idx];
    }

    src_array_[idx] = value;
  }

  int64_t Sum(size_t left, size_t right) {
    return PrefixSum(right) - (left == 0 ? 0 : PrefixSum(left - 1));
  }

 private:
  std::vector<int64_t> src_array_;
  std::vector<int64_t> tree_array_;

  static inline size_t F(size_t i) {
    return (i & (i + 1));
  }

  int64_t PrefixSum(size_t idx) {
    int64_t sum = 0;

    for (size_t i = idx; i <= idx; i = F(i) - 1) {
      sum += tree_array_[i];
    }

    return sum;
  }
};

int main() {
  size_t characters_amount, comm_amount, left, right, idx;
  int64_t new_value;
  std::string command;
  std::vector<int64_t> characters;

  std::cin >> characters_amount;
  characters.resize(characters_amount);

  for (size_t i = 0; i < characters_amount; ++i) {
    std::cin >> characters[i];
  }

  BinaryIndexedTree tree(std::move(characters));

  std::cin >> comm_amount;

  for (size_t i = 0; i < comm_amount; ++i) {
    std::cin >> command;

    if (command == "s") {
      std::cin >> left >> right;

      std::cout << tree.Sum(left - 1, right - 1) << ' ';
    } else if (command == "u") {
      std::cin >> idx >> new_value;

      tree.Update(idx - 1, new_value);
    }
  }
}