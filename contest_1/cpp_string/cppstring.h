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
  String();
  explicit String(size_t beg_size, char symbol);
  String(const char* cstr);  //  NOLINT
  String(const char* cstr, size_t upper_bound);
  String(const String& src);

  ~String();

  [[nodiscard]] size_t Size() const;
  [[nodiscard]] size_t Length() const;
  [[nodiscard]] size_t Capacity() const;

  char* CStr();
  char* Data();
  [[nodiscard]] const char* CStr() const;
  [[nodiscard]] const char* Data() const;

  [[nodiscard]] bool Empty() const;

  void Swap(String&);

  void PushBack(char);
  char PopBack();
  void Clear();

  void Reserve(size_t);
  void Resize(size_t, char);
  void ShrinkToFit();

  char& At(size_t idx);
  char& Front();
  char& Back();
  [[nodiscard]] const char& At(size_t idx) const;
  [[nodiscard]] const char& Front() const;
  [[nodiscard]] const char& Back() const;

  char& operator[](size_t idx) {
    return buffer_[idx];
  }
  const char& operator[](size_t idx) const {
    return buffer_[idx];
  }

  String& operator=(const String& src) {
    if (this != &src) {
      size_ = src.size_;
      capacity_ = src.capacity_;
      delete[] buffer_;

      buffer_ = new char[capacity_ + 1];

      for (size_t i = 0; i < size_; ++i) {
        buffer_[i] = src.buffer_[i];
      }

      buffer_[size_] = '\0';
    }

    return *this;
  }

  friend String operator+(const String& lhs, const String& rhs);
  friend String& operator+=(String& lhs, const String& rhs);

  friend bool operator==(const String&, const String&);
  friend bool operator!=(const String&, const String&);
  friend bool operator>(const String&, const String&);
  friend bool operator>=(const String&, const String&);
  friend bool operator<(const String&, const String&);
  friend bool operator<=(const String&, const String&);

  friend std::ostream& operator<<(std::ostream&, const String&);

 private:
  size_t size_;
  size_t capacity_;
  char* buffer_;

  static size_t CStrLen(const char* cstr);
};