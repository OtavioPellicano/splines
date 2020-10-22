TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

isEmpty(BOOST_INCLUDE_DIR): BOOST_INCLUDE_DIR=$$(BOOST_INCLUDE_DIR)
# set by Qt Creator wizard
isEmpty(BOOST_INCLUDE_DIR): BOOST_INCLUDE_DIR="/usr/include/boost"
!isEmpty(BOOST_INCLUDE_DIR): INCLUDEPATH *= $${BOOST_INCLUDE_DIR}

isEmpty(BOOST_INCLUDE_DIR): {
    message("BOOST_INCLUDE_DIR is not set, assuming Boost can be found automatically in your system")
}

SOURCES += \
        BaseInterpolator.cpp \
        CubicInterpolator.cpp \
        InterpolatorBuilder.cpp \
        LinearInterpolator.cpp \
        MinimumCurvatureInterpolator.cpp \
        Vertex.cpp \
        _tests/test_3d_interpolation.cpp

HEADERS += \
    BaseInterpolator.hpp \
    CubicInterpolator.hpp \
    IInterpolator.hpp \
    InterpolatorBuilder.hpp \
    LinearInterpolator.hpp \
    MinimumCurvatureInterpolator.hpp \
    Vertex.hpp
