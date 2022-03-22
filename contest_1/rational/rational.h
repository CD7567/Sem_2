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
  Rational();
  Rational(int64_t num);  //  NOLINT
  Rational(int64_t num, int64_t den);
  Rational(const Rational& src) = default;

  [[nodiscard]] int64_t GetNumerator() const;
  [[nodiscard]] int64_t GetDenominator() const;

  void SetNumerator(int64_t num);
  void SetDenominator(int64_t den);

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