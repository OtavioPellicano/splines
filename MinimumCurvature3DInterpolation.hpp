#ifndef MINIMUMCURVATURE3DINTERPOLATION_HPP
#define MINIMUMCURVATURE3DINTERPOLATION_HPP

#include "Base3DInterpolation.hpp"

namespace i3d {

class MinimumCurvature3DInterpolation : public Base3DInterpolation
{
public:


    using Base3DInterpolation::Base3DInterpolation;

    double inclination_at_position(double curve_length) const override;
    double azimuth_at_position(double curve_length) const override;

private:

    enum class AngleType{
        inclination,
        azimuth
    };
    double angle_at_position(double curve_length, AngleType angle_type) const;

    double calculate_alpha(const AdjacentVertices& adjacent_vertices) const;

    double calculate_delta_x_projection(double curve_length, const AdjacentVertices& adjacent_vertices) const override;
    double calculate_delta_y_projection(double curve_length, const AdjacentVertices& adjacent_vertices) const override;
    double calculate_delta_z_projection(double curve_length, const AdjacentVertices& adjacent_vertices) const override;

    /**
     * @brief calculate_common_delta_projection
     * computes common parts of delta_any_projection which are: delta_s and factor_f
     * @param curve_length
     * @param adjacent_vertices`
     * @return
     * std::pair<double, double>(delta_s, factor_f)
     */
    std::pair<double, double> calculate_common_delta_projection(double curve_length, const AdjacentVertices& adjacent_vertices) const;
};

}

#endif // MINIMUMCURVATURE3DINTERPOLATION_HPP
