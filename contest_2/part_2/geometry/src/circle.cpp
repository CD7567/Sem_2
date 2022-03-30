#include "../circle.h"

#include "../segment.h"

geometry::Circle::Circle(const Point& center, int64_t radius) : center_(center), radius_(radius) {
}

geometry::Circle& geometry::Circle::Move(const geometry::Vector& vect) noexcept {
  center_.Move(vect);

  return *this;
}

bool geometry::Circle::ContainsPoint(const geometry::Point& point) const noexcept {
  int64_t equation_x = (center_.x_ - point.x_);
  int64_t equation_y = (center_.y_ - point.y_);
  int64_t equation_radius = radius_;

  equation_x *= equation_x;
  equation_y *= equation_y;
  equation_radius *= equation_radius;

  return equation_x + equation_y <= equation_radius;
}
bool geometry::Circle::CrossesSegment(const geometry::Segment& segment) const noexcept {
  long double distance = center_.GetDistance(segment);
  auto radius = static_cast<long double>(radius_);

  return distance <= radius &&
         (center_.GetDistance(segment.begin_) >= radius || center_.GetDistance(segment.end_) >= radius);
}

std::unique_ptr<geometry::IShape> geometry::Circle::Clone() const {
  return std::make_unique<Circle>(*this);
}

std::string geometry::Circle::ToString() const noexcept {
  return "Circle(" + center_.ToString() + ", " + std::to_string(radius_) + ")";
}