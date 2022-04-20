#pragma once

#include <cstdint>
#include <algorithm>

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
  explicit Vector(size_type size) : buffer_(new value_type[size]{}), size_(size), capacity_(size) {};
  Vector(size_type size, const value_type& value) : buffer_(new value_type[size]), size_(size), capacity_(size) {
    std::fill_n(buffer_, size_, value);
  };
  template <typename Iterator, typename = std::enable_if_t<std::is_base_of_v<std::forward_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category>>>
  Vector(Iterator first, Iterator last) {
    difference_type size = last - first;

    buffer_ = new value_type[size];
    size_ = size;
    capacity_ = size;
    size_type i = 0;

    for (auto it = first; it != last; ++it, ++i) {
      buffer_[i] = *it;
    }
  }
  Vector(const std::initializer_list<value_type>& list) {
    difference_type size = list.end() - list.begin();

    buffer_ = new value_type[size];
    size_ = size;
    capacity_ = size;
    size_type i = 0;

    for (auto it = list.begin(); it != list.end(); ++it, ++i) {
      buffer_[i] = *it;
    }
  }
  Vector(const Vector& src) : size_(src.size_), capacity_(src.capacity_) {
    if (capacity_ == 0) {
      buffer_ = nullptr;
    } else {
      buffer_ = new value_type[capacity_];
      for (size_type i = 0; i < size_; ++i) {
        buffer_[i] = src[i];
      }
    }
  }
  Vector(Vector&& other) noexcept : buffer_(other.buffer_), size_(other.size_), capacity_(other.capacity_) {
    other.buffer_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }

  ~Vector() noexcept {
    delete[] buffer_;
  }

  Vector& operator=(const Vector& src) {
    if (this != &src) {
      delete[] buffer_;

      if (src.capacity_ == 0) {
        buffer_ = nullptr;
      } else {
        buffer_ = new value_type[capacity_];
        for (size_type i = 0; i < size_; ++i) {
          buffer_[i] = src[i];
        }
      }

      size_ = src.size_;
      capacity_ = src.capacity_;
    }

    return *this;
  }
  Vector& operator=(Vector&& src) noexcept {
    if (this != &src) {
      delete[] buffer_;

      buffer_ = src.buffer_;
      size_ = src.size_;
      capacity_ = src.capacity_;

      src.buffer_ = nullptr;
      src.size_ = 0;
      src.capacity_ = 0;
    }

    return *this;
  }

  const value_type& operator[](size_type idx) const {
    return buffer_[idx];
  }
  value_type& operator[](size_type idx) {
    return buffer_[idx];
  }

  const value_type& At(size_type idx) const {
    if (idx >= size_) {
      throw std::out_of_range("Index out of bounds");
    } else {
      return buffer_[idx];
    }
  }
  value_type& At(size_type idx) {
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

    if (lhs_it == lhs_end && lhs.size_ < rhs.size_) {
      result = true;
    } else if (rhs_it == rhs_end && lhs.size_ > rhs.size_) {
      result = false;
    } else {

      //lhs_it -= ((lhs_it == lhs_end) ? 1 : 0);
      //rhs_it -= ((lhs_it == lhs_end) ? 1 : 0);
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

  /*
  friend bool operator<(const Vector& lhs, const Vector& rhs) {
    size_type i = 0;

    while (lhs[i] == rhs[i] && i < std::min(rhs.size_, lhs.size_)) {
      i++;
    }

    if (i == lhs.size_ && lhs.size_ < rhs.size_) {
      return true;
    }

    if (i < std::min(rhs.size_, lhs.size_) && (lhs[i] < rhs[i]) ) {
      return true;
    }

    return false;
  }
  */


  [[nodiscard]] size_type Size() const noexcept {
    return size_;
  }
  [[nodiscard]] size_type Capacity() const noexcept {
    return capacity_;
  }
  [[nodiscard]] bool Empty() const noexcept {
    return (size_ == 0);
  }

  const value_type& Front() const {
    return buffer_[0];
  }
  value_type& Front() {
    return buffer_[0];
  }

  const value_type& Back() const {
    return buffer_[size_ - 1];
  }
  value_type& Back() {
    return buffer_[size_ - 1];
  }

  const value_type* Data() const {
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
    return std::make_reverse_iterator(Iterator{buffer_ + size_ - 1});
  }
  ConstReverseIterator rbegin() const noexcept {
    return std::make_reverse_iterator(ConstIterator{buffer_ + size_ - 1});
  }
  ConstReverseIterator crbegin() const noexcept {
    return std::make_reverse_iterator(ConstIterator{buffer_ + size_ - 1});
  }

  ReverseIterator rend() noexcept {
    return std::make_reverse_iterator(Iterator{buffer_ - 1});
  }
  ConstReverseIterator rend() const noexcept {
    return std::make_reverse_iterator(ConstIterator{buffer_ - 1});
  }
  ConstReverseIterator crend() const noexcept {
    return std::make_reverse_iterator(ConstIterator{buffer_ - 1});
  }

  void Swap(Vector& other) noexcept {
    std::swap(*this, other);
  }

  void Resize(size_type new_size) {
    if (new_size > capacity_) {
      value_type* temp = buffer_;
      buffer_ = new value_type[new_size]{};

      for (size_type i = 0; i < size_; ++i) {
        buffer_[i] = std::move(temp[i]);
      }

      size_ = new_size;
      capacity_ = new_size;

      delete[] temp;
    } else {
      size_ = new_size;
    }
  }
  void Resize(size_type new_size, const value_type& value) {
    if (new_size > capacity_) {
      value_type* temp = buffer_;
      buffer_ = new value_type[new_size];

      for (size_t i = 0; i < size_; ++i) {
        buffer_[i] = std::move(temp[i]);
      }

      std::fill_n(buffer_ + size_, new_size - size_, value);

      size_ = new_size;
      capacity_ = new_size;

      delete[] temp;
    } else {
      size_ = new_size;
    }
  }

  void Reserve(size_type new_cap) {
    if (new_cap > capacity_) {
      value_type* temp = buffer_;
      buffer_ = new value_type[new_cap];

      for (size_t i = 0; i < size_; ++i) {
        buffer_[i] = std::move(temp[i]);
      }

      capacity_ = new_cap;

      delete[] temp;
    }
  }

  void ShrinkToFit() {
    value_type* temp = buffer_;
    buffer_ = new value_type[size_];

    for (size_t i = 0; i < size_; ++i) {
      buffer_[i] = std::move(temp[i]);
    }

    capacity_ = size_;

    delete[] temp;
  }

  void Clear() noexcept {
    size_ = 0;
  }

  void PushBack(const value_type& value) {
    if (size_ == capacity_) {
      value_type* temp = buffer_;
      buffer_ = new value_type[GetNewCapacity()];

      for (size_t i = 0; i < size_; ++i) {
        buffer_[i] = std::move(temp[i]);
      }

      buffer_[size_++] = std::move(value);

      delete[] temp;
    }

    buffer_[size_++] = value;
  }
  void PushBack(value_type&& value) {
    if (size_ == capacity_) {
      value_type* temp = buffer_;
      buffer_ = new value_type[GetNewCapacity()];

      for (size_t i = 0; i < size_; ++i) {
        buffer_[i] = std::move(temp[i]);
      }

      delete[] temp;
    }

    buffer_[size_++] = std::move(value);
  }

  value_type PopBack() noexcept {
    return std::move(buffer_[size_--]);
  }

 private:
  size_type GetNewCapacity() noexcept {
    size_type new_cap = 2;

    if (capacity_ != 0) {
      new_cap = 2 * capacity_;
    }

    return new_cap;
  }

  value_type* buffer_;

  size_type size_;
  size_type capacity_;
};