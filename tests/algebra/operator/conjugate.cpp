#include "operator.hpp"
using namespace ieompp::algebra;

TEST_CASE("conjugate_1")
{
    const auto a = Operator1::make_creator(0ul), b = Operator1::make_annihilator(0ul);
    Operator1 a_conj(a), b_conj(b);
    a_conj.conjugate();
    b_conj.conjugate();

    REQUIRE(a_conj == b);
    REQUIRE(b_conj == a);
}

TEST_CASE("conjugate_2")
{
    const auto a = Operator2::make_creator(0ul, true), b = Operator2::make_annihilator(0ul, true);
    Operator2 a_conj(a), b_conj(b);
    a_conj.conjugate();
    b_conj.conjugate();

    REQUIRE(a_conj == b);
    REQUIRE(b_conj == a);
}

TEST_CASE("conjugate_3")
{
    const auto a = Operator3::make_creator(0ul, true, 'a'),
               b = Operator3::make_annihilator(0ul, true, 'a');
    Operator3 a_conj(a), b_conj(b);
    a_conj.conjugate();
    b_conj.conjugate();

    REQUIRE(a_conj == b);
    REQUIRE(b_conj == a);
}

TEST_CASE("get_conjugate_1")
{
    const auto a = Operator1::make_creator(0ul), b = Operator1::make_annihilator(0ul);

    REQUIRE(a.get_conjugate() == b);
    REQUIRE(b.get_conjugate() == a);
}

TEST_CASE("get_conjugate_2")
{
    const auto a = Operator2::make_creator(0ul, true), b = Operator2::make_annihilator(0ul, true);

    REQUIRE(a.get_conjugate() == b);
    REQUIRE(b.get_conjugate() == a);
}

TEST_CASE("get_conjugate_3")
{
    const auto a = Operator3::make_creator(0ul, true, 'a'),
               b = Operator3::make_annihilator(0ul, true, 'a');

    REQUIRE(a.get_conjugate() == b);
    REQUIRE(b.get_conjugate() == a);
}
