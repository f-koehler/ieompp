#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "ieompp/abstract/term.hpp"
#include "ieompp/abstract/commutator.hpp"

using namespace ieompp;
using namespace abstract;
TEST_CASE("commutator", "[abstract]")
{
    SECTION("[cc,c]")
    {
        AbstractTerm a, b;
        a.operators.push_back(make_creator("k_1", "s'"));
        a.operators.push_back(make_annihilator("k_1", "s'"));
        a.prefactor.number = Complex(1., 0.);
        
        b.operators.push_back(make_creator("q", "s"));
        b.prefactor.number = Complex(1., 0.);

        AbstractTermList list;
        algebra::commutate(a, b, list);
        REQUIRE(list.size() == 1);
        auto& result = list.front();

        auto expect = algebra::make_term(
            AbstractPrefactor{Complex(1., 0.), {Kronecker{"k_1", "q"}, Kronecker{"s'", "s"}}},
            {make_creator("k_1", "s'")});
        REQUIRE(result == expect);
    }
}
