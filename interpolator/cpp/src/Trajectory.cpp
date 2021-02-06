#include "interpolator/Trajectory.hpp"

namespace splines
{

Trajectory::Trajectory(const std::initializer_list<Vertex> &vertices, AngleUnit angle_unit)
{
    set_vertices(vertices, angle_unit);
}

Trajectory::Trajectory(const VerticesType &vertices, AngleUnit angle_unit)
{
    set_vertices(vertices, angle_unit);
}

Trajectory::Trajectory(const std::vector<Vertex> &vertices, AngleUnit angle_unit)
{
    set_vertices(vertices, angle_unit);
}

const VerticesType &Trajectory::vertices() const
{
    return m_vertices;
}

std::vector<Vertex> Trajectory::vertices_python() const
{
    std::vector<Vertex> vertices_p(m_vertices.size());
    std::copy(m_vertices.begin(), m_vertices.end(), vertices_p.begin());
    return vertices_p;
}

template <typename VerticesContainer>
void Trajectory::set_vertices(const VerticesContainer &vertices, AngleUnit angle_unit)
{
    for (auto &vertex : vertices)
    {
        m_vertices.emplace(Vertex(vertex.position(), vertex.inclination(), vertex.azimuth(), angle_unit));
    }
}

void Trajectory::add_n_drop(const Vertex &vertex)
{
    m_vertices.emplace(vertex);
    m_vertices.erase(*m_vertices.rbegin());
}

void Trajectory::drop_n_add(const Vertex &vertex)
{
    m_vertices.erase(*m_vertices.begin());
    m_vertices.emplace(vertex);
}

size_t Trajectory::size() const
{
    return m_vertices.size();
}

} // namespace splines
