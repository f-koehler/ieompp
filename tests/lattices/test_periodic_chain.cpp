#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <ieompp/lattices/periodic_chain.hpp>
using namespace ieompp;

const std::size_t N = 10000;

static_assert(lattices::PeriodicChain<double>::coordination_number == 2,
              "Linear chain must have coordination number 2");

template <typename Real>
void test_initialization_real_space()
{
    lattices::PeriodicChain<Real> disc(N, 1.);

    REQUIRE(disc.end() - disc.begin() == N);
    REQUIRE(disc.cend() - disc.cbegin() == N);

    REQUIRE(disc.size() == N);
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
    lattices::PeriodicChain<Real> disc(N);

    REQUIRE(disc.size() == N);
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
void test_neighbors()
{
    lattices::PeriodicChain<Real> disc(N, 1.);

    auto neigh = disc.neighbors(0);
    REQUIRE(neigh[0] == N - 1);
    REQUIRE(neigh[1] == 1);
    for(std::size_t i = 1; i < N - 1; ++i) {
        neigh = disc.neighbors(i);
        REQUIRE(neigh[0] == i - 1);
        REQUIRE(neigh[1] == i + 1);
    }
    neigh = disc.neighbors(N - 1);
    REQUIRE(neigh[0] == N - 2);
    REQUIRE(neigh[1] == 0);
}

template <typename Real>
void test_unique_neighbors()
{
    lattices::PeriodicChain<Real> disc(N, 1.);

    auto neigh = disc.unique_neighbors(0);
    REQUIRE(neigh[0] == 1);
    for(std::size_t i = 1; i < N - 1; ++i) {
        neigh = disc.unique_neighbors(i);
        REQUIRE(neigh[0] == i + 1);
    }
    neigh = disc.unique_neighbors(N - 1);
    REQUIRE(neigh[0] == 0);
}

template <typename Real>
void test_closest()
{
    lattices::PeriodicChain<Real> disc(N, 1.);
    REQUIRE(disc.closest(0.49) == 0);
    REQUIRE(disc.closest(0.51) == 1);
    REQUIRE(disc.closest(-0.49) == 0);
    REQUIRE(disc.closest(-0.51) == N - 1);
    REQUIRE(disc.closest(Real(N - 1) + 0.49) == N - 1);
    REQUIRE(disc.closest(Real(N - 1) + 0.51) == 0);
}

TEST_CASE("initialization (real space)")
{
    test_initialization_real_space<float>();
    test_initialization_real_space<double>();
    test_initialization_real_space<long double>();
}

TEST_CASE("initialization (momentum space)")
{
    test_initialization_momentum_space<float>();
    test_initialization_momentum_space<double>();
    test_initialization_momentum_space<long double>();
}

TEST_CASE("closest site")
{
    test_closest<float>();
    test_closest<double>();
    test_closest<long double>();
}

TEST_CASE("neighbors")
{
    test_neighbors<float>();
    test_neighbors<double>();
    test_neighbors<long double>();
}

TEST_CASE("unique_neighbors")
{
    test_unique_neighbors<float>();
    test_unique_neighbors<double>();
    test_unique_neighbors<long double>();
}
