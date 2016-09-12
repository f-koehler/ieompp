#include "operator.hpp"

BOOST_AUTO_TEST_CASE(test_index_tuple_1)
{
    Op1 op{true, 1ul};

    BOOST_TEST((op.index_tuple() == std::make_tuple(1ul)));
}

BOOST_AUTO_TEST_CASE(test_index_tuple_2)
{
    Op2 op{true, 1ul, true};

    BOOST_TEST((op.index_tuple() == std::make_tuple(1ul, true)));
}

BOOST_AUTO_TEST_CASE(test_index_tuple_3)
{
    Op3 op{true, std::make_tuple(1ul, true, 'a')};

    BOOST_TEST((op.index_tuple() == std::make_tuple(1ul, true, 'a')));
}
