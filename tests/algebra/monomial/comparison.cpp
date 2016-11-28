#include "monomial.hpp"

TEST_CASE("equality")
{
    const Monomial1 a = {{true, 0}}, b = {{true, 1}}, c = {{true, 0}, {false, 0}};

    REQUIRE(a == a);
    REQUIRE(b == b);
    REQUIRE(c == c);

    REQUIRE(!(a == b));
    REQUIRE(!(a == c));
    REQUIRE(!(b == a));
    REQUIRE(!(b == c));
    REQUIRE(!(c == a));
    REQUIRE(!(c == b));
}

TEST_CASE("inequality")
{
    const Monomial1 a = {{true, 0}}, b = {{true, 1}}, c = {{true, 0}, {false, 0}};

    REQUIRE(!(a != a));
    REQUIRE(!(b != b));
    REQUIRE(!(c != c));

    REQUIRE(a != b);
    REQUIRE(a != c);
    REQUIRE(b != a);
    REQUIRE(b != c);
    REQUIRE(c != a);
    REQUIRE(c != b);
}
