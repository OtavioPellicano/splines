﻿#ifndef VERTICESDEFINITION_HPP
#define VERTICESDEFINITION_HPP

#include <cmath>
#include <ostream>
#include <tuple>

namespace splines
{

struct Point
{
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
};

/**
 * @brief The AngleUnit enum
 * The angle selector for inclination and azimuth
 */
enum class AngleUnit
{
    rad,
    deg
};

class Vertex;

typedef std::pair<Vertex, Vertex> AdjacentVertices;

/**
 * @brief The Vertex class
 * represents the vertex abstraction composed of:
 *
 * -> position (curve length with the first trajectory vertex as reference)
 * -> inclination: angle from Z to Y axis
 * -> azimuth: angle from X to Y
 */
class Vertex
{
  public:
    Vertex(
        double position = 0.0, double inclination = 0.0, double azimuth = 0.0, AngleUnit angle_unit = AngleUnit::rad);

    std::partial_ordering operator<=>(const Vertex &other) const;

    /**
     * @brief operator <<
     * @param os
     * @param vt
     * @return
     * std::ostream with the Vertex value separeted by a specific delimiter (default: ',')
     * position, inclination, azimuth [-, rad, rad]
     */
    friend std::ostream &operator<<(std::ostream &os, const Vertex &vt)
    {
        os << vt._position << vt._delimiter << vt._inclination << vt._delimiter << vt._azimuth;
        return os;
    }

    bool approx_equal(const Vertex &vt, double tol_radius = 1E-6) const;

    double position() const;
    double inclination(AngleUnit angle_unit = AngleUnit::rad) const;
    double azimuth(AngleUnit angle_unit = AngleUnit::rad) const;

    std::string delimiter() const;

  private:
    void calculate_tangent(const Vertex &vt, Point &point) const;

    double euclidean_distance(const Point &point_1, const Point &point_2) const;

    double rad_from_deg(double deg) const;

    double deg_from_rad(double rad) const;

    /**
     * @brief angle_in
     * convert the given angle to radian
     *
     * @param angle
     * the angle (inclination or azimuth)
     *
     * @param angle_unit
     * the unit angle
     *
     * @see AngleUnit
     *
     * @return
     * the angle [radian]
     *
     */
    double angle_in(double angle, AngleUnit angle_unit) const;

    /**
     * @brief angle_out
     * convert a radian angle to a specific angle defined in angle_unit variable
     *
     * @param angle
     * the angle (inclination or azimuth)
     *
     * @param angle_unit
     * the unit angle
     *
     * @return
     * the angle [AngleUnit]
     *
     */
    double angle_out(double angle, AngleUnit angle_unit) const;

  private:
    double _position;
    double _inclination; // angle from z axis
    double _azimuth;     // angle from x axis

    std::string _delimiter = ","; // ostream delimiter
};

} // namespace splines

/**
 * Tuple-like API for class Vertex for structured bindings (read only)
 * auto [pos, inc, azm] = Vertex;
 */
namespace std
{

template <> struct tuple_size<splines::Vertex>
{
    static constexpr int value = 3;
};

template <std::size_t Idx> struct tuple_element<Idx, splines::Vertex>
{
    using type = double;
};

} // namespace std

namespace splines
{
template <std::size_t I> auto get(const Vertex &v)
{
    static_assert(I < 3);
    if constexpr (I == 0)
    {
        return v.position();
    }
    else if constexpr (I == 1)
    {
        return v.inclination();
    }
    else // I == 2
    {
        return v.azimuth();
    }
}
} // namespace splines

#endif // VERTICESDEFINITION_HPP
