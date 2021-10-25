#include "interpolator/InterpolatorBuilder.hpp"

namespace splines
{

std::unique_ptr<BaseInterpolator> build_interpolator(const Trajectory &trajectory, InterpolationType interpolation_type)
{
    auto interp_ptr = std::unique_ptr<BaseInterpolator>(nullptr);
    switch (interpolation_type)
    {
    case InterpolationType::linear:
        interp_ptr = std::make_unique<LinearInterpolator>(LinearInterpolator(trajectory));
        break;
    case InterpolationType::minimum_curvature:
        interp_ptr = std::make_unique<MinimumCurvatureInterpolator>(MinimumCurvatureInterpolator(trajectory));
        break;
    case InterpolationType::cubic:
        interp_ptr = std::make_unique<CubicInterpolator>(CubicInterpolator(trajectory));
        break;
    default:
        break;
    }

    return interp_ptr;
}

} // namespace splines
