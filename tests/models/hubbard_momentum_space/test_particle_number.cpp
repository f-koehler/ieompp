#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <blaze/Blaze.h>

#include <ieompp/algebra/monomial.hpp>
#include <ieompp/algebra/operator.hpp>
#include <ieompp/constants.hpp>
#include <ieompp/lattices/periodic_chain.hpp>
#include <ieompp/models/hubbard_common/dispersion.hpp>
#include <ieompp/models/hubbard_momentum_space/basis.hpp>
#include <ieompp/models/hubbard_momentum_space/particle_number.hpp>
#include <ieompp/types/number.hpp>
using namespace ieompp;

TEST_CASE("half-filled, 1d")
{
    const auto kF_1 = -HalfPi<double>::value, kF_2 = HalfPi<double>::value;
    const std::vector<uint64_t> Ns = {6};

    using Monomial = algebra::Monomial<algebra::Operator<uint64_t, bool>>;

    for(const auto N : Ns) {
        const auto brillouin_zone = lattices::PeriodicChain<double, uint64_t>(N);
        const auto lattice        = lattices::PeriodicChain<double, uint64_t>(N, 1.);
        const auto dispersion =
            models::hubbard_common::make_dispersion(brillouin_zone, lattice, 1.);

        blaze::DynamicVector<std::complex<double>> h(N);
        h[0] = 1.;

        const auto k_tol = brillouin_zone.dx() / 100.;

        for(const auto k_idx : brillouin_zone) {
            const auto k = brillouin_zone[k_idx];
            const models::hubbard_momentum_space::Basis3Operator<Monomial> basis(k_idx,
                                                                                 brillouin_zone);
            const auto conjugate_basis = basis.get_conjugate();
            const models::hubbard_momentum_space::
                ParticleNumber<double, models::hubbard_momentum_space::Basis3Operator<Monomial>>
                    obs(basis, conjugate_basis, dispersion, 0.);

            const auto n = obs(h);

            if((k < (kF_1 - k_tol)) || (k > (kF_2 + k_tol))) {
                CAPTURE(N);
                CAPTURE(k_idx);
                CAPTURE(k);
                CAPTURE(k_tol);
                CAPTURE(kF_1);
                CAPTURE(kF_2);
                CAPTURE(n);
                REQUIRE(types::IsZero(n));
            } else {
                CAPTURE(N);
                CAPTURE(k_idx);
                CAPTURE(k);
                CAPTURE(k_tol);
                CAPTURE(kF_1);
                CAPTURE(kF_2);
                CAPTURE(n);
                CAPTURE(dispersion(k_idx));
                CAPTURE(obs.non_vanishing_expectation_values.front().first);
                CAPTURE(obs.non_vanishing_expectation_values.front().second);
                REQUIRE(types::IsEqual(n, 1.));
            }
        }
    }
}
