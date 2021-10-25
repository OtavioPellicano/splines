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
    return this->_vertices;
}

std::vector<Vertex> Trajectory::vertices_python() const
{
    auto vertices_p = std::vector<Vertex>(this->_vertices.size());
    std::copy(this->_vertices.begin(), this->_vertices.end(), vertices_p.begin());
    return vertices_p;
}

template <typename VerticesContainer>
void Trajectory::set_vertices(const VerticesContainer &vertices, AngleUnit angle_unit)
{
    for (auto &vertex : vertices)
    {
        this->_vertices.emplace(Vertex(vertex.position(), vertex.inclination(), vertex.azimuth(), angle_unit));
    }
}

void Trajectory::add_n_drop(const Vertex &vertex)
{
    this->_vertices.emplace(vertex);
    this->_vertices.erase(*this->_vertices.rbegin());
}

void Trajectory::drop_n_add(const Vertex &vertex)
{
    this->_vertices.erase(*this->_vertices.begin());
    this->_vertices.emplace(vertex);
}

size_t Trajectory::size() const
{
    return this->_vertices.size();
}

} // namespace splines
