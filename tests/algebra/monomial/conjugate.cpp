#include "monomial.hpp"

BOOST_AUTO_TEST_CASE(conjugate)
{
    const Monomial1 a = {{true, 0}}, b = {{false, 0}}, c = {{true, 0}, {true, 1}, {false, 2}},
                    d = {{true, 2}, {false, 1}, {false, 0}};
    Monomial1 a_conj{a}, b_conj{b}, c_conj{c}, d_conj{d};
    a_conj.conjugate();
    b_conj.conjugate();
    c_conj.conjugate();
    d_conj.conjugate();

    BOOST_TEST(a_conj == b);
    BOOST_TEST(b_conj == a);
    BOOST_TEST(c_conj == d);
    BOOST_TEST(d_conj == c);
}

BOOST_AUTO_TEST_CASE(get_conjugate)
{
    const Monomial1 a = {{true, 0}}, b = {{false, 0}}, c = {{true, 0}, {true, 1}, {false, 2}},
                    d = {{true, 2}, {false, 1}, {false, 0}};

    BOOST_TEST(a.get_conjugate() == b);
    BOOST_TEST(b.get_conjugate() == a);
    BOOST_TEST(c.get_conjugate() == d);
    BOOST_TEST(d.get_conjugate() == c);
}
