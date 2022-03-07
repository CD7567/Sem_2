#include "cppstring.h"

template <typename T>
void Swap(T& first, T& second) {
  T tmp = first;
  first = second;
  second = tmp;
}

char* String::CStr() {
  char* res;

  if (size_ != 0) {
    res = this->buffer_;
  } else {
    res = new char{'\0'};
  }

  return res;
}

const char* String::CStr() const {
  char* res;

  if (size_ != 0) {
    res = this->buffer_;
  } else {
    res = new char{'\0'};
  }

  return res;
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

const char* String::Data() const {
  char* res;

  if (size_ != 0) {
    res = this->buffer_;
  } else {
    res = nullptr;
  }

  return res;
}

void String::Swap(String& other) {
  ::Swap(this->size_, other.size_);
  ::Swap(this->buffer_size_, other.buffer_size_);
  ::Swap(this->buffer_, other.buffer_);
}

void String::PushBack(char elem) {
  if (buffer_size_ == 0) {
    Reserve(2);
  } else if (size_ == buffer_size_) {
    Reserve(2 * buffer_size_);
  }

  buffer_[size_] = elem;
  size_++;
}

void String::Reserve(size_t new_capacity) {
  if (new_capacity > buffer_size_) {
    auto* new_buffer = new char[new_capacity];
    buffer_size_ = new_capacity;

    for (size_t i = 0; i < size_; ++i) {
      new_buffer[i] = buffer_[i];
    }

    delete[] buffer_;
    buffer_ = new_buffer;
  }
}

void String::Resize(size_t new_size, char symbol) {
  if (new_size > buffer_size_) {
    Reserve(new_size);

    for (size_t i = size_; i < new_size; ++i) {
      buffer_[i] = symbol;
    }
  }

  size_ = new_size;
}

void String::ShrinkToFit() {
  if (size_ != buffer_size_) {
    auto* new_buffer = new char[size_];

    for (size_t i = 0; i < size_; ++i) {
      new_buffer[i] = buffer_[i];
    }

    delete[] buffer_;
    buffer_ = new_buffer;
    buffer_size_ = size_;
  }
}

String operator+(const String& lhs, const String& rhs) {
  String result(lhs.size_ + rhs.size_);

  for (size_t i = 0; i < lhs.size_; ++i) {
    result[i] = lhs[i];
  }

  for (size_t i = 0; i < rhs.size_; ++i) {
    result[lhs.size_ + i] = rhs[i];
  }

  return result;
}

String& operator+=(String& lhs, const String& rhs) {
  lhs.Reserve(lhs.size_ + rhs.size_);
  for (size_t i = 0; i < rhs.size_; ++i) {
    lhs[lhs.size_ + i] = rhs[i];
  }
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