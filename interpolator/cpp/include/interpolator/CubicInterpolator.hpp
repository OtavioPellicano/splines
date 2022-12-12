#ifndef CUBICINTERPOLATOR_HPP
#define CUBICINTERPOLATOR_HPP

#include "BaseInterpolator.hpp"

namespace splines
{

/**
 * @brief The CubicInterpolator class
 * The following class represents the Cubic Interpolation based on IADC/SPE 112623 paper
 */
class CubicInterpolator : public BaseInterpolator
{
  public:
    CubicInterpolator(const Vertices &trajectory);
    CubicInterpolator(CubicInterpolator &&other);
    CubicInterpolator &operator=(CubicInterpolator &&rhs);

  private:
    double inclination_at_position(double position, const AdjacentVertices &adjacent_vertices) const final;
    double azimuth_at_position(double position, const AdjacentVertices &adjacent_vertices) const final;
    double angle_at_position(
        double position, const AdjacentVertices &adjacent_vertices, AngleType angle_type) const final;
    double calculate_delta_x_projection(double position, const AdjacentVertices &adjacent_vertices) const final;
    double calculate_delta_y_projection(double position, const AdjacentVertices &adjacent_vertices) const final;
    double calculate_delta_z_projection(double position, const AdjacentVertices &adjacent_vertices) const final;

    double calculate_ep(double position, const AdjacentVertices &adjacent_vertices) const;
    double calculate_f1(double position, const AdjacentVertices &adjacent_vertices) const;
    double calculate_f2(double position, const AdjacentVertices &adjacent_vertices) const;
    double calculate_f3(double position, const AdjacentVertices &adjacent_vertices) const;
    double calculate_f4(double position, const AdjacentVertices &adjacent_vertices) const;

    enum class ProjectionType
    {
        x,
        y,
        z
    };
    double calculate_delta_projection(
        double position, const AdjacentVertices &adjacent_vertices, ProjectionType projection_type) const;
};

} // namespace splines
#endif // CUBICINTERPOLATOR_HPP
