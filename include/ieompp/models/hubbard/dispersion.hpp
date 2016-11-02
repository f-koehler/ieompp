#ifndef IEOMPP_HUBBARD_DISPERSION_HPP_
#define IEOMPP_HUBBARD_DISPERSION_HPP_

#include <cmath>
#include <vector>

#include "ieompp/types/dot_product.hpp"
#include "ieompp/types/matrix.hpp"

namespace ieompp
{
    namespace hubbard
    {
        template <typename Lattice>
        class Dispersion
        {
        public:
            using Float = typename types::ScalarType<typename Lattice::Vector>::type;
            using Index = typename Lattice::Index;

        private:
            std::vector<Float> _values;

        public:
            Dispersion(const Lattice& momentum_space, const Lattice& lattice, const Float J = 1.)
                : _values(momentum_space.num(), Float(0.))
            {
                const auto num = momentum_space.num();

#pragma omp parallel for
                for(Index i = 0; i < num; ++i) {
                    const auto& momentum = momentum_space[i];
                    Float val            = 0.;
                    for(const auto& vec : lattice.lattice_vectors) {
                        val += types::dot_product(momentum, vec);
                    }
                    _values[i] = -4 * J * val;
                }
            }
        };
    }
}

#endif
