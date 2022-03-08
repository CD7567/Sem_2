#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <array>
#include <type_traits>

#include "matrix.h"
#include "matrix.h"  // check include guards

template<class T, size_t N, size_t M>
void EqualMatrix(const Matrix<T, N, M>& matrix, const std::array<std::array<T, M>, N>& arr) {
  for (size_t i = 0u; i < N; ++i) {
    for (size_t j = 0u; j < M; ++j) {
      REQUIRE(matrix(i, j) == arr[i][j]);
    }
  }
}

TEST_CASE("Matrix") {
  static_assert(sizeof(Matrix<int, 17, 2>) == sizeof(int) * 34);

  Matrix<int, 1, 2> a{1, -1};
  Matrix<int, 2, 1> b{-1, 2};
  REQUIRE(std::as_const(a).RowsNumber() == 1);
  REQUIRE(std::as_const(a).ColumnsNumber() == 2);
  REQUIRE(std::as_const(b).RowsNumber() == 2);
  REQUIRE(std::as_const(b).ColumnsNumber() == 1);

// a = [1, -1]
// b = [-1 ]
//     [ 2 ]

  a(0, 1) = -2;
  REQUIRE(std::as_const(a)(0, 1) == -2);

  b.At(1, 0) = 3;
  REQUIRE(std::as_const(b).At(1, 0) == 3);

// a = [1, -2]
// b = [-1 ]
//     [ 3 ]

  REQUIRE_THROWS_AS(std::as_const(a).At(5, 5), MatrixOutOfRange);  // NOLINT

  EqualMatrix(a += a, {2, -4});
  EqualMatrix(b + b, {-2, 6});

// a = [2, -4]
// b = [-1 ]
//     [ 3 ]

  EqualMatrix(a -= (GetTransposed(b) + GetTransposed(b)), {4, -10});
  EqualMatrix(b - b, {0, 0});

// a = [4, -10]
// b = [-1 ]
//     [ 3 ]

  EqualMatrix(a * b, {-34});
  EqualMatrix(b *= Matrix<int, 1, 1>{-1}, {1, -3});

// a = [4, -10]
// b = [ 1 ]
//     [-3 ]

  EqualMatrix(a *= -1l, {-4, 10});
  EqualMatrix(2ul * b, {2, -6});

// a = [-4, 10]
// b = [ 1 ]
//     [-3 ]

  EqualMatrix(a / 2ul, {-2, 5});
  EqualMatrix(b /= -1l, {-1, 3});

// a = [-4, 10]
// b = [-1 ]
//     [ 3 ]

  REQUIRE(a == a);
  REQUIRE(a != GetTransposed(b));

  std::stringstream is{"-5 1\n0 10"};

  Matrix<int, 2, 2> matrix{};
  is >> matrix;
  EqualMatrix(matrix, std::array<std::array<int, 2>, 2>{-5, 1, 0, 10});

  std::stringstream os;
  os << std::as_const(matrix);
  REQUIRE(os.str() == "-5 1\n0 10\n");
}

