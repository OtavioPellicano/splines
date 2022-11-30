from _interpolator import (
    AngleUnit,
    InterpolatorFactory,
    InterpolationType,
    Vertex,
    Vertices,
)
import pytest
import numpy as np


@pytest.fixture
def trajectory_SPE84246():
    return Vertices(
        [
            Vertex(214.13724, 0.095993095, 0.785398049999999),
            Vertex(598.800936, 0.519235377499999, 1.3447759945),
            Vertex(1550.31948, 0.519235377499999, 1.3447759945),
            Vertex(3018.032064, 2.09439479999999, 4.97418765),
        ]
    )


@pytest.fixture
def interpolation_samples_SPE84246():
    return sorted(
        [
            214.13724,
            598.800936,
            1550.31948,
            3018.032064,
            1295.4,
            2592.052728,
            2690.786592,
            2789.520456,
        ]
    )


def _CompareInterpolationTypeStr(interpolator):

    if interpolator.InterpolationType() == InterpolationType.Linear:
        assert "linear" == interpolator.InterpolationTypeStr()
    elif interpolator.InterpolationType() == InterpolationType.MinimumCurvature:
        assert "minimum_curvature" == interpolator.InterpolationTypeStr()
    elif interpolator.InterpolationType() == InterpolationType.Cubic:
        assert "cubic" == interpolator.InterpolationTypeStr()


def _make_interpolator(trajectory, interpolation_type):
    if interpolation_type == InterpolationType.Linear:
        return InterpolatorFactory.MakeLinearInterpolator(trajectory)
    elif interpolation_type == InterpolationType.MinimumCurvature:
        return InterpolatorFactory.MakeMinimumCurvatureInterpolator(trajectory)
    elif interpolation_type == InterpolationType.Cubic:
        return InterpolatorFactory.MakeCubicInterpolator(trajectory)


@pytest.mark.parametrize(
    "angle_unit", [AngleUnit.Deg, AngleUnit.Rad], ids=["deg", "rad"]
)
@pytest.mark.parametrize(
    "interpolation_type",
    [
        InterpolationType.Linear,
        InterpolationType.MinimumCurvature,
        InterpolationType.Cubic,
    ],
    ids=["linear", "minimum_curvature", "cubic"],
)
def test_3d_interpolation(
    num_regression,
    trajectory_SPE84246,
    interpolation_samples_SPE84246,
    angle_unit,
    interpolation_type,
):

    trajectory = trajectory_SPE84246
    interpolator = _make_interpolator(trajectory, interpolation_type)

    # Compare InterpolationType with InterpolationTypeStr
    _CompareInterpolationTypeStr(interpolator)

    positions = np.zeros_like(interpolation_samples_SPE84246)
    inclinations = np.zeros_like(interpolation_samples_SPE84246)
    azimuths = np.zeros_like(interpolation_samples_SPE84246)
    projections_x = np.zeros_like(interpolation_samples_SPE84246)
    projections_y = np.zeros_like(interpolation_samples_SPE84246)
    projections_z = np.zeros_like(interpolation_samples_SPE84246)

    for i, sample in enumerate(interpolation_samples_SPE84246):
        positions[i] = interpolator.VertexAtPosition(sample).Position()
        inclinations[i] = interpolator.VertexAtPosition(sample).Inclination(angle_unit)
        azimuths[i] = interpolator.VertexAtPosition(sample).Azimuth(angle_unit)

        projections_x[i] = interpolator.XAtPosition(sample)
        projections_y[i] = interpolator.YAtPosition(sample)
        projections_z[i] = interpolator.ZAtPosition(sample)

    assert len(trajectory.Vertices()) == len(interpolator.Trajectory().Positions())

    assert interpolator.Trajectory().ApproxEqual(trajectory, 0.1)

    vertices_pos = interpolator.Trajectory().Positions()
    vertices_inc = interpolator.Trajectory().Inclinations(angle_unit)
    vertices_azm = interpolator.Trajectory().Azimuths(angle_unit)

    for i, vertex in enumerate(trajectory.VerticesSorted()):
        assert pytest.approx(vertices_pos[i]) == vertex.Position()
        assert pytest.approx(vertices_inc[i]) == vertex.Inclination(angle_unit)
        assert pytest.approx(vertices_azm[i]) == vertex.Azimuth(angle_unit)

    res = dict()
    res["position"] = positions
    res["inclination"] = inclinations
    res["azimuth"] = azimuths
    res["projection_x"] = projections_x
    res["projection_y"] = projections_y
    res["projection_z"] = projections_z

    num_regression.check(res)


def test_vertex_repr_operator():
    vertex = Vertex(1.1, 2.2, 3.3)

    delimiter = ","
    assert delimiter == vertex.Delimiter()
    assert str(vertex) == delimiter.join(
        [str(vertex.Position()), str(vertex.Inclination()), str(vertex.Azimuth())]
    )


def test_trajectory_class(trajectory_SPE84246):
    assert trajectory_SPE84246.Size() == 4

    trajectory_deg = list()

    for vertex in trajectory_SPE84246.Vertices():
        trajectory_deg.append(
            Vertex(
                vertex.Position(),
                vertex.Inclination(AngleUnit.Deg),
                vertex.Azimuth(AngleUnit.Deg),
                AngleUnit.Deg,
            )
        )

    def compare_trajectory(t_1, t_2):
        assert t_1.Size() == t_2.Size()

        for (vt_1, vt_2) in zip(t_1.VerticesSorted(), t_2.VerticesSorted()):
            assert pytest.approx(vt_1.Position()) == vt_2.Position()
            assert pytest.approx(vt_1.Inclination()) == vt_2.Inclination()
            assert pytest.approx(vt_1.Azimuth()) == vt_2.Azimuth()
            assert pytest.approx(vt_1.Inclination(AngleUnit.Deg)) == vt_2.Inclination(
                AngleUnit.Deg
            )
            assert pytest.approx(vt_1.Azimuth(AngleUnit.Deg)) == vt_2.Azimuth(
                AngleUnit.Deg
            )

    trajectory_set_sorted = Vertices()
    trajectory_set = Vertices()
    trajectory_set_sorted.SetVertices(trajectory_SPE84246.VerticesSorted())
    trajectory_set.SetVertices(trajectory_SPE84246.Vertices())

    trajectory_deg = Vertices(trajectory_deg)
    compare_trajectory(trajectory_SPE84246, trajectory_deg)
    compare_trajectory(trajectory_set_sorted, trajectory_set)
