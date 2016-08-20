#include "operator.hpp"

BOOST_AUTO_TEST_CASE(make_creator_1)
{
    Op1 op = make_creator(1ul);

    BOOST_TEST(op.creator);
    BOOST_TEST(op.index == 1ul);
}

BOOST_AUTO_TEST_CASE(make_creator_2)
{
    Op2 op = make_creator(1ul, true);

    BOOST_TEST(op.creator);
    BOOST_TEST(op.index1 == 1ul);
    BOOST_TEST(op.index2 == true);
}

BOOST_AUTO_TEST_CASE(make_creator_3)
{
    Op3 op = make_creator(1ul, true, 'a');

    BOOST_TEST(op.creator);
    BOOST_TEST(std::get<0>(op.indices) == 1ul);
    BOOST_TEST(std::get<1>(op.indices) == true);
    BOOST_TEST(std::get<2>(op.indices) == 'a');
}

BOOST_AUTO_TEST_CASE(make_annihilator_1)
{
    Op1 op = make_annihilator(1ul);

    BOOST_TEST(!op.creator);
    BOOST_TEST(op.index == 1ul);
}

BOOST_AUTO_TEST_CASE(make_annihilator_2)
{
    Op2 op = make_annihilator(1ul, true);

    BOOST_TEST(!op.creator);
    BOOST_TEST(op.index1 == 1ul);
    BOOST_TEST(op.index2 == true);
}

BOOST_AUTO_TEST_CASE(make_annihilator_3)
{
    Op3 op = make_annihilator(1ul, true, 'a');

    BOOST_TEST(!op.creator);
    BOOST_TEST(std::get<0>(op.indices) == 1ul);
    BOOST_TEST(std::get<1>(op.indices) == true);
    BOOST_TEST(std::get<2>(op.indices) == 'a');
}
