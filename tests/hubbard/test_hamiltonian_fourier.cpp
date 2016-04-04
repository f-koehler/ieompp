#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>

#include "ieompp/hubbard/hamiltonian_fourier.hpp"
#include "ieompp/discretization/linear.hpp"

using namespace ieompp;
using namespace algebra;
using namespace hubbard;

template <typename Operator, typename Real>
void test_commutate_hopping()
{
    using Complex  = std::complex<Real>;
    using Index    = typename Operator::Index;
    using Spin     = typename Operator::Spin;
    using Term     = Term<Operator, Complex>;
    using TermList = TermList<Term>;

    discretization::LinearDiscretization<Real, Index> discretization(100);
    discretization::LinearDiscretization<Real, Index> lattice(100, 1.);
    HamiltonianFourier<Term> hamiltonian;

    SECTION("creator")
    {
        auto term = make_term(Complex(1., 0.), {make_creator(Index(0), Spin(true))});
        TermList result;
        hamiltonian.commutate_hopping(term, discretization, lattice, result);

        REQUIRE(result.size() == 1);
        REQUIRE(result[0].same_operators(term));
        REQUIRE(result[0].prefactor
                == Complex(-2. * std::cos(dot_product(discretization[term.operators[0].index],
                                                      lattice.lattice_vectors()[0])),
                           0.));
    }

    SECTION("annihilator")
    {
        auto term = make_term(Complex(1., 0.), {make_annihilator(Index(0), Spin(true))});
        TermList result;
        hamiltonian.commutate_hopping(term, discretization, lattice, result);

        REQUIRE(result.size() == 1);
        REQUIRE(result[0].same_operators(term));
        REQUIRE(result[0].prefactor
                == Complex(2. * std::cos(dot_product(discretization[term.operators[0].index],
                                                     lattice.lattice_vectors()[0])),
                           0.));
    }
}

template <typename Operator, typename Real>
void test_commutate_interaction()
{
    using Complex  = std::complex<Real>;
    using Index    = typename Operator::Index;
    using Spin     = typename Operator::Spin;
    using Term     = Term<Operator, Complex>;
    using TermList = TermList<Term>;

    discretization::LinearDiscretization<Real, Index> discretization(10);
    discretization::LinearDiscretization<Real, Index> lattice(100, 1.);
    HamiltonianFourier<Term> hamiltonian;

    SECTION("creator")
    {
        auto term = make_term(Complex(1., 0.), {make_creator(Index(0), Spin(true))});
        TermList result;
        hamiltonian.commutate_interaction(term, discretization, lattice, result);

        auto k            = discretization[term.operators[0].index];
        auto index        = term.operators[0].index;
        auto spin         = term.operators[0].spin;
        auto flipped_spin = !spin;
        Complex prefactor = hamiltonian.U / lattice.num();
        Index i = 0;
        for(auto& idx1 : discretization) {
            const auto k1 = discretization[idx1];
            for(auto idx2 : discretization) {
                const auto k2 = discretization[idx2];
                const auto k3 = k1 + k2 - k;
                auto idx3     = discretization(k3);

                REQUIRE(result[i] == make_term(prefactor, {make_creator(idx1, spin),
                                                           make_creator(idx2, flipped_spin),
                                                           make_annihilator(idx3, flipped_spin)}));
                ++i;
            }
        }

        prefactor = -hamiltonian.U / (2 * lattice.num());
        REQUIRE(result.back() == make_term(prefactor, {make_creator(index, spin)}));
    }

    SECTION("annihilator")
    {
        auto term = make_term(Complex(1., 0.), {make_annihilator(Index(0), Spin(true))});
        TermList result;
        hamiltonian.commutate_interaction(term, discretization, lattice, result);

        auto k            = discretization[term.operators[0].index];
        auto index        = term.operators[0].index;
        auto spin         = term.operators[0].spin;
        auto flipped_spin = !spin;
        Complex prefactor = -hamiltonian.U / lattice.num();
        Index i = 0;
        for(auto& idx1 : discretization) {
            const auto k1 = discretization[idx1];
            for(auto idx2 : discretization) {
                const auto k2 = discretization[idx2];
                const auto k3 = k2 - k1 - k;
                auto idx3     = discretization(k3);

                REQUIRE(result[i] == make_term(prefactor, {make_annihilator(idx1, spin),
                                                           make_creator(idx2, flipped_spin),
                                                           make_annihilator(idx3, flipped_spin)}));
                ++i;
            }
        }

        prefactor = hamiltonian.U / (2 * lattice.num());
        REQUIRE(result.back() == make_term(prefactor, {make_annihilator(index, spin)}));
    }
}

