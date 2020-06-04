#ifndef I3DINTERPOLATION_H
#define I3DINTERPOLATION_H

#include <iostream>

struct Vertex
{
    double curve_length;
    double inclination;     //angle from z axis
    double azimuth;         //angle from x axis
    double x;               //x axis projection
    double y;               //y axis projection
    double z;               //z axis projection
};

class I3DInterpolation
{
public:
    typedef std::pair<Vertex, Vertex> AdjacentVertices;
    virtual Vertex vertex_at_position(double curve_length) const = 0;
    virtual AdjacentVertices calculate_adjacent_vertices(double curve_length) = 0;
    virtual double calculate_inclination(AdjacentVertices const& vertices, double curve_length) const = 0;
    virtual double calculate_azimuth(AdjacentVertices const& vertices, double curve_length) const = 0;
    virtual double calculate_x_projection(AdjacentVertices const& vertices, double curve_length) = 0;
    virtual double calculate_y_projection(AdjacentVertices const& vertices, double curve_length) = 0;
    virtual double calculate_z_projection(AdjacentVertices const& vertices, double curve_length) = 0;

};

#endif // I3DINTERPOLATION_H
