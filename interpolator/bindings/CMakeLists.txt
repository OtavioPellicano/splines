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

set(
    LIB_PYTHON_NAME
    _interpolator.cpython-39-x86_64-linux-gnu.so
)

set(
    LIB_PYTHON_ORG
    ${CMAKE_CURRENT_BINARY_DIR}/${LIB_PYTHON_NAME}
)

set(
    LIB_PYTHON_DST
    ${PYTHON_TESTS_DIR}/${LIB_PYTHON_NAME}
)

add_custom_command(
    TARGET _interpolator
    POST_BUILD
    COMMAND cp ${LIB_PYTHON_ORG} ${LIB_PYTHON_DST}
    VERBATIM
    )