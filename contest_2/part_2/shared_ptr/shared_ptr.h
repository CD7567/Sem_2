#pragma once

#include <cstdint>

struct Counter {
  size_t strong_count_, weak_count_;
};

template <typename T>
class SharedPtr {
 public:
  SharedPtr() : ptr_(nullptr), new_counter_(new Counter{}){};
  SharedPtr(T* object_ptr) : ptr_(object_ptr), new_counter_{new Counter{1}} {  //  NOLINT
    if (object_ptr == nullptr) {
      new_counter_->strong_count_ = 0;
    }
  };
  SharedPtr(const SharedPtr& src) : ptr_(src.ptr_), new_counter_(src.new_counter_) {
    if (new_counter_->strong_count_ != 0) {
      ++new_counter_->strong_count_;
    } else {
      new_counter_ = new Counter{};
    }
  }
  SharedPtr(SharedPtr&& src) noexcept : ptr_(src.ptr_), new_counter_(src.new_counter_) {
    src.ptr_ = nullptr;
    src.new_counter_ = new Counter{};
  }

  ~SharedPtr() {
    if (new_counter_->strong_count_ > 1) {
      --(new_counter_->strong_count_);
    } else {
      delete ptr_;
      delete new_counter_;
    }
  }

  SharedPtr& operator=(const SharedPtr& src) {
    if (this != &src) {
      if (new_counter_->strong_count_ <= 1) {
        delete ptr_;
        delete new_counter_;
      } else {
        --new_counter_->strong_count_;
      }

      if (*src.counter_ == 0) {
        new_counter_ = new Counter{};
      } else {
        new_counter_ = src.new_counter_;
        ++new_counter_->strong_count_;
      }

      ptr_ = src.ptr_;
    }

    return *this;
  }
  SharedPtr& operator=(SharedPtr&& src) noexcept {
    if (this != &src) {
      if (new_counter_->strong_count_ <= 1) {
        delete ptr_;
        delete new_counter_;
      } else {
        --new_counter_->strong_count_;
      }

      ptr_ = src.ptr_;
      new_counter_ = src.counter_;

      src.ptr_ = nullptr;
      src.new_counter_ = new Counter{};
    }

    return *this;
  }

  void Reset(T* new_ptr = nullptr) {
    if (new_counter_->strong_count_ <= 1) {
      delete ptr_;
      delete new_counter_;
    } else {
      --new_counter_;
    }

    ptr_ = new_ptr;
    new_counter_ = new Counter{};

    if (new_ptr != nullptr) {
      ++new_counter_->strong_count_;
    }
  }

  void Swap(SharedPtr& other) {
    if (this != &other) {
      T* tmp = std::move(ptr_);
      Counter* tmp_counter = new_counter_;

      ptr_ = std::move(other.ptr_);
      new_counter_ = other.new_counter_;

      other.ptr_ = std::move(tmp);
      other.new_counter_ = tmp_counter;
    }
  }

  T* Get() const {
    return ptr_;
  }

  [[nodiscard]] size_t UseCount() const {
    return new_counter_->strong_count_;
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
  Counter* new_counter_;
};