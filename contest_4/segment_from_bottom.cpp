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
      tree_[i] = GetMax(tree_[GetLeftChildIdx(i)], tree_[GetRightChildIdx(i)]);
    }
  }

  std::pair<int64_t, size_t> Max(size_t left, size_t right) {
    std::pair<int32_t, size_t> result(INT32_MIN, 0);

    left += size_ / 2;
    right += size_ / 2;

    while (left < right) {
      if (left == GetRightChildIdx(GetParentIdx(left))) {
        result = GetMax(result, tree_[left]);
      }

      left = GetParentIdx(left + 1);

      if (right == GetLeftChildIdx(GetParentIdx(right))) {
        result = GetMax(result, tree_[right]);
      }

      right = GetParentIdx(right - 1);
    }

    if (left == right) {
      result = GetMax(result, tree_[left]);
    }

    return result;
  }

 private:
  size_t size_;
  std::vector<std::pair<int32_t, size_t>> tree_;

  static inline size_t GetLeftChildIdx(size_t idx) {
    return 2 * idx + 1;
  }
  static inline size_t GetRightChildIdx(size_t idx) {
    return 2 * idx + 2;
  }
  static inline size_t GetParentIdx(size_t idx) {
    return (idx - 1) / 2;
  }

  static std::pair<int32_t, size_t> GetMax(const std::pair<int32_t, size_t>& lhs,
                                           const std::pair<int32_t, size_t>& rhs) {
    std::pair<int32_t, size_t> answer;

    if (lhs.first > rhs.first) {
      answer = lhs;
    } else if (lhs.first < rhs.first) {
      answer = rhs;
    } else {
      answer = std::make_pair(lhs.first, lhs.second + rhs.second);
    }

    return answer;
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