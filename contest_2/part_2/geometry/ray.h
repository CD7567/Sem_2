#pragma once

#include "shape.h"

namespace geometry {
  class Ray : IShape {
    Ray(const Point&, const Point&);

    IShape& Move(const geometry::Vector&) noexcept override;

    bool ContainsPoint(const geometry::Point&) const noexcept override;
    bool CrossesSegment(const geometry::Segment&) const noexcept override;

    std::unique_ptr<IShape> Clone() const override;

    std::string ToString() const noexcept override;

    Point begin_;
    Vector direction_;
  };
}