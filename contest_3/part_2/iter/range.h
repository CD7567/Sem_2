#pragma once

#define REVERSE_RANGE_IMPLEMENTED

#include <cstdint>
#include <algorithm>

class Range {
 public:
  Range() = delete;
  explicit Range(const int64_t end) : begin_(0), end_((end > begin_) ? end : begin_), step_(1) {
    end_ += (step_ - (end_ - begin_) % step_) % step_;
  };
  Range(const int64_t begin, const int64_t end, const int64_t step = 1) : begin_(begin), step_(step) {
    if (step != 0) {
      if (step > 0) {
        end_ = (begin < end ? end : begin);
      } else {
        end_ = (begin > end ? end : begin);
      }

      end_ += (step_ - (end_ - begin_) % step_) % step_;
    } else {
      end_ = begin_;
    }
  };

 private:
  class Iterator : public std::iterator<std::forward_iterator_tag, int64_t, std::ptrdiff_t, int64_t*, int64_t&> {
   public:
    using ValueType = int64_t;

    Iterator(const ValueType begin, const ValueType end, const ValueType step, const ValueType position)
            : begin_(begin), end_(end), step_(step), position_(position){};
    Iterator(const Iterator&) = delete;
    Iterator(Iterator&&) = delete;

    Iterator& operator=(const Iterator&) = delete;
    Iterator& operator=(Iterator&&) = delete;

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
  [[nodiscard]] Iterator begin() const {  //  NOLINT
    return {begin_, end_, step_, begin_};
  }

  [[nodiscard]] Iterator end() const {  //  NOLINT
    return {begin_, end_, step_, end_};
  }

  [[nodiscard]] Iterator rbegin() const {  //  NOLINT
    return {end_, begin_, -step_, end_ - step_};
  }

  [[nodiscard]] Iterator rend() const {  //  NOLINT
    return {end_, begin_, -step_, begin_ - step_};
  }

 private:
  int64_t begin_;
  int64_t end_;
  int64_t step_;
};