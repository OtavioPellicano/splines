#include "Base3DInterpolation.hpp"

namespace i3d {

Base3DInterpolation::Base3DInterpolation(const Vertices &vertices)
    : _vertices(vertices)
{
    std::sort(_vertices.begin(), _vertices.end());
}

const Vertices &Base3DInterpolation::vertices() const
{
    return _vertices;
}

AdjacentVertices Base3DInterpolation::calculate_adjacent_vertices(double curve_length) const
{
    auto compare_vertices = [](double curve_length, Vertex const& vt) -> bool {
        return curve_length < vt.curve_length();
    };

    auto upper_vertex =
        std::upper_bound(_vertices.begin(), _vertices.end(), curve_length, compare_vertices);

    if (std::fabs(upper_vertex->curve_length()) > std::numeric_limits<double>::epsilon()) {
        return { *std::prev(upper_vertex), *upper_vertex };
    } else {
        return { *std::prev(upper_vertex), *std::prev(upper_vertex) };
    }
}

}
