#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "ieompp/algebra/term.hpp"

using namespace ieompp;
using namespace algebra;

template <typename Operator>
void test_find_first_displaced_operator()
{
    using Index = typename Operator::Index;
    using Spin  = typename Operator::Spin;

    auto term = make_term(std::complex<double>(1.), {make_creator(Index(0), Spin(0))});
    REQUIRE(term.find_first_displaced_operator() == term.operators.end());

    term = make_term(std::complex<double>(1.),
                     {make_creator(Index(1), Spin(0)), make_creator(Index(0), Spin(0))});
    REQUIRE(term.find_first_displaced_operator() == term.operators.begin());

    term = make_term(std::complex<double>(1.),
                     {make_annihilator(Index(0), Spin(0)), make_creator(Index(1), Spin(0)),
                      make_creator(Index(0), Spin(0))});
    REQUIRE(term.find_first_displaced_operator() == term.operators.begin());
}
