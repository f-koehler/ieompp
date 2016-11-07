#ifndef IEOMPP_MODELS_HUBBARD_LIOUVILLIAN_HPP_
#define IEOMPP_MODELS_HUBBARD_LIOUVILLIAN_HPP_

#include "ieompp/models/hubbard/dispersion.hpp"

namespace ieompp
{
    namespace hubbard
    {
        namespace real_space
        {
            template <typename PrefactorT>
            struct Liouvillian {
                using Prefactor = PrefactorT;

                Prefactor J;
                Prefactor U;
            };

            template <typename Prefactor>
            Liouvillian<Prefactor> make_liouvillian(const Prefactor& J = 1.,
                                                    const Prefactor& U = 1.)
            {
                return Liouvillian<Prefactor>{J, U};
            }
        } // namespace real_space

        namespace momentum_space
        {
            template <typename PrefactorT, typename DispersionT>
            struct Liouvillian {
                using Prefactor  = PrefactorT;
                using Dispersion = DispersionT;

                Dispersion dispersion;
                Prefactor U;
            };

            template <typename Prefactor, typename MomentumSpace, typename Lattice>
            Liouvillian<Prefactor,
                        Dispersion<typename MomentumSpace::Float, typename MomentumSpace::Index>>
            make_liouvillian(const MomentumSpace& momentum_space, const Lattice& lattice,
                             const Prefactor& J = 1., const Prefactor& U = 1.)
            {
                return Liouvillian<Prefactor, Dispersion<typename MomentumSpace::Float,
                                                         typename MomentumSpace::Index>>{
                    make_dispersion(momentum_space, lattice, J), U};
            }
        } // namespace momentum_space
    }     // namespace hubbard
} // namespace ieompp

#endif
