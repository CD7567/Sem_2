#include <cstdint>
#include <iostream>
#include <cmath>
#include <iomanip>

template <typename T>
T Abs(T it) {
  return ((it > 0) ? it : -it);
}

struct Point {
  double x_, y_;
};

std::ostream& operator<<(std::ostream& out, const Point& point) {
  out << point.x_ << " " << point.y_;
  return out;
}

class Vector {
 public:
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

  static double ScalarMultiply(const Vector& lhs, const Vector& rhs) {
    return lhs.dim_x_ * rhs.dim_x_ + lhs.dim_y_ * rhs.dim_y_;
  }

  static double VectorMultiply(const Vector& lhs, const Vector& rhs) {
    return lhs.dim_x_ * rhs.dim_y_ - rhs.dim_x_ * lhs.dim_y_;
  }

 private:
  double dim_x_, dim_y_;
};

class Line {
 public:
  Line() = default;
  Line(int64_t a, int64_t b, int64_t c) : a_(a), b_(b), c_(c){};

  [[nodiscard]] Vector GetNormal() const {
    return Vector(static_cast<double>(b_), static_cast<double>(-a_));
  }

  [[nodiscard]] Point GetIntersection(const Line& other) const {
    double intersect_x =
            static_cast<double>(b_ * other.c_ - other.b_ * c_) / static_cast<double>(a_ * other.b_ - other.a_ * b_);
    double intersect_y =
            static_cast<double>(a_ * other.c_ - other.a_ * c_) / static_cast<double>(other.a_ * b_ - a_ * other.b_);

    return {intersect_x, intersect_y};
  }

  [[nodiscard]] double GetDistance(const Line& other) const {
    double dist;

    if (b_ == 0) {
      dist = Abs((c_ * other.a_ - other.c_ * a_) / (a_ * other.a_));
    } else {
      dist = Abs((c_ * other.b_ - other.c_ * b_) / (GetNormal().GetLength() * other.b_));
    }

    return dist;
  }

  friend std::istream& operator>>(std::istream& in, Line& line) {
    in >> line.a_ >> line.b_ >> line.c_;
    return in;
  }

 private:
  double a_, b_, c_;
};

int main() {
  Line first, second;

  std::cin >> first >> second;

  std::cout << std::setprecision(9) << std::fixed;
  std::cout << first.GetNormal() << std::endl;
  std::cout << second.GetNormal() << std::endl;

  if (first.GetNormal().IsParallel(second.GetNormal())) {
    std::cout << first.GetDistance(second) << std::endl;
  } else {
    std::cout << first.GetIntersection(second) << std::endl;
  }
}