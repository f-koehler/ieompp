#include <catch.hpp>
#include <ieompp/constants.hpp>
#include <ieompp/lattices/periodic_chain.hpp>
#include <ieompp/models/hubbard/dispersion.hpp>

using namespace ieompp;

TEST_CASE("calculate_1d_fermi_momentum")
{
    const auto pi_half = ieompp::HalfPi<double>::value;
    REQUIRE(ieompp::models::hubbard::calculate_fermi_momentum_1d(0.5) == Approx(pi_half));
    REQUIRE(ieompp::models::hubbard::calculate_fermi_momentum_1d(0.25)
            == Approx(1.047197551196597746154214461093167628065723133125035273658));

    for(double n = 0.; n < 1.; n += 0.0001) {
        REQUIRE(ieompp::models::hubbard::calculate_fermi_momentum_1d(n)
                == Approx(std::acos(1 - 2 * n)));
    }
}

TEST_CASE("dispersion_1d")
{
    const std::vector<uint64_t> sizes = {4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048};
    const std::vector<double> Js      = {0.0, 0.5, 1.0, 1.5, 2.0};

    for(const auto& N : sizes) {
        for(const auto& J : Js) {
            const lattices::PeriodicChain<double, uint64_t> lattice(N, 1.), brillouin_zone(N);
            const auto dispersion = models::hubbard::make_dispersion(brillouin_zone, lattice, J);
            for(const auto& k : brillouin_zone) {
                const auto expected = -2 * J * std::cos(brillouin_zone[k] * lattice.dx());
                REQUIRE(dispersion(k) == Approx(expected));
            }
        }
    }
}
