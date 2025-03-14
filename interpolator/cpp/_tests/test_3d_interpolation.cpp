#define BOOST_TEST_MODULE test_3d_interpolation

// The unit_test.hpp must be the first include (it is crashing in Travis CI)
#include <boost/test/included/unit_test.hpp>

#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/test_case.hpp>

namespace utf = boost::unit_test;
namespace data = boost::unit_test::data;

#include <algorithm>
#include <map>
#include <sstream>
#include <typeinfo>

#include <interpolator/InterpolatorFactory.hpp>

using namespace splines;

std::string message_error_vertices_compare(const Vertex &v_1, const Vertex &v_2)
{
    using namespace std;

    return "{" + to_string(v_1.position()) + ", " + to_string(v_1.inclination()) + ", " + to_string(v_1.azimuth()) +
           "} != "
           "{" +
           to_string(v_2.position()) + ", " + to_string(v_2.inclination()) + ", " + to_string(v_2.azimuth()) + "}";
}

enum class InterpolationType
{
    linear,
    minimum_curvature,
    cubic,
};

std::string interpolation_type_str(const BaseInterpolator &interpolator)
{
    if (typeid(interpolator) == typeid(LinearInterpolator))
    {
        return "linear";
    }
    else if (typeid(interpolator) == typeid(MinimumCurvatureInterpolator))
    {
        return "minimum_curvature";
    }
    else if (typeid(interpolator) == typeid(CubicInterpolator))
    {
        return "cubic";
    }
    else
    {
        return "";
    }
}

std::unique_ptr<BaseInterpolator> make_interpolator(const Vertices &trajectory, InterpolationType interpolation_type)
{
    switch (interpolation_type)
    {
    case InterpolationType::linear:
        return InterpolatorFactory::make<LinearInterpolator>(trajectory);
    case InterpolationType::minimum_curvature:
        return InterpolatorFactory::make<MinimumCurvatureInterpolator>(trajectory);
    case InterpolationType::cubic:
        return InterpolatorFactory::make<CubicInterpolator>(trajectory);
    default:
        return nullptr;
    }
}

enum class ProjectionType
{
    x,
    y,
    z
};
std::vector<double> generate_projections(
    const std::unique_ptr<BaseInterpolator> &interpolator, ProjectionType projection,
    InterpolationType interpolation_type, unsigned num_points,
    unsigned num_threads = std::numeric_limits<unsigned>::max())
{
    if (projection == ProjectionType::x)
    {
        return interpolator->generate_x_projections(num_points, num_threads);
    }
    else if (projection == ProjectionType::y)
    {
        return interpolator->generate_y_projections(num_points, num_threads);
    }
    else
    {
        return interpolator->generate_z_projections(num_points, num_threads);
    }
}

