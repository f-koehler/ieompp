#ifndef IEOMPP_HUBBARD_DISPERSION_HPP_
#define IEOMPP_HUBBARD_DISPERSION_HPP_

#include <cmath>

#include "ieompp/types/dot_product.hpp"

namespace ieompp
{
    namespace hubbard
    {
        template <typename Momentum, typename Lattice, typename Prefactor>
        typename types::scalar_type<Momentum>::type
        dispersion(const Momentum& momentum, const Lattice& lattice, const Prefactor& J = 1.)
        {
            typename types::scalar_type<Momentum>::type val = 0.;
            for(auto& vec : lattice.lattice_vectors())
                val += std::cos(types::dot_product(vec, momentum));
            return -2 * J * val;
        }
    }
}

#endif
