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

    BOOST_TEST(vertices.begin()->inclination() == M_PI / 2);
    BOOST_TEST(vertices.begin()->azimuth() == M_PI);

    BOOST_TEST(vertices.begin()->inclination(AngleUnit::deg) == 90.0);
    BOOST_TEST(vertices.begin()->azimuth(AngleUnit::deg) == 180.0);

    BOOST_TEST(vertices.rbegin()->inclination() == M_PI / 4);
    BOOST_TEST(vertices.rbegin()->azimuth() == M_PI / 6);

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
        {1295.4, {1295.4, 29.75, 77.05, AngleUnit::deg}},
        {1550.31948, {1550.31948, 29.75, 77.05, AngleUnit::deg}},
        {2592.052728, {2592.052728, 80.89, 300.71, AngleUnit::deg}},
        {2690.786592, {2690.786592, 90.0, 297.31, AngleUnit::deg}},
        {2789.520456, {2789.520456, 99.11, 293.92, AngleUnit::deg}},
        {3018.032064, {3018.032064, 120.0, 285.0, AngleUnit::deg}},
    };

    for(auto&& item : samples_expected)
    {
        auto const& v_1 = interpolator.vertex_at_position(item.first);
        auto const& v_2 = item.second;
        BOOST_TEST(v_1.approx_equal(v_2, .2), message_error_vertices_compare(v_1, v_2));
    }

}
