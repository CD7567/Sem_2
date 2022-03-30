#include "../segment.h"
#include "../ray.h"
#include "../line.h"

geometry::Ray::Ray(const geometry::Point& begin, const geometry::Vector& direction)
    : begin_(begin), direction_(direction) {
}

geometry::Ray::Ray(const geometry::Point& begin, const geometry::Point& end) : begin_(begin), direction_({begin, end}) {
}

geometry::Ray& geometry::Ray::Move(const geometry::Vector& vect) noexcept {
  begin_.Move(vect);

  return *this;
}

bool geometry::Ray::ContainsPoint(const geometry::Point& point) const noexcept {
  return Vector(begin_, point).IsCodirectional(direction_);
}
bool geometry::Ray::CrossesSegment(const geometry::Segment& segment) const noexcept {
  Point other_ray_point = begin_;
  Line ray_line(begin_, other_ray_point.Move(direction_));
  bool result = false;

  if (ray_line.CrossesSegment(segment)) {
    Vector vect_segment(segment.begin_, segment.end_);
    Vector vect_from_segment(segment.begin_, begin_);

    int64_t prod_1 = vect_from_segment.VectorMultiply(vect_segment);
    int64_t prod_2 = direction_.VectorMultiply(vect_segment);

    result = prod_1 * prod_2 <= 0;
  }

  return result;
}

std::unique_ptr<geometry::IShape> geometry::Ray::Clone() const {
  return std::make_unique<Ray>(*this);
}

std::string geometry::Ray::ToString() const noexcept {
  return "Ray(" + begin_.ToString() + ", " + direction_.ToString() + ")";
}