add_library(
    interpolator
    
    src/BaseInterpolator.cpp
    src/CubicInterpolator.cpp
    src/LinearInterpolator.cpp
    src/MinimumCurvatureInterpolator.cpp
    src/Vertex.cpp
    src/Vertices.cpp

    include/interpolator/BaseInterpolator.hpp
    include/interpolator/CubicInterpolator.hpp
    include/interpolator/InterpolatorFactory.hpp
    include/interpolator/LinearInterpolator.hpp
    include/interpolator/MinimumCurvatureInterpolator.hpp
    include/interpolator/Vertex.hpp
    include/interpolator/IInterpolator.hpp
    include/interpolator/Vertices.hpp
)

target_include_directories(interpolator PUBLIC
$<INSTALL_INTERFACE:include>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

install(
    FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/interpolator/BaseInterpolator.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/interpolator/CubicInterpolator.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/interpolator/InterpolatorFactory.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/interpolator/LinearInterpolator.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/interpolator/MinimumCurvatureInterpolator.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/interpolator/Vertex.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/interpolator/IInterpolator.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/interpolator/Vertices.hpp
    DESTINATION
    ${CMAKE_INSTALL_PREFIX}/include/interpolator
     )

install(TARGETS interpolator EXPORT interpolator_export DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/interpolator)

install(EXPORT interpolator_export FILE interpolator-config.cmake DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/interpolator)
