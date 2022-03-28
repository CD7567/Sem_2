#pragma once

#include "shape.h"
#include "point.h"

namespace geometry {
  class Segment :  {
   public:
    Segment(const Point&, const Point&);

    IShape& Move(const geometry::Vector&) noexcept override;

    bool ContainsPoint(const geometry::Point&) const noexcept override;
    bool CrossesSegment(const geometry::Segment&) const noexcept override;

    std::unique_ptr<IShape> Clone() const override;

    std::string ToString() const noexcept override;

    Point begin, end;
  };
}