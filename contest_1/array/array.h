#pragma once

#define ARRAY_TRAITS_IMPLEMENTED

#include <cstddef>
#include <stdexcept>

class ArrayOutOfRange : public std::out_of_range {
 public:
  ArrayOutOfRange() : std::out_of_range("ArrayOutOfRange") {
  }
};

template <typename T, size_t N>
class Array {
 public:
  const T& operator[](size_t idx) const {
    return array[idx];
  }

  T& operator[](size_t idx) {
    return array[idx];
  }

  const T& At(size_t idx) const {
    if (idx < 0 || idx >= N) {
      throw ArrayOutOfRange();
    } else {
      return array[idx];
    }
  }
  T& At(size_t idx) {
    if (idx < 0 || idx >= N) {
      throw ArrayOutOfRange();
    } else {
      return array[idx];
    }
  }

  const T& Front() const {
    return array[0];
  }
  T& Front() {
    return array[0];
  }

  const T& Back() const {
    return array[N - 1];
  }
  T& Back() {
    return array[N - 1];
  }

  const T* Data() const {
    return array;
  }
  T* Data() {
    return array;
  }

  [[nodiscard]] size_t Size() const {
    return N;
  }

  [[nodiscard]] bool Empty() const {
    return N == 0;
  }

  void Fill(const T& src) {
    for (size_t i = 0; i < N; ++i) {
      array[i] = src;
    }
  }

  void Swap(Array<T, N>& other) {
    for (size_t i = 0; i < N; ++i) {
      std::swap(array[i], other[i]);
    }
  }

  T array[N];
};

//  =========================================
//  Array traits
//  =========================================

template <typename T, size_t N>
size_t GetSize(T (&)[N]) {
  return N;
}

template <typename T>
size_t GetSize(T) {
  return 0;
}

template <typename T>
size_t GetRank(T) {
  return 0;
}

template <typename T, size_t N>
size_t GetRank(T (&array)[N]) {
  return 1 + GetRank(array[0]);
}

template <typename T>
size_t GetNumElements(T) {
  return 1;
}

template <typename T, size_t N>
size_t GetNumElements(T (&array)[N]) {
  return GetSize(array) * GetNumElements(array[0]);
}