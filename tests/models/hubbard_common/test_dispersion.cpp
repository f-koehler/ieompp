#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <ieompp/lattices/linear.hpp>
#include <ieompp/models/hubbard_common/dispersion.hpp>

using namespace ieompp;

TEST_CASE("dispersion_1d")
{
    const uint64_t N = 128;

    lattices::LinearDiscretization<double, uint64_t> lattice(N, 1.), brillouin_zone(N);
    const auto dispersion = models::hubbard_common::make_dispersion(brillouin_zone, lattice, 1.);
    for(const auto& k : brillouin_zone) {
        const auto expected = -4 * std::cos(brillouin_zone[k]);
        REQUIRE(dispersion(k) == Approx(expected).epsilon(0.000000001));
    }
}
