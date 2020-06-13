#ifndef I3DINTERPOLATION_H
#define I3DINTERPOLATION_H

#include "Vertex.hpp"

namespace i3d {

class I3DInterpolation
{
public:
    virtual ~I3DInterpolation() = default;

    virtual Vertex vertex_at_position(double curve_length) const = 0;
    virtual AdjacentVertices calculate_adjacent_vertices(double curve_length) const = 0;
    virtual double inclination_at_position(double curve_length) const = 0;
    virtual double azimuth_at_position(double curve_length) const = 0;

    virtual double x_at_position(double curve_length) const = 0;
    virtual double y_at_position(double curve_length) const = 0;
    virtual double z_at_position(double curve_length) const = 0;

    virtual void add_n_drop(const Vertex& vertex) = 0;
    virtual void drop_n_add(const Vertex& vertex) = 0;

    virtual double calculate_delta_x_projection(double curve_length, const AdjacentVertices& adjacent_vertices) const = 0;
    virtual double calculate_delta_y_projection(double curve_length, const AdjacentVertices& adjacent_vertices) const = 0;
    virtual double calculate_delta_z_projection(double curve_length, const AdjacentVertices& adjacent_vertices) const = 0;

};

}

#endif // I3DINTERPOLATION_H
