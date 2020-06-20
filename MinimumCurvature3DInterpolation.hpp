#ifndef MINIMUMCURVATURE3DINTERPOLATION_HPP
#define MINIMUMCURVATURE3DINTERPOLATION_HPP

#include "Base3DInterpolation.hpp"

namespace i3d {

class MinimumCurvature3DInterpolation : public Base3DInterpolation
{
public:

    using Base3DInterpolation::Base3DInterpolation;

private:
    double inclination_at_position(double position, const AdjacentVertices& adjacent_vertices) const final;
    double azimuth_at_position(double position,  const AdjacentVertices& adjacent_vertices) const final;
    double angle_at_position(double position, const AdjacentVertices& adjacent_vertices, AngleType angle_type) const final;
    double calculate_delta_x_projection(double position, const AdjacentVertices& adjacent_vertices) const final;
    double calculate_delta_y_projection(double position, const AdjacentVertices& adjacent_vertices) const final;
    double calculate_delta_z_projection(double position, const AdjacentVertices& adjacent_vertices) const final;
    InterpolationType interpolation_type() const final;

    double calculate_alpha(const AdjacentVertices& adjacent_vertices) const;

    /**
     * @brief calculate_common_delta_projection
     * computes common parts of delta_any_projection which are: delta_s and factor_f
     * @param position
     * @param adjacent_vertices`
     * @return
     * std::pair<double, double>(delta_s, factor_f)
     */
    std::pair<double, double> calculate_common_delta_projection(double position, const AdjacentVertices& adjacent_vertices) const;

private:
    const InterpolationType m_interpolation_type = InterpolationType::minimum_curvature;
};

}

#endif // MINIMUMCURVATURE3DINTERPOLATION_HPP
