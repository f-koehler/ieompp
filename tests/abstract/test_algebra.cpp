#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "hubbard/abstract/algebra.hpp"

using namespace hubbard;
using namespace abstract;

TEST_CASE("anticommutator", "[abstract]")
{
    SECTION("anticommutates() for abstract operators")
    {
        REQUIRE(anticommutates(make_creator("k_1", "s_1"), make_creator("k_2", "s_2")));

        REQUIRE(anticommutates(make_annihilator("k_1", "s_1"), make_annihilator("k_2", "s_2")));

        REQUIRE(!anticommutates(make_creator("k_1", "s_1"), make_annihilator("k_2", "s_2")));

        REQUIRE(!anticommutates(make_annihilator("k_1", "s_1"), make_creator("k_2", "s_2")));
    }

    SECTION("anticommutator() for abstract operators")
    {
        auto ac = algebra::anticommutator<AbstractOperator, AbstractPrefactor>(
            make_creator("k_1", "s_1"), make_creator("k_2", "s_2"));
        REQUIRE(ac.prefactor == Complex(1., 0.));
        REQUIRE(ac.kroneckers.size() == 2);
        REQUIRE(ac.kroneckers.front() == (Kronecker{"k_2", "k_1"}));
        REQUIRE(ac.kroneckers.back() == (Kronecker{"s_2", "s_1"}));

        ac = algebra::anticommutator<AbstractOperator, AbstractPrefactor>(
            make_creator("k_1", "s_1"), make_creator("k_1", "s_2"));
        REQUIRE(ac.prefactor == Complex(1., 0.));
        REQUIRE(ac.kroneckers.size() == 1);
        REQUIRE(ac.kroneckers.back() == (Kronecker{"s_2", "s_1"}));
        
        ac = algebra::anticommutator<AbstractOperator, AbstractPrefactor>(
            make_creator("k_1", "s_1"), make_creator("k_2", "s_1"));
        REQUIRE(ac.prefactor == Complex(1., 0.));
        REQUIRE(ac.kroneckers.size() == 1);
        REQUIRE(ac.kroneckers.front() == (Kronecker{"k_2", "k_1"}));
        
        ac = algebra::anticommutator<AbstractOperator, AbstractPrefactor>(
            make_creator("k_1", "s_1"), make_creator("k_1", "s_1"));
        REQUIRE(ac.prefactor == Complex(1., 0.));
        REQUIRE(ac.kroneckers.empty());
    }
}

TEST_CASE("commutator", "[abstract]")
{
    SECTION("[cc,c]")
    {
        AbstractTerm a, b;
        a.operators.push_back(make_creator("k_1", "s'"));
        a.operators.push_back(make_annihilator("k_1", "s'"));
        a.prefactor.prefactor = Complex(1., 0.);
        
        b.operators.push_back(make_creator("q", "s"));
        b.prefactor.prefactor = Complex(1., 0.);

        auto c = algebra::commutate(a, b);
        REQUIRE(c.size() == 1);
        auto& result = c.front();

        auto expect = algebra::make_term(
            AbstractPrefactor{Complex(1., 0.), {Kronecker{"k_1", "q"}, Kronecker{"s'", "s"}}},
            {make_creator("k_1", "s'")});
        REQUIRE(result == expect);
    }
}
