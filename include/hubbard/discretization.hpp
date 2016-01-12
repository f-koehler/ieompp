#ifndef DISCRETIZATION_HPP_
#define DISCRETIZATION_HPP_

#include "hubbard/types.hpp"

namespace hubbard
{
    const auto PI = 3.14159265358979323846;

    struct Discretization
    {
        const std::size_t nx, ny;
        const Real kx_min, ky_min;
        const Real kx_max, ky_max;
        const Real dkx, dky;
    };

    Discretization discretize(std::size_t nx, std::size_t ny)
    {
        return Discretization{nx, ny, -PI, -PI, PI, PI, 2 * PI / Real(nx), 2 * PI / Real(ny)};
    }
}

#endif