namespace Samples
{

// The vertices choice were based on SPE 84246 paper, pg. 16
const Vertices SPE84246 = {
    {214.13724, 0.095993095, 0.785398049999999},
    {598.800936, 0.519235377499999, 1.3447759945},
    {1550.31948, 0.519235377499999, 1.3447759945},
    {3018.032064, 2.09439479999999, 4.97418765}};

InterpolationType interpolation_types[] = {
    InterpolationType::linear, InterpolationType::minimum_curvature, InterpolationType::cubic};

ProjectionType projections_type[] = {ProjectionType::x, ProjectionType::y, ProjectionType::z};

std::map<double, Vertex> vertices_at_postion_expected[] = {
    {
        // Linear
        {214.13724, {214.13724, 0.095993095, 0.785398049999999}},
        {598.800936, {598.800936, 0.519235377499999, 1.3447759945}},
        {1550.31948, {1550.31948, 0.519235377499999, 1.3447759945}},
        {3018.032064, {3018.032064, 2.09439479999999, 4.97418765}},

        {1295.4, {1295.4, 0.519235, 1.344776}},
        {2592.052728, {2592.052728, 1.637231, 5.744402}},
        {2690.786592, {2690.786592, 1.743193, 5.565881}},
        {2789.520456, {2789.520456, 1.849155, 5.387360}},
    },
    {
        // Minimum Curvature
        {214.13724, {214.13724, 5.5, 45.0, AngleUnit::deg}},
        {598.800936, {598.800936, 29.75, 77.05, AngleUnit::deg}},
        {1295.4, {1295.4, 29.75, 77.05, AngleUnit::deg}},
        {1550.31948, {1550.31948, 29.75, 77.05, AngleUnit::deg}},
        {2592.052728, {2592.052728, 80.89, 300.71, AngleUnit::deg}},
        {2690.786592, {2690.786592, 90.0, 297.31, AngleUnit::deg}},
        {2789.520456, {2789.520456, 99.11, 293.92, AngleUnit::deg}},
        {3018.032064, {3018.032064, 120.0, 285.0, AngleUnit::deg}},
    },
    {
        // Cubic
        {214.13724, {214.13724, 0.095993095, 0.785398049999999}},
        {598.800936, {598.800936, 0.519235377499999, 1.3447759945}},
        {1550.31948, {1550.31948, 0.519235377499999, 1.3447759945}},
        {3018.032064, {3018.032064, 2.09439479999999, 4.97418765}},

        {1295.4, {1295.4, 0.519235, 1.344776}},
        {2592.052728, {2592.052728, 1.639278, 1.318284}},
        {2690.786592, {2690.786592, 1.744500, 1.315131}},
        {2789.520456, {2789.520456, 1.849832, 1.312366}},
    }};

// the std::array<double, 3> represents the x, y, z expected values
std::map<std::array<double, 3>, Vertex> projection_at_position_expected[] = {
    {
        // Linear
        {{14.512758309548609, 14.512758309548609, 213.1513949}, {214.13724, 0.095993095, 0.785398049999999}},
        {{57.288337987899368, 200.53444778608741, 547.1159741}, {598.800936, 0.519235377499999, 1.3447759945}},
        {{163.09960789898872, 660.68467502025919, 1373.223281}, {1550.31948, 0.519235377499999, 1.3447759945}},
        {{492.07755932444019, -567.08128131544527, 639.3673737}, {3018.032064, 2.09439479999999, 4.97418765}},

        // interpolated vertices:
        {{134.75191664908016, 537.40672380419028, 1151.902482}, {1295.4, 0.5192353775, 1.3447759945}},
        {{1055.2815792122528, 127.35839178394474, 1304.067185}, {2592.052728, 1.63723079377336, 5.74440162129123}},
        {{1009.7933937687196, -77.894972532420184, 1177.583381},
         {2690.78659199999, 1.74319266798144, 5.56588075169589}},
        {{907.63861625602476, -269.5504761021067, 1032.718821}, {2789.520456, 1.84915454218953, 5.38735988210054}},
    },
    {
        // Minimum Curvature
        {{7.2619580385825513, 7.2619580385825513, 213.808524}, {214.13724, 5.5, 45.0, AngleUnit::deg}},
        {{42.252476733170006, 115.05692333344899, 578.1493471}, {598.800936, 29.75, 77.05, AngleUnit::deg}},
        {{148.06374664425934, 575.20715056762072, 1404.256654}, {1550.31948, 29.75, 77.05, AngleUnit::deg}},
        {{757.17661872735744, -65.828691703728509, 2073.043306}, {3018.032064, 120.0, 285.0, AngleUnit::deg}},

        // interpolated vertices:
        {{119.71605539435078, 451.92919935155186, 1182.935855}, {1295.4, 29.75, 77.05, AngleUnit::deg}},
        {{594.72150050487528, 310.07764311833108, 2149.247025}, {2592.052728, 80.89, 300.71, AngleUnit::deg}},
        {{642.38182819109716, 224.1, 2157.081421}, {2690.786592, 90.0, 297.31, AngleUnit::deg}},
        {{684.89863558697255, 135.46634412415341, 2149.244745}, {2789.520456, 99.11, 293.92, AngleUnit::deg}},

    },
    {
        // Cubic
        {{14.512758309548609, 14.512758309548609, 213.1513949}, {214.13724, 0.095993095, 0.785398049999999}},
        {{57.288337987899368, 200.53444778608741, 547.1159741}, {598.800936, 0.519235377499999, 1.3447759945}},
        {{163.09960789898872, 660.68467502025919, 1373.223281}, {1550.31948, 0.519235377499999, 1.3447759945}},
        {{492.07755932444019, -567.08128131544527, 639.3673737}, {3018.032064, 2.09439479999999, 4.97418765}},

        // interpolated vertices:
        {{134.75191664908016, 537.40672380419028, 1151.902482}, {1295.4, 0.5192353775, 1.3447759945}},
        {{422.75305956812298, 1667.0180767766537, 1301.9398363024814},
         {2592.052728, 1.63723079377336, 5.74440162129123}},
        {{447.17066692091004, 1747.4767588236562, 1176.1145966815518},
         {2690.78659199999, 1.74319266798144, 5.56588075169589}},
        {{467.54444833472775, 1812.3960323023969, 1031.9120632871693},
         {2789.520456, 1.84915454218953, 5.38735988210054}},
    }};

}; // namespace Samples

