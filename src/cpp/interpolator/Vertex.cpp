#include "Vertex.hpp"

namespace i3d {

Vertex::Vertex(double position, double inclination, double azimuth, AngleUnit angle_unit)
    : m_position(position)
    , m_inclination(inclination)
    , m_azimuth(azimuth)
{
    if(angle_unit != AngleUnit::rad)
    {
        m_inclination = this->angle_in(inclination, angle_unit);
        m_azimuth = this->angle_in(azimuth, angle_unit);
    }
}

double Vertex::position() const
{
    return m_position;
}

void Vertex::set_position(double position)
{
    m_position = position;
}

double Vertex::inclination(AngleUnit angle_unit) const
{
    return angle_out(m_inclination, angle_unit);
}

void Vertex::set_inclination(double inclination)
{
    m_inclination = inclination;
}

double Vertex::azimuth(AngleUnit angle_unit) const
{
    return angle_out(m_azimuth, angle_unit);
}

void Vertex::set_azimuth(double azimuth)
{
    m_azimuth = azimuth;
}

void Vertex::calculate_tangent(const Vertex &vt, Point &point) const
{
    point.x = vt.m_position * sin(vt.m_inclination) * cos(vt.m_azimuth);
    point.y = vt.m_position * sin(vt.m_inclination) * sin(vt.m_azimuth);
    point.z = vt.m_position * cos(vt.m_inclination);
}

double Vertex::euclidean_distance(const Point &point_1, const Point &point_2) const
{
    return sqrt((point_1.x - point_2.x) * (point_1.x - point_2.x) +
                (point_1.y - point_2.y) * (point_1.y - point_2.y) +
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

}
