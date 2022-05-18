#include <iostream>
#include <vector>
#include <cstdint>

class SegmentTree {
 private:
  struct NodeValueType {
    int32_t value = INT32_MIN;
    size_t amount = 1;
    int32_t promise = 0;
  };

 public:
  explicit SegmentTree(const std::vector<int32_t>& src) : size_(BinTop(src.size())), tree_(size_) {
    size_t leaves_shift = size_ / 2;
    size_t src_size = src.size();

    for (size_t i = 0; i < src_size; ++i) {
      tree_[i + leaves_shift].value = src[i];
    }

    for (size_t i = leaves_shift - 1; i < leaves_shift; --i) {
      tree_[i] = GetMax(tree_[GetLeftChildIdx(i)], tree_[GetRightChildIdx(i)]);
    }
  }

  NodeValueType Max(size_t begin, size_t end) {
    auto max = NodeMax(begin, end, 0, 0, size_ / 2);

    max.value += max.promise;
    max.promise = 0;

    return max;
  }

  void Update(size_t begin, size_t end, int32_t delta) {
    NodeUpdate(begin, end, 0, 0, size_ / 2, delta);
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
  static inline NodeValueType GetMax(const NodeValueType& lhs, const NodeValueType& rhs) {
    NodeValueType result;

    if (lhs.value + lhs.promise > rhs.value + rhs.promise) {
      result = lhs;
    } else if (lhs.value + lhs.promise < rhs.value + rhs.promise) {
      result = rhs;
    } else {
      result.value = lhs.value;
      result.amount = lhs.amount + rhs.amount;
      result.promise = lhs.promise;
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
      size_t left_idx = GetLeftChildIdx(node);
      size_t right_idx = GetRightChildIdx(node);
      int32_t promise = tree_[node].promise;

      tree_[left_idx].promise += promise;
      tree_[right_idx].promise += promise;
      tree_[node].value += promise;
      tree_[node].promise = 0;

      auto left_max = NodeMax(begin, end, GetLeftChildIdx(node), max_begin, (max_begin + max_end) / 2);
      auto right_max = NodeMax(begin, end, GetRightChildIdx(node), (max_begin + max_end) / 2 + 1, max_end);

      result = GetMax(left_max, right_max);
    }

    return result;
  }

  void NodeUpdate(size_t begin, size_t end, size_t node, size_t upd_begin, size_t upd_end, int32_t delta) {
    if (begin <= upd_begin && end >= upd_end) {
      tree_[node].promise += delta;
    } else if (begin <= upd_end && end >= upd_begin ) {
      NodeUpdate(begin, end, GetLeftChildIdx(node), upd_begin, (upd_begin + upd_end) / 2, delta);
      NodeUpdate(begin, end, GetRightChildIdx(node), (upd_begin + upd_end) / 2 + 1, upd_end, delta);
    }
  }
};

int main() {
  size_t n, m, begin, end;
  int32_t delta;
  char command;

  std::cin >> n;
  std::vector<int32_t> honey(n);

  for (size_t i = 0; i < n; ++i) {
    std::cin >> honey[i];
  }

  auto tree = SegmentTree(honey);
  honey.clear();

  std::cin >> m;

  for (size_t i = 0; i < m; ++i) {
    std::cin >> command >> begin >> end;

    switch (command) {
      case 'a':
        std::cin >> delta;
        tree.Update(begin - 1, end - 1, delta);
        break;

      case 'm':
        auto max = tree.Max(begin - 1, end - 1);
        std::cout << max.value << ' ';
        break;
    }
  }
}