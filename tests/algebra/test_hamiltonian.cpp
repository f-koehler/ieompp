#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "ieompp/algebra/hamiltonian.hpp"
#include "ieompp/discretization/linear.hpp"

using namespace ieompp;
using namespace algebra;

template <typename Operator, typename Real>
void test_commutate_hopping()
{
    using Complex  = std::complex<Real>;
    using Index    = typename Operator::Index;
    using Spin     = typename Operator::Spin;
    using Term     = Term<Operator, Complex>;
    using TermList = TermList<Term>;

    discretization::LinearDiscretization<Real> discretization(1000, 1.);
    Hamiltonian<Term> hamiltonian;

    SECTION("creator")
    {
        auto term = ieompp::algebra::make_term(Complex(1., 0.), {make_creator(Index(0), Spin(true))});
        TermList result;
        hamiltonian.commutate_hopping(term, discretization, result);

        REQUIRE(result[0] == make_term(Complex(-1., 0.), {make_creator(Index(1), Spin(true))}));
        REQUIRE(result[1] == make_term(Complex(-1., 0.), {make_creator(Index(999), Spin(true))}));
    }

    SECTION("annihilator")
    {
        auto term = ieompp::algebra::make_term(Complex(1., 0.), {make_annihilator(Index(0), Spin(true))});
        TermList result;
        hamiltonian.commutate_hopping(term, discretization, result);

        REQUIRE(result[0] == make_term(Complex(1., 0.), {make_annihilator(Index(1), Spin(true))}));
        REQUIRE(result[1] == make_term(Complex(1., 0.), {make_annihilator(Index(999), Spin(true))}));
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

    discretization::LinearDiscretization<double> discretization(1000, 1.);
    Hamiltonian<Term> hamiltonian;

    SECTION("creator")
    {
        auto term = ieompp::algebra::make_term(Complex(1., 0.), {make_creator(Index(0), Spin(true))});
        TermList result;
        hamiltonian.commutate_interaction(term, discretization, result);

        REQUIRE(result[0] == make_term(Complex(1., 0.), {make_creator(Index(0), Spin(true)),
                                                         make_creator(Index(0), Spin(false)),
                                                         make_annihilator(Index(0), Spin(false))}));
        REQUIRE(result[1] == make_term(Complex(-0.5, 0.), {make_creator(Index(0), Spin(true))}));
    }

    SECTION("annihilator")
    {
        auto term = ieompp::algebra::make_term(Complex(1., 0.), {make_annihilator(Index(0), Spin(true))});
        TermList result;
        hamiltonian.commutate_interaction(term, discretization, result);

        REQUIRE(result[0] == make_term(Complex(-1., 0.), {make_annihilator(Index(0), Spin(true)),
                                                         make_creator(Index(0), Spin(false)),
                                                         make_annihilator(Index(0), Spin(false))}));
        REQUIRE(result[1] == make_term(Complex(0.5, 0.), {make_annihilator(Index(0), Spin(true))}));
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

    discretization::LinearDiscretization<Real> discretization(1000, 1.);
    Hamiltonian<Term> hamiltonian;

    SECTION("creator")
    {
        auto term = ieompp::algebra::make_term(Complex(1., 0.), {make_creator(Index(0), Spin(true))});
        TermList result;
        auto full = hamiltonian.commutate(term, discretization);
        decltype(full) kinetic, interaction;
        hamiltonian.commutate_hopping(term, discretization, kinetic);
        hamiltonian.commutate_interaction(term, discretization, kinetic);

        REQUIRE(std::equal(kinetic.begin(), kinetic.end(), full.begin()));
        REQUIRE(std::equal(interaction.begin(), interaction.end(), full.begin() + kinetic.size()));
    }

    SECTION("annihilator")
    {
        auto term = ieompp::algebra::make_term(Complex(1., 0.), {make_annihilator(Index(0), Spin(true))});
        TermList result;
        auto full = hamiltonian.commutate(term, discretization);
        decltype(full) kinetic, interaction;
        hamiltonian.commutate_hopping(term, discretization, kinetic);
        hamiltonian.commutate_interaction(term, discretization, interaction);

        REQUIRE(std::equal(kinetic.begin(), kinetic.end(), full.begin()));
        REQUIRE(std::equal(interaction.begin(), interaction.end(), full.begin() + kinetic.size()));
    }
}

TEST_CASE("commutate_hopping", "")
{
    test_commutate_hopping<Operator<long, bool>, float>();
    test_commutate_hopping<Operator<long, bool>, double>();
    test_commutate_hopping<Operator<long, bool>, long double>();

    test_commutate_hopping<Operator<unsigned long, bool>, float>();
    test_commutate_hopping<Operator<unsigned long, bool>, double>();
    test_commutate_hopping<Operator<unsigned long, bool>, long double>();
}

TEST_CASE("commutate_interaction", "")
{
    test_commutate_interaction<Operator<long, bool>, float>();
    test_commutate_interaction<Operator<long, bool>, double>();
    test_commutate_interaction<Operator<long, bool>, long double>();

    test_commutate_interaction<Operator<unsigned long, bool>, float>();
    test_commutate_interaction<Operator<unsigned long, bool>, double>();
    test_commutate_interaction<Operator<unsigned long, bool>, long double>();
}

TEST_CASE("commutate", "")
{
    test_commutate<Operator<long, bool>, float>();
    test_commutate<Operator<long, bool>, double>();
    test_commutate<Operator<long, bool>, long double>();

    test_commutate<Operator<unsigned long, bool>, float>();
    test_commutate<Operator<unsigned long, bool>, double>();
    test_commutate<Operator<unsigned long, bool>, long double>();
}
