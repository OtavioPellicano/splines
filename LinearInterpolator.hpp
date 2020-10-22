#ifndef LINEAR3DINTERPOLATION_HPP
#define LINEAR3DINTERPOLATION_HPP

#include "BaseInterpolator.hpp"

namespace i3d {

/**
 * @brief The LinearInterpolator class
 * The following class represents the linear spline interpolation
 */
class LinearInterpolator : public BaseInterpolator
{
public:
    using BaseInterpolator::BaseInterpolator;

private:
    double inclination_at_position(double position, const AdjacentVertices &adjacent_vertices) const final;
    double azimuth_at_position(double position, const AdjacentVertices &adjacent_vertices) const final;
    double calculate_delta_x_projection(double position, const AdjacentVertices &adjacent_vertices) const final;
    double calculate_delta_y_projection(double position, const AdjacentVertices &adjacent_vertices) const final;
    double calculate_delta_z_projection(double position, const AdjacentVertices &adjacent_vertices) const final;
    double angle_at_position(double position, const AdjacentVertices &adjacent_vertices, AngleType angle_type) const final;
    InterpolationType interpolation_type() const final;

    /**
     * @brief calculate_linear_spline
     * Computes a linear interpolation following the linear spline equation:
     *
     * y_star = (y_1 * (x_2 - x_star) + y_2 * (x_star - x_1)) / (x_2 - x_1)
     *
     * where (x_1, y_1) and (x_2, y_2) are the adjacent points and (x_start, y_star) is the desired interpolated point
     *
     * @param position_1 (x_1)
     * @param angle_1 (y_1)
     * @param position_2 (x_2)
     * @param angle_2 (y_2)
     * @param position (x_star)
     * @return
     * The linear spline interpolation
     *
     */
    double calculate_linear_spline(double position_1, double angle_1, double position_2, double angle_2, double position) const;


private:
    const InterpolationType m_interpolation_type = InterpolationType::linear;
};

}
#endif // LINEAR3DINTERPOLATION_HPP
