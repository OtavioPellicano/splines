#ifndef VERTICESDEFINITION_HPP
#define VERTICESDEFINITION_HPP

#include <cmath>
#include <set>

namespace i3d
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
typedef std::set<Vertex> Vertices;

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

    friend bool approx_equal(const Vertices &vertices_1, const Vertices &vertices_2);
};

} // namespace i3d

#endif // VERTICESDEFINITION_HPP
