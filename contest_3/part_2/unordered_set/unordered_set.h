#pragma once

#include <cstdint>
#include <cstddef>
#include <algorithm>
#include <functional>
#include <vector>
#include <forward_list>

template <typename KeyT>
class UnorderedSet {
 public:
  UnorderedSet() : n_elements_(0) {};
  explicit UnorderedSet(size_t count) : n_elements_(0) {
    buckets_.reserve(count);

    for (size_t i = 0; i < count; ++i) {
      buckets_.push_back(std::forward_list<KeyT>());
    }
  };

  template <typename Iterator,
          typename = std::enable_if_t<std::is_base_of_v<std::forward_iterator_tag,
                  typename std::iterator_traits<Iterator>::iterator_category>>>
  UnorderedSet(Iterator first, Iterator last) : n_elements_(0) {
    for (auto it = first; it != last; ++it) {
      this->Insert(*it);
    }
  }

  UnorderedSet(const UnorderedSet<KeyT>& other) = default;
  UnorderedSet(UnorderedSet&& src) noexcept {
    buckets_ = std::move(src.buckets_);
    n_elements_ = src.n_elements_;
    src.n_elements_ = 0;
  }
  ~UnorderedSet() = default;

  UnorderedSet& operator=(const UnorderedSet&) = default;
  UnorderedSet& operator=(UnorderedSet&& src) noexcept {
    buckets_ = std::move(src.buckets_);
    n_elements_ = src.n_elements_;
    src.n_elements_ = 0;

    return *this;
  }

  [[nodiscard]] size_t BucketCount() const {
    return buckets_.size();
  }
  [[nodiscard]] size_t BucketSize(size_t id) const {
    size_t size = 0;

    if (buckets_.capacity() > id) {
      for (auto it = buckets_[id].begin(); it != buckets_[id].end(); ++it, ++size) {
      }
    }

    return size;
  }

  [[nodiscard]] size_t Bucket(KeyT key) const {
    std::hash<KeyT> hash;

    return (buckets_.empty() ? 0 : hash(key) % buckets_.size());
  }

  [[nodiscard]] size_t Size() const {
    return n_elements_;
  }

  [[nodiscard]] float LoadFactor() const {
    return (this->Empty() ? 0 : static_cast<float>(n_elements_) / static_cast<float>(buckets_.size()));
  }

  [[nodiscard]] bool Empty() const {
    return n_elements_ == 0;
  }

  void Clear() {
    buckets_.clear();
    n_elements_ = 0;
  }

  bool Find(const KeyT& value) const {
    bool answer = false;

    if (!buckets_.empty()) {
      auto& bucket = buckets_[Bucket(value)];

      for (auto it = bucket.begin(); it != bucket.end() && !answer; ++it) {
        answer |= (value == *it);
      }
    }

    return answer;
  }

  void Insert(const KeyT& value) {
    if (n_elements_ == buckets_.size()) {
      this->Reserve((buckets_.empty() ? 1 : 2 * buckets_.size()));
    }

    if (!this->Find(value)) {
      auto& bucket = buckets_[Bucket(value)];

      bucket.insert_after(bucket.before_begin(), value);
    }

    ++n_elements_;
  }

  void Erase(const KeyT& value) {
    if (!buckets_.empty()) {
      auto& bucket = buckets_[Bucket(value)];
      auto before_it = bucket.before_begin();
      auto it = bucket.begin();

      for (; it != bucket.end(); ++it, ++before_it) {
        if (*it == value) {
          break;
        }
      }

      if (it != bucket.end()) {
        bucket.erase_after(before_it);
        --n_elements_;
      }
    }
  }

  void Reserve(size_t new_bucket_count) {
    if (buckets_.size() < new_bucket_count) {
      buckets_.resize(new_bucket_count);
      for (size_t bucket_idx = 0; bucket_idx < buckets_.size(); ++bucket_idx) {
        auto it = buckets_[bucket_idx].before_begin();
        auto real_it = it;
        ++real_it;
        while (real_it != buckets_[bucket_idx].end()) {
          auto right_idx = Bucket(*(real_it));
          if (right_idx == bucket_idx) {
            ++it;
            ++real_it;
          } else {
            buckets_[right_idx].splice_after(buckets_[right_idx].before_begin(), buckets_[bucket_idx], it);
            real_it = it;
            ++real_it;
          }
        }
      }
    }
  }

  void Rehash(size_t new_bucket_count) {
    if (new_bucket_count > buckets_.size() && new_bucket_count >= n_elements_) {
      this->Reserve(new_bucket_count);
    } else if (new_bucket_count < buckets_.size() & new_bucket_count >= n_elements_) {
      for (size_t bucket_idx = 0; bucket_idx < buckets_.size(); ++bucket_idx) {
        auto it = buckets_[bucket_idx].before_begin();
        auto real_it = it;
        ++real_it;
        while (real_it != buckets_[bucket_idx].end()) {
          std::hash<KeyT> hash;
          auto right_idx = hash(*(real_it)) % new_bucket_count;
          if (right_idx == bucket_idx) {
            ++it;
            ++real_it;
          } else {
            buckets_[right_idx].splice_after(buckets_[right_idx].before_begin(), buckets_[bucket_idx], it);
            real_it = it;
            ++real_it;
          }
        }
      }
      auto delta = buckets_.size() - new_bucket_count;
      for (size_t i = 0; i < delta; ++i) {
        buckets_.pop_back();
      }
    }
  }

 private:
  std::vector<std::forward_list<KeyT>> buckets_;
  size_t n_elements_;
};