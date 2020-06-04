#ifndef BASE3DINTERPOLATION_HPP
#define BASE3DINTERPOLATION_HPP

#include "I3DInterpolation.hpp"
#include <vector>

namespace i3d {

class Base3DInterpolation : public I3DInterpolation
{
public:
    Base3DInterpolation(const Vertices& trajectory);

    const Vertices & vertices();

private:
    Vertices _vertices;

};

}

#endif // BASE3DINTERPOLATION_HPP
