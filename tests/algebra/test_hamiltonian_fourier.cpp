#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "hubbard/algebra/hamiltonian_fourier.hpp"
#include "hubbard/discretization/linear.hpp"

using namespace hubbard;
using namespace algebra;

TEST_CASE("commutate_hopping", "")
{
    discretization::LinearDiscretization<double> discretization(100);
    discretization::LinearDiscretization<double> lattice(1000, 1.);
    HamiltonianFourier<Term<Operator<std::size_t, bool>, std::complex<double>>> hamiltonian;

    SECTION("creator")
    {
        auto term =
            hubbard::algebra::make_term(std::complex<double>(1.5, 0.), {make_creator(2ul, true)});
        TermList<decltype(term)> result;
        hamiltonian.commutate_hopping(term, discretization, lattice, result);

        REQUIRE(result[0].prefactor
                == std::complex<double>(-2. * term.prefactor
                                        * std::cos(discretization[term.operators[0].index])));
    }

    SECTION("annihilator")
    {
        auto term = hubbard::algebra::make_term(std::complex<double>(1.5, 0.),
                                                {make_annihilator(2ul, true)});
        TermList<decltype(term)> result;
        hamiltonian.commutate_hopping(term, discretization, lattice, result);

        REQUIRE(result[0].prefactor
                == std::complex<double>(2. * term.prefactor
                                        * std::cos(discretization[term.operators[0].index])));
    }
}

TEST_CASE("commutate_interaction", "")
{
    discretization::LinearDiscretization<double> discretization(100);
    discretization::LinearDiscretization<double> lattice(1000, 1.);
    HamiltonianFourier<Term<Operator<std::size_t, bool>, std::complex<double>>> hamiltonian;

    SECTION("creator")
    {
    }
}
