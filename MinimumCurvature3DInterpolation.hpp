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


};

}

#endif // MINIMUMCURVATURE3DINTERPOLATION_HPP
