#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "hubbard/discretization/linear.hpp"
using namespace hubbard::discretization;

const std::size_t n  = 100;

template <typename Real>
void test_initialization_real_space()
{
    LinearDiscretization<Real> disc(n, 1.);

    SECTION("initialization")
    {
        REQUIRE(disc.indices.size() == n);
        REQUIRE(disc.sites.size() == n);
        REQUIRE(disc.num_x == n);
        REQUIRE(disc.dx == 1.);
        REQUIRE(disc.x_min == Approx(0.));
        REQUIRE(disc.x_max == Approx(Real(n - 1)));

        for(std::size_t i = 0; i < n; ++i) {
            REQUIRE(disc.indices[i] == i);
            REQUIRE(disc[disc.indices[i]] == Approx(Real(i)));
        }
    }
}

template <typename Real>
void test_initialization_momentum_space()
{
    LinearDiscretization<Real> disc(n);

    REQUIRE(disc.x_min == Approx(-hubbard::Pi<Real>::value));
    REQUIRE(disc.x_max == Approx(hubbard::Pi<Real>::value));

    REQUIRE(disc.dx == Approx(hubbard::TwoPi<Real>::value / n));

    REQUIRE(disc.sites.front() == Approx(-hubbard::Pi<Real>::value));
    REQUIRE(disc.sites.back() == Approx(hubbard::Pi<Real>::value - disc.dx));
}

template <typename Real>
void test_closest_site()
{
    LinearDiscretization<Real> disc(n, 1.);

    REQUIRE(disc.closest(0.4) == 0);
    REQUIRE(disc.closest(3.9) == 4);
}

template <typename Real>
void test_neighbours()
{
    LinearDiscretization<Real> disc(n, 1.);

    auto neigh = disc.neighbours(0);
    REQUIRE(neigh[0] == n - 1);
    REQUIRE(neigh[1] == 1);
    for(std::size_t i = 1; i < n - 1; ++i) {
        neigh = disc.neighbours(i);
        REQUIRE(neigh[0] == i - 1);
        REQUIRE(neigh[1] == i + 1);
    }
    neigh = disc.neighbours(n - 1);
    REQUIRE(neigh[0] == n - 2);
    REQUIRE(neigh[1] == 0);
}

template <typename Real>
void test_unique_neighbours()
{
    LinearDiscretization<Real> disc(n, 1.);

    auto neigh = disc.unique_neighbours(0);
    REQUIRE(neigh[0] == 1);
    for(std::size_t i = 1; i < n - 1; ++i) {
        neigh = disc.unique_neighbours(i);
        REQUIRE(neigh[0] == i + 1);
    }
    neigh = disc.unique_neighbours(n - 1);
    REQUIRE(neigh[0] == 0);
}

template <typename Real>
void test_indexing()
{
    LinearDiscretization<Real> disc(n, 1.);

    SECTION("index operator for Index")
    {
        for(auto& idx : disc.indices) REQUIRE(disc[idx] == Approx(Real(idx)));
    }
    
    SECTION("index operator for vector")
    {
        for(auto& idx : disc.indices) REQUIRE(idx == disc[disc[idx]]);
    }
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
    test_closest_site<float>();
    test_closest_site<double>();
    test_closest_site<long double>();
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


TEST_CASE("indexing", "")
{
    test_indexing<float>();
    test_indexing<double>();
    test_indexing<long double>();
}
