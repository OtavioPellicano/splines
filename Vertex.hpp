#ifndef VERTICESDEFINITION_HPP
#define VERTICESDEFINITION_HPP

#include <vector>

namespace i3d {

struct Vertex
{
    double curve_length;
    double inclination;     //angle from z axis
    double azimuth;         //angle from x axis
};

typedef std::pair<Vertex, Vertex> AdjacentVertices;
typedef std::vector<Vertex> Vertices;

}

#endif // VERTICESDEFINITION_HPP
