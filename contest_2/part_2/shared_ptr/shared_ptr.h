#pragma once

#include <cstdint>

template <typename T>
class SharedPtr {
 public:
  SharedPtr() : ptr_(nullptr), counter_(new size_t{}){};
  SharedPtr(T* object_ptr) : ptr_(object_ptr), counter_{new size_t{1}} {  //  NOLINT
    if (object_ptr == nullptr) {
      *counter_ = 0;
    }
  };
  SharedPtr(const SharedPtr& src) : ptr_(src.ptr_), counter_(src.counter_) {
    if (*counter_ != 0) {
      ++(*counter_);
    } else {
      counter_ = new size_t{};
    }
  }
  SharedPtr(SharedPtr&& src) noexcept : ptr_(src.ptr_), counter_(src.counter_) {
    src.ptr_ = nullptr;
    src.counter_ = new size_t{};
  }

  ~SharedPtr() {
    if (*counter_ > 1) {
      --(*counter_);
    } else {
      delete ptr_;
      delete counter_;
    }
  }

  SharedPtr& operator=(const SharedPtr& src) {
    if (this != &src) {
      if (*counter_ <= 1) {
        delete ptr_;
        delete counter_;
      } else {
        --(*counter_);
      }

      if (*src.counter_ == 0) {
        counter_ = new size_t{};
      } else {
        counter_ = src.counter_;
        ++(*counter_);
      }

      ptr_ = src.ptr_;
    }

    return *this;
  }
  SharedPtr& operator=(SharedPtr&& src) noexcept {
    if (this != &src) {
      if (*counter_ <= 1) {
        delete ptr_;
        delete counter_;
      } else {
        --(*counter_);
      }

      ptr_ = src.ptr_;
      counter_ = src.counter_;

      src.ptr_ = nullptr;
      src.counter_ = new size_t{};
    }

    return *this;
  }

  void Reset(T* new_ptr = nullptr) {
    if (*counter_ <= 1) {
      delete ptr_;
      delete counter_;
    } else {
      --(*counter_);
    }

    ptr_ = new_ptr;
    counter_ = new size_t{};

    if (new_ptr != nullptr) {
      ++(*counter_);
    }
  }

  void Swap(SharedPtr& other) {
    if (this != &other) {
      T* tmp = std::move(ptr_);
      size_t* tmp_counter = counter_;

      ptr_ = std::move(other.ptr_);
      counter_ = std::move(other.counter_);

      other.ptr_ = std::move(tmp);
      other.counter_ = tmp_counter;
    }
  }

  T* Get() const {
    return ptr_;
  }

  [[nodiscard]] size_t UseCount() const {
    return *counter_;
  }

  T& operator*() const {
    return *ptr_;
  }
  T* operator->() const {
    return ptr_;
  }

  explicit operator bool() const {
    return ptr_ != nullptr;
  }

 private:
  T* ptr_;
  size_t* counter_;
};