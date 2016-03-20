#ifndef IEOMPP_DISCRETIZATION_LINEAR_HPP_
#define IEOMPP_DISCRETIZATION_LINEAR_HPP_

#include <vector>
#include <array>

#include "ieompp/constants.hpp"

namespace ieompp
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
                std::vector<Index> init_indices();
                std::vector<Vector> init_sites();

            public:
                const std::size_t num;
                const std::size_t num_x;
                const Real dx;
                const Real x_min, x_max, x_diff;
                const std::array<Vector, 1> lattice_vectors;
                const std::vector<Index> indices;
                const std::vector<Real> sites;

                LinearDiscretization(const std::size_t n, const Real& delta_x);
                LinearDiscretization(const std::size_t n);

                const Index& closest(const Vector& v) const;
                inline std::array<Index, 2> neighbours(const Index& idx) const;
                inline std::array<Index, 1> unique_neighbours(const Index& idx) const;
                Vector project(Vector v) const;

                inline std::vector<Index>::const_iterator begin() const;
                inline std::vector<Index>::const_iterator end() const;

                inline const Vector& operator[](const Index& i) const;
                inline const Index& operator[](const Vector& v) const;
        };
    }
}

#include "detail/linear_impl.hpp"

#endif
