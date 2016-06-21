#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <type_traits>

#include "ieompp/algebra/anticommutator.hpp"
#include "ieompp/algebra/operator.hpp"
#include "ieompp/symbolic/index.hpp"
namespace algebra  = ieompp::algebra;
namespace symbolic = ieompp::symbolic;

TEST_CASE("anticommutates", "[anticommutator]")
{
    SECTION("no symbolic index")
    {
        auto creator1    = algebra::make_creator(0);
        auto creator2    = algebra::make_creator(1);
        auto annihilator = algebra::make_annihilator(0);

        REQUIRE((std::is_same<decltype(algebra::anticommutates(creator1, creator2)), bool>::value));

        REQUIRE(algebra::anticommutates(creator1, creator1));
        REQUIRE(algebra::anticommutates(creator2, creator2));
        REQUIRE(algebra::anticommutates(annihilator, annihilator));

        REQUIRE(algebra::anticommutates(creator1, creator2));
        REQUIRE(algebra::anticommutates(creator2, creator1));
        REQUIRE(!algebra::anticommutates(creator1, annihilator));
        REQUIRE(!algebra::anticommutates(annihilator, creator1));
        REQUIRE(algebra::anticommutates(creator2, annihilator));
        REQUIRE(algebra::anticommutates(annihilator, creator2));
    }

    SECTION("symbolic index")
    {
        using Index      = symbolic::SpaceIndex;
        auto creator1    = algebra::make_creator(Index(0));
        auto creator2    = algebra::make_creator(Index(1));
        auto annihilator = algebra::make_annihilator(Index(0));

        REQUIRE(algebra::anticommutates(creator1, creator1));
        REQUIRE(algebra::anticommutates(creator2, creator2));
        REQUIRE(algebra::anticommutates(annihilator, annihilator));

        REQUIRE(algebra::anticommutates(creator1, creator2));
        REQUIRE(algebra::anticommutates(creator2, creator1));
        REQUIRE(!algebra::anticommutates(creator1, annihilator));
        REQUIRE(!algebra::anticommutates(annihilator, creator1));
        REQUIRE(!algebra::anticommutates(creator2, annihilator));
        REQUIRE(!algebra::anticommutates(annihilator, creator2));
    }
}

TEST_CASE("anticommutator", "[anticommutator]")
{
    SECTION("no symbolic index")
    {
        auto creator     = algebra::make_creator(0);
        auto annihilator = algebra::make_annihilator(0);

        REQUIRE(
            (std::is_same<decltype(algebra::anticommutator(creator, annihilator)), int>::value));

        REQUIRE(anticommutator(creator, annihilator) == 1);
    }

    SECTION("two symbolic indices")
    {
        using Index1 = symbolic::SpaceIndex;
        using Index2 = symbolic::SpinIndex;

        auto creator     = algebra::make_creator(Index1(0), Index2(0));
        auto annihilator = algebra::make_creator(Index1(0), Index2(0));
        auto ac          = algebra::anticommutator(creator, annihilator);
        REQUIRE(ac.empty());

        annihilator = algebra::make_creator(Index1(1), Index2(0));
        ac          = algebra::anticommutator(creator, annihilator);
        REQUIRE(ac.size() == 1);
        REQUIRE(ac.front() == symbolic::Kronecker(Index1(0), Index1(1)));

        annihilator = algebra::make_creator(Index1(0), Index2(1));
        ac          = algebra::anticommutator(creator, annihilator);
        REQUIRE(ac.size() == 1);
        REQUIRE(ac.front() == symbolic::Kronecker(Index2(0), Index2(1)));

        annihilator = algebra::make_creator(Index1(1), Index2(1));
        ac          = algebra::anticommutator(creator, annihilator);
        REQUIRE(ac.size() == 2);
        REQUIRE(ac.front() == symbolic::Kronecker(Index1(0), Index1(1)));
        REQUIRE(ac.back() == symbolic::Kronecker(Index2(0), Index2(1)));
    }
}
