#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "hubbard/model1d/discretization.hpp"

TEST_CASE("model1d (real space)", "") {
    using Discretization = hubbard::model1d::Discretization<double>;
    const std::size_t n = 10;
    Discretization disc(n, 1.);

    SECTION("initialization") {
        REQUIRE(disc.indices.size() == n);
        REQUIRE(disc.sites.size() == n);
        REQUIRE(disc.num_x == n);
        REQUIRE(disc.dx == 1.);
        REQUIRE(disc.x_min == 0.);
        REQUIRE(disc.x_max == double(n-1));

        for(std::size_t i = 0; i < n; ++i) {
            REQUIRE(disc.indices[i] == i);
            REQUIRE(disc(disc.indices[i]) == double(i));
        }
    }

    SECTION("closest") {
        REQUIRE(disc.closest(0.4) == 0);
        REQUIRE(disc.closest(3.9) == 4);
    }

    SECTION("index operator") {
        for(auto& idx : disc.indices) REQUIRE(disc(idx) == double(idx));
    }
}
