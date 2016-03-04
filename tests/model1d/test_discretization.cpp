#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "hubbard/model1d/discretization.hpp"

TEST_CASE("model1d (real space)", "")
{
    using Discretization = hubbard::model1d::Discretization<double>;
    const std::size_t n = 10000;
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
            REQUIRE(disc(disc.indices[i]) == double(i));
        }
    }

    SECTION("closest")
    {
        REQUIRE(disc.closest(0.4) == 0);
        REQUIRE(disc.closest(3.9) == 4);
    }

    SECTION("neighbours")
    {
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

    SECTION("unique_neighbours")
    {
        auto neigh = disc.unique_neighbours(0);
        REQUIRE(neigh[0] == 1);
        for(std::size_t i = 1; i < n - 1; ++i) {
            neigh = disc.unique_neighbours(i);
            REQUIRE(neigh[0] == i + 1);
        }
        neigh = disc.unique_neighbours(n - 1);
        REQUIRE(neigh[0] == 0);
    }

    SECTION("index operator")
    {
        for(auto& idx : disc.indices) REQUIRE(disc(idx) == double(idx));
    }
}
