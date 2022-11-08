#ifndef TRAJECTORYBUILDER_HPP
#define TRAJECTORYBUILDER_HPP

#include <memory>

#include "CubicInterpolator.hpp"
#include "LinearInterpolator.hpp"
#include "MinimumCurvatureInterpolator.hpp"

namespace splines
{

class InterpolatorFactory
{
  public:
    /**
     * @brief
     * This static method represents the interpolator factory
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
    static std::unique_ptr<BaseInterpolator> make(const Vertices &trajectory, InterpolationType interpolation_type);
};

} // namespace splines

#endif // TRAJECTORYBUILDER_HPP
