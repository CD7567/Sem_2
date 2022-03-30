#include "../point.h"
#include "../segment.h"
#include "../vector.h"

geometry::Point::Point(int64_t x, int64_t y) : x_(x), y_(y) {
}

geometry::Point& geometry::Point::Move(const geometry::Vector& vect) noexcept {
  x_ += vect.dim_x_;
  y_ += vect.dim_y_;

  return *this;
}

bool geometry::Point::ContainsPoint(const geometry::Point& point) const noexcept {
  return x_ == point.x_ && y_ == point.y_;
}
bool geometry::Point::CrossesSegment(const geometry::Segment& segment) const noexcept {
  return Vector(segment.begin_, *this).IsCodirectional(Vector(*this, segment.end_));
}

std::unique_ptr<geometry::IShape> geometry::Point::Clone() const {
  return std::make_unique<Point>(*this);
}

std::string geometry::Point::ToString() const noexcept {
  return "Point(" + std::to_string(x_) + ", " + std::to_string(y_) + ")";
}

geometry::Vector geometry::Point::operator-(const geometry::Point& rhs) const {
  return {rhs, *this};
}

long double geometry::Point::GetDistance(const geometry::Point& other) const {
  return Vector(*this, other).GetLength();
}

long double geometry::Point::GetDistance(const geometry::Segment& segment) const {
  Vector vect_ab(segment.begin_, segment.end_);
  Vector vect_ac(segment.begin_, *this);
  Vector vect_bc(segment.end_, *this);
  int64_t prod_ab_ac = vect_ab.ScalarMultiply(vect_ac);
  int64_t prod_ab_bc = vect_ab.ScalarMultiply(vect_bc);
  long double distance;

  if (prod_ab_ac > 0 && prod_ab_bc < 0) {
    distance = static_cast<long double>(std::abs(vect_ab.VectorMultiply(vect_ac))) / vect_ab.GetLength();
  } else if (prod_ab_ac <= 0) {
    distance = vect_ac.GetLength();
  } else if (prod_ab_bc >= 0) {
    distance = vect_bc.GetLength();
  }

  return distance;
}

bool geometry::Point::operator==(const Point& rhs) const {
  return (*this).x_ == rhs.x_ && (*this).y_ == rhs.y_;
}