BOOST_AUTO_TEST_CASE(test_readme_example, *utf::tolerance(1E-4))
{
    // Vertices is a vertices wrapper
    // Vertex is basically a struct where you store position, inclination and azimuth as follow:
    // Vertex{position, inclination, azimuth}
    // inclination: angle from z to y axis
    // azimuth: angle from x to y axis
    auto trajectory = splines::Vertices(
        {{214.13724, 0.095993095, 0.785398049999999},
         {598.800936, 0.519235377499999, 1.3447759945},
         {1550.31948, 0.519235377499999, 1.3447759945},
         {3018.032064, 2.09439479999999, 4.97418765}} /*,AngleUnit::rad*/);

    auto linear_interpolator = splines::InterpolatorFactory::make<LinearInterpolator>(trajectory);

    auto position_desired = 2690.786592;

    auto vertex = linear_interpolator->vertex_at_position(position_desired);

    // expected: 2690.79,1.74319,5.56588
    // std::cout << vertex << std::endl;

    // get projections_type (cartesian coordinates)
    auto x = linear_interpolator->x_at_position(position_desired);
    auto y = linear_interpolator->y_at_position(position_desired);
    auto z = linear_interpolator->z_at_position(position_desired);

    // expected: 1009.79, -77.895, 1177.58
    // std::cout << x << ", " << y << ", " << z << std::endl;

    // return 0;

    ///////////////////////////////////////////////////////
    auto x_expected = 1009.79;
    auto y_expected = -77.895;
    auto z_expected = 1177.58;

    BOOST_TEST(x == x_expected);
    BOOST_TEST(y == y_expected);
    BOOST_TEST(z == z_expected);

    std::stringstream vertex_ss;
    vertex_ss << vertex;
    BOOST_CHECK(vertex_ss.str() == "2690.79,1.74319,5.56588");
}

BOOST_AUTO_TEST_CASE(test_angle_conversion, *utf::tolerance(1E-6))
{

    Vertices trajectory = {{0.5, M_PI / 2, M_PI, AngleUnit::rad}, {1.0, 45.0, 30.0, AngleUnit::deg}};

    BOOST_TEST(trajectory.vertices().begin()->inclination() == M_PI / 2);
    BOOST_TEST(trajectory.vertices().begin()->azimuth() == M_PI);

    BOOST_TEST(trajectory.vertices().begin()->inclination(AngleUnit::deg) == 90.0);
    BOOST_TEST(trajectory.vertices().begin()->azimuth(AngleUnit::deg) == 180.0);

    BOOST_TEST(trajectory.vertices().rbegin()->inclination() == M_PI / 4);
    BOOST_TEST(trajectory.vertices().rbegin()->azimuth() == M_PI / 6);
}

