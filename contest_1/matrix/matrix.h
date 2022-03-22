#pragma once

#include <iostream>
#include <cstddef>
#include <stdexcept>

class MatrixIsDegenerateError : public std::runtime_error {
 public:
  MatrixIsDegenerateError() : std::runtime_error("MatrixIsDegenerateError") {
  }
};

class MatrixOutOfRange : public std::out_of_range {
 public:
  MatrixOutOfRange() : std::out_of_range("MatrixOutOfRange") {
  }
};

template <typename T, size_t X, size_t Y>
class Matrix {
 public:
  [[nodiscard]] size_t RowsNumber() const {
    return X;
  }
  [[nodiscard]] size_t ColumnsNumber() const {
    return Y;
  }

  T& operator()(size_t row, size_t col) {
    return matrix[row][col];
  }
  T operator()(size_t row, size_t col) const {
    return matrix[row][col];
  }
  T& At(size_t row, size_t col) {
    if ((row < 0) || (col < 0) || (row >= X) || (col >= Y)) {
      throw MatrixOutOfRange();
    } else {
      return matrix[row][col];
    }
  }
  T At(size_t row, size_t col) const {
    if ((row < 0) || (col < 0) || (row >= X) || (col >= Y)) {
      throw MatrixOutOfRange();
    } else {
      return matrix[row][col];
    }
  }

  Matrix<T, X, Y> operator+(const Matrix<T, X, Y>& other) const {
    Matrix<T, X, Y> result = *this;

    result += other;
    return result;
  }
  Matrix<T, X, Y> operator-(const Matrix<T, X, Y>& other) const {
    Matrix<T, X, Y> result = *this;
    result -= other;

    return result;
  }
  template <size_t Z>
  Matrix<T, X, Z> operator*(const Matrix<T, Y, Z>& other) const {
    Matrix<T, X, Z> result{};

    for (size_t i = 0; i < X; ++i) {
      for (size_t j = 0; j < Z; ++j) {
        for (size_t k = 0; k < Y; ++k) {
          result.matrix[i][j] += matrix[i][k] * other.matrix[k][j];
        }
      }
    }

    return result;
  }
  Matrix<T, X, Y>& operator+=(const Matrix<T, X, Y>& other) {
    for (size_t i = 0; i < X; ++i) {
      for (size_t j = 0; j < Y; ++j) {
        matrix[i][j] += other.matrix[i][j];
      }
    }

    return *this;
  }
  Matrix<T, X, Y>& operator-=(const Matrix<T, X, Y>& other) {
    for (size_t i = 0; i < X; ++i) {
      for (size_t j = 0; j < Y; ++j) {
        matrix[i][j] -= other.matrix[i][j];
      }
    }

    return *this;
  }
  Matrix<T, X, Y>& operator*=(const Matrix<T, Y, Y>& other) {
    auto row = new T[Y]{};

    for (size_t i = 0; i < X; ++i) {
      for (size_t j = 0; j < Y; ++j) {
        for (size_t k = 0; k < Y; ++k) {
          row[j] += matrix[i][k] * other.matrix[k][j];
        }
      }

      for (size_t j = 0; j < Y; ++j) {
        matrix[i][j] = row[j];
        row[j] = 0;
      }
    }

    delete[] row;
    return *this;
  }

  Matrix<T, X, Y> operator*(const T& num) const {
    Matrix<T, X, Y> result = *this;
    result *= num;

    return result;
  }
  Matrix<T, X, Y> operator/(const T& num) const {
    Matrix<T, X, Y> result = *this;
    result /= num;

    return result;
  }
  Matrix<T, X, Y>& operator*=(const T& num) {
    for (size_t i = 0; i < X; ++i) {
      for (size_t j = 0; j < Y; ++j) {
        matrix[i][j] *= num;
      }
    }

    return *this;
  }
  Matrix<T, X, Y>& operator/=(const T& num) {
    for (size_t i = 0; i < X; ++i) {
      for (size_t j = 0; j < Y; ++j) {
        matrix[i][j] /= num;
      }
    }

    return *this;
  }
  friend Matrix<T, X, Y> operator*(const T& num, const Matrix<T, X, Y>& operand) {
    Matrix<T, X, Y> result = operand;
    result *= num;

    return result;
  }
  friend Matrix<T, X, Y>& operator*=(const T& num, Matrix<T, X, Y>& operand) {
    operand *= num;

    return operand;
  }

  friend bool operator==(const Matrix<T, X, Y>& lhs, const Matrix<T, X, Y>& rhs) {
    bool equals = true;

    for (size_t i = 0; (i < X) && equals; ++i) {
      for (size_t j = 0; (j < Y) && equals; ++j) {
        equals &= (lhs.matrix[i][j] == rhs.matrix[i][j]);
      }
    }

    return equals;
  }
  friend bool operator!=(const Matrix<T, X, Y>& lhs, const Matrix<T, X, Y>& rhs) {
    return !(lhs == rhs);
  }

  friend std::istream& operator>>(std::istream& in, Matrix<T, X, Y>& operand) {
    for (size_t i = 0; i < X; ++i) {
      for (size_t j = 0; j < Y; ++j) {
        in >> operand.matrix[i][j];
      }
    }

    return in;
  }
  friend std::ostream& operator<<(std::ostream& out, const Matrix<T, X, Y>& operand) {
    for (size_t i = 0; i < X; ++i) {
      for (size_t j = 0; j < Y - 1; ++j) {
        out << operand.matrix[i][j] << " ";
      }

      out << operand.matrix[i][Y - 1] << '\n';
    }

    return out;
  }

  T matrix[X][Y];
};

template <typename T, size_t X, size_t Y>
Matrix<T, Y, X> GetTransposed(const Matrix<T, X, Y>& matrix) {
  Matrix<T, Y, X> transposed;

  for (size_t i = 0; i < X; ++i) {
    for (size_t j = 0; j < Y; ++j) {
      transposed.matrix[j][i] = matrix.matrix[i][j];
    }
  }

  return transposed;
}