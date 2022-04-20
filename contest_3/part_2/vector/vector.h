#pragma once

#include <cstdint>
#include <algorithm>

template <typename T>
class Vector {
 public:
  using ValueType = T;
  using Pointer = ValueType*;
  using ConstPointer = const ValueType*;
  using Reference = ValueType&;
  using ConstReference = const ValueType&;
  using SizeType = uint64_t;

 private:
  template <bool IsConst>
  class RawIterator {
   public:
    using DifferenceType = std::ptrdiff_t;
    using ItPointer = typename std::conditional_t<IsConst, ConstPointer, Pointer>;
    using ItReference = typename std::conditional_t<IsConst, ConstReference, Reference>;

    RawIterator() noexcept;
    explicit RawIterator(ItPointer ptr) noexcept;
    RawIterator(const RawIterator<IsConst>& src) noexcept;
    /*
    template <typename = typename std::enable_if<IsConst>>
    explicit RawIterator(const RawIterator<false>& src) noexcept;

    /*
    RawIterator& operator=(const RawIterator& src) noexcept {
      if (this != &src) {
        ptr_ = src.ptr_;
      }

      return *this;
    }
    template <typename = typename std::enable_if<IsConst>>
    RawIterator& operator=(const RawIterator<false>& src) noexcept {
      if (this != &src) {
        ptr_ = src.ptr_;
      }

      return *this;
    }

    ItReference operator*() const {
      return *ptr_;
    }
    ItPointer operator->() const noexcept {
      return ptr_;
    }

    friend RawIterator& operator++(RawIterator&) noexcept;
    friend RawIterator operator++(RawIterator&, int) noexcept;

    friend RawIterator& operator--(RawIterator&) noexcept;
    friend RawIterator operator--(RawIterator&, int) noexcept;

    friend RawIterator& operator+=(RawIterator&, DifferenceType n) noexcept;
    friend RawIterator& operator-=(RawIterator&, DifferenceType n) noexcept;

    friend DifferenceType operator-(const RawIterator& lhs, const RawIterator& rhs);

    friend RawIterator operator+(const RawIterator& src, DifferenceType n) noexcept;
    friend RawIterator operator+(DifferenceType n, const RawIterator& src) noexcept;

    friend RawIterator operator-(const RawIterator& src, DifferenceType n) noexcept;

    friend bool operator==(const RawIterator& lhs, const RawIterator& rhs) noexcept;
    friend bool operator!=(const RawIterator& lhs, const RawIterator& rhs) noexcept;
    friend bool operator<(const RawIterator& lhs, const RawIterator& rhs) noexcept;
    friend bool operator>(const RawIterator& lhs, const RawIterator& rhs) noexcept;
    friend bool operator<=(const RawIterator& lhs, const RawIterator& rhs) noexcept;
    friend bool operator>=(const RawIterator& lhs, const RawIterator& rhs) noexcept;
     */

   private:
    friend class Vector;

    ItPointer ptr_;
  };

 public:
  using Iterator = RawIterator<false>;
  using ConstIterator = RawIterator<true>;

  Vector();
  explicit Vector(SizeType size);
  Vector(SizeType size, ValueType value);

 private:
  ValueType* buffer_;

  SizeType size_;
  SizeType capacity_;
};