#ifndef IEOMPP_MODELS_HUBBARD_COMMON_DISPERSION_HPP_
#define IEOMPP_MODELS_HUBBARD_COMMON_DISPERSION_HPP_

#include "ieompp/types/dot_product.hpp"
#include "ieompp/types/matrix.hpp"

#include <cmath>
#include <vector>

namespace ieompp
{
    namespace models
    {
        namespace hubbard_common
        {
            template <typename FloatT, typename IndexT>
            class Dispersion
            {
            public:
                using Float = FloatT;
                using Index = IndexT;

            private:
                std::vector<Float> _values;

            public:
                template <typename MomentumSpace, typename Lattice>
                Dispersion(const MomentumSpace& momentum_space, const Lattice& lattice,
                           const Float& J = 1.)
                    : _values(momentum_space.num(), Float(0.))
                {
                    const auto num = momentum_space.num();

                    const auto lattice_vectors = lattice.lattice_vectors();

#pragma omp parallel for
                    for(Index i = 0; i < num; ++i) {
                        const auto& momentum = momentum_space[i];
                        Float val            = 0.;
                        for(const auto& vec : lattice_vectors) {
                            val += types::dot_product(momentum, vec);
                        }
                        _values[i] = -4 * J * val;
                    }
                }

                const Float& operator()(typename std::vector<Float>::size_type idx) const
                {
                    return _values[idx];
                }
            };

            template <typename MomentumSpace, typename Lattice>
            Dispersion<typename MomentumSpace::Float, typename MomentumSpace::Index>
            make_dispersion(const MomentumSpace& momentum_space, const Lattice& lattice,
                            const typename MomentumSpace::Float& J = 1.)
            {
                return Dispersion<typename MomentumSpace::Float, typename MomentumSpace::Index>(
                    momentum_space, lattice, J);
            }
        } // namespace hubbard_common
    }     // namespace models
} // namespace ieompp

#endif
