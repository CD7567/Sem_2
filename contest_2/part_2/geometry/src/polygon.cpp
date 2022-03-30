#include "../polygon.h"

#include "../point.h"
#include "../segment.h"
#include "../ray.h"

geometry::Polygon::Polygon(const std::vector<Point>& vertex) : vertex_(vertex) {
}

geometry::Polygon& geometry::Polygon::Move(const geometry::Vector& vect) noexcept {
  for (auto& point : vertex_) {
    point.Move(vect);
  }

  return *this;
}

bool geometry::Polygon::ContainsPoint(const geometry::Point& point) const noexcept {
  Ray traced_ray{point, Vector(100001, 1)};
  Segment side(vertex_[0], vertex_[0]);
  size_t intersect_counter = 0, vertex_len = vertex_.size();
  bool point_matches = false;

  for (size_t i = 0; i < vertex_len && !point_matches; ++i) {
    side.begin_ = vertex_[i];
    side.end_ = vertex_[(i + 1) % vertex_len];

    intersect_counter += static_cast<size_t>(traced_ray.CrossesSegment(side));

    point_matches = (point == vertex_[i] || point == vertex_[(i + 1) % vertex_len] || point.CrossesSegment(side));
  }

  return point_matches || static_cast<bool>(intersect_counter % 2);
}
bool geometry::Polygon::CrossesSegment(const geometry::Segment& segment) const noexcept {
  Segment side(vertex_[0], vertex_[0]);
  size_t vertex_len = vertex_.size();
  bool result = false;

  for (size_t i = 0; i < vertex_len && !result; ++i) {
    side.begin_ = vertex_[i];
    side.end_ = vertex_[(i + 1) % vertex_.size()];

    result |= side.CrossesSegment(segment);
  }

  return result;
}

std::unique_ptr<geometry::IShape> geometry::Polygon::Clone() const {
  return std::make_unique<Polygon>(*this);
}

std::string geometry::Polygon::ToString() const noexcept {
  std::string result = "Polygon(";

  for (auto it = vertex_.begin(); it < vertex_.end() - 1; ++it) {
    result += (*it).ToString() + ", ";
  }

  result += (vertex_.end() - 1)->ToString() + ")";

  return result;
}