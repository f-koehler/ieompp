#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "hubbard/abstract/simplify.hpp"

using namespace hubbard;
using namespace abstract;

TEST_CASE("join", "[abstract] [simplify]")
{
    auto term1 = abstract::make_term(AbstractPrefactor{Complex(1., 0.), {Kronecker{"k", "q"}}},
                                     {make_creator("k", "s")});
    auto term2 = abstract::make_term(AbstractPrefactor{Complex(-0.5, 0.5), {Kronecker{"k", "q"}}},
                                     {make_creator("k", "s")});
    AbstractTermList list;
    list.push_back(term1);
    list.push_back(term2);
    simplify::join(list);
    REQUIRE(list.size() == 1);
    REQUIRE(list.front()
            == abstract::make_term(AbstractPrefactor{Complex(0.5, 0.5), {Kronecker{"k", "q"}}},
                                   {make_creator("k", "s")}));
}

TEST_CASE("eval_kroneckers", "[abstract] [simplify]")
{
    auto term = abstract::make_term(AbstractPrefactor{Complex(0., 0.), {Kronecker{"k_1", "k_1"}}},
                                    {make_creator("k", "s")});
    AbstractTermList list;
    list.push_back(term);
    simplify::eval_kroneckers(list);
    REQUIRE(list.size() == 1);
    REQUIRE(list.front() == abstract::make_term(AbstractPrefactor{Complex(0., 0.), {}},
                                                {make_creator("k", "s")}));
}

TEST_CASE("filter", "[abstract] [simplify]")
{
    auto term1 = abstract::make_term(AbstractPrefactor{Complex(0., 0.), {Kronecker{"k", "q"}}},
                                     {make_creator("k", "s")});
    auto term2 = abstract::make_term(AbstractPrefactor{Complex(1., 0.), {Kronecker{"k", "q"}}},
                                     {make_annihilator("k", "s")});
    AbstractTermList list;
    list.push_back(term1);
    list.push_back(term2);
    simplify::filter(list);
    REQUIRE(list.size() == 1);
    REQUIRE(list.front() == term2);
}