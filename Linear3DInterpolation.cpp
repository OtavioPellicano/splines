#include "Linear3DInterpolation.hpp"

namespace i3d {


double Linear3DInterpolation::inclination_at_position(double curve_length) const
{
    auto const& [v_1, v_2] = this->calculate_adjacent_vertices(curve_length);
    return this->calculate_linear_spline(v_1.curve_length(), v_1.inclination(), v_2.curve_length(), v_2.inclination(), curve_length);
}

double Linear3DInterpolation::azimuth_at_position(double curve_length) const
{
    auto const& [v_1, v_2] = this->calculate_adjacent_vertices(curve_length);
    return this->calculate_linear_spline(v_1.curve_length(), v_1.azimuth(), v_2.curve_length(), v_2.azimuth(), curve_length);
}

double Linear3DInterpolation::calculate_linear_spline(double x_0, double y_0, double x_1, double y_1, double x_star) const
{
    return (y_0 * (x_1 - x_star) + y_1 * (x_star - x_0)) / (x_1 - x_0);
}

}
