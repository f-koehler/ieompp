#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "hubbard/abstract/parse.hpp"

using namespace hubbard;
using namespace abstract;

TEST_CASE("is_commutator", "[abstract]")
{
    auto result = parse::is_commutator("[a;b]");
    REQUIRE(std::get<0>(result));
    REQUIRE(std::get<1>(result) == "a");
    REQUIRE(std::get<2>(result) == "b");
}

TEST_CASE("get_operators", "[abstract]")
{
}
