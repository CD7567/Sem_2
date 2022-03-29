#include <iostream>
#include <cstdio>
#include <vector>
#include <cmath>

struct Point {
  int64_t x_, y_;
};

std::istream& operator>>(std::istream& in, Point& point) {
  in >> point.x_ >> point.y_;
  return in;
}

bool operator==(const Point& lhs, const Point& rhs) {
  return lhs.x_ == rhs.x_ && lhs.y_ == rhs.y_;
}

struct Segment {
  Point begin_, end_;
};

class Line {
 public:
  Line(const Point& first, const Point& second)
          : a_(second.y_ - first.y_)
          , b_(first.x_ - second.x_)
          , c_(first.y_ * (second.x_ - first.x_) - first.x_ * (second.y_ - first.y_)) {
  }

  int64_t a_, b_, c_;
};

class Vector {
 public:
  Vector(const Point& beg, const Point& end) : dim_x_(end.x_ - beg.x_), dim_y_(end.y_ - beg.y_){};
  explicit Vector(int64_t dim_x, int64_t dim_y) : dim_x_(dim_x), dim_y_(dim_y){};

  [[nodiscard]] int64_t GetSquaredLength() const {
    return dim_x_ * dim_x_ + dim_y_ * dim_y_;
  }

  Vector operator+(const Vector& other) const {
    return Vector(dim_x_ + other.dim_x_, dim_y_ + other.dim_y_);
  }

  friend std::ostream& operator<<(std::ostream& out, const Vector& vect) {
    out << vect.dim_x_ << " " << vect.dim_y_;
    return out;
  }

  [[nodiscard]] bool IsParallel(const Vector& other) const {
    return dim_x_ * other.dim_y_ == other.dim_x_ * dim_y_;
  }
  [[nodiscard]] bool IsCodirectional(const Vector& other) const {
    return IsParallel(other) && (dim_x_ * other.dim_x_ >= 0) && (dim_y_ * other.dim_y_ >= 0);
  }
  [[nodiscard]] bool IsOppositelyDirected(const Vector& other) const {
    return IsParallel(other) && (dim_x_ * other.dim_x_ <= 0) && (dim_y_ * other.dim_y_ <= 0);
  }

  [[nodiscard]] bool IsZero() const {
    return dim_x_ == 0 && dim_y_ == 0;
  }

  static int64_t ScalarMultiply(const Vector& lhs, const Vector& rhs) {
    return lhs.dim_x_ * rhs.dim_x_ + lhs.dim_y_ * rhs.dim_y_;
  }

  static int64_t VectorMultiply(const Vector& lhs, const Vector& rhs) {
    return lhs.dim_x_ * rhs.dim_y_ - rhs.dim_x_ * lhs.dim_y_;
  }

  int64_t dim_x_, dim_y_;
};

struct Ray {
  Point begin_;
  Vector direction_;
};

bool CrossesSegment(const Point& point, const Segment& segment) {
  return Vector(segment.begin_, point).IsCodirectional(Vector(point, segment.end_));
}

bool CrossesSegment(const Line& line, const Segment& segment) {
  int64_t beg_value = (line.a_ * segment.begin_.x_ + line.b_ * segment.begin_.y_ + line.c_);
  int64_t end_value = (line.a_ * segment.end_.x_ + line.b_ * segment.end_.y_ + line.c_);
  return beg_value * end_value <= 0;
}

bool CrossesSegment(const Ray& ray, const Segment& segment) {
  Point other_ray_point = ray.begin_;
  other_ray_point.x_ += ray.direction_.dim_x_;
  other_ray_point.y_ += ray.direction_.dim_y_;

  Line ray_line{ray.begin_, other_ray_point};
  bool result = false;

  if (CrossesSegment(ray_line, segment)) {
    Vector vect_segment(segment.begin_, segment.end_);
    Vector vect_from_segment(segment.begin_, ray.begin_);
    int64_t prod_1 = Vector::VectorMultiply(vect_from_segment, vect_segment);
    int64_t prod_2 = Vector::VectorMultiply(ray.direction_, vect_segment);
    result = prod_1 * prod_2 <= 0;
  }

  return result;
}

bool ContainsPoint(std::vector<Point> vertex, const Point& point) {
  Ray traced_ray{point, Vector(100001, 1)};
  Segment side;
  size_t intersect_counter = 0, vertex_len = vertex.size();
  bool point_matches = false;

  for (size_t i = 0; i < vertex_len && !point_matches; ++i) {
    side.begin_ = vertex[i];
    side.end_ = vertex[(i + 1) % vertex_len];

    intersect_counter += static_cast<size_t>(CrossesSegment(traced_ray, side));

    point_matches = (point == vertex[i] || point == vertex[(i + 1) % vertex_len] || CrossesSegment(point, side));
  }

  return point_matches || static_cast<bool>(intersect_counter % 2);
}

int main() {
  size_t n;
  Point point;
  Point polygon_point;
  std::vector<Point> polygon;

  std::cin >> n >> point;

  for (size_t i = 0; i < n; ++i) {
    std::cin >> polygon_point;
    polygon.push_back(polygon_point);
  }

  std::cout << (ContainsPoint(polygon, point) ? "YES" : "NO");
}