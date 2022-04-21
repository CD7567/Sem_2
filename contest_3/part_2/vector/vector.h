#pragma once

#define VECTOR_MEMORY_IMPLEMENTED

#include <cstdint>
#include <algorithm>
#include <memory>

template <typename T>
class Vector {
 public:
  using value_type = T;
  using pointer = value_type*;
  using const_pointer = const value_type*;
  using reference = value_type&;
  using const_reference = const value_type&;
  using difference_type = std::ptrdiff_t;
  using size_type = uint64_t;

 private:
  template <bool IsConst>
  class RawIterator {
   public:
    using value_type = Vector::value_type;
    using difference_type = Vector::difference_type;
    using iterator_category = std::random_access_iterator_tag;
    using pointer = typename std::conditional_t<IsConst, Vector::const_pointer, Vector::pointer>;
    using reference = typename std::conditional_t<IsConst, Vector::const_reference, Vector::reference>;

    RawIterator() noexcept: ptr_(nullptr) {};
    RawIterator(pointer ptr) noexcept: ptr_(ptr) {};  //  NOLINT
    RawIterator(const RawIterator<IsConst>& src) noexcept: ptr_(src.ptr_) {};

    template <typename = typename std::enable_if<IsConst>>
    explicit RawIterator(const RawIterator<false>& src) noexcept: ptr_(src.ptr_) {};
    RawIterator(RawIterator&& src) noexcept = default;

    ~RawIterator() noexcept = default;

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

    reference operator*() const {
      return *ptr_;
    }
    pointer operator->() const noexcept {
      return ptr_;
    }

    RawIterator& operator++() noexcept {
      ++ptr_;
      return *this;
    };
    friend RawIterator operator++(RawIterator& it, int) noexcept {
      ++it;
      return {it.ptr_ - 1};
    };

    RawIterator& operator--() noexcept {
      --ptr_;
      return *this;
    }
    friend RawIterator operator--(RawIterator& it, int) noexcept {
      --it;
      return {it.ptr_ + 1};
    }

    RawIterator& operator+=(difference_type n) noexcept {
      ptr_ += n;
      return *this;
    }
    RawIterator& operator-=(difference_type n) noexcept {
      ptr_ -= n;
      return *this;
    }

    friend difference_type operator-(const RawIterator& lhs, const RawIterator& rhs) {
      return rhs.ptr_ - lhs.ptr_;
    }

    RawIterator operator+(difference_type n) noexcept {
      return {ptr_ + n};
    }
    friend RawIterator operator+(difference_type n, const RawIterator& src) noexcept {
      return {src.ptr_ + n};
    }

    friend RawIterator operator-(const RawIterator& src, difference_type n) noexcept {
      return {src.ptr_ - n};
    }

    friend bool operator==(const RawIterator& lhs, const RawIterator& rhs) noexcept {
      return lhs.ptr_ == rhs.ptr_;
    }
    friend bool operator!=(const RawIterator& lhs, const RawIterator& rhs) noexcept {
      return !(lhs == rhs);
    }
    friend bool operator<(const RawIterator& lhs, const RawIterator& rhs) noexcept {
      return lhs.ptr_ < rhs.ptr_;
    }
    friend bool operator>(const RawIterator& lhs, const RawIterator& rhs) noexcept {
      return rhs < lhs;
    }
    friend bool operator<=(const RawIterator& lhs, const RawIterator& rhs) noexcept {
      return !(rhs < lhs);
    }
    friend bool operator>=(const RawIterator& lhs, const RawIterator& rhs) noexcept {
      return !(lhs < rhs);
    }

   private:
    friend class Vector;

    pointer ptr_;
  };

 public:
  using Iterator = RawIterator<false>;
  using ConstIterator = RawIterator<true>;
  using ReverseIterator = std::reverse_iterator<Iterator>;
  using ConstReverseIterator = std::reverse_iterator<ConstIterator>;

  Vector() noexcept: buffer_(nullptr), size_(0), capacity_(0) {};

