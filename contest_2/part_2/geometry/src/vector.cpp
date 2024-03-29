#include "../vector.h"
#include "../point.h"

geometry::Vector::Vector(int64_t dim_x, int64_t dim_y) : dim_x_(dim_x), dim_y_(dim_y) {
}

geometry::Vector::Vector(const Point& beg, const Point& end) : dim_x_(end.x_ - beg.x_), dim_y_(end.y_ - beg.y_) {
}

geometry::Vector geometry::Vector::operator+() const {
  return *this;
}
geometry::Vector geometry::Vector::operator-() const {
  return {-dim_x_, -dim_y_};
}

geometry::Vector& geometry::Vector::operator+=(const Vector& other) {
  dim_x_ += other.dim_x_;
  dim_y_ += other.dim_y_;

  return *this;
}
geometry::Vector& geometry::Vector::operator-=(const Vector& other) {
  dim_x_ -= other.dim_x_;
  dim_y_ -= other.dim_y_;

  return *this;
}
geometry::Vector& geometry::Vector::operator*=(int64_t num) {
  dim_x_ *= num;
  dim_y_ *= num;

  return *this;
}
geometry::Vector& geometry::Vector::operator/=(int64_t num) {
  dim_x_ /= num;
  dim_y_ /= num;

  return *this;
}

geometry::Vector geometry::Vector::operator+(const Vector& other) const {
  return {dim_x_ + other.dim_x_, dim_y_ + other.dim_y_};
}
geometry::Vector geometry::Vector::operator-(const Vector& other) const {
  return {dim_x_ - other.dim_x_, dim_y_ - other.dim_y_};
}
geometry::Vector geometry::Vector::operator*(int64_t num) const {
  return {dim_x_ * num, dim_y_ * num};
}
geometry::Vector geometry::Vector::operator/(int64_t num) const {
  return {dim_x_ / num, dim_y_ / num};
}

geometry::Vector operator*(int64_t num, const geometry::Vector& vect) {
  return {vect.dim_x_ * num, vect.dim_y_ * num};
}

bool geometry::Vector::operator==(const Vector& other) const {
  return dim_x_ == other.dim_x_ && dim_y_ == other.dim_y_;
}
bool geometry::Vector::operator!=(const Vector& other) const {
  return !(*this == other);
}

bool geometry::Vector::IsParallel(const Vector& other) const {
  return dim_x_ * other.dim_y_ == other.dim_x_ * dim_y_;
}
bool geometry::Vector::IsCodirectional(const Vector& other) const {
  return IsParallel(other) && (dim_x_ * other.dim_x_ >= 0) && (dim_y_ * other.dim_y_ >= 0);
}

int64_t geometry::Vector::ScalarMultiply(const Vector& other) const {
  return dim_x_ * other.dim_x_ + dim_y_ * other.dim_y_;
}
int64_t geometry::Vector::VectorMultiply(const Vector& other) const {
  return dim_x_ * other.dim_y_ - other.dim_x_ * dim_y_;
}

std::string geometry::Vector::ToString() const noexcept {
  return "Vector(" + std::to_string(dim_x_) + ", " + std::to_string(dim_y_) + ")";
}

long double geometry::Vector::GetLength() const {
  return sqrtl(static_cast<long double>(dim_x_ * dim_x_ + dim_y_ * dim_y_));
}