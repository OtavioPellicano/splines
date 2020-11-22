from _interpolator import(AngleUnit, BuildInterpolator,
                          InterpolationType, Vertex)
import pytest
import numpy as np


@pytest.fixture
def trajectory_SPE84246():
    return set([Vertex(214.13724, 0.095993095, 0.785398049999999),
                Vertex(598.800936, 0.519235377499999, 1.3447759945),
                Vertex(1550.31948, 0.519235377499999, 1.3447759945),
                Vertex(3018.032064, 2.09439479999999, 4.97418765)])


@pytest.fixture
def interpolation_samples_SPE84246():
    return sorted([214.13724, 598.800936, 1550.31948, 3018.032064, 1295.4, 2592.052728, 2690.786592, 2789.520456])


@pytest.mark.parametrize(
    'angle_unit',
    [AngleUnit.Deg, AngleUnit.Rad],
    ids=['deg', 'rad'])
@pytest.mark.parametrize(
    'interpolation_type',
    [InterpolationType.Linear,
     InterpolationType.MinimumCurvature,
     InterpolationType.Cubic],
    ids=['linear', 'minimum_curvature', 'cubic'])
def test_3d_interpolation(
        num_regression,
        trajectory_SPE84246,
        interpolation_samples_SPE84246,
        angle_unit,
        interpolation_type
    ):

    vertices = trajectory_SPE84246
    interpolator = BuildInterpolator(vertices, interpolation_type)

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

    res = dict()
    res['measured_depth'] = positions
    res['inclination'] = inclinations
    res['azimuth'] = azimuths
    res['projection_x'] = projections_x
    res['projection_y'] = projections_y
    res['projection_z'] = projections_z

    num_regression.check(res)
