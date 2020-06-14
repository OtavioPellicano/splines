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
        Base3DInterpolation.cpp \
        Linear3DInterpolation.cpp \
        MinimumCurvature3DInterpolation.cpp \
        Vertex.cpp \
        _tests/test_3d_interpolation.cpp

HEADERS += \
    Base3DInterpolation.hpp \
    I3DInterpolation.hpp \
    Linear3DInterpolation.hpp \
    MinimumCurvature3DInterpolation.hpp \
    Vertex.hpp
