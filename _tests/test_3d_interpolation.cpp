#define BOOST_TEST_MODULE test_3d_interpolation
#include <boost/test/included/unit_test.hpp>
namespace utf = boost::unit_test;

#include <map>
#include <MinimumCurvature3DInterpolation.hpp>
using namespace i3d;

std::string message_error_vertices_compare(const Vertex& v_1, const Vertex& v_2)
{
    using namespace std;

    return "{" + to_string(v_1.curve_length()) + ", " + to_string(v_1.inclination()) + ", " + to_string(v_1.azimuth()) + "} != "
           "{" + to_string(v_2.curve_length()) + ", " + to_string(v_2.inclination()) + ", " + to_string(v_2.azimuth()) + "}";
}

BOOST_AUTO_TEST_CASE( test_calculate_adjacent_vertices )
{

    Vertices trajectory_points =
    {
        {2.0, 0.0, 0.0},
        {0.0, 0.0, 0.0},
        {1.0, 0.0, 0.0},
    };

    MinimumCurvature3DInterpolation interpolator{trajectory_points};

    std::map<double, AdjacentVertices> expected =
    {
        {0.0, { {0.0, 0.0, 0.0}, {1.0, 0.0, 0.0} } },
        {0.5, { {0.0, 0.0, 0.0}, {1.0, 0.0, 0.0} } },
        {1.0, { {1.0, 0.0, 0.0}, {2.0, 0.0, 0.0} } },
        {1.5, { {1.0, 0.0, 0.0}, {2.0, 0.0, 0.0} } },
        {2.0, { {2.0, 0.0, 0.0}, {2.0, 0.0, 0.0} } },
    };

    for(auto&& item : expected)
    {
        auto adjacent_vertices = interpolator.calculate_adjacent_vertices(item.first);
        BOOST_TEST(adjacent_vertices.first.approx_equal(item.second.first), message_error_vertices_compare(adjacent_vertices.first, item.second.first));
        BOOST_TEST(adjacent_vertices.second.approx_equal(item.second.second), message_error_vertices_compare(adjacent_vertices.second, item.second.second));
    }

}

BOOST_AUTO_TEST_CASE( test_angle_conversion , * utf::tolerance(1E-6))
{

    Vertices vertices =
    {
        {0.5, M_PI / 2, M_PI, AngleUnit::rad},
        {1.0, 45.0, 30.0, AngleUnit::deg}
    };

    BOOST_TEST(vertices[0].inclination() == M_PI / 2);
    BOOST_TEST(vertices[0].azimuth() == M_PI);

    BOOST_TEST(vertices[0].inclination(AngleUnit::deg) == 90.0);
    BOOST_TEST(vertices[0].azimuth(AngleUnit::deg) == 180.0);

    BOOST_TEST(vertices[1].inclination() == M_PI / 4);
    BOOST_TEST(vertices[1].azimuth() == M_PI / 6);

}

BOOST_AUTO_TEST_CASE( test_vertex_at_position_minimum_curvature_interpolation)
{

    Vertices trajectory =
    {
        {214.13724, 5.5, 45.0, AngleUnit::deg},
        {598.800936, 29.75, 77.05, AngleUnit::deg},
        {1550.31948, 29.75, 77.05, AngleUnit::deg},
        {3018.032064, 120.0, 285.0, AngleUnit::deg},
    };

    MinimumCurvature3DInterpolation interpolator{trajectory};

    std::map<double, Vertex> samples_expected =
    {
        {214.13724, {214.13724, 5.5, 45.0, AngleUnit::deg}},
        {598.800936, {598.800936, 29.75, 77.05, AngleUnit::deg}},
        {1550.31948, {1550.31948, 29.75, 77.05, AngleUnit::deg}},
        {3018.032064, {3018.032064, 120.0, 285.0, AngleUnit::deg}},
    };

    for(auto&& item : samples_expected)
    {
        auto vt_1 = interpolator.vertex_at_position(item.first);
        auto vt_2 = item.second;

        if(!vt_1.approx_equal(vt_2))
        {
            std::cout << message_error_vertices_compare(vt_1, vt_2) << std::endl;
        }
    }

}
