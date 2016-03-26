#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "ieompp/discretization/linear.hpp"
using namespace ieompp::discretization;

const std::size_t N = 100;

template <typename Real, typename Index>
void test_initialization_real_space()
{
    LinearDiscretization<Real, Index> disc(0, Index(N) - 1, 1.);

    REQUIRE(disc.end()-disc.begin() == Index(N));
    REQUIRE(disc.cend()-disc.cbegin() == Index(N));

    for(Index i = 0; i < Index(N); ++i) {
        REQUIRE(disc.begin()[i] == i);
        /* REQUIRE(*(disc.begin() + i) == i); */
        REQUIRE(disc[i] == Approx(Real(i)));
        REQUIRE(disc[disc[i]] == i);
    }
}

template <typename Real, typename Index>
void test_initialization_momentum_space()
{
    LinearDiscretization<Real, Index> disc(0, Index(N) - 1);
    // TODO: need more getter functions for these tests

    /* REQUIRE(disc.x_min == Approx(-ieompp::Pi<Real>::value)); */
    /* REQUIRE(disc.x_max == Approx(ieompp::Pi<Real>::value)); */

    /* REQUIRE(disc.dx == Approx(ieompp::TwoPi<Real>::value / n)); */

    /* REQUIRE(disc.sites.front() == Approx(-ieompp::Pi<Real>::value)); */
    /* REQUIRE(disc.sites.back() == Approx(ieompp::Pi<Real>::value - disc.dx)); */
}

template <typename Real, typename Index>
void test_neighbours()
{
    LinearDiscretization<Real, Index> disc(0, Index(N) - 1, 1.);

    auto neigh = disc.neighbours(0);
    REQUIRE(neigh[0] == Index(N) - 1);
    REQUIRE(neigh[1] == 1);
    for(Index i = 1; i < Index(N) - 1; ++i) {
        neigh = disc.neighbours(i);
        REQUIRE(neigh[0] == i - 1);
        REQUIRE(neigh[1] == i + 1);
    }
    neigh = disc.neighbours(Index(N) - 1);
    REQUIRE(neigh[0] == Index(N) - 2);
    REQUIRE(neigh[1] == 0);
}

template <typename Real, typename Index>
void test_unique_neighbours()
{
    LinearDiscretization<Real, Index> disc(0, Index(N) - 1, 1.);

    auto neigh = disc.unique_neighbours(0);
    REQUIRE(neigh[0] == 1);
    for(Index i = 1; i < Index(N) - 1; ++i) {
        neigh = disc.unique_neighbours(i);
        REQUIRE(neigh[0] == i + 1);
    }
    neigh = disc.unique_neighbours(Index(N) - 1);
    REQUIRE(neigh[0] == 0);
}

TEST_CASE("initialization (real space)", "")
{
    test_initialization_real_space<float, long>();
    test_initialization_real_space<double, long>();
    test_initialization_real_space<long double, long>();

    test_initialization_real_space<float, unsigned long>();
    test_initialization_real_space<double, unsigned long>();
    test_initialization_real_space<long double, unsigned long>();
}

TEST_CASE("initialization (momentum space)", "")
{
    test_initialization_momentum_space<float, long>();
    test_initialization_momentum_space<double, long>();
    test_initialization_momentum_space<long double, long>();

    test_initialization_momentum_space<float, unsigned long>();
    test_initialization_momentum_space<double, unsigned long>();
    test_initialization_momentum_space<long double, unsigned long>();
}

TEST_CASE("neighbours", "")
{
    test_neighbours<float, long>();
    test_neighbours<double, long>();
    test_neighbours<long double, long>();

    test_neighbours<float, unsigned long>();
    test_neighbours<double, unsigned long>();
    test_neighbours<long double, unsigned long>();
}

TEST_CASE("unique_neighbours")
{
    test_unique_neighbours<float, long>();
    test_unique_neighbours<double, long>();
    test_unique_neighbours<long double, long>();

    test_unique_neighbours<float, unsigned long>();
    test_unique_neighbours<double, unsigned long>();
    test_unique_neighbours<long double, unsigned long>();
}
