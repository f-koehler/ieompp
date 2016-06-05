#include "catch.hpp"

#include "ieompp/operator.hpp"

TEST_CASE("index type", "[operator]")
{
    SECTION("one index")
    {
        REQUIRE((
            std::is_same<typename ieompp::index_type<0, ieompp::Operator<int>>::type, int>::value));
    }

    SECTION("two indices")
    {
        REQUIRE((std::is_same<typename ieompp::index_type<0, ieompp::Operator<int, bool>>::type,
                              int>::value));

        REQUIRE((std::is_same<typename ieompp::index_type<1, ieompp::Operator<int, bool>>::type,
                              bool>::value));
    }

    SECTION("three indices")
    {
        REQUIRE((std::is_same<
                 typename ieompp::index_type<0, ieompp::Operator<int, bool, const char*>>::type,
                 int>::value));

        REQUIRE((std::is_same<
                 typename ieompp::index_type<1, ieompp::Operator<int, bool, const char*>>::type,
                 bool>::value));

        REQUIRE((std::is_same<
                 typename ieompp::index_type<2, ieompp::Operator<int, bool, const char*>>::type,
                 const char*>::value));
    }
}

// TODO: test get_index

TEST_CASE("make creator", "[operator]")
{
    SECTION("one index")
    {
        auto op = ieompp::make_creator(42);
        REQUIRE((std::is_same<decltype(op), ieompp::Operator<int>>::value));
        REQUIRE((std::is_same<typename ieompp::index_type<0, decltype(op)>::type, int>::value));
        REQUIRE(op.creator);
        REQUIRE(op.index == 42);
    }

    SECTION("two indices")
    {
        auto op = ieompp::make_creator(1.f, true);
        REQUIRE((std::is_same<decltype(op), ieompp::Operator<float, bool>>::value));
        REQUIRE((std::is_same<typename ieompp::index_type<0, decltype(op)>::type, float>::value));
        REQUIRE((std::is_same<typename ieompp::index_type<1, decltype(op)>::type, bool>::value));
        REQUIRE(op.creator);
        REQUIRE(op.index1 == 1.f);
        REQUIRE(op.index2 == true);
    }

    SECTION("three indices")
    {
        auto op = ieompp::make_creator(1.f, true, 'c');
        REQUIRE((std::is_same<decltype(op), ieompp::Operator<float, bool, char>>::value));
        REQUIRE((std::is_same<typename ieompp::index_type<0, decltype(op)>::type, float>::value));
        REQUIRE((std::is_same<typename ieompp::index_type<1, decltype(op)>::type, bool>::value));
        REQUIRE((std::is_same<typename ieompp::index_type<2, decltype(op)>::type, char>::value));
        REQUIRE(op.creator);
        REQUIRE(ieompp::get_index<0>(op) == 1.f);
        REQUIRE(ieompp::get_index<1>(op) == true);
        REQUIRE(ieompp::get_index<2>(op) == 'c');
    }
}

TEST_CASE("make annihilator", "[operator]")
{
    SECTION("one index")
    {
        auto op = ieompp::make_annihilator(42);
        REQUIRE((std::is_same<decltype(op), ieompp::Operator<int>>::value));
        REQUIRE((std::is_same<typename ieompp::index_type<0, decltype(op)>::type, int>::value));
        REQUIRE(!op.creator);
        REQUIRE(op.index == 42);
    }

    SECTION("two indices")
    {
        auto op = ieompp::make_annihilator(1.f, true);
        REQUIRE((std::is_same<decltype(op), ieompp::Operator<float, bool>>::value));
        REQUIRE((std::is_same<typename ieompp::index_type<0, decltype(op)>::type, float>::value));
        REQUIRE((std::is_same<typename ieompp::index_type<1, decltype(op)>::type, bool>::value));
        REQUIRE(!op.creator);
        REQUIRE(op.index1 == 1.f);
        REQUIRE(op.index2 == true);
    }

    SECTION("three indices")
    {
        auto op = ieompp::make_annihilator(1.f, true, 'c');
        REQUIRE((std::is_same<decltype(op), ieompp::Operator<float, bool, char>>::value));
        REQUIRE((std::is_same<typename ieompp::index_type<0, decltype(op)>::type, float>::value));
        REQUIRE((std::is_same<typename ieompp::index_type<1, decltype(op)>::type, bool>::value));
        REQUIRE((std::is_same<typename ieompp::index_type<2, decltype(op)>::type, char>::value));
        REQUIRE(!op.creator);
        REQUIRE(ieompp::get_index<0>(op) == 1.f);
        REQUIRE(ieompp::get_index<1>(op) == true);
        REQUIRE(ieompp::get_index<2>(op) == 'c');
    }
}
