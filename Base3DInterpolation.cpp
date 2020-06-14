#include "Base3DInterpolation.hpp"

namespace i3d {

Base3DInterpolation::Base3DInterpolation(const Vertices &vertices)
    : m_vertices(vertices)
{

}

const Vertices &Base3DInterpolation::vertices() const
{
    return m_vertices;
}

void Base3DInterpolation::set_vertices(const Vertices &vertices)
{
    m_vertices = vertices;
}

AdjacentVertices Base3DInterpolation::calculate_adjacent_vertices(double curve_length) const
{
    auto upper_vertex = m_vertices.upper_bound(curve_length);

    if (std::fabs(upper_vertex->curve_length()) > std::numeric_limits<double>::epsilon()) {
        return { *std::prev(upper_vertex), *upper_vertex };
    } else {
        return { *std::prev(upper_vertex), *std::prev(upper_vertex) };
    }
}

Vertex Base3DInterpolation::vertex_at_position(double curve_length) const
{
    if (curve_length < m_vertices.begin()->curve_length()
        || std::fabs(curve_length - m_vertices.begin()->curve_length()) < std::numeric_limits<double>::epsilon()) {
        return *m_vertices.begin();
    } else if (
        curve_length > m_vertices.rbegin()->curve_length()
        || std::fabs(m_vertices.rbegin()->curve_length() - curve_length) < std::numeric_limits<double>::epsilon()) {
        return *m_vertices.rbegin();
    } else {

        auto const& adjacent_vertices = this->calculate_adjacent_vertices(curve_length);

        if (adjacent_vertices.first.approx_equal(adjacent_vertices.second)) {
            return { adjacent_vertices.first };
        }

        auto inclination_interpolated = this->inclination_at_position(curve_length, adjacent_vertices);
        auto azimuth_interpolated = this->azimuth_at_position(curve_length, adjacent_vertices);

        return { curve_length, inclination_interpolated, azimuth_interpolated };
    }
}

void Base3DInterpolation::add_n_drop(const Vertex &vertex)
{
    m_vertices.emplace(vertex);
    m_vertices.erase(*m_vertices.rbegin());
}

void Base3DInterpolation::drop_n_add(const Vertex &vertex)
{
    m_vertices.erase(*m_vertices.begin());
    m_vertices.emplace(vertex);
}

double Base3DInterpolation::x_at_position(double curve_length) const
{
    return this->projection_at_position(&Base3DInterpolation::calculate_delta_x_projection, curve_length);
}

double Base3DInterpolation::y_at_position(double curve_length) const
{
    return this->projection_at_position(&Base3DInterpolation::calculate_delta_y_projection, curve_length);
}

double Base3DInterpolation::z_at_position(double curve_length) const
{
    return this->projection_at_position(&Base3DInterpolation::calculate_delta_z_projection , curve_length);
}

double Base3DInterpolation::projection_at_position(double (Base3DInterpolation::* delta_calculator)(double, const AdjacentVertices&) const, double curve_length) const
{

    auto sum_delta = 0.0;
    for(auto it_v = m_vertices.begin(); it_v != m_vertices.end(); ++it_v)
    {
        if(it_v->curve_length() > curve_length || fabs(curve_length - it_v->curve_length()) < std::numeric_limits<double>::epsilon())
        {
            const AdjacentVertices& adjacent_vertices = {
                it_v != m_vertices.begin() ? *std::prev(it_v) : Vertex{0.0, 0.0, 0.0},
                this->vertex_at_position(curve_length)};

            sum_delta += std::invoke(
                        delta_calculator,
                        *this,
                        adjacent_vertices.second.curve_length(),
                        adjacent_vertices);

            break;
        }

        const AdjacentVertices& adjacent_vertices = {it_v != m_vertices.begin() ? *std::prev(it_v) : Vertex{0.0, 0.0, 0.0}, *it_v};
        sum_delta += std::invoke(
                    delta_calculator,
                    *this,
                    it_v->curve_length(),
                    adjacent_vertices);
    }


    return sum_delta;
}


}
