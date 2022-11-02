#include "interpolator/InterpolatorFactory.hpp"

namespace splines
{

std::unique_ptr<BaseInterpolator> InterpolatorFactory::make(
    const Trajectory &trajectory, InterpolationType interpolation_type)
{
    switch (interpolation_type)
    {
    case InterpolationType::linear:
        return std::make_unique<LinearInterpolator>(LinearInterpolator(trajectory));
    case InterpolationType::minimum_curvature:
        return std::make_unique<MinimumCurvatureInterpolator>(MinimumCurvatureInterpolator(trajectory));
    case InterpolationType::cubic:
        return std::make_unique<CubicInterpolator>(CubicInterpolator(trajectory));
    default:
        return nullptr;
    }
}

} // namespace splines
