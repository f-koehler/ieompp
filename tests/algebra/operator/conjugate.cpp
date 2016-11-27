#include "operator.hpp"
using namespace ieompp::algebra;

BOOST_AUTO_TEST_CASE(conjugate_1)
{
    const Operator1 a{true, 0ul}, b{false, 0ul};
    Operator1 a_conj(a), b_conj(b);
    a_conj.conjugate();
    b_conj.conjugate();

    BOOST_TEST(a_conj == b);
    BOOST_TEST(b_conj == a);
}

BOOST_AUTO_TEST_CASE(conjugate_2)
{
    const Operator2 a{true, 0ul, true}, b{false, 0ul, true};
    Operator2 a_conj(a), b_conj(b);
    a_conj.conjugate();
    b_conj.conjugate();

    BOOST_TEST(a_conj == b);
    BOOST_TEST(b_conj == a);
}

BOOST_AUTO_TEST_CASE(conjugate_3)
{
    const Operator3 a{true, {0ul, true, 'a'}}, b{false, {0ul, true, 'a'}};
    Operator3 a_conj(a), b_conj(b);
    a_conj.conjugate();
    b_conj.conjugate();

    BOOST_TEST(a_conj == b);
    BOOST_TEST(b_conj == a);
}

BOOST_AUTO_TEST_CASE(get_conjugate_1)
{
    const Operator1 a{true, 0ul}, b{false, 0ul};

    BOOST_TEST(a.get_conjugate() == b);
    BOOST_TEST(b.get_conjugate() == a);
}

BOOST_AUTO_TEST_CASE(get_conjugate_2)
{
    const Operator2 a{true, 0ul, true}, b{false, 0ul, true};

    BOOST_TEST(a.get_conjugate() == b);
    BOOST_TEST(b.get_conjugate() == a);
}

BOOST_AUTO_TEST_CASE(get_conjugate_3)
{
    const Operator3 a{true, {0ul, true, 'a'}}, b{false, {0ul, true, 'a'}};

    BOOST_TEST(a.get_conjugate() == b);
    BOOST_TEST(b.get_conjugate() == a);
}
