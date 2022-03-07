#pragma once

#include <stdexcept>
#include <cstdint>

class StringViewOutOfRange : public std::out_of_range {
 public:
  StringViewOutOfRange() : std::out_of_range("StringViewOutOfRange") {
  }
};

class StringView {
 public:
  StringView() : str_(nullptr), size_(0) {
  }
  StringView(const char* cstr) : str_(cstr), size_(StrLen(cstr)) {  //  NOLINT
  }
  StringView(const char* cstr, size_t size) : str_(cstr), size_(size) {
  }

  [[nodiscard]] const char& At(size_t idx) const {
    if (idx >= size_) {
      throw StringViewOutOfRange();
    } else {
      return str_[idx];
    }
  }
  [[nodiscard]] const char& operator[](size_t idx) const {
    return str_[idx];
  }

  [[nodiscard]] const char& Front() const {
    return str_[0];
  }
  [[nodiscard]] const char& Back() const {
    return str_[size_ - 1];
  }
  [[nodiscard]] const char* Data() const {
    return str_;
  }

  [[nodiscard]] size_t Size() const {
    return size_;
  }
  [[nodiscard]] size_t Length() const {
    return size_;
  }

  [[nodiscard]] bool Empty() const {
    return size_ == 0;
  }

  void Swap(StringView& other) {
    Swap(str_, other.str_);
    Swap(size_, other.size_);
  }

  void RemovePrefix(size_t prefix_size) {
    str_ += prefix_size;
    size_ -= prefix_size;
  }
  void RemoveSuffix(size_t suffix_size) {
    size_ -= suffix_size;
  }

  StringView Substr(size_t pos, size_t count = -1) {
    if (pos >= size_) {
      throw StringViewOutOfRange();
    } else {
      return {str_ + pos, Min(count, size_ - pos)};
    }
  }

 private:
  const char* str_;
  size_t size_;

  static size_t StrLen(const char* cstr) {
    const char* it = cstr;

    while (*it != '\0') {
      ++it;
    }

    return it - cstr;
  }

  static size_t Min(size_t first, size_t second) {
    return ((first < second) ? first : second);
  }

  template <typename T>
  static void Swap(T& first, T& second) {
    T tmp = first;
    first = second;
    second = tmp;
  }
};