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

class Vector {
 public:
  Vector(const Point& beg, const Point& end)
          : dim_x_(end.x_ - beg.x_), dim_y_(end.y_ - beg.y_){};
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

Point k_base_point;
bool Compare(const Point& lhs, const Point& rhs) {
  bool result = false;
  Vector vect_lhs = Vector(k_base_point, lhs);
  Vector vect_rhs = Vector(k_base_point, rhs);
  int64_t prod = Vector::VectorMultiply(vect_lhs, vect_rhs);

  if (prod < 0) {
    result = true;
  } else if (prod == 0) {
    result = vect_lhs.GetSquaredLength() < vect_rhs.GetSquaredLength();
  }

  return result;
}

long double GetSurface(const Point* polygon_beg, const std::vector<size_t>& indices) {
  int64_t surf = 0;
  Point begin = polygon_beg[indices.front()];

  for (auto i = indices.begin() + 1; i < indices.end() - 1; ++i) {
    surf += Vector::VectorMultiply(Vector(begin, polygon_beg[*i]), Vector(begin, polygon_beg[*(i + 1)]));
  }

  return std::abs(static_cast<long double>(surf) / 2);
}

std::vector<size_t> GrahamScan(Point* array_beg, size_t length) {
  size_t idx = 0;
  int64_t prod;
  size_t next_point_idx;
  std::vector<size_t> result;

  for (size_t i = 1; i < length; ++i) {
    if ((array_beg[i].x_ < array_beg[idx].x_) ||
        (array_beg[i].x_ == array_beg[idx].x_ && array_beg[i].y_ < array_beg[idx].y_)) {
      idx = i;
    }
  }

  std::swap(array_beg[0], array_beg[idx]);
  k_base_point = array_beg[0];
  result.push_back(0);
  std::sort(array_beg + 1, array_beg + length, Compare);

  for (size_t i = 1; i < length - 1; ++i) {
    if (array_beg[i] != array_beg[i + 1]) {
      prod = Vector::VectorMultiply(Vector(array_beg[result.back()], array_beg[i]),
                                    Vector(array_beg[i], array_beg[(i + 1) % length]));
      if (prod < 0) {
        result.push_back(i);
      } else if (prod > 0) {
        next_point_idx = i + 1;

        do {
          i = result.back();
          result.pop_back();
          prod = Vector::VectorMultiply(Vector(array_beg[result.back()], array_beg[i]),
                                        Vector(array_beg[i], array_beg[next_point_idx % length]));
        } while (prod > 0);

        if (prod != 0) {
          result.push_back(i);
        }

        i = next_point_idx - 1;
      }
    }
  }

  if (Compare(array_beg[result.back()], array_beg[length - 1])) {
    result.push_back(length - 1);
  } else {
    result.pop_back();
    result.push_back(length - 1);
  }

  return result;
}

int main() {
  size_t n;
  std::cin >> n;

  auto polygon = new Point[n];

  for (size_t i = 0; i < n; ++i) {
    std::cin >> polygon[i];
  }

  std::vector<size_t> result = GrahamScan(polygon, n);

  std::cout << result.size() << std::endl;
  for (auto it = result.begin(); it < result.end(); ++it) {
    std::cout << polygon[*it] << std::endl;
  }

  std::cout << std::setprecision(1) << std::fixed;
  std::cout << GetSurface(polygon, result);

  delete[] polygon;
}