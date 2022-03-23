#include <cstdint>
#include <iostream>
#include <cmath>
#include <iomanip>

class Vector {
 public:
  explicit Vector(int32_t beg_x, int32_t beg_y, int32_t end_x, int32_t end_y) : beg_x_(beg_x), beg_y_(beg_y), end_x_(end_x), end_y_(end_y),
                                                                                dim_x_(static_cast<double>(end_x - beg_x)), dim_y_(static_cast<double>(end_y - beg_y)) {
  };

  [[nodiscard]] double GetLength() const{
    return sqrt(dim_x_ * dim_x_ + dim_y_ * dim_y_);
  }

  Vector operator+(const Vector& other) const {
    return Vector(0, 0, static_cast<int32_t>(dim_x_ + other.dim_x_), static_cast<int32_t>(dim_y_ + other.dim_y_));
  }

  friend std::ostream& operator<<(std::ostream& out, const Vector& vect) {
    out << vect.dim_x_ << " " << vect.dim_y_;
    return out;
  }

  static double ScalarMultiply(const Vector& lhs, const Vector& rhs) {
    return lhs.dim_x_ * rhs.dim_x_ + lhs.dim_y_ * rhs.dim_y_;
  }

  static double VectorMultiply(const Vector& lhs, const Vector& rhs) {
    return lhs.dim_x_ * rhs.dim_y_ - rhs.dim_x_ * lhs.dim_y_;
  }

 private:
  double beg_x_, beg_y_, end_x_, end_y_;
  double dim_x_, dim_y_;
};


int main() {
  int32_t x_beg_1, y_beg_1, x_end_1, y_end_1;
  int32_t x_beg_2, y_beg_2, x_end_2, y_end_2;

  std::cin >> x_beg_1 >> y_beg_1 >> x_end_1 >> y_end_1;
  std::cin >> x_beg_2 >> y_beg_2 >> x_end_2 >> y_end_2;

  Vector first(x_beg_1, y_beg_1, x_end_1, y_end_1);
  Vector second(x_beg_2, y_beg_2, x_end_2, y_end_2);

  std::cout << std::setprecision(6) << std::fixed;
  std::cout << first.GetLength() << " " << second.GetLength() << std::endl;
  std::cout << first + second << std::endl;
  std::cout << Vector::ScalarMultiply(first, second) << " " << Vector::VectorMultiply(first, second) << std::endl;
  std::cout << std::abs(Vector::VectorMultiply(first, second) / 2) << std::endl;
}