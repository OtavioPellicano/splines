Splines
========

.. image:: https://travis-ci.com/OtavioPellicano/splines.svg?branch=master
    :alt: Travis Status
    :scale: 100%
    :target: https://travis-ci.com/github/OtavioPellicano/splines

Splines is a library for 3D curves interpolation in spherical coordinates. This kind of interpolation is massively used in Oil & Gas field in
applications such as following the string position or in efforts calculation. The library is fully implemented in C++, but there is a Python 3
interface for a more convenient and simpler use.

Interpolations Currently Supported
=================================

- Linear
- Cubic
- Minimum Curvature

Example in C++
==============

.. code-block:: cpp

    #include <iostream>
    
    #include <interpolator/InterpolatorBuilder.hpp>

    int main()
    {
    
        // Vertices is a typedef for std::set<Vertex>
        // Vertex is basically a struct where you store position, inclination and azimuth as follow: 
        // Vertex{position, inclination, azimuth}
        // inclination: angle from z to y axis
        // azimuth: angle from x to y axis
        const Vertices vertices = {
                {214.13724, 0.095993095, 0.785398049999999}, // default angle unit: radian
                {598.800936, 0.519235377499999, 1.3447759945},
                {1550.31948, 0.519235377499999, 1.3447759945},
                {3018.032064, 2.09439479999999, 4.97418765}};

        auto linear_interpolator = build_interpolator(vertices, InterpolationType::linear);
        
        auto const position_desired = 2690.786592;
        
        auto vertex = linear_interpolator->vertex_at_position(position_desired);
        
        // print expected: {2690.786592, 1.743193, 5.565881}
        std::cout << "{" 
                  << vertex.position() << ", "
                  << vertex.inclination() << ", "
                  << vertex.azimuth() << "}" << std::endl;
                  
        // get projections (cartesian coordinates)
        auto x = linear_interpolator->x_at_position(position_desired);
        auto y = linear_interpolator->y_at_position(position_desired);
        auto z = linear_interpolator->z_at_position(position_desired);

        // print expected: {1009.7933937687196, -77.894972532420184, 1177.583381}
        std::cout << "{" 
                  << x << ", "
                  << y << ", "
                  << z << "}" << std::endl;

        return 0;
    }
    
Example in Python 3
==============

.. code-block:: python

    from _interpolator import (AngleUnit, BuildInterpolator, InterpolationType, Vertex)

    def main:
    
        # Vertices is a typedef for std::set<Vertex>
        # Vertex is basically a struct where you store position, inclination and azimuth as follow: 
        # Vertex{position, inclination, azimuth}
        # inclination: angle from z to y axis
        # azimuth: angle from x to y axis
        vertices = set([Vertex(214.13724, 0.095993095, 0.785398049999999), # default angle unti: radian
                        Vertex(598.800936, 0.519235377499999, 1.3447759945),
                        Vertex(1550.31948, 0.519235377499999, 1.3447759945),
                        Vertex(3018.032064, 2.09439479999999, 4.97418765)])

        linear_interpolator = BuildInterpolator(vertices, InterpolationType.Linear)
        
        position_desired = 2690.786592;
        
        vertex = linear_interpolator.VertexAtPosition(position_desired);
        
        # print expected: {2690.786592, 1.743193, 5.565881}
        print(f"\{ {vertex.Position()} , {vertex.Inclination()}, {vertex.Azimuth()} \}")
                  
        # get projections (cartesian coordinates)
        x = linear_interpolator.XAtPosition(position_desired);
        y = linear_interpolator.YAtPosition(position_desired);
        z = linear_interpolator.ZAtPosition(position_desired);

        # print expected: {1009.7933937687196, -77.894972532420184, 1177.583381}
        print(f"\{ {x} , {y}, {z} \}")

