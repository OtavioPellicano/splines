#include "interpolator/Vertex.hpp"

namespace splines
{

Vertex::Vertex(double position, double inclination, double azimuth, AngleUnit angle_unit)
    : _position(position)
    , _inclination(inclination)
    , _azimuth(azimuth)
{
    if (angle_unit != AngleUnit::rad)
    {
        this->_inclination = this->angle_in(inclination, angle_unit);
        this->_azimuth = this->angle_in(azimuth, angle_unit);
    }
}

bool Vertex::operator<(const Vertex &vt) const
{
    return this->_position < vt._position;
}

bool Vertex::operator>(const Vertex &vt) const
{
    return !(*this < vt);
}

bool Vertex::approx_equal(const Vertex &vt, double tol_radius) const
{
    static Point point_1, point_2;
    this->calculate_tangent(*this, point_1);
    this->calculate_tangent(vt, point_2);

    return this->euclidean_distance(point_1, point_2) < tol_radius;
}

double Vertex::position() const
{
    return this->_position;
}

double Vertex::inclination(AngleUnit angle_unit) const
{
    return angle_out(this->_inclination, angle_unit);
}

double Vertex::azimuth(AngleUnit angle_unit) const
{
    return angle_out(this->_azimuth, angle_unit);
}

void Vertex::calculate_tangent(const Vertex &vt, Point &point) const
{
    point.x = vt._position * sin(vt._inclination) * cos(vt._azimuth);
    point.y = vt._position * sin(vt._inclination) * sin(vt._azimuth);
    point.z = vt._position * cos(vt._inclination);
}

double Vertex::euclidean_distance(const Point &point_1, const Point &point_2) const
{
    return sqrt(
        (point_1.x - point_2.x) * (point_1.x - point_2.x) + (point_1.y - point_2.y) * (point_1.y - point_2.y) +
        (point_1.z - point_2.z) * (point_1.z - point_2.z));
}

double Vertex::rad_from_deg(double deg) const
{
    return deg * M_PI / 180.0;
}

double Vertex::deg_from_rad(double rad) const
{
    return rad * 180.0 / M_PI;
}

double Vertex::angle_in(double angle, AngleUnit angle_unit) const
{
    switch (angle_unit)
    {
    case AngleUnit::deg:
        return rad_from_deg(angle);
    default:
        return angle;
    }
}

double Vertex::angle_out(double angle, AngleUnit angle_unit) const
{
    switch (angle_unit)
    {
    case AngleUnit::deg:
        return deg_from_rad(angle);
    default:
        return angle;
    }
}

std::string Vertex::delimiter() const
{
    return this->_delimiter;
}

} // namespace splines