  explicit Vector(size_type size) : buffer_(nullptr), size_(0), capacity_(0) {
    if (size != 0) {
      size_type constructed = 0;

      try {
        buffer_ = allocator_.allocate(size);

        for (; constructed < size; ++constructed) {
          new(buffer_ + constructed) value_type();
        }

        size_ = size;
        capacity_ = size;
      } catch (...) {
          for (size_type i = 0; i < constructed; ++i) {
            buffer_[i].~value_type();
          }

        allocator_.deallocate(buffer_, size);
        buffer_ = nullptr;
        throw;
      }
    }
  };

  Vector(size_type size, const_reference value) : buffer_(nullptr), size_(0), capacity_(0) {
    if (size != 0) {
      size_type constructed = 0;

      try {
        buffer_ = allocator_.allocate(size);

        for (; constructed < size; ++constructed) {
          new(buffer_ + constructed) value_type(value);
        }

        size_ = size;
        capacity_ = size;
      } catch (...) {
          for (size_type i = 0; i < constructed; ++i) {
            buffer_[i].~value_type();
          }

        allocator_.deallocate(buffer_, size);
        buffer_ = nullptr;
        throw;
      }
    }
  };

  template <typename Iterator, typename = std::enable_if_t<std::is_base_of_v<std::forward_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category>>>
  Vector(Iterator first, Iterator last) : buffer_(nullptr), size_(0), capacity_(0) {
    difference_type size = last - first;

    if (size != 0) {
      size_type constructed = 0;

      try {
        buffer_ = allocator_.allocate(size);
        pointer it_buffer = buffer_;

        for (auto it = first; it != last; ++it, ++it_buffer, ++constructed) {
          new(it_buffer) value_type(*it);
        }

        size_ = size;
        capacity_ = size;
      } catch (...) {
          for (size_type i = 0; i < constructed; ++i) {
            buffer_[i].~value_type();
          }

        allocator_.deallocate(buffer_, capacity_);
        buffer_ = nullptr;
        throw;
      }
    }
  }

  Vector(const std::initializer_list<value_type>& list) {
    difference_type size = list.end() - list.begin();

    if (size != 0) {
      size_type constructed = 0;

      try {
        buffer_ = allocator_.allocate(size);
        pointer it_buffer = buffer_;

        for (auto it = list.begin(); it != list.end(); ++it, ++it_buffer, ++constructed) {
          new(it_buffer) value_type(*it);
        }

        size_ = size;
        capacity_ = size;
      } catch (...) {
          for (size_type i = 0; i < constructed; ++i) {
            buffer_[i].~value_type();
          }

        allocator_.deallocate(buffer_, capacity_);
        buffer_ = nullptr;
        throw;
      }
    }
  }

  Vector(const Vector& src) : buffer_(nullptr), size_(0), capacity_(0) {
    if (src.capacity_ != 0) {
      size_type constructed = 0;

      try {
        buffer_ = allocator_.allocate(src.capacity_);

        for (size_type i = 0; i < src.size_; ++i, ++constructed) {
          new (buffer_ + i) value_type(src.buffer_[i]);
        }

        size_ = src.size_;
        capacity_ = src.capacity_;
      } catch (...) {
        for (size_type i = 0; i < constructed; ++i) {
          buffer_[i].~value_type();
        }

        allocator_.deallocate(buffer_, src.capacity_);
        buffer_ = nullptr;
        throw;
      }
    }
  }

  Vector(Vector&& other) noexcept: buffer_(other.buffer_), size_(other.size_), capacity_(other.capacity_) {
    other.buffer_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }

  ~Vector() noexcept {
    for (size_type i = 0; i < size_; ++i) {
      buffer_[i].~value_type();
    }

    allocator_.deallocate(buffer_, capacity_);
  }

