#include <iostream>
#include <cmath>
#include <iomanip>

int32_t Sign(double d) {
  int sign;

  if (d == 0) {
    sign = 0;
  } else {
    sign = ((d > 0) ? 1 : -1);
  }

  return sign;
}

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

bool IsConvex(const Point* polygon_beg, const Point* polygon_end) {
  bool is_convex = true;
  int32_t sign = 0, curr_sign;
  size_t length = polygon_end - polygon_beg, mid_idx, right_idx;

  for (size_t idx = 0; idx < length && is_convex; ++idx) {
    mid_idx = (idx + 1) % length;
    right_idx = (idx + 2) % length;
    curr_sign = Sign(Vector::VectorMultiply(Vector(polygon_beg[idx], polygon_beg[mid_idx]), Vector(polygon_beg[mid_idx], polygon_beg[right_idx])));

    if (sign == 0) {
      sign = curr_sign;
    } else if (curr_sign != 0) {
      is_convex = sign == curr_sign;
    }
  }

  return is_convex;
}

int main() {
  size_t N;

  std::cin >> N;

  auto polygon = new Point[N];

  for (size_t i = 0; i < N; ++i) {
    std::cin >> polygon[i];
  }

  std::cout << std::setprecision(10) << std::fixed;
  std::cout << (IsConvex(polygon, polygon + N) ? "YES" : "NO") << std::endl;

  delete[] polygon;
}