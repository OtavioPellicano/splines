#include "Linear3DInterpolation.hpp"

namespace i3d {

double Linear3DInterpolation::inclination_at_position(double curve_length, const AdjacentVertices& adjacent_vertices) const
{
    auto const& [v_1, v_2] = adjacent_vertices;
    return this->calculate_linear_spline(v_1.curve_length(), v_1.inclination(), v_2.curve_length(), v_2.inclination(), curve_length);
}

double Linear3DInterpolation::azimuth_at_position(double curve_length, const AdjacentVertices& adjacent_vertices) const
{
    auto const& [v_1, v_2] = adjacent_vertices;
    return this->calculate_linear_spline(v_1.curve_length(), v_1.azimuth(), v_2.curve_length(), v_2.azimuth(), curve_length);
}

double Linear3DInterpolation::calculate_linear_spline(double x_1, double y_1, double x_2, double y_2, double x_star) const
{
    auto delta_x = (x_2 - x_1);
    if(delta_x < std::numeric_limits<double>::epsilon())
    {
        return y_1;
    }
    return (y_1 * (x_2 - x_star) + y_2 * (x_star - x_1)) / delta_x;
}

}
