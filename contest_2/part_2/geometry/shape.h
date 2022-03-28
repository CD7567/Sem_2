#pragma once

//#include "library.h"
#include <memory>
#include <string>
#include <cstdint>
#include <iostream>
#include "point.h"
#include "vector.h"
#include "segment.h"

namespace geometry {
  class IShape {
   public:
    virtual IShape& Move(const geometry::Vector&) noexcept = 0;
    [[nodiscard]] virtual bool ContainsPoint(const geometry::Point&) const noexcept = 0;
    [[nodiscard]] virtual bool CrossesSegment(const geometry::Segment&) const noexcept = 0;
    [[nodiscard]] virtual std::unique_ptr<IShape> Clone() const = 0;
    [[nodiscard]] virtual std::string ToString() const noexcept = 0;
  };
}