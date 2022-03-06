#pragma once

#include <cstdint>
#include <iostream>
#include <stdexcept>

class StringOutOfRange : public std::out_of_range {
 public:
  StringOutOfRange() : std::out_of_range("StringOutOfRange") {
  }
};

class String {
 public:
  String() : size_(0), buffer_size_(0), buffer_(nullptr){};
  explicit String(size_t beg_size, char symbol = '\0') : size_(beg_size), buffer_size_(beg_size), buffer_(new char[beg_size]) {
    for (size_t i = 0; i < size_; ++i) {
      buffer_[i] = symbol;
    }
  }
  String(const char* cstr) { // NOLINT
    size_t cstr_size = CStrLen(cstr);
    size_ = cstr_size;
    buffer_size_ = cstr_size;

    buffer_ = new char[cstr_size];
    for (int i = 0; i < cstr_size; ++i) {
      buffer_[i] = cstr[i];
    }
  }
  String(const char* cstr, size_t upper_bound) {
    size_ = upper_bound;
    buffer_size_ = upper_bound;

    buffer_ = new char[upper_bound];
    for (int i = 0; i < upper_bound; ++i) {
      buffer_[i] = cstr[i];
    }
  }
  String(const String& src) : size_(src.size_), buffer_size_(src.buffer_size_), buffer_(new char[src.buffer_size_]) {
    for (size_t i = 0; i < size_; ++i) {
      buffer_[i] = src.buffer_[i];
    }
  }

  ~String() {
    delete[] buffer_;
  }

  [[nodiscard]] size_t Size() const {
    return size_;
  }
  [[nodiscard]] size_t Length() const {
    return size_;
  }
  [[nodiscard]] size_t Capacity() const {
    return buffer_size_;
  }

  [[nodiscard]] char* CStr() const;
  [[nodiscard]] char* Data() const;

  [[nodiscard]] bool Empty() const {
    return (size_ == 0);
  }

  void Clear() {
    size_ = 0;
  }

  void Swap(String&);

  // Needs guard?
  char PopBack() {
    return buffer_[size_--];
  }
  void PushBack(char);

  void Reserve(size_t);
  void Resize(size_t, char);
  void ShrinkToFit();

  char& At(size_t idx) {
    if (idx >= size_) {
      throw StringOutOfRange();
    } else {
      return buffer_[idx];
    }
  }
  [[nodiscard]] const char& At(size_t idx) const {
    if (idx >= size_) {
      throw StringOutOfRange();
    } else {
      return buffer_[idx];
    }  }
  char& Front() {
    return buffer_[0];
  }
  [[nodiscard]] const char& Front() const {
    return buffer_[0];
  }
  char& Back() {
    return buffer_[size_ - 1];
  }
  [[nodiscard]] const char& Back() const {
    return buffer_[size_ - 1];
  }

  String& operator=(const String& src) {
    if (this != &src) {
      size_ = src.size_;
      buffer_size_ = src.buffer_size_;

      delete[] this->buffer_;
      this->buffer_ = new char[buffer_size_];

      for (size_t i = 0; i < size_; ++i) {
        this->buffer_[i] = src.buffer_[i];
      }
    }

    return *this;
  }

  char& operator[](size_t idx) {
    return buffer_[idx];
  }
  const char& operator[](size_t idx) const {
    return buffer_[idx];
  }

  friend String operator+(const String& lhs, const String& rhs);
  friend String& operator+=(String& lhs, const String& rhs);

  friend std::ostream& operator<<(std::ostream&, const String&);

 private:
  size_t size_;
  size_t buffer_size_;
  char* buffer_;

  static size_t CStrLen(const char* cstr);
};