  Vector& operator=(const Vector& src) {
    if (this != &src) {
      if (src.capacity_ != 0) {
        size_type constructed = 0;
        pointer temp = nullptr;

        try {
          temp = allocator_.allocate(src.capacity_);

          for (size_type i = 0; i < src.size_; ++i, ++constructed) {
            new(temp + i) value_type(src.buffer_[i]);
          }

          for (size_type i = 0; i < size_; ++i) {
            buffer_[i].~value_type();
          }
          allocator_.deallocate(buffer_, capacity_);

          buffer_ = temp;
          size_ = src.size_;
          capacity_ = src.capacity_;
        } catch (...) {
          for (size_type i = 0; i < constructed; ++i) {
            temp[i].~value_type();
          }

          allocator_.deallocate(temp, src.capacity_);
          throw;
        }
      } else {
        for (size_type i = 0; i < size_; ++i) {
          buffer_[i].~value_type();
        }

        allocator_.deallocate(buffer_, size_);
        buffer_ = nullptr;
        size_ = 0;
        capacity_ = 0;
      }
    }

    return *this;
  }
  Vector& operator=(Vector&& src) noexcept {
    if (this != &src) {
      for (size_type i = 0; i < size_; ++i) {
        buffer_[i].~value_type();
      }
      allocator_.deallocate(buffer_, capacity_);

      buffer_ = src.buffer_;
      size_ = src.size_;
      capacity_ = src.capacity_;

      src.buffer_ = nullptr;
      src.size_ = 0;
      src.capacity_ = 0;
    }

    return *this;
  }

  const_reference operator[](size_type idx) const {
    return buffer_[idx];
  }
  reference operator[](size_type idx) {
    return buffer_[idx];
  }

  const_reference At(size_type idx) const {
    if (idx >= size_) {
      throw std::out_of_range("Index out of bounds");
    } else {
      return buffer_[idx];
    }
  }
  reference At(size_type idx) {
    if (idx >= size_) {
      throw std::out_of_range("Index out of bounds");
    } else {
      return buffer_[idx];
    }
  }

  friend bool operator==(const Vector& lhs, const Vector& rhs) noexcept {
    return !(lhs < rhs || rhs < lhs);
  }
  friend bool operator!=(const Vector& lhs, const Vector& rhs) noexcept {
    return !(lhs == rhs);
  }

  friend bool operator<(const Vector& lhs, const Vector& rhs) noexcept {
    bool result;
    auto rhs_it = rhs.begin();
    auto lhs_it = lhs.begin();
    auto lhs_end = lhs.end();
    auto rhs_end = rhs.end();

    while (lhs_it != lhs_end && rhs_it != rhs_end && *lhs_it == *rhs_it) {
      ++lhs_it;
      ++rhs_it;
    }

    if (lhs_it == lhs_end) {
      result = lhs.size_ < rhs.size_;
    } else if (rhs_it == rhs_end) {
      result = false;
    } else {
      result = *lhs_it < *rhs_it;
    }

    return result;
  }
  friend bool operator>(const Vector& lhs, const Vector& rhs) noexcept {
    return rhs < lhs;
  }
  friend bool operator<=(const Vector& lhs, const Vector& rhs) noexcept {
    return !(rhs < lhs);
  }
  friend bool operator>=(const Vector& lhs, const Vector& rhs) noexcept {
    return !(lhs < rhs);
  }

  [[nodiscard]] size_type Size() const noexcept {
    return size_;
  }
  [[nodiscard]] size_type Capacity() const noexcept {
    return capacity_;
  }
  [[nodiscard]] bool Empty() const noexcept {
    return (size_ == 0);
  }

  const_reference Front() const {
    return buffer_[0];
  }
  reference Front() {
    return buffer_[0];
  }

  const_reference Back() const {
    return buffer_[size_ - 1];
  }
  reference Back() {
    return buffer_[size_ - 1];
  }

  pointer Data() noexcept {
    return buffer_;
  }
  const_pointer Data() const noexcept {
    return buffer_;
  }

  Iterator begin() noexcept {
    return Iterator(buffer_);
  }
  ConstIterator begin() const noexcept {
    return ConstIterator(buffer_);
  }
  ConstIterator cbegin() const noexcept {
    return ConstIterator(buffer_);
  }

  Iterator end() noexcept {
    return Iterator(buffer_ + size_);
  }
  ConstIterator end() const noexcept {
    return ConstIterator(buffer_ + size_);
  }
  ConstIterator cend() const noexcept {
    return ConstIterator(buffer_ + size_);
  }

