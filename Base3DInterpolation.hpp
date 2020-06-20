#ifndef BASE3DINTERPOLATION_HPP
#define BASE3DINTERPOLATION_HPP

#include "I3DInterpolation.hpp"
#include <algorithm>

namespace i3d {

class Base3DInterpolation : public I3DInterpolation
{
public:
    using I3DInterpolation::inclination_at_position;
    using I3DInterpolation::azimuth_at_position;

    virtual ~Base3DInterpolation() = default;

    Base3DInterpolation(const Vertices& vertices);

    const Vertices & vertices() const;
    void set_vertices(const Vertices &vertices);

    AdjacentVertices calculate_adjacent_vertices(double position) const final;

    Vertex vertex_at_position(double position) const override;

    double inclination_at_position(double position) const final;
    double azimuth_at_position(double position) const final;

    void add_n_drop(const Vertex& vertex) final;
    void drop_n_add(const Vertex& vertex) final;

    double x_at_position(double position) const final;
    double y_at_position(double position) const final;
    double z_at_position(double position) const final;

private:

    double projection_at_position(double (Base3DInterpolation::* delta_calculator)(double, const AdjacentVertices&) const, double position) const;

protected:
    virtual double calculate_delta_x_projection(double position, const AdjacentVertices& adjacent_vertices) const = 0;
    virtual double calculate_delta_y_projection(double position, const AdjacentVertices& adjacent_vertices) const = 0;
    virtual double calculate_delta_z_projection(double position, const AdjacentVertices& adjacent_vertices) const = 0;

    virtual double inclination_at_position(double position, const AdjacentVertices& adjacent_vertices) const = 0;
    virtual double azimuth_at_position(double position,  const AdjacentVertices& adjacent_vertices) const = 0;

    enum class AngleType
    {
        inclination,
        azimuth
    };
    virtual double angle_at_position(double position, const AdjacentVertices &adjacent_vertices, AngleType angle_type) const = 0;

private:
    Vertices m_vertices;

};

}
#endif // BASE3DINTERPOLATION_HPP
