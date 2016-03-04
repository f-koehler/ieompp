#ifndef HUBBARD_DISCRETIZATION_SQUARE_HPP_
#define HUBBARD_DISCRETIZATION_SQUARE_HPP_

#include <array>
#include <tuple>
#include <vector>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <Eigen/Dense>
#pragma GCC diagnostic pop

#include "hubbard/constants.hpp"

namespace hubbard
{
    namespace discretization
    {
        template <typename RealT>
        struct SquareDiscretization {
            using Real   = RealT;
            using Index  = std::tuple<std::size_t, std::size_t>;
            using Vector = Eigen::Matrix<Real, 2, 1>;

            std::vector<Index> indices;
            std::vector<std::vector<Vector>> sites;
            const std::size_t num_x, num_y;
            const Real dx, dy;
            const Real x_min, y_min, x_max, y_max;

            // init in real space
            SquareDiscretization(const std::size_t nx, const std::size_t ny, const Real& delta_x,
                                 const Real& delta_y);

            // init in momentum space
            SquareDiscretization(const std::size_t nx, const std::size_t ny);

            Index closest(const Vector& v) const;
            std::array<Index, 4> neighbours(const Index& idx) const;
            std::array<Index, 2> unique_neighbours(const Index& idx) const;

            inline const Vector& operator[](const Index& i) const;
            inline Vector& operator[](const Index& i);
        };
    }
}

#include "detail/square_impl.hpp"

#endif
