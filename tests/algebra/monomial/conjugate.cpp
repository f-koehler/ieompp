#include "monomial.hpp"

TEST_CASE("conjugate")
{
    const Monomial1 a = {{true, 0}}, b = {{false, 0}}, c = {{true, 0}, {true, 1}, {false, 2}},
                    d = {{true, 2}, {false, 1}, {false, 0}};
    Monomial1 a_conj{a}, b_conj{b}, c_conj{c}, d_conj{d};
    a_conj.conjugate();
    b_conj.conjugate();
    c_conj.conjugate();
    d_conj.conjugate();

    REQUIRE(a_conj == b);
    REQUIRE(b_conj == a);
    REQUIRE(c_conj == d);
    REQUIRE(d_conj == c);
}

TEST_CASE("get_conjugate")
{
    const Monomial1 a = {{true, 0}}, b = {{false, 0}}, c = {{true, 0}, {true, 1}, {false, 2}},
                    d = {{true, 2}, {false, 1}, {false, 0}};

    REQUIRE(a.get_conjugate() == b);
    REQUIRE(b.get_conjugate() == a);
    REQUIRE(c.get_conjugate() == d);
    REQUIRE(d.get_conjugate() == c);
}
