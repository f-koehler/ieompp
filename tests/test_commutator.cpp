#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "hubbard/algebra.hpp"

TEST_CASE("anticommutates", "[algebra]") {
    SECTION("creator & creator") {
        auto ret = anticommutates(
            make_creator(0, true),
            make_creator(0, true)
        );
        REQUIRE(ret == true);

        ret = anticommutates(
            make_creator(0, false),
            make_creator(0, true)
        );
        REQUIRE(ret == true);
        
        ret = anticommutates(
            make_creator(1, true),
            make_creator(0, true)
        );
        REQUIRE(ret == true);
    }
    
    SECTION("annihilator & creator") {
        auto ret = anticommutates(
            make_annihilator(0, true),
            make_creator(0, true)
        );
        REQUIRE(ret == false);

        ret = anticommutates(
            make_annihilator(0, false),
            make_creator(0, true)
        );
        REQUIRE(ret == true);
        
        ret = anticommutates(
            make_annihilator(1, true),
            make_creator(0, true)
        );
        REQUIRE(ret == true);
    }
    
    SECTION("annihilator & annihilator") {
        auto ret = anticommutates(
            make_annihilator(0, true),
            make_annihilator(0, true)
        );
        REQUIRE(ret == false);

        ret = anticommutates(
            make_annihilator(0, true),
            make_annihilator(0, true)
        );
        REQUIRE(ret == true);
        
        ret = anticommutates(
            make_annihilator(1, true),
            make_annihilator(0, true)
        );
        REQUIRE(ret == true);
    }
}
