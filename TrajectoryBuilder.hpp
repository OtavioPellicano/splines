#ifndef TRAJECTORYBUILDER_HPP
#define TRAJECTORYBUILDER_HPP

#include <memory>

#include "MinimumCurvature3DInterpolation.hpp"
#include "Linear3DInterpolation.hpp"

namespace i3d {

/**
 * @brief build_trajectory
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
std::unique_ptr<Base3DInterpolation> build_trajectory(const Vertices& vertices, InterpolationType interpolation_type);

}


#endif // TRAJECTORYBUILDER_HPP
