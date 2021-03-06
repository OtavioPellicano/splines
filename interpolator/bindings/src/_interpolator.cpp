#ifndef HPP_INTERPOLATOR_BINDINGS
#define HPP_INTERPOLATOR_BINDINGS

#include <pybind11/eigen.h>
#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include <interpolator/InterpolatorBuilder.hpp>

using namespace splines;
namespace py = pybind11;

class PyIInterpolator : public IInterpolator
{
  public:
    using IInterpolator::IInterpolator;

    const Trajectory &trajectory() const override
    {
        PYBIND11_OVERLOAD_PURE(const Trajectory &, IInterpolator, trajectory);
    }

    void set_trajectory(const Trajectory &trajectory) override
    {
        PYBIND11_OVERLOAD_PURE(void, IInterpolator, set_trajectory, trajectory);
    }

    Vertex vertex_at_position(double position) const override
    {
        PYBIND11_OVERLOAD_PURE(Vertex, IInterpolator, vertex_at_position, position);
    }

    double inclination_at_position(double position) const override
    {
        PYBIND11_OVERLOAD_PURE(double, IInterpolator, inclination_at_position, position);
    }

    double azimuth_at_position(double position) const override
    {
        PYBIND11_OVERLOAD_PURE(double, IInterpolator, azimuth_at_position, position);
    }

    double x_at_position(double position) const override
    {
        PYBIND11_OVERLOAD_PURE(double, IInterpolator, x_at_position, position);
    }

    double y_at_position(double position) const override
    {
        PYBIND11_OVERLOAD_PURE(double, IInterpolator, y_at_position, position);
    }

    double z_at_position(double position) const override
    {
        PYBIND11_OVERLOAD_PURE(double, IInterpolator, z_at_position, position);
    }

    void add_n_drop(const Vertex &vertex) override
    {
        PYBIND11_OVERLOAD_PURE(void, IInterpolator, add_n_drop, vertex);
    }

    void drop_n_add(const Vertex &vertex) override
    {
        PYBIND11_OVERLOAD_PURE(void, IInterpolator, drop_n_add, vertex);
    }

    InterpolationType interpolation_type() const override
    {
        PYBIND11_OVERLOAD_PURE(InterpolationType, IInterpolator, interpolation_type);
    }

    std::string interpolation_type_str() const override
    {
        PYBIND11_OVERLOAD_PURE(std::string, IInterpolator, interpolation_type_str);
    }
};

class PyBaseInterpolator : public BaseInterpolator
{
  public:
    using BaseInterpolator::BaseInterpolator;

    double calculate_delta_x_projection(double position, const AdjacentVertices &adjacent_vertices) const override
    {
        PYBIND11_OVERLOAD_PURE(double, BaseInterpolator, calculate_delta_x_projection, position, adjacent_vertices);
    }

    double calculate_delta_y_projection(double position, const AdjacentVertices &adjacent_vertices) const override
    {
        PYBIND11_OVERLOAD_PURE(double, BaseInterpolator, calculate_delta_y_projection, position, adjacent_vertices);
    }

    double calculate_delta_z_projection(double position, const AdjacentVertices &adjacent_vertices) const override
    {
        PYBIND11_OVERLOAD_PURE(double, BaseInterpolator, calculate_delta_z_projection, position, adjacent_vertices);
    }

    double inclination_at_position(double position, const AdjacentVertices &adjacent_vertices) const override
    {
        PYBIND11_OVERLOAD_PURE(double, BaseInterpolator, inclination_at_position, position, adjacent_vertices);
    }

    double azimuth_at_position(double position, const AdjacentVertices &adjacent_vertices) const override
    {
        PYBIND11_OVERLOAD_PURE(double, BaseInterpolator, azimuth_at_position, position, adjacent_vertices);
    }

    double angle_at_position(
        double position, const AdjacentVertices &adjacent_vertices, AngleType angle_type) const override
    {
        PYBIND11_OVERLOAD_PURE(double, BaseInterpolator, angle_at_position, position, adjacent_vertices, angle_type);
    }
};

