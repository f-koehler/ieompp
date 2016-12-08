#include "excited_fermi_sea_1d.hpp"
using namespace ieompp;

TEST_CASE("particle_number (1d, half-filled)")
{
    const auto kF_1 = -HalfPi<double>::value, kF_2 = HalfPi<double>::value;
    const std::vector<uint64_t> Ns = {3, 6, 7, 8, 9, 15, 16, 32, 33, 42, 63};

    for(const auto N : Ns) {
        const auto brillouin_zone = lattices::PeriodicChain<double, uint64_t>(N);
        const auto lattice        = lattices::PeriodicChain<double, uint64_t>(N, 1.);
        const auto dispersion =
            models::hubbard_common::make_dispersion(brillouin_zone, lattice, 1.);

        const auto k_tol = brillouin_zone.dx() / 10.;

        for(auto momentum_index : brillouin_zone) {
            const auto momentum = brillouin_zone[momentum_index];
            if((momentum < (kF_1 - k_tol)) || (momentum > (kF_2 + k_tol))) {
                models::hubbard_momentum_space::ExcitedFermiSea<Monomial> state;
                REQUIRE(!state.annihilate_particle(std::make_tuple(momentum_index, true),
                                                   dispersion, 0.));
                REQUIRE(
                    !state.create_particle(std::make_tuple(momentum_index, true), dispersion, 0.));
            } else {
                models::hubbard_momentum_space::ExcitedFermiSea<Monomial> state;
                REQUIRE(state.annihilate_particle(std::make_tuple(momentum_index, true), dispersion,
                                                  0.));
                REQUIRE(
                    state.create_particle(std::make_tuple(momentum_index, true), dispersion, 0.));
                REQUIRE(!state.vanishes);
                REQUIRE(state.created_particles.empty());
                REQUIRE(state.annihilated_particles.empty());
            }
        }
    }
}

TEST_CASE("on-site correlation (1d, half-filled)")
{
    const auto kF_1 = -HalfPi<double>::value, kF_2 = HalfPi<double>::value;
    const std::vector<uint64_t> Ns = {3, 6, 7, 8, 9, 15, 16, 32, 33, 42, 63};

    for(const auto N : Ns) {
        const auto brillouin_zone = lattices::PeriodicChain<double, uint64_t>(N);
        const auto lattice        = lattices::PeriodicChain<double, uint64_t>(N, 1.);
        const auto dispersion =
            models::hubbard_common::make_dispersion(brillouin_zone, lattice, 1.);

        const auto k_tol = brillouin_zone.dx() / 10.;

        for(auto momentum_index : brillouin_zone) {
            const auto momentum = brillouin_zone[momentum_index];
            if((momentum < (kF_1 - k_tol)) || (momentum > (kF_2 + k_tol))) {
                models::hubbard_momentum_space::ExcitedFermiSea<Monomial> state;
                REQUIRE(
                    state.create_particle(std::make_tuple(momentum_index, true), dispersion, 0.));
                REQUIRE(state.annihilate_particle(std::make_tuple(momentum_index, true), dispersion,
                                                  0.));
                REQUIRE(!state.vanishes);
                REQUIRE(state.created_particles.empty());
                REQUIRE(state.annihilated_particles.empty());
            } else {
                models::hubbard_momentum_space::ExcitedFermiSea<Monomial> state;
                REQUIRE(
                    !state.create_particle(std::make_tuple(momentum_index, true), dispersion, 0.));
                REQUIRE(!state.annihilate_particle(std::make_tuple(momentum_index, true),
                                                   dispersion, 0.));
            }
        }
    }
}
