cmake_minimum_required(VERSION 3.21)
project(Sem_2)

set(CMAKE_CXX_STANDARD 20)

add_executable(Sem_2 main.cpp)
add_executable(TestsRunRational test/test_rational.cpp)

add_library(Rational contest_1/rational/rational.h contest_1/rational/rational.cpp)
target_link_libraries(TestsRunRational Rational)

target_include_directories(TestsRunRational PUBLIC contest_1/rational)