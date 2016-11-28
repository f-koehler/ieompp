#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <ieompp/algebra/monomial.hpp>
#include <ieompp/algebra/operator.hpp>
#include <ieompp/lattices/periodic_chain.hpp>
#include <ieompp/models/hubbard_common/dispersion.hpp>
#include <ieompp/models/hubbard_momentum_space/excited_fermi_sea.hpp>

using namespace ieompp;

TEST_CASE("single_creator")
{
    using Monomial   = algebra::Monomial<algebra::Operator<uint64_t, bool>>;
    const uint64_t N = 128, N1 = N / 4 + 1, N2 = (N - N / 4) + 1;

    const lattices::PeriodicChain<double, uint64_t> brillouin_zone(N), lattice(N, 1.);
    const auto dispersion = models::hubbard_common::make_dispersion(brillouin_zone, lattice, 1.);

    std::vector<Monomial> monomials;
    monomials.reserve(brillouin_zone.size());

    for(const auto& k : brillouin_zone) {
        monomials.emplace_back(Monomial{{{true, k, true}}});
    }

    for(uint64_t k = 0; k < 1; ++k) {
        const models::hubbard_momentum_space::ExcitedFermiSea<Monomial> state(monomials[k],
                                                                              dispersion, 0.);
        REQUIRE(state.vanishes);
    }

    /* for(uint64_t k = N1; k < N2; ++k) { */
    /*     const models::hubbard_momentum_space::ExcitedFermiSea<Monomial> state(monomials[k], */
    /*                                                                           dispersion, 0.); */
    /*     BOOST_CHECK(!state.vanishes); */
    /* } */

    /* for(uint64_t k = N2; k < N; ++k) { */
    /*     const models::hubbard_momentum_space::ExcitedFermiSea<Monomial> state(monomials[k], */
    /*                                                                           dispersion, 0.); */
    /*     REQUIRE(state.vanishes); */
    /* } */
}
