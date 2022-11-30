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
     * The template argument Interpolator should be one of the interpolator
     * options: LinearInterpolator, CubicInterpolator,...
     *
     * @param trajectory
     * All trajectory vertices available to build the curve
     *
     * @return
     * A smart_ptr with the interpolator object
     */
    template <typename Interpolator> static std::unique_ptr<Interpolator> make(const Vertices &trajectory)
    {
        return std::make_unique<Interpolator>(Interpolator(trajectory));
    }
};

} // namespace splines

#endif // TRAJECTORYBUILDER_HPP
