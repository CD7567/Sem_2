#include "cppstring.h"

template <typename T>
void Swap(T& first, T& second) {
  T tmp = first;
  first = second;
  second = tmp;
}

// =================================================
// Realization of class String
// =================================================

String::String() : size_(0), capacity_(0), buffer_(new char[1]{'\0'}) {
}
String::String(size_t beg_size, char symbol = '\0') : size_(beg_size), capacity_(beg_size) {
  if (beg_size == 0) {
    buffer_ = new char[1]{'\0'};
  } else {
    buffer_ = new char[beg_size + 1];

    for (size_t i = 0; i < size_; ++i) {
      buffer_[i] = symbol;
    }

    buffer_[size_] = '\0';
  }
}
String::String(const char* cstr) {  // NOLINT
  size_t cstr_size = CStrLen(cstr);
  size_ = cstr_size;
  capacity_ = cstr_size;

  buffer_ = new char[cstr_size + 1];

  for (size_t i = 0; i < cstr_size; ++i) {
    buffer_[i] = cstr[i];
  }

  buffer_[cstr_size] = '\0';
}
String::String(const char* cstr, size_t upper_bound) {
  size_ = upper_bound;
  capacity_ = upper_bound;

  buffer_ = new char[upper_bound + 1];
  for (size_t i = 0; i < upper_bound; ++i) {
    buffer_[i] = cstr[i];
  }

  buffer_[upper_bound] = '\0';
}
String::String(const String& src) : size_(src.size_), capacity_(src.capacity_), buffer_(new char[src.capacity_ + 1]) {
  for (size_t i = 0; i < size_; ++i) {
    buffer_[i] = src.buffer_[i];
  }
  buffer_[src.capacity_] = '\0';
}

String::~String() {
  delete[] buffer_;
}

size_t String::Size() const {
  return size_;
}
size_t String::Length() const {
  return size_;
}
size_t String::Capacity() const {
  return capacity_;
}

char* String::CStr() {
  return buffer_;
}
char* String::Data() {
  char* res;

  if (size_ != 0) {
    res = this->buffer_;
  } else {
    res = nullptr;
  }

  return res;
}
const char* String::CStr() const {
  return buffer_;
}
const char* String::Data() const {
  char* res;

  if (size_ != 0) {
    res = this->buffer_;
  } else {
    res = nullptr;
  }

  return res;
}

bool String::Empty() const {
  return (size_ == 0);
}

void String::Swap(String& other) {
  ::Swap(this->size_, other.size_);
  ::Swap(this->capacity_, other.capacity_);
  ::Swap(this->buffer_, other.buffer_);
}

void String::PushBack(char elem) {
  if (capacity_ == 0) {
    Reserve(2);
  } else if (size_ == capacity_) {
    Reserve(2 * capacity_);
  }

  buffer_[size_] = elem;
  buffer_[size_ + 1] = '\0';
  size_++;
}
char String::PopBack() {
  buffer_[size_] = '\0';
  return buffer_[--size_];
}
void String::Clear() {
  if (size_ != 0) {
    buffer_[0] = '\0';
  }
  size_ = 0;
}

void String::Reserve(size_t new_capacity) {
  if (new_capacity > capacity_) {
    auto* new_buffer = new char[new_capacity + 1];
    capacity_ = new_capacity;

    for (size_t i = 0; i < size_; ++i) {
      new_buffer[i] = buffer_[i];
    }

    new_buffer[size_] = '\0';

    delete[] buffer_;
    buffer_ = new_buffer;
  }
}
void String::Resize(size_t new_size, char symbol) {
  if (new_size > capacity_) {
    Reserve(new_size);

    for (size_t i = size_; i < new_size; ++i) {
      buffer_[i] = symbol;
    }

    buffer_[new_size] = '\0';
  }

  size_ = new_size;
}
void String::ShrinkToFit() {
  if (size_ != capacity_) {
    auto* new_buffer = new char[size_ + 1];

    for (size_t i = 0; i < size_; ++i) {
      new_buffer[i] = buffer_[i];
    }

    new_buffer[size_] = '\0';

    delete[] buffer_;
    buffer_ = new_buffer;
    capacity_ = size_;
  }
}

char& String::At(size_t idx) {
  if (idx >= size_) {
    throw StringOutOfRange();
  } else {
    return buffer_[idx];
  }
}
char& String::Front() {
  return buffer_[0];
}
char& String::Back() {
  return buffer_[size_ - 1];
}
const char& String::At(size_t idx) const {
  if (idx >= size_) {
    throw StringOutOfRange();
  } else {
    return buffer_[idx];
  }
}
const char& String::Front() const {
  return buffer_[0];
}
const char& String::Back() const {
  return buffer_[size_ - 1];
}

String operator+(const String& lhs, const String& rhs) {
  String result = lhs;
  result += rhs;

  return result;
}
String& operator+=(String& lhs, const String& rhs) {
  if (lhs.size_ + rhs.size_ > lhs.capacity_) {
    lhs.Reserve(2 * (lhs.size_ + rhs.size_));
  }

  for (size_t i = 0; i < rhs.size_; ++i) {
    lhs[lhs.size_ + i] = rhs[i];
  }

  lhs.buffer_[lhs.size_ + rhs.size_] = '\0';

  lhs.size_ += rhs.size_;
  return lhs;
}

bool operator==(const String& lhs, const String& rhs) {
  bool equals = true;

  if (lhs.size_ == rhs.size_) {
    for (size_t i = 0; (i < lhs.size_) && (i < rhs.size_) && equals; ++i) {
      equals = (lhs.buffer_[i] == rhs.buffer_[i]);
    }
  } else {
    equals = false;
  }

  return equals;
}
bool operator!=(const String& lhs, const String& rhs) {
  return !(lhs == rhs);
}
bool operator<(const String& lhs, const String& rhs) {
  bool less = true;

  if (lhs.size_ != 0 && rhs.size_ != 0) {
    size_t idx = 0;

    for (; (idx < lhs.size_) && (idx < rhs.size_) && (lhs[idx] == rhs[idx]); ++idx) {
    }

    if (idx != lhs.size_) {
      if (idx == rhs.size_) {
        less = false;
      } else {
        less = lhs[idx] < rhs[idx];
      }
    } else {
      if (idx == rhs.size_) {
        less = false;
      }
    }
  } else {
    less = (lhs.size_ == 0) && (rhs.size_ != 0);
  }

  return less;
}
bool operator<=(const String& lhs, const String& rhs) {
  return (lhs < rhs) || (lhs == rhs);
}
bool operator>(const String& lhs, const String& rhs) {
  bool greater = true;

  if (lhs.size_ != 0 && rhs.size_ != 0) {
    size_t idx = 0;

    for (; (idx < lhs.size_) && (idx < rhs.size_) && (lhs[idx] == rhs[idx]); ++idx) {
    }

    if (idx != rhs.size_) {
      if (idx == lhs.size_) {
        greater = false;
      } else {
        greater = rhs[idx] < lhs[idx];
      }
    } else {
      if (idx == lhs.size_) {
        greater = false;
      }
    }
  } else {
    greater = (lhs.size_ != 0) && (rhs.size_ == 0);
  }

  return greater;
}
bool operator>=(const String& lhs, const String& rhs) {
  return !(lhs < rhs);
}

std::ostream& operator<<(std::ostream& out, const String& str) {
  out << str.CStr();
  return out;
}

size_t String::CStrLen(const char* cstr) {
  size_t size;

  for (size = 0; cstr[size] != '\0'; ++size) {
  }

  return size;
}