#include <iostream>
#include <cmath>

struct Point {
  double x_, y_;
};

std::istream& operator>>(std::istream& in, Point& point) {
  in >> point.x_ >> point.y_;
  return in;
}

class Vector {
 public:
  Vector(Point beg, Point end) : dim_x_(static_cast<double>(end.x_ - beg.x_)), dim_y_(static_cast<double>(end.y_ - beg.y_)){};
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

  static double ScalarMultiply(const Vector& lhs, const Vector& rhs) {
    return lhs.dim_x_ * rhs.dim_x_ + lhs.dim_y_ * rhs.dim_y_;
  }

  static double VectorMultiply(const Vector& lhs, const Vector& rhs) {
    return lhs.dim_x_ * rhs.dim_y_ - rhs.dim_x_ * lhs.dim_y_;
  }

 private:
  double dim_x_, dim_y_;
};

int main() {
  Point p_a, p_b, p_c;

  std::cin >> p_c >> p_a >> p_b;

  Vector vect_ab(p_a, p_b), vect_ac(p_a, p_c), vect_bc(p_b, p_c);

  if (vect_ac.IsParallel(vect_bc)) {
    std::cout << "YES" << std::endl;
  } else {
    std::cout << "NO" << std::endl;
  }

  if (vect_ac.IsCodirectional(vect_ab)) {
    std::cout << "YES" << std::endl;
  } else {
    std::cout << "NO" << std::endl;
  }

  if (vect_ac.IsOppositelyDirected(vect_bc)) {
    std::cout << "YES" << std::endl;
  } else {
    std::cout << "NO" << std::endl;
  }
}