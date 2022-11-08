#include "interpolator/Vertices.hpp"

namespace splines
{

Vertices::Vertices(const std::initializer_list<Vertex> &vertices, AngleUnit angle_unit)
{
    set_vertices(vertices, angle_unit);
}

Vertices::Vertices(const std::set<Vertex> &vertices, AngleUnit angle_unit)
{
    set_vertices(vertices, angle_unit);
}

Vertices::Vertices(const std::vector<Vertex> &vertices, AngleUnit angle_unit)
{
    set_vertices(vertices, angle_unit);
}

const std::set<Vertex> &Vertices::vertices() const
{
    return this->_vertices;
}

std::vector<Vertex> Vertices::vertices_python() const
{
    auto vertices_p = std::vector<Vertex>(this->_vertices.size());
    std::copy(this->_vertices.begin(), this->_vertices.end(), vertices_p.begin());
    return vertices_p;
}

template <typename VerticesContainer>
void Vertices::set_vertices(const VerticesContainer &vertices, AngleUnit angle_unit)
{
    for (auto &vertex : vertices)
    {
        this->_vertices.emplace(Vertex(vertex.position(), vertex.inclination(), vertex.azimuth(), angle_unit));
    }
}

void Vertices::add_n_drop(const Vertex &vertex)
{
    this->_vertices.emplace(vertex);
    this->_vertices.erase(*this->_vertices.rbegin());
}

void Vertices::drop_n_add(const Vertex &vertex)
{
    this->_vertices.erase(*this->_vertices.begin());
    this->_vertices.emplace(vertex);
}

size_t Vertices::size() const
{
    return this->_vertices.size();
}

std::vector<double> Vertices::positions() const
{
    auto res = std::vector<double>(this->_vertices.size());
    std::transform(this->_vertices.begin(), this->_vertices.end(), res.begin(), [](const Vertex &vt) -> double {
        return vt.position();
    });

    return res;
}

std::vector<double> Vertices::inclinations(AngleUnit angle_unit) const
{
    auto res = std::vector<double>(this->_vertices.size());
    std::transform(
        this->_vertices.begin(), this->_vertices.end(), res.begin(),
        [angle_unit = angle_unit](const Vertex &vt) -> double { return vt.inclination(angle_unit); });

    return res;
}

std::vector<double> Vertices::azimuths(AngleUnit angle_unit) const
{
    auto res = std::vector<double>(this->_vertices.size());
    std::transform(
        this->_vertices.begin(), this->_vertices.end(), res.begin(),
        [angle_unit = angle_unit](const Vertex &vt) -> double { return vt.azimuth(angle_unit); });

    return res;
}

bool Vertices::approx_equal(const Vertices &other, double tol_radius) const
{
    return std::equal(
        this->_vertices.cbegin(), this->_vertices.cend(), other.cbegin(), other.cend(),
        [tol_radius = tol_radius](const Vertex &v1, const Vertex &v2) { return v1.approx_equal(v2, tol_radius); });
}

std::string Vertices::delimiter() const
{
    return this->_vertices.cbegin()->delimiter();
}

std::set<Vertex>::iterator Vertices::begin()
{
    return this->_vertices.begin();
}

std::set<Vertex>::iterator Vertices::end()
{
    return this->_vertices.end();
}

std::set<Vertex>::const_iterator Vertices::cbegin() const
{
    return this->_vertices.cbegin();
}

std::set<Vertex>::const_iterator Vertices::cend() const
{
    return this->_vertices.cend();
}

} // namespace splines
