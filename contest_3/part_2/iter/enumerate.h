#pragma once

//#define TEMPORARY_ENUMERATE_IMPLEMENTED

#include <cstdint>
#include <algorithm>

template <typename Collection,
          typename CollIterator = std::conditional_t<std::is_const_v<Collection>, typename Collection::const_iterator,
                                                     typename Collection::iterator>>
class Enumerate {
 public:
  using ItValueType = std::pair<size_t, typename CollIterator::reference>;

 private:
  class Iterator
      : public std::iterator<std::forward_iterator_tag, ItValueType, std::ptrdiff_t, ItValueType*, ItValueType&> {
   public:
    Iterator() = delete;
    Iterator(const CollIterator& it, size_t idx) : idx_(idx), iterator_(it){};
    Iterator(const Iterator&) = default;
    Iterator(Iterator&&) noexcept = default;

    ~Iterator() = default;

    Iterator& operator=(const Iterator&) = default;
    Iterator& operator=(Iterator&&) noexcept = default;

    Iterator& operator++() {
      ++idx_;
      ++iterator_;
      return *this;
    }
    Iterator operator++(int) {
      auto last_iter = iterator_;
      ++idx_;
      ++iterator_;

      return {last_iter, idx_ - 1};
    }

    ItValueType operator*() const {
      return {idx_, *iterator_};
    }

    friend bool operator==(const Iterator& lhs, const Iterator& rhs) noexcept {
      return lhs.idx_ == rhs.idx_;
    }
    friend bool operator!=(const Iterator& lhs, const Iterator& rhs) noexcept {
      return !(lhs == rhs);
    }
    friend bool operator<(const Iterator& lhs, const Iterator& rhs) noexcept {
      return lhs.idx_ < rhs.idx_;
    }
    friend bool operator>(const Iterator& lhs, const Iterator& rhs) noexcept {
      return rhs < lhs;
    }
    friend bool operator<=(const Iterator& lhs, const Iterator& rhs) noexcept {
      return !(rhs < lhs);
    }
    friend bool operator>=(const Iterator& lhs, const Iterator& rhs) noexcept {
      return !(lhs < rhs);
    }

   private:
    size_t idx_;
    CollIterator iterator_;
  };

 public:
  using iterator = Iterator;                   //  NOLINT
  using const_iterator = Iterator;             //  NOLINT
  using reference = ItValueType&;              //  NOLINT
  using const_reference = const ItValueType&;  //  NOLINT

  Enumerate() = delete;
  explicit Enumerate(Collection& coll) : temp_collection_(), begin_(coll.begin()), end_(coll.end()) {
  }
  explicit Enumerate(Collection&& coll)
      : temp_collection_(coll), begin_(temp_collection_.begin()), end_(temp_collection_.end()) {
  }
  Enumerate(Enumerate&) = delete;
  Enumerate(Enumerate&&) = delete;

  ~Enumerate() = default;

  Iterator begin() const {  //  NOLINT
    return {begin_, 0};
  }

  Iterator end() const {  //  NOLINT
    return {end_, static_cast<size_t>(std::distance(begin_, end_))};
  }

 private:
  Collection temp_collection_;
  CollIterator begin_;
  CollIterator end_;
};