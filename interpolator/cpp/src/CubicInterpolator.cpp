#include "interpolator/CubicInterpolator.hpp"

namespace i3d
{

double CubicInterpolator::inclination_at_position(double position, const AdjacentVertices &adjacent_vertices) const
{
    return this->angle_at_position(position, adjacent_vertices, AngleType::inclination);
}

double CubicInterpolator::azimuth_at_position(double position, const AdjacentVertices &adjacent_vertices) const
{
    return this->angle_at_position(position, adjacent_vertices, AngleType::azimuth);
}

double CubicInterpolator::angle_at_position(
    double position, const AdjacentVertices &adjacent_vertices, BaseInterpolator::AngleType angle_type) const
{
    auto const delta_s_star = position - adjacent_vertices.first.position();
    if (angle_type == AngleType::inclination)
    {
        return fabs(delta_s_star) > std::numeric_limits<double>::epsilon()
                   ? acos(this->calculate_delta_z_projection(position, adjacent_vertices) / delta_s_star)
                   : adjacent_vertices.first.inclination();
    }
    else // azimuth
    {
        auto const inc_star = this->inclination_at_position(position, adjacent_vertices);
        auto const sin_inc_star = sin(inc_star);
        if (sin_inc_star < std::numeric_limits<double>::epsilon() ||
            fabs(inc_star) < std::numeric_limits<double>::epsilon() /*azimuth not defined*/)
        {
            return 0.0;
        }
        else
        {
            return fabs(delta_s_star) > std::numeric_limits<double>::epsilon()
                       ? acos(
                             (this->calculate_delta_x_projection(position, adjacent_vertices) / delta_s_star) /
                             sin_inc_star)
                       : adjacent_vertices.first.azimuth();
        }
    }
}

double CubicInterpolator::calculate_delta_x_projection(double position, const AdjacentVertices &adjacent_vertices) const
{
    return this->calculate_delta_projection(position, adjacent_vertices, ProjectionType::x);
}

double CubicInterpolator::calculate_delta_y_projection(double position, const AdjacentVertices &adjacent_vertices) const
{
    return this->calculate_delta_projection(position, adjacent_vertices, ProjectionType::y);
}

double CubicInterpolator::calculate_delta_z_projection(double position, const AdjacentVertices &adjacent_vertices) const
{
    return this->calculate_delta_projection(position, adjacent_vertices, ProjectionType::z);
}

InterpolationType CubicInterpolator::interpolation_type() const
{
    return m_interpolation_type;
}

double CubicInterpolator::calculate_ep(double position, const AdjacentVertices &adjacent_vertices) const
{
    auto const delta_s = adjacent_vertices.second.position() - adjacent_vertices.first.position();
    return delta_s > std::numeric_limits<double>::epsilon()
               ? (position - adjacent_vertices.first.position()) / (delta_s)
               : 0.0;
}

double CubicInterpolator::calculate_f1(double position, const AdjacentVertices &adjacent_vertices) const
{
    auto const ep = this->calculate_ep(position, adjacent_vertices);
    return 1 + (2 * ep - 3) * ep * ep;
}

double CubicInterpolator::calculate_f2(double position, const AdjacentVertices &adjacent_vertices) const
{
    auto const ep = this->calculate_ep(position, adjacent_vertices);
    return ep * (ep - 1) * (ep - 1) * (adjacent_vertices.second.position() - adjacent_vertices.first.position());
}

double CubicInterpolator::calculate_f3(double position, const AdjacentVertices &adjacent_vertices) const
{
    auto const ep = this->calculate_ep(position, adjacent_vertices);
    return (3 - 2 * ep) * ep * ep;
}

double CubicInterpolator::calculate_f4(double position, const AdjacentVertices &adjacent_vertices) const
{
    auto const ep = this->calculate_ep(position, adjacent_vertices);
    return ep * ep * (ep - 1) * (adjacent_vertices.second.position() - adjacent_vertices.first.position());
}

double CubicInterpolator::calculate_delta_projection(
    double position, const AdjacentVertices &adjacent_vertices, ProjectionType projection_type) const
{
    auto const f1 = this->calculate_f1(position, adjacent_vertices);
    auto const f2 = this->calculate_f2(position, adjacent_vertices);
    auto const f3 = this->calculate_f3(position, adjacent_vertices);
    auto const f4 = this->calculate_f4(position, adjacent_vertices);

    auto const &v_1 = adjacent_vertices.first;
    auto const &v_2 = adjacent_vertices.second;

    auto const sin_inc_1 = sin(v_1.inclination());
    auto const sin_inc_2 = sin(v_2.inclination());
    auto const sin_azm_1 = sin(v_1.azimuth());
    auto const sin_azm_2 = sin(v_2.azimuth());

    auto const cos_azm_1 = cos(v_1.azimuth());
    auto const cos_azm_2 = cos(v_2.azimuth());

    auto a1 = 0.0;
    auto a2 = 0.0;
    auto a3 = 0.0;
    auto a4 = 0.0;

    auto const delta_s = v_2.position() - v_1.position();
    if (projection_type == ProjectionType::x)
    {
        a1 = sin_inc_1 * cos_azm_1;
        a3 = sin_inc_2 * cos_azm_2;

        if (fabs(delta_s) > std::numeric_limits<double>::epsilon())
        {
            auto const cos_inc_1 = cos(v_1.inclination());
            auto const cos_inc_2 = cos(v_2.inclination());

            auto const dinc_ds = this->calculate_delta_angle(v_1.inclination(), v_2.inclination()) / delta_s;
            auto const dazm_ds = this->calculate_delta_angle(v_1.azimuth(), v_1.azimuth()) / delta_s;
            a2 = (cos_inc_1 * cos_azm_1 * dinc_ds - sin_inc_1 * sin_azm_1 * dazm_ds);
            a4 = (cos_inc_2 * cos_azm_2 * dinc_ds - sin_inc_2 * sin_azm_2 * dazm_ds);
        }
        else
        {
            // for delta_s -> 0; delta_inc -> delta_azm -> 0
            a2 = cos(v_1.inclination() + v_1.azimuth());
            a4 = cos(v_2.inclination() + v_2.azimuth());
        }
    }
    else if (projection_type == ProjectionType::y)
    {
        a1 = sin_inc_1 * sin_azm_1;
        a3 = sin_inc_2 * sin_azm_2;

        if (fabs(delta_s) > std::numeric_limits<double>::epsilon())
        {
            auto const cos_inc_1 = cos(v_1.inclination());
            auto const cos_inc_2 = cos(v_2.inclination());

            auto const dinc_ds = this->calculate_delta_angle(v_1.inclination(), v_2.inclination()) / delta_s;
            auto const dazm_ds = this->calculate_delta_angle(v_1.azimuth(), v_1.azimuth()) / delta_s;
            a2 = (cos_inc_1 * sin_azm_1 * dinc_ds + sin_inc_1 * cos_azm_1 * dazm_ds);
            a4 = (cos_inc_2 * sin_azm_2 * dinc_ds + sin_inc_2 * cos_azm_2 * dazm_ds);
        }
        else
        {
            // for delta_s -> 0; delta_inc -> delta_azm -> 0
            a2 = sin(v_1.inclination() + v_1.azimuth());
            a4 = sin(v_2.inclination() + v_2.azimuth());
        }
    }
    else // z
    {
        a1 = cos(v_1.inclination());
        a3 = cos(v_2.inclination());

        if (fabs(delta_s) > std::numeric_limits<double>::epsilon())
        {
            auto const dinc_ds = (v_2.inclination() - v_1.inclination()) / delta_s;
            a2 = -sin(v_1.inclination()) * dinc_ds;
            a4 = -sin(v_2.inclination()) * dinc_ds;
        }
        else
        {
            // for delta_s -> 0; delta_inc -> delta_azm -> 0
            a2 = -sin(v_1.inclination());
            a4 = -sin(v_2.inclination());
        }
    }

    return (a1 * f1 + a2 * f2 + a3 * f3 + a4 * f4) * (position - v_1.position());
}

} // namespace i3d
