#include "excited_fermi_sea_1d.hpp"
using namespace ieompp;

TEST_CASE("create_particle (1d, half-filled)")
{
    const auto kF_1 = -HalfPi<double>::value, kF_2 = HalfPi<double>::value;
    const std::vector<uint64_t> Ns = {3, 4, 8, 9, 16, 17, 32, 33, 64, 67, 128, 131, 256, 512, 2123};

    for(const auto N : Ns) {
        const auto brillouin_zone = lattices::PeriodicChain<double, uint64_t>(N);
        const auto lattice        = lattices::PeriodicChain<double, uint64_t>(N, 1.);
        const auto dispersion =
            models::hubbard_common::make_dispersion(brillouin_zone, lattice, 1.);

        for(auto momentum_index : brillouin_zone) {
            const auto momentum = brillouin_zone[momentum_index];
            if((momentum < kF_1) || (momentum > kF_2)) {
                models::hubbard_momentum_space::ExcitedFermiSea<Monomial> state;
                REQUIRE(
                    state.create_particle(std::make_tuple(momentum_index, true), dispersion, 0.));
                REQUIRE(
                    state.create_particle(std::make_tuple(momentum_index, false), dispersion, 0.));
                REQUIRE(state.created_particles.front() == std::make_tuple(momentum_index, true));
                REQUIRE(state.created_particles.back() == std::make_tuple(momentum_index, false));

                REQUIRE(
                    !state.create_particle(std::make_tuple(momentum_index, true), dispersion, 0.));
                REQUIRE(
                    !state.create_particle(std::make_tuple(momentum_index, false), dispersion, 0.));
            } else {
                models::hubbard_momentum_space::ExcitedFermiSea<Monomial> state;
                REQUIRE(
                    !state.create_particle(std::make_tuple(momentum_index, true), dispersion, 0.));
                REQUIRE(!state.vanishes);
                REQUIRE(
                    !state.create_particle(std::make_tuple(momentum_index, false), dispersion, 0.));
            }
        }
    }
}
