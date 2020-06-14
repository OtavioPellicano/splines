#include "Linear3DInterpolation.hpp"

namespace i3d {

double Linear3DInterpolation::inclination_at_position(double curve_length, const AdjacentVertices &adjacent_vertices) const
{
    auto const& [v_1, v_2] = adjacent_vertices;
    return this->calculate_linear_spline(v_1.curve_length(), v_1.inclination(), v_2.curve_length(), v_2.inclination(), curve_length);
}

double Linear3DInterpolation::azimuth_at_position(double curve_length, const AdjacentVertices &adjacent_vertices) const
{
    auto const& [v_1, v_2] = adjacent_vertices;
    return this->calculate_linear_spline(v_1.curve_length(), v_1.azimuth(), v_2.curve_length(), v_2.azimuth(), curve_length);
}

double Linear3DInterpolation::calculate_delta_x_projection(double curve_length, const AdjacentVertices &adjacent_vertices) const
{
    auto const& [v_1, v_2] = adjacent_vertices;
    auto delta_s = curve_length - v_1.curve_length();

    return delta_s * sin(v_2.inclination()) * cos(v_2.azimuth());
}

double Linear3DInterpolation::calculate_delta_y_projection(double curve_length, const AdjacentVertices &adjacent_vertices) const
{
    auto const& [v_1, v_2] = adjacent_vertices;
    auto delta_s = curve_length - v_1.curve_length();

    return delta_s * sin(v_2.inclination()) * sin(v_2.azimuth());
}

double Linear3DInterpolation::calculate_delta_z_projection(double curve_length, const AdjacentVertices &adjacent_vertices) const
{
    auto const& [v_1, v_2] = adjacent_vertices;
    auto delta_s = curve_length - v_1.curve_length();

    return delta_s * cos(v_2.inclination());
}

double Linear3DInterpolation::calculate_linear_spline(double x_1, double y_1, double x_2, double y_2, double x_star) const
{
    auto delta_x = (x_2 - x_1);
    return delta_x < std::numeric_limits<double>::epsilon() ? y_1 : (y_1 * (x_2 - x_star) + y_2 * (x_star - x_1)) / delta_x;
}

}
