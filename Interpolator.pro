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

message($${PWD})


INCLUDEPATH += /usr/include/python3.8

INCLUDEPATH += /usr/include/pybind11

INCLUDEPATH += /usr/include/eigen3

INCLUDEPATH += src/cpp

INCLUDEPATH += src/bindings

LIBS += -L/usr/local/python3.8/lib -lpython3.8 -ldl -lutil


SOURCES += \
        src/bindings/_interpolator/_interpolator.cpp \
        src/cpp/interpolator/BaseInterpolator.cpp \
        src/cpp/interpolator/CubicInterpolator.cpp \
        src/cpp/interpolator/InterpolatorBuilder.cpp \
        src/cpp/interpolator/LinearInterpolator.cpp \
        src/cpp/interpolator/MinimumCurvatureInterpolator.cpp \
        src/cpp/interpolator/Vertex.cpp \
        src/cpp/interpolator/_tests/test_3d_interpolation.cpp

HEADERS += \
    src/cpp/interpolator/BaseInterpolator.hpp \
    src/cpp/interpolator/CubicInterpolator.hpp \
    src/cpp/interpolator/IInterpolator.hpp \
    src/cpp/interpolator/InterpolatorBuilder.hpp \
    src/cpp/interpolator/LinearInterpolator.hpp \
    src/cpp/interpolator/MinimumCurvatureInterpolator.hpp \
    src/cpp/interpolator/Vertex.hpp

DISTFILES += \
    src/python/interpolator/_tests/test_sum.py
