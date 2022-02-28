#pragma once

#include <cstdint>
#include <stdexcept>
#include <iostream>

class RationalDivisionByZero : public std::runtime_error {
 public:
  RationalDivisionByZero() : std::runtime_error("RationalDivisionByZero") {
  }
};

class Rational {
 public:
  Rational() : numerator_(0ll), denominator_(1ll) {
  }
  Rational(int64_t num) : numerator_(num), denominator_(1ll) {  // NOLINT
  }
  Rational(int64_t num, int64_t den) : numerator_(num), denominator_(den) {
    if (denominator_ == 0) {
      throw RationalDivisionByZero();
    } else {
      Rationalize();
    }
  }
  Rational(const Rational& src) = default;

  int32_t GetNumerator() const {
    return static_cast<int32_t>(numerator_);
  }
  int32_t GetDenominator() const {
    return static_cast<int32_t>(denominator_);
  }

  void SetNumerator(const int64_t num) {
    numerator_ = num;
    Rationalize();
  }
  void SetDenominator(const int64_t den) {
    if (den == 0) {
      throw RationalDivisionByZero();
    } else {
      denominator_ = den;
      Rationalize();
    }
  }

  friend Rational operator+(const Rational&);
  friend Rational operator-(const Rational&);
  friend Rational& operator++(Rational&);
  friend Rational& operator--(Rational&);
  friend Rational operator++(Rational&, int);
  friend Rational operator--(Rational&, int);

  friend Rational operator+(const Rational&, const Rational&);
  friend Rational operator-(const Rational&, const Rational&);
  friend Rational operator*(const Rational&, const Rational&);
  friend Rational operator/(const Rational&, const Rational&);
  friend Rational& operator+=(Rational&, const Rational&);
  friend Rational& operator-=(Rational&, const Rational&);
  friend Rational& operator*=(Rational&, const Rational&);
  friend Rational& operator/=(Rational&, const Rational&);

  friend bool operator==(const Rational&, const Rational&);
  friend bool operator!=(const Rational&, const Rational&);
  friend bool operator<(const Rational&, const Rational&);
  friend bool operator<=(const Rational&, const Rational&);
  friend bool operator>(const Rational&, const Rational&);
  friend bool operator>=(const Rational&, const Rational&);

  friend std::istream& operator>>(std::istream&, Rational&);
  friend std::ostream& operator<<(std::ostream&, const Rational&);

 private:
  int64_t numerator_;
  int64_t denominator_;

  void Rationalize();
};