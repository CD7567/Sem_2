#pragma once

#include "shape.h"

namespace geometry {
class Vector {
 public:
  Vector(int64_t dim_x, int64_t dim_y);
  Vector(const Point&, const Point&);

  Vector& operator=(const Vector&) = default;

  Vector operator+() const;
  Vector operator-() const;

  Vector& operator+=(const Vector&);
  Vector& operator-=(const Vector&);
  Vector& operator*=(int64_t);
  Vector& operator/=(int64_t);

  Vector operator+(const Vector&) const;
  Vector operator-(const Vector&) const;
  Vector operator*(int64_t) const;
  Vector operator/(int64_t) const;
  friend Vector operator*(int64_t, const Vector&);

  bool operator==(const Vector&) const;
  bool operator!=(const Vector&) const;

  [[nodiscard]] bool IsParallel(const Vector&) const;
  [[nodiscard]] bool IsCodirectional(const Vector&) const;

  [[nodiscard]] int64_t ScalarMultiply(const Vector&) const;
  [[nodiscard]] int64_t VectorMultiply(const Vector&) const;

  [[nodiscard]] std::string ToString() const noexcept;

  [[nodiscard]] long double GetLength() const;

  int64_t dim_x_, dim_y_;
};
}  // namespace geometry