#define BOOST_TEST_MODULE test_3d_interpolation
#include <boost/test/included/unit_test.hpp>
namespace utf = boost::unit_test;

#include <map>

#include "InterpolatorBuilder.hpp"

using namespace i3d;

std::string message_error_vertices_compare(const Vertex& v_1, const Vertex& v_2)
{
    using namespace std;

    return "{" + to_string(v_1.position()) + ", " + to_string(v_1.inclination()) + ", " + to_string(v_1.azimuth()) + "} != "
           "{" + to_string(v_2.position()) + ", " + to_string(v_2.inclination()) + ", " + to_string(v_2.azimuth()) + "}";
}

namespace Trajectory {

//The vertices choise were based on SPE 84246 paper, pg. 16
const Vertices SPE84246 =
    {
        {214.13724, 0.095993095, 0.785398049999999},
        {598.800936, 0.519235377499999, 1.3447759945},
        {1550.31948, 0.519235377499999, 1.3447759945},
        {3018.032064, 2.09439479999999, 4.97418765}
    };

};

BOOST_AUTO_TEST_CASE( test_angle_conversion , * utf::tolerance(1E-6) )
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

BOOST_AUTO_TEST_CASE( test_vertex_at_position_minimum_curvature_interpolation )
{

    Vertices trajectory = Trajectory::SPE84246;

    MinimumCurvatureInterpolator interpolator{trajectory};

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

BOOST_AUTO_TEST_CASE( test_vertex_at_position_linear_interpolation )
{

    Vertices trajectory = Trajectory::SPE84246;

    LinearInterpolator interpolator{trajectory};

    std::map<double, Vertex> samples_expected =
    {
        {214.13724, {214.13724, 0.095993095, 0.785398049999999}},
        {598.800936, {598.800936, 0.519235377499999, 1.3447759945}},
        {1550.31948, {1550.31948, 0.519235377499999, 1.3447759945}},
        {3018.032064, {3018.032064, 2.09439479999999, 4.97418765}},

        {1295.4, {1295.4, 0.519235, 1.344776}},
        {2592.052728, {2592.052728, 1.637231, 5.744402}},
        {2690.786592, {2690.786592, 1.743193, 5.565881}},
        {2789.520456, {2789.520456, 1.849155, 5.387360}},
    };


    for(auto&& item : samples_expected)
    {
        auto const& v_1 = interpolator.vertex_at_position(item.first);
        auto const& v_2 = item.second;
        BOOST_TEST(v_1.approx_equal(v_2, 1E-2), message_error_vertices_compare(v_1, v_2));
    }

}

BOOST_AUTO_TEST_CASE( test_vertex_at_position_cubic_interpolation )
{

        Vertices trajectory = Trajectory::SPE84246;

        CubicInterpolator interpolator{trajectory};

        std::map<double, Vertex> samples_expected =
            {
                {214.13724, {214.13724, 0.095993095, 0.785398049999999}},
                {598.800936, {598.800936, 0.519235377499999, 1.3447759945}},
                {1550.31948, {1550.31948, 0.519235377499999, 1.3447759945}},
                {3018.032064, {3018.032064, 2.09439479999999, 4.97418765}},

                {1295.4, {1295.4, 0.519235, 1.344776}},
                {2592.052728, {2592.052728, 1.639278, 1.318284}},
                {2690.786592, {2690.786592, 1.744500, 1.315131}},
                {2789.520456, {2789.520456, 1.849832, 1.312366}},
            };


    for(auto&& item : samples_expected)
    {
        auto const& v_1 = interpolator.vertex_at_position(item.first);
        auto const& v_2 = item.second;
        BOOST_TEST(v_1.approx_equal(v_2, 1E-2), message_error_vertices_compare(v_1, v_2));
    }

}

BOOST_AUTO_TEST_CASE( test_add_and_drop )
{

    Vertices trajectory = Trajectory::SPE84246;

    MinimumCurvatureInterpolator interpolator{trajectory};

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
        BOOST_TEST(it_1->approx_equal(*it_2, 1E-3), message_error_vertices_compare(*it_1, *it_2));
    }

}

BOOST_AUTO_TEST_CASE( test_projection_at_position_minimum_curvature , * utf::tolerance(1E-6))
{
    Vertices trajectory = Trajectory::SPE84246;

    MinimumCurvatureInterpolator interpolator{trajectory};

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
        auto x = interpolator.x_at_position(item.second.position());
        auto y = interpolator.y_at_position(item.second.position());
        auto z = interpolator.z_at_position(item.second.position());

        BOOST_TEST(x == item.first[0]);
        BOOST_TEST(y == item.first[1]);
        BOOST_TEST(z == item.first[2]);

    }

}

