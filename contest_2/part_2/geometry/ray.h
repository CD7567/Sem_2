#pragma once

#include "vector.h"
#include "point.h"

namespace geometry {
class Ray : public IShape {
 public:
  Ray(const Point&, const Vector&);
  Ray(const Point&, const Point&);

  ~Ray() noexcept override = default;

  Ray& Move(const geometry::Vector&) noexcept override;

  [[nodiscard]] bool ContainsPoint(const geometry::Point&) const noexcept override;
  [[nodiscard]] bool CrossesSegment(const geometry::Segment&) const noexcept override;

  [[nodiscard]] std::unique_ptr<IShape> Clone() const override;

  [[nodiscard]] std::string ToString() const noexcept override;

  Point begin_;
  Vector direction_;
};
}  // namespace geometry