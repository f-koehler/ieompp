#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>
#include "hubbard/algebra/agenda.hpp"
using namespace hubbard::algebra;

template <typename Operator, typename Real>
void test_add_term()
{
    using Complex = std::complex<Real>;
    using Term    = Term<Operator, Complex>;
    using Index   = typename Operator::Index;
    using Spin    = typename Operator::Spin;

    Agenda<Term> agenda;

    // add first term
    auto t    = Term{Complex(1.), {make_creator(Index(0), Spin(true))}};
    auto find = agenda.is_known(t);
    REQUIRE(!std::get<0>(find));

    agenda.add_new_term(t, std::get<1>(find));
    REQUIRE(agenda.terms().size() == 1);
    REQUIRE(agenda.known().size() == 1);
    REQUIRE(agenda.todo().size() == 1);
    REQUIRE(agenda.results().size() == 1);
    REQUIRE(agenda.terms()[0].same_operators(t));
    REQUIRE(*agenda.known().begin() == 0);

    find = agenda.is_known(t);
    REQUIRE(std::get<0>(find));

    // add second term
    t    = Term{Complex(1.), {make_creator(Index(2), Spin(true))}};
    find = agenda.is_known(t);
    REQUIRE(!std::get<0>(find));

    agenda.add_new_term(t, std::get<1>(find));
    REQUIRE(agenda.terms().size() == 2);
    REQUIRE(agenda.known().size() == 2);
    REQUIRE(agenda.todo().size() == 2);
    REQUIRE(agenda.results().size() == 2);
    REQUIRE(agenda.terms()[1].same_operators(t));
    REQUIRE(*std::next(agenda.known().begin(), 1) == 1);

    find = agenda.is_known(t);
    REQUIRE(std::get<0>(find));

    // add third term
    t    = Term{Complex(1.), {make_creator(Index(1), Spin(true))}};
    find = agenda.is_known(t);
    REQUIRE(!std::get<0>(find));

    agenda.add_new_term(t, std::get<1>(find));
    REQUIRE(agenda.terms().size() == 3);
    REQUIRE(agenda.known().size() == 3);
    REQUIRE(agenda.todo().size() == 3);
    REQUIRE(agenda.results().size() == 3);
    REQUIRE(agenda.terms()[2].same_operators(t));
    REQUIRE(*std::next(agenda.known().begin(), 1) == 2);

    find = agenda.is_known(t);
    REQUIRE(std::get<0>(find));
}

TEST_CASE("add_term", "")
{
    test_add_term<Operator<long, bool>, double>();
}