PYBIND11_MODULE(_interpolator, m)
{

    py::enum_<AngleUnit>(m, "AngleUnit").value("Rad", AngleUnit::rad).value("Deg", AngleUnit::deg);

    py::class_<Vertex>(m, "Vertex")
        .def(
            py::init<double, double, double, AngleUnit>(), py::arg("position") = 0.0, py::arg("inclination") = 0.0,
            py::arg("azimuth") = 0.0, py::arg("angle_unit") = AngleUnit::rad)
        .def("Position", &Vertex::position)
        .def("SetPosition", &Vertex::set_position, py::arg("position"))
        .def("Inclination", &Vertex::inclination, py::arg("AngleUnit") = AngleUnit::rad)
        .def("SetInclination", &Vertex::set_inclination, py::arg("inclination"))
        .def("Azimuth", &Vertex::azimuth, py::arg("AngleUnit") = AngleUnit::rad)
        .def("SetAzimuth", &Vertex::set_azimuth, py::arg("azimuth"))
        .def(
            "__repr__",
            [](const Vertex &vt) {
                std::stringstream ss;
                ss << vt;
                return ss.str();
            })
        .def("SetDelimiter", &Vertex::set_delimiter, py::arg("delimiter"))
        .def("Delimiter", &Vertex::delimiter);

    py::class_<Trajectory>(m, "Trajectory")
        .def(py::init<>())
        .def(
            py::init<const std::vector<Vertex> &, AngleUnit>(), py::arg("vertex"),
            py::arg("angle_unit") = AngleUnit::rad)
        .def("Vertices", &Trajectory::vertices)
        .def("VerticesSorted", &Trajectory::vertices_python)
        .def(
            "SetVertices", &Trajectory::set_vertices<std::vector<Vertex>>, py::arg("vertices"),
            py::arg("angle_unit") = AngleUnit::rad)
        .def(
            "SetVertices", &Trajectory::set_vertices<VerticesType>, py::arg("vertices"),
            py::arg("angle_unit") = AngleUnit::rad)
        .def("AddNDrop", &Trajectory::add_n_drop, py::arg("vertex"))
        .def("DropNAdd", &Trajectory::drop_n_add, py::arg("vertex"))
        .def("Size", &Trajectory::size);

    py::enum_<InterpolationType>(m, "InterpolationType")
        .value("Linear", InterpolationType::linear)
        .value("MinimumCurvature", InterpolationType::minimum_curvature)
        .value("Cubic", InterpolationType::cubic);

    py::class_<IInterpolator, PyIInterpolator>(m, "IInterpolator")
        .def(py::init<>())
        .def("VertexAtPosition", &IInterpolator::vertex_at_position, py::arg("position"))
        .def("InclinationAtPosition", &IInterpolator::inclination_at_position, py::arg("position"))
        .def("AzimuthAtPosition", &IInterpolator::azimuth_at_position, py::arg("position"))
        .def("XAtPosition", &IInterpolator::x_at_position, py::arg("position"))
        .def("YAtPosition", &IInterpolator::y_at_position, py::arg("position"))
        .def("ZAtPosition", &IInterpolator::z_at_position, py::arg("position"))
        .def("AddNDrop", &IInterpolator::add_n_drop, py::arg("vertex"))
        .def("DropNAdd", &IInterpolator::drop_n_add, py::arg("vertex"))
        .def("InterpolationType", &IInterpolator::interpolation_type)
        .def("InterpolationTypeStr", &IInterpolator::interpolation_type_str);

    py::class_<BaseInterpolator, PyBaseInterpolator, IInterpolator>(m, "BaseInterpolator")
        .def("Trajectory", &BaseInterpolator::trajectory)
        .def("SetTrajectory", &BaseInterpolator::set_trajectory, py::arg("trajectory"));

    py::class_<LinearInterpolator, BaseInterpolator>(m, "LinearInterpolator")
        .def(py::init<const Trajectory &>(), py::arg("trajectory"));
    py::class_<MinimumCurvatureInterpolator, BaseInterpolator>(m, "MinimumCurvatureInterpolator")
        .def(py::init<const Trajectory &>(), py::arg("trajectory"));
    py::class_<CubicInterpolator, BaseInterpolator>(m, "CubicInterpolator")
        .def(py::init<const Trajectory &>(), py::arg("trajectory"));

    // functions
    m.def("BuildInterpolator", &build_interpolator, py::arg("trajectory"), py::arg("interpolation_type"));
}

#endif // HPP_INTERPOLATOR_BINDINGS
