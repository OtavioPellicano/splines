#ifndef I3DINTERPOLATION_H
#define I3DINTERPOLATION_H

#include "Vertex.hpp"

namespace i3d {

class I3DInterpolation
{
public:
    typedef std::pair<Vertex, Vertex> AdjacentVertices;
    virtual Vertex vertex_at_position(double curve_length) const = 0;
    virtual AdjacentVertices calculate_adjacent_vertices(double curve_length) = 0;
    virtual double calculate_inclination(double curve_length) const = 0;
    virtual double calculate_azimuth(double curve_length) const = 0;
    virtual double calculate_x_projection(double curve_length) = 0;
    virtual double calculate_y_projection(double curve_length) = 0;
    virtual double calculate_z_projection(double curve_length) = 0;

};

}

#endif // I3DINTERPOLATION_H
