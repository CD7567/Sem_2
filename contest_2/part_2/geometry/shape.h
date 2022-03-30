#pragma once

#include <memory>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

namespace geometry {
class Vector;
class Point;
class Segment;
class Ray;
class Line;
class Circle;
class Polygon;

class IShape {

 public:
  virtual ~IShape() noexcept = default;

  virtual IShape& Move(const geometry::Vector&) noexcept = 0;
  [[nodiscard]] virtual bool ContainsPoint(const geometry::Point&) const noexcept = 0;
  [[nodiscard]] virtual bool CrossesSegment(const geometry::Segment&) const noexcept = 0;
  [[nodiscard]] virtual std::unique_ptr<IShape> Clone() const = 0;
  [[nodiscard]] virtual std::string ToString() const noexcept = 0;
};
}  // namespace geometry