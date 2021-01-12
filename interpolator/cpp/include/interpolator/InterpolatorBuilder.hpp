#ifndef TRAJECTORYBUILDER_HPP
#define TRAJECTORYBUILDER_HPP

#include <memory>

#include "CubicInterpolator.hpp"
#include "LinearInterpolator.hpp"
#include "MinimumCurvatureInterpolator.hpp"

namespace i3d
{

/**
 * @brief build_interpolator
 * This function represents the builder for interpolation type
 *
 * @param vertices
 * All trajectory vertices available to build the curve
 *
 * @param interpolation_type
 * The type of interpolation
 *
 * @return
 * A smart_ptr with the interpolation object
 */
std::unique_ptr<BaseInterpolator> build_interpolator(const Vertices &vertices, InterpolationType interpolation_type);

} // namespace i3d

#endif // TRAJECTORYBUILDER_HPP
