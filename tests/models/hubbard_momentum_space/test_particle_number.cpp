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

void test_particle_number(const uint64_t& N, const uint64_t& k_idx) {
    using Monomial = algebra::Monomial<algebra::Operator<uint64_t, bool>>;

    static const auto kF_1 = -HalfPi<double>::value, kF_2 = HalfPi<double>::value;

    const auto lattice        = lattices::PeriodicChain<double, uint64_t>(N, 1.);
    const auto brillouin_zone = lattices::PeriodicChain<double, uint64_t>(N);
    const auto dispersion = models::hubbard_common::make_dispersion(brillouin_zone, lattice, 1.);

    blaze::DynamicVector<std::complex<double>> h(N);
    h[0] = 1.;

    const auto k_tol = brillouin_zone.dx() / 100.;

    const auto k = brillouin_zone[k_idx];
    const models::hubbard_momentum_space::Basis3Operator<Monomial> basis(k_idx, brillouin_zone);
    const auto conjugate_basis = basis.get_conjugate();
    const auto observable      = models::hubbard_momentum_space::make_particle_number(
        basis, conjugate_basis, dispersion, 0.);
    const auto n = observable(h);

    if((k < (kF_1 - k_tol)) || (k > (kF_2 + k_tol))) {
        CAPTURE(N);
        CAPTURE(n);
        REQUIRE(types::IsZero(n));
    } else {
        CAPTURE(N);
        CAPTURE(n);
        REQUIRE(types::IsEqual(n, 1.));
    }
}

TEST_CASE("half-filled, 1d")
{
    const std::vector<uint64_t> Ns = {3, 5, 6, 7, 8, 9, 10, 15, 16, 23, 24, 32, 35, 42};

    for(const auto N : Ns) {
        const auto brillouin_zone = lattices::PeriodicChain<double, uint64_t>(N);
        for(const auto k_idx : brillouin_zone) {
            test_particle_number(N, k_idx);
        }
    }
}
