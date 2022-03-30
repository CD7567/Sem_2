#pragma once

#include <memory>
#include <stdexcept>

class BadAnyCast : public std::bad_cast {
 public:
  [[nodiscard]] const char* what() const noexcept override {
    return "BadAnyCast error";
  }
};

class IHolder {
 public:
  virtual ~IHolder() noexcept = default;

  virtual IHolder* Clone() = 0;
};

template <typename T>
class AnyHolder : public IHolder {
 public:
  explicit AnyHolder(const T& src) : object_(src){};
  explicit AnyHolder(T&& src) : object_(std::move(src)){};

  ~AnyHolder() noexcept override = default;

  AnyHolder* Clone() override {
    return new AnyHolder(*this);
  }

  T object_;
};

class Any {
 public:
  Any() : ptr_(nullptr) {
  }
  Any(const Any& other) : ptr_(other.ptr_->Clone()) {
  }
  Any(Any&& other) noexcept : ptr_(other.ptr_->Clone()) {
  }
  template <typename T>
  explicit Any(const T& src) {
    ptr_ = std::shared_ptr(AnyHolder(src));
  }
  template <typename T>
  Any(T&& src) {  //  NOLINT
    ptr_ = std::make_shared<AnyHolder<T>>(std::forward<T>(src));
  }

  Any& operator=(const Any& src) {
    if (this != &src) {
      ptr_ = std::shared_ptr<IHolder>(src.ptr_->Clone());
    }

    return *this;
  }
  Any& operator=(Any&& src) noexcept {
    if (this != &src) {
      ptr_ = std::move(src.ptr_);
    }

    return *this;
  }

  void Reset() {
    ptr_ = nullptr;
  }

  void Swap(Any& other) {
    auto tmp = std::move(other.ptr_);
    other.ptr_ = std::move(ptr_);
    ptr_ = std::move(tmp);
  }

  [[nodiscard]] bool HasValue() const {
    return static_cast<bool>(ptr_);
  }

  template <typename T>
  friend T AnyCast(const Any&);

 private:
  std::shared_ptr<IHolder> ptr_;
};

template <typename T>
T AnyCast(const Any& any) {
  auto* holder = dynamic_cast<AnyHolder<T>*>(any.ptr_.get());

  if (holder == nullptr) {
    throw BadAnyCast();
  } else {
    return holder->object_;
  }
}

template <typename T, typename... V>
Any MakeAny(V&&... args) {
  return Any(T(std::forward<V>(args)...));
}