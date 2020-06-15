#ifndef TRAJECTORYBUILDER_HPP
#define TRAJECTORYBUILDER_HPP

#include <memory>

#include "MinimumCurvature3DInterpolation.hpp"
#include "Linear3DInterpolation.hpp"

namespace i3d {

std::unique_ptr<Base3DInterpolation> build_trajectory(const Vertices& vertices, InterpolationType interpolation_type);

}


#endif // TRAJECTORYBUILDER_HPP
