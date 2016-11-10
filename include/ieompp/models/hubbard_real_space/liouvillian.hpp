#ifndef IEOMPP_MODELS_HUBBARD_MOMENTUM_SPACE_LIOUVILLIAN_HPP_
#define IEOMPP_MODELS_HUBBARD_MOMENTUM_SPACE_LIOUVILLIAN_HPP_

#include "ieompp/models/hubbard_common/dispersion.hpp"

namespace ieompp
{
    namespace models
    {
        namespace hubbard_real_space
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
        } // namespace hubbard_real_space
    }     // namespace models
} // namespace ieompp

#endif
