#ifndef TRAJECTORYBUILDER_HPP
#define TRAJECTORYBUILDER_HPP

#include <memory>

#include "CubicInterpolator.hpp"
#include "LinearInterpolator.hpp"
#include "MinimumCurvatureInterpolator.hpp"

namespace splines
{

/**
 * @brief build_interpolator
 * This function represents the builder for interpolation type
 *
 * @param trajectory
 * All trajectory vertices available to build the curve
 *
 * @param interpolation_type
 * The type of interpolation
 *
 * @return
 * A smart_ptr with the interpolation object
 */
std::unique_ptr<BaseInterpolator> build_interpolator(
    const Trajectory &trajectory, InterpolationType interpolation_type);

} // namespace splines

#endif // TRAJECTORYBUILDER_HPP
