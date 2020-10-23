#ifndef BASE3DINTERPOLATION_HPP
#define BASE3DINTERPOLATION_HPP

#include "IInterpolator.hpp"
#include <algorithm>

namespace i3d {

/**
 * @brief The BaseInterpolator class
 * The following class computes the common parts which will be used by specific interpolation types
 */
class BaseInterpolator : public IInterpolator
{
public:
    using IInterpolator::inclination_at_position;
    using IInterpolator::azimuth_at_position;

    virtual ~BaseInterpolator() = default;

    BaseInterpolator(const Vertices& vertices);

    const Vertices & vertices() const;
    void set_vertices(const Vertices &vertices);

    Vertex vertex_at_position(double position) const final;

    double inclination_at_position(double position) const final;
    double azimuth_at_position(double position) const final;

    void add_n_drop(const Vertex& vertex) final;
    void drop_n_add(const Vertex& vertex) final;

    double x_at_position(double position) const final;
    double y_at_position(double position) const final;
    double z_at_position(double position) const final;

private:

    typedef double (BaseInterpolator::*DeltaCalculator )(double, const AdjacentVertices&) const;

    /**
     * @brief projection_at_position
     * The main accumulator for any projection variation types (calls here DeltaCalculator), e.g. calculate_delta_x_projection
     *
     * @param delta_calculator
     * A Function Pointer for DeltaCalculator method
     *
     * @param position
     * The position represents the curve length with the first vertex as reference
     *
     * @return
     * the projection given a DeltaCalculator and position
     */
    double projection_at_position(DeltaCalculator delta_calculator, double position) const;

    /**
     * @brief calculate_adjacent_vertices
     * This method returns the vertices between the position
     *
     * @param position
     * The position represents the curve length with the first vertex as reference
     *
     * @return
     * The adjacent vertices
     */
    AdjacentVertices calculate_adjacent_vertices(double position) const;

protected:

    /**
     * @brief calculate_delta_x_projection
     * Computes the projection X variation given a position and the adjacent vertices
     *
     * @param position
     * The position represents the curve length with the first vertex as reference
     *
     * @param adjacent_vertices
     * The adjacent vertices
     *
     * @return
     * The projection X variation
     */
    virtual double calculate_delta_x_projection(double position, const AdjacentVertices& adjacent_vertices) const = 0;

    /**
     * @brief calculate_delta_y_projection
     * Computes the projection X variation given a position and the adjacent vertices
     *
     * @param position
     * The position represents the curve length with the first vertex as reference
     *
     * @param adjacent_vertices
     * The adjacent vertices
     *
     * @return
     * The projection Y variation
     */
    virtual double calculate_delta_y_projection(double position, const AdjacentVertices& adjacent_vertices) const = 0;

    /**
     * @brief calculate_delta_z_projection
     * Computes the projection Z variation given a position and the adjacent vertices
     *
     * @param position
     * The position represents the curve length with the first vertex as reference
     *
     * @param adjacent_vertices
     * The adjacent vertices
     *
     * @return
     * The projection Z variation
     */
    virtual double calculate_delta_z_projection(double position, const AdjacentVertices& adjacent_vertices) const = 0;

    /**
     * @brief inclination_at_position
     * The same of @see I3DInterpolation::inclination_at_position, but with adjacent vertices as argument for performance propouse
     *
     * @param position
     * The position represents the curve length with the first vertex as reference
     *
     * @param adjacent_vertices
     * The adjacent vertices
     *
     * @return
     * inclination interpolated
     */
    virtual double inclination_at_position(double position, const AdjacentVertices& adjacent_vertices) const = 0;

    /**
     * @brief azimuth_at_position
     * The same of @see I3DInterpolation::azimuth_at_position, but with adjacent vertices as argument for performance propouse
     *
     * @param position
     * The position represents the curve length with the first vertex as reference
     *
     * @param adjacent_vertices
     * The adjacent vertices
     *
     * @return
     * azimuth interpolated
     */
    virtual double azimuth_at_position(double position,  const AdjacentVertices& adjacent_vertices) const = 0;


    enum class AngleType
    {
        inclination,
        azimuth
    };
    /**
     * @brief angle_at_position
     * There is some parts in common when you computes inclination and azimuth interpolation (basically boundary conditions)
     * This method agroup them
     *
     * @param position
     * The position represents the curve length with the first vertex as reference
     *
     * @param adjacent_vertices
     * The adjacent vertices
     *
     * @param angle_type
     * The angle type: inclination or azimuth
     *
     * @return
     * the angle interpolated
     */
    virtual double angle_at_position(double position, const AdjacentVertices &adjacent_vertices, AngleType angle_type) const = 0;

protected:

    /**
     * @brief calculate_delta_angle
     * This method computes the smallest path between two angles, considering the sign
     *
     * @param angle_1
     * @param angle_2
     * @return
     */
    double calculate_delta_angle(double angle_1, double angle_2) const;

private:
    Vertices m_vertices;

};

}
#endif // BASE3DINTERPOLATION_HPP
