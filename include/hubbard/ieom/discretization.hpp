#ifndef HUBBARD_IEOM_DISCRETIZATION_HPP_
#define HUBBARD_IEOM_DISCRETIZATION_HPP_

#include "hubbard/types.hpp"

#include <tuple>

namespace hubbard
{
    const auto PI = 3.14159265358979323846;

    namespace ieom
    {
        struct Discretization
        {
            const std::size_t nx, ny;
            const Real kx_min, ky_min;
            const Real kx_max, ky_max;
            const Real kx_diff, ky_diff;
            const Real dkx, dky;

            std::size_t kx_index(const Real& kx) {
                return std::round((kx-kx_min)/dkx);
            }
            
            std::size_t ky_index(const Real& ky) {
                return std::round((ky-ky_min)/dky);
            }
        };

        Discretization discretize(std::size_t nx, std::size_t ny)
        {
            return Discretization{
                nx, ny, -PI, -PI, PI, PI, 2 * PI, 2 * PI, 2 * PI / Real(nx), 2 * PI / Real(ny)};
        }
    }
}

#endif
