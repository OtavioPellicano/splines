#ifndef VERTICESDEFINITION_HPP
#define VERTICESDEFINITION_HPP

#include <vector>

namespace i3d {

struct Vertex
{
    double curve_length;
    double inclination;     //angle from z axis
    double azimuth;         //angle from x axis

    bool operator<(const Vertex& vt)
    {
        return this->curve_length < vt.curve_length;
    }

    bool operator>(const Vertex& vt)
    {
        return !(*this < vt);
    }

};

typedef std::pair<Vertex, Vertex> AdjacentVertices;
typedef std::vector<Vertex> Vertices;

}

#endif // VERTICESDEFINITION_HPP
