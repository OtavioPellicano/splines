#include "interpolator/BaseInterpolator.hpp"
#include "interpolator/utils/Multithreading.hpp"

namespace splines
{

BaseInterpolator::BaseInterpolator(const Vertices &trajectory)
    : _trajectory(trajectory)
{
}

BaseInterpolator::BaseInterpolator(BaseInterpolator &&other)
    : _trajectory(std::move(other._trajectory))
{
}

BaseInterpolator &BaseInterpolator::operator=(BaseInterpolator &&rhs)
{
    this->_trajectory = std::move(rhs._trajectory);
    return *this;
}

BaseInterpolator::BaseInterpolator(const BaseInterpolator &other)
    : _trajectory(other._trajectory)
{
}

BaseInterpolator &BaseInterpolator::operator=(const BaseInterpolator &rhs)
{
    this->_trajectory = rhs._trajectory;
    return *this;
}

const Vertices &BaseInterpolator::trajectory() const
{
    return this->_trajectory;
}

void BaseInterpolator::set_trajectory(const Vertices &trajectory)
{
    this->_trajectory = trajectory;
}

AdjacentVertices BaseInterpolator::calculate_adjacent_vertices(double position) const
{
    auto upper_vertex = this->_trajectory.vertices().upper_bound(position);

    if (std::fabs(upper_vertex->position()) > std::numeric_limits<double>::epsilon())
    {
        return {*std::prev(upper_vertex), *upper_vertex};
    }
    else
    {
        return {*std::prev(upper_vertex), *std::prev(upper_vertex)};
    }
}

double BaseInterpolator::calculate_delta_angle(double angle_1, double angle_2) const
{
    auto const delta_angle = angle_2 - angle_1;
    return delta_angle > 0.0 ? acos(cos(delta_angle)) : -acos(cos(delta_angle));
}

Vertex BaseInterpolator::vertex_at_position(double position) const
{
    if (position < this->_trajectory.vertices().begin()->position() ||
        std::fabs(position - this->_trajectory.vertices().begin()->position()) < std::numeric_limits<double>::epsilon())
    {
        return *this->_trajectory.vertices().begin();
    }
    else if (
        position > this->_trajectory.vertices().rbegin()->position() ||
        std::fabs(this->_trajectory.vertices().rbegin()->position() - position) <
            std::numeric_limits<double>::epsilon())
    {
        return *this->_trajectory.vertices().rbegin();
    }
    else
    {

        auto const &adjacent_vertices = this->calculate_adjacent_vertices(position);

        auto inclination_interpolated = this->inclination_at_position(position, adjacent_vertices);
        auto azimuth_interpolated = this->azimuth_at_position(position, adjacent_vertices);

        return {position, inclination_interpolated, azimuth_interpolated};
    }
}

double BaseInterpolator::inclination_at_position(double position) const
{
    return this->inclination_at_position(position, this->calculate_adjacent_vertices(position));
}

double BaseInterpolator::azimuth_at_position(double position) const
{
    return this->azimuth_at_position(position, this->calculate_adjacent_vertices(position));
}

void BaseInterpolator::add_n_drop(const Vertex &vertex)
{
    this->_trajectory.add_n_drop(vertex);
}

void BaseInterpolator::drop_n_add(const Vertex &vertex)
{
    this->_trajectory.drop_n_add(vertex);
}

double BaseInterpolator::x_at_position(double position) const
{
    return this->projection_at_position(&BaseInterpolator::calculate_delta_x_projection, position);
}

double BaseInterpolator::y_at_position(double position) const
{
    return this->projection_at_position(&BaseInterpolator::calculate_delta_y_projection, position);
}

double BaseInterpolator::z_at_position(double position) const
{
    return this->projection_at_position(&BaseInterpolator::calculate_delta_z_projection, position);
}

std::vector<Vertex> BaseInterpolator::generate_vertices(std::size_t num_vertices, unsigned num_threads) const
{
    if (num_vertices < _trajectory.vertices().size())
    {
        return _trajectory.vertices_python();
    }

    std::vector<double> positions = this->generate_positions(num_vertices);
    return utils::Multithreading::run<Vertex>(
        positions.begin(), positions.end(), num_threads, [this](double pos) { return this->vertex_at_position(pos); });
}

std::vector<double> BaseInterpolator::generate_positions(std::size_t num_positions) const
{
    double first_trajectory_position = _trajectory.cbegin()->position();
    double last_trajectory_position = std::prev(_trajectory.cend())->position();
    double trajectory_length = last_trajectory_position - first_trajectory_position;
    double increment = trajectory_length / static_cast<double>(num_positions);

    std::vector<double> positions(num_positions);
    positions[0] = first_trajectory_position;
    for (std::size_t i = 1; i < positions.size(); ++i)
    {
        positions[i] = positions[i - 1] + increment;
    }

    double &last_position = positions[positions.size() - 1];
    if (last_position > last_trajectory_position)
    {
        last_position = last_trajectory_position;
    }

    return positions;
}

double BaseInterpolator::projection_at_position(DeltaCalculator delta_calculator, double position) const
{

    auto sum_delta = 0.0;
    for (auto it_v = this->_trajectory.vertices().begin(); it_v != this->_trajectory.vertices().end(); ++it_v)
    {
        if (it_v->position() > position || fabs(position - it_v->position()) < std::numeric_limits<double>::epsilon())
        {
            auto const &adjacent_vertices = AdjacentVertices{
                it_v != this->_trajectory.vertices().begin() ? *std::prev(it_v) : Vertex{0.0, 0.0, 0.0},
                this->vertex_at_position(position)};

            sum_delta += std::invoke(delta_calculator, *this, adjacent_vertices.second.position(), adjacent_vertices);

            break;
        }

        auto const &adjacent_vertices = AdjacentVertices{
            it_v != this->_trajectory.vertices().begin() ? *std::prev(it_v) : Vertex{0.0, 0.0, 0.0}, *it_v};
        sum_delta += std::invoke(delta_calculator, *this, it_v->position(), adjacent_vertices);
    }

    return sum_delta;
}

} // namespace splines
