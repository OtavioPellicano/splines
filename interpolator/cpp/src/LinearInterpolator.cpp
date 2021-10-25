#include "interpolator/LinearInterpolator.hpp"

namespace splines
{

double LinearInterpolator::inclination_at_position(double position, const AdjacentVertices &adjacent_vertices) const
{
    return this->angle_at_position(position, adjacent_vertices, AngleType::inclination);
}

double LinearInterpolator::azimuth_at_position(double position, const AdjacentVertices &adjacent_vertices) const
{
    return this->angle_at_position(position, adjacent_vertices, AngleType::azimuth);
}

double LinearInterpolator::calculate_delta_x_projection(
    double position, const AdjacentVertices &adjacent_vertices) const
{
    auto const &[v_1, v_2] = adjacent_vertices;
    auto delta_s = position - v_1.position();

    return delta_s * sin(v_2.inclination()) * cos(v_2.azimuth());
}

double LinearInterpolator::calculate_delta_y_projection(
    double position, const AdjacentVertices &adjacent_vertices) const
{
    auto const &[v_1, v_2] = adjacent_vertices;
    auto delta_s = position - v_1.position();

    return delta_s * sin(v_2.inclination()) * sin(v_2.azimuth());
}

double LinearInterpolator::calculate_delta_z_projection(
    double position, const AdjacentVertices &adjacent_vertices) const
{
    auto const &[v_1, v_2] = adjacent_vertices;
    auto delta_s = position - v_1.position();

    return delta_s * cos(v_2.inclination());
}

double LinearInterpolator::calculate_linear_spline(
    double position_1, double angle_1, double position_2, double angle_2, double position) const
{
    auto const delta_p = position_2 - position_1;

    return delta_p < std::numeric_limits<double>::epsilon()
               ? angle_2
               : (angle_1 * (position_2 - position) + angle_2 * (position - position_1)) / delta_p;
}

double LinearInterpolator::angle_at_position(
    double position, const AdjacentVertices &adjacent_vertices, AngleType angle_type) const
{
    auto const &[v_1, v_2] = adjacent_vertices;
    auto angle_1 = v_1.inclination();
    auto angle_2 = v_2.inclination();

    if (angle_type == AngleType::azimuth)
    {
        angle_1 = v_1.azimuth();
        angle_2 = v_2.azimuth();

        // azimuth is undefined if inclination is zero
        if (std::fabs(v_1.inclination()) < std::numeric_limits<double>::epsilon())
        {
            if (std::fabs(v_2.inclination()) > std::numeric_limits<double>::epsilon())
            {
                return v_2.azimuth();
            }
            else
            {
                return 0.0;
            }
        }
    }

    auto d_angle = angle_2 - angle_1;
    if (fabs(d_angle) > M_PI)
    {
        if (d_angle > 0.0)
        {
            angle_1 += M_PI * 2;
        }
        else
        {
            angle_2 += M_PI * 2;
        }
    }

    auto res = this->calculate_linear_spline(v_1.position(), angle_1, v_2.position(), angle_2, position);

    res = std::fmod(res, M_PI * 2.0);
    return res < 0.0 ? (res + M_PI * 2.0) : res;
}

} // namespace splines
