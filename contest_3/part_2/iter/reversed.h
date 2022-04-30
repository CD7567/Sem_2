#pragma once

#define TEMPORARY_ENUMERATE_IMPLEMENTED

#include <cstdint>
#include <algorithm>

template <typename Collection, typename CollIterator = std::conditional_t<std::is_const_v<Collection>, typename Collection::const_iterator, typename Collection::iterator>>
class Reversed {
 public:
  using value_type = std::conditional_t<std::is_const_v<Collection>, const typename Collection::value_type, typename Collection::value_type>;  //  NOLINT
  using reference = value_type&;  //  NOLINT
  using const_reference = const value_type&;  //  NOLINT

 private:
  class Iterator : public std::iterator<std::forward_iterator_tag, value_type , std::ptrdiff_t, value_type*, reference> {
   public:
    Iterator() = delete;
    Iterator(const CollIterator& it) : iterator_(it){};
    Iterator(const Iterator&) = default;
    Iterator(Iterator&&) noexcept = default;

    ~Iterator() = default;

    Iterator& operator=(const Iterator&) = default;
    Iterator& operator=(Iterator&&)  noexcept = default;

    Iterator& operator++() {
      ++iterator_;
      return *this;
    }
    Iterator operator++(int) {
      auto last_iter = iterator_;
      ++iterator_;

      return {last_iter};
    }

    reference operator*() const {
      return *iterator_;
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
    CollIterator iterator_;
  };

 public:
  using iterator = Iterator;  //  NOLINT

  Reversed() = delete;
  explicit Reversed(Collection& coll) : temp_collection_(), begin_(coll.begin()), end_(coll.end()) {}
  explicit Reversed(Collection&& coll) : temp_collection_(coll), begin_(temp_collection_.begin()), end_(temp_collection_.end()) {}
  Reversed(Reversed&) = delete;
  Reversed(Reversed&&) = delete;

  ~Reversed() = default;

  Iterator begin() const {
    return {end_};
  }

  Iterator end() const {
    return {end_};
  }

 private:
  Collection temp_collection_;
  CollIterator begin_;
  CollIterator end_;
};