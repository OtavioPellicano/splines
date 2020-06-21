#ifndef LINEAR3DINTERPOLATION_HPP
#define LINEAR3DINTERPOLATION_HPP

#include "Base3DInterpolation.hpp"

namespace i3d {

/**
 * @brief The Linear3DInterpolation class
 * The following class represents the linear spline interpolation
 */
class Linear3DInterpolation : public Base3DInterpolation
{
public:
    using Base3DInterpolation::Base3DInterpolation;

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
     * @param x_1
     * @param y_1
     * @param x_2
     * @param y_2
     * @param x_star
     * @return
     * The linear spline interpolation
     *
     */
    double calculate_linear_spline(double x_1, double y_1, double x_2, double y_2, double x_star) const;

private:
    const InterpolationType m_interpolation_type = InterpolationType::linear;
};

}
#endif // LINEAR3DINTERPOLATION_HPP
