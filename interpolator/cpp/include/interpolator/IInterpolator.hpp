#ifndef I3DINTERPOLATION_H
#define I3DINTERPOLATION_H

#include "Vertices.hpp"

namespace splines
{

/**
 * @brief The InterpolationType enum
 * This enum represents all kind of interpolation available
 */
enum class InterpolationType
{
    linear,
    minimum_curvature,
    cubic,
};

/**
 * @brief The IInterpolator class
 * This class represents the Interpolation Interface.
 * In this class there are methods which will be exposed to the user.
 */
class IInterpolator
{
  public:
    virtual ~IInterpolator() = default;

    virtual const Vertices &trajectory() const = 0;
    virtual void set_trajectory(const Vertices &trajectory) = 0;

    /**
     * @brief vertex_at_position
     * The Vertex at position with a specific interpolation.
     *
     * @param position
     * The position represents the curve length with the first vertex as reference
     *
     * @return
     * The Vertex interpolated. @see Vertex
     */
    virtual Vertex vertex_at_position(double position) const = 0;

    /**
     * @brief inclination_at_position
     * The inclination at position with a specific interpolation.
     *
     * @param position
     * The position represents the curve length with the first vertex as reference
     *
     * @return
     * the inclination interpolated
     */
    virtual double inclination_at_position(double position) const = 0;

    /**
     * @brief azimuth_at_position
     * The azimuth at position with a specific interpolation.
     *
     * @param position
     * The position represents the curve length with the first vertex as reference
     *
     * @return
     * The azimuth interpolated
     */
    virtual double azimuth_at_position(double position) const = 0;

    /**
     * @brief x_at_position
     *
     * @param position
     * The position represents the curve length with the first vertex as reference
     *
     * @return
     * The projection in X axis
     */
    virtual double x_at_position(double position) const = 0;

    /**
     * @brief y_at_position
     *
     * @param position
     * The position represents the curve length with the first vertex as reference
     *
     * @return
     * The projection in Y axis
     */
    virtual double y_at_position(double position) const = 0;

    /**
     * @brief z_at_position
     *
     * @param position
     * The position represents the curve length with the first vertex as reference
     *
     * @return
     * The projection in Z axis
     */
    virtual double z_at_position(double position) const = 0;

    /**
     * @brief add_n_drop
     * This method add a vertex into trajectory range and remove the last vertex, leaving range constant
     *
     * @param vertex
     * @see Vertex
     */
    virtual void add_n_drop(const Vertex &vertex) = 0;

    /**
     * @brief drop_n_add
     * This method remove the first vertex and add a vertex into trajectory, leaving range constant
     *
     * @param vertex
     * @see Vertex
     */
    virtual void drop_n_add(const Vertex &vertex) = 0;

    /**
     * @brief interpolation_type
     * This method retuns the interpolation type
     *
     * @return
     * @see InterpolationType
     */
    virtual InterpolationType interpolation_type() const = 0;

    /**
     * @brief interpolation_type
     * This method retuns the interpolation type as std::string
     *
     * @return
     * @see InterpolationType
     */
    virtual std::string interpolation_type_str() const = 0;
};

} // namespace splines

#endif // I3DINTERPOLATION_H
