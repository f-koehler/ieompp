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

    SECTION("self comparison")
    {
        Operator a = make_creator(Index(0), Spin(0));
        REQUIRE(!(a < a));
        REQUIRE(!(a > a));
    }

    SECTION("creator and annihilator")
    {
        Operator a = make_creator(Index(0), Spin(0));
        Operator b = make_annihilator(Index(0), Spin(0));
        REQUIRE(a < b);
        REQUIRE(!(a > b));
        REQUIRE(!(b < a));
        REQUIRE(b > a);
    }

    SECTION("different index")
    {
        Operator a = make_creator(Index(0), Spin(0));
        Operator b = make_creator(Index(1), Spin(0));
        REQUIRE(a < b);
        REQUIRE(!(a > b));
        REQUIRE(!(b < a));
        REQUIRE(b > a);
    }


    SECTION("different spin")
    {
        Operator a = make_creator(Index(0), Spin(0));
        Operator b = make_creator(Index(0), Spin(1));
        REQUIRE(a < b);
        REQUIRE(!(a > b));
        REQUIRE(!(b < a));
        REQUIRE(b > a);
    }

    SECTION("annihilator & creator with different index")
    {
        Operator a = make_creator(Index(1), Spin(0));
        Operator b = make_annihilator(Index(0), Spin(0));
        REQUIRE(!(a > b));
        REQUIRE(a < b);
    }

    SECTION("annihilator & creator with different spin")
    {
        Operator a = make_creator(Index(0), Spin(0));
        Operator b = make_annihilator(Index(0), Spin(1));
        REQUIRE(!(a > b));
        REQUIRE(a < b);
    }
}

TEST_CASE("comparison")
{
    test_comparison<Operator<unsigned long, bool>>();
    test_comparison<Operator<long, bool>>();
}
