#ifndef HUBBARD_SQUARE2D_HPP_
#define HUBBARD_SQUARE2D_HPP_

#include "hubbard/types.hpp"

#include <tuple>

namespace hubbard
{
    namespace square2d
    {
        using Momentum = std::tuple<Real, Real>;
        std::ostream& operator<<(std::ostream& strm, const Momentum& rhs) {
            strm << "(" << std::get<0>(rhs) << "," << std::get<1>(rhs) << ")";
            return strm;
        }

        struct Discretization
        {
            const std::size_t nx, ny;
            const Real kx_min, ky_min;
            const Real kx_max, ky_max;
            const Real kx_diff, ky_diff;
            const Real dkx, dky;

            inline Real kx(std::size_t index) const { return kx_min + index * dkx; }
            inline Real ky(std::size_t index) const { return ky_min + index * dky; }

            inline std::size_t kx_index(const Real& kx) const
            {
                return std::round((kx - kx_min) / dkx);
            }

            inline std::size_t ky_index(const Real& ky) const
            {
                return std::round((ky - ky_min) / dky);
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
