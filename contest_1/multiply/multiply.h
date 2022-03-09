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

  LightNumBuffer operator*(const LightNumBuffer& rhs) {
    size_t new_size = size_ + rhs.size_;
    LightNumBuffer res(new_size);

    int32_t shift;
    for (size_t i = 0; i < rhs.size_; ++i) {
      shift = 0;

      for (size_t j = 0; j < size_; ++j) {
        res.array_[i + j] += shift + array_[j] * rhs.array_[i];
        shift = res.array_[i + j] / kBase;
        res.array_[i + j] %= kBase;
      }

      res.array_[i + size_] = shift;
    }

    size_t end_size;
    for (end_size = new_size; (end_size > 1) && (res.array_[end_size - 1] == 0); --end_size) {
    }

    res.size_ = end_size;
    res.is_negative_ = is_negative_ xor rhs.is_negative_;

    if (res.size_ == 1 && res.array_[0] == 0) {
      res.is_negative_ = false;
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
};

int main() {
  char str[10005];

  std::cin >> str;
  LightNumBuffer lhs(str);

  std::cin >> str;
  LightNumBuffer rhs(str);

  std::cout << lhs * rhs;
}