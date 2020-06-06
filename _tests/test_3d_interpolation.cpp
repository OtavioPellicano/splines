#define BOOST_TEST_MODULE test_3d_interpolation
#include <boost/test/included/unit_test.hpp>

#include <map>
#include <Base3DInterpolation.hpp>
using namespace i3d;

std::string message_error_vertices_compare(const Vertex& v_1, const Vertex& v_2)
{
    using namespace std;

    return "{" + to_string(v_1.curve_length) + ", " + to_string(v_1.inclination) + ", " + to_string(v_1.azimuth) + "} != "
           "{" + to_string(v_2.curve_length) + ", " + to_string(v_2.inclination) + ", " + to_string(v_2.azimuth) + "}";
}

BOOST_AUTO_TEST_CASE( test_calculate_adjacent_vertices )
{

    Vertices trajectory_points =
    {
        {2.0, 0.0, 0.0},
        {0.0, 0.0, 0.0},
        {1.0, 0.0, 0.0},
    };

    Base3DInterpolation interpolator{trajectory_points};

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
