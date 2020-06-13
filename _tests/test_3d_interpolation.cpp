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

BOOST_AUTO_TEST_CASE( test_add_and_drop )
{

    Vertices trajectory =
    {
        {214.13724, 5.5, 45.0, AngleUnit::deg},
        {598.800936, 29.75, 77.05, AngleUnit::deg},
        {1550.31948, 29.75, 77.05, AngleUnit::deg},
        {3018.032064, 120.0, 285.0, AngleUnit::deg},
    };

    MinimumCurvature3DInterpolation interpolator{trajectory};

    Vertices samples =
    {
        {10.0, 5.5, 45.0, AngleUnit::deg},
        {1600.0, 29.75, 77.05, AngleUnit::deg},
    };

    interpolator.add_n_drop(*samples.begin());
    BOOST_TEST(interpolator.vertices().size() == trajectory.size(), "different size");
    interpolator.add_n_drop(*samples.rbegin());
    BOOST_TEST(interpolator.vertices().size() == trajectory.size(), "different size");
    interpolator.drop_n_add(*samples.begin());
    BOOST_TEST(interpolator.vertices().size() == trajectory.size(), "different size");
    interpolator.drop_n_add(*samples.rbegin());
    BOOST_TEST(interpolator.vertices().size() == trajectory.size(), "different size");

    Vertices expected =
    {
        {214.13724, 5.5, 45.0, AngleUnit::deg},
        {598.800936, 29.75, 77.05, AngleUnit::deg},
        {1550.31948, 29.75, 77.05, AngleUnit::deg},
        {1600.0, 29.75, 77.05, AngleUnit::deg},
    };

    auto const& vertices = interpolator.vertices();

    for(Vertices::const_iterator it_1 = vertices.begin(), it_2 = expected.begin(); it_1 != vertices.end(); ++it_1, ++it_2)
    {
        BOOST_TEST(it_1->approx_equal(*it_2), message_error_vertices_compare(*it_1, *it_2));
    }

}

/**
 * @brief BOOST_AUTO_TEST_CASE
 *
 * The vertices choise were based on paper SPE 84246, pg. 16
 *
 */
BOOST_AUTO_TEST_CASE( test_projection_at_position , * utf::tolerance(1E-6))
{
    Vertices trajectory =
    {
        {214.13724, 0.095993095, 0.785398049999999},
        {598.800936, 0.519235377499999, 1.3447759945},
        {1550.31948, 0.519235377499999, 1.3447759945},
        {3018.032064, 2.09439479999999, 4.97418765}
    };

    MinimumCurvature3DInterpolation interpolator{trajectory};

    // the std::array<double, 3> represents the x, y, z expected values
    std::map<std::array<double, 3>, Vertex> samples_expected =
    {
        {{7.2619580385825513, 7.2619580385825513, 213.808524}, {214.13724, 5.5, 45.0, AngleUnit::deg}},
        {{42.252476733170006, 115.05692333344899, 578.1493471}, {598.800936, 29.75, 77.05, AngleUnit::deg}},
        {{148.06374664425934 , 575.20715056762072, 1404.256654}, {1550.31948, 29.75, 77.05, AngleUnit::deg}},
        {{757.17661872735744, -65.828691703728509, 2073.043306}, {3018.032064, 120.0, 285.0, AngleUnit::deg}},

        //interpolated vertices:
        {{119.71605539435078, 451.92919935155186, 1182.935855}, {1295.4, 29.75, 77.05, AngleUnit::deg}},
        {{594.72150050487528, 310.07764311833108, 2149.247025}, {2592.052728, 80.89, 300.71, AngleUnit::deg}},
        {{642.38182819109716, 224.1, 2157.081421}, {2690.786592, 90.0, 297.31, AngleUnit::deg}},
        {{684.89863558697255, 135.46634412415341, 2149.244745}, {2789.520456, 99.11, 293.92, AngleUnit::deg}},

    };

    for(auto& item: samples_expected)
    {
        auto x = interpolator.x_at_position(item.second.curve_length());
        auto y = interpolator.y_at_position(item.second.curve_length());
        auto z = interpolator.z_at_position(item.second.curve_length());

        BOOST_TEST(x == item.first[0]);
        BOOST_TEST(y == item.first[1]);
        BOOST_TEST(z == item.first[2]);

    }


}
