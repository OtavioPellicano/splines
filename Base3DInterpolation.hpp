#ifndef BASE3DINTERPOLATION_HPP
#define BASE3DINTERPOLATION_HPP

#include "I3DInterpolation.hpp"
#include <algorithm>
#include <cmath>

namespace i3d {

class Base3DInterpolation : public I3DInterpolation
{
public:
    Base3DInterpolation(const Vertices& vertices);

    const Vertices & vertices() const;

    AdjacentVertices calculate_adjacent_vertices(double curve_length) const override;

private:
    Vertices _vertices;

};

}

#endif // BASE3DINTERPOLATION_HPP
