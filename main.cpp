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

double GetDistanceToLine(Point from, Point to, Point point) {
  Vector vect_ab(from, to), vect_ac(from, point);

  return std::abs(Vector::VectorMultiply(vect_ab, vect_ac)) / vect_ab.GetLength();
}

double GetDistanceToRay(Point from, Point to, Point point) {
  Vector vect_ab(from, to), vect_ac(from, point), vect_bc(to, point);
  double prod_ab_ac = Vector::ScalarMultiply(vect_ab, vect_ac);
  double distance;

  if (prod_ab_ac > 0) {
    distance = std::abs(Vector::VectorMultiply(vect_ab, vect_ac)) / vect_ab.GetLength();
  } else {
    distance = vect_ac.GetLength();
  }

  return distance;
}

double GetDistanceToSection(Point from, Point to, Point point) {
  Vector vect_ab(from, to), vect_ac(from, point), vect_bc(to, point);
  double prod_ab_ac = Vector::ScalarMultiply(vect_ab, vect_ac);
  double prod_ab_bc = Vector::ScalarMultiply(vect_ab, vect_bc);
  double distance;

  if (prod_ab_ac > 0 && prod_ab_bc < 0) {
    distance = std::abs(Vector::VectorMultiply(vect_ab, vect_ac)) / vect_ab.GetLength();
  } else if (prod_ab_ac <= 0) {
    distance = vect_ac.GetLength();
  } else if (prod_ab_bc >= 0) {
    distance = vect_bc.GetLength();
  }

  return distance;
}

int main() {
  Point p_a, p_b, p_c;

  std::cin >> p_c >> p_a >> p_b;

  Vector vect_ab(p_a, p_b), vect_ac(p_a, p_c), vect_bc(p_b, p_c);

  std::cout << std::setprecision(9) << std::fixed;
  std::cout << GetDistanceToLine(p_a, p_b, p_c) << std::endl;
  std::cout << GetDistanceToRay(p_a, p_b, p_c) << std::endl;
  std::cout << GetDistanceToSection(p_a, p_b, p_c) << std::endl;
}