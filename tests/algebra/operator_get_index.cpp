#include "operator.hpp"
#include <ieompp/algebra/operator/get_index.hpp>
using namespace ieompp::algebra;

// check IndexType
static_assert(std::is_same<IndexType<0, Op1>::Type, std::size_t>::value,
              "Index 0 of Op1 should be of type std::size_t");

static_assert(std::is_same<IndexType<0, Op2>::Type, std::size_t>::value,
              "Index 0 of Op2 should be of type std::size_t");
static_assert(std::is_same<IndexType<1, Op2>::Type, bool>::value,
              "Index 1 of Op2 should be of type bool");

static_assert(std::is_same<IndexType<0, Op3>::Type, std::size_t>::value,
              "Index 0 of Op3 should be of type std::size_t");
static_assert(std::is_same<IndexType<1, Op3>::Type, bool>::value,
              "Index 1 of Op3 should be of type bool");
static_assert(std::is_same<IndexType<2, Op3>::Type, char>::value,
              "Index 2 of Op3 should be of type char");

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
    Op3 op{true, std::make_tuple(1ul, true, 'a')};

    BOOST_TEST(get_index<0>(op) == 1ul);
    BOOST_TEST(get_index<1>(op) == true);
    BOOST_TEST(get_index<2>(op) == 'a');
}
