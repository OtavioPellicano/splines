#ifndef VERTICES_H
#define VERTICES_H

#include <algorithm>
#include <set>
#include <vector>

#include "Vertex.hpp"

namespace splines
{

/**
 * @brief The Vertices class
 * The Vertices class is a vertices wrapper
 */
class Vertices
{
  public:
    Vertices() = default;
    Vertices(const std::initializer_list<Vertex> &vertices, AngleUnit angle_unit = AngleUnit::rad);
    Vertices(const std::set<Vertex> &vertices, AngleUnit angle_unit = AngleUnit::rad);
    Vertices(const std::vector<Vertex> &vertices, AngleUnit angle_unit = AngleUnit::rad);

    const std::set<Vertex> &vertices() const;

    // this method was created to keep vertices sorted in python side
    std::vector<Vertex> vertices_python() const;

    template <typename VerticesContainer>
    void set_vertices(const VerticesContainer &vertices, AngleUnit angle_unit = AngleUnit::rad);

    void add_n_drop(const Vertex &vertex);
    void drop_n_add(const Vertex &vertex);

    size_t size() const;

    // Composite Pattern (@see Vertex)
    std::vector<double> positions() const;
    std::vector<double> inclinations(AngleUnit angle_unit = AngleUnit::rad) const;
    std::vector<double> azimuths(AngleUnit angle_unit = AngleUnit::rad) const;
    bool approx_equal(const Vertices &other, double tol_radius = 1E-6) const;
    std::string delimiter() const;

    // iterators
    std::set<Vertex>::iterator begin();
    std::set<Vertex>::iterator end();
    std::set<Vertex>::const_iterator cbegin() const;
    std::set<Vertex>::const_iterator cend() const;

  private:
    std::set<Vertex> _vertices;
};

} // namespace splines

#endif // VERTICES_H
