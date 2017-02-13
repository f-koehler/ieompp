#include <catch.hpp>

#include <ieompp/constants.hpp>
#include <ieompp/lattices/periodic_chain.hpp>
#include <ieompp/models/hubbard/dispersion.hpp>
#include <ieompp/models/hubbard_real_space/expectation_value.hpp>
using namespace ieompp;

TEST_CASE("ExpectationValue1D")
{
    const auto pi = Pi<double>::value;

    const std::vector<double> fillings = {0.1, 0.25, 0.5, 0.75, 0.99};

    for(const auto& n : fillings) {
        const auto k_F = models::hubbard::calculate_fermi_momentum_1d(n);
        for(uint64_t N = 3; N <= 64; ++N) {
            const lattices::PeriodicChain<double, uint64_t> lattice(N, 1.);
            models::hubbard_real_space::ExpectationValue1DHalfFilled<double, decltype(lattice)> ev(
                lattice, n, models::hubbard::calculate_fermi_momentum_1d(n));

            const auto max_dist = N / 2;
            for(auto dist = 1ul; dist <= max_dist; ++dist) {
                const auto lattice_dist = ev.lattice_distance(0, dist);
                CAPTURE(N);
                CAPTURE(k_F);
                const auto val = std::sin(k_F * dist) / (dist * pi);
                REQUIRE(lattice_dist == dist);
                REQUIRE(ev(0, dist) == Approx(val));
                REQUIRE(ev(dist, 0) == Approx(val));
            }
        }
    }
}
