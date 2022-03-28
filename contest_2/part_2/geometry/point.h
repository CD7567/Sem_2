#pragma once

#include "shape.h"
#include "vector.h"
#include "segment.h"

namespace geometry {
  class Point : public geometry::IShape {
   public:
    Point(int64_t, int64_t);

    Point& Move(const geometry::Vector&) noexcept override;

    //[[nodiscard]] bool ContainsPoint(const geometry::Point&) const noexcept override;
    [[nodiscard]] bool CrossesSegment(const geometry::Segment&) const noexcept override;

    [[nodiscard]] std::unique_ptr<IShape> Clone() const override;

    [[nodiscard]] std::string ToString() const noexcept override;

    int64_t x_, y_;
  };
}