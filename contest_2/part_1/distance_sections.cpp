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

bool CheckBoxIntersect(double beg_1, double end_1, double beg_2, double end_2) {
  if (beg_1 > end_1) {
    std::swap(beg_1, end_1);
  }
  if (beg_2 > end_2) {
    std::swap (beg_2, end_2);
  }

  return std::max(beg_1, beg_2) <= std::min(end_1, end_2);
}

bool DoSectionsIntersect(const Point& f_beg, const Point& f_end, const Point& s_beg, const Point& s_end) {
  Vector vect_ab(f_beg, f_end), vect_cd(s_beg, s_end);
  Vector vect_ac(f_beg, s_beg), vect_ad(f_beg, s_end), vect_bc(f_end, s_beg);
  bool do_intersect = false;

  do_intersect = CheckBoxIntersect(f_beg.x_, f_end.x_, s_beg.x_, s_end.x_)
                 && CheckBoxIntersect(f_beg.y_, f_end.y_, s_beg.y_, s_end.y_)
                 && Vector::VectorMultiply(vect_ab, vect_ac) * Vector::VectorMultiply(vect_ab, vect_ad) <= 0
                 && Vector::VectorMultiply(vect_cd, vect_ac) * Vector::VectorMultiply(vect_cd, vect_bc) <= 0;

  return do_intersect;
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

double GetDistanceSections(const Point& f_beg, const Point& f_end, const Point& s_beg, const Point& s_end) {
  double distance = 0;

  if (!DoSectionsIntersect(f_beg, f_end, s_beg, s_end)) {
    distance = std::min(GetDistanceToSection(f_beg, f_end, s_beg), GetDistanceToSection(f_beg, f_end, s_end));
    distance = std::min(distance, GetDistanceToSection(s_beg, s_end, f_beg));
    distance = std::min(distance, GetDistanceToSection(s_beg, s_end, f_end));
  }

  return distance;
}

int main() {
  Point p_a, p_b, p_c, p_d;

  std::cin >> p_a >> p_b >> p_c >> p_d;

  std::cout << std::setprecision(10) << std::fixed;
  std::cout << GetDistanceSections(p_a, p_b, p_c, p_d) << std::endl;
}