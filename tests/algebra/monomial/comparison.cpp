#include "monomial.hpp"

BOOST_AUTO_TEST_CASE(equality)
{
    const Monomial1 a = {{true, 0}}, b = {{true, 1}}, c = {{true, 0}, {false, 0}};

    BOOST_TEST(a == a);
    BOOST_TEST(b == b);
    BOOST_TEST(c == c);

    BOOST_TEST(!(a == b));
    BOOST_TEST(!(a == c));
    BOOST_TEST(!(b == a));
    BOOST_TEST(!(b == c));
    BOOST_TEST(!(c == a));
    BOOST_TEST(!(c == b));
}

BOOST_AUTO_TEST_CASE(inequality)
{
    const Monomial1 a = {{true, 0}}, b = {{true, 1}}, c = {{true, 0}, {false, 0}};

    BOOST_TEST(!(a != a));
    BOOST_TEST(!(b != b));
    BOOST_TEST(!(c != c));

    BOOST_TEST(a != b);
    BOOST_TEST(a != c);
    BOOST_TEST(b != a);
    BOOST_TEST(b != c);
    BOOST_TEST(c != a);
    BOOST_TEST(c != b);
}
