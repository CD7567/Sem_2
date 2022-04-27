#pragma once

#include <cstdint>
#include <algorithm>

class Range {
 public:
  using ValueType = int64_t;

  Range() = delete;
  explicit Range(const int64_t end) : begin_(0), end_((end > begin_) ? end : begin_), step_(1) {};
  Range(const int64_t begin, const int64_t end, const int64_t step = 1) : begin_(begin), end_((end > begin) ? end : begin), step_(step) {};

 private:
 class Iterator : public std::iterator<
                                       std::forward_iterator_tag,
                                       Range::ValueType,
                                       std::ptrdiff_t,
                                       Range::ValueType*,
                                       Range::ValueType&
                                      > {
   public:
    using ValueType = Range::ValueType;

    Iterator(const ValueType begin, const ValueType end, const ValueType step, const ValueType position) : begin_(begin), end_(end), step_(step), position_(position) {};
    //Iterator(const Iterator&) = delete;
    //Iterator(Iterator&&) = default;

    //Iterator& operator=(const Iterator&) = delete;
    //Iterator& operator=(Iterator&&) = default;

    Iterator& operator++() {
      position_ += step_;
      return *this;
    }
    Iterator operator++(int) {
      ++(*this);
      return {begin_, end_, step_, position_ - step_};
    }

    ValueType operator*() const {
      return position_;
    }

    friend bool operator==(const Iterator& lhs, const Iterator& rhs) noexcept {
      return lhs.position_ == rhs.position_;
    }
    friend bool operator!=(const Iterator& lhs, const Iterator& rhs) noexcept {
      return !(lhs == rhs);
    }
    friend bool operator<(const Iterator& lhs, const Iterator& rhs) noexcept {
      return lhs.position_ < rhs.position_;
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
    const ValueType begin_;
    const ValueType end_;
    const ValueType step_;

    ValueType position_;
  };

 public:
  [[nodiscard]] Iterator begin() const {
    return Iterator(begin_, end_, step_, begin_);
  }

  [[nodiscard]] Iterator end() const {
    return Iterator(begin_, end_, step_, end_);
  }

 private:
  const int64_t begin_;
  const int64_t end_;
  const int64_t step_;
};