#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "ieompp/abstract/parse.hpp"

using namespace ieompp;
using namespace abstract;

TEST_CASE("is_commutator")
{
    auto result = parse::is_commutator("[a;b]");
    REQUIRE(std::get<0>(result));
    REQUIRE(std::get<1>(result) == "a");
    REQUIRE(std::get<2>(result) == "b");
}

TEST_CASE("is_operator")
{
    SECTION("plain ASCII")
    {
        auto result = parse::is_operator(u8"c_{k_1,up}^\\dagger");
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
        auto result = parse::is_operator(u8"c_{k,σ}^†");
        REQUIRE(std::get<0>(result));
        REQUIRE(std::get<1>(result) == "k");
        REQUIRE(std::get<2>(result) == "σ");
        REQUIRE(std::get<3>(result));

        result = parse::is_operator("c_{k,σ}");
        REQUIRE(std::get<0>(result));
        REQUIRE(std::get<1>(result) == "k");
        REQUIRE(std::get<2>(result) == "σ");
        REQUIRE(!std::get<3>(result));
    }

    SECTION("complex indices")
    {
        auto result = parse::is_operator("c_{k+q,σ-s}^†");
        REQUIRE(std::get<0>(result));
        REQUIRE(std::get<1>(result) == "k+q");
        REQUIRE(std::get<2>(result) == "σ-s");
        REQUIRE(std::get<3>(result));

        result = parse::is_operator("c_{k+q,σ-s}");
        REQUIRE(std::get<0>(result));
        REQUIRE(std::get<1>(result) == "k+q");
        REQUIRE(std::get<2>(result) == "σ-s");
        REQUIRE(!std::get<3>(result));
    }
}

TEST_CASE("is_kronecker")
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
        auto result = parse::is_kronecker("δ_{←,→}");
        REQUIRE(std::get<0>(result));
        REQUIRE(std::get<1>(result) == "←");
        REQUIRE(std::get<2>(result) == "→");
    }

    SECTION("complex indices")
    {
        auto result = parse::is_kronecker("δ_{k_1+q,σ-σ'}");
        REQUIRE(std::get<0>(result));
        REQUIRE(std::get<1>(result) == "k_1+q");
        REQUIRE(std::get<2>(result) == "σ-σ'");
    }
}

TEST_CASE("is_number")
{
    auto result = parse::is_number("(12.123e-12,-4.0)");
    REQUIRE(std::get<0>(result));
    REQUIRE(std::get<1>(result) == "12.123e-12");
    REQUIRE(std::get<2>(result) == "-4.0");
}

TEST_CASE("parse_term")
{
    auto expression = "c_{k_1,up}^\\dagger c_{k_2,up}^{\\dagger}";
    auto result = parse::parse_term(expression);
    REQUIRE(result.operators.size() == 2);
}
