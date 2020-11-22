find_package(pybind11 CONFIG REQUIRED)

set(
    SOURCE_FILES
    src/_interpolator.cpp
)

pybind11_add_module(
    _interpolator 
    ${SOURCE_FILES} 
)


target_link_libraries(
    _interpolator
    PRIVATE
        interpolator
)

target_include_directories(
    _interpolator
    PRIVATE
        /usr/include/eigen3
    )