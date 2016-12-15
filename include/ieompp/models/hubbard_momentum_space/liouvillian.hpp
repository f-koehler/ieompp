#ifndef IEOMPP_MODELS_HUBBARD_MOMENTUM_SPACE_LIOUVILLIAN_HPP_
#define IEOMPP_MODELS_HUBBARD_MOMENTUM_SPACE_LIOUVILLIAN_HPP_

#include "ieompp/models/hubbard/dispersion.hpp"

namespace ieompp
{
    namespace models
    {
        namespace hubbard_momentum_space
        {
            template <typename PrefactorT, typename DispersionT>
            struct Liouvillian {
                using Prefactor  = PrefactorT;
                using Dispersion = DispersionT;

                Dispersion dispersion;
                Prefactor U;
            };

            template <typename Prefactor, typename MomentumSpace, typename Lattice>
            Liouvillian<Prefactor, hubbard::Dispersion<typename MomentumSpace::Float>>
            make_liouvillian(const MomentumSpace& momentum_space, const Lattice& lattice,
                             const Prefactor& J = 1., const Prefactor& U = 1.)
            {
                return Liouvillian<Prefactor, hubbard::Dispersion<typename MomentumSpace::Float>>{
                    hubbard::make_dispersion(momentum_space, lattice, J), U};
            }
        } // namespace hubbard_momentum_space
    }     // namespace models
} // namespace ieompp

#endif
