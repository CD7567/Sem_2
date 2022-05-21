#include <iostream>
#include <vector>
#include <cstdint>

class SegmentTree {
 private:
  using NodeValueType = typename std::pair<int32_t, size_t>;

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

  NodeValueType Max(size_t begin, size_t end) {
    return NodeMax(begin, end, 0, 0, size_ / 2);
  }

 private:
  size_t size_;
  std::vector<NodeValueType> tree_;

  static inline size_t GetLeftChildIdx(size_t idx) {
    return 2 * idx + 1;
  }
  static inline size_t GetRightChildIdx(size_t idx) {
    return 2 * idx + 2;
  }
  static inline std::pair<int32_t, size_t> GetMax(const NodeValueType& lhs, const NodeValueType& rhs) {
    NodeValueType result;

    if (lhs.first > rhs.first) {
      result = lhs;
    } else if (lhs.first < rhs.first) {
      result = rhs;
    } else {
      result.first = lhs.first;
      result.second = lhs.second + rhs.second;
    }

    return result;
  }

  static inline size_t Log(size_t num) {
    return 31 - __builtin_clz(num);
  }
  static inline size_t BinTop(size_t size) {
    size_t top = 1 << Log(size);

    top <<= ((top < size) ? 2 : 1);

    return top - 1;
  }

  NodeValueType NodeMax(size_t begin, size_t end, size_t node, size_t max_begin, size_t max_end) {
    NodeValueType result;

    if (begin <= max_begin && end >= max_end) {
      result = tree_[node];
    } else if (max_end < begin || max_begin > end) {
      result = {INT32_MIN, 0};
    } else {
      auto left_max = NodeMax(begin, end, GetLeftChildIdx(node), max_begin, (max_begin + max_end) / 2);
      auto right_max = NodeMax(begin, end, GetRightChildIdx(node), (max_begin + max_end) / 2 + 1, max_end);

      result = GetMax(left_max, right_max);
    }

    return result;
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