template <typename Operator, typename Real>
void test_commutate()
{
    using Complex  = std::complex<Real>;
    using Index    = typename Operator::Index;
    using Spin     = typename Operator::Spin;
    using Term     = Term<Operator, Complex>;
    using TermList = TermList<Term>;

    discretization::LinearDiscretization<Real, Index> discretization(10);
    discretization::LinearDiscretization<Real, Index> lattice(100, 1.);
    HamiltonianFourier<Term> hamiltonian;

    SECTION("creator")
    {
        auto term =
            ieompp::algebra::make_term(Complex(1., 0.), {make_creator(Index(0), Spin(true))});
        TermList result;
        auto full = hamiltonian.commutate(term, discretization, lattice);
        decltype(full) kinetic, interaction;
        hamiltonian.commutate_hopping(term, discretization, lattice, kinetic);
        hamiltonian.commutate_interaction(term, discretization, lattice, interaction);

        REQUIRE(kinetic.size() + interaction.size() == full.size());
        REQUIRE(std::equal(kinetic.begin(), kinetic.end(), full.begin()));
        REQUIRE(std::equal(interaction.begin(), interaction.end(), full.begin() + kinetic.size()));
    }

    SECTION("annihilator")
    {
        auto term =
            ieompp::algebra::make_term(Complex(1., 0.), {make_annihilator(Index(0), Spin(true))});
        TermList result;
        auto full = hamiltonian.commutate(term, discretization, lattice);
        decltype(full) kinetic, interaction;
        hamiltonian.commutate_hopping(term, discretization, lattice, kinetic);
        hamiltonian.commutate_interaction(term, discretization, lattice, interaction);

        REQUIRE(kinetic.size() + interaction.size() == full.size());
        REQUIRE(std::equal(kinetic.begin(), kinetic.end(), full.begin()));
        REQUIRE(std::equal(interaction.begin(), interaction.end(), full.begin() + kinetic.size()));
    }
}

TEST_CASE("commutate_hopping")
{
    test_commutate_hopping<Operator<long, bool>, float>();
    test_commutate_hopping<Operator<long, bool>, double>();
    test_commutate_hopping<Operator<long, bool>, long double>();

    test_commutate_hopping<Operator<unsigned long, bool>, float>();
    test_commutate_hopping<Operator<unsigned long, bool>, double>();
    test_commutate_hopping<Operator<unsigned long, bool>, long double>();
}

TEST_CASE("commutate_interaction")
{
    test_commutate_interaction<Operator<long, bool>, float>();
    test_commutate_interaction<Operator<long, bool>, double>();
    test_commutate_interaction<Operator<long, bool>, long double>();

    test_commutate_interaction<Operator<unsigned long, bool>, float>();
    test_commutate_interaction<Operator<unsigned long, bool>, double>();
    test_commutate_interaction<Operator<unsigned long, bool>, long double>();
}

TEST_CASE("commutate")
{
    test_commutate<Operator<long, bool>, float>();
    test_commutate<Operator<long, bool>, double>();
    test_commutate<Operator<long, bool>, long double>();

    test_commutate<Operator<unsigned long, bool>, float>();
    test_commutate<Operator<unsigned long, bool>, double>();
    test_commutate<Operator<unsigned long, bool>, long double>();
}
