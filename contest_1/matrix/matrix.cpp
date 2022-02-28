#include "matrix.h"

template<typename T, size_t x, size_t y>
T& Matrix<T, x, y>::At(size_t row, size_t col) {
   if ((row < 0) || (col < 0) || (row >= x) || (col >= y)) {
       throw MatrixOutOfRange();
   } else {
       return matrix[row][col];
   }
}

template<typename T, size_t x, size_t y>
T Matrix<T, x, y>::At(size_t row, size_t col) const {
    if ((row < 0) || (col < 0) || (row >= x) || (col >= y)) {
        throw MatrixOutOfRange();
    } else {
        return matrix[row][col];
    }
}

template<typename T, size_t x, size_t y>
Matrix<T, x, y> operator+(const Matrix<T, x, y>& lhs, const Matrix<T, x, y>& rhs) {
    Matrix<T, x, y> result;

    for (size_t i = 0; i < x; ++i) {
        for (size_t j = 0; j < y; ++j) {
            result.matrix[i][j] = lhs.matrix[i][j] + rhs.matrix[i][j];
        }
    }

    return result;
}

template<typename T, size_t x, size_t y>
Matrix<T, x, y> operator-(const Matrix<T, x, y>& lhs, const Matrix<T, x, y>& rhs) {
    Matrix<T, x, y> result;

    for (size_t i = 0; i < x; ++i) {
        for (size_t j = 0; j < y; ++j) {
            result.matrix[i][j] = lhs.matrix[i][j] - rhs.matrix[i][j];
        }
    }

    return result;
}

template<typename T, size_t x, size_t y, size_t z>
Matrix<T, x, y> operator*(const Matrix<T, x, y>& lhs, const Matrix<T, y, z>& rhs) {
    Matrix<T, x, z> result{};

    for (size_t i = 0; i < x; ++i) {
        for (size_t j = 0; j < z; ++j) {
            for (size_t k = 0; k < y; ++k) {
                result.matrix[i][j] += lhs.matrix[i][k] * rhs.matrix[k][j];
            }
        }
    }

    return result;
}

template<typename T, size_t x, size_t y>
Matrix<T, x, y>& operator+=(Matrix<T, x, y>& lhs, const Matrix<T, x, y>& rhs) {
    for (size_t i = 0; i < x; ++i) {
        for (size_t j = 0; j < y; ++j) {
            lhs.matrix[i][j] += rhs.matrix[i][j];
        }
    }

    return lhs;
}

template<typename T, size_t x, size_t y>
Matrix<T, x, y>& operator-=(Matrix<T, x, y>& lhs, const Matrix<T, x, y>& rhs) {
    for (size_t i = 0; i < x; ++i) {
        for (size_t j = 0; j < y; ++j) {
            lhs.matrix[i][j] -= rhs.matrix[i][j];
        }
    }

    return lhs;
}

template<typename T, size_t x, size_t y>
Matrix<T, x, y>& operator*=(Matrix<T, x, y>& lhs, const Matrix<T, y, y>& rhs) {
    T result[x][y]{};

    for (size_t i = 0; i < x; ++i) {
        for (size_t j = 0; j < y; ++j) {
            for (size_t k = 0; k < y; ++k) {
                result[i][j] += lhs.matrix[i][k] * rhs.matrix[k][j];
            }
        }
    }

    for (size_t i = 0; i < x; ++i) {
        for (size_t j = 0; j < y; ++j) {
            lhs.matrix[i][j] = result[i][j];
        }
    }

    return lhs;
}

template <typename T, size_t x, size_t y>
Matrix<T, x, y> operator*(const Matrix<T, x, y>& matrix, const T& num) {
    Matrix<T, x, y> result;

    for (size_t i = 0; i < x; ++i) {
        for (size_t j = 0; j < y; ++j) {
            result.matrix[i][j] = matrix.matrix[i][j] * num;
        }
    }

    return result;
}

