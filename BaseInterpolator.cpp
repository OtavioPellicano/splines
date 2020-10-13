#include "BaseInterpolator.hpp"

namespace i3d {

BaseInterpolator::BaseInterpolator(const Vertices &vertices)
    : m_vertices(vertices)
{

}

const Vertices &BaseInterpolator::vertices() const
{
    return m_vertices;
}

void BaseInterpolator::set_vertices(const Vertices &vertices)
{
    m_vertices = vertices;
}

AdjacentVertices BaseInterpolator::calculate_adjacent_vertices(double position) const
{
    auto upper_vertex = m_vertices.upper_bound(position);

    if (std::fabs(upper_vertex->position()) > std::numeric_limits<double>::epsilon()) {
        return { *std::prev(upper_vertex), *upper_vertex };
    } else {
        return { *std::prev(upper_vertex), *std::prev(upper_vertex) };
    }
}

Vertex BaseInterpolator::vertex_at_position(double position) const
{
    if (position < m_vertices.begin()->position()
        || std::fabs(position - m_vertices.begin()->position()) < std::numeric_limits<double>::epsilon()) {
        return *m_vertices.begin();
    } else if (
        position > m_vertices.rbegin()->position()
        || std::fabs(m_vertices.rbegin()->position() - position) < std::numeric_limits<double>::epsilon()) {
        return *m_vertices.rbegin();
    } else {

        auto const& adjacent_vertices = this->calculate_adjacent_vertices(position);

        if (adjacent_vertices.first.approx_equal(adjacent_vertices.second)) {
            return { adjacent_vertices.first };
        }

        auto inclination_interpolated = this->inclination_at_position(position, adjacent_vertices);
        auto azimuth_interpolated = this->azimuth_at_position(position, adjacent_vertices);

        return { position, inclination_interpolated, azimuth_interpolated };
    }
}

double BaseInterpolator::inclination_at_position(double position) const
{
    return this->inclination_at_position(position, this->calculate_adjacent_vertices(position));
}

double BaseInterpolator::azimuth_at_position(double position) const
{
    return this->azimuth_at_position(position, this->calculate_adjacent_vertices(position));
}

void BaseInterpolator::add_n_drop(const Vertex &vertex)
{
    m_vertices.emplace(vertex);
    m_vertices.erase(*m_vertices.rbegin());
}

void BaseInterpolator::drop_n_add(const Vertex &vertex)
{
    m_vertices.erase(*m_vertices.begin());
    m_vertices.emplace(vertex);
}

double BaseInterpolator::x_at_position(double position) const
{
    return this->projection_at_position(&BaseInterpolator::calculate_delta_x_projection, position);
}

double BaseInterpolator::y_at_position(double position) const
{
    return this->projection_at_position(&BaseInterpolator::calculate_delta_y_projection, position);
}

double BaseInterpolator::z_at_position(double position) const
{
    return this->projection_at_position(&BaseInterpolator::calculate_delta_z_projection , position);
}

double BaseInterpolator::projection_at_position(DeltaCalculator delta_calculator, double position) const
{

    auto sum_delta = 0.0;
    for(auto it_v = m_vertices.begin(); it_v != m_vertices.end(); ++it_v)
    {
        if(it_v->position() > position || fabs(position - it_v->position()) < std::numeric_limits<double>::epsilon())
        {
            const AdjacentVertices& adjacent_vertices = {
                it_v != m_vertices.begin() ? *std::prev(it_v) : Vertex{0.0, 0.0, 0.0},
                this->vertex_at_position(position)};

            sum_delta += std::invoke(
                        delta_calculator,
                        *this,
                        adjacent_vertices.second.position(),
                        adjacent_vertices);

            break;
        }

        const AdjacentVertices& adjacent_vertices = {it_v != m_vertices.begin() ? *std::prev(it_v) : Vertex{0.0, 0.0, 0.0}, *it_v};
        sum_delta += std::invoke(
                    delta_calculator,
                    *this,
                    it_v->position(),
                    adjacent_vertices);
    }


    return sum_delta;
}


}
