#include <pybind11/eigen.h>
#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

int sum(int a, int b)
{
    return a + b;
}

namespace py = pybind11;

PYBIND11_MODULE(_interpolator, m)
{
    m.def("Sum", &sum);
}
