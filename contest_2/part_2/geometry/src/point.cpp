#include "../point.h"

geometry::Point::Point(int64_t x, int64_t y) : x_(x), y_(y){}

geometry::Point& geometry::Point::Move(const geometry::Vector& vect) noexcept {
  x_ += vect.dim_x_;
  y_ += vect.dim_y_;
}

bool geometry::Point::ContainsPoint(const geometry::Point& point) const noexcept {
  return x_ == point.x_ && y_ == point.y_;
}
bool geometry::Point::CrossesSegment(const geometry::Segment& segment) const noexcept {
  return Vector(segment.begin, *this).IsCodirectional(Vector(*this, segment.end));
}

std::unique_ptr<geometry::IShape> geometry::Point::Clone() const {
  return std::make_unique<Point>(x_, y_);
}

std::string geometry::Point::ToString() const noexcept {
  return "Point(" + std::to_string(x_) + ", " + std::to_string(y_) + ")";
}