#pragma once

#include "shape.h"

namespace geometry {
class Point : public IShape {
 public:
  Point(int64_t, int64_t);

  ~Point() noexcept override = default;

  Point& Move(const Vector&) noexcept override;

  [[nodiscard]] bool ContainsPoint(const Point&) const noexcept override;
  [[nodiscard]] bool CrossesSegment(const Segment&) const noexcept override;

  [[nodiscard]] std::unique_ptr<IShape> Clone() const override;

  [[nodiscard]] std::string ToString() const noexcept override;

  geometry::Vector operator-(const Point&) const;

  [[nodiscard]] long double GetDistance(const Point& other) const;
  [[nodiscard]] long double GetDistance(const Segment&) const;

  bool operator==(const Point&) const;

  int64_t x_, y_;
};
}  // namespace geometry