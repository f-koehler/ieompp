#include "operator.hpp"
using namespace ieompp::algebra;

TEST_CASE("same_indices_1")
{
    const auto a = Operator1::make_creator(0ul), b = Operator1::make_annihilator(1ul);

    REQUIRE(a.same_indices(a));
    REQUIRE(b.same_indices(b));

    REQUIRE(!a.same_indices(b));
    REQUIRE(!b.same_indices(a));
}

TEST_CASE("same_indices_2")
{
    const auto a = Operator2::make_creator(0ul, true), b = Operator2::make_annihilator(0ul, false),
               c = Operator2::make_creator(1ul, true);

    REQUIRE(a.same_indices(a));
    REQUIRE(b.same_indices(b));
    REQUIRE(c.same_indices(c));

    REQUIRE(!a.same_indices(b));
    REQUIRE(!a.same_indices(c));

    REQUIRE(!b.same_indices(a));
    REQUIRE(!b.same_indices(c));

    REQUIRE(!c.same_indices(a));
    REQUIRE(!c.same_indices(b));
}

TEST_CASE("same_indices_3")
{
    const auto a = Operator3::make_creator(0ul, true, 'a'),
               b = Operator3::make_annihilator(0ul, true, 'b'),
               c = Operator3::make_creator(0ul, false, 'a'),
               d = Operator3::make_annihilator(1ul, true, 'a');

    REQUIRE(a.same_indices(a));
    REQUIRE(b.same_indices(b));
    REQUIRE(c.same_indices(c));
    REQUIRE(d.same_indices(d));

    REQUIRE(!a.same_indices(b));
    REQUIRE(!a.same_indices(c));
    REQUIRE(!a.same_indices(d));

    REQUIRE(!b.same_indices(a));
    REQUIRE(!b.same_indices(c));
    REQUIRE(!b.same_indices(d));

    REQUIRE(!c.same_indices(a));
    REQUIRE(!c.same_indices(b));
    REQUIRE(!c.same_indices(d));

    REQUIRE(!d.same_indices(a));
    REQUIRE(!d.same_indices(b));
    REQUIRE(!d.same_indices(c));
}
