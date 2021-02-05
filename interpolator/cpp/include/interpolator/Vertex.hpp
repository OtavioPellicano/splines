#ifndef VERTICESDEFINITION_HPP
#define VERTICESDEFINITION_HPP

#include <cmath>
#include <ostream>

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

    bool operator<(const Vertex &vt) const
    {
        return this->m_position < vt.m_position;
    }

    bool operator>(const Vertex &vt) const
    {
        return !(*this < vt);
    }

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
        os << vt.m_position << vt.m_delimiter << vt.m_inclination << vt.m_delimiter << vt.m_azimuth;
        return os;
    }

    bool approx_equal(const Vertex &vt, double tol_radius = 1E-6) const
    {
        static Point point_1, point_2;
        this->calculate_tangent(*this, point_1);
        this->calculate_tangent(vt, point_2);

        return this->euclidean_distance(point_1, point_2) < tol_radius;
    }

    double position() const;
    void set_position(double position);

    double inclination(AngleUnit angle_unit = AngleUnit::rad) const;
    void set_inclination(double inclination);

    double azimuth(AngleUnit angle_unit = AngleUnit::rad) const;
    void set_azimuth(double azimuth);

    std::string delimiter() const;
    void set_delimiter(const std::string &delimiter);

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
    double m_position;
    double m_inclination; // angle from z axis
    double m_azimuth;     // angle from x axis

    std::string m_delimiter = ","; // ostream delimiter
};

} // namespace splines

#endif // VERTICESDEFINITION_HPP
