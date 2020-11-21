cmake_minimum_required(VERSION 3.15)

project(interpolator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_definitions(-fPIC)

add_subdirectory(interpolator/cpp)
add_subdirectory(interpolator/bindings)

add_executable(test_boost interpolator/cpp/_tests/test_3d_interpolation.cpp)
target_link_libraries(test_boost PRIVATE interpolator)
