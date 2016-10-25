#ifndef IEOMPP_HUBBARD_DISPERSION_HPP_
#define IEOMPP_HUBBARD_DISPERSION_HPP_

#include <cmath>

#include <ieompp/types/dot_product.hpp>

namespace ieompp
{
    namespace hubbard
    {
        template <typename MomentumT, typename LatticeT>
        struct Dispersion {
            using Momentum = MomentumT;
            using Lattice  = LatticeT;
            using Scalar   = typename types::scalar_type<Momentum>::type;

            Scalar J;

            Scalar operator()(const Momentum& momentum, const Lattice& lattice) const
            {
                Scalar val = 0.;
                for(auto& vec : lattice.lattice_vectors())
                    val += std::cos(types::DotProduct(vec, momentum));
                return -2 * J * val;
            }
        };
    }
}

#endif
