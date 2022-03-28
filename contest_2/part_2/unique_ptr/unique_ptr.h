#pragma once

template <typename T>
class UniquePtr {
 public:
  UniquePtr() : ptr_(nullptr){};
  explicit UniquePtr(T* ptr) : ptr_(ptr){};
  UniquePtr(const UniquePtr&) = delete;
  UniquePtr(UniquePtr&& src) noexcept : ptr_(src.ptr_) {
    src.ptr_ = nullptr;
  }

  ~UniquePtr() {
    delete ptr_;
  }

  UniquePtr& operator=(const UniquePtr&) = delete;
  UniquePtr& operator=(UniquePtr&& src) noexcept {
    if (this != &src) {
      delete ptr_;
      ptr_ = src.ptr_;
      src.ptr_ = nullptr;
    }

    return *this;
  };

  T* Release() {
    T* ptr = ptr_;
    ptr_ = nullptr;

    return ptr;
  }
  void Reset(T* new_ptr = nullptr) {
    delete ptr_;

    ptr_ = new_ptr;
  }

  void Swap(UniquePtr& other) {
    T* tmp = std::move(ptr_);
    ptr_ = std::move(other.ptr_);
    other.ptr_ = std::move(tmp);
  }

  T* Get() const {
    return ptr_;
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
};