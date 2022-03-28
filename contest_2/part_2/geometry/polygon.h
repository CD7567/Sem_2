#pragma once

#include <vector>
#include "shape.h"

namespace geometry {
  class Polygon : IShape {
    Polygon(const std::vector<Point>&);

    IShape & Move(const geometry::Vector &) noexcept override;

    bool ContainsPoint(const geometry::Point &) const noexcept override;
    bool CrossesSegment(const geometry::Segment &) const noexcept override;

    std::unique_ptr<IShape> Clone() const override;

    std::string ToString() const noexcept override;

    std::vector<Point> vertex_;
  };
}