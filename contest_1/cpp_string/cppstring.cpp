#include "cppstring.h"

template <typename T>
void Swap(T& first, T& second) {
  T& tmp = first;
  first = second;
  second = tmp;
}

char* String::CStr() const {
  char* res;

  if (size_ != 0) {
    res
  } else {
    cstr = nullptr;
  }

  return cstr;
}
char* String::Data() const {
  char* cstr;

  if (size_ != 0) {
    cstr = new char[this->size_ + 1];

    for (size_t i = 0; i < size_; ++i) {
      cstr[i] = buffer_[i];
    }

    cstr[this->size_] = '\0';
  } else {
    cstr = nullptr;
  }

  return cstr;
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
    char* new_buffer = new char[new_capacity];
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
    char* new_buffer = new char[size_];

    for (size_t i = 0; i < size_; ++i) {
      new_buffer[i] = buffer_[i];
    }

    delete[] buffer_;
    buffer_ = new_buffer;
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