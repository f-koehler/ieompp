#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "ieompp/algebra/operator.hpp"

using namespace ieompp;
using namespace algebra;

template <typename Operator>
void test_comparison()
{
    using Index = typename Operator::Index;
    using Spin  = typename Operator::Spin;

    Operator a = make_creator(Index(0), Spin(0));
    REQUIRE(!(a < a));
    REQUIRE(!(a > a));

    Operator b = make_annihilator(Index(0), Spin(0));
    REQUIRE(a < b);
    REQUIRE(!(a > b));
    REQUIRE(!(b < a));
    REQUIRE(b > a);

    b = make_creator(Index(1), Spin(0));
    REQUIRE(a < b);
    REQUIRE(!(a > b));
    REQUIRE(!(b < a));
    REQUIRE(b > a);

    b = make_creator(Index(0), Spin(1));
    REQUIRE(a < b);
    REQUIRE(!(a > b));
    REQUIRE(!(b < a));
    REQUIRE(b > a);
}

TEST_CASE("comparison")
{
    test_comparison<Operator<unsigned long, bool>>();
    test_comparison<Operator<long, bool>>();
}
