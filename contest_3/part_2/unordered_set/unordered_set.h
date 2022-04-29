#pragma once

#include <cstdint>
#include <cstddef>
#include <algorithm>
#include <functional>
#include <vector>
#include <forward_list>
#include <iterator>

template <typename KeyT>
class UnorderedSet {
 private:
  using iterator = typename std::forward_list<KeyT>::const_iterator;

 public:
  UnorderedSet() : n_elements_(0), n_buckets_(0) {}
  explicit UnorderedSet(size_t count) : n_elements_(0), n_buckets_(count) {
    buckets_.reserve(count);

    for (size_t i = 0; i < count; ++i) {
      buckets_.template emplace_back(list_.before_begin(), list_.before_begin());
    }
  }
  UnorderedSet(const UnorderedSet&) = default;
  UnorderedSet(UnorderedSet&&)  noexcept = default;

  UnorderedSet& operator=(const UnorderedSet&) = default;
  UnorderedSet& operator=(UnorderedSet&&)  noexcept = default;

  [[nodiscard]] size_t Size() const {
    return n_elements_;
  }

 private:
  size_t n_elements_;
  size_t n_buckets_;
  static constexpr std::hash<KeyT> hash_{};

  std::vector<std::pair<iterator, iterator>> buckets_;
  std::forward_list<KeyT> list_;
};