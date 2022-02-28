#include "rational.h"
#include <cctype>
#include <cstring>

int64_t GCD(int64_t lhs, int64_t rhs) {
  int64_t result;

  if ((lhs == 0) || (rhs == 0)) {
    result = ((lhs == 0) ? std::abs(rhs) : std::abs(lhs));
  } else {
    lhs %= rhs;
    if (lhs != 0) {
      rhs %= lhs;
    }

    result = GCD(lhs, rhs);
  }

  return result;
}

// =================================================
// Realization of class Rationalize
// =================================================

Rational operator+(const Rational& src) {
  return {src};
}
Rational operator-(const Rational& src) {
  Rational src_clone(src);
  src_clone.numerator_ = -src.numerator_;

  return src_clone;
}
Rational& operator++(Rational& src) {
  src.numerator_ += src.denominator_;
  return src;
}
Rational& operator--(Rational& src) {
  src.numerator_ -= src.denominator_;
  return src;
}
Rational operator++(Rational& src, int) {
  Rational src_clone(src);
  ++src;
  return src_clone;
}
Rational operator--(Rational& src, int) {
  Rational src_clone(src);
  --src;
  return src_clone;
}

Rational operator+(const Rational& lhs, const Rational& rhs) {
  return {lhs.numerator_ * rhs.denominator_ + rhs.numerator_ * lhs.denominator_, lhs.denominator_ * rhs.denominator_};
}
Rational operator-(const Rational& lhs, const Rational& rhs) {
  return lhs + (-rhs);
}
Rational operator*(const Rational& lhs, const Rational& rhs) {
  return {lhs.numerator_ * rhs.numerator_, lhs.denominator_ * rhs.denominator_};
}
Rational operator/(const Rational& lhs, const Rational& rhs) {
  if (rhs.numerator_ == 0) {
    throw RationalDivisionByZero();
  } else {
    return {lhs.numerator_ * rhs.denominator_, lhs.denominator_ * rhs.numerator_};
  }
}
Rational& operator+=(Rational& lhs, const Rational& rhs) {
  lhs.numerator_ = lhs.numerator_ * rhs.denominator_ + rhs.numerator_ * lhs.denominator_;
  lhs.denominator_ = lhs.denominator_ * rhs.denominator_;
  lhs.Rationalize();

  return lhs;
}
Rational& operator-=(Rational& lhs, const Rational& rhs) {
  lhs.numerator_ = lhs.numerator_ * rhs.denominator_ - rhs.numerator_ * lhs.denominator_;
  lhs.denominator_ = lhs.denominator_ * rhs.denominator_;
  lhs.Rationalize();

  return lhs;
}
Rational& operator*=(Rational& lhs, const Rational& rhs) {
  lhs.numerator_ = lhs.numerator_ * rhs.numerator_;
  lhs.denominator_ = lhs.denominator_ * rhs.denominator_;
  lhs.Rationalize();

  return lhs;
}
Rational& operator/=(Rational& lhs, const Rational& rhs) {
  if (rhs.numerator_ == 0) {
    throw RationalDivisionByZero();
  } else {
    lhs.numerator_ = lhs.numerator_ * rhs.denominator_;
    lhs.denominator_ = lhs.denominator_ * rhs.numerator_;
    lhs.Rationalize();

    return lhs;
  }
}

bool operator==(const Rational& lhs, const Rational& rhs) {
  return (lhs.numerator_ == rhs.numerator_) && (lhs.denominator_ == rhs.denominator_);
}
bool operator!=(const Rational& lhs, const Rational& rhs) {
  return !(lhs == rhs);
}
bool operator<(const Rational& lhs, const Rational& rhs) {
  Rational help = lhs - rhs;

  return help.numerator_ < 0;
}
bool operator<=(const Rational& lhs, const Rational& rhs) {
  return !(lhs > rhs);
}
bool operator>(const Rational& lhs, const Rational& rhs) {
  Rational help = lhs - rhs;

  return help.numerator_ > 0;
}
bool operator>=(const Rational& lhs, const Rational& rhs) {
  return !(lhs < rhs);
}

std::istream& operator>>(std::istream& in, Rational& num) {
  char input[30], numerator[15], denominator[15]{'1', '\0'};
  in >> input;

  size_t slash_pos_idx = 0, input_len = strlen(input);

  while ((slash_pos_idx < input_len) && (std::isdigit(input[slash_pos_idx]) || input[slash_pos_idx] == '-')) {
    ++slash_pos_idx;
  }

  for (size_t i = 0; i < slash_pos_idx; ++i) {
    numerator[i] = input[i];
  }
  numerator[slash_pos_idx] = '\0';

  size_t i = 0;
  for (size_t j = slash_pos_idx + 1; j < input_len; ++i, ++j) {
    denominator[i] = input[j];
  }

  if (i != 0) {
    denominator[i] = '\0';
  }

  num.numerator_ = std::stoll(numerator);
  num.denominator_ = std::stoll(denominator);

  if (num.denominator_ == 0) {
    throw RationalDivisionByZero();
  }
  num.Rationalize();

  return in;
}

std::ostream& operator<<(std::ostream& out, const Rational& num) {
  out << num.numerator_;

  if (num.denominator_ != 1) {
    out << '/' << num.denominator_;
  }

  return out;
}

void Rational::Rationalize() {
  if (denominator_ < 0) {
    numerator_ = -numerator_;
    denominator_ = -denominator_;
  }

  int64_t divisor = GCD(numerator_, denominator_);

  numerator_ /= divisor;
  denominator_ /= divisor;
}