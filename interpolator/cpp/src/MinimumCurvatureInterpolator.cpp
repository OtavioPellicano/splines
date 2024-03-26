#include "interpolator/MinimumCurvatureInterpolator.hpp"

namespace splines
{

MinimumCurvatureInterpolator::MinimumCurvatureInterpolator(const Vertices &trajectory)
    : BaseInterpolator(trajectory)
{
}

template <typename Interpolator>
MinimumCurvatureInterpolator::MinimumCurvatureInterpolator(Interpolator &&other)
    requires std::same_as<Interpolator, MinimumCurvatureInterpolator>
    : BaseInterpolator(std::forward<Interpolator>(other))
{
}

template <typename Interpolator>
MinimumCurvatureInterpolator &MinimumCurvatureInterpolator::operator=(Interpolator &&rhs)
    requires std::same_as<Interpolator, MinimumCurvatureInterpolator>
{
    *this = std::forward<Interpolator>(rhs);
    return *this;
}

double MinimumCurvatureInterpolator::inclination_at_position(
    double position, const AdjacentVertices &adjacent_vertices) const
{
    return this->angle_at_position(position, adjacent_vertices, AngleType::inclination);
}

double MinimumCurvatureInterpolator::azimuth_at_position(
    double position, const AdjacentVertices &adjacent_vertices) const
{
    return this->angle_at_position(position, adjacent_vertices, AngleType::azimuth);
}

double MinimumCurvatureInterpolator::calculate_alpha(const AdjacentVertices &adjacent_vertices) const
{
    auto const &v_1 = adjacent_vertices.first;
    auto const &v_2 = adjacent_vertices.second;

    auto const arg = sin(v_1.inclination()) * cos(this->calculate_delta_angle(v_1.azimuth(), v_2.azimuth())) *
                         sin(v_2.inclination()) +
                     cos(v_1.inclination()) * cos(v_2.inclination());

    if (arg > 1.0)
    {
        return 0.0;
    }
    else if (arg < -1.0)
    {
        return M_PI;
    }
    else
    {
        return acos(arg);
    }
}

double MinimumCurvatureInterpolator::calculate_delta_x_projection(
    double position, const AdjacentVertices &adjacent_vertices) const
{
    auto const &[v_1, v_2] = adjacent_vertices;
    auto const [delta_s, factor_f] = this->calculate_common_delta_projection(position, adjacent_vertices);

    return (delta_s / 2.0) *
           (sin(v_2.inclination()) * cos(v_2.azimuth()) + sin(v_1.inclination()) * cos(v_1.azimuth())) * factor_f;
}

double MinimumCurvatureInterpolator::calculate_delta_y_projection(
    double position, const AdjacentVertices &adjacent_vertices) const
{
    auto const &[v_1, v_2] = adjacent_vertices;
    auto const [delta_s, factor_f] = this->calculate_common_delta_projection(position, adjacent_vertices);

    return (delta_s / 2.0) *
           (sin(v_2.inclination()) * sin(v_2.azimuth()) + sin(v_1.inclination()) * sin(v_1.azimuth())) * factor_f;
}

double MinimumCurvatureInterpolator::calculate_delta_z_projection(
    double position, const AdjacentVertices &adjacent_vertices) const
{
    auto const &[v_1, v_2] = adjacent_vertices;
    auto const [delta_s, factor_f] = this->calculate_common_delta_projection(position, adjacent_vertices);

    return (delta_s / 2.0) * (cos(v_2.inclination()) + cos(v_1.inclination())) * factor_f;
}

std::pair<double, double> MinimumCurvatureInterpolator::calculate_common_delta_projection(
    double position, const AdjacentVertices &adjacent_vertices) const
{
    auto const &v_1 = adjacent_vertices.first;
    auto const delta_s = position - v_1.position();
    if (delta_s < std::numeric_limits<double>::epsilon())
    {
        return {0.0, 0.0};
    }

    auto const alpha = std::max(this->calculate_alpha(adjacent_vertices), std::numeric_limits<double>::epsilon());
    auto factor_f = (2.0 / alpha) * tan(alpha / 2.0);

    return {delta_s, factor_f};
}

double MinimumCurvatureInterpolator::angle_at_position(
    double position, const AdjacentVertices &adjacent_vertices, AngleType angle_type) const
{
    auto const alpha = this->calculate_alpha(adjacent_vertices);

    auto const &v_1 = adjacent_vertices.first;
    auto const &v_2 = adjacent_vertices.second;

    auto const ds = v_2.position() - v_1.position();
    auto ds_star = position - v_1.position();

    // Avoid flaky
    if (ds_star > ds)
    {
        ds_star = ds - std::numeric_limits<double>::epsilon();
    }
    // Avoid flaky
    if (ds_star < std::numeric_limits<double>::epsilon())
    {
        ds_star = std::numeric_limits<double>::epsilon();
    }

    if (angle_type == AngleType::inclination)
    {

        auto const weight = ds_star / ds;
        auto const comp_weight = 1 - weight;

        if (alpha < std::numeric_limits<double>::epsilon())
        {
            return v_1.inclination();
        }
        else
        {
            auto const numerator =
                (sin(comp_weight * alpha) * cos(v_1.inclination()) + sin(weight * alpha) * cos(v_2.inclination()));

            return acos(numerator / sin(alpha));
        }
    }
    else
    {
        if (std::fabs(v_2.inclination()) < std::numeric_limits<double>::epsilon())
        {
            return 0.0;
        }
        else
        {
            if (std::fabs(v_2.azimuth() - v_1.azimuth()) < std::numeric_limits<double>::epsilon())
            { // straight hole condition
                return v_2.azimuth();
            }
            else
            {
                auto const sin_inc_1 = sin(v_1.inclination());
                auto const sin_azm_1 = sin(v_1.azimuth());
                auto const cos_azm_1 = cos(v_1.azimuth());
                auto const sin_inc_2 = sin(v_2.inclination());
                auto const sin_azm_2 = sin(v_2.azimuth());
                auto const cos_azm_2 = cos(v_2.azimuth());

                auto azm_star = atan2(
                    sin_inc_1 * sin_azm_1 * sin((1 - ds_star / ds) * alpha) +
                        sin_inc_2 * sin_azm_2 * (sin(ds_star * alpha / ds)),
                    sin_inc_1 * cos_azm_1 * sin((1 - ds_star / ds) * alpha) +
                        sin_inc_2 * cos_azm_2 * (sin(ds_star * alpha / ds)));

                return azm_star < 0 ? (azm_star + M_PI * 2) : azm_star;
            }
        }
    }
}

} // namespace splines
