#include "operator.hpp"
using namespace ieompp::algebra;

TEST_CASE("same_indices_1")
{
    Operator1 a{true, 0ul}, b{false, 1ul};

    REQUIRE(a.same_indices(a));
    REQUIRE(b.same_indices(b));

    REQUIRE(!a.same_indices(b));
    REQUIRE(!b.same_indices(a));
}

TEST_CASE("same_indices_2")
{
    Operator2 a{true, 0ul, true}, b{false, 0ul, false}, c{true, 1ul, true};

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
    Operator3 a{true, {0ul, true, 'a'}}, b{false, {0ul, true, 'b'}}, c{true, {0ul, false, 'a'}},
        d{false, {1ul, true, 'a'}};

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
