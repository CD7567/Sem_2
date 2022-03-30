#include "../line.h"
#include "../point.h"
#include "../vector.h"
#include "../segment.h"

geometry::Line::Line(int64_t a, int64_t b, int64_t c) : a_(a), b_(b), c_(c) {
}

geometry::Line::Line(const geometry::Point& first, const geometry::Point& second)
    : a_(second.y_ - first.y_)
    , b_(first.x_ - second.x_)
    , c_(first.y_ * (second.x_ - first.x_) - first.x_ * (second.y_ - first.y_)) {
}

geometry::Line& geometry::Line::Move(const geometry::Vector& vect) noexcept {
  c_ -= vect.dim_x_ * a_ + vect.dim_y_ * b_;
  return *this;
}

bool geometry::Line::ContainsPoint(const geometry::Point& point) const noexcept {
  return a_ * point.x_ + b_ * point.y_ + c_ == 0;
}
bool geometry::Line::CrossesSegment(const geometry::Segment& segment) const noexcept {
  int64_t beg_value = (a_ * segment.begin_.x_ + b_ * segment.begin_.y_ + c_);
  int64_t end_value = (a_ * segment.end_.x_ + b_ * segment.end_.y_ + c_);
  return beg_value * end_value <= 0;
}

std::unique_ptr<geometry::IShape> geometry::Line::Clone() const {
  return std::make_unique<Line>(*this);
}

std::string geometry::Line::ToString() const noexcept {
  return "Line(" + std::to_string(a_) + ", " + std::to_string(b_) + ", " + std::to_string(c_) + ")";
}