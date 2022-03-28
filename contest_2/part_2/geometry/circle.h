#pragma once

#include "shape.h"

namespace geometry {
  class Circle : public IShape {
    Circle(const Point&, int64_t);

    IShape & Move(const geometry::Vector &) noexcept override;

    bool ContainsPoint(const geometry::Point &) const noexcept override;
    bool CrossesSegment(const geometry::Segment &) const noexcept override;

    std::unique_ptr<IShape> Clone() const override;

    std::string ToString() const noexcept override;

    Point center;
    int64_t radius;
  };
}