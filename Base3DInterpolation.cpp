#include "Base3DInterpolation.hpp"

namespace i3d {

Base3DInterpolation::Base3DInterpolation(const Vertices &vertices)
    : m_vertices(vertices)
{
    std::sort(m_vertices.begin(), m_vertices.end());
}

const Vertices &Base3DInterpolation::vertices() const
{
    return m_vertices;
}

AdjacentVertices Base3DInterpolation::calculate_adjacent_vertices(double curve_length) const
{
    auto compare_vertices = [](double curve_length, Vertex const& vt) -> bool {
        return curve_length < vt.curve_length();
    };

    auto upper_vertex =
        std::upper_bound(m_vertices.begin(), m_vertices.end(), curve_length, compare_vertices);

    if (std::fabs(upper_vertex->curve_length()) > std::numeric_limits<double>::epsilon()) {
        return { *std::prev(upper_vertex), *upper_vertex };
    } else {
        return { *std::prev(upper_vertex), *std::prev(upper_vertex) };
    }
}

Vertex Base3DInterpolation::vertex_at_position(double curve_length) const
{
    if (curve_length < m_vertices.front().curve_length()
        || std::fabs(curve_length - m_vertices.front().curve_length()) < std::numeric_limits<double>::epsilon()) {
        return m_vertices.front();
    } else if (
        curve_length > m_vertices.back().curve_length()
        || std::fabs(m_vertices.back().curve_length() - curve_length) < std::numeric_limits<double>::epsilon()) {
        return m_vertices.back();
    } else {

        auto adjacent_vertices = this->calculate_adjacent_vertices(curve_length);

        if (adjacent_vertices.first.approx_equal(adjacent_vertices.second)) {
            return { adjacent_vertices.first };
        }

        auto inclination_interpolated = this->inclination_at_position(curve_length);
        auto azimuth_interpolated = this->azimuth_at_position(curve_length);

        return { curve_length, inclination_interpolated, azimuth_interpolated };
    }
}

}
