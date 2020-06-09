#ifndef VERTICESDEFINITION_HPP
#define VERTICESDEFINITION_HPP

#include <set>
#include <cmath>

namespace i3d {

struct Point
{
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
};

enum class AngleUnit
{
    rad,
    deg
};

class Vertex
{
public:

    Vertex(double curve_length = 0.0, double inclination = 0.0, double azimuth = 0.0, AngleUnit angle_unit = AngleUnit::rad);

    bool operator<(const Vertex& vt) const
    {
        return this->m_curve_length < vt.m_curve_length;
    }

    bool operator>(const Vertex& vt) const
    {
        return !(*this < vt);
    }

    bool approx_equal(const Vertex& vt, double tol_radius=1E-6) const
    {
        static Point point_1, point_2;
        this->calculate_tangent(*this, point_1);
        this->calculate_tangent(vt, point_2);

        return this->euclidean_distance(point_1, point_2) < tol_radius;
    }

    double curve_length() const;
    void set_curve_length(double curve_length);

    double inclination(AngleUnit angle_unit=AngleUnit::rad) const;
    void set_inclination(double inclination);

    double azimuth(AngleUnit angle_unit=AngleUnit::rad) const;
    void set_azimuth(double azimuth);


private:
    void calculate_tangent(const Vertex& vt, Point& point) const;

    double euclidean_distance(const Point& point_1, const Point& point_2) const;

    double rad_from_deg(double deg) const;

    double deg_from_rad(double rad) const;

    double angle_in(double angle, AngleUnit angle_unit) const;

    double angle_out(double angle, AngleUnit angle_unit) const;

private:

    double m_curve_length;
    double m_inclination;     //angle from z axis
    double m_azimuth;         //angle from x axis

};

typedef std::pair<Vertex, Vertex> AdjacentVertices;
typedef std::set<Vertex> Vertices;

}

#endif // VERTICESDEFINITION_HPP
