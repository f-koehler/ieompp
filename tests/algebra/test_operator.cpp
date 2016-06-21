#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <tuple>
#include <type_traits>

#include "ieompp/algebra/operator.hpp"
#include "ieompp/symbolic/index.hpp"
namespace algebra  = ieompp::algebra;
namespace symbolic = ieompp::symbolic;

TEST_CASE("index type", "[operator]")
{
    SECTION("one index")
    {
        REQUIRE((std::is_same<typename algebra::index_type<0, algebra::Operator<int>>::type,
                              int>::value));
    }

    SECTION("two indices")
    {
        REQUIRE((std::is_same<typename algebra::index_type<0, algebra::Operator<int, bool>>::type,
                              int>::value));

        REQUIRE((std::is_same<typename algebra::index_type<1, algebra::Operator<int, bool>>::type,
                              bool>::value));
    }

    SECTION("three indices")
    {
        REQUIRE((std::is_same<
                 typename algebra::index_type<0, algebra::Operator<int, bool, const char*>>::type,
                 int>::value));

        REQUIRE((std::is_same<
                 typename algebra::index_type<1, algebra::Operator<int, bool, const char*>>::type,
                 bool>::value));

        REQUIRE((std::is_same<
                 typename algebra::index_type<2, algebra::Operator<int, bool, const char*>>::type,
                 const char*>::value));
    }
}

TEST_CASE("make creator & get_index", "[operator]")
{
    SECTION("one index")
    {
        auto op = algebra::make_creator(42);
        REQUIRE((std::is_same<decltype(op), algebra::Operator<int>>::value));
        REQUIRE((std::is_same<typename algebra::index_type<0, decltype(op)>::type, int>::value));
        REQUIRE(op.creator);
        REQUIRE(op.index == 42);
    }

    SECTION("two indices")
    {
        auto op = algebra::make_creator(1.f, true);
        REQUIRE((std::is_same<decltype(op), algebra::Operator<float, bool>>::value));
        REQUIRE((std::is_same<typename algebra::index_type<0, decltype(op)>::type, float>::value));
        REQUIRE((std::is_same<typename algebra::index_type<1, decltype(op)>::type, bool>::value));
        REQUIRE(op.creator);
        REQUIRE(op.index1 == 1.f);
        REQUIRE(op.index2 == true);
    }

    SECTION("three indices")
    {
        auto op = algebra::make_creator(1.f, true, 'c');
        REQUIRE((std::is_same<decltype(op), algebra::Operator<float, bool, char>>::value));
        REQUIRE((std::is_same<typename algebra::index_type<0, decltype(op)>::type, float>::value));
        REQUIRE((std::is_same<typename algebra::index_type<1, decltype(op)>::type, bool>::value));
        REQUIRE((std::is_same<typename algebra::index_type<2, decltype(op)>::type, char>::value));
        REQUIRE(op.creator);
        REQUIRE(algebra::get_index<0>(op) == 1.f);
        REQUIRE(algebra::get_index<1>(op) == true);
        REQUIRE(algebra::get_index<2>(op) == 'c');
    }
}

TEST_CASE("make annihilator & get_index", "[operator]")
{
    SECTION("one index")
    {
        auto op = algebra::make_annihilator(42);
        REQUIRE((std::is_same<decltype(op), algebra::Operator<int>>::value));
        REQUIRE((std::is_same<typename algebra::index_type<0, decltype(op)>::type, int>::value));
        REQUIRE(!op.creator);
        REQUIRE(op.index == 42);
    }

    SECTION("two indices")
    {
        auto op = algebra::make_annihilator(1.f, true);
        REQUIRE((std::is_same<decltype(op), algebra::Operator<float, bool>>::value));
        REQUIRE((std::is_same<typename algebra::index_type<0, decltype(op)>::type, float>::value));
        REQUIRE((std::is_same<typename algebra::index_type<1, decltype(op)>::type, bool>::value));
        REQUIRE(!op.creator);
        REQUIRE(op.index1 == 1.f);
        REQUIRE(op.index2 == true);
    }

    SECTION("three indices")
    {
        auto op = algebra::make_annihilator(1.f, true, 'c');
        REQUIRE((std::is_same<decltype(op), algebra::Operator<float, bool, char>>::value));
        REQUIRE((std::is_same<typename algebra::index_type<0, decltype(op)>::type, float>::value));
        REQUIRE((std::is_same<typename algebra::index_type<1, decltype(op)>::type, bool>::value));
        REQUIRE((std::is_same<typename algebra::index_type<2, decltype(op)>::type, char>::value));
        REQUIRE(!op.creator);
        REQUIRE(algebra::get_index<0>(op) == 1.f);
        REQUIRE(algebra::get_index<1>(op) == true);
        REQUIRE(algebra::get_index<2>(op) == 'c');
    }
}

TEST_CASE("has symbolic index", "[operator] [symbolic]")
{
    REQUIRE(!algebra::has_symbolic_index<algebra::Operator<int>>::value);
    REQUIRE((algebra::has_symbolic_index<algebra::Operator<int, symbolic::SpaceIndex>>::value));
    REQUIRE((algebra::has_symbolic_index<algebra::Operator<symbolic::SpaceIndex, int>>::value));
}
