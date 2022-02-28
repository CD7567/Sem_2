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

template <typename T, size_t x, size_t y>
class Matrix {
public:

    [[nodiscard]] size_t RowsNumber() const {
        return x;
    }
    [[nodiscard]] size_t ColumnsNumber() const {
        return y;
    }

    T& operator()(size_t row, size_t col) {
        return matrix[row][col];
    }
    T operator()(size_t row, size_t col) const {
        return matrix[row][col];
    }
    T& At(size_t, size_t);
    T At(size_t, size_t) const;

    friend Matrix<T, x, y> operator+(const Matrix<T, x, y>&, const Matrix<T, x, y>&);
    friend Matrix<T, x, y> operator-(const Matrix<T, x, y>&, const Matrix<T, x, y>&);
    template<size_t z>
    friend Matrix<T, x, y> operator*(const Matrix<T, x, y>&, const Matrix<T, y, z>&);
    friend Matrix<T, x, y>& operator+=(Matrix<T, x, y>&, const Matrix<T, x, y>&);
    friend Matrix<T, x, y>& operator-=(Matrix<T, x, y>&, const Matrix<T, x, y>&);
    friend Matrix<T, x, y>& operator*=(Matrix<T, x, y>&, const Matrix<T, y, y>&);

    friend Matrix<T, x, y> operator*(const Matrix<T, x, y>&, const T&);
    friend Matrix<T, x, y> operator/(const Matrix<T, x, y>&, const T&);
    friend Matrix<T, x, y>& operator*=(Matrix<T, x, y>&, const T&);
    friend Matrix<T, x, y>& operator/=(Matrix<T, x, y>&, const T&);
    friend Matrix<T, x, y> operator*(const T&, const Matrix<T, x, y>&);
    friend Matrix<T, x, y> operator/(const T&, const Matrix<T, x, y>&);
    friend Matrix<T, x, y>& operator*=(const T&, Matrix<T, x, y>&);
    friend Matrix<T, x, y>& operator/=(const T&, Matrix<T, x, y>&);

    friend bool operator==(const Matrix<T, x, y>&, const Matrix<T, x, y>&);
    friend bool operator!=(const Matrix<T, x, y>&, const Matrix<T, x, y>&);

    friend std::istream& operator>>(std::istream&, Matrix<T, x, y>&);
    friend std::ostream& operator<<(std::ostream&, const Matrix<T, x, y>&);

    T matrix[x][y];
};

template <typename T, size_t x, size_t y>
Matrix<T, y, x> GetTransposed(const Matrix<T, x, y>&);