#include "operator.hpp"
using namespace ieompp::algebra;

TEST_CASE("index_tuple_1")
{
    Operator1 op{true, 1ul};

    REQUIRE((op.index_tuple() == std::make_tuple(1ul)));
}

TEST_CASE("index_tuple_2")
{
    Operator2 op{true, 1ul, true};

    REQUIRE((op.index_tuple() == std::make_tuple(1ul, true)));
}

TEST_CASE("index_tuple_3")
{
    Operator3 op{true, {1ul, true, 'a'}};

    REQUIRE((op.index_tuple() == std::make_tuple(1ul, true, 'a')));
}
