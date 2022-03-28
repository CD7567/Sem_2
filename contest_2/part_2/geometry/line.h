#pragma once

#include "shape.h"

namespace geometry {
  class Line : IShape {
    Line(const Point&, const Point&);

    IShape& Move(const geometry::Vector&) noexcept override;

    bool ContainsPoint(const geometry::Point&) const noexcept override;
    bool CrossesSegment(const geometry::Segment&) const noexcept override;

    std::unique_ptr<IShape> Clone() const override;

    std::string ToString() const noexcept override;

    int64_t a_, b_, c_;
  };
}