  ReverseIterator rbegin() noexcept {
    return std::make_reverse_iterator(Iterator{buffer_ + size_});
  }
  ConstReverseIterator rbegin() const noexcept {
    return std::make_reverse_iterator(ConstIterator{buffer_ + size_});
  }
  ConstReverseIterator crbegin() const noexcept {
    return std::make_reverse_iterator(ConstIterator{buffer_ + size_});
  }

  ReverseIterator rend() noexcept {
    return std::make_reverse_iterator(Iterator{buffer_});
  }
  ConstReverseIterator rend() const noexcept {
    return std::make_reverse_iterator(ConstIterator{buffer_});
  }
  ConstReverseIterator crend() const noexcept {
    return std::make_reverse_iterator(ConstIterator{buffer_});
  }

  void Swap(Vector& other) noexcept {
    std::swap(*this, other);
  }

  void Resize(size_type new_size) {
    if (new_size > capacity_) {
      size_type constructed = 0;
      pointer temp = nullptr;

      try {
        temp = allocator_.allocate(new_size);

        for (size_type i = 0; i < size_; ++i, ++constructed) {
          new (temp + i) value_type(std::move(buffer_[i]));
        }

        for (size_type i = size_; i < new_size; ++i, ++constructed) {
          new (temp + i) value_type();
        }

        for (size_type i = 0; i < size_; ++i) {
          buffer_[i].~value_type();
        }
        allocator_.deallocate(buffer_, capacity_);

        buffer_ = temp;
        size_ = new_size;
        capacity_ = new_size;
      } catch (...) {
        for (size_type i = 0; i < constructed; ++i) {
          temp[i].~value_type();
        }

        allocator_.deallocate(temp, new_size);
        throw;
      }
    } else {
      for (size_type i = new_size; i < size_; ++i) {
        buffer_[i].~value_type();
      }

      for (size_type i = size_; i < new_size; ++i) {
        new (buffer_ + i) value_type();
      }

      size_ = new_size;
    }
  }
  void Resize(size_type new_size, const_reference value) {
    if (new_size > capacity_) {
      size_type constructed = 0;
      pointer temp = nullptr;

      try {
        temp = allocator_.allocate(new_size);

        for (size_type i = 0; i < size_; ++i, ++constructed) {
          new (temp + i) value_type(std::move(buffer_[i]));
        }

        for (size_type i = size_; i < new_size; ++i, ++constructed) {
          new (temp + i) value_type(value);
        }

        for (size_type i = 0; i < size_; ++i) {
          buffer_[i].~value_type();
        }
        allocator_.deallocate(buffer_, capacity_);

        buffer_ = temp;
        size_ = new_size;
        capacity_ = new_size;
      } catch (...) {
        for (size_type i = 0; i < constructed; ++i) {
          temp[i].~value_type();
        }

        allocator_.deallocate(temp, new_size);
        throw;
      }
    } else {
      for (size_type i = new_size; i < size_; ++i) {
        buffer_[i].~value_type();
      }

      for (size_type i = size_; i < new_size; ++i) {
        new (buffer_ + i) value_type(value);
      }

      size_ = new_size;
    }
  }

  void Reserve(size_type new_cap) {
    if (new_cap > capacity_) {
      size_type constructed = 0;
      pointer temp = nullptr;

      try {
        temp = allocator_.allocate(new_cap);

        for (size_type i = 0; i < size_; ++i, ++constructed) {
          new (temp + i) value_type(std::move(buffer_[i]));
        }

        for (size_type i = 0; i < size_; ++i) {
          buffer_[i].~value_type();
        }
        allocator_.deallocate(buffer_, capacity_);

        buffer_ = temp;
        capacity_ = new_cap;
      } catch (...) {
        for (size_type i = 0; i < new_cap; ++i) {
          temp[i].~value_type();
        }

        allocator_.deallocate(temp, new_cap);
        throw;
      }
    }
  }

