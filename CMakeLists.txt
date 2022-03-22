cmake_minimum_required(VERSION 3.21)
project(Sem_2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")


add_executable(Sem_2 main.cpp)
add_executable(TestsRunRational test/contest_1/test_rational.cpp)
add_executable(TestsRunMatrix test/contest_1/test_matrix.cpp)
add_executable(TestsRunBigInt test/contest_1/test_big_int.cpp)
add_executable(TestsRunString test/contest_1/test_string.cpp)
add_executable(TestsRunStringView test/contest_1/test_string_view.cpp)

add_library(Rational contest_1/rational/rational.h contest_1/rational/rational.cpp)
add_library(BigInt contest_1/big_int/big_integer.h contest_1/big_int/big_integer.cpp contest_1/multiply/multiply.h)
add_library(String contest_1/cpp_string/cppstring.h contest_1/cpp_string/cppstring.cpp)

target_link_libraries(TestsRunRational PUBLIC Rational)
target_link_libraries(TestsRunBigInt PUBLIC BigInt)
target_link_libraries(TestsRunString PUBLIC String)
target_link_libraries(Sem_2 PUBLIC String)

target_include_directories(TestsRunRational PUBLIC contest_1/rational)
target_include_directories(TestsRunMatrix PUBLIC contest_1/matrix)
target_include_directories(TestsRunBigInt PUBLIC contest_1/big_int)
target_include_directories(TestsRunString PUBLIC contest_1/cpp_string)
target_include_directories(TestsRunStringView PUBLIC contest_1/string_view)
target_include_directories(Sem_2 PUBLIC contest_1/cpp_string)