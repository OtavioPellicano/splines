#ifndef BASE3DINTERPOLATION_HPP
#define BASE3DINTERPOLATION_HPP

#include "I3DInterpolation.hpp"
#include <algorithm>

namespace i3d {

class Base3DInterpolation : public I3DInterpolation
{
public:
    virtual ~Base3DInterpolation() = default;

    Base3DInterpolation(const Vertices& vertices);

    const Vertices & vertices() const;

    AdjacentVertices calculate_adjacent_vertices(double curve_length) const override;

    Vertex vertex_at_position(double curve_length) const override;

    //TODO: Remove this implementation after specific interpolation was created

    double calculate_x_projection(double curve_length) const {}
    double calculate_y_projection(double curve_length) const {}
    double calculate_z_projection(double curve_length) const {}

private:
    Vertices m_vertices;

};

}

#endif // BASE3DINTERPOLATION_HPP
