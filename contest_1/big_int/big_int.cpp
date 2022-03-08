#include "big_int.h"

#include <cstring>
#include <cctype>
#include <iostream>

template <typename T>
bool Min(const T& lhs, const T& rhs) {
  return ((lhs < rhs) ? lhs : rhs);
}

// ===========================================================
// Realization of BigInteger class
// ===========================================================

BigInteger::BigInteger(const char* src) {
  bool is_src_negative = (src[0] == '-');
  bool has_sign = !std::isdigit(src[0]);
  size_t dec_length = strlen(src);
  auto unit = new char[kBaseDecLen];
  is_negative_ = is_src_negative;

  size_t i;
  size_t k;
  for (i = dec_length - 1; i >= kBaseDecLen; i -= kBaseDecLen) {
    for (size_t j = kBaseDecLen; j != 0; --j) {
      unit[j - 1] = src[i + j - kBaseDecLen];
    }

    buffer_.PushBack(std::stoi(unit));
  }

  for (i += 1, k = kBaseDecLen - 1; i > static_cast<size_t>(has_sign); --i, --k) {
    unit[k] = src[i - 1];
  }

  if (k < kBaseDecLen) {
    for (++k; k != 0; --k) {
      unit[k - 1] = '0';
    }
  }

  buffer_.PushBack(std::stoi(unit));
  delete[] unit;
}
BigInteger::BigInteger(int32_t src) {
  if (src < 0) {
    is_negative_ = true;
    src = -src;
  }

  if (src == 0) {
    buffer_.PushBack(0);
  } else {
    while (src > 0) {
      buffer_.PushBack(static_cast<int32_t>(src % kBase));
      src /= kBase;
    }
  }
}
BigInteger::BigInteger(int64_t src) {
  if (src < 0) {
    is_negative_ = true;
    src = -src;
  }

  if (src == 0) {
    buffer_.PushBack(0);
  } else {
    while (src > 0) {
      buffer_.PushBack(static_cast<int32_t>(src % kBase));
      src /= kBase;
    }
  }
}
BigInteger::BigInteger(const BigInteger& src) {
  is_negative_ = src.is_negative_;
  buffer_ = Buffer(src.buffer_);
}

BigInteger operator+(const BigInteger& rhs) {
  return rhs;
}
BigInteger operator-(const BigInteger& rhs) {
  BigInteger copy = rhs;
  copy.is_negative_ = !copy.is_negative_;

  return copy;
}

BigInteger& operator++(BigInteger& src) {
  src += 1;
  return src;
}
BigInteger& operator--(BigInteger& src) {
  src -= 1;
  return src;
}
BigInteger operator++(BigInteger& src, int) {
  BigInteger src_clone(src);
  ++src;
  return src_clone;
}
BigInteger operator--(BigInteger& src, int) {
  BigInteger src_clone(src);
  --src;
  return src_clone;
}

