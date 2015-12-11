#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "hubbard/algebra.hpp"

TEST_CASE("anticommutator", "[algebra] [operator]") {
    using Operator = Operator<int, bool>;
    SECTION("creator & creator") {
        Operator a(true, 0, true), b(true, 0, true);
        REQUIRE(Operator::anticommutator(a, b) == 0);

        a = Operator(true, 0, false);
        REQUIRE(Operator::anticommutator(a, b) == 0);
        
        a = Operator(true, 1, true);
        REQUIRE(Operator::anticommutator(a, b) == 0);
    }
    
    SECTION("annihilator & annihilator") {
        Operator a(false, 0, true), b(false, 0, true);
        REQUIRE(Operator::anticommutator(a, b) == 0);

        a = Operator(false, 0, false);
        REQUIRE(Operator::anticommutator(a, b) == 0);
        
        a = Operator(false, 1, true);
        REQUIRE(Operator::anticommutator(a, b) == 0);
    }
    
    SECTION("creator & annihilator") {
        Operator a(true, 0, true), b(false, 0, true);
        REQUIRE(Operator::anticommutator(a, b) == 1);

        a = Operator(true, 0, false);
        REQUIRE(Operator::anticommutator(a, b) == 0);
        
        a = Operator(true, 1, true);
        REQUIRE(Operator::anticommutator(a, b) == 0);
    }
}
