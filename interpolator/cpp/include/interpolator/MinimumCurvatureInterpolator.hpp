#ifndef MINIMUMCURVATURE3DINTERPOLATION_HPP
#define MINIMUMCURVATURE3DINTERPOLATION_HPP

#include "BaseInterpolator.hpp"

namespace splines
{

/**
 * @brief The MinimumCurvatureInterpolator class
 * The following class represents the Minimum Curvature Interpolation based on SPE 84246 paper
 */
class MinimumCurvatureInterpolator : public BaseInterpolator
{
  public:
    MinimumCurvatureInterpolator(const Vertices &trajectory);

  private:
    double inclination_at_position(double position, const AdjacentVertices &adjacent_vertices) const final;
    double azimuth_at_position(double position, const AdjacentVertices &adjacent_vertices) const final;
    double angle_at_position(
        double position, const AdjacentVertices &adjacent_vertices, AngleType angle_type) const final;
    double calculate_delta_x_projection(double position, const AdjacentVertices &adjacent_vertices) const final;
    double calculate_delta_y_projection(double position, const AdjacentVertices &adjacent_vertices) const final;
    double calculate_delta_z_projection(double position, const AdjacentVertices &adjacent_vertices) const final;

    /**
     * @brief calculate_alpha
     * This method computes the alpha angle which is the angle between two Normal vectors (Frenet basis).
     * In Oil & Gas nomenclature, that angle correpond to dogleg
     *
     * @param adjacent_vertices
     * The adjacent vertices
     *
     * @return
     * the angle between two adjacent Normal vectors (Frenet basis)
     */
    double calculate_alpha(const AdjacentVertices &adjacent_vertices) const;

    /**
     * @brief calculate_common_delta_projection
     * computes common parts of delta_any_projection which are: delta_s and factor_f
     *
     * @param position
     *
     * @param adjacent_vertices
     *
     * @return
     * std::pair<double, double>(delta_s, factor_f)
     */
    std::pair<double, double> calculate_common_delta_projection(
        double position, const AdjacentVertices &adjacent_vertices) const;
};

} // namespace splines

#endif // MINIMUMCURVATURE3DINTERPOLATION_HPP
