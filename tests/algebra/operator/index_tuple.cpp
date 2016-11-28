#include "operator.hpp"
using namespace ieompp::algebra;

TEST_CASE("index_tuple_1")
{
    const auto op = Operator1::make_creator(1ul);

    REQUIRE((op.index_tuple() == std::make_tuple(1ul)));
}

TEST_CASE("index_tuple_2")
{
    const auto op = Operator2::make_creator(1ul, true);

    REQUIRE((op.index_tuple() == std::make_tuple(1ul, true)));
}

TEST_CASE("index_tuple_3")
{
    const auto op = Operator3::make_creator(1ul, true, 'a');

    REQUIRE((op.index_tuple() == std::make_tuple(1ul, true, 'a')));
}
