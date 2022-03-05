cmake_minimum_required(VERSION 3.21)
project(Sem_2)

set(CMAKE_CXX_STANDARD 20)

add_executable(Sem_2 main.cpp)
add_executable(TestsRunRational test/test_rational.cpp)
add_executable(TestsRunMatrix test/test_matrix.cpp)
add_executable(TestsRunBigInt test/test_big_int.cpp)

add_library(Rational contest_1/rational/rational.h contest_1/rational/rational.cpp)
add_library(Matrix contest_1/matrix/matrix.h contest_1/matrix/matrix.cpp)
add_library(BigInt contest_1/big_int/big_int.h contest_1/big_int/big_int.cpp)

target_link_libraries(TestsRunRational Rational)
target_link_libraries(TestsRunMatrix Matrix)
target_link_libraries(TestsRunBigInt BigInt)

target_include_directories(TestsRunRational PUBLIC contest_1/rational)
target_include_directories(TestsRunMatrix PUBLIC contest_1/matrix)
target_include_directories(TestsRunBigInt PUBLIC contest_1/big_int)