  void ShrinkToFit() {
    if (size_ != capacity_) {
      if (size_ != 0) {
        size_type constructed = 0;
        pointer temp = nullptr;

        try {
          temp = allocator_.allocate(size_);

          for (size_type i = 0; i < size_; ++i, ++constructed) {
            new (temp + i) value_type(std::move(buffer_[i]));
          }

          for (size_type i = 0; i < size_; ++i) {
            buffer_[i].~value_type();
          }
          allocator_.deallocate(buffer_, capacity_);

          buffer_ = temp;
          capacity_ = size_;
        } catch (...) {
          for (size_type i = 0; i < constructed; ++i) {
            buffer_[i] = std::move(temp[i]);
            temp[i].~value_type();
          }

          allocator_.deallocate(temp, size_);
          throw;
        }
      } else {
        allocator_.deallocate(buffer_, capacity_);
        buffer_ = nullptr;
        capacity_ = 0;
      }

    }
  }

  void Clear() noexcept {
    for (size_type i = 0; i < size_; ++i) {
      buffer_[i].~value_type();
    }

    size_ = 0;
  }

  void PushBack(const_reference value) {
    if (size_ == capacity_) {
      size_type new_cap = GetNewCapacity();
      size_type constructed = 0;
      pointer temp = nullptr;

      try {
        temp = allocator_.allocate(new_cap);

        for (size_type i = 0; i < size_; ++i, ++constructed) {
          new (temp + i) value_type(buffer_[i]);
        }

        for (size_type i = 0; i < size_; ++i) {
          buffer_[i].~value_type();
        }
        allocator_.deallocate(buffer_, capacity_);

        buffer_ = temp;
        capacity_ = new_cap;
      } catch (...) {
        for (size_type i = 0; i < constructed; ++i) {
          temp[i].~value_type();
        }

        allocator_.deallocate(temp, new_cap);
        throw;
      }
    }

    new (buffer_ + size_++) value_type(value);
  }
  void PushBack(value_type&& value) {
    if (size_ == capacity_) {
      size_type new_cap = GetNewCapacity();
      size_type constructed = 0;
      pointer temp = nullptr;

      try {
        temp = allocator_.allocate(new_cap);

        for (size_type i = 0; i < size_; ++i, ++constructed) {
          new (temp + i) value_type(std::move(buffer_[i]));
        }

        for (size_type i = 0; i < size_; ++i) {
          buffer_[i].~value_type();
        }
        allocator_.deallocate(buffer_, capacity_);

        buffer_ = temp;
        capacity_ = new_cap;
      } catch (...) {
        for (size_type i = 0; i < constructed; ++i) {
          temp[i].~value_type();
        }

        allocator_.deallocate(temp, new_cap);
        throw;
      }
    }

    new (buffer_ + size_++) value_type(std::move(value));
  }

  template <typename... Args>
  void EmplaceBack(Args&&... args) {
    if (size_ == capacity_) {
      size_type new_cap = GetNewCapacity();
      size_type constructed = 0;
      pointer temp = nullptr;

      try {
        temp = allocator_.allocate(new_cap);

        for (size_type i = 0; i < size_; ++i, ++constructed) {
          new (temp + i) value_type(std::move(buffer_[i]));
        }

        for (size_type i = 0; i < size_; ++i) {
          buffer_[i].~value_type();
        }
        allocator_.deallocate(buffer_, capacity_);

        buffer_ = temp;
        capacity_ = new_cap;
      } catch (...) {
        for (size_type i = 0; i < constructed; ++i) {
          temp[i].~value_type();
        }

        allocator_.deallocate(temp, new_cap);
        throw;
      }
    }

    new (buffer_ + size_++) value_type(std::forward<Args>(args)...);
  }

  value_type PopBack() {
    value_type removed = std::move(buffer_[--size_]);

    buffer_[size_].~value_type();

    return std::move(removed);
  }

 private:
  size_type GetNewCapacity() noexcept {
    size_type new_cap = 2;

    if (capacity_ != 0) {
      new_cap = 2 * capacity_;
    }

    return new_cap;
  }

  std::allocator<value_type> allocator_;
  pointer buffer_;

  size_type size_;
  size_type capacity_;
};