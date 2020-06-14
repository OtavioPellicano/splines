#ifndef LINEAR3DINTERPOLATION_HPP
#define LINEAR3DINTERPOLATION_HPP

#include "Base3DInterpolation.hpp"

namespace i3d {

class Linear3DInterpolation: public Base3DInterpolation
{
public:
    using Base3DInterpolation::Base3DInterpolation;
private:

    double inclination_at_position(double curve_length, const AdjacentVertices& adjacent_vertices) const override;
    double azimuth_at_position(double curve_length,  const AdjacentVertices& adjacent_vertices) const override;


private:

    double calculate_linear_spline(double x_1, double y_1, double x_2, double y_2, double x_star) const;
};

}
#endif // LINEAR3DINTERPOLATION_HPP
