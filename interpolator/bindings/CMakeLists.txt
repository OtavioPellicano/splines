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

set_target_properties(
    _interpolator
    PROPERTIES
            OUTPUT_NAME _interpolator
            PREFIX ""
            SUFFIX ".so" #Linux
)

add_custom_command(
    TARGET _interpolator
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:_interpolator> ${ARTIFACTS_DIR_PYTHON}
    )

add_custom_command(
    TARGET _interpolator
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:_interpolator> ${PYTHON_TESTS_DIR}
    )
