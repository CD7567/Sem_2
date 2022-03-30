#include <iostream>
#include <cmath>
#include <iomanip>
#include <vector>
#include <algorithm>

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

long double GetSurface(std::vector<Point> polygon) {
  int64_t surf = 0;
  Point begin = polygon.front();

  for (auto it = polygon.begin() + 1; it < polygon.end() - 1; ++it) {
    surf += Vector::VectorMultiply(Vector(begin, *it), Vector(begin, *(it + 1)));
  }

  return std::abs(static_cast<long double>(surf) / 2);
}

std::vector<Point> JarvisMarch(std::vector<Point> points) {
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
  return result;
}

int main() {
  size_t n;
  std::cin >> n;

  auto points = std::vector<Point>(n);

  for (size_t i = 0; i < n; ++i) {
    std::cin >> points[i];
  }

  std::vector<Point> result = JarvisMarch(points);

  std::cout << result.size() << std::endl;
  for (auto it = result.begin(); it < result.end(); ++it) {
    std::cout << *it << std::endl;
  }

  std::cout << std::setprecision(1) << std::fixed;
  std::cout << GetSurface(result);
}