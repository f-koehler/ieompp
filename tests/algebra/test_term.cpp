#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "ieompp/algebra/operator.hpp"
#include "ieompp/algebra/term.hpp"

using namespace ieompp;
using namespace algebra;

#include <iostream>

template <typename Operator>
void test_find_first_displaced_operator()
{
    using Index = typename Operator::Index;
    using Spin  = typename Operator::Spin;

    auto term = make_term(std::complex<double>(1.), {make_creator(Index(0), Spin(0))});
    REQUIRE(term.find_first_displaced_operator() == term.operators.end());

    term = make_term(std::complex<double>(1.),
                     {make_creator(Index(0), Spin(0)), make_creator(Index(0), Spin(0))});
    REQUIRE(term.find_first_displaced_operator() == term.operators.end());

    term = make_term(std::complex<double>(1.),
                     {make_creator(Index(1), Spin(0)), make_creator(Index(0), Spin(0))});
    REQUIRE(term.find_first_displaced_operator() == term.operators.begin());

    term = make_term(std::complex<double>(1.),
                     {make_annihilator(Index(0), Spin(0)), make_creator(Index(1), Spin(0)),
                      make_annihilator(Index(0), Spin(0))});
    REQUIRE(term.find_first_displaced_operator() == term.operators.begin());

    std::swap(term.operators[0], term.operators[1]);
    REQUIRE(term.find_first_displaced_operator() == term.operators.end());
}

template <typename Operator>
void test_effect()
{
    using Index = typename Operator::Index;
    using Spin  = typename Operator::Spin;

    auto effect = make_term(std::complex<double>(1.), {make_creator(Index(0), Spin(0))}).effect();
    REQUIRE(std::get<0>(effect));
    REQUIRE(std::get<1>(effect) == 1);

    effect = make_term(std::complex<double>(1.),
                       {make_creator(Index(0), Spin(0)), make_creator(Index(0), Spin(0))})
                 .effect();
    REQUIRE(std::get<0>(effect));
    REQUIRE(std::get<1>(effect) == 2);

    effect = make_term(std::complex<double>(1.),
                       {make_annihilator(Index(0), Spin(0)), make_creator(Index(1), Spin(0)),
                        make_annihilator(Index(0), Spin(0))})
                 .effect();
    REQUIRE(!std::get<0>(effect));
    REQUIRE(std::get<1>(effect) == 2);
}

TEST_CASE("find_first_displaced_operator")
{
    test_find_first_displaced_operator<Operator<unsigned long, bool>>();
    test_find_first_displaced_operator<Operator<long, bool>>();
}

TEST_CASE("effect")
{
    test_effect<Operator<unsigned long, bool>>();
    test_effect<Operator<long, bool>>();
}
