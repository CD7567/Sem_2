#include <algorithm>
#include <cctype>
#include <cstring>
#include <cstdint>
#include <iostream>

class LightNumBuffer {
 public:
  explicit LightNumBuffer(size_t beg_size) {
    array_ = new int32_t[beg_size]{};
    container_size_ = beg_size;
  }
  LightNumBuffer(const LightNumBuffer& src) {
    size_ = src.size_;
    container_size_ = src.container_size_;
    array_ = new int32_t[container_size_]{};

    for (size_t i = 0; i < size_; ++i) {
      array_[i] = src.array_[i];
    }
  }

  explicit LightNumBuffer(const char* src) {
    bool is_src_negative = (src[0] == '-');
    bool has_sign = !std::isdigit(src[0]);
    size_t dec_length = strlen(src);
    auto* unit = new char[kBaseDecLen + 1]{};
    is_negative_ = is_src_negative;
    array_ = new int32_t[2];
    container_size_ = 2;

    size_t i;
    size_t k;
    for (i = dec_length - 1; i >= kBaseDecLen; i -= kBaseDecLen) {
      for (size_t j = kBaseDecLen; j != 0; --j) {
        unit[j - 1] = src[i + j - kBaseDecLen];
      }

      PushBack(std::stoi(unit));
    }

    for (i += 1, k = kBaseDecLen - 1; i > static_cast<size_t>(has_sign); --i, --k) {
      unit[k] = src[i - 1];
    }

    if (k < kBaseDecLen) {
      for (++k; k != 0; --k) {
        unit[k - 1] = '0';
      }
    }

    PushBack(std::stoi(unit));
    delete[] unit;
  }

  ~LightNumBuffer() {
    delete[] array_;
  };

  [[nodiscard]] const int32_t* Begin() const {
    return array_;
  }
  [[nodiscard]] const int32_t* End() const {
    return array_ + size_;
  }

  int32_t& operator[](size_t idx) {
    return array_[idx];
  };
  const int32_t& operator[](size_t idx) const {
    return array_[idx];
  };

  LightNumBuffer operator-(const LightNumBuffer& rhs) {
    LightNumBuffer res(std::max(size_, rhs.size_));
    size_t subtraction_bound = std::min(size_, rhs.size_);
    size_t upper_bound = std::max(size_, rhs.size_);
    res.size_ = std::max(size_, rhs.size_);

    for (size_t i = 0; i < subtraction_bound; ++i) {
      res.array_[i] = array_[i] - rhs.array_[i];
    }

    for (size_t i = subtraction_bound; i < size_; ++i) {
      res.array_[i] = array_[i];
    }

    for (size_t i = subtraction_bound; i < rhs.size_; ++i) {
      res.array_[i] = -rhs.array_[i];
    }

    for (size_t i = 0; i < upper_bound - 1; ++i) {
      if (res.array_[i] < 0) {
        res.array_[i + 1]--;
        res.array_[i] += kBase;
      }
    }

    if (res.array_[upper_bound - 1] < 0) {
      for (size_t i = 0; i < upper_bound - 1; ++i) {
        if (res.array_[i] != 0) {
          res.array_[i] = kBase - res.array_[i];
          res.array_[i + 1]++;
        }
      }
      res.array_[upper_bound - 1] = -res.array_[upper_bound - 1];
      res.is_negative_ = true;
    }

    for (res.size_ = upper_bound; (res.size_ > 1) && (res.array_[res.size_ - 1] == 0); --res.size_) {
    }

    return res;
  }

  friend std::ostream& operator<<(std::ostream& out, const LightNumBuffer& num) {
    const int32_t* first_unit_ptr = num.Begin();

    out << (num.is_negative_ ? "-" : "") << *(num.End() - 1);
    for (const int32_t* i = num.End() - 2; i >= first_unit_ptr; --i) {
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

  void PushBack(int32_t elem) {
    if (size_ == container_size_) {
      container_size_ *= 2;
      auto* new_array = new int32_t[container_size_];

      for (size_t i = 0; i < size_; ++i) {
        new_array[i] = array_[i];
      }

      delete[] array_;
      array_ = new_array;
    }

    array_[size_++] = elem;
  }

 private:
  static const int32_t kBase = 10000;
  static const size_t kBaseDecLen = 4;

  int32_t* array_;
  bool is_negative_ = false;
  size_t size_ = 0;
  size_t container_size_;

  void Normalize() {
    for (size_t i = 0; i < size_ - 1; ++i) {
      array_[i + 1] += array_[i] / kBase;
      array_[i] %= kBase;
    }

    if (array_[size_ - 1] > kBase) {
      PushBack(array_[size_ - 1] / kBase);
      array_[size_ - 2] %= kBase;
    }
  }
};

int main() {
  char str[10005];

  std::cin >> str;
  LightNumBuffer lhs(str);

  std::cin >> str;
  LightNumBuffer rhs(str);

  LightNumBuffer res = lhs - rhs;

  std::cout << res;
}