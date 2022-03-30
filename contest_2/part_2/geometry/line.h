#pragma once

#include "shape.h"

namespace geometry {
class Line : public IShape {
 public:
  Line(int64_t, int64_t, int64_t);
  Line(const Point&, const Point&);

  ~Line() noexcept override = default;

  Line& Move(const geometry::Vector&) noexcept override;

  [[nodiscard]] bool ContainsPoint(const geometry::Point&) const noexcept override;
  [[nodiscard]] bool CrossesSegment(const geometry::Segment&) const noexcept override;

  [[nodiscard]] std::unique_ptr<IShape> Clone() const override;

  [[nodiscard]] std::string ToString() const noexcept override;

  int64_t a_, b_, c_;
};
}  // namespace geometry