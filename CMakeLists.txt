cmake_minimum_required(VERSION 3.21)
project(Sem_2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

add_executable(Sem_2 main.cpp)

# ------------------------------------------------------------

add_executable(TestsRunRational test/contest_1/test_rational.cpp)
add_executable(TestsRunMatrix test/contest_1/test_matrix.cpp)
add_executable(TestsRunBigInt test/contest_1/test_big_int.cpp)
add_executable(TestsRunString test/contest_1/test_string.cpp)
add_executable(TestsRunStringView test/contest_1/test_string_view.cpp)

# ------------------------------------------------------------

add_executable(TestsRunGeometry test/contest_2/test_geometry.cpp)
add_executable(TestsRunUniquePtr test/contest_2/test_unique_ptr.cpp)
add_executable(TestsRunSharedPtr test/contest_2/test_shared_ptr.cpp)
add_executable(TestsRunAny test/contest_2/test_any.cpp)
add_executable(TestsRunTokenize test/contest_2/test_tokenize.cpp)

# ------------------------------------------------------------

add_executable(TestsRunVector test/contest_3/test_vector.cpp)
add_executable(TestsRunUnorderedSet test/contest_3/test_unordered_set.cpp)
add_executable(TestsRunRange test/contest_3/test_range.cpp)
add_executable(TestsRunEnumerate test/contest_3/test_enumerate.cpp)
add_executable(TestsRunReversed test/contest_3/test_reversed.cpp)

# ------------------------------------------------------------

add_library(Rational contest_1/rational/rational.h contest_1/rational/rational.cpp)
add_library(BigInt contest_1/big_int/big_integer.h contest_1/big_int/big_integer.cpp contest_1/multiply/multiply.h)
add_library(String contest_1/cpp_string/cppstring.h contest_1/cpp_string/cppstring.cpp)

# ------------------------------------------------------------

set(GEOMETRY_FILES
        contest_2/part_2/geometry/shape.h
        contest_2/part_2/geometry/vector.h
        contest_2/part_2/geometry/point.h
        contest_2/part_2/geometry/segment.h
        contest_2/part_2/geometry/line.h
        contest_2/part_2/geometry/ray.h
        contest_2/part_2/geometry/polygon.h
        contest_2/part_2/geometry/circle.h
        contest_2/part_2/geometry/src/vector.cpp
        contest_2/part_2/geometry/src/point.cpp
        contest_2/part_2/geometry/src/segment.cpp
        contest_2/part_2/geometry/src/line.cpp
        contest_2/part_2/geometry/src/ray.cpp
        contest_2/part_2/geometry/src/polygon.cpp
        contest_2/part_2/geometry/src/circle.cpp
        )

add_library(Geometry ${GEOMETRY_FILES})
add_library(Tokenizer contest_2/part_2/tokenize/tokenize.h contest_2/part_2/tokenize/tokenize.cpp)

# ------------------------------------------------------------

# add_library(Vector contest_3/part_2/vector/vector.h contest_3/part_2/vector/vector.cpp)

# ------------------------------------------------------------

target_link_libraries(Sem_2 PUBLIC String)

# ------------------------------------------------------------

target_link_libraries(TestsRunRational PUBLIC Rational)
target_link_libraries(TestsRunBigInt PUBLIC BigInt)
target_link_libraries(TestsRunString PUBLIC String)

# ------------------------------------------------------------

target_link_libraries(TestsRunGeometry PUBLIC Geometry)
target_link_libraries(TestsRunTokenize PUBLIC Tokenizer)

# ------------------------------------------------------------

# target_link_libraries(TestsRunVector PUBLIC Vector)

# ------------------------------------------------------------

target_include_directories(Sem_2 PUBLIC contest_1/cpp_string)

# ------------------------------------------------------------

target_include_directories(TestsRunRational PUBLIC contest_1/rational)
target_include_directories(TestsRunMatrix PUBLIC contest_1/matrix)
target_include_directories(TestsRunBigInt PUBLIC contest_1/big_int)
target_include_directories(TestsRunString PUBLIC contest_1/cpp_string)
target_include_directories(TestsRunStringView PUBLIC contest_1/string_view)

# ------------------------------------------------------------

target_include_directories(TestsRunGeometry PUBLIC contest_2/part_2/geometry)
target_include_directories(TestsRunUniquePtr PUBLIC contest_2/part_2/unique_ptr)
target_include_directories(TestsRunSharedPtr PUBLIC contest_2/part_2/shared_ptr)
target_include_directories(TestsRunAny PUBLIC contest_2/part_2/any)
target_include_directories(TestsRunTokenize PUBLIC contest_2/part_2/tokenize)

# ------------------------------------------------------------

target_include_directories(TestsRunVector PUBLIC contest_3/part_2/vector)
target_include_directories(TestsRunUnorderedSet PUBLIC contest_3/part_2/unordered_set)
target_include_directories(TestsRunRange PUBLIC contest_3/part_2/iter)
target_include_directories(TestsRunEnumerate PUBLIC contest_3/part_2/iter)
target_include_directories(TestsRunReversed PUBLIC contest_3/part_2/iter)