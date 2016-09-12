#include "operator.hpp"

BOOST_AUTO_TEST_CASE(test_same_indices_1)
{
    Op1 a{true, 0ul}, b{false, 1ul};

    BOOST_TEST(a.same_indices(a));
    BOOST_TEST(b.same_indices(b));

    BOOST_TEST(!a.same_indices(b));
    BOOST_TEST(!b.same_indices(a));
}

BOOST_AUTO_TEST_CASE(test_same_indices_2)
{
    Op2 a{true, 0ul, true}, b{false, 0ul, false}, c{true, 1ul, true};

    BOOST_TEST(a.same_indices(a));
    BOOST_TEST(b.same_indices(b));
    BOOST_TEST(c.same_indices(c));

    BOOST_TEST(!a.same_indices(b));
    BOOST_TEST(!a.same_indices(c));

    BOOST_TEST(!b.same_indices(a));
    BOOST_TEST(!b.same_indices(c));

    BOOST_TEST(!c.same_indices(a));
    BOOST_TEST(!c.same_indices(b));
}

BOOST_AUTO_TEST_CASE(test_same_indices_3)
{
    Op3 a{true, std::make_tuple(0ul, true, 'a')}, b{false, std::make_tuple(0ul, true, 'b')},
        c{true, std::make_tuple(0ul, false, 'a')}, d{false, std::make_tuple(1ul, true, 'a')};

    BOOST_TEST(a.same_indices(a));
    BOOST_TEST(b.same_indices(b));
    BOOST_TEST(c.same_indices(c));
    BOOST_TEST(d.same_indices(d));

    BOOST_TEST(!a.same_indices(b));
    BOOST_TEST(!a.same_indices(c));
    BOOST_TEST(!a.same_indices(d));

    BOOST_TEST(!b.same_indices(a));
    BOOST_TEST(!b.same_indices(c));
    BOOST_TEST(!b.same_indices(d));

    BOOST_TEST(!c.same_indices(a));
    BOOST_TEST(!c.same_indices(b));
    BOOST_TEST(!c.same_indices(d));

    BOOST_TEST(!d.same_indices(a));
    BOOST_TEST(!d.same_indices(b));
    BOOST_TEST(!d.same_indices(c));
}
