#include "TrajectoryBuilder.hpp"

namespace i3d {

std::unique_ptr<Base3DInterpolation> build_trajectory(const Vertices &vertices, InterpolationType interpolation_type)
{
    std::unique_ptr<Base3DInterpolation> interp_ptr = nullptr;
    switch (interpolation_type) {
    case InterpolationType::linear:
        interp_ptr = std::make_unique<Linear3DInterpolation> (Linear3DInterpolation(vertices));
        break;
    case InterpolationType::minimum_curvature:
        interp_ptr = std::make_unique<MinimumCurvature3DInterpolation> (MinimumCurvature3DInterpolation(vertices));
        break;
    default:
        break;
    }

    return interp_ptr;
}

}
