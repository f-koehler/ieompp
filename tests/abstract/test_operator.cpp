#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "ieompp/abstract/operator.hpp"
#include "ieompp/abstract/commutator.hpp"

using namespace ieompp;
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
        REQUIRE(ac.number == std::complex<double>(1., 0.));
        REQUIRE(ac.kroneckers.size() == 2);
        REQUIRE(ac.kroneckers.front() == (Kronecker{"k_2", "k_1"}));
        REQUIRE(ac.kroneckers.back() == (Kronecker{"s_2", "s_1"}));

        ac = algebra::anticommutator<AbstractOperator, AbstractPrefactor>(
            make_creator("k_1", "s_1"), make_creator("k_1", "s_2"));
        REQUIRE(ac.number == std::complex<double>(1., 0.));
        REQUIRE(ac.kroneckers.size() == 1);
        REQUIRE(ac.kroneckers.back() == (Kronecker{"s_2", "s_1"}));
        
        ac = algebra::anticommutator<AbstractOperator, AbstractPrefactor>(
            make_creator("k_1", "s_1"), make_creator("k_2", "s_1"));
        REQUIRE(ac.number == std::complex<double>(1., 0.));
        REQUIRE(ac.kroneckers.size() == 1);
        REQUIRE(ac.kroneckers.front() == (Kronecker{"k_2", "k_1"}));
        
        ac = algebra::anticommutator<AbstractOperator, AbstractPrefactor>(
            make_creator("k_1", "s_1"), make_creator("k_1", "s_1"));
        REQUIRE(ac.number == std::complex<double>(1., 0.));
        REQUIRE(ac.kroneckers.empty());
    }
}