BigInteger operator+(const BigInteger& lhs, const BigInteger& rhs) {
  BigInteger res;

  if (lhs.is_negative_) {
    if (rhs.is_negative_) {
      BigInteger::RawSum(lhs, rhs, res);
      res.is_negative_ = true;
    } else {
      BigInteger::RawSubtract(lhs, rhs, res);
      res.is_negative_ = !res.is_negative_;
    }
  } else {
    if (rhs.is_negative_) {
      BigInteger::RawSubtract(lhs, rhs, res);
    } else {
      BigInteger::RawSum(lhs, rhs, res);
    }
  }

  res.CheckSign();
  return res;
}
BigInteger operator-(const BigInteger& lhs, const BigInteger& rhs) {
  BigInteger res;

  if (lhs.is_negative_) {
    if (rhs.is_negative_) {
      BigInteger::RawSubtract(lhs, rhs, res);
      res.is_negative_ = !res.is_negative_;
    } else {
      BigInteger::RawSum(lhs, rhs, res);
      res.is_negative_ = true;
    }
  } else {
    if (rhs.is_negative_) {
      BigInteger::RawSum(lhs, rhs, res);
    } else {
      BigInteger::RawSubtract(lhs, rhs, res);
    }
  }

  res.CheckSign();
  return res;
}
BigInteger operator*(const BigInteger& lhs, const BigInteger& rhs) {
  BigInteger result;

  if (lhs.buffer_.GetSize() + rhs.buffer_.GetSize() > 7500) {
    throw BigIntegerOverflow();
  } else {
    BigInteger::RawMultiply(lhs, rhs, result);
    result.is_negative_ = lhs.is_negative_ xor rhs.is_negative_;
  }

  result.CheckSign();
  return result;
}
BigInteger& operator+=(BigInteger& lhs, const BigInteger& rhs) {
  if (lhs.is_negative_) {
    if (rhs.is_negative_) {
      BigInteger::RawSum(lhs, rhs, lhs);
      lhs.is_negative_ = true;
    } else {
      BigInteger::RawSubtract(lhs, rhs, lhs);
      lhs.is_negative_ = !lhs.is_negative_;
    }
  } else {
    if (rhs.is_negative_) {
      BigInteger::RawSubtract(lhs, rhs, lhs);
    } else {
      BigInteger::RawSum(lhs, rhs, lhs);
    }
  }

  lhs.CheckSign();
  return lhs;
}
BigInteger& operator-=(BigInteger& lhs, const BigInteger& rhs) {
  if (lhs.is_negative_) {
    if (rhs.is_negative_) {
      BigInteger::RawSubtract(lhs, rhs, lhs);
      lhs.is_negative_ = !lhs.is_negative_;
    } else {
      BigInteger::RawSum(lhs, rhs, lhs);
      lhs.is_negative_ = true;
    }
  } else {
    if (rhs.is_negative_) {
      BigInteger::RawSum(lhs, rhs, lhs);
    } else {
      BigInteger::RawSubtract(lhs, rhs, lhs);
    }
  }

  lhs.CheckSign();
  return lhs;
}
BigInteger& operator*=(BigInteger& lhs, const BigInteger& rhs) {
  BigInteger result = lhs * rhs;

  lhs = result;

  lhs.CheckSign();
  return lhs;
}

bool operator==(const BigInteger& lhs, const BigInteger& rhs) {
  bool equals = true;

  if ((lhs.buffer_.GetSize() == rhs.buffer_.GetSize()) && (lhs.is_negative_ == rhs.is_negative_)) {
    size_t upper_bound = lhs.buffer_.GetSize();

    for (size_t i = 0; (i < upper_bound) && equals; ++i) {
      equals = (lhs.buffer_[i] == rhs.buffer_[i]);
    }
  } else {
    equals = false;
  }

  return equals;
}
bool operator!=(const BigInteger& lhs, const BigInteger& rhs) {
  return !(lhs == rhs);
}
bool operator<(const BigInteger& lhs, const BigInteger& rhs) {
  size_t lhs_size = lhs.buffer_.GetSize();
  size_t rhs_size = rhs.buffer_.GetSize();
  size_t upper_bound = Min(rhs_size, lhs_size);
  bool result = false;

  if (lhs.IsNegative() != rhs.IsNegative()) {
    result = lhs.IsNegative();
  } else if (lhs_size != rhs_size) {
    result = (lhs_size < rhs_size);
  } else {
    for (size_t i = upper_bound; i > 0 && !result; --i) {
      result = lhs.buffer_[i - 1] < rhs.buffer_[i - 1];
    }
  }

  return result;
}
bool operator>(const BigInteger& lhs, const BigInteger& rhs) {
  size_t lhs_size = lhs.buffer_.GetSize();
  size_t rhs_size = rhs.buffer_.GetSize();
  size_t upper_bound = Min(rhs_size, lhs_size);
  bool result = false;

  if (lhs.IsNegative() != rhs.IsNegative()) {
    result = !lhs.IsNegative();
  } else if (lhs_size != rhs_size) {
    result = (lhs_size > rhs_size);
  } else {
    for (size_t i = upper_bound; i > 0 && !result; --i) {
      result = lhs.buffer_[i - 1] > rhs.buffer_[i - 1];
    }
  }

  return result;
}
bool operator<=(const BigInteger& lhs, const BigInteger& rhs) {
  return !(lhs > rhs);
}
bool operator>=(const BigInteger& lhs, const BigInteger& rhs) {
  return !(lhs < rhs);
}

