#pragma once

#include <vector>
#include "shape.h"

namespace geometry {
class Polygon : public IShape {
 public:
  explicit Polygon(const std::vector<Point>&);

  ~Polygon() noexcept override = default;

  Polygon& Move(const geometry::Vector&) noexcept override;

  [[nodiscard]] bool ContainsPoint(const geometry::Point&) const noexcept override;
  [[nodiscard]] bool CrossesSegment(const geometry::Segment&) const noexcept override;

  [[nodiscard]] std::unique_ptr<IShape> Clone() const override;

  std::string ToString() const noexcept override;

  std::vector<Point> vertex_;
};
}  // namespace geometry