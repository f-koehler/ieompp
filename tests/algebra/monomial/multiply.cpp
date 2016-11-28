#include "monomial.hpp"

TEST_CASE("multiply_assign")
{
    Monomial1 a       = {{true, 0}, {false, 1}};
    const Monomial1 b = {{true, 2}, {false, 3}}, c = {{true, 0}, {false, 1}, {true, 2}, {false, 3}};
    a *= b;
    REQUIRE(a == c);
}

TEST_CASE("multiply")
{
    const Monomial1 a = {{true, 0}, {false, 1}}, b = {{true, 2}, {false, 3}},
                    c = {{true, 0}, {false, 1}, {true, 2}, {false, 3}};
    REQUIRE((a * b) == c);
}