// The BOOST_DATA_TEST_CASE has to implement operator<< to be printable
// For some reason I implemented the operator<<, but error remains, so I have to
// use BOOST_TEST_DONT_PRINT_LOG_VALUE Macro anyway
typedef std::map<double, Vertex> MapVt;
BOOST_TEST_DONT_PRINT_LOG_VALUE(MapVt)
BOOST_TEST_DONT_PRINT_LOG_VALUE(InterpolationType)
BOOST_TEST_DONT_PRINT_LOG_VALUE(ProjectionType)

BOOST_DATA_TEST_CASE(
    test_vertex_at_position, data::make(Samples::interpolation_types) ^ Samples::vertices_at_postion_expected,
    interpolation_type, samples_expected)
{
    auto tol = 1E-4;
    auto interpolator = make_interpolator(Samples::SPE84246, interpolation_type);

    BOOST_TEST(interpolator->trajectory().approx_equal(Samples::SPE84246));

    for (auto const &[position, vertex_expected] : samples_expected)
    {
        auto const &v_1 = interpolator->vertex_at_position(position);
        auto const &v_2 = vertex_expected;

        BOOST_TEST(
            v_1.approx_equal(v_2, .2),
            interpolation_type_str(*interpolator) + ": " + message_error_vertices_compare(v_1, v_2));

        // compare using structured binding
        auto [pos, inc, azm] = v_1;
        auto [pos_exp, inc_exp, azm_exp] = v_2;

        BOOST_TEST(fabs(pos - pos_exp) < tol);
        BOOST_TEST(fabs(inc - inc_exp) < tol);
        BOOST_TEST(fabs(azm - azm_exp) < tol);
    }
}

BOOST_AUTO_TEST_CASE(test_add_and_drop)
{

    Vertices trajectory_sample = Samples::SPE84246;

    MinimumCurvatureInterpolator interpolator{trajectory_sample};

    Vertices samples = {
        {10.0, 5.5, 45.0, AngleUnit::deg},
        {1600.0, 29.75, 77.05, AngleUnit::deg},
    };

    interpolator.add_n_drop(*samples.vertices().begin());
    BOOST_TEST(interpolator.trajectory().size() == trajectory_sample.size(), "different size");
    interpolator.add_n_drop(*samples.vertices().rbegin());
    BOOST_TEST(interpolator.trajectory().size() == trajectory_sample.size(), "different size");
    interpolator.drop_n_add(*samples.vertices().begin());
    BOOST_TEST(interpolator.trajectory().size() == trajectory_sample.size(), "different size");
    interpolator.drop_n_add(*samples.vertices().rbegin());
    BOOST_TEST(interpolator.trajectory().size() == trajectory_sample.size(), "different size");

    auto expected = Vertices(
        {
            {214.13724, 5.5, 45.0},
            {598.800936, 29.75, 77.05},
            {1550.31948, 29.75, 77.05},
            {1600.0, 29.75, 77.05},
        },
        AngleUnit::deg);

    auto const &trajectory = interpolator.trajectory();

    for (auto it_1 = trajectory.vertices().begin(), it_2 = expected.vertices().begin();
         it_1 != trajectory.vertices().end(); ++it_1, ++it_2)
    {
        BOOST_TEST(it_1->approx_equal(*it_2, 1E-3), message_error_vertices_compare(*it_1, *it_2));
    }
}

typedef std::map<std::array<double, 3>, Vertex> MapArrVt;
BOOST_TEST_DONT_PRINT_LOG_VALUE(MapArrVt)

