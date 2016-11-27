#include "operator.hpp"
using namespace ieompp::algebra;

BOOST_AUTO_TEST_CASE(index_tuple_1)
{
    Operator1 op{true, 1ul};

    BOOST_TEST((op.index_tuple() == std::make_tuple(1ul)));
}

BOOST_AUTO_TEST_CASE(index_tuple_2)
{
    Operator2 op{true, 1ul, true};

    BOOST_TEST((op.index_tuple() == std::make_tuple(1ul, true)));
}

BOOST_AUTO_TEST_CASE(index_tuple_3)
{
    Operator3 op{true, {1ul, true, 'a'}};

    BOOST_TEST((op.index_tuple() == std::make_tuple(1ul, true, 'a')));
}
