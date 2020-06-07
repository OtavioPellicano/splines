#include "Vertex.hpp"

namespace i3d {

Vertex::Vertex(double curve_length, double inclination, double azimuth, AngleType angle_type)
    : m_curve_length(curve_length)
    , m_inclination(inclination)
    , m_azimuth(azimuth)
    , m_angle_type(angle_type)
{
    if(m_angle_type != AngleType::rad)
    {
        m_inclination = this->angle_in(inclination);
        m_azimuth = this->angle_in(azimuth);
    }
}

double Vertex::curve_length() const
{
    return m_curve_length;
}

void Vertex::set_curve_length(double curve_length)
{
    m_curve_length = curve_length;
}

double Vertex::inclination() const
{
    return angle_out(m_inclination);
}

void Vertex::set_inclination(double inclination)
{
    m_inclination = inclination;
}

double Vertex::azimuth() const
{
    return angle_out(m_azimuth);
}

void Vertex::set_azimuth(double azimuth)
{
    m_azimuth = azimuth;
}

AngleType Vertex::angle_type() const
{
    return m_angle_type;
}

void Vertex::set_angle_type(const AngleType &angle_type)
{
    m_angle_type = angle_type;
}

void Vertex::calculate_tangent(const Vertex &vt, Point &point) const
{
    point.x = vt.m_curve_length * sin(vt.m_inclination) * cos(vt.m_azimuth);
    point.y = vt.m_curve_length * sin(vt.m_inclination) * sin(vt.m_azimuth);
    point.z = vt.m_curve_length * cos(vt.m_inclination);
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

double Vertex::angle_in(double angle) const
{
    switch (m_angle_type)
    {
    case AngleType::deg:
        return rad_from_deg(angle);
    default:
        return angle;
    }
}

double Vertex::angle_out(double angle) const
{
    switch (m_angle_type)
    {
    case AngleType::deg:
        return deg_from_rad(angle);
    default:
        return angle;
    }
}

}
