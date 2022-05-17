#include <iostream>
#include <vector>
#include <cstdint>

inline size_t Log(size_t num) {
  return 31 - __builtin_clz(num);
}

inline size_t BinTop(size_t size) {
  size_t top = 1 << Log(size);

  top <<= ((top < size) ? 2 : 1);

  return top - 1;
}

class SegmentTree {
 public:
  explicit SegmentTree(const std::vector<int32_t>& src) : size_(BinTop(src.size())), tree_(size_, {INT32_MIN, 1}) {
    size_t leaves_shift = size_ / 2;
    size_t src_size = src.size();

    for (size_t i = 0; i < src_size; ++i) {
      tree_[i + leaves_shift].first = src[i];
    }

    for (size_t i = leaves_shift - 1; i < leaves_shift; --i) {
      std::pair<int32_t, size_t>& left_child = tree_[GetLeftChildIdx(i)];
      std::pair<int32_t, size_t>& right_child = tree_[GetRightChildIdx(i)];

      if (left_child.first > right_child.first) {
        tree_[i].first = left_child.first;
        tree_[i].second = left_child.second;
      } else if (left_child.first < right_child.first) {
        tree_[i].first = right_child.first;
        tree_[i].second = right_child.second;
      } else {
        tree_[i].first = left_child.first;
        tree_[i].second = left_child.second + right_child.second;
      }
    }
  }

  std::pair<int32_t, size_t> Max(size_t begin, size_t end) {
    return NodeMax(begin, end, 0, 0, size_ / 2);
  }

 private:
  size_t size_;
  std::vector<std::pair<int32_t, size_t>> tree_;

  std::pair<int32_t, size_t> NodeMax(size_t begin, size_t end, size_t node, size_t max_begin, size_t max_end) {
    std::pair<int32_t, size_t> result;

    if (begin <= max_begin && end >= max_end) {
      result = tree_[node];
    } else if (max_end < begin || max_begin > end) {
      result = {INT32_MIN, 0};
    } else {
      size_t left_idx = GetLeftChildIdx(node), right_idx = GetRightChildIdx(node);
      auto left_max = NodeMax(begin, end, left_idx, max_begin, (max_begin + max_end) / 2);
      auto right_max = NodeMax(begin, end, right_idx, (max_begin + max_end) / 2 + 1, max_end);


      if (left_max.first > right_max.first) {
        result.first = left_max.first;
        result.second = left_max.second;
      } else if (left_max.first < right_max.first) {
        result.first = right_max.first;
        result.second = right_max.second;
      } else {
        result.first = left_max.first;
        result.second = left_max.second + right_max.second;
      }
    }

    return result;
  }

  static inline size_t GetLeftChildIdx(size_t idx) {
    return 2 * idx + 1;
  }
  static inline size_t GetRightChildIdx(size_t idx) {
    return 2 * idx + 2;
  }
};

int main() {
  size_t n, k, begin, end;

  std::cin >> n;
  std::vector<int32_t> subs(n);

  for (size_t i = 0; i < n; ++i) {
    std::cin >> subs[i];
  }

  auto tree = SegmentTree(subs);
  subs.clear();

  std::cin >> k;

  for (size_t i = 0; i < k; ++i) {
    std::cin >> begin >> end;
    auto max = tree.Max(begin - 1, end - 1);

    std::cout << max.first << ' ' << max.second << '\n';
  }
}