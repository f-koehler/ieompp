#ifndef HUBBARD_DISCRETIZATION_LINEAR_HPP_
#define HUBBARD_DISCRETIZATION_LINEAR_HPP_

#include <vector>
#include <array>

#include "hubbard/constants.hpp"

namespace hubbard
{
    namespace discretization
    {
        template <typename RealT>
        struct LinearDiscretization {
            using Real   = RealT;
            using Index  = std::size_t;
            using Vector = RealT;

            std::vector<Index> indices;
            std::vector<Real> sites;
            const std::size_t num_x;
            const Real dx;
            const Real x_min, x_max;
            const Real x_diff;
            const std::array<Vector, 1> lattice_vectors;

            LinearDiscretization(const std::size_t n, const Real& delta_x);
            LinearDiscretization(const std::size_t n);

            inline bool out_of_bounds(const Vector& v) const;
            const Index& closest(const Vector& v) const;
            Index& closest(const Vector& v);
            inline std::array<Index, 2> neighbours(const Index& idx) const;
            inline std::array<Index, 1> unique_neighbours(const Index& idx) const;

            inline const Vector& operator[](const Index& i) const;
            inline Vector& operator[](const Index& i);
            inline const Index& operator[](const Vector& v) const;
            inline Index& operator[](const Vector& v);
        };
    }
}

#include "detail/linear_impl.hpp"

#endif