std::istream& operator>>(std::istream& in, BigInteger& num) {
  char src[30005];
  in >> src;

  num = BigInteger(src);

  return in;
}
std::ostream& operator<<(std::ostream& out, const BigInteger& num) {
  const int32_t* first_unit_ptr = num.buffer_.Begin();

  out << (num.is_negative_ ? "-" : "") << *(num.buffer_.End() - 1);
  for (const int32_t* i = num.buffer_.End() - 2; i >= first_unit_ptr; --i) {
    int32_t unit = *i;
    out << (unit / 1000);
    unit %= 1000;
    out << (unit / 100);
    unit %= 100;
    out << (unit / 10);
    unit %= 10;
    out << unit;
  }

  return out;
}

bool BigInteger::IsNegative() const {
  return is_negative_;
}

void BigInteger::Normalize() {
  size_t size = buffer_.GetSize();

  for (size_t i = 0; i < size - 1; ++i) {
    if (buffer_[i] >= kBase) {
      buffer_[i + 1] += buffer_[i] / kBase;
      buffer_[i] %= kBase;
    }
  }

  if (buffer_[size - 1] >= kBase) {
    buffer_.PushBack(buffer_[size - 1] / kBase);
    buffer_[size - 1] %= kBase;
  }
}
void BigInteger::CheckSign() {
  if ((buffer_.GetSize() == 1) && (buffer_[0] == 0)) {
    is_negative_ = false;
  }
}

void BigInteger::RawSum(const BigInteger& lhs, const BigInteger& rhs, BigInteger& res) {
  size_t sum_bound = std::min(lhs.buffer_.GetSize(), rhs.buffer_.GetSize());

  res.buffer_.Resize(std::max(lhs.buffer_.GetSize(), rhs.buffer_.GetSize()));

  for (size_t i = 0; i < sum_bound; ++i) {
    res.buffer_[i] = lhs.buffer_[i] + rhs.buffer_[i];
  }

  for (size_t i = sum_bound; i < lhs.buffer_.GetSize(); ++i) {
    res.buffer_[i] = lhs.buffer_[i];
  }

  for (size_t i = sum_bound; i < rhs.buffer_.GetSize(); ++i) {
    res.buffer_[i] = rhs.buffer_[i];
  }

  res.Normalize();
}
void BigInteger::RawSubtract(const BigInteger& lhs, const BigInteger& rhs, BigInteger& res) {
  size_t l_size = lhs.buffer_.GetSize();
  size_t r_size = rhs.buffer_.GetSize();
  size_t subtraction_bound = std::min(l_size, r_size);
  size_t upper_bound = std::max(l_size, r_size);

  res.buffer_.Resize(upper_bound);

  for (size_t i = 0; i < subtraction_bound; ++i) {
    res.buffer_[i] = lhs.buffer_[i] - rhs.buffer_[i];
  }

  for (size_t i = subtraction_bound; i < l_size; ++i) {
    res.buffer_[i] = lhs.buffer_[i];
  }

  for (size_t i = subtraction_bound; i < r_size; ++i) {
    res.buffer_[i] = -rhs.buffer_[i];
  }

  for (size_t i = 0; i < upper_bound - 1; ++i) {
    if (res.buffer_[i] < 0) {
      res.buffer_[i + 1]--;
      res.buffer_[i] += kBase;
    }
  }

  if (res.buffer_[upper_bound - 1] < 0) {
    for (size_t i = 0; i < upper_bound - 1; ++i) {
      if (res.buffer_[i] != 0) {
        res.buffer_[i] = kBase - res.buffer_[i];
        res.buffer_[i + 1]++;
      }
    }
    res.buffer_[upper_bound - 1] = -res.buffer_[upper_bound - 1];
    res.is_negative_ = true;
  }

  size_t end_size;
  for (end_size = upper_bound; (end_size > 1) && (res.buffer_[end_size - 1] == 0); --end_size) {
  }

  res.buffer_.SetSize(end_size);
}
void BigInteger::RawMultiply(const BigInteger& lhs, const BigInteger& rhs, BigInteger& res) {
  size_t lhs_size = lhs.buffer_.GetSize();
  size_t rhs_size = lhs.buffer_.GetSize();
  size_t new_size = lhs_size + rhs_size;

  res.buffer_.Resize(new_size);

  int32_t shift;

  for (size_t i = 0; i < rhs_size; ++i) {
    shift = 0;

    for (size_t j = 0; j < lhs_size; ++j) {
      res.buffer_[i + j] += shift + lhs.buffer_[j] * rhs.buffer_[i];
      shift = res.buffer_[i + j] / kBase;
      res.buffer_[i + j] %= kBase;
    }

    res.buffer_[i + lhs_size] = shift;
  }

  size_t end_size;

  for (end_size = new_size; (end_size > 1) && (res.buffer_[end_size - 1] == 0); --end_size) {
  }

  res.buffer_.SetSize(end_size);
}

