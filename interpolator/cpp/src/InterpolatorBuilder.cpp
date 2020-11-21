#include "interpolator/InterpolatorBuilder.hpp"

namespace i3d {

std::unique_ptr<BaseInterpolator> build_interpolator(const Vertices &vertices, InterpolationType interpolation_type)
{
    std::unique_ptr<BaseInterpolator> interp_ptr = nullptr;
    switch (interpolation_type) {
    case InterpolationType::linear:
        interp_ptr = std::make_unique<LinearInterpolator> (LinearInterpolator(vertices));
        break;
    case InterpolationType::minimum_curvature:
        interp_ptr = std::make_unique<MinimumCurvatureInterpolator> (MinimumCurvatureInterpolator(vertices));
        break;
    case InterpolationType::cubic:
        interp_ptr = std::make_unique<CubicInterpolator> (CubicInterpolator(vertices));
        break;
    default:
        break;
    }

    return interp_ptr;
}

}