BOOST_DATA_TEST_CASE(
    test_projection_at_position, data::make(Samples::interpolation_types) ^ Samples::projection_at_position_expected,
    interpolation_type, samples_expected)
{

    auto tol = 1E-4;
    auto interpolator = make_interpolator(Samples::SPE84246, interpolation_type);

    auto error_msg = [&interpolator](double current, double expected) -> std::string {
        return interpolation_type_str(*interpolator) + ": current != expected: " + std::to_string(current) +
               " != " + std::to_string(expected);
    };

    for (auto &[point, vertex_expected] : samples_expected)
    {
        auto x = interpolator->x_at_position(vertex_expected.position());
        auto y = interpolator->y_at_position(vertex_expected.position());
        auto z = interpolator->z_at_position(vertex_expected.position());

        BOOST_TEST(fabs(x - point[0]) < tol, error_msg(x, point[0]));
        BOOST_TEST(fabs(y - point[1]) < tol, error_msg(x, point[1]));
        BOOST_TEST(fabs(z - point[2]) < tol, error_msg(x, point[2]));
    }
}

BOOST_AUTO_TEST_CASE(test_factory)
{
    auto linear_trajectory = InterpolatorFactory::make<LinearInterpolator>(Samples::SPE84246);
    auto minimum_curvature_trajectory = InterpolatorFactory::make<MinimumCurvatureInterpolator>(Samples::SPE84246);
    auto cubic_trajectory = InterpolatorFactory::make<CubicInterpolator>(Samples::SPE84246);

    BOOST_CHECK(typeid(*linear_trajectory) == typeid(LinearInterpolator));
    BOOST_CHECK(typeid(*minimum_curvature_trajectory) == typeid(MinimumCurvatureInterpolator));
    BOOST_CHECK(typeid(*cubic_trajectory) == typeid(CubicInterpolator));
}

BOOST_AUTO_TEST_CASE(test_operator_ostream)
{

    auto stream_v = [](std::stringstream &ss, Vertex &vt) -> std::stringstream & {
        ss << vt.position() << "," << vt.inclination() << "," << vt.azimuth();
        return ss;
    };

    auto stream_o = [](std::stringstream &ss, Vertex &vt) -> std::stringstream & {
        ss << vt;
        return ss;
    };

    auto stream_out = [](Vertices &vts,
                         std::stringstream &(*st)(std::stringstream &ss, Vertex &vt)) -> std::stringstream {
        std::stringstream ss;
        for (auto vt : vts.vertices())
        {
            st(ss, vt);
        }
        return ss;
    };

    auto vertices = Samples::SPE84246;
    std::string str_vertices = stream_out(vertices, stream_v).str().c_str();
    std::string str_operator = stream_out(vertices, stream_o).str().c_str();

    BOOST_TEST(str_vertices == str_operator);
}

BOOST_AUTO_TEST_CASE(test_trajectory_class, *utf::tolerance(1E-6))
{

    auto compare_trajectory = [](const Vertices &t_1, const Vertices &t_2) {
        BOOST_TEST(t_1.size() == t_2.size());

        for (auto vt_1 = t_1.vertices().begin(), vt_2 = t_2.vertices().begin(); vt_1 != t_1.vertices().end();
             ++vt_1, ++vt_2)
        {
            BOOST_TEST(vt_1->position() == vt_2->position());
            BOOST_TEST(vt_1->inclination() == vt_2->inclination());
            BOOST_TEST(vt_1->azimuth() == vt_2->azimuth());
            BOOST_TEST(vt_1->inclination(AngleUnit::deg) == vt_2->inclination(AngleUnit::deg));
            BOOST_TEST(vt_1->azimuth(AngleUnit::deg) == vt_2->azimuth(AngleUnit::deg));
        }
    };

    auto trajectory = Samples::SPE84246;

    BOOST_TEST(trajectory.size() == 4);

    auto trajectory_set = Vertices({});

    trajectory_set.set_vertices(trajectory.vertices() /*,AngleUnit::rad*/);

    auto trajectory_deg = trajectory;

    std::for_each(trajectory_deg.vertices().begin(), trajectory_deg.vertices().end(), [](const Vertex &vt) {
        return Vertex(vt.position(), vt.inclination(AngleUnit::deg), vt.azimuth(AngleUnit::deg), AngleUnit::deg);
    });

    compare_trajectory(trajectory, trajectory_deg);
    compare_trajectory(trajectory, trajectory_set);
}

