#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <ieompp/lattices/linear.hpp>
#include <ieompp/models/hubbard_common/dispersion.hpp>

using namespace ieompp;

TEST_CASE("dispersion_1d")
{
    const std::vector<uint64_t> sizes = {4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048};
    const std::vector<double> Js      = {0.0, 0.5, 1.0, 1.5, 2.0};

    for(const auto& N : sizes) {
        for(const auto& J : Js) {
            const lattices::LinearDiscretization<double, uint64_t> lattice(N, 1.),
                brillouin_zone(N);
            const auto dispersion =
                models::hubbard_common::make_dispersion(brillouin_zone, lattice, J);
            for(const auto& k : brillouin_zone) {
                const auto expected = -4 * J * std::cos(brillouin_zone[k]);
                REQUIRE(dispersion(k) == Approx(expected).epsilon(0.000000001));
            }
        }
    }
}
