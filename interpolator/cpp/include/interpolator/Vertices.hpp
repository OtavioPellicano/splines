#ifndef VERTICES_H
#define VERTICES_H

#include <set>
#include <vector>

#include "Vertex.hpp"

namespace splines
{

typedef std::set<Vertex> VerticesType;

class Vertices
{
  public:
    Vertices() = default;
    Vertices(const std::initializer_list<Vertex> &vertices, AngleUnit angle_unit = AngleUnit::rad);
    Vertices(const VerticesType &vertices, AngleUnit angle_unit = AngleUnit::rad);
    Vertices(const std::vector<Vertex> &vertices, AngleUnit angle_unit = AngleUnit::rad);

    const VerticesType &vertices() const;

    // this method was created to keep vertices sorted in python side
    std::vector<Vertex> vertices_python() const;

    template <typename VerticesContainer>
    void set_vertices(const VerticesContainer &vertices, AngleUnit angle_unit = AngleUnit::rad);

    void add_n_drop(const Vertex &vertex);
    void drop_n_add(const Vertex &vertex);

    size_t size() const;

  private:
    VerticesType m_vertices;
};

} // namespace splines

#endif // VERTICES_H
