#include "vector.h"

// ==============================================
// Implementation of Vector::RawIterator
// ==============================================

template <typename T>
template <bool IsConst>
Vector<T>::RawIterator<IsConst>::RawIterator() noexcept: ptr_(nullptr) {}

template <typename T>
template <bool IsConst>
Vector<T>::RawIterator<IsConst>::RawIterator(ItPointer ptr) noexcept: ptr_(ptr) {}

template <typename T>
template <bool IsConst>
Vector<T>::RawIterator<IsConst>::RawIterator(const Vector<T>::RawIterator<IsConst>& src) noexcept: ptr_(src.ptr_) {}

/*
template <typename T>
template <bool IsConst>
template <typename>
Vector<T>::RawIterator<IsConst>::RawIterator(const Vector::RawIterator<false>& src) noexcept: ptr_(src.ptr_) {}

template <typename T, bool IsConst>
typename Vector<T>::template RawIterator<IsConst> operator++(typename Vector<T>::template RawIterator<IsConst>& it) noexcept {
  ++it.ptr_;
  return it;
}
*/
// ==============================================
// Implementation of Vector
// ==============================================

template <typename T>
Vector<T>::Vector() : buffer_(nullptr), size_(0), capacity_(0) {}

template <typename T>
Vector<T>::Vector(SizeType size) : buffer_(new ValueType[size]{}), size_(size), capacity_(capacity_) {}

template <typename T>
Vector<T>::Vector(SizeType size, ValueType value) : buffer_(std::fill_n(new ValueType[size], value)), size_(size), capacity_(capacity_) {}
