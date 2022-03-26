#include <iostream>
#include <cmath>
#include <iomanip>

struct Point {
  double x_, y_;
};

std::istream& operator>>(std::istream& in, Point& point) {
  in >> point.x_ >> point.y_;
  return in;
}

bool operator==(const Point& lhs, const Point& rhs) {
  return lhs.x_ == rhs.x_ && lhs.y_ == rhs.y_;
}

class Vector {
 public:
  Vector(const Point& beg, const Point& end) : dim_x_(static_cast<double>(end.x_ - beg.x_)), dim_y_(static_cast<double>(end.y_ - beg.y_)){};
  explicit Vector(double dim_x, double dim_y) : dim_x_(dim_x), dim_y_(dim_y){};

  [[nodiscard]] double GetLength() const {
    return sqrt(dim_x_ * dim_x_ + dim_y_ * dim_y_);
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

  static double ScalarMultiply(const Vector& lhs, const Vector& rhs) {
    return lhs.dim_x_ * rhs.dim_x_ + lhs.dim_y_ * rhs.dim_y_;
  }

  static double VectorMultiply(const Vector& lhs, const Vector& rhs) {
    return lhs.dim_x_ * rhs.dim_y_ - rhs.dim_x_ * lhs.dim_y_;
  }

 private:
  double dim_x_, dim_y_;
};

double GetSurface(const Point* polygon_beg, const Point* polygon_end) {
  double surf = 0;
  Point begin = *polygon_beg;

  for (const Point* it = polygon_beg + 1; it < polygon_end - 1; ++it) {
    surf += Vector::VectorMultiply(Vector(begin, *it), Vector(begin, *(it + 1)));
  }

  return std::abs(surf / 2);
}

int main() {
  size_t n;

  std::cin >> n;

  auto polygon = new Point[n];

  for (size_t i = 0; i < n; ++i) {
    std::cin >> polygon[i];
  }

  std::cout << std::setprecision(15) << std::fixed;
  std::cout << GetSurface(polygon, polygon + n) << std::endl;

  delete[] polygon;
}