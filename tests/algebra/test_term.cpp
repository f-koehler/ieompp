#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "hubbard/algebra/operator.hpp"
#include "hubbard/algebra/term.hpp"
#include <iostream>

using namespace hubbard;
using namespace algebra;

TEST_CASE("Term<>::ordered()", "[algebra]")
{
    auto factor = Complex(1., 0.);

    SECTION("ordered terms")
    {
        auto term = make_term(factor, {make_creator(0, true), make_annihilator(0, true)});
        REQUIRE(term.ordered());
        term = make_term(factor, {make_creator(0, true), make_creator(0, false),
                                  make_annihilator(0, true), make_annihilator(0, false)});
        REQUIRE(term.ordered());
    }
    
    SECTION("unordered terms")
    {
        auto term = make_term(factor, {make_annihilator(0, true), make_creator(0, true)});
        REQUIRE(!term.ordered());
        term = make_term(factor, {make_annihilator(0, true), make_creator(0, false),
                                  make_annihilator(0, true), make_annihilator(0, false)});
        REQUIRE(!term.ordered());
    }
}

TEST_CASE("find_unordered_operator", "[algebra]")
{
    auto factor = Complex(1., 0.);
    auto term   = make_term(factor, {make_creator(0, true), make_annihilator(0, true)});
    REQUIRE(find_unordered_operator(term) == term.operators.end());

    term = make_term(factor, {make_annihilator(0, true), make_creator(0, true)});
    REQUIRE(find_unordered_operator(term) == term.operators.begin());

    term = make_term(factor,
                     {make_creator(0, true), make_annihilator(0, true), make_creator(0, true)});
    REQUIRE(find_unordered_operator(term) == term.operators.begin() + 1);
}
