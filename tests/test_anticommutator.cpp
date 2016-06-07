#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <type_traits>

#include "ieompp/anticommutator.hpp"
#include "ieompp/operator.hpp"
#include "ieompp/symbolic/index.hpp"

TEST_CASE("anticommutates", "[anticommutator]")
{
    SECTION("no symbolic index")
    {
        auto creator1 = ieompp::make_creator(0);
        auto creator2 = ieompp::make_creator(1);
        auto annihilator = ieompp::make_annihilator(0);

        REQUIRE((std::is_same<decltype(ieompp::anticommutates(creator1, creator2)), bool>::value));

        REQUIRE(ieompp::anticommutates(creator1, creator1));
        REQUIRE(ieompp::anticommutates(creator2, creator2));
        REQUIRE(ieompp::anticommutates(annihilator, annihilator));

        REQUIRE(ieompp::anticommutates(creator1, creator2));
        REQUIRE(ieompp::anticommutates(creator2, creator1));
        REQUIRE(!ieompp::anticommutates(creator1, annihilator));
        REQUIRE(!ieompp::anticommutates(annihilator, creator1));
        REQUIRE(ieompp::anticommutates(creator2, annihilator));
        REQUIRE(ieompp::anticommutates(annihilator, creator2));
    }

    SECTION("symbolic index")
    {
        using Index      = ieompp::symbolic::SpaceIndex<int>;
        auto creator1    = ieompp::make_creator(Index(0));
        auto creator2    = ieompp::make_creator(Index(1));
        auto annihilator = ieompp::make_annihilator(Index(0));

        REQUIRE(ieompp::anticommutates(creator1, creator1));
        REQUIRE(ieompp::anticommutates(creator2, creator2));
        REQUIRE(ieompp::anticommutates(annihilator, annihilator));

        REQUIRE(ieompp::anticommutates(creator1, creator2));
        REQUIRE(ieompp::anticommutates(creator2, creator1));
        REQUIRE(!ieompp::anticommutates(creator1, annihilator));
        REQUIRE(!ieompp::anticommutates(annihilator, creator1));
        REQUIRE(!ieompp::anticommutates(creator2, annihilator));
        REQUIRE(!ieompp::anticommutates(annihilator, creator2));
    }
}
