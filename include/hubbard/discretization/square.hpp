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
        class SquareDiscretization {
            public:
                using Real   = RealT;
                using Index  = std::tuple<std::size_t, std::size_t>;
                using Vector = Eigen::Matrix<Real, 2, 1>;

            private:
                std::vector<Index> init_indices();
                std::vector<std::vector<Vector>> init_sites();

            public:
                const std::size_t num;
                const std::size_t num_x, num_y;
                const Real dx, dy;
                const Real x_min, y_min, x_max, y_max;
                const Real x_diff, y_diff;
                const std::array<Vector, 2> lattice_vectors;
                const std::vector<Index> indices;
                const std::vector<std::vector<Vector>> sites;

                // init in real space
                SquareDiscretization(const std::size_t nx, const std::size_t ny, const Real& delta_x,
                                     const Real& delta_y);

                // init in momentum space
                SquareDiscretization(const std::size_t nx, const std::size_t ny);

                const Index& closest(const Vector& v) const;
                std::array<Index, 4> neighbours(const Index& idx) const;
                std::array<Index, 2> unique_neighbours(const Index& idx) const;
                Vector project(Vector v) const;

                inline std::vector<Index>::const_iterator begin() const;
                inline std::vector<Index>::const_iterator end() const;

                inline const Vector& operator[](const Index& i) const;
                inline const Index& operator[](const Vector& v) const;
        };
    }
}

#include "detail/square_impl.hpp"

#endif
