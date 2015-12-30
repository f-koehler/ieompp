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

TEST_CASE("is_operator", "[abstract]")
{
    SECTION("plain ASCII")
    {
        auto result = parse::is_operator("c_{k_1,up}^\\dagger");
        REQUIRE(std::get<0>(result));
        REQUIRE(std::get<1>(result) == "k_1");
        REQUIRE(std::get<2>(result) == "up");
        REQUIRE(std::get<3>(result));

        result = parse::is_operator("c_{k,\\spinU}");
        REQUIRE(std::get<0>(result));
        REQUIRE(std::get<1>(result) == "k");
        REQUIRE(std::get<2>(result) == "\\spinU");
        REQUIRE(!std::get<3>(result));
    }

    SECTION("unicode")
    {
        auto result = parse::is_operator("c_{k,\u2191}^\u2020");
        REQUIRE(std::get<0>(result));
        REQUIRE(std::get<1>(result) == "k");
        REQUIRE(std::get<2>(result) == "\u2191");
        REQUIRE(std::get<3>(result));

        result = parse::is_operator("c_{k,\u2193}");
        REQUIRE(std::get<0>(result));
        REQUIRE(std::get<1>(result) == "k");
        REQUIRE(std::get<2>(result) == "\u2193");
        REQUIRE(!std::get<3>(result));
    }
}

TEST_CASE("is_kronecker", "[abstract]")
{
    SECTION("plain ASCII")
    {
        auto result = parse::is_kronecker("\\delta_{k_1,k_2}");
        REQUIRE(std::get<0>(result));
        REQUIRE(std::get<1>(result) == "k_1");
        REQUIRE(std::get<2>(result) == "k_2");
    }

    SECTION("unicode")
    {
        auto result = parse::is_kronecker("\u03b4_{\u2191,\u2193}");
        REQUIRE(std::get<0>(result));
        REQUIRE(std::get<1>(result) == "\u2191");
        REQUIRE(std::get<2>(result) == "\u2193");
    }
}

TEST_CASE("parse_term", "[abstract]")
{
    auto expression = "c_{k_1,up}^\\dagger c_{k_2,up}^{\\dagger}";
    auto result = parse::parse_term(expression);
    REQUIRE(result.operators.size() == 2);
}
