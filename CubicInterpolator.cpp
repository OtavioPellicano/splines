#include "CubicInterpolator.hpp"

namespace i3d {

double CubicInterpolator::inclination_at_position(double position, const AdjacentVertices &adjacent_vertices) const
{
    auto const inc_1 = adjacent_vertices.first.inclination();
    auto const inc_2 = adjacent_vertices.second.inclination();
    auto const delta_s = adjacent_vertices.second.position() - adjacent_vertices.first.position();
    auto const delta_inc = adjacent_vertices.second.inclination() - adjacent_vertices.first.inclination();
    auto const dinc_ds = delta_inc / delta_s;
    auto const f1 = calculate_f1(position, adjacent_vertices);
    auto const f2 = calculate_f2(position, adjacent_vertices);
    auto const f3 = calculate_f3(position, adjacent_vertices);
    auto const f4 = calculate_f4(position, adjacent_vertices);

    return acos(cos(inc_1) * f1 - sin(inc_1) * dinc_ds * f2 + cos(inc_2) * f3 - sin(inc_2) * dinc_ds * f4);

}

double CubicInterpolator::azimuth_at_position(double position, const AdjacentVertices &adjacent_vertices) const
{

    auto const sin_inc_star = sin(inclination_at_position(position, adjacent_vertices));
    if(sin_inc_star < std::numeric_limits<double>::epsilon())
    {
        return M_PI / 2.0;
    }
    else
    {
        auto const& v_1 = adjacent_vertices.first;
        auto const& v_2 = adjacent_vertices.second;

        auto const delta_s = v_2.position() - v_1.position();

        auto const dinc_ds = (v_2.inclination() - v_1.inclination()) / delta_s;
        auto const dazm_ds = (v_2.azimuth() - v_1.azimuth()) / delta_s;

        auto const sin_inc_1 = sin(v_1.inclination());
        auto const sin_inc_2 = sin(v_2.inclination());
        auto const cos_inc_1 = cos(v_1.inclination());
        auto const cos_inc_2 = cos(v_2.inclination());


        auto const sin_azm_1 = sin(v_1.azimuth());
        auto const sin_azm_2 = sin(v_2.azimuth());
        auto const cos_azm_1 = cos(v_1.azimuth());
        auto const cos_azm_2 = cos(v_2.azimuth());

        auto const f1 = calculate_f1(position, adjacent_vertices);
        auto const f2 = calculate_f2(position, adjacent_vertices);
        auto const f3 = calculate_f3(position, adjacent_vertices);
        auto const f4 = calculate_f4(position, adjacent_vertices);

        auto res = f1 * sin_inc_1 * cos_azm_1;
        res += f2 * (cos_inc_1 * cos_azm_1 * dinc_ds - sin_inc_1 * sin_azm_1 * dazm_ds);
        res += f3 * (sin_inc_2 * cos_azm_2);
        res += f4 * (cos_inc_2 * cos_azm_2 * dinc_ds - sin_inc_2 * sin_azm_2 * dazm_ds);
        res /= sin_inc_star;
        return acos(res);

    }

}

double CubicInterpolator::calculate_delta_x_projection(double position, const AdjacentVertices &adjacent_vertices) const
{
    return 0.0;
}

double CubicInterpolator::calculate_delta_y_projection(double position, const AdjacentVertices &adjacent_vertices) const
{
    return 0.0;
}

double CubicInterpolator::calculate_delta_z_projection(double position, const AdjacentVertices &adjacent_vertices) const
{
    return 0.0;
}

InterpolationType CubicInterpolator::interpolation_type() const
{
    return m_interpolation_type;
}

double CubicInterpolator::calculate_ep(double position, const AdjacentVertices &adjacent_vertices) const
{
    auto const delta_s = adjacent_vertices.second.position() - adjacent_vertices.first.position();
    return delta_s > std::numeric_limits<double>::epsilon() ? (position - adjacent_vertices.first.position()) / (delta_s) : 0.0;
}

double CubicInterpolator::calculate_f1(double position, const AdjacentVertices &adjacent_vertices) const
{
    auto const ep = calculate_ep(position, adjacent_vertices);
    return 1 + (2 * ep - 3) * ep * ep;
}

double CubicInterpolator::calculate_f2(double position, const AdjacentVertices &adjacent_vertices) const
{
    auto const ep = calculate_ep(position, adjacent_vertices);
    return ep * (ep - 1) * (ep - 1) * (adjacent_vertices.second.position() - adjacent_vertices.first.position());
}

double CubicInterpolator::calculate_f3(double position, const AdjacentVertices &adjacent_vertices) const
{
    auto const ep = calculate_ep(position, adjacent_vertices);
    return (3 - 2 * ep) * ep * ep;
}

double CubicInterpolator::calculate_f4(double position, const AdjacentVertices &adjacent_vertices) const
{
    auto const ep = calculate_ep(position, adjacent_vertices);
    return ep * ep * (ep - 1) * (adjacent_vertices.second.position() - adjacent_vertices.first.position());
}



}
