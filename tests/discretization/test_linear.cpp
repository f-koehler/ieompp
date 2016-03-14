#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "hubbard/discretization/linear.hpp"

using Discretization = hubbard::discretization::LinearDiscretization<double>;
const std::size_t n  = 100;

TEST_CASE("initialization (real space)", "")
{
    Discretization disc(n, 1.);

    SECTION("initialization")
    {
        REQUIRE(disc.indices.size() == n);
        REQUIRE(disc.sites.size() == n);
        REQUIRE(disc.num_x == n);
        REQUIRE(disc.dx == 1.);
        REQUIRE(disc.x_min == 0.);
        REQUIRE(disc.x_max == double(n - 1));

        for(std::size_t i = 0; i < n; ++i) {
            REQUIRE(disc.indices[i] == i);
            REQUIRE(disc[disc.indices[i]] == double(i));
        }
    }
}

TEST_CASE("initialization (momentum space)", "")
{
    Discretization disc(n);

    REQUIRE(disc.x_min == -hubbard::Pi<double>::value);
    REQUIRE(disc.x_max == hubbard::Pi<double>::value);

    REQUIRE(disc.sites.front() == -hubbard::Pi<double>::value);
    REQUIRE(disc.sites.back() == hubbard::Pi<double>::value);

    REQUIRE(disc.dx == hubbard::TwoPi<double>::value / (n - 1));
}

TEST_CASE("closest site", "")
{
    Discretization disc(n, 1.);

    REQUIRE(disc.closest(0.4) == 0);
    REQUIRE(disc.closest(3.9) == 4);
}

TEST_CASE("neighbours", "")
{
    Discretization disc(n, 1.);

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

TEST_CASE("unique_neighbours")
{
    Discretization disc(n, 1.);

    auto neigh = disc.unique_neighbours(0);
    REQUIRE(neigh[0] == 1);
    for(std::size_t i = 1; i < n - 1; ++i) {
        neigh = disc.unique_neighbours(i);
        REQUIRE(neigh[0] == i + 1);
    }
    neigh = disc.unique_neighbours(n - 1);
    REQUIRE(neigh[0] == 0);
}


TEST_CASE("indexing", "")
{
    Discretization disc(n, 1.);

    SECTION("index operator for Index")
    {
        for(auto& idx : disc.indices) REQUIRE(disc[idx] == double(idx));
    }
    
    SECTION("index operator for vector")
    {
        for(auto& idx : disc.indices) REQUIRE(idx == disc[disc[idx]]);
    }
}
