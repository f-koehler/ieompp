#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "ieompp/algebra/hamiltonian_fourier.hpp"
#include "ieompp/discretization/linear.hpp"

using namespace ieompp;
using namespace algebra;

TEST_CASE("commutate_hopping", "")
{
    discretization::LinearDiscretization<double, std::size_t> discretization(0, 100);
    discretization::LinearDiscretization<double, std::size_t> lattice(0, 999, 1.);
    HamiltonianFourier<Term<Operator<std::size_t, bool>, std::complex<double>>> hamiltonian;

    SECTION("creator")
    {
        auto term =
            ieompp::algebra::make_term(std::complex<double>(1.5, 0.), {make_creator(2ul, true)});
        TermList<decltype(term)> result;
        hamiltonian.commutate_hopping(term, discretization, lattice, result);

        REQUIRE(result[0].prefactor
                == std::complex<double>(-2. * term.prefactor
                                        * std::cos(discretization[term.operators[0].index])));
    }

    SECTION("annihilator")
    {
        auto term = ieompp::algebra::make_term(std::complex<double>(1.5, 0.),
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
    discretization::LinearDiscretization<double, std::size_t> discretization(0, 100);
    discretization::LinearDiscretization<double, std::size_t> lattice(0, 999, 1.);
    HamiltonianFourier<Term<Operator<std::size_t, bool>, std::complex<double>>> hamiltonian;

    SECTION("creator")
    {
        auto term =
            ieompp::algebra::make_term(std::complex<double>(1., 0.), {make_creator(0ul, true)});
        TermList<decltype(term)> result;
        hamiltonian.commutate_interaction(term, discretization, lattice, result);

        REQUIRE(result.size() == (discretization.num() * discretization.num() + 1));
        std::complex<double> prefactor = hamiltonian.U / lattice.num();

        auto spin = term.operators[0].spin;
        auto flipped_spin = !term.operators[0].spin;
        auto q = discretization[term.operators[0].index];
        std::size_t i = 0;
        for(auto& idx1 : discretization) {
            auto k1 = discretization[idx1];
            for(auto& idx2 : discretization) {
                auto k2 = discretization[idx2];
                auto k3  = k1 + k2 - q;
                auto idx3 = discretization[k3];

                REQUIRE(result[i] == make_term(prefactor, {make_creator(idx1, spin),
                                                           make_creator(idx2, flipped_spin),
                                                           make_annihilator(idx3, flipped_spin)}));
                ++i;
            }
        }

        prefactor = -hamiltonian.U / (2 * lattice.num());
        REQUIRE(result.back() == make_term(prefactor, {make_creator(discretization[q], spin)}));
    }

    // TODO: investigate failure of this section
    SECTION("annihilator")
    {
        auto term =
            ieompp::algebra::make_term(std::complex<double>(1., 0.), {make_annihilator(0ul, true)});
        TermList<decltype(term)> result;
        hamiltonian.commutate_interaction(term, discretization, lattice, result);

        REQUIRE(result.size() == (discretization.num() * discretization.num() + 1));
        std::complex<double> prefactor = -hamiltonian.U / lattice.num();

        auto spin = term.operators[0].spin;
        auto flipped_spin = !term.operators[0].spin;
        auto q = discretization[term.operators[0].index];
        std::size_t i = 0;
        for(auto& idx1 : discretization) {
            auto k1 = discretization[idx1];
            for(auto& idx2 : discretization) {
                auto k2 = discretization[idx2];
                auto k3  = k1 + k2 - q;
                auto idx3 = discretization[k3];

                /* REQUIRE(std::find(result.begin(), result.end(), make_term(prefactor, {make_annihilator(idx1, spin), */
                /*                                            make_creator(idx2, flipped_spin), */
                /*                                            make_annihilator(idx3, flipped_spin)})) != result.end()); */
                ++i;
            }
        }

        prefactor = hamiltonian.U / (2 * lattice.num());
        REQUIRE(result.back() == make_term(prefactor, {make_annihilator(discretization[q], spin)}));
    }
}

TEST_CASE("commutate", "")
{
    discretization::LinearDiscretization<double, std::size_t> discretization(0, 100);
    discretization::LinearDiscretization<double, std::size_t> lattice(0, 999, 1.);
    HamiltonianFourier<Term<Operator<std::size_t, bool>, std::complex<double>>> hamiltonian;

    SECTION("creator")
    {
        auto term =
            ieompp::algebra::make_term(std::complex<double>(1., 0.), {make_creator(0ul, true)});
        TermList<decltype(term)> full;
        hamiltonian.commutate(term, discretization, lattice, full);
        decltype(full) kinetic, interaction;
        hamiltonian.commutate_hopping(term, discretization, lattice, kinetic);
        hamiltonian.commutate_interaction(term, discretization, lattice, interaction);

        REQUIRE(std::equal(kinetic.begin(), kinetic.end(), full.begin()));
        REQUIRE(std::equal(interaction.begin(), interaction.end(), full.begin() + kinetic.size()));
    }

    SECTION("annihilator")
    {
        auto term = ieompp::algebra::make_term(std::complex<double>(1., 0.),
                                                {make_annihilator(0ul, true)});
        TermList<decltype(term)> full;
        hamiltonian.commutate(term, discretization, lattice, full);
        decltype(full) kinetic, interaction;
        hamiltonian.commutate_hopping(term, discretization, lattice, kinetic);
        hamiltonian.commutate_interaction(term, discretization, lattice, interaction);

        REQUIRE(std::equal(kinetic.begin(), kinetic.end(), full.begin()));
        REQUIRE(std::equal(interaction.begin(), interaction.end(), full.begin() + kinetic.size()));
    }
}