// ===========================================================
// Realization of Buffer class
// ===========================================================

#define Buff BigInteger::Buffer

template <typename T>
Buff<T>::Buffer(size_t beg_size) {
  array_ = new T[beg_size]{};
  container_size_ = beg_size;
}

template <typename T>
Buff<T>::Buffer(const Buffer<T>& src) {
  size_ = src.size_;
  container_size_ = src.container_size_;
  array_ = new T[container_size_]{};

  for (int i = 0; i < size_; ++i) {
    array_[i] = src.array_[i];
  }
}

template <typename T>
Buff<T>::~Buffer() {
  delete[] array_;
}

template <typename T>
size_t Buff<T>::GetSize() const {
  return size_;
}

template <typename T>
size_t Buff<T>::GetContainerSize() const {
  return size_;
}

template <typename T>
void Buff<T>::SetSize(size_t new_size) {
  size_ = new_size;
}

template <typename T>
T* Buff<T>::Begin() {
  return array_;
}

template <typename T>
T* Buff<T>::End() {
  return array_ + size_;
}

template <typename T>
const T* Buff<T>::Begin() const {
  return array_;
}

template <typename T>
const T* Buff<T>::End() const {
  return array_ + size_;
}

template <typename T>
T& Buff<T>::operator[](size_t idx) {
  return array_[idx];
}

template <typename T>
const T& Buff<T>::operator[](size_t idx) const {
  return array_[idx];
}

template <typename T>
void Buff<T>::Resize(size_t new_size) {
  if (new_size > size_) {
    size_t upper_bound = std::min(new_size, size_);
    auto new_array = new T[new_size];

    for (size_t i = 0; i < upper_bound; ++i) {
      new_array[i] = array_[i];
    }

    for (size_t i = upper_bound; i < new_size; ++i) {
      new_array[i] = 0;
    }

    delete[] array_;
    array_ = new_array;
  }

  size_ = new_size;
  container_size_ = new_size;
}

template <typename T>
void Buff<T>::PushBack(T elem) {
  if (size_ == container_size_) {
    container_size_ *= 2;
    auto new_array = new T[container_size_];

    for (size_t i = 0; i < size_; ++i) {
      new_array[i] = array_[i];
    }

    delete[] array_;
    array_ = new_array;
  }

  array_[size_++] = elem;
}