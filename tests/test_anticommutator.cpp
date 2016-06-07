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
        auto creator1    = ieompp::make_creator(0);
        auto creator2    = ieompp::make_creator(1);
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

TEST_CASE("anticommutator", "[anticommutator]")
{
    SECTION("no symbolic index")
    {
        auto creator     = ieompp::make_creator(0);
        auto annihilator = ieompp::make_annihilator(0);

        REQUIRE((std::is_same<decltype(ieompp::anticommutator(creator, annihilator)), int>::value));

        REQUIRE(anticommutator(creator, annihilator) == 1);
    }

    SECTION("two symbolic indices")
    {
        using Index1 = ieompp::symbolic::SpaceIndex<int>;
        using Index2 = ieompp::symbolic::SpinIndex<int>;

        auto creator     = ieompp::make_creator(Index1(0), Index2(0));
        auto annihilator = ieompp::make_creator(Index1(0), Index2(0));
        auto ac = ieompp::anticommutator(creator, annihilator);
        REQUIRE(ac.empty());

        annihilator = ieompp::make_creator(Index1(1), Index2(0));
        ac = ieompp::anticommutator(creator, annihilator);
        REQUIRE(ac.size() == 1);
        REQUIRE(ac.front() == ieompp::symbolic::Kronecker(Index1(0), Index1(1)));

        annihilator = ieompp::make_creator(Index1(0), Index2(1));
        ac = ieompp::anticommutator(creator, annihilator);
        REQUIRE(ac.size() == 1);
        REQUIRE(ac.front() == ieompp::symbolic::Kronecker(Index2(0), Index2(1)));

        annihilator = ieompp::make_creator(Index1(1), Index2(1));
        ac = ieompp::anticommutator(creator, annihilator);
        REQUIRE(ac.size() == 2);
        REQUIRE(ac.front() == ieompp::symbolic::Kronecker(Index1(0), Index1(1)));
        REQUIRE(ac.back() == ieompp::symbolic::Kronecker(Index2(0), Index2(1)));
    }
}
