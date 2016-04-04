#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "ieompp/discretization/linear.hpp"
using namespace ieompp;
using namespace ieompp::discretization;

const std::size_t N = 100;

template <typename Real>
void test_initialization_real_space()
{
    LinearDiscretization<Real> disc(N, 1.);

    REQUIRE(disc.end() - disc.begin() == N);
    REQUIRE(disc.cend() - disc.cbegin() == N);

    REQUIRE(disc.num() == N);
    REQUIRE(disc.first() == 0);
    REQUIRE(disc.last() == N - 1);
    REQUIRE(disc.x_min() == Approx(0.));
    REQUIRE(disc.x_max() == Approx(Real(N - 1)));
    REQUIRE(disc.x_length() == Approx(Real(N)));
    REQUIRE(disc.dx() == Approx(1.));
    REQUIRE(disc.lattice_vectors().size() == 1);
    REQUIRE(disc.lattice_vectors()[0] == Approx(1.));

    for(std::size_t i = 0; i < N; ++i) {
        REQUIRE(disc.begin()[i] == i);
        REQUIRE(disc[i] == Approx(Real(i)));
        REQUIRE(disc(disc[i]) == i);
    }
}

template <typename Real>
void test_initialization_momentum_space()
{
    LinearDiscretization<Real> disc(N);

    REQUIRE(disc.num() == N);
    REQUIRE(disc.first() == 0);
    REQUIRE(disc.last() == N - 1);
    REQUIRE(disc.x_min() == Approx(-Pi<Real>::value));
    REQUIRE(disc.x_max() == Approx(Pi<Real>::value));
    REQUIRE(disc.x_length() == Approx(TwoPi<Real>::value));
    REQUIRE(disc.dx() == Approx(TwoPi<Real>::value / N));
    REQUIRE(disc.lattice_vectors().size() == 1);
    REQUIRE(disc.lattice_vectors()[0] == Approx(disc.dx()));

    for(std::size_t i = 0; i < N; ++i) {
        REQUIRE(disc.begin()[i] == i);
        REQUIRE(disc[i] == Approx(-Pi<Real>::value + i * disc.dx()));
        REQUIRE(disc(disc[i]) == i);
    }
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

template <typename Real>
void test_closest()
{
    LinearDiscretization<Real> disc(N, 1.);
    REQUIRE(disc.closest(0.49) == 0);
    REQUIRE(disc.closest(0.51) == 1);
    REQUIRE(disc.closest(-0.49) == 0);
    REQUIRE(disc.closest(-0.51) == N - 1);
    REQUIRE(disc.closest(Real(N - 1) + 0.49) == N - 1);
    REQUIRE(disc.closest(Real(N - 1) + 0.51) == 0);
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

TEST_CASE("closest site", "")
{
    test_closest<float>();
    test_closest<double>();
    test_closest<long double>();
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
