#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "hubbard/abstract/algebra.hpp"

using namespace hubbard;
using namespace abstract;

TEST_CASE("simplify_terms", "[simplify]")
{
    SECTION("simplify_filter")
    {
        auto term1 = abstract::make_term(AbstractPrefactor{Complex(0., 0.), {Kronecker{"k", "q"}}},
                                         {make_creator("k", "s")});
        AbstractTermList list;
        list.push_back(term1);
        simplify::filter(list);
        REQUIRE(list.empty());
    }
}