template <typename T, size_t x, size_t y>
Matrix<T, x, y> operator/(const Matrix<T, x, y>& matrix, const T& num) {
    Matrix<T, x, y> result;

    for (size_t i = 0; i < x; ++i) {
        for (size_t j = 0; j < y; ++j) {
            result.matrix[i][j] = matrix.matrix[i][j] / num;
        }
    }

    return result;
}

template <typename T, size_t x, size_t y>
Matrix<T, x, y>& operator*=(Matrix<T, x, y>& matrix, const T& num) {
    for (size_t i = 0; i < x; ++i) {
        for (size_t j = 0; j < y; ++j) {
            matrix.matrix[i][j] *= num;
        }
    }

    return matrix;
}

template <typename T, size_t x, size_t y>
Matrix<T, x, y>& operator/=(Matrix<T, x, y>& matrix, const T& num) {
    for (size_t i = 0; i < x; ++i) {
        for (size_t j = 0; j < y; ++j) {
            matrix.matrix[i][j] /= num;
        }
    }

    return matrix;
}

template <typename T, size_t x, size_t y>
Matrix<T, x, y> operator*(const T& num, const Matrix<T, x, y>& matrix) {
    Matrix<T, x, y> result;

    for (size_t i = 0; i < x; ++i) {
        for (size_t j = 0; j < y; ++j) {
            result.matrix[i][j] = matrix.matrix[i][j] * num;
        }
    }

    return result;
}

template <typename T, size_t x, size_t y>
Matrix<T, x, y> operator/(const T& num, const Matrix<T, x, y>& matrix) {
    Matrix<T, x, y> result;

    for (size_t i = 0; i < x; ++i) {
        for (size_t j = 0; j < y; ++j) {
            result.matrix[i][j] = matrix.matrix[i][j] / num;
        }
    }

    return result;
}

template <typename T, size_t x, size_t y>
Matrix<T, x, y>& operator*=(const T& num, Matrix<T, x, y>& matrix) {
    for (size_t i = 0; i < x; ++i) {
        for (size_t j = 0; j < y; ++j) {
            matrix.matrix[i][j] *= num;
        }
    }

    return matrix;
}

template <typename T, size_t x, size_t y>
Matrix<T, x, y>& operator/=(const T& num, Matrix<T, x, y>& matrix) {
    for (size_t i = 0; i < x; ++i) {
        for (size_t j = 0; j < y; ++j) {
            matrix.matrix[i][j] /= num;
        }
    }

    return matrix;
}

template <typename T, size_t x, size_t y>
Matrix<T, y, x> GetTransposed(const Matrix<T, x, y>& matrix) {
    Matrix<T, y, x> transposed;

    for (size_t i = 0; i < x; ++i) {
        for (size_t j = 0; j < y; ++j) {
            transposed.matrix[j][i] = matrix[i][j];
        }
    }

    return transposed;
}

template <typename T, size_t x, size_t y>
bool operator==(const Matrix<T, x, y>& lhs, const Matrix<T, x, y>& rhs) {
    bool equals = true;

    for (size_t i = 0; (i < x) && equals; ++i) {
        for (size_t j = 0; (j < y) && equals; ++j) {
            equals &= (lhs.matrix[i][j] == rhs.matrix[i][j]);
        }
    }

    return equals;
}

template <typename T, size_t x, size_t y>
bool operator!=(const Matrix<T, x, y>& lhs, const Matrix<T, x, y>& rhs) {
    return !(lhs == rhs);
}

template <typename T, size_t x, size_t y>
std::istream& operator>>(std::istream& in, Matrix<T, x, y>& matrix) {
    for (size_t i = 0; i < x; ++i) {
        for (size_t j = 0; j < y; ++j) {
            in >> matrix.matrix[i][j];
        }
    }
}

template <typename T, size_t x, size_t y>
std::ostream& operator<<(std::ostream& out, const Matrix<T, x, y>& matrix) {
    for (size_t i = 0; i < x; ++i) {
        for (size_t j = 0; j < y; ++j) {
            out << matrix.matrix[i][j];
        }
    }
}