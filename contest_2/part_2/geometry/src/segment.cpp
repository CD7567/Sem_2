#include "../segment.h"
#include "../vector.h"

bool CheckBoxIntersect(int64_t beg_1, int64_t end_1, int64_t beg_2, int64_t end_2) {
  if (beg_1 > end_1) {
    std::swap(beg_1, end_1);
  }
  if (beg_2 > end_2) {
    std::swap(beg_2, end_2);
  }

  return std::max(beg_1, beg_2) <= std::min(end_1, end_2);
}

geometry::Segment::Segment(const Point& begin, const Point& end) : begin_(begin), end_(end) {
}

geometry::Segment& geometry::Segment::Move(const geometry::Vector& vect) noexcept {
  begin_.Move(vect);
  end_.Move(vect);

  return *this;
}

bool geometry::Segment::ContainsPoint(const geometry::Point& point) const noexcept {
  return Vector(point.x_ - begin_.x_, point.y_ - begin_.y_)
      .IsCodirectional(Vector(end_.x_ - point.x_, end_.y_ - point.y_));
}
bool geometry::Segment::CrossesSegment(const geometry::Segment& other) const noexcept {
  Vector vect_ab(begin_, end_), vect_cd(other.begin_, other.end_);
  Vector vect_ac(begin_, other.begin_), vect_ad(begin_, other.end_), vect_bc(end_, other.begin_);

  return CheckBoxIntersect(begin_.x_, end_.x_, other.begin_.x_, other.end_.x_) &&
         CheckBoxIntersect(begin_.y_, end_.y_, other.begin_.y_, other.end_.y_) &&
         vect_ab.VectorMultiply(vect_ac) * vect_ab.VectorMultiply(vect_ad) <= 0 &&
         vect_cd.VectorMultiply(vect_ac) * vect_cd.VectorMultiply(vect_bc) <= 0;
}

std::unique_ptr<geometry::IShape> geometry::Segment::Clone() const {
  return std::make_unique<Segment>(*this);
}

std::string geometry::Segment::ToString() const noexcept {
  return "Segment(" + begin_.ToString() + ", " + end_.ToString() + ")";
}