BOOST_AUTO_TEST_CASE( test_projection_at_position_linear , * utf::tolerance(1E-6))
{
    Vertices trajectory = Trajectory::SPE84246;

    LinearInterpolator interpolator{trajectory};

    // the std::array<double, 3> represents the x, y, z expected values
    std::map<std::array<double, 3>, Vertex> samples_expected =
        {
            {{14.512758309548609, 14.512758309548609, 213.1513949}, {214.13724, 0.095993095, 0.785398049999999}},
            {{57.288337987899368, 200.53444778608741, 547.1159741}, {598.800936, 0.519235377499999, 1.3447759945}},
            {{163.09960789898872 , 660.68467502025919, 1373.223281}, {1550.31948, 0.519235377499999, 1.3447759945}},
            {{492.07755932444019, -567.08128131544527, 639.3673737}, {3018.032064, 2.09439479999999, 4.97418765}},

            //interpolated vertices:
            {{134.75191664908016, 537.40672380419028, 1151.902482}, {1295.4, 0.5192353775, 1.3447759945}},
            {{1055.2815792122528, 127.35839178394474, 1304.067185}, {2592.052728, 1.63723079377336, 5.74440162129123}},
            {{1009.7933937687196, -77.894972532420184, 1177.583381}, {2690.78659199999, 1.74319266798144, 5.56588075169589}},
            {{907.63861625602476, -269.5504761021067, 1032.718821}, {2789.520456, 1.84915454218953, 5.38735988210054}},
        };


    for(auto& item: samples_expected)
    {
        auto x = interpolator.x_at_position(item.second.position());
        auto y = interpolator.y_at_position(item.second.position());
        auto z = interpolator.z_at_position(item.second.position());

        BOOST_TEST(x == item.first[0]);
        BOOST_TEST(y == item.first[1]);
        BOOST_TEST(z == item.first[2]);

    }

}

BOOST_AUTO_TEST_CASE( test_projection_at_position_cubic , * utf::tolerance(1E-6))
{
    Vertices trajectory = Trajectory::SPE84246;

    CubicInterpolator interpolator{trajectory};

    // the std::array<double, 3> represents the x, y, z expected values
    std::map<std::array<double, 3>, Vertex> samples_expected =
        {
            {{14.512758309548609, 14.512758309548609, 213.1513949}, {214.13724, 0.095993095, 0.785398049999999}},
            {{57.288337987899368, 200.53444778608741, 547.1159741}, {598.800936, 0.519235377499999, 1.3447759945}},
            {{163.09960789898872 , 660.68467502025919, 1373.223281}, {1550.31948, 0.519235377499999, 1.3447759945}},
            {{492.07755932444019, -567.08128131544527, 639.3673737}, {3018.032064, 2.09439479999999, 4.97418765}},

            //interpolated vertices:
            {{134.75191664908016, 537.40672380419028, 1151.902482}, {1295.4, 0.5192353775, 1.3447759945}},
            {{422.75305956812298, 1667.0180767766537, 1301.9398363024814}, {2592.052728, 1.63723079377336, 5.74440162129123}},
            {{447.17066692091004, 1747.4767588236562, 1176.1145966815518}, {2690.78659199999, 1.74319266798144, 5.56588075169589}},
            {{467.54444833472775, 1812.3960323023969, 1031.9120632871693}, {2789.520456, 1.84915454218953, 5.38735988210054}},
        };


    for(auto& item: samples_expected)
    {
        auto x = interpolator.x_at_position(item.second.position());
        auto y = interpolator.y_at_position(item.second.position());
        auto z = interpolator.z_at_position(item.second.position());

        BOOST_TEST(x == item.first[0]);
        BOOST_TEST(y == item.first[1]);
        BOOST_TEST(z == item.first[2]);

    }

}

BOOST_AUTO_TEST_CASE( test_builder )
{

    auto linear_trajectory = build_interpolator(Trajectory::SPE84246, InterpolationType::linear);
    auto minimum_curvature_trajectory = build_interpolator(Trajectory::SPE84246, InterpolationType::minimum_curvature);
    auto cubic_trajectory = build_interpolator(Trajectory::SPE84246, InterpolationType::cubic);

    BOOST_CHECK(linear_trajectory->interpolation_type() == InterpolationType::linear);
    BOOST_CHECK(minimum_curvature_trajectory->interpolation_type() == InterpolationType::minimum_curvature);
    BOOST_CHECK(cubic_trajectory->interpolation_type() == InterpolationType::cubic);

}
