#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "ieompp/abstract/simplify.hpp"

using namespace ieompp;
using namespace abstract;

TEST_CASE("join")
{
    auto list = AbstractTermList{
        abstract::make_term(AbstractPrefactor{std::complex<double>(1., 0.), {Kronecker{"k", "q"}}},
                            {make_creator("k", "s")}),
        abstract::make_term(
            AbstractPrefactor{std::complex<double>(-0.5, 0.5), {Kronecker{"k", "q"}}},
            {make_creator("k", "s")})};
    simplify::join(list);
    REQUIRE(list.size() == 1);
    REQUIRE(list.front() == abstract::make_term(AbstractPrefactor{std::complex<double>(0.5, 0.5),
                                                                  {Kronecker{"k", "q"}}},
                                                {make_creator("k", "s")}));
}

TEST_CASE("eval_kroneckers")
{
    auto list = AbstractTermList{abstract::make_term(
        AbstractPrefactor{std::complex<double>(0., 0.), {Kronecker{"k_1", "k_1"}}},
        {make_creator("k", "s")})};
    simplify::eval_kroneckers(list);
    REQUIRE(list.size() == 1);
    REQUIRE(list.front() == abstract::make_term(AbstractPrefactor{std::complex<double>(0., 0.), {}},
                                                {make_creator("k", "s")}));
}

TEST_CASE("filter")
{
    auto term1 =
        abstract::make_term(AbstractPrefactor{std::complex<double>(0., 0.), {Kronecker{"k", "q"}}},
                            {make_creator("k", "s")});
    auto term2 =
        abstract::make_term(AbstractPrefactor{std::complex<double>(1., 0.), {Kronecker{"k", "q"}}},
                            {make_annihilator("k", "s")});
    AbstractTermList list;
    list.push_back(term1);
    list.push_back(term2);
    simplify::filter(list);
    REQUIRE(list.size() == 1);
    REQUIRE(list.front() == term2);
}
