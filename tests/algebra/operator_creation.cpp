#include "operator.hpp"
#include <ieompp/algebra/operator/make.hpp>
using namespace ieompp::algebra;

// TODO: the next three tests are very basic
BOOST_AUTO_TEST_CASE(make_creator_types_1)
{
    auto op1       = make_creator(1ul);
    std::size_t ul = 1ul;
    auto op2       = make_creator(ul);

    static_assert(std::is_same<decltype(op1), decltype(op2)>::value,
                  "op1 and op2 should be of same type");
}

BOOST_AUTO_TEST_CASE(make_creator_types_2)
{
    auto op1       = make_creator(1ul, true);
    std::size_t ul = 1ul;
    bool b         = false;
    auto op2       = make_creator(ul, b);

    static_assert(std::is_same<decltype(op1), decltype(op2)>::value,
                  "op1 and op2 should be of same type");
}

BOOST_AUTO_TEST_CASE(make_creator_types_3)
{
    auto op1       = make_creator(1ul, true, 'c');
    std::size_t ul = 1ul;
    bool b         = false;
    const char c   = 'a';
    auto op2       = make_creator(ul, b, c);

    static_assert(std::is_same<decltype(op1), decltype(op2)>::value,
                  "op1 and op2 should be of same type");
}

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
