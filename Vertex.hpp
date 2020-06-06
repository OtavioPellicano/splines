#ifndef VERTICESDEFINITION_HPP
#define VERTICESDEFINITION_HPP

#include <vector>
#include <cmath>

namespace i3d {

struct Point
{
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
};

struct Vertex
{
    double curve_length = 0.0;
    double inclination = 0.0;     //angle from z axis
    double azimuth = 0.0;         //angle from x axis

    bool operator<(const Vertex& vt) const
    {
        return this->curve_length < vt.curve_length;
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

private:
    void calculate_tangent(const Vertex& vt, Point& point) const
    {
        point.x = vt.curve_length * sin(vt.inclination) * cos(vt.azimuth);
        point.y = vt.curve_length * sin(vt.inclination) * sin(vt.azimuth);
        point.z = vt.curve_length * cos(vt.inclination);
    }

    double euclidean_distance(const Point& point_1, const Point& point_2) const
    {
        return sqrt((point_1.x - point_2.x) * (point_1.x - point_2.x) +
                    (point_1.y - point_2.y) * (point_1.y - point_2.y) +
                    (point_1.z - point_2.z) * (point_1.z - point_2.z));
    }

};

typedef std::pair<Vertex, Vertex> AdjacentVertices;
typedef std::vector<Vertex> Vertices;

}

#endif // VERTICESDEFINITION_HPP
