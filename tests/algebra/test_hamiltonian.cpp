#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "hubbard/algebra/hamiltonian.hpp"
#include "hubbard/discretization/linear.hpp"

using namespace hubbard;
using namespace algebra;

TEST_CASE("commutate_hopping", "")
{
    discretization::LinearDiscretization<double> discretization(1000, 1.);
    Hamiltonian<hubbard::algebra::Term<Operator<std::size_t, bool>, std::complex<double>>>
        hamiltonian;

    SECTION("creator")
    {
        auto term =
            hubbard::algebra::make_term(std::complex<double>(1., 0.), {make_creator(0ul, true)});
        TermList<decltype(term)> result;
        hamiltonian.commutate_hopping(term, discretization, result);

        REQUIRE(result[0] == make_term(std::complex<double>(-1., 0.), {make_creator(1ul, true)}));
        REQUIRE(result[1] == make_term(std::complex<double>(-1., 0.), {make_creator(999ul, true)}));
    }

    SECTION("annihilator")
    {
        auto term = hubbard::algebra::make_term(std::complex<double>(1., 0.),
                                                {make_annihilator(0ul, true)});
        TermList<decltype(term)> result;
        hamiltonian.commutate_hopping(term, discretization, result);

        REQUIRE(result[0] == make_term(std::complex<double>(1., 0.), {make_annihilator(1ul, true)}));
        REQUIRE(result[1] == make_term(std::complex<double>(1., 0.), {make_annihilator(999ul, true)}));
    }
}

TEST_CASE("commutate_interaction", "")
{
    discretization::LinearDiscretization<double> discretization(1000, 1.);
    Hamiltonian<hubbard::algebra::Term<Operator<std::size_t, bool>, std::complex<double>>>
        hamiltonian;

    SECTION("creator")
    {
        auto term =
            hubbard::algebra::make_term(std::complex<double>(1., 0.), {make_creator(0ul, true)});
        TermList<decltype(term)> result;
        hamiltonian.commutate_interaction(term, discretization, result);

        REQUIRE(result[0] == make_term(std::complex<double>(1., 0.),
                                       {make_creator(0ul, true), make_creator(0ul, false),
                                        make_annihilator(0ul, false)}));
        REQUIRE(result[1] == make_term(std::complex<double>(-0.5, 0.), {make_creator(0ul, true)}));
    }

    SECTION("annihilator")
    {
        auto term = hubbard::algebra::make_term(std::complex<double>(1., 0.),
                                                {make_annihilator(0ul, true)});
        TermList<decltype(term)> result;
        hamiltonian.commutate_interaction(term, discretization, result);

        REQUIRE(result[0] == make_term(std::complex<double>(-1., 0.),
                                       {make_annihilator(0ul, true), make_creator(0ul, false),
                                        make_annihilator(0ul, false)}));
        REQUIRE(result[1]
                == make_term(std::complex<double>(0.5, 0.), {make_annihilator(0ul, true)}));
    }
}
