#pragma once

#include "point.h"

namespace geometry {
class Circle : public IShape {
 public:
  Circle(const Point&, int64_t);

  ~Circle() noexcept override = default;

  Circle& Move(const geometry::Vector&) noexcept override;

  [[nodiscard]] bool ContainsPoint(const geometry::Point&) const noexcept override;
  [[nodiscard]] bool CrossesSegment(const geometry::Segment&) const noexcept override;

  [[nodiscard]] std::unique_ptr<IShape> Clone() const override;

  [[nodiscard]] std::string ToString() const noexcept override;

  Point center_;
  int64_t radius_;
};
}  // namespace geometry