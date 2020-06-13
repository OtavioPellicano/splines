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
    void set_vertices(const Vertices &vertices);

    AdjacentVertices calculate_adjacent_vertices(double curve_length) const override;

    Vertex vertex_at_position(double curve_length) const override;

    void add_n_drop(const Vertex& vertex) override;
    void drop_n_add(const Vertex& vertex) override;

    double x_at_position(double curve_length) const override;
    double y_at_position(double curve_length) const override;
    double z_at_position(double curve_length) const override;

private:
    double projection_at_position(double (I3DInterpolation::* delta_calculator)(double, const AdjacentVertices&) const, double curve_length) const;


private:
    Vertices m_vertices;

};

}
#endif // BASE3DINTERPOLATION_HPP
