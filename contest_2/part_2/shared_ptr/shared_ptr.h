#pragma once

#include <cstdint>

template <typename T>
class SharedPtr {
 public:
  SharedPtr() : ptr_(nullptr){};


 private:
  T* ptr_;
  size_t
};