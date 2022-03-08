#pragma once

#include <iostream>
#include <cstdint>
#include <stdexcept>

class BigIntegerOverflow : public std::runtime_error {
 public:
  BigIntegerOverflow() : std::runtime_error("BigIntegerOverflow") {
  }
};

class BigIntegerDivisionByZero : public std::runtime_error {
 public:
  BigIntegerDivisionByZero() : std::runtime_error("BigIntegerDivisionByZero") {
  }
};

class BigInteger {
 public:
  BigInteger() = default;
  explicit BigInteger(const char*);
  explicit BigInteger(int32_t);
  BigInteger(int64_t);  //  NOLINT
  BigInteger(const BigInteger&);

  BigInteger& operator=(const BigInteger&) = default;

  friend BigInteger operator+(const BigInteger&);
  friend BigInteger operator-(const BigInteger&);

  friend BigInteger& operator++(BigInteger&);
  friend BigInteger& operator--(BigInteger&);
  friend BigInteger operator++(BigInteger&, int);
  friend BigInteger operator--(BigInteger&, int);

  friend BigInteger operator+(const BigInteger&, const BigInteger&);
  friend BigInteger operator-(const BigInteger&, const BigInteger&);
  friend BigInteger operator*(const BigInteger&, const BigInteger&);
  friend BigInteger& operator+=(BigInteger&, const BigInteger&);
  friend BigInteger& operator-=(BigInteger&, const BigInteger&);
  friend BigInteger& operator*=(BigInteger&, const BigInteger&);

  friend std::istream& operator>>(std::istream& in, BigInteger&);
  friend std::ostream& operator<<(std::ostream& out, const BigInteger& num);

  friend bool operator==(const BigInteger& lhs, const BigInteger& rhs);
  friend bool operator!=(const BigInteger& lhs, const BigInteger& rhs);
  friend bool operator<(const BigInteger& lhs, const BigInteger& rhs);
  friend bool operator>(const BigInteger& lhs, const BigInteger& rhs);
  friend bool operator<=(const BigInteger& lhs, const BigInteger& rhs);
  friend bool operator>=(const BigInteger& lhs, const BigInteger& rhs);

  explicit operator bool() const {
    return (buffer_.GetSize() > 1) || (buffer_[0] != 0);
  }

  [[nodiscard]]
  bool IsNegative() const;

  template <typename T>
  class Buffer {
   public:
    explicit Buffer(size_t beg_size);
    Buffer(const Buffer&);

    ~Buffer();

    [[nodiscard]] size_t GetSize() const;
    [[nodiscard]] size_t GetContainerSize() const;

    void SetSize(size_t new_size);

    T* Begin();
    T* End();
    [[nodiscard]] const T* Begin() const;
    [[nodiscard]] const T* End() const;

    T& operator[](size_t idx);
    const T& operator[](size_t idx) const;

    Buffer& operator=(const Buffer& src) {
      delete[] array_;
      size_ = src.size_;
      container_size_ = src.container_size_;

      array_ = new T[container_size_];

      for (int i = 0; i < size_; ++i) {
        array_[i] = src.array_[i];
      }

      return *this;
    }

    void PushBack(T elem);
    void Resize(size_t new_size);

   private:
    T* array_;
    size_t size_ = 0;
    size_t container_size_;
  };

 private:
  const static int32_t kBase = 10000;
  const static size_t kBaseDecLen = 4;

  bool is_negative_ = false;
  Buffer<int32_t> buffer_ = Buffer<int32_t>(2);

  void Normalize();
  void CheckSign();

  static void RawSum(const BigInteger&, const BigInteger&, BigInteger&);
  static void RawSubtract(const BigInteger&, const BigInteger&, BigInteger&);
  static void RawMultiply(const BigInteger&, const BigInteger&, BigInteger&);
};