#include <catch.hpp>

#include <ieompp/constants.hpp>
#include <ieompp/lattices/periodic_chain.hpp>
#include <ieompp/models/hubbard/dispersion.hpp>
#include <ieompp/models/hubbard_real_space/expectation_value.hpp>
using namespace ieompp;

TEST_CASE("ExpectationValue1DHalfFilled (explicit)")
{
    const auto pi = Pi<double>::value;
    const lattices::PeriodicChain<double, uint64_t> lattice(5, 1.);
    models::hubbard_real_space::ExpectationValue1DHalfFilled<double, decltype(lattice)> ev(
        lattice, 0.5, models::hubbard::calculate_fermi_momentum_1d(0.5));

    REQUIRE(ev(0, 0) == Approx(0.5));
    REQUIRE(ev(1, 1) == Approx(0.5));
    REQUIRE(ev(2, 2) == Approx(0.5));
    REQUIRE(ev(3, 3) == Approx(0.5));
    REQUIRE(ev(4, 4) == Approx(0.5));

    double val = 1. / pi;
    REQUIRE(ev(0, 1) == Approx(val));
    REQUIRE(ev(1, 0) == Approx(val));

    REQUIRE(ev(1, 2) == Approx(val));
    REQUIRE(ev(2, 1) == Approx(val));

    REQUIRE(ev(2, 3) == Approx(val));
    REQUIRE(ev(3, 2) == Approx(val));

    REQUIRE(ev(3, 4) == Approx(val));
    REQUIRE(ev(4, 3) == Approx(val));

    REQUIRE(ev(4, 0) == Approx(val));
    REQUIRE(ev(0, 4) == Approx(val));

    val = 0.;
    REQUIRE(ev(0, 2) == Approx(val));
    REQUIRE(ev(2, 0) == Approx(val));

    REQUIRE(ev(1, 3) == Approx(val));
    REQUIRE(ev(3, 1) == Approx(val));

    REQUIRE(ev(2, 4) == Approx(val));
    REQUIRE(ev(4, 2) == Approx(val));

    REQUIRE(ev(3, 0) == Approx(val));
    REQUIRE(ev(0, 3) == Approx(val));

    REQUIRE(ev(4, 1) == Approx(val));
    REQUIRE(ev(1, 4) == Approx(val));
}

TEST_CASE("ExpectationValue1DHalfFilled")
{
    const auto pi  = Pi<double>::value;
    const auto k_F = models::hubbard::calculate_fermi_momentum_1d(.5);

    for(uint64_t N = 3; N <= 512; ++N) {
        const lattices::PeriodicChain<double, uint64_t> lattice(N, 1.);
        models::hubbard_real_space::ExpectationValue1DHalfFilled<double, decltype(lattice)> ev(
            lattice, .5, models::hubbard::calculate_fermi_momentum_1d(0.5));

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
