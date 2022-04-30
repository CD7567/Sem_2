#pragma once

#include <cstdint>
#include <cstddef>
#include <algorithm>
#include <functional>
#include <vector>
#include <forward_list>
#include <iterator>

template <typename KeyT, bool IsConst>
struct Position {
  std::conditional_t<IsConst, const std::forward_list<KeyT>&, std::forward_list<KeyT>&> bucket_;
  typename std::forward_list<KeyT>::const_iterator pre_iterator_;
};

template <typename KeyT>
class UnorderedSet {
 public:
  UnorderedSet() : n_elements_(0){};
  explicit UnorderedSet(size_t count) : n_elements_(0), buckets_({count}) {
  }
  template <typename Iterator,
            typename = std::enable_if_t<std::is_base_of_v<std::forward_iterator_tag,
                                                          typename std::iterator_traits<Iterator>::iterator_category>>>
  UnorderedSet(Iterator first, Iterator last) : n_elements_(0) {
    for (auto it = first; it != last; ++it) {
      Insert(*it);
    }
  }
  UnorderedSet(const UnorderedSet& src) = default;
  UnorderedSet(UnorderedSet&& src) noexcept : buckets_(std::move(src.buckets_)), n_elements_(src.n_elements_) {
    src.n_elements_ = 0;
  }

  ~UnorderedSet() = default;

  UnorderedSet& operator=(const UnorderedSet& src) = default;
  UnorderedSet& operator=(UnorderedSet&& src) noexcept {
    buckets_ = std::move(src.buckets_);
    n_elements_ = src.n_elements_;

    src.n_elements_ = 0;

    return *this;
  }

  [[nodiscard]] size_t Size() const {
    return n_elements_;
  }

  [[nodiscard]] size_t BucketCount() const {
    return buckets_.size();
  }
  [[nodiscard]] size_t BucketSize(size_t id) const {
    size_t result = 0;

    if (id < buckets_.size()) {
      auto& bucket = buckets_[id];
      result = static_cast<size_t>(std::distance(bucket.begin(), bucket.end()));
    }

    return result;
  }
  [[nodiscard]] size_t Bucket(const KeyT& elem) const {
    return (buckets_.empty() ? 0 : kHash(elem) % buckets_.size());
  }

  [[nodiscard]] float LoadFactor() const {
    return (buckets_.empty() ? 0 : static_cast<float>(n_elements_) / static_cast<float>(buckets_.size()));
  }

  [[nodiscard]] bool Empty() const {
    return n_elements_ == 0;
  }

  void Clear() {
    n_elements_ = 0;
    buckets_.clear();
  }

  void Insert(const KeyT& elem) {
    if (buckets_.empty()) {
      buckets_.resize(1);
    }

    auto position = GetPosition(elem);
    auto next_it = position.pre_iterator_;
    ++next_it;

    if (next_it == position.bucket_.end()) {
      position.bucket_.insert_after(position.bucket_.before_begin(), elem);
      ++n_elements_;
    }

    if (LoadFactor() > 1.0f) {
      Reserve(2 * buckets_.size());
    }
  }

  [[nodiscard]] bool Find(const KeyT& elem) const {
    auto result = false;

    if (!Empty()) {
      auto position = GetPosition(elem);
      result = ++position.pre_iterator_ != position.bucket_.end();
    }

    return result;
  }

  void Erase(const KeyT& elem) {
    auto position = GetPosition(elem);
    auto next_it = position.pre_iterator_;
    ++next_it;

    if (next_it != position.bucket_.end()) {
      position.bucket_.erase_after(position.pre_iterator_);
      --n_elements_;
    }
  }

  void Reserve(size_t new_bucket_count) {
    if (new_bucket_count > buckets_.size()) {
      buckets_.resize(new_bucket_count);

      for (size_t src_list_idx = 0; src_list_idx < buckets_.size(); ++src_list_idx) {
        auto next_it = ++buckets_[src_list_idx].before_begin();

        for (auto it = buckets_[src_list_idx].before_begin(); next_it != buckets_[src_list_idx].end();) {
          auto idx = Bucket(*next_it);

          if (idx != src_list_idx) {
            buckets_[idx].splice_after(buckets_[idx].before_begin(), buckets_[src_list_idx], it);
            next_it = it;
            ++next_it;
          } else {
            ++it;
            ++next_it;
          }
        }
      }
    }
  }
  void Rehash(size_t new_bucket_count) {
    if (new_bucket_count != buckets_.size() &&
        (static_cast<float>(n_elements_) / static_cast<float>(new_bucket_count)) <= 1.0f) {
      if (new_bucket_count > buckets_.size()) {
        Reserve(new_bucket_count);
      } else {
        for (size_t src_list_idx = 0; src_list_idx < buckets_.size(); ++src_list_idx) {
          auto next_it = ++buckets_[src_list_idx].before_begin();
          for (auto it = buckets_[src_list_idx].before_begin(); next_it != buckets_[src_list_idx].end();) {
            auto idx = kHash(*next_it) % new_bucket_count;

            if (idx != src_list_idx) {
              buckets_[idx].splice_after(buckets_[idx].before_begin(), buckets_[src_list_idx], it);
            } else {
              ++it;
              ++next_it;
            }
          }
        }

        buckets_.resize(new_bucket_count);
        buckets_.shrink_to_fit();
      }
    }
  }

 private:
  inline Position<KeyT, false> GetPosition(const KeyT& elem) {
    auto idx = Bucket(elem);
    auto& bucket = buckets_[Bucket(elem)];
    auto it = bucket.before_begin();
    auto next_it = it;
    ++next_it;

    for (; next_it != bucket.end(); ++it, ++next_it) {
      if (*next_it == elem) {
        break;
      }
    }

    return {bucket, it};
  }
  inline Position<KeyT, true> GetPosition(const KeyT& elem) const {
    auto& bucket = buckets_[Bucket(elem)];
    auto it = bucket.before_begin();
    auto next_it = it;
    ++next_it;

    for (; next_it != bucket.end(); ++it, ++next_it) {
      if (*next_it == elem) {
        break;
      }
    }

    return {bucket, it};
  }

  size_t n_elements_;
  static constexpr std::hash<KeyT> kHash{};

  std::vector<std::forward_list<KeyT>> buckets_;
};