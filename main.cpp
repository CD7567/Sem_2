#include <iostream>
#include <cmath>
#include <vector>

struct Point {
  int64_t x_, y_;
};

std::istream& operator>>(std::istream& in, Point& point) {
  in >> point.x_ >> point.y_;
  return in;
}
std::ostream& operator<<(std::ostream& out, Point& point) {
  out << point.x_ << " " << point.y_;
  return out;
}

bool operator==(const Point& lhs, const Point& rhs) {
  return lhs.x_ == rhs.x_ && lhs.y_ == rhs.y_;
}
bool operator!=(const Point& lhs, const Point& rhs) {
  return !(lhs == rhs);
}

int64_t SquaredDistance(const Point& lhs, const Point& rhs) {
  return (lhs.x_ - rhs.x_) * (lhs.x_ - rhs.x_) + (lhs.y_ - rhs.y_) * (lhs.y_ - rhs.y_);
}

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

class Polygon {
  friend Vector;
 public:
  Polygon() : vertex_({}){};
  Polygon(const Polygon& other) = default;
  explicit Polygon(std::vector<Point>& vector) : vertex_(std::move(vector)){};

  friend std::istream& operator>>(std::istream& in, Polygon& polygon) {
    size_t size;
    Point point;
    in >> size;
    for (size_t i = 0; i < size; ++i) {
      in >> point;
      polygon.vertex_.push_back(point);
    }
    return in;
  }
  friend std::ostream& operator<<(std::ostream& out, const Polygon& polygon) {
    out << polygon.vertex_.size() << std::endl;
    for (auto i : polygon.vertex_) {
      out << i << std::endl;
    }
    return out;
  }

  bool ContainsPoint(const Point& point) {
    Ray traced_ray{point, Vector(9000, 1)};
    Segment side;
    size_t intersect_counter = 0, vertex_len = vertex_.size();
    bool point_matches = false;

    for (size_t i = 0; i < vertex_len && !point_matches; ++i) {
      side.begin_ = vertex_[i];
      side.end_ = vertex_[(i + 1) % vertex_len];

      intersect_counter += static_cast<size_t>(CrossesSegment(traced_ray, side));

      point_matches = (point == vertex_[i] || point == vertex_[(i + 1) % vertex_len] || CrossesSegment(point, side));
    }

    return point_matches || static_cast<bool>(intersect_counter % 2);
  }
  bool HullContainsPoint(const Point& point) {
    Segment side;
    size_t vertex_len = vertex_.size();
    bool point_matches = false;

    for (size_t i = 0; i < vertex_len && !point_matches; ++i) {
      side.begin_ = vertex_[i];
      side.end_ = vertex_[(i + 1) % vertex_len];

      point_matches |= (point == vertex_[i] || point == vertex_[(i + 1) % vertex_len] || CrossesSegment(point, side));
    }

    return point_matches;
  }

  std::vector<Point> vertex_;
};

long double GetSurface(std::vector<Point> polygon) {
  int64_t surf = 0;
  Point begin = polygon.front();

  for (auto it = polygon.begin() + 1; it < polygon.end() - 1; ++it) {
    surf += Vector::VectorMultiply(Vector(begin, *it), Vector(begin, *(it + 1)));
  }

  return std::abs(static_cast<long double>(surf) / 2);
}

Polygon JarvisMarch(std::vector<Point> points) {
  std::vector<Point> result;
  size_t base_idx = 0, length = points.size();

  for (size_t i = 1; i < length; ++i) {
    if ((points[i].x_ < points[base_idx].x_) ||
        (points[i].x_ == points[base_idx].x_ && points[i].y_ < points[base_idx].y_)) {
      base_idx = i;
    }
  }

  Point base_point = points[base_idx];
  result.push_back(base_point);

  Point next_in_hull = points[(base_idx + 1) % points.size()];

  for (auto& point : points) {
    if (Vector::VectorMultiply({base_point, point}, {base_point, next_in_hull}) < 0) {
      next_in_hull = point;
    } else if (Vector::VectorMultiply({base_point, point}, {base_point, next_in_hull}) == 0) {
      if (SquaredDistance(base_point, point) > SquaredDistance(base_point, next_in_hull)) {
        next_in_hull = point;
      }
    }
  }
  result.push_back(next_in_hull);

  Point hull_last_point = result.back();

  do {
    next_in_hull = points[(base_idx + 1) % points.size()];

    for (auto& point : points) {
      if (Vector::VectorMultiply({hull_last_point, point}, {hull_last_point, next_in_hull}) < 0) {
        next_in_hull = point;
      } else if (Vector::VectorMultiply({hull_last_point, point}, {hull_last_point, next_in_hull}) == 0) {
        if (SquaredDistance(hull_last_point, point) > SquaredDistance(hull_last_point, next_in_hull)) {
          next_in_hull = point;
        }
      }
    }

    result.push_back(next_in_hull);
    hull_last_point = result.back();
  } while (result.back() != base_point);

  result.pop_back();
  return Polygon(result);
}

int main() {
  std::vector<Polygon> convex_hulls;
  Polygon labs;
  Polygon civil;

  std::cin >> civil;
  std::cin >> labs;

  Polygon hull_temporary;
  Polygon remains(civil);
  Polygon remains_reserve;

  do {
    hull_temporary = JarvisMarch(remains.vertex_);

    if (GetSurface(hull_temporary.vertex_) > 0.0) {
      convex_hulls.push_back(hull_temporary);

      for (auto& element : remains.vertex_) {
        if (!(hull_temporary.HullContainsPoint(element))) {
          remains_reserve.vertex_.push_back(element);
        }
      }

      remains.vertex_ = remains_reserve.vertex_;
      remains_reserve.vertex_.clear();
    } else {
      break;
    }
  } while (remains.vertex_.size() > 2);

  std::vector<size_t> answer(labs.vertex_.size(), 0);

  for (size_t i = 0; i < labs.vertex_.size(); ++i) {
    for (size_t j = 0; j < convex_hulls.size(); ++j) {
      if (convex_hulls[j].ContainsPoint(labs.vertex_[i])) {
        answer[i] = std::max(answer[i], j);
      }
    }
  }

  for (auto& element : answer) {
    std::cout << element << std::endl;
  }
}