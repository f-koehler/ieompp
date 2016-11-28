#include "operator.hpp"
using namespace ieompp::algebra;

TEST_CASE("conjugate_1")
{
    const Operator1 a{true, 0ul}, b{false, 0ul};
    Operator1 a_conj(a), b_conj(b);
    a_conj.conjugate();
    b_conj.conjugate();

    REQUIRE(a_conj == b);
    REQUIRE(b_conj == a);
}

TEST_CASE("conjugate_2")
{
    const Operator2 a{true, 0ul, true}, b{false, 0ul, true};
    Operator2 a_conj(a), b_conj(b);
    a_conj.conjugate();
    b_conj.conjugate();

    REQUIRE(a_conj == b);
    REQUIRE(b_conj == a);
}

TEST_CASE("conjugate_3")
{
    const Operator3 a{true, {0ul, true, 'a'}}, b{false, {0ul, true, 'a'}};
    Operator3 a_conj(a), b_conj(b);
    a_conj.conjugate();
    b_conj.conjugate();

    REQUIRE(a_conj == b);
    REQUIRE(b_conj == a);
}

TEST_CASE("get_conjugate_1")
{
    const Operator1 a{true, 0ul}, b{false, 0ul};

    REQUIRE(a.get_conjugate() == b);
    REQUIRE(b.get_conjugate() == a);
}

TEST_CASE("get_conjugate_2")
{
    const Operator2 a{true, 0ul, true}, b{false, 0ul, true};

    REQUIRE(a.get_conjugate() == b);
    REQUIRE(b.get_conjugate() == a);
}

TEST_CASE("get_conjugate_3")
{
    const Operator3 a{true, {0ul, true, 'a'}}, b{false, {0ul, true, 'a'}};

    REQUIRE(a.get_conjugate() == b);
    REQUIRE(b.get_conjugate() == a);
}
