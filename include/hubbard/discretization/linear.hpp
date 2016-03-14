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
        class LinearDiscretization {
            public:
                using Real   = RealT;
                using Index  = std::size_t;
                using Vector = RealT;

            private:
                static std::vector<Index> init_indices(const std::size_t num);
                static std::vector<Vector> init_sites(const std::size_t num, const Real x_min, const Real dx);

            public:
                const std::size_t num;
                const std::size_t num_x;
                const Real dx;
                const Real x_min, x_max;
                const Real x_diff;
                const std::array<Vector, 1> lattice_vectors;
                const std::vector<Index> indices;
                const std::vector<Real> sites;

                LinearDiscretization(const std::size_t n, const Real& delta_x);
                LinearDiscretization(const std::size_t n);

                inline bool out_of_bounds(const Vector& v) const;
                const Index& closest(const Vector& v) const;
                inline std::array<Index, 2> neighbours(const Index& idx) const;
                inline std::array<Index, 1> unique_neighbours(const Index& idx) const;

                inline std::vector<Index>::const_iterator begin() const;
                inline std::vector<Index>::const_iterator end() const;

                inline const Vector& operator[](const Index& i) const;
                inline const Index& operator[](const Vector& v) const;
        };
    }
}

#include "detail/linear_impl.hpp"

#endif
