cmake_minimum_required(VERSION 3.12)

project(interpolator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost COMPONENTS REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

message("boost dir: ${Boost_INCLUDE_DIR}")

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

set(
    PYTHON_TESTS_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/python/_tests
)

set(
    BUILD_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/build
)

set(
    ARTIFACTS_DIR_CPP
    ${BUILD_DIR}/artifacts/cpp
)

set(
    ARTIFACTS_DIR_PYTHON
    ${BUILD_DIR}/artifacts/python
)

add_definitions(-fPIC)
# Workaround to use boost-test from conda environment.
# @see https://github.com/conda-forge/boost-cpp-feedstock/issues/48
add_definitions(-DBoost_NO_BOOST_CMAKE=ON)

add_subdirectory(interpolator/cpp)
add_subdirectory(interpolator/bindings)

file(MAKE_DIRECTORY ${ARTIFACTS_DIR_CPP})
file(COPY ${PYTHON_TESTS_DIR}/. DESTINATION ${ARTIFACTS_DIR_PYTHON})

add_executable(test_boost interpolator/cpp/_tests/test_3d_interpolation.cpp)
target_link_libraries(test_boost PRIVATE interpolator)
target_link_libraries(
    test_boost PRIVATE
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)

add_custom_command(
    TARGET test_boost
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:test_boost> ${ARTIFACTS_DIR_CPP}
)
