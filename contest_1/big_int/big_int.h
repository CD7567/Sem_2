#pragma once

#include <iostream>
#include <cstdint>

class BigInteger {
 public:
  BigInteger() = default;

  explicit BigInteger(const char *);

  BigInteger(long long); // NOLINT
  BigInteger(const BigInteger &);

  BigInteger &operator=(const BigInteger &) = default;

  friend BigInteger operator+(const BigInteger &);

  friend BigInteger operator-(const BigInteger &);

  friend BigInteger operator+(const BigInteger &, const BigInteger &);

  friend BigInteger operator-(const BigInteger &, const BigInteger &);

  friend std::istream &operator>>(std::istream &in, BigInteger &);

  friend std::ostream &operator<<(std::ostream &out, const BigInteger &num);

  friend bool operator==(const BigInteger &lhs, const BigInteger &rhs);

  [[nodiscard]]
  bool IsNegative() const {
    return is_negative_;
  }

  [[deprecated]]
  static void Info(BigInteger &);

  void Invert() {
    is_negative_ = !is_negative_;
  }

  template<typename T>
  class Buffer {
   public:
    explicit Buffer(size_t beg_size);

    Buffer(const Buffer &);

    ~Buffer() {
      delete[] array_;
    };

    [[nodiscard]] size_t GetSize() const {
      return size_;
    }

    [[nodiscard]] size_t GetContainerSize() const {
      return container_size_;
    };

    void SetSize(size_t new_size) {
      size_ = new_size;
    }

    T *Begin() {
      return array_;
    }

    T *End() {
      return array_ + size_;
    }

    [[nodiscard]] const T *Begin() const {
      return array_;
    }

    [[nodiscard]] const T *End() const {
      return array_ + size_;
    }

    T &operator[](size_t idx) {
      return array_[idx];
    };

    const T &operator[](size_t idx) const {
      return array_[idx];
    };

    Buffer &operator=(const Buffer &src) {
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

    void Resize(size_t new_size) {
      size_t upper_bound = std::min(new_size, size_);
      T *new_array = new T[new_size];

      for (size_t i = 0; i < upper_bound; ++i) {
        new_array[i] = array_[i];
      }

      delete[] array_;
      array_ = new_array;
      size_ = new_size;
      container_size_ = new_size;
    }

   private:
    T *array_;
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

  static void RawSum(const BigInteger &, const BigInteger &, BigInteger &);

  static void RawSubtract(const BigInteger &, const BigInteger &, BigInteger &);
};