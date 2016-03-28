#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "ieompp/discretization/linear.hpp"
using namespace ieompp::discretization;

const std::size_t N = 100;

template <typename Real>
void test_initialization_real_space()
{
    LinearDiscretization<Real> disc(N, 1.);

    REQUIRE(disc.end() - disc.begin() == N);
    REQUIRE(disc.cend() - disc.cbegin() == N);

    for(std::size_t i = 0; i < N; ++i) {
        REQUIRE(disc.begin()[i] == i);
        REQUIRE(disc[i] == Approx(Real(i)));
        REQUIRE(disc[disc[i]] == i);
    }
}

template <typename Real>
void test_initialization_momentum_space()
{
    LinearDiscretization<Real> disc(N);
    // TODO: need more getter functions for these tests

    /* REQUIRE(disc.x_min == Approx(-ieompp::Pi<Real>::value)); */
    /* REQUIRE(disc.x_max == Approx(ieompp::Pi<Real>::value)); */

    /* REQUIRE(disc.dx == Approx(ieompp::TwoPi<Real>::value / n)); */

    /* REQUIRE(disc.sites.front() == Approx(-ieompp::Pi<Real>::value)); */
    /* REQUIRE(disc.sites.back() == Approx(ieompp::Pi<Real>::value - disc.dx)); */
}

template <typename Real>
void test_neighbours()
{
    LinearDiscretization<Real> disc(N, 1.);

    auto neigh = disc.neighbours(0);
    REQUIRE(neigh[0] == N - 1);
    REQUIRE(neigh[1] == 1);
    for(std::size_t i = 1; i < N - 1; ++i) {
        neigh = disc.neighbours(i);
        REQUIRE(neigh[0] == i - 1);
        REQUIRE(neigh[1] == i + 1);
    }
    neigh = disc.neighbours(N - 1);
    REQUIRE(neigh[0] == N - 2);
    REQUIRE(neigh[1] == 0);
}

template <typename Real>
void test_unique_neighbours()
{
    LinearDiscretization<Real> disc(N, 1.);

    auto neigh = disc.unique_neighbours(0);
    REQUIRE(neigh[0] == 1);
    for(std::size_t i = 1; i < N - 1; ++i) {
        neigh = disc.unique_neighbours(i);
        REQUIRE(neigh[0] == i + 1);
    }
    neigh = disc.unique_neighbours(N - 1);
    REQUIRE(neigh[0] == 0);
}

TEST_CASE("initialization (real space)", "")
{
    test_initialization_real_space<float>();
    test_initialization_real_space<double>();
    test_initialization_real_space<long double>();
}

TEST_CASE("initialization (momentum space)", "")
{
    test_initialization_momentum_space<float>();
    test_initialization_momentum_space<double>();
    test_initialization_momentum_space<long double>();
}

TEST_CASE("neighbours", "")
{
    test_neighbours<float>();
    test_neighbours<double>();
    test_neighbours<long double>();
}

TEST_CASE("unique_neighbours")
{
    test_unique_neighbours<float>();
    test_unique_neighbours<double>();
    test_unique_neighbours<long double>();
}
