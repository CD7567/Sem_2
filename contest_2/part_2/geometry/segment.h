#pragma once

#include "point.h"

namespace geometry {
class Segment : public IShape {
 public:
  Segment(const Point&, const Point&);

  ~Segment() noexcept override = default;

  Segment& Move(const geometry::Vector&) noexcept override;

  [[nodiscard]] bool ContainsPoint(const geometry::Point&) const noexcept override;
  [[nodiscard]] bool CrossesSegment(const geometry::Segment&) const noexcept override;

  [[nodiscard]] std::unique_ptr<IShape> Clone() const override;

  [[nodiscard]] std::string ToString() const noexcept override;

  Point begin_, end_;
};
}  // namespace geometry