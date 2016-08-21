#include "operator.hpp"

BOOST_AUTO_TEST_CASE(get_index1)
{
    Op1 op{true, 1ul};

    BOOST_TEST(get_index<0>(op) == 1ul);
}

BOOST_AUTO_TEST_CASE(get_index2)
{
    Op2 op{true, 1ul, true};

    BOOST_TEST(get_index<0>(op) == 1ul);
    BOOST_TEST(get_index<1>(op) == true);
}

BOOST_AUTO_TEST_CASE(get_index3)
{
    Op3 op{true, {1ul, true, 'a'}};

    BOOST_TEST(get_index<0>(op) == 1ul);
    BOOST_TEST(get_index<1>(op) == true);
    BOOST_TEST(get_index<2>(op) == 'a');
}
