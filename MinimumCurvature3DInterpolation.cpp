#include "MinimumCurvature3DInterpolation.hpp"

namespace i3d {

double MinimumCurvature3DInterpolation::inclination_at_position(double curve_length, const AdjacentVertices &adjacent_vertices) const
{
    return this->angle_at_position(curve_length, adjacent_vertices, AngleType::inclination);
}

double MinimumCurvature3DInterpolation::azimuth_at_position(double curve_length, const AdjacentVertices &adjacent_vertices) const
{
    return this->angle_at_position(curve_length, adjacent_vertices, AngleType::azimuth);
}

double MinimumCurvature3DInterpolation::calculate_alpha(const AdjacentVertices &adjacent_vertices) const
{
    auto const& v_1 = adjacent_vertices.first;
    auto const& v_2 = adjacent_vertices.second;

    auto const arg =
            sin(v_1.inclination()) * cos(v_2.azimuth() - v_1.azimuth()) * sin(v_2.inclination()) +
            cos(v_1.inclination()) * cos(v_2.inclination());

    if (arg > 1.0) {
        return 0.0;
    } else if (arg < -1.0) {
        return M_PI;
    } else {
        return acos(arg);
    }
}

double MinimumCurvature3DInterpolation::calculate_delta_x_projection(double curve_length, const AdjacentVertices &adjacent_vertices) const
{
    auto const& [v_1, v_2] = adjacent_vertices;
    auto const [delta_s, factor_f] = this->calculate_common_delta_projection(curve_length, adjacent_vertices);

    return (delta_s / 2.0) * (sin(v_2.inclination()) * cos(v_2.azimuth()) + sin(v_1.inclination()) * cos(v_1.azimuth())) * factor_f;
}

double MinimumCurvature3DInterpolation::calculate_delta_y_projection(double curve_length, const AdjacentVertices &adjacent_vertices) const
{
    auto const& [v_1, v_2] = adjacent_vertices;
    auto const [delta_s, factor_f] = this->calculate_common_delta_projection(curve_length, adjacent_vertices);

    return (delta_s / 2.0) * (sin(v_2.inclination()) * sin(v_2.azimuth()) + sin(v_1.inclination()) * sin(v_1.azimuth())) * factor_f;
}

double MinimumCurvature3DInterpolation::calculate_delta_z_projection(double curve_length, const AdjacentVertices &adjacent_vertices) const
{
    auto const& [v_1, v_2] = adjacent_vertices;
    auto const [delta_s, factor_f] = this->calculate_common_delta_projection(curve_length, adjacent_vertices);

    return (delta_s / 2.0) * (cos(v_2.inclination()) + cos(v_1.inclination()) ) * factor_f;
}

std::pair<double, double> MinimumCurvature3DInterpolation::calculate_common_delta_projection(double curve_length, const AdjacentVertices &adjacent_vertices) const
{
    auto const& v_1 = adjacent_vertices.first;
    auto const delta_s = curve_length - v_1.curve_length();
    if(delta_s < std::numeric_limits<double>::epsilon())
    {
        return {0.0, 0.0};
    }

    auto const alpha = std::max(this->calculate_alpha(adjacent_vertices), std::numeric_limits<double>::epsilon());
    auto factor_f = (2.0 / alpha) * tan(alpha / 2.0);

    return {delta_s, factor_f};
}

double MinimumCurvature3DInterpolation::angle_at_position(double curve_length, const AdjacentVertices &adjacent_vertices, AngleType angle_type) const
{
    auto const alpha = this->calculate_alpha(adjacent_vertices);

    auto const& v_1 = adjacent_vertices.first;
    auto const& v_2 = adjacent_vertices.second;

    auto const delta_md = v_2.curve_length() - v_1.curve_length();
    auto delta_md_star = curve_length - v_1.curve_length();

    // Avoid flaky
    if (delta_md_star > delta_md) {
        delta_md_star = delta_md - std::numeric_limits<double>::epsilon();
    }
    // Avoid flaky
    if (delta_md_star < std::numeric_limits<double>::epsilon()) {
        delta_md_star = std::numeric_limits<double>::epsilon();
    }

    if (angle_type == AngleType::inclination) {

        auto const weight = delta_md_star / delta_md;
        auto const comp_weight = 1 - weight;

        if (alpha < std::numeric_limits<double>::epsilon()) {
            return v_1.inclination();
        } else {
            auto const numerator =
            (sin(comp_weight * alpha) * cos(v_1.inclination())
            + sin(weight * alpha) * cos(v_2.inclination()));

            return acos(numerator / sin(alpha));
        }
    } else {
        if (std::fabs(v_2.inclination()) < std::numeric_limits<double>::epsilon()) {
            return 0.0;
        } else {
            if (std::fabs(v_2.azimuth() - v_1.azimuth()) < std::numeric_limits<double>::epsilon()){   // straight hole condition
                return v_2.azimuth();
            } else {
                auto const sin_inc_1 = sin(v_1.inclination());
                auto const sin_azm_1 = sin(v_1.azimuth());
                auto const cos_azm_1 = cos(v_1.azimuth());
                auto const sin_inc_2 = sin(v_2.inclination());
                auto const sin_azm_2 = sin(v_2.azimuth());
                auto const cos_azm_2 = cos(v_2.azimuth());

                auto azm_star = atan2(
                 sin_inc_1 * sin_azm_1 * sin((1 - delta_md_star / delta_md) * alpha)
                     + sin_inc_2 * sin_azm_2 * (sin(delta_md_star * alpha / delta_md)),
                 sin_inc_1 * cos_azm_1 * sin((1 - delta_md_star / delta_md) * alpha)
                     + sin_inc_2 * cos_azm_2 * (sin(delta_md_star * alpha / delta_md)));

                return azm_star < 0 ? (azm_star + M_PI * 2) : azm_star;
            }
        }
    }
}


}
