#include "monomial.hpp"

BOOST_AUTO_TEST_CASE(multiply_assign)
{
    Monomial1 a       = {{true, 0}, {false, 1}};
    const Monomial1 b = {{true, 2}, {false, 3}}, c = {{true, 0}, {false, 1}, {true, 2}, {false, 3}};
    a *= b;
    BOOST_TEST(a == c);
}

BOOST_AUTO_TEST_CASE(multiply)
{
    const Monomial1 a = {{true, 0}, {false, 1}}, b = {{true, 2}, {false, 3}},
                    c = {{true, 0}, {false, 1}, {true, 2}, {false, 3}};
    BOOST_TEST((a * b) == c);
}