BOOST_DATA_TEST_CASE(test_move_semantics, data::make(Samples::interpolation_types), interpolation_type)
{
    auto move_object = [](std::unique_ptr<BaseInterpolator> interpolator) -> void {};

    auto interpolator = make_interpolator(Samples::SPE84246, interpolation_type);
    move_object(std::move(interpolator));
    BOOST_CHECK(interpolator == nullptr);
}

BOOST_AUTO_TEST_CASE(test_structured_binding, *utf::tolerance(1E-6))
{
    splines::Vertices vertices_sample = Samples::SPE84246;

    auto v_it = vertices_sample.begin();
    for (auto [pos, inc, azm] : vertices_sample)
    {
        BOOST_TEST(pos == v_it->position());
        BOOST_TEST(inc == v_it->inclination());
        BOOST_TEST(azm == v_it->azimuth());
        ++v_it;
    }
}

BOOST_DATA_TEST_CASE(test_generate_vertices, data::make(Samples::interpolation_types), interpolation_type)
{
    std::size_t num_vertices = 100;
    auto interpolator = make_interpolator(Samples::SPE84246, interpolation_type);

    std::vector<Vertex> vertices_expected = interpolator->generate_vertices(num_vertices, 1);

    std::vector<std::vector<Vertex>> vertices_mt;
    vertices_mt.push_back(interpolator->generate_vertices(num_vertices));
    vertices_mt.push_back(interpolator->generate_vertices(num_vertices, 2));
    vertices_mt.push_back(interpolator->generate_vertices(num_vertices, 4));
    vertices_mt.push_back(interpolator->generate_vertices(num_vertices, 8));

    auto vertices_cmp = [](const auto &v_1, const auto &v_2) {
        for (size_t i = 0; i < v_1.size(); ++i)
        {
            BOOST_TEST(v_1[i].approx_equal(v_2[i]));
        }
    };

    for (auto &vertices : vertices_mt)
    {
        vertices_cmp(vertices_expected, vertices);
    }
}

BOOST_DATA_TEST_CASE(
    test_generate_projections,
    data::make<InterpolationType>(Samples::interpolation_types) ^ data::make(Samples::projections_type),
    interpolation_type, projection_type)
{
    std::size_t num_points = 100;
    auto interpolator = make_interpolator(Samples::SPE84246, interpolation_type);

    std::vector<Vertex> vertices_expected = interpolator->generate_vertices(num_points, 1);

    std::vector<std::vector<double>> projections_mt = {
        generate_projections(interpolator, projection_type, interpolation_type, num_points),
        generate_projections(interpolator, projection_type, interpolation_type, num_points, 2),
        generate_projections(interpolator, projection_type, interpolation_type, num_points, 4),
        generate_projections(interpolator, projection_type, interpolation_type, num_points, 8)};

    auto vertices_cmp = [&interpolator = interpolator](
                            const std::vector<Vertex> &v, const std::vector<double> &projs, ProjectionType proj_t) {
        double tol = 1e-6;
        for (size_t i = 0; i < v.size(); ++i)
        {
            double v_projection;
            if (proj_t == ProjectionType::x)
            {
                v_projection = interpolator->x_at_position(v[i].position());
            }
            else if (proj_t == ProjectionType::y)
            {
                v_projection = interpolator->y_at_position(v[i].position());
            }
            else
            {
                v_projection = interpolator->z_at_position(v[i].position());
            }
            BOOST_TEST(std::fabs(v_projection - projs[i]) < tol);
        }
    };

    for (auto &projs : projections_mt)
    {
        vertices_cmp(vertices_expected, projs, projection_type);
    }
}
