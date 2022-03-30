#pragma once

#include <cstdint>

#define WEAK_PTR_IMPLEMENTED

struct Counter {
  size_t strong_count_, weak_count_;
};

class BadWeakPtr : public std::runtime_error {
 public:
  BadWeakPtr() : std::runtime_error("BadWeakPtr") {
  }
};

template <typename T>
class WeakPtr;

template <typename T>
class SharedPtr {
 public:
  friend class WeakPtr<T>;

  SharedPtr() : ptr_(nullptr), counter_(new Counter{}){};
  SharedPtr(T* object_ptr) : ptr_(object_ptr), counter_{new Counter{1, 0}} {  //  NOLINT
    if (object_ptr == nullptr) {
      counter_->strong_count_ = 0;
    }
  };
  SharedPtr(const SharedPtr& src) : ptr_(src.ptr_), counter_(src.counter_) {
    if (counter_->strong_count_ != 0) {
      ++counter_->strong_count_;
    } else {
      counter_ = new Counter{};
    }
  }
  SharedPtr(SharedPtr&& src) noexcept : ptr_(src.ptr_), counter_(src.counter_) {
    src.ptr_ = nullptr;
    src.counter_ = new Counter{};
  }
  explicit SharedPtr(const WeakPtr<T>& src) : ptr_(src.ptr_), counter_(src.counter_) {
    if (src.Expired()) {
      throw BadWeakPtr();
    } else {
      ++counter_->strong_count_;
    }
  }

  ~SharedPtr() {
    if (counter_->strong_count_ > 1) {
      --(counter_->strong_count_);
    } else {
      delete ptr_;

      if (counter_->weak_count_ == 0) {
        delete counter_;
      }
    }
  }

  SharedPtr& operator=(const SharedPtr& src) {
    if (this != &src) {
      if (counter_->strong_count_ <= 1) {
        delete ptr_;

        if (counter_->weak_count_ == 0) {
          delete counter_;
        }
      } else {
        --counter_->strong_count_;
      }

      if (src.counter_->strong_count_ == 0) {
        counter_ = new Counter{};
      } else {
        counter_ = src.counter_;
        ++counter_->strong_count_;
      }

      ptr_ = src.ptr_;
    }

    return *this;
  }
  SharedPtr& operator=(SharedPtr&& src) noexcept {
    if (this != &src) {
      if (counter_->strong_count_ <= 1) {
        delete ptr_;

        if (counter_->weak_count_ == 0) {
          delete counter_;
        }
      } else {
        --counter_->strong_count_;
      }

      ptr_ = src.ptr_;
      counter_ = src.counter_;

      src.ptr_ = nullptr;
      src.counter_ = new Counter{};
    }

    return *this;
  }

  void Reset(T* new_ptr = nullptr) {
    if (counter_->strong_count_ <= 1) {
      delete ptr_;

      if (counter_->weak_count_ == 0) {
        delete counter_;
      }
    } else {
      --counter_->strong_count_;
    }

    ptr_ = new_ptr;
    counter_ = new Counter{};

    if (new_ptr != nullptr) {
      ++counter_->strong_count_;
    }
  }

  void Swap(SharedPtr& other) {
    if (this != &other) {
      T* tmp = std::move(ptr_);
      Counter* tmp_counter = counter_;

      ptr_ = std::move(other.ptr_);
      counter_ = other.counter_;

      other.ptr_ = std::move(tmp);
      other.counter_ = tmp_counter;
    }
  }

  T* Get() const {
    return ptr_;
  }

  [[nodiscard]] size_t UseCount() const {
    return counter_->strong_count_;
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
  Counter* counter_{};
};

template <typename T>
class WeakPtr {
 public:
  friend class SharedPtr<T>;

  WeakPtr() : ptr_(nullptr), counter_(new Counter{}){};
  WeakPtr(const WeakPtr& src) : ptr_(src.ptr_), counter_(src.counter_) {
    if (src.ptr_ == nullptr) {
      counter_ = new Counter{};
    } else {
      ++src.counter_->weak_count_;
    }
  }
  WeakPtr(WeakPtr&& src) noexcept : ptr_(src.ptr_), counter_(src.counter_) {
    src.ptr_ = nullptr;
    src.counter_ = new Counter{};
  }
  WeakPtr(const SharedPtr<T>& src) : ptr_(src.ptr_), counter_(src.counter_) {  //  NOLINT
    ++counter_->weak_count_;
  }

  ~WeakPtr() {
    if (counter_->strong_count_ != 0) {
      --counter_->weak_count_;
    } else {
      if (counter_->weak_count_ == 1) {
        delete counter_;
      } else {
        --counter_->weak_count_;
      }
    }
  }

  WeakPtr& operator=(const WeakPtr& src) {
    if (this != &src) {
      if (counter_->strong_count_ != 0) {
        --counter_->weak_count_;
      } else if (counter_->weak_count_ == 1) {
        delete counter_;
      }

      ptr_ = src.ptr_;
      counter_ = src.counter_;

      ++counter_->weak_count_;
    }

    return *this;
  }
  WeakPtr& operator=(WeakPtr&& src) noexcept {
    if (this != &src) {
      if (counter_->strong_count_ != 0) {
        --counter_->weak_count_;
      } else if (counter_->weak_count_ == 1) {
        delete counter_;
      }

      ptr_ = std::move(src.ptr_);
      counter_ = src.counter_;

      src.ptr_ = nullptr;
      src.counter_ = new Counter{};
    }

    return *this;
  }

  void Swap(WeakPtr& other) {
    if (this != &other) {
      T* tmp = std::move(ptr_);
      Counter* tmp_counter = counter_;

      ptr_ = std::move(other.ptr_);
      counter_ = other.counter_;

      other.ptr_ = std::move(tmp);
      other.counter_ = tmp_counter;
    }
  }

  void Reset() {
    if (counter_->strong_count_ == 0) {
      if (counter_->weak_count_ == 1) {
        delete counter_;
      } else {
        --counter_->weak_count_;
      }
    } else {
      --counter_->weak_count_;
    }

    ptr_ = nullptr;
    counter_ = new Counter();
  }

  [[nodiscard]] size_t UseCount() const {
    return counter_->strong_count_;
  }

  [[nodiscard]] bool Expired() const {
    return counter_->strong_count_ == 0;
  }

  SharedPtr<T> Lock() const {
    SharedPtr<T> result;

    if (counter_->strong_count_ != 0) {
      ++counter_->strong_count_;
      result = std::move(SharedPtr(*this));
    }

    return result;
  }

 private:
  T* ptr_;
  Counter* counter_;
};

template <typename T, typename... V>
SharedPtr<T> MakeShared(V&&... args) {
  return {new T(std::forward<V>(args)...)};
}